import QtQuick 2.1;

MouseArea {
    id: behavior;
    anchors.fill: parent;
    anchors.centerIn: parent;
    hoverEnabled: false;

    property variant model: ListModel { }
    property int pageOffset: 0;

    QtObject {
        id: internal;

        property bool dragged: false;
    }

    Component.onCompleted: {
        parent.onFlipped.connect(behavior.onFlipped);

        parent.loadFrontPageForeward(behavior.model.get(pageOffset));
    }

    function onFlipped() {
        if (!parent.flipping) {
            if (pageOffset == 0 && !parent.fromRight) {
                pageOffset = 1;
            } else if (pageOffset == behavior.model.count - 1 && parent.fromRight) {
                --pageOffset;
            } else {
                pageOffset += parent.fromRight ? 1 : -1;
            }
        }
    }

    onPressed: {
        if (!parent.flipping) {
            parent.enablePageAngleBehavior = false;
            parent.smooth = false;
            parent.resetFlipablePageAngle = true;
            parent.progress = 0.0;
            if (mouseX > parent.width / 2.0) {
                parent.loadFrontPageForeward(behavior.model.get(pageOffset));
            } else {
                parent.loadFrontPageBackward(behavior.model.get(pageOffset));
            }
            parent.resetFlipablePageAngle = false;
            parent.enablePageAngleBehavior = true;
            parent.smooth = true;
        }

        parent.flipping = true;
        parent.pageReleased = false;
        parent.pageGrabbed = true;
        parent.evenPage = parent.pageSide === Flipable.Front;
        internal.dragged = false;

        parent.enablePageAngleBehavior = true;
        parent.smooth = true;

        if ((mouseX > parent.width / 2.0 || pageOffset == 0)
                && pageOffset != behavior.model.count - 1) {
            parent.fromRight = true;

            if (pageOffset < behavior.model.count - 1) {
                parent.loadBackPageForeward(behavior.model.get(pageOffset + 1));
            } else {
                parent.loadBackPageForeward(behavior.model.get(pageOffset - 1));
            }
        } else {
            parent.fromRight = false;

            if (pageOffset > 0) {
                parent.loadBackPageBackward(behavior.model.get(pageOffset - 1));
            } else {
                parent.loadBackPageBackward(behavior.model.get(pageOffset + 1));
            }
        }

        parent.progress = calculateProgress();
    }

    onReleased: {
        parent.pageGrabbed = false;
        parent.pageReleased = true;

        if (parent.progress > 0.5) {
            parent.progress = 1.0;
        } else {
            parent.progress = 0.0;
        }
    }

    onPositionChanged: {
        internal.dragged = true;
        parent.progress = calculateProgress();
    }

    function calculateProgress() {
        var p = (width - mouseX) / width;

        if (p < 0.0) {
            p = 0.0;
        } else if (p > 1.0) {
            p = 1.0;
        }

        if (!parent.fromRight)
            p = 1.0 - p;

        return p;
    }
}
