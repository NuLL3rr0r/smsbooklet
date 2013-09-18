import QtQuick 2.1;

Rectangle {
    id: page;
    width: parent.width;
    height: parent.height;
    x: 0.0;
    y: 0.0;
    clip: true;

    readonly property alias container: pageContainer;

    property double shadow: 0.0;

    Item {
        id: pageContainer;
        anchors.fill: parent;
        anchors.centerIn: parent;
    }

    Image {
        source: "page_shadow.png";
        anchors.fill: parent;
        anchors.centerIn: parent;
        opacity: page.shadow;
    }
}
