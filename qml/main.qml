import QtQuick
import QtQuick.Controls
import RealDesert

Window {
    Rectangle {
        id: "search"
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 3
        }
        height: 90
        color: "green"
        radius: 5
    }

    Rectangle {
        visible: !FileSearch.working
        anchors {
            top: search.bottom
            bottom: act.top
            left: parent.left
            right: parent.right
            margins: 3
        }
        border {
            width: 1
            color: "grey"
        }
        radius: 5

        Text {
            x: 5
            y: 3
            text: "Drop file here"
        }

        DropArea {
            anchors.fill: parent

            onEntered: function(drag) {
                drag.accepted = drag.hasUrls && DropTarget.acceptEntered(drag.urls)
            }
            onDropped: function(drag) {
                DropTarget.handle(drag.urls)
            }
        }
    }

    Rectangle {
        visible: FileSearch.working
        anchors {
            top: search.bottom
            bottom: act.top
            left: parent.left
            right: parent.right
            margins: 3
        }
        border {
            width: 1
            color: "grey"
        }
        radius: 5

        Text {
            anchors.centerIn: parent
            text: FileSearch.file
        }
    }

    Rectangle {
        id: "act"
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 3
        }
        height: 60
        color: "red"
        radius: 5
    }
}
