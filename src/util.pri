INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

include (common/common.pri)
include (dir/dir.pri)
include (encode/encode.pri)
include (file/file.pri)
include (string/string.pri)

win32 {
    include (win/win.pri)
}
