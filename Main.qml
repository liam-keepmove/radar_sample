import QtQuick
import QtQuick.Window

Window {
    id: mainWindow;
    visible: true;
    visibility: "Maximized";
//  visibility: "FullScreen";
    title: qsTr("radar");
    Rectangle {
        id: root;
        anchors.fill: parent;
        color: "black";
        Radar {
            anchors.fill: parent;
            anchors.centerIn: parent;
            model: dataModel;
            delegate: Rectangle {
                width: 15;
                height: width;
                color: "green";
            }
        }
    }
}
