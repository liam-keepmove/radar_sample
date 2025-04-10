import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Effects

Item {
    id: root;
    property alias source: image.source;
    property color color;

    Image {
        id: image;
        visible: false;
        fillMode: Image.PreserveAspectFit;
        mipmap: true;
    }

    MultiEffect {
        anchors.fill: parent;
        source: image;

        colorization: 1.0;
        colorizationColor: root.color;
    }
}
