import Calculator

MainUI {
	anchors.fill: parent

	display.text: backend ? backend.display : ""
}
