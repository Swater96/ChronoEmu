//
// Chrono Emu (C) 2016
//
// Warden System
// Credits TomRus88
//

#include "StdAfx.h"

CWardenDataStorage::CWardenDataStorage()
{
    InternalDataID = 1;
}

CWardenDataStorage::~CWardenDataStorage()
{
    std::map<uint32, WardenData*>::iterator itr1 = _data_map.begin();
    for (; itr1 != _data_map.end(); ++itr1)
        delete itr1->second;

    std::map<uint32, WardenDataResult*>::iterator itr2 = _result_map.begin();
    for (; itr2 != _result_map.end(); ++itr2)
        delete itr2->second;
}

void CWardenDataStorage::Init()
{
    LoadWardenDataResult();
}

void CWardenDataStorage::LoadWardenDataResult()
{
    QueryResult *result = WorldDatabase.Query("SELECT `check`, `data`, `result`, `address`, `length`, `str` FROM warden_data_result");

    uint32 count = 0;

    if (!result)
    {
        sLog.outString("World: %u Warden Entries Loaded", count);
        return;
    }

    result->GetRowCount();
	if (result)
    do
    {
        ++count;

        Field *fields = result->Fetch();

        uint8 type = fields[0].GetUInt8();

        uint32 id = GenerateInternalDataID();
        WardenData *wd = new WardenData();
        wd->Type = type;

        if (type == PAGE_CHECK_A || type == PAGE_CHECK_B || type == DRIVER_CHECK)
        {
            std::string data = fields[1].GetString();
            wd->i.SetHexStr(data.c_str());
            int len = data.size() / 2;
            if (wd->i.GetNumBytes() < len)
            {
                uint8 temp[24];
                memset(temp, 0, len);
                memcpy(temp, wd->i.AsByteArray(), wd->i.GetNumBytes());
                std::reverse(temp, temp + len);
                wd->i.SetBinary((uint8*)temp, len);
            }
        }

        if (type == MEM_CHECK || type == MODULE_CHECK)
            MemCheckIds.push_back(id);

        if (type == MEM_CHECK || type == PAGE_CHECK_A || type == PAGE_CHECK_B || type == PROC_CHECK)
        {
            wd->Address = fields[3].GetUInt32();
            wd->Length = fields[4].GetUInt8();
        }

        // PROC_CHECK support missing
        if (type == MEM_CHECK || type == MPQ_CHECK || type == LUA_STR_CHECK || type == DRIVER_CHECK || type == MODULE_CHECK)
            wd->str = fields[5].GetString();

        _data_map[id] = wd;

        if (type == MPQ_CHECK || type == MEM_CHECK)
        {
            std::string result = fields[2].GetString();
            WardenDataResult *wr = new WardenDataResult();
            wr->res.SetHexStr(result.c_str());
            int len = result.size() / 2;
            if (wr->res.GetNumBytes() < len)
            {
                uint8 *temp = new uint8[len];
                memset(temp, 0, len);
                memcpy(temp, wr->res.AsByteArray(), wr->res.GetNumBytes());
                std::reverse(temp, temp + len);
                wr->res.SetBinary((uint8*)temp, len);
                delete [] temp;
            }
            _result_map[id] = wr;
        }
    } while (result->NextRow());

    delete result;

    sLog.outString("World: %u Warden Entries Loaded", count);
}

WardenData *CWardenDataStorage::GetWardenDataById(uint32 Id)
{
    std::map<uint32, WardenData*>::const_iterator itr = _data_map.find(Id);
    if (itr != _data_map.end())
        return itr->second;
    return nullptr;
}

WardenDataResult *CWardenDataStorage::GetWardenResultById(uint32 Id)
{
    std::map<uint32, WardenDataResult*>::const_iterator itr = _result_map.find(Id);
    if (itr != _result_map.end())
        return itr->second;
    return nullptr;
}
