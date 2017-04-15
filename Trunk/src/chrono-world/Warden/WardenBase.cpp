//
// Chrono Emu (C) 2016
//
// Warden System
// Credits TomRus88
//

#include "StdAfx.h"

#define IN_MILLISECONDS 1000

WardenBase::WardenBase() : iCrypto(16), oCrypto(16), m_WardenCheckTimer(10000/*10 sec*/), m_WardenKickTimer(0), m_WardenDataSent(false), m_initialized(false)
{
}

WardenBase::~WardenBase()
{
    delete[] Module->CompressedData;
    delete Module;
    Module = nullptr;
    m_initialized = false;
}

void WardenBase::Init(WorldSession *pClient, BigNumber *K)
{
    ASSERT(false);
}

ClientWardenModule *WardenBase::GetModuleForClient(WorldSession *session)
{
    ASSERT(false);
    return nullptr;
}

void WardenBase::InitializeModule()
{
    ASSERT(false);
}

void WardenBase::RequestHash()
{
    ASSERT(false);
}

void WardenBase::HandleHashResult(ByteBuffer &buff)
{
    ASSERT(false);
}

void WardenBase::RequestData()
{
    ASSERT(false);
}

void WardenBase::HandleData(ByteBuffer &buff)
{
    ASSERT(false);
}

void WardenBase::SendModuleToClient()
{
    sLog.outString("Send module to client");

    // Create packet structure
    WardenModuleTransfer pkt;

    uint32 size_left = Module->CompressedSize;
    uint32 pos = 0;
    uint16 burst_size;
    while (size_left > 0)
    {
        burst_size = size_left < 500 ? size_left : 500;
        pkt.Command = WARDEN_SMSG_MODULE_CACHE;
        pkt.DataSize = burst_size;
        memcpy(pkt.Data, &Module->CompressedData[pos], burst_size);
        size_left -= burst_size;
        pos += burst_size;

        EncryptData((uint8*)&pkt, burst_size + 3);
        WorldPacket pkt1(SMSG_WARDEN_DATA, burst_size + 3);
        pkt1.append((uint8*)&pkt, burst_size + 3);
        Client->SendPacket(&pkt1);
    }
}

void WardenBase::RequestModule()
{
    sLog.outString("Request module");

    // Create packet structure
    WardenModuleUse Request;
    Request.Command = WARDEN_SMSG_MODULE_USE;

    memcpy(Request.Module_Id, Module->ID, 16);
    memcpy(Request.Module_Key, Module->Key, 16);
    Request.Size = Module->CompressedSize;

    // Encrypt with warden RC4 key.
    EncryptData((uint8*)&Request, sizeof(WardenModuleUse));

    WorldPacket pkt(SMSG_WARDEN_DATA, sizeof(WardenModuleUse));
    pkt.append((uint8*)&Request, sizeof(WardenModuleUse));
    Client->SendPacket(&pkt);
}

void WardenBase::Update()
{
	if (m_initialized)
	{
		uint32 currentTimestamp = getMSTime();
		uint32 diff = currentTimestamp - _previousTimestamp;
		_previousTimestamp = currentTimestamp;

		if (m_WardenDataSent)
		{
			// 1.5 minutes after send packet
			if ((m_WardenKickTimer > 90 * IN_MILLISECONDS) && sWorld.m_WardenEnabled)
				Client->GetPlayer()->Kick();
			else
				m_WardenKickTimer += diff;
		}
		else if (m_WardenCheckTimer > 0)
		{
			if (diff >= m_WardenCheckTimer)
				RequestData();
			else
				m_WardenCheckTimer -= diff;
		}
	}
}

void WardenBase::DecryptData(uint8 *Buffer, uint32 Len)
{
    iCrypto.UpdateData(Len, Buffer);
}

void WardenBase::EncryptData(uint8 *Buffer, uint32 Len)
{
    oCrypto.UpdateData(Len, Buffer);
}

void WardenBase::PrintHexArray(const char *Before, const uint8 *Buffer, uint32 Len, bool BreakWithNewline)
{
    printf(Before);
    for (uint32 i = 0; i < Len; ++i)
        printf("%02X ", Buffer[i]);
    if (BreakWithNewline)
        printf("\n");
}

bool WardenBase::IsValidCheckSum(uint32 checksum, const uint8 *Data, const uint16 Length)
{
    uint32 newchecksum = BuildChecksum(Data, Length);

    if (checksum != newchecksum)
    {
        sLog.outString("CHECKSUM IS NOT VALID");
        return false;
    }
    else
    {
        sLog.outString("CHECKSUM IS VALID");
        return true;
    }
}

uint32 WardenBase::BuildChecksum(const uint8* data, uint32 dataLen)
{
    uint8 hash[20];
    SHA1(data, dataLen, hash);
    uint32 checkSum = 0;
    for (uint8 i = 0; i < 5; ++i)
        checkSum = checkSum ^ *(uint32*)(&hash[0] + i * 4);
    return checkSum;
}

void WorldSession::HandleWardenDataOpcode(WorldPacket & recv_data)
{
    m_Warden->DecryptData(const_cast<uint8*>(recv_data.contents()), recv_data.size());
    uint8 Opcode;
    recv_data >> Opcode;
    sLog.outString("Got packet, opcode %02X, size %u", Opcode, recv_data.size());
    recv_data.hexlike();

    switch(Opcode)
    {
        case WARDEN_CMSG_MODULE_MISSING:
            m_Warden->SendModuleToClient();
            break;
        case WARDEN_CMSG_MODULE_OK:
            m_Warden->RequestHash();
            break;
        case WARDEN_CMSG_CHEAT_CHECKS_RESULT:
            m_Warden->HandleData(recv_data);
            break;
        case WARDEN_CMSG_MEM_CHECKS_RESULT:
            sLog.outString("NYI WARDEN_CMSG_MEM_CHECKS_RESULT received!");
            break;
        case WARDEN_CMSG_HASH_RESULT:
            m_Warden->HandleHashResult(recv_data);
            m_Warden->InitializeModule();
            break;
        case WARDEN_CMSG_MODULE_FAILED:
            sLog.outString("NYI WARDEN_CMSG_MODULE_FAILED received!");
            break;
        default:
            sLog.outString("Got unknown warden opcode %02X of size %u.", Opcode, recv_data.size() - 1);
            break;
    }
}
