TEMPLATE = app
TARGET = ../bin/mosa

windows {
	TARGET =  ../../bin/mosa
}

include(../shared/shared.pri)


HEADERS += 	\
						mosaannealer.h \


SOURCES += 	\
						mosaannealer.cpp \
						mosa.cpp
