import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Calculator

Rectangle {
    property alias btnpad: buttons
    property alias display: display

    width: Constants.width
    height: Constants.height
    color: Constants.backgroundColor

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            Layout.alignment: Qt.AlignTop

            Rectangle {
                Layout.fillWidth: true

                height: 50
                color: "#10710C"
                radius: 5

                Text {
                    id: display
                    height: parent.height
                    width: parent.width
                    padding: 5
                    font: Constants.largeFont
                    horizontalAlignment: Qt.AlignRight
                    verticalAlignment: Qt.AlignVCenter
                    color: "white"
                    text: "0"
                }
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            GridLayout {
                columns: 4

                Repeater {
                    id: buttons
                    model: [
                        "%", "CE", "C", "DEL",
                        "1/x", "x²", "√x", "÷",
                        "7", "8", "9", "×",
                        "4", "5", "6", "-",
                        "1", "2", "3", "+",
                        "+/-", "0", ".", "="
                    ]
                    delegate: CalcButton {
                        model: buttons.model
                        idx: index
                    }
                }
            }
        }
    }
}
