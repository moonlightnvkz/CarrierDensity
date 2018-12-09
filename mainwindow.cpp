#include <QLineSeries>
#include <QColor>
#include <QChart>
#include <QChartView>
#include <QSizePolicy>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <memory>
#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "doublevalidator.h"
#include "model.h"
#include "controller.h"

#define READ_AND_VALIDATE(control) control->validator()->locale().toDouble(control->text())

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QChart *chart = new QChart();
    chart->legend()->hide();
    chartView->setChart(chart);
    ui->mainLayout->replaceWidget(ui->chartWidget, chartView);

//    ui->eg->setValidator(new DoubleValidator(-20.0, 20.0, 2, ui->eg));
    ui->ed->setValidator(new DoubleValidator(-20.0, 20.0, 2, ui->ed));
    ui->ea->setValidator(new DoubleValidator(-20.0, 20.0, 2, ui->ea));

    DoubleValidator *validator;
//    validator = new DoubleValidator(1e-30, 1, 2, ui->me);
//    validator->setNotation(DoubleValidator::ScientificNotation);
//    ui->me->setValidator(validator);

//    validator = new DoubleValidator(1e-30, 1, 2, ui->mh);
//    validator->setNotation(DoubleValidator::ScientificNotation);
//    ui->mh->setValidator(validator);

//    validator = new DoubleValidator(0.0, 1e30, 2, ui->nc);
//    validator->setNotation(DoubleValidator::ScientificNotation);
//    ui->nc->setValidator(validator);

//    validator = new DoubleValidator(0.0, 1e30, 2, ui->nv);
//    validator->setNotation(DoubleValidator::ScientificNotation);
//    ui->nv->setValidator(validator);

    validator = new DoubleValidator(0.0, 1e30, 2, ui->nd0);
    validator->setNotation(DoubleValidator::ScientificNotation);
    ui->nd0->setValidator(validator);

    validator = new DoubleValidator(0.0, 1e30, 2, ui->na0);
    validator->setNotation(DoubleValidator::ScientificNotation);
    ui->na0->setValidator(validator);

    ui->tempFrom->setValidator(new DoubleValidator(0.0, 1e4, 1, ui->tempFrom));
    ui->tempTo->setValidator(new DoubleValidator(0.0, 1e4, 1, ui->tempTo));
    on_material_currentIndexChanged(ui->material->currentIndex());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tempFrom_editingFinished()
{
    validateTemperatureRange();
}

void MainWindow::on_tempTo_editingFinished()
{
    validateTemperatureRange();
}

void MainWindow::validateTemperatureRange()
{
    double from = ui->tempFrom->validator()->locale().toDouble(ui->tempFrom->text());
    double to= ui->tempTo->validator()->locale().toDouble(ui->tempTo->text());
    if (from > to) {
        ui->tempFrom->setText(ui->tempTo->text());
        ui->tempTo->setText(ui->tempFrom->text());
    }

    Controller::GetInstance().UpdateT(from, to);
    recalculateData();
}

void MainWindow::updateChart()
{
    assert(chartView && chartView->chart() && chartView->chart()->series().size() == 7);
    qDebug() << static_cast<int>(chartState);

    QChart * chart = chartView->chart();
    auto list = chart->series();
    for (auto *series : list) {
        series->hide();
    }

    QLineSeries *visible = static_cast<QLineSeries*>(list[static_cast<int>(chartState)]);
    visible->show();

    double min = std::numeric_limits<double>::max();
    double max = -std::numeric_limits<double>::max();
    for (QPointF point : visible->points()) {
        if (point.y() > max) max = point.y();
        if (point.y() < min) min = point.y();
    }
    chart->axisY()->setRange(min, max);
}

