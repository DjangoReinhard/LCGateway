QT += network widgets testlib
CONFIG += c++11 console
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LINUXCNC = /usr/local/src/linuxcnc-deb11
requires(qtConfig(udpsocket))

INCLUDEPATH += \
    src/model \
    src/service \
    src/test \
    /usr/include/python3.9 \    # only linuxcnc uses python
    $${LINUXCNC}/include \
    $${LINUXCNC}/src/emc/rs274ngc \
    $${LINUXCNC}/src/emc/tooldata \
    $${LINUXCNC}/src \

SOURCES += \
    src/model/credentials.cpp \
    src/model/httperror.cpp \
    src/model/httpmessage.cpp \
    src/model/httprequest.cpp \
    src/model/httpresponse.cpp \
    src/model/sysevent.cpp \
    src/model/redstatus.cpp \
    src/service/authhandler.cpp \
    src/service/restclient.cpp \
    src/service/restserver.cpp \
    src/service/errorreader.cpp \
    src/service/authenticator.cpp \
    src/service/commandhandler.cpp \
    src/service/abstractservice.cpp \
    src/service/broadcaster.cpp \
    src/service/gateway.cpp \
    src/service/statusreader.cpp \
    src/test/echohandler.cpp \
    src/test/testclient.cpp \
    src/test/unittests.cpp

unix:!mac {
  LIBS += -Wl,-rpath=$${LINUXCNC}/lib
  QMAKE_CXXFLAGS += -std=gnu++11
}
LIBS += -L$${LINUXCNC}/lib
LIBS += -L/usr/lib
LIBS += \
  -lm \
  -llinuxcnc \
  -lposemath \
  -lnml \
  -lrs274 \
  -llinuxcncini \
  -lpyplugin \
  -llinuxcnchal \
  -ltooldata \
  -lstdc++ \
  -lboost_python39 \
  -lpython3.9 \
  -lcrypt \
  -lpthread \
  -ldl \
  -lutil

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/model/credentials.h \
    src/model/httpmessage.h \
    src/model/httpresponse.h \
    src/model/redstatus.h \
    src/model/httprequest.h \
    src/model/sysevent.h \
    src/service/broadcaster.h \
    src/service/abstractservice.h \
    src/service/authhandler.h \
    src/service/commandhandler.h \
    src/service/gateway.h \
    src/service/restserver.h \
    src/service/statusreader.h \
    src/service/restclient.h \
    src/service/errorreader.h \
    src/service/authenticator.h \
    src/test/echohandler.h \
    src/test/testclient.h \
    src/test/unittests.h

