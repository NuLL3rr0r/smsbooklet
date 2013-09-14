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
                        /*var angleOffset = flipBookBehavior.pageOffset * -180.0;
                        console.log(-720 % 360)
                        if (angleOffset % 360 == 0)
                            angleOffset = 0;
                        else {
                            if (angleOffset % 180 == 0) {
                                angleOffset = -180
                            }
                        }

                        var rightAngle = angleOffset + (flipBook.fromRight ? -90.0 : 9.0);
                        var a = angleOffset + (flipBook.progress * (flipBook.fromRight ? -180.0 : 180.0));

                        if (a <= rightAngle) {
                            pageClipper.x = 0.0;
                            flipablePage.x = 0.0;
                        } else {
                            pageClipper.x = flipBook.width / 2.0;
                            flipablePage.x = -flipBook.width / 2.0;
                        }

                        if (a % 180.0 != 0.0) {
                            flipBook.flipping = true;
                        }*/


                        ////////////// 333333333333
                        //console.log("ddddddd")

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
                                pageClipper.x = flipBook.width / 2.0;
                                flipablePage.x = -flipBook.width / 2.0;
                            }
                        } else {
                            if (a <= 90.0) {
                                pageClipper.x = 0.0;
                                flipablePage.x = 0.0;
                                //console.log("eeeeeeee")
                            } else {
                                //console.log("ddddddd")
                                pageClipper.x = flipBook.width / 2.0;
                                flipablePage.x = -flipBook.width / 2.0;
                            }
                        }

                        //if (a != 0.0 && a != -180.0 && a != 180.0) {
                        if (a % 180.0 != 0.0) {
                            flipBook.flipping = true;
                        }

                        ///// 22222222222
                        /*var a = 0.0;
                        var rightAngle = flipBook.evenPage
                                ? (flipBook.fromRight ? -90.0 : 90.0)
                                : (flipBook.fromRight ? -270.0 : 270.0);
                        var straightAngle = flipBook.fromRight ? -180.0 : 180.0;

                        if (evenPage) {
                            a = flipBook.progress * straightAngle;
                        } else {
                            //console.log("a ", a, "   rightAngle  ", rightAngle)
                            ///console.log("straightAnglee  ", straightAngle)
                            a = ((1.0 - flipBook.progress) * straightAngle) + straightAngle;
                        }

                        if (a < rightAngle) {
                            pageClipper.x = 0.0;
                            flipablePage.x = 0.0;
                        } else {
                            pageClipper.x = flipBook.width / 2.0;
                            flipablePage.x = -flipBook.width / 2.0;
                        }

                        if (a == -360.0 || a == 360.0) {
                            pageAngleBehavior.enabled = false;
                            a = 0.0;
                        }

                        if (a == 0.0 || a == -180.0 || a == 180.0) {
                            flipBook.flipping = false;
                            flipped();
                        } else {
                            flipBook.flipping = true;
                        }*/

                        ///////// 1111111
                        /*if (flipBook.fromRight) {
                            if (internal.flipablePageAngle <= 0.0 &&
                                    internal.flipablePageAngle >= -180.0) {
                                a = flipBook.progress * -180.0;

                                if (a < -90.0) {
                                    pageClipper.x = 0.0;
                                    flipablePage.x = 0.0;
                                } else {
                                    pageClipper.x = flipBook.width / 2.0;
                                    flipablePage.x = -flipBook.width / 2.0;
                                }
                            } else {
                                a = (flipBook.progress * -180.0) + internal.flipablePageAngle;

                                if (a < -270.0) {
                                    pageClipper.x = 0.0;
                                    flipablePage.x = 0.0;
                                } else {
                                    pageClipper.x = flipBook.width / 2.0;
                                    flipablePage.x = -flipBook.width / 2.0;
                                }
                            }
                        } else {
                            a = flipBook.progress * 180.0;

                            /*if (a < 90.0) {
                                pageClipper.x = 0.0;
                                flipablePage.x = 0.0;
                            } else {
                                pageClipper.x = flipBook.width / 2.0;
                                flipablePage.x = -flipBook.width / 2.0;
                            }*/
                        //}

                        return a;
                    }

                    onAngleChanged: {
                        /*if (!flipBook.flipping)
                            return;*/

                        if (flipBook.pageReleased) {
                            if (angle % 180.0 == 0.0) {
                                //console.log(flipBookBehavior.pageOffset)
                                //console.log(angle)
                                pageAngleBehavior.enabled = false;
                                smoothener.enabled = false;
                                //pageClipper.x = flipBook.width / 2.0;
                                //flipablePage.x = -flipBook.width / 2.0;
                                //angle = 0.0;
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

            /*
            transform: [
                Rotation {
                    id: detailsFlipableRotation;
                    origin.x: flipBook.width / 2.0;
                    origin.y: flipBook.height / 2.0;
                    axis.x: 0.0;
                    axis.y: 1.0;
                    axis.z: 0.0;
                    angle: 0.0;
                }
            ]

            states: State {
                name: "back";

                PropertyChanges {
                    target: detailsFlipableRotation;
                    angle: -180.0;
                }

                when: flipablePage.flipped;
            }

            transitions: Transition {
                ParallelAnimation {
                    NumberAnimation {
                        target: detailsFlipableRotation;
                        property: "angle";
                        duration: 2000.0;
                        easing.type: "InOutCubic";
                    }

                    NumberAnimation {
                        target: flipBook;
                        property: "shadow";
                        from: 0.0;
                        to: 0.2;
                        duration: 1000.0;
                        easing.type: "InCubic";
                    }

                    SequentialAnimation {
                        PauseAnimation {
                            duration: 1000.0;
                        }

                        ScriptAction {
                            script: {
                                if(pageClipper.x != 0.0) {
                                    pageClipper.x = 0.0;
                                    flipablePage.x = 0.0;
                                } else {
                                    pageClipper.x = flipBook.width / 2.0;
                                    flipablePage.x = -flipBook.width / 2.0;
                                }
                            }
                        }

                        ParallelAnimation {
                            NumberAnimation {
                                target: flipBook;
                                property: "shadow";
                                from: 0.2; to: 0.0;
                                duration: 1000.0;
                                easing.type: "OutCubic";
                            }
                        }
                    }
                }

                onRunningChanged: {
                    if (!running) {
                        flipBookBehavior.updatePages();
                        flipBook.isAnimRunning = false;
                    } else {
                        flipBook.isAnimRunning = true;
                    }
                }
            }*/
        }
    }

    FlipBookBehavior {
        id: flipBookBehavior;
    }

    function loadFrontPage(page) {
        for(var i = frontPageItem.container.children.length; i > 0 ; --i)
          frontPageItem.container.children[i - 1].destroy();
        for(var i = flipableFrontPage.container.children.length; i > 0 ; --i)
          flipableFrontPage.container.children[i - 1].destroy();

        Qt.createQmlObject(page.contents, frontPageItem.container);
        Qt.createQmlObject(page.contents, flipableFrontPage.container);
    }

    function loadBackPage(page) {
        for(var i = backPageItem.container.children.length; i > 0 ; --i)
          backPageItem.container.children[i - 1].destroy();
        for(var i = flipableBackPage.container.children.length; i > 0 ; --i)
          flipableBackPage.container.children[i - 1].destroy();

        Qt.createQmlObject(page.contents, backPageItem.container);
        Qt.createQmlObject(page.contents, flipableBackPage.container);
    }


    /*function loadPages(frontPage, backPage) {
        for(var i = frontPageItem.container.children.length; i > 0 ; --i)
          frontPageItem.container.children[i - 1].destroy();
        for(var i = flipableFrontPage.container.children.length; i > 0 ; --i)
          flipableFrontPage.container.children[i - 1].destroy();

        for(var i = backPageItem.container.children.length; i > 0 ; --i)
          backPageItem.container.children[i - 1].destroy();
        for(var i = flipableBackPage.container.children.length; i > 0 ; --i)
          flipableBackPage.container.children[i - 1].destroy();

        if (flipablePage.side == Flipable.Front) {
            Qt.createQmlObject(frontPage.contents, frontPageItem.container);
            Qt.createQmlObject(frontPage.contents, flipableFrontPage.container);
            Qt.createQmlObject(backPage.contents, backPageItem.container);
            Qt.createQmlObject(backPage.contents, flipableBackPage.container);
        } else {
            Qt.createQmlObject(backPage.contents, frontPageItem.container);
            Qt.createQmlObject(backPage.contents, flipableFrontPage.container);
            Qt.createQmlObject(frontPage.contents, backPageItem.container);
            Qt.createQmlObject(frontPage.contents, flipableBackPage.container);
        }
    }*/

    /*function nextPage() {
        if (!flipBook.isAnimRunning) {
            flipBook.flipped = !flipBook.flipped;
        }
    }

    function previousPage() {
        if (!flipBook.isAnimRunning) {
            flipBook.flipped = !flipBook.flipped;
        }
    }*/

    /*function nextPage() {
        if (!flipBook.isAnimRunning) {
            pageClipper.x = flipBook.width / 2.0;
            flipablePage.x = -flipBook.width / 2.0;

            if (flipablePage.state == "-360.0") {
                detailsFlipableRotation.angle = 0.0;
                flipablePage.state = "";
            }

            if(flipablePage.state == "") {
                flipablePage.state = "-180.0";
            } else if (flipablePage.state == "-180.0") {
                flipablePage.state = "-360.0";
            }
        }
    }

    function previousPage() {
        if (!flipBook.isAnimRunning) {
            pageClipper.x = 0.0;
            flipablePage.x = 0.0;

            if (flipablePage.state == "") {
                detailsFlipableRotation.angle = -360.0;
                flipablePage.state = "-360.0";
            }

            if(flipablePage.state == "-180.0") {
                flipablePage.state = "";
            } else if (flipablePage.state == "-360.0") {
                flipablePage.state = "-180.0";
            }
        }
    }*/
}

