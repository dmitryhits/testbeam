## makefile for decode.C
DEBUG       := -g3 -Wall
INCDIR      := include
SRCDIR      := src

ALL_SRC     := $(wildcard src/*.cc)
SOURCES     := $(filter-out decode_make.cc, $(ALL_SRC))
#src/decode_make.cc
OBJECTS     := $(SOURCES:.cc=.o)
EXECUTABLE  := decode
CC          := g++
LDFLAGS     := $(DEBUG)
ROOTLIBS    := $(shell root-config --glibs)
ROOTCFLAGS  := $(shell root-config --cflags)
LIBS        := $(ROOTLIBS)
INCLUDES    := -I./include -I$(ROOTSYS)/include
CXXFLAGS    := -c $(DEBUG) -fPIC -fno-var-tracking -Wno-deprecated -D_GNU_SOURCE -O2 $(INCLUDES) $(ROOTCFLAGS) -DDEV $(DEBUG)

RM          := rm



# Rules ====================================
#all: $(SOURCES) $(EXECUTABLE)
all: decode

decode: $(OBJECTS)
	$ @echo "SOURCES $(SOURCES)"
	$ @echo "OBJECTS $(OBJECTS)"
	$ @echo "@       $@"
	$ @echo "<       $<"
	$ @echo "^       $^"
	$(CC) $(LDFLAGS) -o $@ $^  $(LIBS)

%.o:  $(SRCDIR)/%.cc $(INCLUDEDIR)/%.hh
	$ #$ @echo "CREATING OBJECT $%
	$ $(CC) $(CXXFLAGS) -c $<
## $(EXECUTABLE): $(OBJECTS)
## 	$(CC) $(LDFLAGS) -o $@ $<  $(LIBS)

clean:
	find $(SRCDIR) -name '*.o' -exec $(RM) -v {} ';' 
	$(RM) decode

