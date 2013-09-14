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
        //updatePages();

        parent.loadFrontPage(behavior.model.get(pageOffset));
    }

    function onFlipped() {
        if (!parent.flipping) {
            console.log("Flipped " + pageOffset)
            if (pageOffset == 0 && !parent.fromRight) {
                console.log("pageOffset = 1")
                pageOffset = 1;
            } else if (pageOffset == behavior.model.count - 1 && parent.fromRight) {
                console.log("--pageOffset")
                --pageOffset;
            } else {
                console.log("pageOffset += parent.fromRight ? 1 : -1")
                pageOffset += parent.fromRight ? 1 : -1;
            }
        }

        //parent.loadFrontPage(behavior.model.get(pageOffset));
        //parent.loadBackPage(behavior.model.get(pageOffset));

        //console.log("fffffffff");
        /*parent.enablePageAngleBehavior = false;
        parent.smooth = false;
        //parent.progress = 0.0;
        parent.smooth = true;
        parent.enablePageAngleBehavior = true;*/
    }

    onPressed: {
        /*if (parent.flipping)
            return;*/

        //console.log("aaaa  " + parent.flipablePageAngle % 360);
        //console.log("bbbb  " + parent.flipablePageAngle % 180);
        if (!parent.flipping) {
            parent.enablePageAngleBehavior = false;
            parent.smooth = false;
            parent.resetFlipablePageAngle = true;
            parent.progress = 0.0;
            /*if (!parent.fromRight) {
                parent.pageClipperX = flipBook.width / 2.0;
                parent.flipablePageX = -flipBook.width / 2.0;
            } else {
                parent.pageClipperX = 0.0;
                parent.flipablePageX = 0.0;
            }*/
            //console.log("pagefffffffOffset  ", pageOffset)
            parent.loadFrontPage(behavior.model.get(pageOffset));
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
            console.log(1)
            parent.fromRight = true;

            if (pageOffset < behavior.model.count - 1) {
                parent.loadBackPage(behavior.model.get(pageOffset + 1));
            } else {
                parent.loadBackPage(behavior.model.get(pageOffset - 1));
            }

            /*if (parent.progress > 0.5 && pageOffset < behavior.model.count - 1) {
                parent.progress = 0.0;
                ++pageOffset;
                updatePages();
            }*/
        } else {
            console.log(2)
            parent.fromRight = false;

            if (pageOffset > 0) {
                parent.loadBackPage(behavior.model.get(pageOffset - 1));
            } else {
                parent.loadBackPage(behavior.model.get(pageOffset + 1));
            }

            /*if (parent.progress < 0.5 && pageOffset > 0) {
                parent.progress = 1.0;
                --pageOffset;
                updatePages();
            }*/
        }

        parent.progress = calculateProgress();
        //console.log("p" + parent.progress);
    }

    onReleased: {
        /*if (!parent.flipping)
            return;*/

        parent.pageGrabbed = false;
        parent.pageReleased = true;

        if (parent.progress > 0.5) {
            parent.progress = 1.0;
        } else {
            parent.progress = 0.0;
        }

        /*if (internal.dragged) {
            if (parent.progress > 0.5) {
                parent.progress = 1.0;
            } else {
                parent.progress = 0.0;
            }
        } else {
            if (parent.fromRight) {
                parent.progress = 1.0;
            } else {
                parent.progress = 0.0;
            }
        }*/

        //parent.enablePageAngleBehavior = false;
        //parent.smooth = false;
    }

    onPositionChanged: {
        internal.dragged = true;
        parent.progress = calculateProgress();

//        if (parent.progress > 0.5 && pageOffset < behavior.model.count - 1) {
//            ++pageOffset;
//        } else if (parent.progress <= 0.5 && pageOffset > 0) {
//            --pageOffset;
//        }

        //console.log("OnPositionChanged  ", "pageOffset  ", pageOffset)

        /*
        if (pageOffset == 0 && !parent.fromRight) {
            pageOffset = 1;
        } else if (pageOffset == behavior.model.count - 1 && parent.fromRight) {
            pageOffset -= 1;
        } else {
            pageOffset += parent.fromRight ? 1 : -1;
        }*/
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

    function updatePages() {
        /*if (pageOffset >= 0 && pageOffset < behavior.model.count - 1) {
            parent.loadPages(behavior.model.get(pageOffset),
                             behavior.model.get(pageOffset + 1));
        }*/
    }

    function nextPage() {
        /*if (pageOffset >= behavior.model.count - 1 && parent.progress > 0.5)
            return;

        parent.smooth = false;
        if (parent.progress > 0.5 && pageOffset < behavior.model.count - 1) {
            pageOffset += 1;
            updatePages();
        }
        parent.progress = 0.0;
        parent.fromRight = true;
        parent.smooth = true;
        parent.progress = 1.0;*/
    }

    function previousPage() {
        /*if (pageOffset == 0 && parent.progress < 0.5)
            return;

        parent.smooth = false;
        if (parent.progress < 0.5 && pageOffset > 0) {
            pageOffset -= 1;
            updatePages();
        }
        parent.progress = 1.0;
        parent.fromRight = false;
        parent.smooth = true;
        parent.progress = 0.0;*/
    }
}

