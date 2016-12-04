INCLUDES = -I$(top_srcdir)/src/chrono-logonserver \
   -I$(top_srcdir)/src/chrono-realmserver -I$(top_srcdir)/src/chrono-shared -I$(top_srcdir)/src/chrono-voicechat \
   -I$(top_srcdir)/src/chrono-world -I$(srcdir) -I$(srcdir)/../../../../dep/include \
   -I$(srcdir)/../../../../dep/src -I$(srcdir)/../../../../src
INCLUDES += -I$(srcdir)/../../../chrono-shared/g3dlite -I$(srcdir)/../../../chrono-shared/vmap
AM_CXXFLAGS = -DSCRIPTLIB

lib_LTLIBRARIES = libSpellHandlers.la

libSpellHandlers_la_SOURCES = DruidSpells.cpp HunterSpells.cpp ItemSpells_1.cpp \
MageSpells.cpp PaladinSpells.cpp PriestSpells.cpp OldSpellCPPStuff.cpp RogueSpells.cpp \
ShamanSpells.cpp WarlockSpells.cpp WarriorSpells.cpp QIspells.cpp Setup.cpp
