SHARED = ../shared
INCLUDEPATH += $$SHARED/
MOC_DIR     = .moc
OBJECTS_DIR = .obj
UI_DIR      = .ui
RCC_DIR     = .rcc

HEADERS += 	$$SHARED/archive.h \
						$$SHARED/annealer.h \
						$$SHARED/solution.h \
						$$SHARED/state.h \
						$$SHARED/setstate.h \
						$$SHARED/singlestate.h \
						$$SHARED/solver.h \
						$$SHARED/dtlzsolver.h \
						$$SHARED/dtlzjon.h \
						$$SHARED/jefsupport.h \
						$$SHARED/random.h

SOURCES += 	$$SHARED/archive.cpp \
						$$SHARED/annealer.cpp \
						$$SHARED/solution.cpp \
						$$SHARED/state.cpp \
						$$SHARED/setstate.cpp \
						$$SHARED/singlestate.cpp \
						$$SHARED/dtlzsolver.cpp \
						$$SHARED/dtlzjon.cpp \
						$$SHARED/jefsupport.cpp \
						$$SHARED/random.cpp
						
#The following line was inserted by qt3to4
QT +=  qt3support
CONFIG += console