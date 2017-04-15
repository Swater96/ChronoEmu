//
// Chrono Emu (C) 2016
//
// Warden System
// Credits TomRus88
//

#ifndef _WARDEN_BASE_H
#define _WARDEN_BASE_H

enum WardenOpcodes
{
    // Client->Server
    WARDEN_CMSG_MODULE_MISSING                  = 0,
    WARDEN_CMSG_MODULE_OK                       = 1,
    WARDEN_CMSG_CHEAT_CHECKS_RESULT             = 2,
    WARDEN_CMSG_MEM_CHECKS_RESULT               = 3,        // only sent if MEM_CHECK bytes doesn't match
    WARDEN_CMSG_HASH_RESULT                     = 4,
    WARDEN_CMSG_MODULE_FAILED                   = 5,        // this is sent when client failed to load uploaded module due to cache fail

    // Server->Client
    WARDEN_SMSG_MODULE_USE                      = 0,
    WARDEN_SMSG_MODULE_CACHE                    = 1,
    WARDEN_SMSG_CHEAT_CHECKS_REQUEST            = 2,
    WARDEN_SMSG_MODULE_INITIALIZE               = 3,
    WARDEN_SMSG_MEM_CHECKS_REQUEST              = 4,        // byte len; whole(!EOF) { byte unk(1); byte index(++); string module(can be 0); int offset; byte len; byte[] bytes_to_compare[len]; }
    WARDEN_SMSG_HASH_REQUEST                    = 5
};

#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

struct WardenModuleUse
{
    uint8 Command;
    uint8 Module_Id[16];
    uint8 Module_Key[16];
    uint32 Size;
};

struct WardenModuleTransfer
{
    uint8 Command;
    uint16 DataSize;
    uint8 Data[500];
};

struct WardenHashRequest
{
    uint8 Command;
    uint8 Seed[16];
};

#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

struct ClientWardenModule
{
    uint8 ID[16];
    uint8 Key[16];
    uint32 CompressedSize;
    uint8 *CompressedData;
};

class WardenBase
{
    friend class WardenWin;
    friend class WardenMac;

    public:
        WardenBase();
        ~WardenBase();

        virtual void Init(WorldSession *pClient, BigNumber *K);
        virtual ClientWardenModule *GetModuleForClient(WorldSession *session);
        virtual void InitializeModule();
        virtual void RequestHash();
        virtual void HandleHashResult(ByteBuffer &buff);
        virtual void RequestData();
        virtual void HandleData(ByteBuffer &buff);

        void SendModuleToClient();
        void RequestModule();
        void Update();
        void DecryptData(uint8 *Buffer, uint32 Len);
        void EncryptData(uint8 *Buffer, uint32 Len);

        static void PrintHexArray(const char *Before, const uint8 *Buffer, uint32 Len, bool BreakWithNewline);
        static bool IsValidCheckSum(uint32 checksum, const uint8 *Data, const uint16 Length);
        static uint32 BuildChecksum(const uint8 *data, uint32 dataLen);

    private:
        WorldSession *Client;
        uint8 InputKey[16];
        uint8 OutputKey[16];
        uint8 Seed[16];
        SARC4 iCrypto;
        SARC4 oCrypto;
        uint32 m_WardenCheckTimer;                          // timer between data packets
        bool m_WardenDataSent;
        uint32 m_WardenKickTimer;                           // time after send packet
        uint32 m_WardenTimer;
		uint32 _previousTimestamp;
        ClientWardenModule *Module;
        bool m_initialized;
};

#endif
