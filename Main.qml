import QtQuick
import QtQuick.Window
import org.data_model 1.0

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
            radiusOfDetect: 24000;
            model: DataModel {}
            delegate: Rectangle {
                required property var model;
                width: 10;
                height: width;
                radius: width / 2;
                color: "green";
                Text {
                    text: parent.model.area;
                    color: "white";
                }
            }
        }
    }
}
