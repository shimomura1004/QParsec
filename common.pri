SOURCE_ROOT = $$PWD

includerRelPath = $$_PRO_FILE_PWD_
includerRelPath ~= s,^$$re_escape($$SOURCE_ROOT/),,

BUILD_ROOT = $$OUT_PWD
BUILD_ROOT ~= s,$$re_escape($$includerRelPath)$,,

PROROOT = $$SOURCE_ROOT
win32: PROLIB = $$BUILD_ROOT/bin
else: PROLIB = $$BUILD_ROOT/lib
PROTEST = $$BUILD_ROOT/tests

win32: LIBPATH += $${BUILD_ROOT}/bin
else: LIBPATH += $${BUILD_ROOT}/lib

CONFIG += c++11
