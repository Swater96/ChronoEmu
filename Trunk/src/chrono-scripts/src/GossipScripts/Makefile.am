INCLUDES = -I$(top_srcdir)/src/chrono-logonserver \
   -I$(top_srcdir)/src/chrono-realmserver -I$(top_srcdir)/src/chrono-shared -I$(top_srcdir)/src/chrono-voicechat \
   -I$(top_srcdir)/src/chrono-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../chrono-shared/g3dlite -I$(srcdir)/../../../chrono-shared/vmap

AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libSunGossipScripts.la

libSunGossipScripts_la_SOURCES = \
Gossip_Battlemaster.cpp \
Gossip_Innkeepers.cpp \
Gossip_Guard.cpp \
Gossip_Trainer.cpp \
Gossip_Mulgore.cpp \
Gossip_Tanaris.cpp \
Gossip_Stormwind.cpp\
Gossip_Moonglade.cpp\
Gossip_DarkmoonFaire.cpp\
Gossip_Theramore.cpp\
Gossip_Teldrassil.cpp\
Setup.cpp
