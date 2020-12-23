QT       += core gui widgets core gui datavisualization network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
CONFIG += resources_big
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bootupdialog.cpp \
    graphmodifier.cpp \
    graphwindow.cpp \
    loaddatathread.cpp \
    main.cpp \
    mainwindow.cpp \
    progressdialog.cpp \
    selectdialog.cpp

HEADERS += \
    bootupdialog.h \
    graphmodifier.h \
    graphwindow.h \
    include.h \
    loaddatathread.h \
    mainwindow.h \
    progressdialog.h \
    selectdialog.h

FORMS += \
    bootupdialog.ui \
    graphwindow.ui \
    mainwindow.ui \
    progressdialog.ui \
    selectdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data.qrc


