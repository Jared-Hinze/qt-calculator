import QtQuick.Controls
import Calculator

ApplicationWindow {
    id: app
    width: Constants.width
    height: Constants.height

    visible: true
    title: "TestApp"

    Main {
        id: main
        anchors.fill: parent
    }
}

