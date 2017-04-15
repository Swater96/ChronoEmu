//
// Chrono Emu (C) 2016
//
// Warden System
// Credits TomRus88
//

#ifndef _WARDEN_MAC_H
#define _WARDEN_MAC_H

class WardenMac : WardenBase
{
    public:
        WardenMac();
        ~WardenMac();

        void Init(WorldSession *pClient, BigNumber *K);
        ClientWardenModule *GetModuleForClient(WorldSession *session);
        void InitializeModule();
        void RequestHash();
        void HandleHashResult(ByteBuffer &buff);
        void RequestData();
        void HandleData(ByteBuffer &buff);
};

#endif
