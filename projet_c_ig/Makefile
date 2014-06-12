# Variable definitions.

PLATFORM	= $(shell uname)
PROFILING	= -p
#PROFILING	= 
CC		= gcc ${PROFILING}
OBJDIR		= ./objs
TESTS		= ./tests
INCLUDES	= ./include
INCFLAGS	:= -I${INCLUDES}
#OPTFLAGS	:= -Os -DNDEBUG -Werror
OPTFLAGS	:= -g
CCFLAGS		:= -c ${OPTFLAGS} -Wall -std=c99
SRCDIR		:= ./src
MATHFLAGS 	:= -lm

# The list of objects to include in the library

LIBEIOBJS	:= ${OBJDIR}/ei_application.o ${OBJDIR}/ei_widget.o \
			${OBJDIR}/ei_geometrymanager.o ${OBJDIR}/ei_event.o \
			${OBJDIR}/ei_widgetclass.o ${OBJDIR}/ei_shape.o \
			${OBJDIR}/ei_button.o ${OBJDIR}/ei_core.o \
			${OBJDIR}/ei_linkedlist.o ${OBJDIR}/ei_callback.o \
			${OBJDIR}/ei_utilities.o ${OBJDIR}/ei_debug.o



# Platform specific definitions (OS X, Linux, Windows)

ifeq (${PLATFORM},Darwin)

# Building for Mac OS X

PLATDIR		= _osx
INCFLAGS	:= ${INCFLAGS} -I/opt/local/include/SDL
LINK		= ${CC}
LIBEI		= ${OBJDIR}/libei.a
LIBEIBASE	= ${PLATDIR}/libeibase.a
LIBS		= ${LIBEIBASE} -L/opt/local/lib -lSDL -lSDL_ttf -lSDL_gfx -lSDL_image -framework AppKit

else
   ifeq (${PLATFORM},Linux)

# Building for Linux

PLATDIR		= _x11
INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL
LINK		= ${CC}
LIBEI		= ${OBJDIR}/libei.a
LIBEIBASE	= ${PLATDIR}/libeibase.a
LIBSDLGFX       = ${PLATDIR}/libSDL_gfx.a
#LIBSDLGFX       = -lSDL_gfx
LIBS		= ${LIBEIBASE} -lSDL -lSDL_ttf ${LIBSDLGFX} -lSDL_image -lm

	else

# Building for Windows

PLATDIR		= _win
INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL -I/usr/local/include/SDL
LINK		= ${CC} -mwindows -mno-cygwin
LIBEI		= ${OBJDIR}/libei.lib
LIBEIBASE	= ${PLATDIR}/libeibase.lib
LIBS		= -lmingw32 ${LIBEIBASE} /usr/lib/SDL_ttf.lib /usr/local/lib/libSDL_gfx.a /usr/lib/SDL_image.lib -L/usr/lib -lSDL

	endif
 endif



# Main target of the makefile. To build specific targets, call "make <target_name>"

TARGETS		=	${LIBEI} \
				minimal frame button hello_world_absolute hello_world_relative puzzle

PERSO_TESTS =  ensiskype toplevel test_shape test_placer test_relative \
			   test_button2 test_geom test_resize ensiskype test_inter move

all : ${TARGETS}

tests : ${PERSO_TESTS}

########## Test-programs

# minimal

minimal : ${OBJDIR}/minimal.o ${LIBEIBASE}
	${LINK} -o minimal ${OBJDIR}/minimal.o ${LIBS}

${OBJDIR}/minimal.o : ${TESTS}/minimal.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minimal.c -o ${OBJDIR}/minimal.o

# frame

frame : ${OBJDIR}/frame.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame ${OBJDIR}/frame.o ${LIBEI} ${LIBS}

${OBJDIR}/frame.o : ${TESTS}/frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame.c -o ${OBJDIR}/frame.o

# button

button : ${OBJDIR}/button.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o button ${OBJDIR}/button.o ${LIBEI} ${LIBS}

${OBJDIR}/button.o : ${TESTS}/button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button.c -o ${OBJDIR}/button.o

# hello_world_absolute

hello_world_absolute : ${OBJDIR}/hello_world_absolute.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world_absolute ${OBJDIR}/hello_world_absolute.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world_absolute.o : ${TESTS}/hello_world_absolute.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world_absolute.c -o ${OBJDIR}/hello_world_absolute.o

# hello_world_relative

hello_world_relative : ${OBJDIR}/hello_world_relative.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world_relative ${OBJDIR}/hello_world_relative.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world_relative.o : ${TESTS}/hello_world_relative.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world_relative.c -o ${OBJDIR}/hello_world_relative.o

# puzzle

puzzle : ${OBJDIR}/puzzle.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o puzzle ${OBJDIR}/puzzle.o ${LIBEI} ${LIBS}

${OBJDIR}/puzzle.o : ${TESTS}/puzzle.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/puzzle.c -o ${OBJDIR}/puzzle.o

# move_resize

move : ${OBJDIR}/move_resize.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o move ${OBJDIR}/move_resize.o ${LIBEI} ${LIBS}

