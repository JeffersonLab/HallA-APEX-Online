SHELL           = /bin/sh

FOR             = g77 -c -fPIC -shared

SOURCES         = fpp_align.f fpp_last.f fpp_set.f fpp_trackit.f fpp_trk4.f fpp_align_corr.f
OBJECTS         = $(SOURCES:.f=.o)

CERN_LIBS       = -lmathlib -lgraflib -lgrafX11 -lpacklib

ifeq ($(OSTYPE),linux)
CERN_ROOT       = /site/cernlib/i386_rhel5/2005
CERN_LIBRARY    = $(CERN_ROOT)/lib
FFLAGS  = -q -B88 -B108 -B111 -N2 -N22 -N90 -f -c
LFLAGS  = -B108 -L$(CERN_LIBRARY)/ -L/usr/X11R6/lib -L/usr/lib
LIBS            = $(CERN_LIBS) $(FLIBS) -lX11 -lm -L/usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/ -lg2c
EXE             = fpp_align_linux.o
endif

$(EXE): $(OBJECTS)
	$(FOR)  $^ -o $@ $(LFLAGS) $(LIBS)
.f.o:
	$(FOR)  $^ -static -o $@ $(LFLAGS) $(LIBS)
build: $(SOURCES) $(OBJECTS)
	$(FOR)  $^ -static -o $@ $(LFLAGS) $(LIBS)
clean:
	rm -f *.o

