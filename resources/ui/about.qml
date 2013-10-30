import QtQuick 2.1;

Rectangle {
    id: mainWindow;
    width: cppWindow.getScreenWidth();
    height: cppWindow.getScreenHeight();

    LayoutMirroring.enabled: true;
    LayoutMirroring.childrenInherit: true;

    Component.onCompleted: {
    }

    FontLoader {
        id: textFont;
        source: FontPath;
    }

    BorderImage {
        id: bgImage;
        source: BackgroundPath;
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
            id: appLogoRect;
            anchors.top: parent.top;
            anchors.horizontalCenter: parent.horizontalCenter;
            width: parent.width;

            Image {
                id: appLogo;
                source: AppLogoPath;
                asynchronous: true;
                cache: true;
                smooth: true;
                anchors.horizontalCenter: parent.horizontalCenter;
                width: cppWindow.getScreenWidth() / 1.666666666666667;
                height: cppWindow.getScreenHeight() / 3.368421052631579;
            }

            Text {
                id: versionText;
                text: "نسخه ۱.۰۰";
                font.family: textFont.name;
                font.pixelSize: 24.0;
                color: "#656565";
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top: appLogo.bottom;
                anchors.topMargin: 24.0;
            }

            Text {
                id: descriptionText;
                text: "کتابچه اس ام اس در حال حاضر شامل حدودا ۸۰ هزار اس ام اس در موضوعات گوناگون است که برای شما امکان اشتراک گذاری پیام را با سایر برنامه ها فراهم می آورد.";
                font.family: textFont.name;
                font.pixelSize: 18.0;
                color: "#656565";
                horizontalAlignment: Text.AlignHCenter;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top: versionText.bottom;
                anchors.topMargin: 24.0;
                wrapMode: Text.WordWrap;
                width: parent.width;
            }

            Text {
                id: feedbackText;
                text: "<a href=\"http://www.approsia.com/feedback\">بازخورد</a>";
                font.family: textFont.name;
                font.pixelSize: 18.0;
                color: "#656565";
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top: descriptionText.bottom;
                anchors.topMargin: 24.0;
                onLinkActivated: Qt.openUrlExternally(link);
            }
        }

        Rectangle {
            id: brandingLogoRect;
            anchors.bottom: parent.bottom;
            anchors.horizontalCenter: parent.horizontalCenter;
            height: childrenRect.height;

            Text {
                id: developedByText;
                text: "Developed by";
                font.pixelSize: 14.0;
                color: "#656565";
                anchors.horizontalCenter: parent.horizontalCenter;
            }

            Image {
                source: BrandingLogoPath;
                asynchronous: true;
                cache: true;
                smooth: true;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.top: developedByText.bottom;
                anchors.topMargin: 12.0;
            }
        }
    }
}

