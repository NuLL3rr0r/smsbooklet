import QtQuick 2.1;

MouseArea {
    id: pageScrollBehavior;

    anchors.fill: parent;
    anchors.centerIn: parent;
    hoverEnabled: false;

    signal signal_updated();
    signal signal_pressed();
    signal signal_released();
    signal signal_positionChanged();

    QtObject {
        id: internal;

        property bool isDefaultPosSet : false;
        property double length : 0.0;
        property double rangeDiff : 0.0;
    }

    Component.onCompleted: {
        refresh();
    }

    onPressed: {
        update();
        signal_pressed();
    }

    onReleased: {
        update();
        signal_released();
    }

    onPositionChanged: {
        update();
        signal_positionChanged();
    }

    function calculateProgress() {
        var p = 1.0;
        if (internal.isDefaultPosSet) {
            p = (width - mouseX) / width;
        } else {
            p = (internal.rangeDiff - (parent.defaultValue - parent.minValue)) / internal.rangeDiff;
            internal.isDefaultPosSet = true;
        }

        if (p < 0.0) {
            p = 0.0;
        } else if (p > 1.0) {
            p = 1.0;
        }

        return 1.0 - p;
    }

    function refresh() {
        internal.length = parent.barImageW - parent.knobImageW;
        internal.rangeDiff = parent.maxValue - parent.minValue;
        update();
    }

    function update() {
        var p = calculateProgress();
        parent.progress = parent.minValue + (internal.rangeDiff * p);
        parent.xKnob = internal.length * p;
        signal_updated();
    }
}

