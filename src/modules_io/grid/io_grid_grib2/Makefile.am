#
# $Id: Makefile.am,v 1.2 2007/10/23 11:12:17 oconrad Exp $
#
if DEBUG
DBGFLAGS = -g -DDEBUG
endif
if SAGA_UNICODE
UC_DEFS = -D_SAGA_UNICODE
endif
DEP_LIBS = -lgrib2c
DEF_SAGA           = -D_SAGA_LINUX -D_TYPEDEF_BYTE -D_TYPEDEF_WORD -DUSE_JPEG2000 -DUSE_PNG
CXX_INCS           = -I$(top_srcdir)/src/saga_core
AM_CXXFLAGS        = -fPIC $(CXX_INCS) $(DEF_SAGA) $(UC_DEFS) $(DBGFLAGS)
#AM_LDFLAGS         = -fPIC -shared -lpq -ljasper
AM_LDFLAGS         = -fPIC -shared -ljasper
pkglib_LTLIBRARIES = libio_grid_grib2.la
libio_grid_grib2_la_SOURCES =\
grib2_import.cpp\
MLB_Interface.cpp\
grib2_import.h\
MLB_Interface.h

libio_grid_grib2_la_LIBADD = $(top_srcdir)/src/saga_core/saga_api/libsaga_api.la


