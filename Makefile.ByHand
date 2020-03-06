# for alternate compiler and/or flags
# SGI IRIX needs "-Wf,-XNl6000" in its DEBUG_FLAGS
# CC          = gcc
# DEBUG_FLAGS = -g


# flags to make X include files happy
# X_CFLAGS = -DSYSV -D_NO_PROTO

# other include flags
INCLUDE_FLAGS = 

# where to find X libraries
LIBDIRS = -L/usr/lib/X11

# extra libraries needed to link X
# SVR4 on 386/486 needs "-lsocket -lnsl -lm -ladm -lgen"
SYSLIBS = 


# normal make commands -- should not need editing below this line


ATHENALIBS = -lXaw -lXmu
XTLIBS     = -lXt
XLIBS      = -lXmu -lXext -lX11


CFLAGS = $(DEBUG_FLAGS) $(X_CFLAGS) $(INCLUDE_FLAGS)
LIBS   = $(LIBDIRS) $(ATHENALIBS) $(XTLIBS) $(XLIBS) $(SYSLIBS)


PROG = xfretboard
OBJS = Fretboard.o xfretboard.o


$(PROG): $(OBJS)
	$(CC) $(OBJS) $(LIBS)  -o $(PROG)

clean:
	rm -f $(OBJS) $(PROG)


Fretboard.o: Fretboard.c Fretboard.h FretboardP.h \
	     lead_cursor lead_cursor_mask chord_cursor chord_cursor_mask
	$(CC) -c $(CFLAGS) Fretboard.c

xfretboard.o: xfretboard.c Fretboard.h XFretboard.h
	$(CC) -c $(CFLAGS) xfretboard.c

XFretboard.h: XFretboard.ad
	./ad2h XFretboard


