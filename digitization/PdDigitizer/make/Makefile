SILENT      ?= @

INCDIR = ../include
SRCDIR = ../src
BINDIR = ../bin
BUILDDIR = ../build
#UTILITIESDIR = ../../utilities
LINKDEFIN := $(wildcard $(INCDIR)/*LinkDef.h)
SOSUFFIX = so

$(shell mkdir -p ${BUILDDIR})
$(shell mkdir -p ${BINDIR})
define __MAKECINT__
endef
#$(shell rootcint -f ${SRCDIR}/PdLibDict.cc -c ${INCDIR}/LinkDef.h)
#$(shell mv ${SRCDIR}/PdLibDict.h ${INCDIR})

TARGET = PdDigitizer
BIN_TARGET = ${BINDIR}/${TARGET}

SRC = $(wildcard ${SRCDIR}/*.cc)
OBJS = $(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,${SRC})
DEPS := $(patsubst $(SRCDIR)/%.cc, $(BUILDDIR)/%.d, $(SRC))
ROOTDICTHEADERS := $(wildcard $(INCDIR)/*.hh)
BUILDDIRCONF := $(BUILDDIR)/.buildconf

LIBNAME = $(TARGET)
SHLIB := $(LIBDIR)/lib$(LIBNAME).$(SOSUFFIX)
AUTOLINKDEF := $(BUILDDIR)/LinkDef_$(LIBNAME).h
PYTHON := python
ROOTSOSUFFIX := so
ROOTDICTCCNAME := $(BUILDDIR)/$(LIBNAME)DICT.C
ROOTDICTHHNAME := $(BUILDDIR)/$(LIBNAME)DICT.h
ROOTDICTOBJECT := $(ROOTDICTCCNAME:.C=.o)
ROOTCINT = rootcint
CC = gcc
XX = g++

# Make dependencies
MAKEDEPEND = rm -f $@; \
	$(CXX) $(CCFLAGS) -MM $(CPPFLAGS) $< > $@.$$$$;                           \
        cp $@.$$$$ $@.temp.$$$$; sed -e 's/\#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
        -e '/^$$/ d' -e 's/$$/ :/' < $@.$$$$ >> $@.temp.$$$$;                         \
	sed 's,\($*\)\.o[ :]*,$(BUILDDIR)/\1.o $@ : ,g' < $@.temp.$$$$ > $@ ;         \
        rm -f $@*.$$$$;
#
CPPFLAGS += -I$(ROOTSYS)/include -pipe -Wall -O -g
CPPFLAGS += -I$(INCDIR)
LDLIBS:= $(shell root-config --libs)

#
${BIN_TARGET}:$(BUILDDIR)/$(TARGET).o $(OBJS)
	$(XX) $(LDLIBS) $(CPPFLAGS) -o $(BIN_TARGET) $(BUILDDIR)/$(TARGET).o $(OBJS)
	chmod a+x $(BIN_TARGET)
#
$(BUILDDIR)/$(TARGET).o : ../$(TARGET).cc
	$(SILENT)echo "Compiling file............. $(<F)"
	$(XX) $(LDLIBS) $(CPPFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc 
	$(SILENT)echo "Compiling file............. $(<F)"
	$(XX) $(LDLIBS) $(CPPFLAGS) -c $< -o $@

.PHONY:clean
clean:
	rm -rf ${OBJS} $(BIN_TARGET) $(BUILDDIR)/$(TARGET).o
