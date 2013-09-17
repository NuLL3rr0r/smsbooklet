QT +=sql

# Add more folders to ship with the application, here
smsdb_resources.source = resources
smsdb_resources.target =
DEPLOYMENTFOLDERS = smsdb_resources

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
    application.hpp \
    mainwindow.hpp \
    make_unique.hpp \
    db.hpp \
    window.hpp \
    pagemodel.hpp \
    rt.hpp \
    dbtables.hpp

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    main.cpp \
    application.cpp \
    mainwindow.cpp \
    make_unique.cpp \
    db.cpp \
    window.cpp \
    pagemodel.cpp \
    rt.cpp \
    dbtables.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

RESOURCES +=

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
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
    android/version.xml \
    resources/ui/Flipable/FlipableBehavior.qml \
    resources/ui/Flipable/Flipable.qml \
    resources/ui/Flipable/Page.qml \
    resources/ui/Flipable/page_shadow.png \
    resources/ui/mainwindow.qml \
    resources/ui/splashscreen.png \
    resources/db/messages.db

