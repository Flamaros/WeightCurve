import QtQuick 2.2

Item {
    id: aabb

    property real   childrenWidth: {
        if (children.length)
            return children[0].width
        else
            return false
    }

    property real   childrenHeight: {
        if (children.length)
            return children[0].height
        else
            return false
    }

    property real   childrenRotation: {
        if (children.length)
            return children[0].rotation
        else
            return false
    }

    property real   childrenScaling: {
        if (children.length)
            return children[0].scale
        else
            return false
    }

    onChildrenWidthChanged: update()
    onChildrenHeightChanged: update()
    onChildrenRotationChanged: update()
    onChildrenScalingChanged: update()

    function    update()
    {
        var rect = mapFromItem(children[0], children[0].x, children[0].y, children[0].width, children[0].height)
        aabb.width = rect.width
        aabb.height = rect.height
    }
}
