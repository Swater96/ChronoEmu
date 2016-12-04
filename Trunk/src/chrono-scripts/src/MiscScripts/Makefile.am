INCLUDES = -I$(top_srcdir)/src/chrono-logonserver \
   -I$(top_srcdir)/src/chrono-realmserver -I$(top_srcdir)/src/chrono-shared -I$(top_srcdir)/src/chrono-voicechat \
   -I$(top_srcdir)/src/chrono-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../chrono-shared/g3dlite -I$(srcdir)/../../../chrono-shared/vmap

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libSunMiscScripts.la

libSunMiscScripts_la_SOURCES = 
Creatures.cpp \
GameObjects.cpp \
RandomScripts.cpp \
DarkmoonFaireObjects.cpp \
Setup.cpp
