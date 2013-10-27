import QtQuick 2.1;
import "FlipBook"
import "Notification"

Rectangle {
    id: mainWindow;
    width: cppWindow.getScreenWidth();
    height: cppWindow.getScreenHeight();

    LayoutMirroring.enabled: true;
    LayoutMirroring.childrenInherit: true;

    Component.onCompleted: {
    }

    FlipBook {
        anchors.fill: parent;
        anchors.centerIn: parent;
        model: PageModel;
    }

    Notification {
        id: notification;
        anchors.fill: parent;
        anchors.centerIn: parent;
    }

    function notify(message) {
        notification.notificationText = "";
        notification.notificationText = message;
    }
}

