INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/Semaphore.h \
    $$PWD/Singleton.h \
    $$PWD/BlockFifo.h \
    $$PWD/CharFifo.h

SOURCES += \
    $$PWD/BlockFifo.cpp \
    $$PWD/CharFifo.cpp \
    $$PWD/fifo.tcc