void MainWindow::recalculateData()
{
    Controller::GetInstance().Recalculate();
    assert(chartView && chartView->chart());
    const std::vector<double> &temperature = Controller::GetInstance().GetTemperature();

    // !NB: order must be the same as in the enum
    const std::vector<double> *data[] = { &Controller::GetInstance().GetElectronsConcentration(),
                                          &Controller::GetInstance().GetHolesConcentration(),
                                          &Controller::GetInstance().GetDonorsConcentration(),
                                          &Controller::GetInstance().GetAcceptorsConcentration(),
                                          &Controller::GetInstance().GetElectronsMobility(),
                                          &Controller::GetInstance().GetHolesMobility(),
                                          &Controller::GetInstance().GetConductivity() };
    QChart * chart = chartView->chart();
    chart->removeAllSeries();
    for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); ++i) {
        const auto *datum = data[i];
        assert(datum->size() == temperature.size());
        QLineSeries *series = new QLineSeries(chart);
        for (size_t i = 0; i < temperature.size(); ++i) {
            series->append(temperature[i], datum->at(i));
        }
        series->setColor(QColor(0, 0, 255));
//        series->hide();
        chart->addSeries(series);
    }

    chart->createDefaultAxes();

    updateChart();
}

// load
void MainWindow::on_action_ASCII_2_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Загрузить из ASCII"), "./", tr("Text files (*.txt)"));

    std::ifstream is(filename.toStdString());

    if (Controller::GetInstance().LoadFromASCII(is)) {
        recalculateData();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Не удалось загрузить ASII файл :(");
        msgBox.exec();
    }
}

// save
void MainWindow::on_action_ASCII_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить в ASCII"), "./", tr("Text files (*.txt)"));

    std::ofstream os(filename.toStdString());
    if (!Controller::GetInstance().SaveToASCII(os)) {
        QMessageBox msgBox;
        msgBox.setText("Не удалось сохранить ASII файл :(");
        msgBox.exec();
    }
}

// load mobility
void MainWindow::on_action_2_triggered()
{
    if (Controller::GetInstance().LoadMobility()) {
        recalculateData();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Не удалось загрузить подвижность :(");
        msgBox.exec();
    }
}

void MainWindow::on_action_3_triggered()
{
    chartState = ChartState::ElectronsConcentration;
    recalculateData();
}

void MainWindow::on_action_4_triggered()
{
    chartState = ChartState::HolesConcentration;
    recalculateData();
}

void MainWindow::on_action_5_triggered()
{
    chartState = ChartState::DonorsConcentration;
    recalculateData();
}

void MainWindow::on_action_6_triggered()
{
    chartState = ChartState::AcceptorsConcentration;
    recalculateData();
}

void MainWindow::on_action_7_triggered()
{
    chartState = ChartState::ElectronsMobility;
    recalculateData();
}

void MainWindow::on_action_8_triggered()
{
    chartState = ChartState::HolesMobility;
    recalculateData();
}

void MainWindow::on_action_9_triggered()
{
    chartState = ChartState::Conductivity;
    recalculateData();
}

void MainWindow::on_material_currentIndexChanged(int index)
{
    Controller::GetInstance().UpdateMaterial(index);
    ui->ed->setText(QString::number(Controller::GetInstance().GetEd()));
    ui->ea->setText(QString::number(Controller::GetInstance().GetEa()));
    ui->nd0->setText(QString::number(Controller::GetInstance().GetNd0()));
    ui->na0->setText(QString::number(Controller::GetInstance().GetNa0()));
    const auto &temp = Controller::GetInstance().GetTemperature();
    ui->tempFrom->setText(QString::number(temp[0]));
    ui->tempTo->setText(QString::number(temp.back()));
    recalculateData();
}

void MainWindow::on_ed_editingFinished()
{
    double value = READ_AND_VALIDATE(ui->ed);
    qDebug() << "DDDDDDDDDDDDDDDDDDDDDDDDDDDDD" << value;
    Controller::GetInstance().UpdateEd(value);
    recalculateData();
}

void MainWindow::on_nd0_editingFinished()
{
    double value = READ_AND_VALIDATE(ui->nd0);
    Controller::GetInstance().UpdateNd0(value);
    recalculateData();
}

void MainWindow::on_ea_editingFinished()
{
    double value = READ_AND_VALIDATE(ui->ea);
    qDebug() << "AAAAAAAAAAAAAAAAAAAAAAAAAAAA" << value;
    Controller::GetInstance().UpdateEa(value);
    recalculateData();
}

void MainWindow::on_na0_editingFinished()
{
    double value = READ_AND_VALIDATE(ui->na0);
    Controller::GetInstance().UpdateNa0(value);
    recalculateData();
}
