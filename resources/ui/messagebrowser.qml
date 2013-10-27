import QtQuick 2.1;
import "FlipBook"
import "Notification"

Rectangle {
    id: messagebrowser;
    width: cppWindow.getScreenWidth();
    height: cppWindow.getScreenHeight();

    LayoutMirroring.enabled: true;
    LayoutMirroring.childrenInherit: true;

    property string msgAddedToFav: "به لیست علایق اضافه شد.";
    property string msgRemovedFromFav: "از لیست علایق حذف شد.";

    Component.onCompleted: {
    }

    FlipBook {
        id: flipBook;
        anchors.fill: parent;
        anchors.centerIn: parent;
        model: PageModel;
    }

    Notification {
        id: notification;
        anchors.fill: parent;
        anchors.centerIn: parent;
    }
}

