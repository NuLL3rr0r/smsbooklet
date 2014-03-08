QT += sql
QT -= network

# Add more folders to ship with the application, here
database.source = resources/db
database.target = resources
fonts.source = resources/fnt
fonts.target = resources
images.source = resources/img
images.target = resources
DEPLOYMENTFOLDERS = database
DEPLOYMENTFOLDERS += fonts
DEPLOYMENTFOLDERS += images

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

win32 {
    QMAKE_CXXFLAGS += -std=c++11
}

unix {
    QMAKE_CXXFLAGS += -std=c++11
}

HEADERS += \
    about.hpp \
    application.hpp \
    db.hpp \
    dbtables.hpp \
    defs.hpp \
    localization.hpp \
    mainwindow.hpp \
    make_unique.hpp \
    messagebrowser.hpp \
    pagemodel.hpp \
    rt.hpp \
    subcategorybrowser.hpp \
    window.hpp

SOURCES += \
    main.cpp \
    about.cpp \
    application.cpp \
    db.cpp \
    dbtables.cpp \
    localization.cpp \
    mainwindow.cpp \
    make_unique.cpp \
    messagebrowser.cpp \
    pagemodel.cpp \
    rt.cpp \
    subcategorybrowser.cpp \
    window.cpp

android {
HEADERS += \
    android.hpp
SOURCES += \
    android.cpp
}

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

RESOURCES += resources/resources.qrc

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/src/smsdb/Android.java \
    android/version.xml \
    resources/ui/FlipBook/FlipBook.qml \
    resources/ui/FlipBook/FlipBookBehavior.qml \
    resources/ui/FlipBook/Page.qml \
    resources/ui/FlipBook/page_shadow.png \
    resources/ui/Notification/Notification.qml \
    resources/ui/PageScroll/PageScroll.qml \
    resources/ui/PageScroll/PageScrollBehavior.qml \
    resources/ui/about.qml \
    resources/ui/mainwindow.qml \
    resources/ui/messagebrowser.qml \
    resources/ui/subcategorybrowser.qml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

