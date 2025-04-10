import QtQuick
import QtQuick.Window
import QtSvg
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
                required property var model;
                width: 10;
                height: width;
                color: {
                    switch(model.area) {
                        case 1: return "green";
                        case 2: return "lightgreen";
                        case 4: return "blue";
                        case 8: return "white";
                        default: return "red";
                    }
                }
                radius: width / 2;
                SvgImage {}
            }
        }
    }
}
