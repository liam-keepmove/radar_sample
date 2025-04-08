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
            model: DataModel {}
            delegate: Rectangle {
                width: 15;
                height: width;
                color: "green";
            }
        }
    }
}
