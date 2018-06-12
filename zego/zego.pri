macx: PLATEFORM_NAME = "mac"
else:win32: PLATEFORM_NAME = "win"
else:linux-*: PLATEFORM_NAME = "linux-$${ARCHITECTURE}"
else: PLATEFORM_NAME = "unknown"

DEBUG_RELEASE_NAME=release
DEBUG_RELEASE_SUBFIX=
CONFIG(debug, debug|release) {
    DEBUG_RELEASE_NAME=debug
    DEBUG_RELEASE_SUBFIX=d
}

C_LIB_NAME=ZegoLiveRoom$${DEBUG_RELEASE_SUBFIX}
win32{
C_LIB_PATH=$$PWD/win/libs/x86
INCLUDEPATH += $$PWD/win/include
}

LIBS += -L$${C_LIB_PATH} -l$${C_LIB_NAME}

message( "C_LIB_NAME=$${C_LIB_NAME}" )
message( "C_LIB_PATH=$${C_LIB_PATH}" )

