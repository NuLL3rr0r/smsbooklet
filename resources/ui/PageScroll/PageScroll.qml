import QtQuick 2.1;

Item {
    id: pageScroll;

    property double defaultValue: 1.0;
    property double minValue: 1.0;
    property double maxValue: 10.0; // this should be 'YOUR_MAX + 1'
    property double progress: 0.0;

    property alias xKnob: knob.x;
    property alias barImageSource: barImage.source;
    property alias knobImageSource: knobImage.source;
    property alias barImageW: barImage.width;
    property alias barImageH: barImage.height;
    property alias knobImageW: knobImage.width;
    property alias knobImageH: knobImage.height;

    signal signal_updated();
    signal signal_pressed();
    signal signal_released();
    signal signal_positionChanged();

    QtObject {
        id: internal;

        property bool isInitialized : false;
    }

    Component.onCompleted: {
        internal.isInitialized = true;
    }

    Component.onDestruction: {
    }

    onDefaultValueChanged: {
        if (internal.isInitialized) {
            pageScrollBehavior.refresh();
        }
    }

    onMinValueChanged: {
        if (internal.isInitialized) {
            pageScrollBehavior.refresh();
        }
    }

    onMaxValueChanged: {
        if (internal.isInitialized) {
            pageScrollBehavior.refresh();
        }
    }

    onBarImageWChanged: {
        if (internal.isInitialized) {
            pageScrollBehavior.refresh();
        }
    }

    onKnobImageWChanged: {
        if (internal.isInitialized) {
            pageScrollBehavior.refresh();
        }
    }

    PageScrollBehavior {
        id: pageScrollBehavior;

        onSignal_pressed: {
            parent.signal_pressed();
        }

        onSignal_released: {
            parent.signal_released();
        }

        onSignal_positionChanged: {
            parent.signal_positionChanged();
        }
    }

    Rectangle {
        id: bar;
        x: (knob.width / 2.0);
        anchors.verticalCenter: parent.verticalCenter;

        Image {
            id: barImage;
            anchors.verticalCenter: parent.verticalCenter;
        }

        Rectangle {
            id: knob;
            anchors.verticalCenter: parent.verticalCenter;

            Image {
                anchors.verticalCenter: parent.verticalCenter;
                id: knobImage;
            }
        }
    }
}

