#
# $Id: Makefile.am 2048 2014-03-06 13:45:59Z manfred-e $
#
if DEBUG
DBGFLAGS = -g -DDEBUG
endif

MLIBPATH = "MODULE_LIBRARY_PATH=\"$(libdir)/saga\""
MSHAREPATH = "SHARE_PATH=\"$(prefix)/share/saga\""
BASEFLAGS = -D_SAGA_LINUX -D_TYPEDEF_BYTE -D_TYPEDEF_WORD $(DBGFLAGS) -I.. -D$(MLIBPATH) -D$(MSHAREPATH) 
if SAGA_UNICODE
AM_CXXFLAGS = -fPIC `wx-config --unicode=yes --cxxflags` $(BASEFLAGS) -D_SAGA_UNICODE $(GOMPFLAGS)
AM_LDFLAGS = -fPIC `wx-config --unicode=yes --libs` -lm
else
AM_CXXFLAGS = -fPIC `wx-config --unicode=no --cxxflags` $(BASEFLAGS) $(GOMPFLAGS)
AM_LDFLAGS = -fPIC `wx-config --unicode=no --libs` -lm
endif

bin_PROGRAMS = saga_cmd
saga_cmd_LDADD = ../saga_api/libsaga_api.la
saga_cmd_SOURCES =\
callback.cpp\
module_library.cpp\
saga_cmd.cpp\
callback.h\
module_library.h

SUBDIRS = man
