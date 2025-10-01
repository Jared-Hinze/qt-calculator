pragma Singleton
import QtQuick
import QtQuick.Studio.Application

QtObject {
	readonly property int width: 400
	readonly property int height: 500

	property string relativeFontDirectory: "fonts"

	readonly property font font: Qt.font({
		family: "Courier New, Courier, monospace",
		pixelSize: 24
	})
	readonly property font largeFont: Qt.font({
		family: "Courier New, Courier, monospace",
		pixelSize: 40
	})

	readonly property color backgroundColor: "#444444"


	property StudioApplication application: StudioApplication {
		fontPath: Qt.resolvedUrl("../TestAppContent/" + relativeFontDirectory)
	}
}
