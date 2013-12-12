import QtQuick 2.1;
import "FlipBook"
import "Notification"
import "PageScroll"

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
        fontPath: FontPath;
    }

    FontLoader {
        id: textFont;
        source: FontPath;
    }

    function getCurrentPageNumber() {
        return flipBook.getCurrentPageNumber();
    }

    function navigateToPage(number) {
        flipBook.navigateToPage(number);
    }

    function formatNumsToPersian(str)
    {
        var persian = "";

        for (var i = 0; i < str.length; ++i) {
            switch (str.charAt(i)) {
            case '0':
                persian += "۰";
                break;
            case '1':
                persian += "۱";
                break;
            case '2':
                persian += "۲";
                break;
            case '3':
                persian += "۳";
                break;
            case '4':
                persian += "۴";
                break;
            case '5':
                persian += "۵";
                break;
            case '6':
                persian += "۶";
                break;
            case '7':
                persian += "۷";
                break;
            case '8':
                persian += "۸";
                break;
            case '9':
                persian += "۹";
                break;
            default:
                persian += str.charAt(i);
            }
        }

        return persian;
    }
}


