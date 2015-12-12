import QtQuick 2.1;
import "FlipBook"
import "Notification"
import "PageScroll"

Rectangle {
    id: root;
    width: cppWindow.getScreenWidth();
    height: cppWindow.getScreenHeight();

    LayoutMirroring.enabled: true;
    LayoutMirroring.childrenInherit: true;

    QtObject {
        id: internal;

        property string currentWindowId: "MainWindow";
    }

    Component.onCompleted: {
    }

    Rectangle {
        id: mainWindow;
        anchors.fill: parent;
        anchors.centerIn: parent;

        FlipBook {
            id: mainWindowFlipBook;
            anchors.fill: parent;
            anchors.centerIn: parent;
            model: MainWindowPageModel;
        }
    }

    Rectangle {
        id: subCategoryBrowser;
        anchors.fill: parent;
        anchors.centerIn: parent;
        visible: false;

        FlipBook {
            id: subCategoryBrowserFlipBook;
            anchors.fill: parent;
            anchors.centerIn: parent;
            model: SubCategoryBrowserPageModel;
        }
    }

    Rectangle {
        id: messageBrowser;
        anchors.fill: parent;
        anchors.centerIn: parent;
        visible: false;

        property string msgAddedToFav: "به لیست علایق اضافه شد.";
        property string msgRemovedFromFav: "از لیست علایق حذف شد.";
        property string msgCopiedToClipboard: "به حافظه انتقال یافت.";

        FlipBook {
            id: messageBrowserFlipBook;
            anchors.fill: parent;
            anchors.centerIn: parent;
            model: MessageBrowserPageModel;
        }

        function getCurrentPageNumber() {
            return messageBrowserFlipBook.getCurrentPageNumber();
        }

        function navigateToPage(number) {
            messageBrowserFlipBook.navigateToPage(number);
        }
    }

    Rectangle {
        id: aboutWindow;
        anchors.fill: parent;
        anchors.centerIn: parent;
        visible: false;

        BorderImage {
            id: aboutWindowBgImage;
            source: AboutWindowBackgroundPath;
            asynchronous: true;
            cache: true;
            smooth: true;
            anchors.centerIn: parent;
            anchors.fill: parent;
            horizontalTileMode: BorderImage.Stretch;
            verticalTileMode: BorderImage.Stretch;
            border {
                top: 0;
                right: 0;
                bottom: 0;
                left: 0;
            }
        }

        Rectangle {
            anchors.centerIn: parent;
            width: parent.width * 0.8;
            height: parent.height * 0.8;
            color: "transparent";

            Rectangle {
                id: aboutWindowAppLogoRect;
                anchors.top: parent.top;
                anchors.horizontalCenter: parent.horizontalCenter;
                width: parent.width;

                Image {
                    id: aboutWindowAppLogo;
                    source: AboutWindowAppLogoPath;
                    asynchronous: true;
                    cache: true;
                    smooth: true;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    width: cppWindow.getScreenWidth() / 1.666666666666667;
                    height: cppWindow.getScreenHeight() / 3.368421052631579;
                }

                Text {
                    id: aboutWindowVersionText;
                    text: "نسخه ۲.۰.۰";
                    font.family: textFont.name;
                    font.pixelSize: 24.0;
                    color: "#656565";
                    anchors.horizontalCenter: parent.horizontalCenter;
                    anchors.top: aboutWindowAppLogo.bottom;
                    anchors.topMargin: 24.0;
                }

                Text {
                    id: aboutWindowDescriptionText;
                    text: "کتابچه اس ام اس در حال حاضر شامل حدودا ۸۰ هزار اس ام اس در موضوعات گوناگون است که برای شما امکان اشتراک گذاری پیام را با سایر برنامه ها فراهم می آورد.";
                    font.family: textFont.name;
                    font.pixelSize: 18.0;
                    color: "#656565";
                    horizontalAlignment: Text.AlignHCenter;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    anchors.top: aboutWindowVersionText.bottom;
                    anchors.topMargin: 24.0;
                    wrapMode: Text.WordWrap;
                    width: parent.width;
                }

                Text {
                    id: aboutWindowFeedbackText;
                    text: "<a href=\"http://www.approsia.com/feedback\">بازخورد</a>";
                    font.family: textFont.name;
                    font.pixelSize: 18.0;
                    color: "#656565";
                    anchors.horizontalCenter: parent.horizontalCenter;
                    anchors.top: aboutWindowDescriptionText.bottom;
                    anchors.topMargin: 24.0;
                    onLinkActivated: Qt.openUrlExternally(link);
                }
            }

            Rectangle {
                id: aboutWindowBrandingLogoRect;
                anchors.bottom: parent.bottom;
                anchors.horizontalCenter: parent.horizontalCenter;
                height: childrenRect.height;

                Text {
                    id: aboutWindowDevelopedByText;
                    text: "Developed by";
                    font.pixelSize: 14.0;
                    color: "#656565";
                    anchors.horizontalCenter: parent.horizontalCenter;
                }

                Image {
                    source: AboutWindowBrandingLogoPath;
                    asynchronous: true;
                    cache: true;
                    smooth: true;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    anchors.top: aboutWindowDevelopedByText.bottom;
                    anchors.topMargin: 12.0;
                }
            }
        }
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

    function notify(message) {
        notification.notificationText = "";
        notification.notificationText = message;
    }

    function initSubCategoryBrowser() {
        subCategoryBrowserFlipBook.init();
    }

    function initMessageBrowser() {
        messageBrowserFlipBook.init();
    }

    function showWindow(windowId) {
        switch (windowId) {
        case "MainWindow":
        case "SubCategoryBrowser":
        case "MessageBrowser":
        case "AboutWindow":
            mainWindow.visible = false;
            subCategoryBrowser.visible = false;
            messageBrowser.visible = false;
            aboutWindow.visible = false;
            internal.currentWindowId = windowId;
            break;
        }

        switch (windowId) {
        case "MainWindow":
            mainWindow.visible = true;
            break;
        case "SubCategoryBrowser":
            subCategoryBrowser.visible = true;
            break;
        case "MessageBrowser":
            messageBrowser.visible = true;
            break;
        case "AboutWindow":
            aboutWindow.visible = true;
            break;
        }
    }

    function getCurrentWindowId() {
        return internal.currentWindowId;
    }

    function getMessageBrowserCurrentPageNumber() {
        return messageBrowser.getCurrentPageNumber();
    }

    function navigateToMessageBrowserPage(number) {
        return messageBrowser.navigateToPage(number);
    }
}

