TEMPLATE = app
TARGET = ../bin/samosa
windows {
	TARGET = ../../bin/samosa
}
include(../shared/shared.pri)


HEADERS += 	\
						dominanceannealer.h \


SOURCES += 	\
						dominanceannealer.cpp \
						samosa.cpp 

