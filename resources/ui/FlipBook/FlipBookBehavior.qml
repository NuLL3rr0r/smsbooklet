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
        property bool flipped: false;
    }

    Component.onCompleted: {
        parent.onFlipStarted.connect(behavior.onFlipStarted);
        parent.onFlippedBack.connect(behavior.onFlippedBack);
        parent.onFlipped.connect(behavior.onFlipped);

        parent.currentPageNumber = 1;
        parent.loadFullPage(behavior.model.get(pageOffset));
    }

    function onFlipStarted() {
        if (!internal.flipped)
           return;

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

    function onFlippedBack() {
        if (!parent.fullPageVisible) {
            internal.flipped = false;
            parent.fullPageVisible = true;
            parent.destroyTemporaryPages();
        }
    }

    function onFlipped() {
        if (parent.flipablePageAngle === 180.0 || parent.flipablePageAngle === -180.0) {
            var p = parent.fromRight ? pageOffset + 1 : pageOffset - 1;
            parent.currentPageNumber = p + 1;
            parent.loadFullPage(behavior.model.get(p));
        }
        if (parent.flipablePageAngle === 180.0 || parent.flipablePageAngle === -180.0
                || parent.flipablePageAngle === 0.0) {
            parent.fullPageVisible = true;
            internal.flipped = true;
            parent.destroyTemporaryPages();
        }
    }

    onPressed: {
        if (parent.flipping) {
            return;
        }

        parent.lastflipablePageAngle = parent.flipablePageAngle;

        if (behavior.model.count <= 1)
            return;

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
        parent.fullPageVisible = false;

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
                parent.loadBackPageBackward(behavior.model.get(pageOffset));
                parent.loadBackPageForeward(behavior.model.get(pageOffset + 1));
            } else {
                parent.loadBackPageForeward(behavior.model.get(pageOffset - 1));
            }
        } else {
            parent.fromRight = false;

            if (pageOffset > 0) {
                parent.loadBackPageForeward(behavior.model.get(pageOffset));
                parent.loadBackPageBackward(behavior.model.get(pageOffset - 1));
            } else {
                parent.loadBackPageBackward(behavior.model.get(pageOffset + 1));
            }
        }

        parent.progress = calculateProgress();
    }

    onReleased: {
        if (!parent.flipping)
            return;

        parent.pageGrabbed = false;
        parent.pageReleased = true;

        if (parent.progress > 0.5) {
            parent.progress = 1.0;
        } else {
            parent.progress = 0.0;
        }
    }

    onPositionChanged: {
        if (behavior.model.count <= 1)
            return;

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

    function getCurrentPageNumber() {
        return parent.currentPageNumber;
    }

    function navigateToPage(number) {
        if (parent.flipping) {
            return;
        }

        if (number > 0 && number <= behavior.model.count) {
            internal.flipped = false;
            pageOffset = number - 1;
            parent.currentPageNumber = number;
            parent.loadFullPage(behavior.model.get(pageOffset));
            if (!parent.fullPageVisible) {
                parent.fullPageVisible = true;
            }
        }
    }
}

