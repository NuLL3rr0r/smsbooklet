import QtQuick 2.1;

Item {
    id: flipBook;

    property alias model: flipBookBehavior.model;
    property double shadow: 0.0;
    property alias smooth: smoothener.enabled;
    property double progress;
    property bool fromRight: true
    property alias flipped: flipablePage.flipped;
    property bool isAnimRunning: false;

    Component.onCompleted: {
    }

    Component.onDestruction: {
    }

    Behavior on progress {
        id: smoothener;
        enabled: true;
        SmoothedAnimation {
            velocity: 1.0;
        }
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

            property bool flipped: false;
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
                    angle: 0.0;
                }
            ]

            /*states: State {
                name: "back";

                PropertyChanges {
                    target: detailsFlipableRotation;
                    angle: -180.0;
                }

                when: flipablePage.flipped;
            }*/


            states: [
                State {
                    name: "-180.0";
                    PropertyChanges {
                        target: detailsFlipableRotation;
                        angle: -180.0;
                    }
                },
                State {
                    name: "-360.0";
                    PropertyChanges {
                        target: detailsFlipableRotation;
                        angle: -360.0;
                    }
                },
                State {
                    name: "default";
                    PropertyChanges {
                        target: detailsFlipableRotation;
                        angle: 0.0;
                    }
                }
            ]


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
            }
        }
    }

    FlipBookBehavior {
        id: flipBookBehavior;
    }

    function loadPages(frontPage, backPage) {
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
    }

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

    function nextPage() {
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
    }
}

