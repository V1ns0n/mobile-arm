TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    egm.pb.cc


HEADERS += \
    egm.pb.h



unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lprotobuf

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libprotobuf.a
