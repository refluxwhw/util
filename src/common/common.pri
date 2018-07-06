INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/Semaphore.h \
    $$PWD/Singleton.h \
    $$PWD/BlockFifo.h \
    $$PWD/CharFifo.h \
    $$PWD/Fifo.hpp \
    $$PWD/TBuffer.hpp \
    $$PWD/TRingBuffer.hpp

SOURCES += \
    $$PWD/BlockFifo.cpp \
    $$PWD/CharFifo.cpp

