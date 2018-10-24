# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:42 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := MTASSimulation
G4TARGET := $(name)
G4EXLIB := true
G4VIS_USE := false
G4DEBUG := false

# source ~/geant4.10.03.p01-install/share/Geant4-10.3.1/geant4make/geant4make.sh

ifndef G4INSTALL
  G4INSTALL = ~/Geant4/geant4.10.04.p02-install/share/Geant4-10.4.2/geant4make
endif

.PHONY: all
all: lib bin

CPPFLAGS += -I$(shell $(ROOTSYS)/bin/root-config --incdir)
include $(G4INSTALL)/config/architecture.gmk

#source ~/root-install/bin/thisroot.sh

EXTRALIBS += $(shell $(ROOTSYS)/bin/root-config --libs)
#LDLIBS += $(shell $(ROOTSYS)/usr/bin/root-config --libs) 
LDFLAGS += `root-config --libs`

include $(G4INSTALL)/config/binmake.gmk
