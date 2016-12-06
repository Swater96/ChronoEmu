INCLUDES += -I$(srcdir) -I$(srcdir)/../chrono-shared

bin_PROGRAMS = chrono-realmserver

chrono_realmserver_SOURCES = \
ClientManager.h \
ClusterManager.h \
LogonCommClient.h \
LogonCommHandler.h \
Master.h \
ObjectStorage.h \
RStdAfx.h \
Session.h \
Structures.h \
WorkerOpcodes.h \
WorkerServer.h \
WorkerServerSocket.h \
WorldSocket.h \
CharacterHandler.cpp \
ClientManager.cpp \
ClusterManager.cpp \
LogonCommClient.cpp \
LogonCommHandler.cpp \
Master.cpp \
ObjectStorage.cpp \
QueryHandler.cpp \
RStdAfx.cpp \
Session.cpp \
WorkerServer.cpp \
WorkerServerSocket.cpp \
WorldSocket.cpp 
	
chrono_realmserver_LDADD = -lshared -lz

chrono_realmserver_LDFLAGS = -L$(srcdir)/../chrono-shared \
               -L/usr/lib
               
