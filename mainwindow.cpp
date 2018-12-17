#include <QLineSeries>
#include <QColor>
#include <QChart>
#include <QChartView>
#include <QSizePolicy>
#include <QFileDialog>
#include <QMessageBox>
#include <QLogValueAxis>
#include <QValueAxis>
#include <QWidgetAction>
#include <fstream>
#include <memory>
#include <string>
#include <cmath>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_about.h"
#include "doublevalidator.h"
#include "model.h"
#include "controller.h"

#define READ_AND_VALIDATE(control) control->validator()->locale().toDouble(control->text())

#define SET_TEXT_LOCALE(elem, value) ui-> elem ->setText(ui-> elem ->validator()->locale().toString(value))

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Удельная проводимость");

    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->setLocalizeNumbers(true);
    chartView->setChart(chart);
    ui->mainLayout->replaceWidget(ui->chartWidget, chartView);
    ui->chartWidget->deleteLater();

    ui->eg->setValidator(new DoubleValidator(0.0, 20.0, 9, ui->eg));
    ui->ed->setValidator(new DoubleValidator(0.0, 20.0, 9, ui->ed));
    ui->ea->setValidator(new DoubleValidator(0.0, 20.0, 9, ui->ea));

    DoubleValidator *validator;
    validator = new DoubleValidator(0, 1e1, 9, ui->me);
    validator->setNotation(DoubleValidator::ScientificNotation);
    ui->me->setValidator(validator);

    validator = new DoubleValidator(0, 1e1, 9, ui->mh);
    validator->setNotation(DoubleValidator::ScientificNotation);
    ui->mh->setValidator(validator);

    validator = new DoubleValidator(0.0, 1e30, 9, ui->nd0);
    validator->setNotation(DoubleValidator::ScientificNotation);
    ui->nd0->setValidator(validator);

    validator = new DoubleValidator(0.0, 1e30, 9, ui->na0);
    validator->setNotation(DoubleValidator::ScientificNotation);
    ui->na0->setValidator(validator);

    ui->tempFrom->setValidator(new DoubleValidator(0.0, 1e4, 9, ui->tempFrom));
    ui->tempTo->setValidator(new DoubleValidator(0.0, 1e4, 9, ui->tempTo));

    QActionGroup *chartsGroup = new QActionGroup(this);
    chartsGroup->addAction(ui->action_3);
    chartsGroup->addAction(ui->action_4);
    chartsGroup->addAction(ui->action_5);
    chartsGroup->addAction(ui->action_6);
    chartsGroup->addAction(ui->action_7);
    chartsGroup->addAction(ui->action_8);
    chartsGroup->addAction(ui->action_9);

    SET_TEXT_LOCALE(ed, Controller::GetInstance().GetEd());
    SET_TEXT_LOCALE(ea, Controller::GetInstance().GetEa());
    SET_TEXT_LOCALE(nd0, Controller::GetInstance().GetNd0());
    SET_TEXT_LOCALE(na0, Controller::GetInstance().GetNa0());
    const auto &temp = Controller::GetInstance().GetTemperature();
    SET_TEXT_LOCALE(tempFrom, temp[0]);
    SET_TEXT_LOCALE(tempTo, temp.back());

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
    updateData();
}

void MainWindow::updateChart()
{
    assert(chartView && chartView->chart() && chartView->chart()->series().size() == 7);

    QChart * chart = chartView->chart();
    auto list = chart->series();
    for (auto *series : list) {
        series->hide();
    }

    QLineSeries *visible = static_cast<QLineSeries*>(list[static_cast<int>(chartState)]);
    visible->show();

    double x_min = std::numeric_limits<double>::max();
    double x_max = -std::numeric_limits<double>::max();
    double y_min = std::numeric_limits<double>::max();
    double y_max = -std::numeric_limits<double>::max();
    for (QPointF point : visible->points()) {
        double x = point.x();
        double y = point.y();

        if (x > x_max) x_max = x;
        if (x < x_min) x_min = x;
        if (y > y_max) y_max = y;
        if (y < y_min) y_min = y;
    }

    QValueAxis *x = new QValueAxis;
    x->setRange(x_min, x_max);
    x->setMinorTickCount(-1);
    x->setTitleText("Температура, К");

    QAbstractAxis *y;
    if (logarithmic) {
        QLogValueAxis *y_log = new QLogValueAxis;

        y_log->setBase(10);
        y_log->setLabelFormat("%.1g");
//        y_log->setMinorTickCount(-1);

        y_log->setRange(y_min, y_max);

        y = y_log;
        y->setGridLineVisible(false);
    } else {
        QValueAxis *y_value = new QValueAxis;

        y_value->setLabelFormat("%.1g");
        y_value->applyNiceNumbers();

        y = y_value;
    }

    y->setTitleText(visible->name());

    if (chart->axisX()) chart->removeAxis(chart->axisX());
    if (chart->axisY()) chart->removeAxis(chart->axisY());

    chart->addAxis(x, Qt::AlignBottom);
    chart->addAxis(y, Qt::AlignLeft);

    visible->attachAxis(chart->axisX());
    visible->attachAxis(chart->axisY());
}

