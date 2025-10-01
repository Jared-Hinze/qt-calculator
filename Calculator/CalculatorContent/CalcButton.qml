import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Calculator

Button {
    id: root
    enabled: backend ? (!backend.locked || text === "C") : false

    // Signal Handlers (Slots)
    onClicked: backend.updateDisplay(text)
    onModelChanged: calculateColors()
    Component.onCompleted: calculateColors()

    // Data from the Repeater
    property var model
    property int idx

    // Component properties
    property color defaultColor
    property color hoveredColor
    property color pressedColor

    Layout.fillHeight: true
    Layout.fillWidth: true

    text: model[idx]
    font: Constants.font

    function calculateColors() {
        if (!root.model) return;

        var index = root.idx;
        var len = root.model.length;

        if (index === len - 1) {
            defaultColor = "#AAAAAA";
            hoveredColor = "#999999";
            pressedColor = "#888888";
        } else if (index < 8 || index % 4 === 3) {
            defaultColor = "#555555";
            hoveredColor = "#777777";
            pressedColor = "#666666";
        } else {
            defaultColor = "#777777";
            hoveredColor = "#555555";
            pressedColor = "#444444";
        }
    }

    background: Rectangle {
        id: bg
        anchors.fill: parent
        radius: 5

        color: root.defaultColor

        states: [
            State {
                name: "pressed"
                when: root.pressed
                PropertyChanges { target: bg; color: root.pressedColor }
            },
            State {
                name: "hovered"
                when: root.hovered && !root.pressed
                PropertyChanges { target: bg; color: root.hoveredColor }
            }
        ]

        transitions: Transition {
            ColorAnimation {
                duration: 100
            }
        }
    }
}
