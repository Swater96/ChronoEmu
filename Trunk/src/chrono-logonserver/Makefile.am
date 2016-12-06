INCLUDES += -I$(srcdir) -I$(srcdir)/../../dep/include -I$(srcdir)/../chrono-shared

bin_PROGRAMS = chrono-logonserver

chrono_logonserver_SOURCES = \
	AccountCache.cpp \
	AccountCache.h \
	AuthSocket.cpp \
	AuthSocket.h \
	AuthStructs.h \
	LogonCommServer.cpp \
	LogonCommServer.h \
	LogonConsole.cpp \
	LogonConsole.h \
	LogonOpcodes.cpp \
	LogonOpcodes.h \
	LogonStdAfx.cpp \
	LogonStdAfx.h \
	AutoPatcher.h \
	AutoPatcher.cpp \
	Main.cpp \
	Main.h \
	PeriodicFunctionCall_Thread.h

chrono_logonserver_LDADD = -lshared -lz
chrono_logonserver_LDFLAGS = -L$(srcdir)/../chrono-shared
