TEMPLATE            = app
DESTDIR             = $$PWD/../../bin
CONFIG              += console
QMAKE_LIBS_OPENGL   =

HEADERS             += variabledefine.h \
                       report.h \
                       reportssis.h \
                       reportbess.h \
                       reportprq.h \
                       reportpdfwriter.h \
                       scalenorms.h

SOURCES             += main.cpp \
                       report.cpp \
                       reportssis.cpp \
                       reportbess.cpp \
                       reportprq.cpp \
                       reportpdfwriter.cpp \
                       scalenorms.cpp

RESOURCES           += selreport.qrc

TRANSLATIONS        += $$basename(_PRO_FILE_PWD_)_zh_CN.ts

QT_LIBS_CONFIG      += mysqlwrapper
QT_LIBS_OUTPUT      = $$PWD/../../bin
include($$PWD/../../../../working/qtlibs/qtlibs.pri)
