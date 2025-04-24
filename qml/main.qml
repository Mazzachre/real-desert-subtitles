import QtQuick 6.4
import QtQuick.Controls 6.4
import com.realdesert 1.0

Window {
    id: "root"

    Rectangle {
        id: "file"
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 3
        }
        height: App.file.length != 0 ? 30 : 0

        Item {
            visible: App.file.length != 0
            anchors.fill: parent

            Label {
                anchors.verticalCenter: parent.verticalCenter
                x: 5
                width: 100
                text: "Filename"
            }

            TextEdit {
                anchors.verticalCenter: parent.verticalCenter
                x: 110
                width: parent.width - 210
                text: App.file
                readOnly: true
                selectByMouse: true
            }

            Button {
                anchors.verticalCenter: parent.verticalCenter
                x: parent.width - 105
                width: 90
                text: 'Clear'
                enabled: App.mode != Mode.Working
                onClicked: {
                    App.clear();
                }
            }
        }
    }

    Rectangle {
        id: "search"
        anchors {
            top: file.bottom
            left: parent.left
            right: parent.right
            margins: 3
        }
        height: App.mode == Mode.Retry || App.mode == Mode.Selecting || App.mode == Mode.Selected ? 95 : 0

        Item {
            visible: App.mode == Mode.Retry || App.mode == Mode.Selecting
            anchors.fill: parent

            Rectangle {
                width: parent.width - 120
                height: 30

                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 5
                    width: 90
                    text: "Type"
                }

                ComboBox {
                    id: "searchType"
                    anchors.verticalCenter: parent.verticalCenter
                    x: 100
                    width: 120
                    model: [ "Movie", "Show" ]
                }
            }

            Rectangle {
                width: parent.width - 120
                height: 30
                y: 30

                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 5
                    width: 90
                    text: "Title"
                }

                TextField {
                    id: "searchTitle"
                    anchors.verticalCenter: parent.verticalCenter
                    x: 100
                    width: parent.width - 105
                }
            }

            Rectangle {
                width: parent.width - 120
                height: 30
                y: 60

                Label {
                    visible: searchType.currentValue == "Movie"
                    anchors.verticalCenter: parent.verticalCenter
                    x: 5
                    width: 90
                    text: "Year"
                }

                TextField {
                    id: "searchYear"
                    visible: searchType.currentValue == "Movie"
                    anchors.verticalCenter: parent.verticalCenter
                    x: 100
                    width: 80
                }

                Label {
                    visible: searchType.currentValue == "Show"
                    anchors.verticalCenter: parent.verticalCenter
                    x: 5
                    width: 90
                    text: "Season"
                }

                TextField {
                    id: "searchSeason"
                    visible: searchType.currentValue == "Show"
                    anchors.verticalCenter: parent.verticalCenter
                    x: 100
                    width: 50
                }

                Label {
                    visible: searchType.currentValue == "Show"
                    anchors.verticalCenter: parent.verticalCenter
                    x: 190
                    width: 60
                    text: "Episode"
                }

                TextField {
                    id: "searchEpisode"
                    visible: searchType.currentValue == "Show"
                    anchors.verticalCenter: parent.verticalCenter
                    x: 250
                    width: 50
                }
            }

            Button {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                x: parent.width - 105
                width: 90
                text: 'Search'
                onClicked: {
                    if (searchType.currentValue == "Movie") {
                        App.searchMovie(searchTitle.text, searchYear.text);
                    }
                    if (searchType.currentValue == "Show") {
                        App.searchMovie(searchTitle.text, searchSeason.text, searchEpisode.text);
                    }
                }
            }
        }

        Item {
            visible: App.mode == Mode.Selected
            anchors.fill: parent

            Rectangle {
                height: 30
                width: parent.width

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 5
                    text: "Imdb ID"
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 110
                    text: SelectedFeature.imdb
                }
            }

            Rectangle {
                height: 30
                width: parent.width
                y: 30

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 5
                    text: "Title"
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 110
                    text: SelectedFeature.title
                }
            }

            Rectangle {
                height: 30
                width: parent.width
                y: 60

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 5
                    text: "Year"
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 110
                    text: SelectedFeature.year
                }
            }
        }
    }

    Rectangle {
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

        Item {
            visible: App.mode == Mode.Search
            anchors.fill: parent

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

        Item {
            visible: App.mode == Mode.Working
            anchors.fill: parent

            Text {
                anchors.centerIn: parent
                text: "Working..."
            }
        }

        Item {
            visible: App.mode == Mode.Done
            anchors.fill: parent

            Rectangle {
                anchors.centerIn: parent
                width: parent.width - 10
                height: 120

                border {
                    width: 1
                    color: "grey"
                }
                radius: 5

                Item {
                    width: parent.width
                    height: 25
                    y: 10

                    Text {
                        width: (parent.width / 2) - 10
                        horizontalAlignment: Text.AlignRight
                        text: "Remaining downloads"
                    }

                    Text {
                        width: parent.width / 2
                        x: parent.width / 2
                        text: App.remaining
                    }
                }

                Item {
                    width: parent.width
                    height: 25
                    y: 35

                    Text {
                        width: (parent.width / 2) - 10
                        horizontalAlignment: Text.AlignRight
                        text: "Resets  in"
                    }

                    Text {
                        width: parent.width / 2
                        x: parent.width / 2
                        text: App.reset
                    }
                }

                Item {
                    width: parent.width
                    height: 30
                    y: 60

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: App.target
                    }
                }

                Item {
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: 90
                    width: 200

                    Button {
                        width: 90
                        text: "Clear"
                        onClicked: {
                            App.finish();
                        }
                    }

                    Button {
                        width: 90
                        text: "Stay"
                        x: 110
                        onClicked: {
                            App.stay();
                        }
                    }
                }
            }
        }

        Item {
            visible: App.mode == Mode.Retry
            anchors.fill: parent

            Text {
                anchors.centerIn: parent
                text: "No results for search..."
            }
        }

        Item {
            visible: App.mode == Mode.Selecting
            anchors.fill: parent

            ListView {
                anchors {
                    fill: parent
                    margins: 3
                }
                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.StopAtBounds
                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AlwaysOn
                    active: ScrollBar.AlwaysOn
                }
                clip: true
                model: FeatureModel
                spacing: 5

                delegate: Rectangle {
                    height: 24
                    width: root.width - 24
                    radius: 3
                    color: featureMouseHandler.containsMouse ? "#FFD580" : "#FFFFFF"

                    MouseArea {
                        id: "featureMouseHandler"
                        anchors.fill: parent
                        enabled: true
                        hoverEnabled: true
                        onClicked: App.selectFeature(id);
                    }

                    Row {
                        spacing: 5
                        anchors.fill: parent

                        Item {
                            width: parent.width * 0.15
                            height: parent.height

                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                text: imdb
                            }
                        }

                        Item {
                            width: parent.width * 0.7
                            height: parent.height

                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                text: title
                            }
                        }

                        Item {
                            width: parent.width * 0.15
                            height: parent.height

                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                text: year
                            }
                        }
                    }
                }
            }
        }

        Item {
            visible: App.mode == Mode.Selected
            anchors.fill: parent

            ListView {
                anchors {
                    fill: parent
                    margins: 3
                }
                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.StopAtBounds
                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AlwaysOn
                    active: ScrollBar.AlwaysOn
                }
                clip: true
                model: SubtitleModel
                spacing: 5

                delegate: Rectangle {
                    height: 24
                    width: root.width - 24
                    radius: 3
                    color: featureMouseHandler.containsMouse ? "#FFD580" : "#FFFFFF"

                    MouseArea {
                        id: "featureMouseHandler"
                        anchors.fill: parent
                        enabled: true
                        hoverEnabled: true
                        onClicked: App.selectSubtitle(id)
                    }

                    Row {
                        spacing: 5
                        anchors.fill: parent

                        Item {
                            width: parent.width - (5 * parent.height + 10)
                            height: parent.height

                            Text {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                leftPadding: 10
                                text: name
                            }
                        }

                        Item {
                            width: parent.height + 2
                            height: parent.height

                            Image {
                                visible: fpo
                                anchors.centerIn: parent
                                width: parent.height - 4
                                height: parent.height - 4
                                source: "qrc:/res/images/foreign.svg"
                            }
                        }

                        Item {
                            width: parent.height + 2
                            height: parent.height

                            Image {
                                visible: sdh
                                anchors.centerIn: parent
                                width: parent.height - 4
                                height: parent.height - 4
                                source: "qrc:/res/images/hearing-impaired.svg"
                            }
                        }

                        Item {
                            width: parent.height + 2
                            height: parent.height

                            Image {
                                visible: match
                                anchors.centerIn: parent
                                width: parent.height - 4
                                height: parent.height - 4
                                source: "qrc:/res/images/hash.svg"
                            }
                        }

                        Item {
                            id: "hasCommentsIcon"
                            width: parent.height + 2
                            height: parent.height

                            Image {
                                visible: hasComments
                                anchors.centerIn: parent
                                width: parent.height - 4
                                height: parent.height - 4
                                source: "qrc:/res/images/comment.svg"

                                MouseArea {
                                    anchors.fill: parent
                                    enabled: true
                                    hoverEnabled: true
                                    onEntered: function() {
                                        commentsPopup.x = hasCommentsIcon.x - commentsPopup.width
                                        commentsPopup.width = Math.min(commentsPopup.width, 625)
                                        commentsPopup.open()
                                    }
                                    onExited: function() {
                                        commentsPopup.close()
                                    }
                                }
                            }
                        }

                        Popup {
                            id: "commentsPopup"

                            contentItem: Text {
                                text: comments
                                wrapMode: Text.Wrap
                            }
                        }
                    }
                }
            }
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
        height: 10
    }

    Dialog {
        id: "errorDialog"
        modal: true
        anchors.centerIn: parent
        width: parent.width - 10
        height: parent.height - 10
        standardButtons: Dialog.Ok
        title: "Error"

        //TODO Put this in a read box?
        Text {
            id: "errorText"
            width: parent.width - 20
            height: parent.height -20
            wrapMode: Text.Wrap
            text: ""
        }

        onAccepted: {
            App.clear();
        }
    }

    Connections {
        target: App
        function onError(head, body) {
            errorDialog.title = head;
            errorText.text = body;
            errorDialog.open();
        }
    }
}
