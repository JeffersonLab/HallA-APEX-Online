.DELETE_ON_ERROR:

ROOTCONFIG  := root-config
ROOTCFLAGS  := $(shell $(ROOTCONFIG) --cflags)
ROOTLDFLAGS := $(shell $(ROOTCONFIG) --ldflags)
ROOTGLIBS   := $(shell $(ROOTCONFIG) --glibs)

CXX         := g++
CXXFLAGS    := -g -O2 -Wall -fPIC $(ROOTCFLAGS)
LD          := g++
LDFLAGS     := -g -O2 -Wall $(ROOTLDFLAGS)

LIBS        := $(ROOTGLIBS)
DEFINES     := -DWITHRINT

SRC_FILES   := $(wildcard *.cxx)
HDR_FILES   := eventdisplay.h def.h 
OBJ_FILES   := $(SRC_FILES:%.cxx=%.o)
EXE_FILES   := program 

.PHONY: all clean

all: $(EXE_FILES)


$(EXE_FILES): $(OBJ_FILES) 
	$(LD) $(LDFLAGS) $(OBJ_FILES) $(LIBS) -o $@

%.o: %.cxx ${HDR_FILES}
	$(CXX) $(CXXFLAGS) $(DEFINES) -c $< -o $@


clean:
	rm -f $(OBJ_FILES) $(EXE_FILES)
