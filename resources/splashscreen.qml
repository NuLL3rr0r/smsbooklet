import QtQuick 2.1;
import QtGraphicalEffects 1.0;

Rectangle {
    id : splashScreen;
    color: "#0074D3";
    width: 480;
    height: 768;

    property int passedSecondsSinceShown: 0;

    signal signal_PoppedUp();
    signal signal_TimedOut();

    Component.onCompleted: {
        blurInAnim.start();
    }

    BorderImage {
        id: splashImage;
        source: "splashscreen.png"
        asynchronous: true;
        cache: true;
        smooth: true;
        anchors.centerIn: parent;
        anchors.fill: parent;
        horizontalTileMode: BorderImage.Stretch
        verticalTileMode: BorderImage.Stretch
        border { left: 24; top: 24; right: 24; bottom: 24 }
        visible: false;
    }

    FastBlur {
        id: splashBlur;
        anchors.fill: splashImage;
        source: splashImage;
        radius: 150.0;
        cached: false;
    }

    PropertyAnimation {
        id: blurInAnim;
        target: splashBlur;
        property: "radius";
        to: 0.0;
        duration: 1500.0;
        easing.type: Easing.OutQuint;

        onRunningChanged: {
            if (!blurInAnim.running) {
                showTimer.start();
                signal_PoppedUp();
            }
        }
    }

    PropertyAnimation {
        id: blurOutAnim;
        target: splashBlur;
        property: "radius";
        to: 150.0;
        duration: 1500.0;
        easing.type: Easing.InQuint;

        onRunningChanged: {
            if (!blurOutAnim.running) {
                signal_TimedOut();
            }
        }
    }

    Timer {
        id: showTimer;
        interval: 100;
        running: false;
        repeat: true;

        onTriggered: {
            splashScreen.passedSecondsSinceShown += 100;
            if (splashScreen.passedSecondsSinceShown >= 1500) {
                showTimer.stop();
                blurOutAnim.start();
            }
        }
    }
}

