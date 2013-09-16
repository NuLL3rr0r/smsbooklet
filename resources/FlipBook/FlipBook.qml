import QtQuick 2.1;

Item {
    id: flipBook;

    property alias model: flipBookBehavior.model;
    property double shadow: 0.0;
    property double progress: 0.0;
    property alias smooth: smoothener.enabled;
    property bool fromRight: true;
    property alias flipping: flipablePage.flipping;
    property alias pageSide: flipablePage.side;
    property bool evenPage: true;
    property alias enablePageAngleBehavior: pageAngleBehavior.enabled;
    property alias flipablePageAngle: detailsFlipableRotation.angle;
    property bool resetFlipablePageAngle: false;
    property bool pageGrabbed: false;
    property bool pageReleased: false;

    property alias pageClipperX: pageClipper.x;
    property alias flipablePageX: flipablePage.x;

    signal flipped();

    Behavior on progress {
        id: smoothener;
        enabled: false;
        SmoothedAnimation {
            velocity: 1.0;
        }
    }

    Component.onCompleted: {
    }

    Component.onDestruction: {
    }

    Item {
        width: flipBook.width / 2.0;
        height: flipBook.height;
        clip: true;
        Page {
            id: frontPageItem;
            width: flipBook.width;
            shadow: flipBook.shadow;
        }
    }

    Item {
        width: flipBook.width / 2.0;
        height: flipBook.height;
        x: flipBook.width / 2.0;
        clip: true;
        Page {
            id: backPageItem;
            width: flipBook.width;
            x: -flipBook.width / 2.0;
            shadow: flipBook.shadow;
        }
    }

    Item {
        id: pageClipper;
        clip: true;
        width: parent.width / 2.0;
        x: parent.width / 2.0;
        height: parent.height;

        Flipable {
            id: flipablePage;
            x: -flipBook.width / 2.0;
            width: flipBook.width;
            height: flipBook.height;

            property bool flipping: false;
            property double progress;
            progress: flipBook.progress;

            front: Page {
                id: flipableFrontPage;
                shadow: flipBook.shadow * 4.0;
            }

            back: Page {
                id: flipableBackPage;
                shadow: flipBook.shadow * 4.0;
            }

            transform: [
                Rotation {
                    id: detailsFlipableRotation;
                    origin.x: flipBook.width / 2.0;
                    origin.y: flipBook.height / 2.0;
                    axis.x: 0.0;
                    axis.y: 1.0;
                    axis.z: 0.0;
                    angle: {
                        if (flipBook.resetFlipablePageAngle) {
                            pageAngleBehavior.enabled = false;
                            smoothener.enabled = false;
                            return 0;
                        }

                        var a = flipBook.progress * (flipBook.fromRight ? -180.0 : 180.0);

                        if (flipBook.fromRight) {
                            if (a <= -90.0) {
                                pageClipper.x = 0.0;
                                flipablePage.x = 0.0;
                            } else {
                                var x = flipBook.width / 2.0;
                                pageClipper.x = x;
                                flipablePage.x = -x;
                            }
                        } else {
                            if (a <= 90.0) {
                                pageClipper.x = 0.0;
                                flipablePage.x = 0.0;
                            } else {
                                var x = flipBook.width / 2.0;
                                pageClipper.x = x;
                                flipablePage.x = -x;
                            }
                        }

                        if (a % 180.0 != 0.0 || a != 0.0) {
                            flipBook.flipping = true;
                        }

                        return a;
                    }

                    onAngleChanged: {
                        if (flipBook.pageReleased) {
                            if (angle % 180.0 == 0.0) {
                                pageAngleBehavior.enabled = false;
                                smoothener.enabled = false;
                                flipped();
                                flipBook.flipping = false;
                            }
                        }
                    }

                    Behavior on angle {
                        id: pageAngleBehavior;
                        enabled: false;
                        SmoothedAnimation {
                            velocity: 200.0;
                        }
                    }
                }
            ]
        }
    }

    FlipBookBehavior {
        id: flipBookBehavior;
    }

    function loadFrontPageForeward(page) {
        for(var i = frontPageItem.container.children.length; i > 0 ; --i)
          frontPageItem.container.children[i - 1].destroy();
        for(var i = flipableFrontPage.container.children.length; i > 0 ; --i)
          flipableFrontPage.container.children[i - 1].destroy();

        Qt.createQmlObject(page.contents, frontPageItem.container);
        Qt.createQmlObject(page.contents, flipableFrontPage.container);
    }

    function loadBackPageForeward(page) {
        for(var i = backPageItem.container.children.length; i > 0 ; --i)
          backPageItem.container.children[i - 1].destroy();
        for(var i = flipableBackPage.container.children.length; i > 0 ; --i)
          flipableBackPage.container.children[i - 1].destroy();

        Qt.createQmlObject(page.contents, backPageItem.container);
        Qt.createQmlObject(page.contents, flipableBackPage.container);
    }

    function loadFrontPageBackward(page) {
        for(var i = backPageItem.container.children.length; i > 0 ; --i)
          backPageItem.container.children[i - 1].destroy();
        for(var i = flipableFrontPage.container.children.length; i > 0 ; --i)
          flipableFrontPage.container.children[i - 1].destroy();

        Qt.createQmlObject(page.contents, backPageItem.container);
        Qt.createQmlObject(page.contents, flipableFrontPage.container);
    }

    function loadBackPageBackward(page) {
        for(var i = frontPageItem.container.children.length; i > 0 ; --i)
          frontPageItem.container.children[i - 1].destroy();
        for(var i = flipableBackPage.container.children.length; i > 0 ; --i)
          flipableBackPage.container.children[i - 1].destroy();

        Qt.createQmlObject(page.contents, frontPageItem.container);
        Qt.createQmlObject(page.contents, flipableBackPage.container);
    }
}

