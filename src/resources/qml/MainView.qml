import QtQuick 2.5
import QtQuick.Controls 1.4

ApplicationWindow {
    visible: true
    width: 640
    height: 480

    Item {
        id: buttons
        anchors.top: parent.top
        width: parent.width
        height: parent.height / 4

        Item {
            id: leftButton
            anchors.left: parent.left
            width: parent.width / 3
            height: parent.height

            Button {
                id: restartButton
                anchors.centerIn: parent
                text: "Restart"
                onClicked: {
                    workerManager.restart(listView.currentIndex);
                }
            }
        }

        Item {
            id: centerButton
            anchors.left: leftButton.right
            width: parent.width / 3
            height: parent.height

            Button {
                id: pauseButton
                anchors.centerIn: parent
                text: "Pause"
                onClicked: {
                    workerManager.pause(listView.currentIndex);
                }
            }
        }

        Item {
            id: rightButton
            anchors.left: centerButton.right
            anchors.right: parent.right
            height: parent.height

            Button {
                id: stopButton
                anchors.centerIn: parent
                text: "Stop"
                onClicked: {
                    workerManager.stop(listView.currentIndex);
                }
            }
        }
    }

    Item {
        id: forms
        anchors.top: buttons.bottom
        width: parent.width
        height: parent.height / 4

        Item {
            id: nameArea
            height: parent.height / 2
            anchors.left: parent.left
            width: parent.width / 2

            TextField {
                id: nameTextField
                anchors.centerIn: parent
                width: parent.width * 2 / 3
                placeholderText: "Enter worker's name"
            }
        }

        Item {
            id: stepsArea
            height: parent.height / 2
            anchors.left: nameArea.right
            width: parent.width / 4

            TextField {
                id: stepsTextField
                anchors.centerIn: parent
                width: parent.width * 2 / 3
                placeholderText: "Steps number"
                validator: IntValidator {bottom: 1}
            }
        }

        Item {
            id: timeArea
            height: parent.height / 2
            anchors.left: stepsArea.right
            anchors.right: parent.right

            TextField {
                id: timeTextField
                anchors.centerIn: parent
                width: parent.width * 2 / 3
                placeholderText: "Step time"
                validator: IntValidator {bottom: 1}
            }
        }

        Item {
            id: startButtonArea
            anchors.top: nameArea.bottom
            width: parent.width
            height: parent.height / 4

            Button {
                id: startButton
                anchors.centerIn: parent
                text: "Start new worker"
                onClicked: {
                    workerManager.startWorker(nameTextField.text, stepsTextField.text, timeTextField.text);
                }
            }
        }
    }

    Item {
        id: listArea
        anchors.top: forms.bottom
        anchors.bottom: parent.bottom
        width: parent.width

        ListView {
            id: listView
            anchors.fill: parent

            Component {
                id: contactsDelegate
                Rectangle {
                    id: wrapper
                    height: nameArea.height
                    width: parent.width
                    anchors.topMargin: 5
                    anchors.bottomMargin: 5
                    color: listView.currentIndex == index ? "black" : "lightGreen"

                    Item {
                        id: nameArea
                        height: nameText.height
                        width: parent.width / 4
                        anchors.left: parent.left

                        Text {
                            id: nameText
                            anchors.centerIn: parent
                            font.pixelSize: 16
                            text: name
                            color: listView.currentIndex == index ? "lightGreen" : "black"
                        }
                    }

                    Item {
                        id: statusArea
                        height: nameText.height
                        width: parent.width / 4
                        anchors.left: nameArea.right

                        Text {
                            id: statusText
                            anchors.centerIn: parent
                            font.pixelSize: 16
                            text: status
                            color: listView.currentIndex == index ? "lightGreen" : "black"
                        }
                    }

                    Item {
                        id: progressWrapper
                        height: nameArea.height
                        anchors.left: statusArea.right
                        anchors.right: parent.right

                        ProgressBar {
                            id: progress
                            anchors.centerIn: parent
                            minimumValue: 0
                            maximumValue: noSteps - 1
                            value: step
                        }
                    }

                    MouseArea {
                        id: selector
                        anchors.fill: parent
                        onClicked: {
                            listView.currentIndex = index;
                        }
                    }
                }
            }

            model: workerManager.workersModel
            delegate: contactsDelegate
            focus: true
        }
    }
}
