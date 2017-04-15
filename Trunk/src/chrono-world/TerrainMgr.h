//
// MaNGoS Maps Support
// Credits: MaNGoS Team & Andy from AscentEmu
//

#ifndef _TERRAIN_H
#define _TERRAIN_H

#define TERRAIN_INVALID_HEIGHT -50000
#define TERRAIN_TILE_SIZE 533.3333333f
#define TERRAIN_NUM_TILES 64
#define TERRAIN_MAP_RESOLUTION 128

class TerrainHolder;
class TerrainTile;

#define MAP_AREA_NO_AREA      0x0001

#define MAP_HEIGHT_NO_HEIGHT  0x0001
#define MAP_HEIGHT_AS_INT16   0x0002
#define MAP_HEIGHT_AS_INT8    0x0004

#define MAP_LIQUID_NO_TYPE    0x0001
#define MAP_LIQUID_NO_HEIGHT  0x0002


struct TileMapHeader
{
	uint32 mapMagic;
	uint32 versionMagic;
	uint32 buildMagic;
	uint32 areaMapOffset;
	uint32 areaMapSize;
	uint32 heightMapOffset;
	uint32 heightMapSize;
	uint32 liquidMapOffset;
	uint32 liquidMapSize;
	uint32 holesOffset;
	uint32 holesSize;
};

struct TileMapAreaHeader
{
	uint32 fourcc;
	uint16 flags;
	uint16 gridArea;
};

struct TileMapHeightHeader
{
	uint32 fourcc;
	uint32 flags;
	float gridHeight;
	float gridMaxHeight;
};

struct TileMapLiquidHeader
{
	uint32 fourcc;
	uint16 flags;
	uint16 liquidType;
	uint8 offsetX;
	uint8 offsetY;
	uint8 width;
	uint8 height;
	float liquidLevel;
};

class TileMap
{
public:

	//Area Map
	uint16 m_area;
	uint16* m_areaMap;

	//Height Map
	union
	{
		float* m_heightMap8F;
		uint16* m_heightMap8S;
		uint8* m_heightMap8B;
	};
	union
	{
		float* m_heightMap9F;
		uint16* m_heightMap9S;
		uint8* m_heightMap9B;
	};
	uint32 m_heightMapFlags;
	float m_heightMapMult;
	float m_tileHeight;

	//Liquid Map
	uint8* m_liquidType;
	float* m_liquidMap;
	float m_liquidLevel;
	uint8 m_liquidOffX;
	uint8 m_liquidOffY;
	uint8 m_liquidHeight;
	uint8 m_liquidWidth;
	uint16 m_defaultLiquidType;

	TileMap()
	{
		m_areaMap = nullptr;
		m_area = 0;
		m_tileHeight = 0;
		m_heightMapFlags = 0;
		m_heightMap8F = nullptr;
		m_heightMap9F = nullptr;
		m_heightMapMult = 1;

		m_liquidType = nullptr;
		m_liquidMap = nullptr;
		m_liquidLevel = 0;
		m_liquidOffX = 0;
		m_liquidOffY = 0;
		m_liquidHeight = 0;
		m_liquidWidth = 0;
		m_defaultLiquidType = 0;
	}

	~TileMap()
	{
		delete[] m_areaMap;
		delete[] m_heightMap8F;
		delete[] m_heightMap9F;

		delete[] m_liquidType;
		delete[] m_liquidMap;
	}


	void Load(char* filename);

	void LoadLiquidData(FILE* f, TileMapHeader &header);
	void LoadHeightData(FILE* f, TileMapHeader &header);
	void LoadAreaData(FILE* f, TileMapHeader &header);

	float GetHeight(float x, float y);
	float GetHeightB(float x, float y, int x_int, int y_int);
	float GetHeightS(float x, float y, int x_int, int y_int);
	float GetHeightF(float x, float y, int x_int, int y_int);

	float GetLiquidHeight(float x, float y);
	uint8 GetLiquidType(float x, float y);

	uint32 GetArea(float x, float y);
};

class TerrainTile
{
public:
	Chrono::Threading::AtomicCounter m_refs;

	TerrainHolder* m_parent;
	uint32 m_mapid;
	int32 m_tx;
	int32 m_ty;

	//Children
	TileMap m_map;

	TerrainTile(TerrainHolder* parent, uint32 mapid, int32 x, int32 y);
	~TerrainTile();
	void AddRef() { ++m_refs; }
	void DecRef() { if (--m_refs == 0) delete this; }

