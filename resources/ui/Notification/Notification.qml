import QtQuick 2.1;

Item {
    id: notification;

    property alias notificationText: notificationText.text;
    property alias backgroundColor: notificationWindow.color;
    property alias fontPath: textFont.source;
    property alias textColor: notificationText.color;
    property alias timoutInterval: timeoutTimer.interval;

    QtObject {
        id: privates;
        readonly property real spacing: 16.0;
    }

    FontLoader {
        id: textFont;
    }

    Rectangle {
        id: notificationWindow;
        visible: false;
        opacity: 0.0;
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: parent.height * 0.2;
        color: "#222";

        Text {
            id: notificationText;
            anchors.centerIn: parent;
            font.family: textFont.name;
            color: "#FFF";
            wrapMode: Text.WrapAnywhere;

            Rectangle {
                id: textBorderArea;
                visible: false;
                anchors.fill: parent;
                anchors.centerIn: parent;
            }

            onTextChanged: {
                fadeOutAnim.stop();

                notificationText.wrapMode = Text.NoWrap;
                var w = notificationText.paintedWidth;
                var h = notificationText.paintedHeight;
                notificationText.wrapMode = Text.WrapAnywhere;
                var maxW = notification.width * 0.8;
                if (w > maxW) {
                    w = maxW;
                }

                notificationText.width = w;
                notificationWindow.width = w + privates.spacing;
                notificationWindow.height = h + privates.spacing;
                notificationWindow.visible = true;
                fadeInAnim.start();
            }
        }
    }

    PropertyAnimation {
        id: fadeInAnim;
        target: notificationWindow;
        property: "opacity";
        to: 1.0;
        duration: 350.0;
        easing {
            type: Easing.InExpo;
            amplitude: 0.0;
            overshoot: 0.0;
            period: 0.0;
        }

        onStopped: {
            timeoutTimer.start();
        }
    }

    PropertyAnimation {
        id: fadeOutAnim;
        target: notificationWindow;
        property: "opacity";
        to: 0.0;
        duration: 350.0;
        easing {
            type: Easing.OutExpo;
            amplitude: 0.0;
            overshoot: 0.0;
            period: 0.0;
        }

        onStopped: {
            notificationWindow.visible = false;
        }
    }

    Timer {
        id: timeoutTimer;
        interval: 3500;
        repeat: false;
        onTriggered: {
            fadeOutAnim.start();
        }
    }
}

