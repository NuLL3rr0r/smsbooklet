import QtQuick 2.1;

MouseArea {
    id: behavior;
    anchors.fill: parent;
    anchors.centerIn: parent;
    hoverEnabled: false;

    property variant model: ListModel { }
    property int pageOffset: 0;

    Component.onCompleted: {
        updatePages();
    }

    onPressed: {
        /*parent.smooth = false;

        if (mouseX > parent.width / 2.0) {
            parent.fromRight = true;

            if (parent.progress > 0.5 && pageOffset < behavior.model.count - 1) {
                parent.progress = 0.0;
                ++pageOffset;
                updatePages();
            }
        } else {
            parent.fromRight = false;

            if (parent.progress < 0.5 && pageOffset > 0) {
                parent.progress = 1.0;
                --pageOffset;
                updatePages();
            }
        }

        parent.smooth = true;*/
    }

    onReleased: {
        if (parent.isAnimRunning)
            return;

        if (mouseX > parent.width / 2.0) {
            if (pageOffset < behavior.model.count - 1) {
                ++pageOffset;
                parent.nextPage();
            }
        } else {
            if (pageOffset > 0) {
                --pageOffset;
                parent.previousPage();
            }
        }

        /*if (parent.progress > 0.5) {
            parent.progress = 1.0;
        } else {
            parent.progress = 0.0;
        }
        parent.smooth = false;*/
    }

    onPositionChanged: {
        //parent.progress = (width - mouseX) / width;
    }

    function updatePages() {
        if (pageOffset >= 0 && pageOffset < behavior.model.count - 1) {
            parent.loadPages(behavior.model.get(pageOffset),
                             behavior.model.get(pageOffset + 1));
        }
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

