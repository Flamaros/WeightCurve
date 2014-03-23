import QtQuick 2.2
import "Utilities.js" as Utilities

Rectangle {
    id: chart

    width: 800
    height: 400

    // TODO add legendary on left of chart for all curves with colors corresponding (curve names in color of curves)
    property string unitLabel: "kilogrammes"
    property var    curves
    property date   startDate
    property date   endDate
    property var    loosingFocusItem    // can help to remove focus from comment TextArea (for a proper update of comments in popups)
    property int    daysRepartition: {  // number of days between two verticale lines
        var range = Utilities.dateDiffInDays(endDate, startDate)
        if (range > 365)
            return 365 / 12
        else if (range > 365 / 2)
            return 365 / 12
        else if (range > 365 / 4)
            return 15
        else if (range > 31)
            return 7
        return 1
    }

/*    onCurvesChanged: {
        if (curves.length)
        {
            console.log("Chart.curves : " + curves + "curves count : " + curves.length)
            for (var i = 0; i < curves.length; i++)
            {
                console.log("Chart.curves.name : " + curves[i].name)
                console.log("Chart.curves.points.length : " + curves[i].points.length)
                console.log("\n")
            }
        }
    }*/

    property int    __legendMargin: 10
    property real   __chartTop: __legendMargin
    property real   __chartBottom: chart.height - xAxisLabels.height - 2 * __legendMargin
    property real   __chartLeft: yAxisLabels.x + yAxisLabels.width + __legendMargin
    property real   __chartRight: chart.width - __legendMargin
    property real   __chartWidth: __chartRight - __chartLeft
    property real   __chartHeight: __chartBottom - __chartTop
    property real   __chartRoundedMin: Math.floor(__chartMin - 0.5)   // Used for expand a little the chart amplitude
    property real   __chartRoundedMax: Math.ceil(__chartMax + 0.5)   // Used for expand a little the chart amplitude
    property real   __chartMin: {
        if (curves.length)
        {
            var min = curves[0].min
            for (var i = 1; i < curves.length; i++)
            {
                if (curves[i].min < min)
                    min = curves[i].min
            }
            return min
        }
        return 0
    }
    property real   __chartMax: {
        if (curves.length)
        {
            var max = curves[0].max
            for (var i = 1; i < curves.length; i++)
            {
                if (curves[i].max > max)
                    max = curves[i].max
            }
            return max
        }
        return 0
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onContainsMouseChanged: {
            if (containsMouse)
                loosingFocusItem.focus = false
        }
    }

    // left yAxis legend
    AABB {
        id: unitLabelText
        x: __legendMargin
        y: parent.height / 2 - height / 2

        Text {
            text: unitLabel
            font.bold: true
            font.pointSize: 10

            anchors.centerIn: parent
            rotation: -90
            transformOrigin: Item.Center
        }
    }

    Rectangle {
//        color: "#FF00FF"
        anchors.fill: yAxisLabels
    }
    Item {
        id: yAxisLabels
        x: __legendMargin + unitLabelText.width + __legendMargin
        y: childrenRect.y
        width: childrenRect.width
        height: childrenRect.height

        Repeater {
            id: yAxisRepeater
            model: __chartRoundedMax - __chartRoundedMin + 1 // + 1 cause we are inclusive
            Text {
                y: __legendMargin + index * (__chartHeight / (yAxisRepeater.count - 1)) - height / 2    // yAxisRepeater.count - 1 to get the right repartition
                text: (__chartRoundedMax - index).toFixed(2)
                font.pixelSize: 12
            }
        }
    }
    // --

    // bottom xAxis legend
    Rectangle {
//        color: "#FF00FF"
        anchors.fill: xAxisLabels
    }
    Item {
        id: xAxisLabels
        x: childrenRect.x
        y: chart.height - height - __legendMargin
        width: childrenRect.width
        height: childrenRect.height

        Repeater {
            id: xAxisRepeater
            model: Utilities.dateDiffInDays(endDate, startDate) + 1

            AABB {
                x: __chartLeft + index * (__chartWidth / (xAxisRepeater.count - 1)) - (width - xAxisLabelsText.height / 2)
                visible: index % daysRepartition == 0

                Text {
                    id: xAxisLabelsText
                    text: Utilities.addDays(startDate, index + 1).toLocaleDateString(Qt.LocalDate)
                    font.pixelSize: 12

                    anchors.centerIn: parent
                    rotation: -45
                    transformOrigin: Item.Center
                }
            }
        }
    }
    // --

    Rectangle {
        id: chartZone
        x: chart.__chartLeft
        y: chart.__chartTop
        width: chart.__chartWidth
        height: chart.__chartHeight
//        color: "#660000"

        Repeater {  // Horizontal lines of the chart grid
            id: horizontalLines
            model: yAxisRepeater.count
            Line {
                x1: -height  // -height to avoid a little hole on the bottom-left corner
                y1: index * (__chartHeight / (horizontalLines.count - 1))    // horizontalLines.count - 1 to get the right repartition
                x2: chartZone.width
                y2: y1
                color: "#868686"
            }
        }

        Repeater {  // Vertical lines of the chart grid
            id: verticalLines
            model: xAxisRepeater.count
            Line {
                x1: index * (__chartWidth / (verticalLines.count - 1))
                y1: 0
                x2: x1 + 0.0001 // An espislon is added cause vertical lines aren't drawn
                y2: chartZone.height
                color: "#868686"
                visible: index % daysRepartition == 0
            }
        }

        Repeater {
            id: curvesRepeater
            model: chart.curves

            Repeater {
                id: pointsRepeater
                model: modelData.points

                property color  color: modelData.color
                property var    comments: modelData.comments

                Item {
                    id: point

                    enabled: __valid
                    visible: __valid

                    property var    pointIndex: index
                    property var    __x: fixX(modelData.x)
                    property var    __y: fixY(modelData.y)
                    property bool   __valid: !isNaN(modelData.y)

                    function    fixX(x) {return x * (chartZone.width / (xAxisRepeater.count - 1))}  // x repartition need match the legend not the model

                    function    fixY(y) {return chartZone.height - chartZone.height * (y - chart.__chartRoundedMin) / (chart.__chartRoundedMax - chart.__chartRoundedMin)}

                    Line {
                        x1: visible ? pointsRepeater.itemAt(point.pointIndex - 1).__x : 0
                        y1: visible ? pointsRepeater.itemAt(point.pointIndex - 1).__y : 0
                        x2: point.__x
                        y2: point.__y
                        visible: point.__valid && pointsRepeater.itemAt(point.pointIndex - 1) && pointsRepeater.itemAt(point.pointIndex - 1).__valid // visible only if previous point is valid (defined and not nan)
                        color: pointsRepeater.color
                    }
                    Rectangle {
                        x: point.__x - width / 2
                        y: point.__y - height / 2
                        width: 5
                        height: 5
                        color: pointsRepeater.color

                        MouseArea {
                            id: hoverArea
                            anchors.fill: parent
                            hoverEnabled: true

                            onHoveredChanged: {
                                if (containsMouse)
                                {
                                    chartTips.pointX = point.__x
                                    chartTips.pointY = point.__y
                                    chartTips.date = Utilities.addDays(startDate, modelData.x + 1)
                                    chartTips.weight = modelData.y
                                    chartTips.comment = pointsRepeater.comments[index]
                                    chartTips.visible = true
                                }
                                else
                                    chartTips.visible = false
                            }
                        }
                    }
                }
            }
        }

        ChartTips {
            id: chartTips
            limitZone: parent
        }
    }
}
