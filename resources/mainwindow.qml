import QtQuick 2.1;
import "FlipBook"

Rectangle {
    id: mainWindow;
    width: 480.0;
    height: 768.0;

    Component.onCompleted: {
    }

    FlipBook {
        anchors.fill: parent;
        anchors.centerIn: parent;
        model: PageModel;
    }
}

