import QtQuick 2.1;

Item {
    id: notification;

    property alias notificationText: notificationText.text;
    property alias backgroundColor: notificationWindow.color;
    property alias textColor: notificationText.color;
    property alias timoutInterval: timeoutTimer.interval;

    Rectangle {
        id: notificationWindow;
        visible: false;
        opacity: 0.0;
        anchors.centerIn: parent;
        color: "#222";
        width: parent.width * 0.8;
        height: 44.0;

        Text {
            id: notificationText;
            anchors.centerIn: parent;
            color: "#FFF";
            wrapMode: Text.WrapAnywhere;

            Rectangle {
                id: textBorderArea;
                visible: false;
                anchors.fill: parent;
                anchors.centerIn: parent;
            }

            Behavior on text {
                ScriptAction {
                    script: {
                        fadeOutAnim.stop();
                        notificationWindow.visible = true;
                        fadeInAnim.start();
                    }
                }
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

        onRunningChanged: {
            if (!fadeInAnim.running) {
                timeoutTimer.start();
            }
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

        onRunningChanged: {
            if (!fadeOutAnim.running) {
                notificationWindow.visible = false;
            }
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

