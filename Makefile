CXX = g++-4.6
CXXFLAGS = -Wall -MMD -std=c++0x

XLIB_INC = -L/usr/X11R6/lib
LDFLAGS = -lX11 -lrt -lXext

OBJECTS = 	main.o \
		XLibWindow.o \
		Engine.o \
		GameClient.o \
		GameServer.o \
		Graphics.o \
		Entity.o

DEPENDS = ${OBJECTS:.o=.d}
EXEC = a1

ifeq "$(DEBUGBUILD)" "true"
	DEBUG = -g -ggdb
else
	DEBUG = -DNDEBUG -O3
endif
CXXFLAGS += $(DEBUG)

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} ${XLIB_INC} ${LDFLAGS}

debug :
	$(MAKE) DEBUGBUILD=true

clean :
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}	# reads the .d files and reruns dependencies

