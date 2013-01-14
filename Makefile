CXX = g++
CXXFLAGS = -Wall -MMD -std=c++0x

XLIB_INC = -L/usr/X11R6/lib
LDFLAGS = -lX11 -lrt

OBJECTS = 	main.o \
		XLibWindow.o \
		XLibGraphics.o \
		Game.o

DEPENDS = ${OBJECTS:.o=.d}
EXEC = a1

ifeq "$(DEBUGBUILD)" "true"
	DEBUG = -g -ggdb
else
	DEBUG = -DNDEBUG
endif
CXXFLAGS += $(DEBUG)

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} ${XLIB_INC} ${LDFLAGS}

debug :
	$(MAKE) DEBUGBUILD=true

clean :
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}	# reads the .d files and reruns dependencies

