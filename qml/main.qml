import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Window 2.11
import QtCharts 2.0

ChartView {
    id: root

    width: 640
    height: 480

    theme: ChartView.ChartThemeBlueIcy
    title: "line"
    anchors.fill: parent
    antialiasing: true

    LineSeries {
        id: series

        name: "LineSeries"

        XYPoint { x: 0; y: 0 }
        XYPoint { x: 1.1; y: 2.1 }
        XYPoint { x: 1.9; y: 3.3 }
        XYPoint { x: 2.1; y: 2.1 }
        XYPoint { x: 2.9; y: 4.9 }
        XYPoint { x: 3.4; y: 3.0 }
        XYPoint { x: 4.1; y: 3.3 }

    }
}