${OBJDIR}/move_resize.o : ${TESTS}/move_resize.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/move_resize.c -o ${OBJDIR}/move_resize.o

########## Tests personnels


# ATTENTION a LINKER LES BIBLIOTHEQUES MATHEMATIQUES
test_shape : ${OBJDIR}/test_shape.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_shape ${OBJDIR}/test_shape.o ${LIBEI} ${LIBS} ${MATHFLAGS}

${OBJDIR}/test_shape.o : ${TESTS}/test_shape.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_shape.c -o ${OBJDIR}/test_shape.o ${MATHFLAGS} 

test_placer : ${OBJDIR}/test_placer.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_placer ${OBJDIR}/test_placer.o ${LIBEI} ${LIBS} ${MATHFLAGS}

${OBJDIR}/test_placer.o : ${TESTS}/test_placer.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_placer.c -o ${OBJDIR}/test_placer.o ${MATHFLAGS} 

test_relative : ${OBJDIR}/test_relative.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_relative ${OBJDIR}/test_relative.o ${LIBEI} ${LIBS} ${MATHFLAGS}

${OBJDIR}/test_relative.o : ${TESTS}/test_relative.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_relative.c -o ${OBJDIR}/test_relative.o ${MATHFLAGS}

test_geom : ${OBJDIR}/test_geom.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_geom ${OBJDIR}/test_geom.o ${LIBEI} ${LIBS} ${MATHFLAGS}

${OBJDIR}/test_geom.o : ${TESTS}/test_geom.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_geom.c -o ${OBJDIR}/test_geom.o ${MATHFLAGS}

test_button2 : ${OBJDIR}/test_button2.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_button2 ${OBJDIR}/test_button2.o ${LIBEI} ${LIBS} ${MATHFLAGS}

${OBJDIR}/test_button2.o : ${TESTS}/test_button2.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_button2.c -o ${OBJDIR}/test_button2.o ${MATHFLAGS} 

test_button3 : ${OBJDIR}/test_button3.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_button3 ${OBJDIR}/test_button3.o ${LIBEI} ${LIBS} ${MATHFLAGS}

${OBJDIR}/test_button3.o : ${TESTS}/test_button3.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_button3.c -o ${OBJDIR}/test_button3.o ${MATHFLAGS} 

test_resize : ${OBJDIR}/test_resize.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_resize ${OBJDIR}/test_resize.o ${LIBEI} ${LIBS} ${MATHFLAGS}

${OBJDIR}/test_resize.o : ${TESTS}/test_resize.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_resize.c -o ${OBJDIR}/test_resize.o ${MATHFLAGS} 

test_frame_2 : ${OBJDIR}/test_frame_2.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_frame_2 ${OBJDIR}/test_frame_2.o ${LIBEI} ${LIBS} ${MATHFLAGS}

${OBJDIR}/test_frame_2.o : ${TESTS}/test_frame_2.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_frame_2.c -o ${OBJDIR}/test_frame_2.o ${MATHFLAGS} 

toplevel : ${OBJDIR}/toplevel.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o toplevel ${OBJDIR}/toplevel.o ${LIBEI} ${LIBS} ${MATHFLAGS}

${OBJDIR}/toplevel.o : ${TESTS}/toplevel.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/toplevel.c -o ${OBJDIR}/toplevel.o ${MATHFLAGS} 

ensiskype : ${OBJDIR}/ensiskype.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o ensiskype ${OBJDIR}/ensiskype.o ${LIBEI} ${LIBS} ${MATHFLAGS}

${OBJDIR}/ensiskype.o : ${TESTS}/ensiskype.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/ensiskype.c -o ${OBJDIR}/ensiskype.o ${MATHFLAGS} 

test_inter : ${OBJDIR}/test_inter.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_inter ${OBJDIR}/test_inter.o ${LIBEI} ${LIBS} ${MATHFLAGS}

${OBJDIR}/test_inter.o : ${TESTS}/test_inter.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_inter.c -o ${OBJDIR}/test_inter.o ${MATHFLAGS} 

########## Compilation séparée des sources

ei_widget : ${OBJDIR}/ei_widget.o

ei_widgetclass : ${OBJDIR}/ei_widgetclass.o

ei_shape : ${OBJDIR}/ei_shape.o

ei_global : ${OBJDIR}/ei_global.o

ei_event : ${OBJDIR}/ei_event.o

ei_geometrymanager : ${OBJDIR}/ei_geometrymanager.o

ei_gridder : ${OBJDIR}/ei_gridder.o

# Compilation
${OBJDIR}/%.o: ${SRCDIR}/%.c 
	$(CC) $(CCFLAGS) ${INCFLAGS} $< -o $@

# Building of the library libei

${LIBEI} : ${LIBEIOBJS}
	ar rcs ${LIBEI} ${LIBEIOBJS}



# Building of the doxygen documentation.

doc :
	doxygen docs/doxygen.cfg



# Removing all built files.

clean:
	rm -f ${TARGETS} ${PERSO_TESTS}
	rm -f *.exe gmon.out
	rm -f ${OBJDIR}/*

runf: all
	./frame
