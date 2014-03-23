import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
    id: pointPopUp
    visible: false
    property real   pointX
    property real   pointY
    property date   date
    property real   weight
    property string comment
    property Item   limitZone

    x: {
        if (pointX + width <= limitZone.width)
            return pointX
        return limitZone.width - width
    }
    y: pointY - height - 10
    z: Number.MAX_VALUE // If z isn't forced this tips isn't draw on top of curves (curious cause it's draw after)
    width: column.implicitWidth + 10 * 2
    height: column.implicitHeight + 10 * 2
    color: "#FFFFFF"
    border.color: "#000000"
    border.width: 1
    radius: 5

    Column {
        id: column
        anchors.centerIn: parent
        width: grid.implicitWidth > commentText.contentWidth ? grid.implicitWidth : commentText.contentWidth

        Grid {
            id: grid
            columns: 2
            columnSpacing: 8
            rowSpacing: 3

            Text {
                id: dateLabel
                text: qsTr("Date:")
                font.pixelSize: 12
            }
            Text {
                id: dateText
                text: date.toLocaleDateString()
                font.pixelSize: 12
            }

            Text {
                id: weightLabel
                text: qsTr("Weight:")
                font.pixelSize: 12
            }
            Text {
                id: weightText
                text: weight.toFixed(2)
                font.pixelSize: 12
            }

            Text {
                id: commentLabel
                text: qsTr("Comment:")
                font.pixelSize: 12
            }
        }

        Text {
            id: commentText
            text: comment
            width: 200
            wrapMode: Text.WordWrap
            font.pixelSize: 12
        }
    }
}