	void Load()
	{
		char filename[1024];

		//Normal map stuff
		sprintf(filename, "maps/%03u%02u%02u.map", m_mapid, m_tx, m_ty);
		m_map.Load(filename);
	}
};

class TerrainHolder
{
public:
	uint32 m_mapid;
	TerrainTile* m_tiles[TERRAIN_NUM_TILES][TERRAIN_NUM_TILES];
	FastMutex m_lock[TERRAIN_NUM_TILES][TERRAIN_NUM_TILES];
	Chrono::Threading::AtomicCounter m_tilerefs[TERRAIN_NUM_TILES][TERRAIN_NUM_TILES];

	TerrainHolder(uint32 mapid)
	{
		for (int32 i = 0; i < TERRAIN_NUM_TILES; ++i)
			for (int32 j = 0; j < TERRAIN_NUM_TILES; ++j)
				m_tiles[i][j] = nullptr;
		m_mapid = mapid;
	}

	~TerrainHolder()
	{
		for (int32 i = 0; i < TERRAIN_NUM_TILES; ++i)
			for (int32 j = 0; j < TERRAIN_NUM_TILES; ++j)
				UnloadTile(i, j);
	}


	TerrainTile* GetTile(float x, float y);
	TerrainTile* GetTile(int32 tx, int32 ty)
	{
		TerrainTile* rv = nullptr;
		m_lock[tx][ty].Acquire();
		rv = m_tiles[tx][ty];
		if (rv != nullptr)
			rv->AddRef();
		m_lock[tx][ty].Release();

		return rv;
	}

	void LoadTile(float x, float y)
	{
		int32 tx = (int32)(32 - (x / TERRAIN_TILE_SIZE));
		int32 ty = (int32)(32 - (y / TERRAIN_TILE_SIZE));
		LoadTile(tx, ty);
	}
	void LoadTile(int32 tx, int32 ty)
	{
		m_lock[tx][ty].Acquire();
		++m_tilerefs[tx][ty];
		if (m_tiles[tx][ty] == nullptr)
		{
			m_tiles[tx][ty] = new TerrainTile(this, m_mapid, tx, ty);
			m_tiles[tx][ty]->Load();
		}
		m_lock[tx][ty].Release();
	}
	void UnloadTile(float x, float y)
	{
		int32 tx = (int32)(32 - (x / TERRAIN_TILE_SIZE));
		int32 ty = (int32)(32 - (y / TERRAIN_TILE_SIZE));
		UnloadTile(tx, ty);
	}

	void UnloadTile(int32 tx, int32 ty)
	{
		m_lock[tx][ty].Acquire();
		if (m_tiles[tx][ty] == nullptr)
		{
			m_lock[tx][ty].Release();
			return;
		}
		m_lock[tx][ty].Release();

		if (--m_tilerefs[tx][ty] == 0)
		{
			m_lock[tx][ty].Acquire();
			if (m_tiles[tx][ty] != nullptr)
				m_tiles[tx][ty]->DecRef();
			m_tiles[tx][ty] = nullptr;
			m_lock[tx][ty].Release();
		}
	}

	//test
	float GetLandHeight(float x, float y)
	{
		TerrainTile* tile = GetTile(x, y);

		if (tile == nullptr)
			return TERRAIN_INVALID_HEIGHT;
		float rv = tile->m_map.GetHeight(x, y);
		tile->DecRef();
		return rv;
	}

	float GetLiquidHeight(float x, float y)
	{
		TerrainTile* tile = GetTile(x, y);

		if (tile == nullptr)
			return TERRAIN_INVALID_HEIGHT;
		float rv = tile->m_map.GetLiquidHeight(x, y);
		tile->DecRef();
		return rv;
	}

	uint8 GetLiquidType(float x, float y)
	{
		TerrainTile* tile = GetTile(x, y);

		if (tile == nullptr)
			return 0;
		uint8 rv = tile->m_map.GetLiquidType(x, y);
		tile->DecRef();
		return rv;
	}

	uint32 GetAreaFlag(float x, float y)
	{
		TerrainTile* tile = GetTile(x, y);

		if (tile == nullptr)
			return 0;
		uint32 rv = tile->m_map.GetArea(x, y);
		tile->DecRef();
		return rv;
	}
};

#endif