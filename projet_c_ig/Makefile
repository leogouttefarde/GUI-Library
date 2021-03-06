# Variable definitions.

PLATFORM	= $(shell uname)
#PROFILING	= -p
PROFILING	= 
CC		= gcc ${PROFILING}
OBJDIR		= ./objs
SRCDIR		:= ./src
TESTS		= ./tests
INCLUDES	= ./include
INCFLAGS	:= -I${INCLUDES} -I${SRCDIR}
OPTFLAGS	:= -O3 -DNDEBUG -Werror
#OPTFLAGS	:= -g
CCFLAGS		:= -c ${OPTFLAGS} -Werror -Wall -std=c99
MATHFLAGS 	:= -lm

# The list of objects to include in the library

LIBEIOBJS	:= ${OBJDIR}/ei_application.o ${OBJDIR}/ei_widget.o \
		   ${OBJDIR}/ei_geometrymanager.o ${OBJDIR}/ei_event.o \
		   ${OBJDIR}/ei_widgetclass.o ${OBJDIR}/ei_radiobutton.o \
		   ${OBJDIR}/ei_button.o ${OBJDIR}/ei_core.o \
		   ${OBJDIR}/ei_linkedlist.o ${OBJDIR}/ei_callback.o \
		   ${OBJDIR}/ei_utilities.o ${OBJDIR}/ei_tag.o \
		   ${OBJDIR}/ei_gridder.o ${OBJDIR}/ei_dynamics.o \
		   ${OBJDIR}/ei_root.o ${OBJDIR}/ei_entry.o


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
	LIBSDLGFX	= ${PLATDIR}/libSDL_gfx.a
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

PERSO_TESTS	=	ensiskype toplevel test_placer test_relative \
			test_button2 test_geom ensiskype test_inter \
			ensigrid test_button3 radiobutton \
			extension tag entry gridder

all : ${TARGETS} ${PERSO_TESTS}

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

# tag

tag : ${OBJDIR}/tag.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o tag ${OBJDIR}/tag.o ${LIBEI} ${LIBS}

${OBJDIR}/tag.o : ${TESTS}/tag.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/tag.c -o ${OBJDIR}/tag.o


########## Tests personnels


# ATTENTION a LINKER LES BIBLIOTHEQUES MATHEMATIQUES


test_placer : ${OBJDIR}/test_placer.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_placer ${OBJDIR}/test_placer.o ${LIBEI} ${LIBS}

${OBJDIR}/test_placer.o : ${TESTS}/test_placer.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_placer.c -o ${OBJDIR}/test_placer.o 

test_relative : ${OBJDIR}/test_relative.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_relative ${OBJDIR}/test_relative.o ${LIBEI} ${LIBS}

${OBJDIR}/test_relative.o : ${TESTS}/test_relative.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_relative.c -o ${OBJDIR}/test_relative.o

test_geom : ${OBJDIR}/test_geom.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_geom ${OBJDIR}/test_geom.o ${LIBEI} ${LIBS}

${OBJDIR}/test_geom.o : ${TESTS}/test_geom.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_geom.c -o ${OBJDIR}/test_geom.o

test_button2 : ${OBJDIR}/test_button2.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_button2 ${OBJDIR}/test_button2.o ${LIBEI} ${LIBS}

${OBJDIR}/test_button2.o : ${TESTS}/test_button2.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_button2.c -o ${OBJDIR}/test_button2.o

test_button3 : ${OBJDIR}/test_button3.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_button3 ${OBJDIR}/test_button3.o ${LIBEI} ${LIBS}

${OBJDIR}/test_button3.o : ${TESTS}/test_button3.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_button3.c -o ${OBJDIR}/test_button3.o

radiobutton : ${OBJDIR}/radiobutton.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o radiobutton ${OBJDIR}/radiobutton.o ${LIBEI} ${LIBS}

${OBJDIR}/radiobutton.o : ${TESTS}/radiobutton.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/radiobutton.c -o ${OBJDIR}/radiobutton.o

entry : ${OBJDIR}/entry.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o entry ${OBJDIR}/entry.o ${LIBEI} ${LIBS}

${OBJDIR}/entry.o : ${TESTS}/entry.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/entry.c -o ${OBJDIR}/entry.o

test_frame_2 : ${OBJDIR}/test_frame_2.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_frame_2 ${OBJDIR}/test_frame_2.o ${LIBEI} ${LIBS}

${OBJDIR}/test_frame_2.o : ${TESTS}/test_frame_2.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_frame_2.c -o ${OBJDIR}/test_frame_2.o

toplevel : ${OBJDIR}/toplevel.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o toplevel ${OBJDIR}/toplevel.o ${LIBEI} ${LIBS}

${OBJDIR}/toplevel.o : ${TESTS}/toplevel.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/toplevel.c -o ${OBJDIR}/toplevel.o

ensiskype : ${OBJDIR}/ensiskype.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o ensiskype ${OBJDIR}/ensiskype.o ${LIBEI} ${LIBS}

${OBJDIR}/ensiskype.o : ${TESTS}/ensiskype.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/ensiskype.c -o ${OBJDIR}/ensiskype.o

test_inter : ${OBJDIR}/test_inter.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o test_inter ${OBJDIR}/test_inter.o ${LIBEI} ${LIBS}

${OBJDIR}/test_inter.o : ${TESTS}/test_inter.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/test_inter.c -o ${OBJDIR}/test_inter.o

ensigrid : ${OBJDIR}/ensigrid.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o ensigrid ${OBJDIR}/ensigrid.o ${LIBEI} ${LIBS}

${OBJDIR}/ensigrid.o : ${TESTS}/ensigrid.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/ensigrid.c -o ${OBJDIR}/ensigrid.o

extension : ${OBJDIR}/extension.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o extension ${OBJDIR}/extension.o ${LIBEI} ${LIBS}

${OBJDIR}/extension.o : ${TESTS}/extension.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/extension.c -o ${OBJDIR}/extension.o

gridder : ${OBJDIR}/gridder.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o gridder ${OBJDIR}/gridder.o ${LIBEI} ${LIBS}

${OBJDIR}/gridder.o : ${TESTS}/gridder.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/gridder.c -o ${OBJDIR}/gridder.o

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
