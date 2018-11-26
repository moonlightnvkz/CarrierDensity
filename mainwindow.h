#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tempFrom_editingFinished();

    void on_tempTo_editingFinished();

    void on_action_ASCII_2_triggered();

    void on_action_ASCII_triggered();

    void on_action_2_triggered();

    void on_action_6_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_action_5_triggered();

    void on_action_7_triggered();

    void on_action_8_triggered();

    void on_action_9_triggered();

private:
    enum class ChartState {
        ElectronsConcentration = 0,
        HolesConcentration,
        DonorsConcentration,
        AcceptorsConcentration,
        ElectronsMobility,
        HolesMobility,
        Conductivity,
    };

    Ui::MainWindow *ui;

    QtCharts::QChartView *chartView;

    ChartState chartState = ChartState::Conductivity;

    void validateTemperatureRange();

    void updateChart();

    void recalculateData();
};

#endif // MAINWINDOW_H
