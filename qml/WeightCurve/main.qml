import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

ApplicationWindow {
    id: window
    title: qsTr("Weight Curve")
    width: minimumWidth
    height: minimumHeight
    minimumWidth: 800
    minimumHeight: 480

    FileDialog {
        id: importFileDialog
        nameFilters: [ "CVS files (*.csv)" ]
        folder: "E:\Personnal\Flamaros"
        onAccepted: application.importFromCSVFile(fileUrl)
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Import CSV")
                onTriggered: importFileDialog.open();
            }
            MenuItem {
                text: qsTr("Export CSV")
                enabled: false
                onTriggered: {} // TODO implement it
            }
            MenuSeparator {}
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    GroupBox {
        id: weightGroup
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.right: graphicOptionsGroup.left
        anchors.rightMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8
        title: qsTr("Weight")
        clip: true

        Grid
        {
            id: weightGroupGrid
            anchors.fill: parent
            columns: 2
            rows: 2
            spacing: 8

            Row {
                id: dayRow
                spacing: 8

                Label {
                    id: dayLabel
                    text: qsTr("Date:")
                }

                Label {
                    id: date
                    text: Qt.formatDate(application.inputDate, "dd/MM/yyyy") // TODO binder une property c++ qui gere la date pour les inputs
                    color: dayCalendar.visible ? "red" : linkColor
                    font.underline: true

                    MouseArea {
                        anchors.fill: parent
                        onClicked: dayCalendar.visible = !dayCalendar.visible
                    }
                }
            }

            Label {
                text: qsTr("Comment:")
            }

            Grid {
                id: weightGrid
                spacing: 8
                columns: 2

                Label {
                    id: morningLabel
                    text: qsTr("Morning")
                    font.pixelSize: 12
                }

                TextField {
                    id: morningWeight
                    placeholderText: qsTr("Enter your weight")
                    font.pixelSize: 12
                    text: !isNaN(application.morningWeight) ? application.morningWeight.toFixed(2) : "";
                    onAccepted: {
                        application.morningWeight = parseFloat(text);
                    }
                }

                Label {
                    id: noonLabel
                    color: "#462406"
                    text: qsTr("Noon")
                    font.pixelSize: 12
                }

                TextField {
                    id: noonWeight
                    placeholderText: qsTr("Enter your weight")
                    font.pixelSize: 12
                    text: !isNaN(application.noonWeight) ? application.noonWeight.toFixed(2) : "";
                    onAccepted: {
                        application.noonWeight = parseFloat(text);
                    }
                }

                Label {
                    id: eveningLabel
                    text: qsTr("Evening")
                    font.pixelSize: 12
                }

                TextField {
                    id: eveningWeight
                    placeholderText: qsTr("Enter your weight")
                    font.pixelSize: 12
                    text: !isNaN(application.eveningWeight) ? application.eveningWeight.toFixed(2) : "";
                    onAccepted: {
                        application.eveningWeight = parseFloat(text);
                    }
                }
            }

            TextArea {
                id: dayComment
                clip: true
                height: weightGrid.height
                width: weightGroupGrid.width - weightGroupGrid.spacing - weightGrid.width
                text: application.comment
                onFocusChanged: {
                    console.log(focus)
                    if (!focus)
                        application.comment = text
                }
/*                onTextChanged: {  // Some characters aren't correctly handle and it can be really slow cause of update time of curves
                    application.comment = text
                }*/
            }
        }
    }

    GroupBox {
        id: graphicOptionsGroup
        anchors.top: weightGroup.top
        anchors.bottom: weightGroup.bottom
        anchors.right: parent.right
        anchors.rightMargin: 8
        width: 150
        title: "Graphic Options"

        ColumnLayout {
            id: graphicOptionsGroupGrid
            anchors.fill: parent
            spacing: 8

            ExclusiveGroup {
                id: durationGroup
                current: quarterRange
            }
            RadioButton {
                exclusiveGroup: durationGroup
                text: qsTr("30 days")
                onCheckedChanged: {
                    if (checked) {
                        var date = new Date()
                        date.setDate(date.getDate() -  30)
                        application.startDate = date
                        application.endDate = new Date()
                    }
                }
            }
            RadioButton {
                id: quarterRange
                exclusiveGroup: durationGroup
                text: qsTr("3 months")
                onCheckedChanged: {
                    if (checked) {
                        var date = new Date()
                        date.setDate(date.getDate() -  365 / 4)
                        application.startDate = date
                        application.endDate = new Date()
                    }
                }
            }
            RadioButton {
                exclusiveGroup: durationGroup
                text: qsTr("6 months")
                onCheckedChanged: {
                    if (checked) {
                        var date = new Date()
                        date.setDate(date.getDate() -  365 / 2)
                        application.startDate = date
                        application.endDate = new Date()
                    }
                }
            }
            RadioButton {
                exclusiveGroup: durationGroup
                text: qsTr("1 year")
                onCheckedChanged: {
                    if (checked) {
                        var date = new Date()
                        date.setDate(date.getDate() -  365)
                        application.startDate = date
                        application.endDate = new Date()
                    }
                }
            }
        }
    }

    Chart {
        id: chart
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.top: weightGroup.bottom
        anchors.topMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8

        startDate: application.startDate
        endDate: application.endDate
        curves: [application.morningCurve,
            application.noonCurve,
            application.eveningCurve]
        loosingFocusItem: dayComment
    }

    Item {
        id: dayCalendar
        anchors.fill: parent
        visible: false

        Rectangle {
            anchors.fill: parent
            opacity: 0.9
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#060606" }
                GradientStop { position: 1.0; color: "#000000" }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: dayCalendar.visible = false
            hoverEnabled: true
        }

        Calendar {
            anchors.centerIn: parent

            onSelectedDateChanged: {
                application.inputDate = selectedDate;
            }
        }
    }
}
