TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    egm.pb.cc

HEADERS += \
    egm.pb.h



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lprotobuf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lprotobuf
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/ -lprotobuf

INCLUDEPATH += $$PWD/../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../usr/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/release/libprotobuf.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/debug/libprotobuf.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/release/protobuf.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/debug/protobuf.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/libprotobuf.a
