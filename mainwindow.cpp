#include <QLineSeries>
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

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->mainLayout->replaceWidget(ui->chartWidget, chartView);

    ui->eg->setValidator(new DoubleValidator(-20.0, 20.0, 2, ui->eg));
    ui->ed->setValidator(new DoubleValidator(-20.0, 20.0, 2, ui->ed));
    ui->ea->setValidator(new DoubleValidator(-20.0, 20.0, 2, ui->ea));

    DoubleValidator *validator;
    validator = new DoubleValidator(1e-30, 1, 2, ui->me);
    validator->setNotation(DoubleValidator::ScientificNotation);
    ui->me->setValidator(validator);

    validator = new DoubleValidator(1e-30, 1, 2, ui->mh);
    validator->setNotation(DoubleValidator::ScientificNotation);
    ui->mh->setValidator(validator);

    validator = new DoubleValidator(0.0, 1e30, 2, ui->nc);
    validator->setNotation(DoubleValidator::ScientificNotation);
    ui->nc->setValidator(validator);

    validator = new DoubleValidator(0.0, 1e30, 2, ui->nv);
    validator->setNotation(DoubleValidator::ScientificNotation);
    ui->nv->setValidator(validator);

    validator = new DoubleValidator(0.0, 1e30, 2, ui->nd0);
    validator->setNotation(DoubleValidator::ScientificNotation);
    ui->nd0->setValidator(validator);

    validator = new DoubleValidator(0.0, 1e30, 2, ui->na0);
    validator->setNotation(DoubleValidator::ScientificNotation);
    ui->na0->setValidator(validator);

    ui->tempFrom->setValidator(new DoubleValidator(0.0, 1e4, 1, ui->tempFrom));
    ui->tempTo->setValidator(new DoubleValidator(0.0, 1e4, 1, ui->tempTo));
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

    updateChart();
}

void MainWindow::updateChart()
{
    QLineSeries *series = new QLineSeries(this);
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    QStringList colors = QColor::colorNames();
    series->setColor(colors[static_cast<int>(chartState) % colors.size()]);
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");
    chartView->setChart(chart);
}

// load
void MainWindow::on_action_ASCII_2_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Загрузить из ASCII"), "./", tr("Text files (*.txt)"));

    std::ifstream is(filename.toStdString());

    if (Controller::GetInstance().LoadFromASCII(is)) {
        updateChart();
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
    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить в ASCII"), "./", tr("Text files (*.txt)"));

    std::ifstream is(filename.toStdString());
    if (!Controller::GetInstance().LoadMobility(is)) {
        QMessageBox msgBox;
        msgBox.setText("Не удалось загрузить подвижность :(");
        msgBox.exec();
    }
}

void MainWindow::on_action_3_triggered()
{
    chartState = ChartState::ElectronsConcentration;
    updateChart();
}

void MainWindow::on_action_4_triggered()
{
    chartState = ChartState::HolesConcentration;
    updateChart();
}

void MainWindow::on_action_5_triggered()
{
    chartState = ChartState::DonorsConcentration;
    updateChart();
}

void MainWindow::on_action_6_triggered()
{
    chartState = ChartState::AcceptorsConcentration;
    updateChart();
}

void MainWindow::on_action_7_triggered()
{
    chartState = ChartState::ElectronsMobility;
    updateChart();
}

void MainWindow::on_action_8_triggered()
{
    chartState = ChartState::HolesMobility;
    updateChart();
}

void MainWindow::on_action_9_triggered()
{
    chartState = ChartState::Conductivity;
    updateChart();
}
