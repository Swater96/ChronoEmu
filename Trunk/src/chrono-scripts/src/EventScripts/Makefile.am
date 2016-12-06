INCLUDES = -I$(top_srcdir)/src/chrono-logonserver \
   -I$(top_srcdir)/src/chrono-realmserver -I$(top_srcdir)/src/chrono-shared -I$(top_srcdir)/src/chrono-voicechat \
   -I$(top_srcdir)/src/chrono-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src -I../Common
INCLUDES += -I$(srcdir)/../../../chrono-shared/g3dlite -I$(srcdir)/../../../chrono-shared/vmap

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libSunEventScripts.la

libSunEventScripts_la_SOURCES = Setup.cpp \
../Common/Base.cpp \
Halloween.cpp \
WinterVeil.cpp \
Event_DarkPortal.cpp
