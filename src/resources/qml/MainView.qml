import QtQuick 2.5
import QtQuick.Controls 1.4

ApplicationWindow {
    visible: true
    width: 640
    height: 480

    SplitView {
        id: splitView
        anchors.top: parent.top
        width: parent.width
        height: 100
    }

    ListView {
        id: listView
        anchors.top: splitView.bottom
        anchors.bottom: parent.bottom
        width: parent.width

        Component {
            id: contactsDelegate
            Rectangle {
                id: wrapper
                width: 180
                height: contactInfo.height
                color: ListView.isCurrentItem ? "black" : "red"
                Text {
                    id: contactInfo
                    text: name + ": " + status
                    color: wrapper.ListView.isCurrentItem ? "red" : "black"
                }
            }
        }

        model: workerManager.workersModel
        delegate: contactsDelegate
        focus: true
    }

}