void MainWindow::updateData()
{
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

    constexpr const char *titles[] = { "Концентрация электронов, см-3",
                                       "Концентрация дырок, см-3",
                                       "Концентрация заряженных доноров, см-3",
                                       "Концентрация заряженных акцепторов, см-3",
                                       "Подвижность электронов, см2/(В*с)",
                                       "Подвижность дырок, см2/(В*с)",
                                       "Удельная проводимость, 1/(Ом*м)" };

    QChart * chart = chartView->chart();
    chart->removeAllSeries();
    for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); ++i) {
        const auto *datum = data[i];
        assert(datum->size() == temperature.size());
        QLineSeries *series = new QLineSeries(chart);
        for (size_t i = 0; i < temperature.size(); ++i) {
            double v = datum->at(i);
//            if (v < 1e-20) v = 1e-20;
            series->append(temperature[i], v);
        }
        series->setColor(QColor(0, 0, 255));
        series->setName(titles[i]);
        chart->addSeries(series);
    }
    ui->material->setCurrentIndex(Controller::GetInstance().GetMaterial());
    SET_TEXT_LOCALE(ed, Controller::GetInstance().GetEd());
    SET_TEXT_LOCALE(ea, Controller::GetInstance().GetEa());
    SET_TEXT_LOCALE(nd0, Controller::GetInstance().GetNd0());
    SET_TEXT_LOCALE(na0, Controller::GetInstance().GetNa0());
    SET_TEXT_LOCALE(tempFrom, temperature[0]);
    SET_TEXT_LOCALE(tempTo, temperature.back());
    SET_TEXT_LOCALE(eg, Controller::GetInstance().GetEg());
    SET_TEXT_LOCALE(me, Controller::GetInstance().GetMe());
    SET_TEXT_LOCALE(mh, Controller::GetInstance().GetMh());

    updateChart();
}

// load
void MainWindow::on_action_ASCII_2_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Загрузить из ASCII"), "./", tr("Text files (*.txt)"));
    if (filename.size() == 0) {
        return;
    }
    std::ifstream is(filename.toStdString());

    if (Controller::GetInstance().LoadFromASCII(is)) {
        updateData();
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
    if (filename.size() == 0) {
        return;
    }
    std::ofstream os(filename.toStdString());
    if (!Controller::GetInstance().SaveToASCII(os)) {
        QMessageBox msgBox;
        msgBox.setText("Не удалось сохранить ASII файл :(");
        msgBox.exec();
    }
}

void MainWindow::on_action_3_triggered()
{
    chartState = ChartState::ElectronsConcentration;
    updateData();
}

void MainWindow::on_action_4_triggered()
{
    chartState = ChartState::HolesConcentration;
    updateData();
}

void MainWindow::on_action_5_triggered()
{
    chartState = ChartState::DonorsConcentration;
    updateData();
}

void MainWindow::on_action_6_triggered()
{
    chartState = ChartState::AcceptorsConcentration;
    updateData();
}

void MainWindow::on_action_7_triggered()
{
    chartState = ChartState::ElectronsMobility;
    updateData();
}

void MainWindow::on_action_8_triggered()
{
    chartState = ChartState::HolesMobility;
    updateData();
}

void MainWindow::on_action_9_triggered()
{
    chartState = ChartState::Conductivity;
    updateData();
}

void MainWindow::on_material_currentIndexChanged(int index)
{
    Controller::GetInstance().UpdateMaterial(index);

    if (index == static_cast<int>(Model::Preset::Custom)) {
        ui->eg->setEnabled(true);
        ui->me->setEnabled(true);
        ui->mh->setEnabled(true);
    } else {
        SET_TEXT_LOCALE(eg, Controller::GetInstance().GetEg());
        SET_TEXT_LOCALE(me, Controller::GetInstance().GetMe());
        SET_TEXT_LOCALE(mh, Controller::GetInstance().GetMh());
        ui->eg->setEnabled(false);
        ui->me->setEnabled(false);
        ui->mh->setEnabled(false);
    }

    updateData();
}

void MainWindow::on_ed_editingFinished()
{
    double value = READ_AND_VALIDATE(ui->ed);
    Controller::GetInstance().UpdateEd(value);
    updateData();
}

void MainWindow::on_nd0_editingFinished()
{
    double value = READ_AND_VALIDATE(ui->nd0);
    Controller::GetInstance().UpdateNd0(value);
    updateData();
}

void MainWindow::on_ea_editingFinished()
{
    double value = READ_AND_VALIDATE(ui->ea);
    Controller::GetInstance().UpdateEa(value);
    updateData();
}

void MainWindow::on_na0_editingFinished()
{
    double value = READ_AND_VALIDATE(ui->na0);
    Controller::GetInstance().UpdateNa0(value);
    updateData();
}

void MainWindow::on_eg_editingFinished()
{
    double value = READ_AND_VALIDATE(ui->eg);
    Controller::GetInstance().UpdateEg(value);
    updateData();
}

void MainWindow::on_me_editingFinished()
{
    double value = READ_AND_VALIDATE(ui->me);
    Controller::GetInstance().UpdateMe(value);
    updateData();
}

void MainWindow::on_mh_editingFinished()
{
    double value = READ_AND_VALIDATE(ui->mh);
    Controller::GetInstance().UpdateMh(value);
    updateData();
}

void MainWindow::on_about_triggered()
{
    QDialog *about = new QDialog(this);
    Ui::About about_ui;
    about_ui.setupUi(about);

    about->setWindowTitle("О программе");

    about->show();
}

void MainWindow::on_action_log_triggered(bool checked)
{
    logarithmic = checked;
    updateChart();
}
