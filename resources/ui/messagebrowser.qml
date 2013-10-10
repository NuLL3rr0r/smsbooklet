import QtQuick 2.1;
import "FlipBook"

Rectangle {
    id: messagebrowser;
    width: cppWindow.getScreenWidth();
    height: cppWindow.getScreenHeight();

    LayoutMirroring.enabled: true;
    LayoutMirroring.childrenInherit: true;

    Component.onCompleted: {
    }

    FlipBook {
        id: flipBook;
        anchors.fill: parent;
        anchors.centerIn: parent;
        model: PageModel;
    }
}

