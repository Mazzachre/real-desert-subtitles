import QtQuick
import QtQuick.Controls

Window {
    id: "root"

    Rectangle {
        id: "search"
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 3
        }
        height: 90

        Item {
            visible: !(FileSearch.working || FeatureModel.results || SubtitleModel.results || SelectedFeature.result)

            Text {
                text: "Search for something?"
            }
        }

        Item {
            visible: SelectedFeature.result

            Column {
                Text {
                    text: SelectedFeature.filename
                }

                Text {
                    text: SelectedFeature.title
                }

                Text {
                    text: SelectedFeature.year
                }

                Text {
                    text: SelectedFeature.imdb
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
            visible: !(FileSearch.working || FeatureModel.results || SubtitleModel.results)
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
            visible: FileSearch.working
            anchors.fill: parent

            Text {
                anchors.centerIn: parent
                text: FileSearch.file
            }
        }

        Item {
            visible: FeatureModel.results
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
                        onClicked: FeatureModel.selectFeature(id);
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
                                text: id
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
            visible: SubtitleModel.results
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
                        onClicked: console.log(id);
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

                            Text {
                                text: comments
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
        height: 60
        color: "red"
        radius: 5

        Button {
            anchors.centerIn: parent
            text: 'Clear'
            onClicked: {
                FeatureModel.clear();
                FileSearch.clear();
                SelectedFeature.clear();
                SubtitleModel.clear();
            }
        }
    }

    Dialog {
        id: "errorDialog"
        modal: true
        anchors.centerIn: parent
        width: parent.width - 10
        height: parent.height - 10
        standardButtons: Dialog.Ok
        title: "Error"

        Text {
            id: "errorText"
            width: parent.width - 20
            height: parent.height -20
            wrapMode: Text.Wrap
            text: ""
        }
    }

    Connections {
        target: FileSearch
        function onError(error) {
            errorText.text = error;
            errorDialog.open();
        }
    }
}
