import QtQuick 2.1;

Rectangle {
    id : splashScreen;
    color: "#0074D3";
    width: 480;
    height: 768;

    Component.onCompleted: {
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
    }
}

