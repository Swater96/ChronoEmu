#include "StdAfx.h"
#include "LUAEngine.h"
#include "LUAFunctions.h"
#include <ScriptSetup.h>
#pragma warning(disable:4244)

#if PLATFORM != PLATFORM_WIN32
#include <dirent.h>
#endif

bool lua_is_starting_up = false;
bool questexist;
ScriptMgr * m_scriptMgr;
LuaEngineMgr g_luaMgr;
LuaEngine g_engine;

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_SCRIPT_ENGINE | SCRIPT_TYPE_SCRIPT_ENGINE_LUA;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
	m_scriptMgr = mgr;
	g_luaMgr.Startup();
}
extern "C" SCRIPT_DECL void _exp_script_restart(ScriptMgr * mgr)
{
	//m_scriptMgr = mgr;
	printf("_exp_script_restart called. \n");
	g_luaMgr.Unload();
}

template<typename T> const char * GetTClassName() { return "UNKNOWN"; }

template<>
const char * GetTClassName<Unit>()
{
	return "Unit";
}

template<>
const char * GetTClassName<Item>()
{
	return "Item";
}

template<>
const char * GetTClassName<GameObject>()
{
	return "GameObject";
}

template<typename T>
struct RegType
{
	const char * name;
	int(*mfunc)(lua_State*,shared_ptr<T>);
};


/************************************************************************/
/* SCRIPT FUNCTION TABLES                                               */
/************************************************************************/
#include "NameSpaces\FunctionTables.cpp"


template<typename T> RegType<T>* GetMethodTable() { return NULL; }
template<>
RegType<Unit>* GetMethodTable<Unit>() { return UnitMethods; }

template<>
RegType<Item>* GetMethodTable<Item>() { return ItemMethods; }

template<>
RegType<GameObject>* GetMethodTable<GameObject>() { return GOBMethods; }

template <typename T> class Lunar {
	typedef struct { T *pT; } userdataTypePtr;
public:
	typedef int (*mfp)(lua_State *L, T* ptr);
	typedef struct { const char *name; mfp mfunc; } RegType;

	static void Register(lua_State *L) {
		lua_newtable(L);
		int methods = lua_gettop(L);

		luaL_newmetatable(L, GetTClassName<T>());
		int metatable = lua_gettop(L);

		// store method table in globals so that
		// scripts can add functions written in Lua.
		lua_pushvalue(L, methods);
		set(L, LUA_GLOBALSINDEX, GetTClassName<T>());

		// hide metatable from Lua getmetatable()
		lua_pushvalue(L, methods);
		set(L, metatable, "__metatable");

		lua_pushvalue(L, methods);
		set(L, metatable, "__index");

		lua_pushcfunction(L, tostring_T);
		set(L, metatable, "__tostring");

		lua_pushcfunction(L, gc_T);
		set(L, metatable, "__gc");

		lua_newtable(L);                // mt for method table
		lua_pushcfunction(L, new_T);
		lua_pushvalue(L, -1);           // dup new_T function
		set(L, methods, "new");         // add new_T to method table
		set(L, -3, "__call");           // mt.__call = new_T
		lua_setmetatable(L, methods);

		// fill method table with methods from class T
		for (RegType *l = ((RegType*)GetMethodTable<T>()); l->name; l++) {
			lua_pushstring(L, l->name);
			lua_pushlightuserdata(L, (void*)l);
			lua_pushcclosure(L, thunk, 1);
			lua_settable(L, methods);
		}

		lua_pop(L, 2);  // drop metatable and method table
	}

	// call named lua method from userdata method table
	static int call(lua_State *L, const char *method,
		int nargs=0, int nresults=LUA_MULTRET, int errfunc=0)
	{
		int base = lua_gettop(L) - nargs;  // userdata index
		if (!luaL_checkudata(L, base, T::className)) {
			lua_settop(L, base-1);           // drop userdata and args
			lua_pushfstring(L, "not a valid %s userdata", T::className);
			return -1;
		}

		lua_pushstring(L, method);         // method name
		lua_gettable(L, base);             // get method from userdata
		if (lua_isnil(L, -1)) {            // no method?
			lua_settop(L, base-1);           // drop userdata and args
			lua_pushfstring(L, "%s missing method '%s'", T::className, method);
			return -1;
		}
		lua_insert(L, base);               // put method under userdata, args

		int status = lua_pcall(L, 1+nargs, nresults, errfunc);  // call method
		if (status) {
			const char *msg = lua_tostring(L, -1);
			if (msg == NULL) msg = "(error with no message)";
			lua_pushfstring(L, "%s:%s status = %d\n%s",
				T::className, method, status, msg);
			lua_remove(L, base);             // remove old message
			return -1;
		}
		return lua_gettop(L) - base + 1;   // number of results
	}

	// push onto the Lua stack a userdata containing a pointer to T object
	static int push(lua_State *L, shared_ptr<T> obj, bool gc=false) {
		if (!obj) { lua_pushnil(L); return 0; }
		luaL_getmetatable(L, GetTClassName<T>());  // lookup metatable in Lua registry
		if (lua_isnil(L, -1)) luaL_error(L, "%s missing metatable", GetTClassName<T>());
		int mt = lua_gettop(L);
		subtable(L, mt, "userdata", "v");
		userdataTypePtr *ud =
			static_cast<userdataTypePtr*>(pushuserdata(L, obj.get(), sizeof(userdataTypePtr)));
		if (ud) {
			ud->pT = obj.get();  // store pointer to object in userdata
			lua_pushvalue(L, mt);
			lua_setmetatable(L, -2);
			if (gc == false) {
				lua_checkstack(L, 3);
				subtable(L, mt, "do not trash", "k");
				lua_pushvalue(L, -2);
				lua_pushboolean(L, 1);
				lua_settable(L, -3);
				lua_pop(L, 1);
			}
		}
		lua_replace(L, mt);
		lua_settop(L, mt);
		return mt;  // index of userdata containing pointer to T object
	}
	static int push(lua_State *L, T * obj, bool gc=false) {
		if (!obj) { lua_pushnil(L); return 0; }
		luaL_getmetatable(L, GetTClassName<T>());  // lookup metatable in Lua registry
		if (lua_isnil(L, -1)) luaL_error(L, "%s missing metatable", GetTClassName<T>());
		int mt = lua_gettop(L);
		subtable(L, mt, "userdata", "v");
		userdataTypePtr *ud =
			static_cast<userdataTypePtr*>(pushuserdata(L, obj, sizeof(userdataTypePtr)));
		if (ud) {
			ud->pT = obj;  // store pointer to object in userdata
			lua_pushvalue(L, mt);
			lua_setmetatable(L, -2);
			if (gc == false) {
				lua_checkstack(L, 3);
				subtable(L, mt, "do not trash", "k");
				lua_pushvalue(L, -2);
				lua_pushboolean(L, 1);
				lua_settable(L, -3);
				lua_pop(L, 1);
			}
		}
		lua_replace(L, mt);
		lua_settop(L, mt);
		return mt;  // index of userdata containing pointer to T object
	}

	// get userdata from Lua stack and return pointer to T object
	static shared_ptr<T> check(lua_State *L, int narg) {
		userdataTypePtr *ud =
			static_cast<userdataTypePtr*>(luaL_checkudata(L, narg, GetTClassName<T>()));
		if(!ud) { luaL_typerror(L, narg, GetTClassName<T>()); return NULLUNIT; }
		return shared_ptr<T>(ud->pT);  // pointer to T object
	}
	static T* checkptr(lua_State *L, int narg) {
		userdataTypePtr *ud =
			static_cast<userdataTypePtr*>(luaL_checkudata(L, narg, GetTClassName<T>()));
		if(!ud) { luaL_typerror(L, narg, GetTClassName<T>()); return NULL; }
		return (ud->pT);  // pointer to T object
	}

private:
	Lunar();  // hide default constructor

	// member function dispatcher
	static int thunk(lua_State *L) {
		// stack has userdata, followed by method args
		T *obj = checkptr(L, 1);  // get 'self', or if you prefer, 'this'
		lua_remove(L, 1);  // remove self so member function args start at index 1
		// get member function from upvalue
		RegType *l = static_cast<RegType*>(lua_touserdata(L, lua_upvalueindex(1)));
		//return (obj->*(l->mfunc))(L);  // call member function
		return l->mfunc(L,obj);
	}

	// create a new T object and
	// push onto the Lua stack a userdata containing a pointer to T object
	static int new_T(lua_State *L) {
		lua_remove(L, 1);   // use classname:new(), instead of classname.new()
		T *obj = NULL/*new T(L)*/;  // call constructor for T objects
		assert(false);
		push(L, obj, true); // gc_T will delete this object
		return 1;           // userdata containing pointer to T object
	}

	// garbage collection metamethod
	static int gc_T(lua_State *L) {
		if (luaL_getmetafield(L, 1, "do not trash")) {
			lua_pushvalue(L, 1);  // dup userdata
			lua_gettable(L, -2);
			if (!lua_isnil(L, -1)) return 0;  // do not delete object
		}
		userdataTypePtr *ud = static_cast<userdataTypePtr*>(lua_touserdata(L, 1));
		T *obj = ud->pT;
		if (obj) delete obj;  // call destructor for T objects
		return 0;
	}

	static int tostring_T (lua_State *L) {
		char buff[32];
		userdataTypePtr *ud = static_cast<userdataTypePtr*>(lua_touserdata(L, 1));
		T *obj = ud->pT;
		sprintf(buff, "%p", obj);
		lua_pushfstring(L, "%s (%s)", GetTClassName<T>(), buff);
		return 1;
	}

	static void set(lua_State *L, int table_index, const char *key) {
		lua_pushstring(L, key);
		lua_insert(L, -2);  // swap value and key
		lua_settable(L, table_index);
	}

	static void weaktable(lua_State *L, const char *mode) {
		lua_newtable(L);
		lua_pushvalue(L, -1);  // table is its own metatable
		lua_setmetatable(L, -2);
		lua_pushliteral(L, "__mode");
		lua_pushstring(L, mode);
		lua_settable(L, -3);   // metatable.__mode = mode
	}

	static void subtable(lua_State *L, int tindex, const char *name, const char *mode) {
		lua_pushstring(L, name);
		lua_gettable(L, tindex);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			lua_checkstack(L, 3);
			weaktable(L, mode);
			lua_pushstring(L, name);
			lua_pushvalue(L, -2);
			lua_settable(L, tindex);
		}
	}

	static void *pushuserdata(lua_State *L, void *key, size_t sz) {
		void *ud = 0;
		lua_pushlightuserdata(L, key);
		lua_gettable(L, -2);     // lookup[key]
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);         // drop nil
			lua_checkstack(L, 3);
			ud = lua_newuserdata(L, sz);  // create new userdata
			lua_pushlightuserdata(L, key);
			lua_pushvalue(L, -2);  // dup userdata
			lua_settable(L, -4);   // lookup[key] = userdata
		}
		return ud;
	}
};

void report(lua_State * L)
{
	const char * msg= lua_tostring(L,-1);
	uint8 count = 0;
	while(msg && count <= 20)
	{
		lua_pop(L,-1);
		printf("\t%s\n", msg);
		msg=lua_tostring(L,-1);
		count++;
	}
}

LuaEngine::LuaEngine()
{
	this->L = lua_open();
}

LuaEngine::~LuaEngine()
{
	lua_close(L);
}

void LuaEngine::LoadScripts()
{
	set<string> luaFiles;
	set<string> luaBytecodeFiles;

#ifdef WIN32
	WIN32_FIND_DATA fd;
	HANDLE h;

	h = FindFirstFile("scripts\\*.*", &fd);
	if(h == INVALID_HANDLE_VALUE)
		return;
	
	do 
	{
		char * fn = strrchr(fd.cFileName, '\\');
		if(!fn)
			fn=fd.cFileName;
        char * ext = strrchr(fd.cFileName, '.');
		if(!stricmp(ext, ".lua"))
			luaFiles.insert(string(fn));
		else if(!stricmp(ext, ".luc"))
			luaBytecodeFiles.insert(string(fn));
	} while(FindNextFile(h, &fd));
	FindClose(h);
#else
	struct dirent ** list;
	int filecount = scandir("./scripts", &list, 0, 0);
	if(filecount <= 0 || !list)
		return;

	while(filecount--)
	{
		char* ext = strrchr(list[filecount]->d_name, '.');
		if(ext != NULL && !strcmp(ext, ".lua"))
			{
				string full_path = string(list[filecount]->d_name);
				luaFiles.insert(string(full_path.c_str()));
		}
		else if(!stricmp(ext, ".luc"))
		{
		string full_path = string(list[filecount]->d_name);
		luaBytecodeFiles.insert(string(full_path.c_str()));
		}

		free(list[filecount]);
	}
	free(list);
#endif

	// we prefer precompiled code.
	for(set<string>::iterator itr = luaBytecodeFiles.begin(); itr != luaBytecodeFiles.end(); ++itr)
	{
		set<string>::iterator it2 = luaFiles.find(*itr);
		if(it2 == luaFiles.end())
			luaFiles.erase(it2);
	}

	luaL_openlibs(L);
	RegisterCoreFunctions();

	if(lua_is_starting_up)
		Log.Notice("Blua Engine", "Loading Scripts... ");

	char filename[200];

	for(set<string>::iterator itr = luaFiles.begin(); itr != luaFiles.end(); ++itr)
	{
#ifdef WIN32
			snprintf(filename, 200, "scripts\\%s", itr->c_str());
#else
			snprintf(filename, 200, "scripts/%s", itr->c_str());
#endif
		if(lua_is_starting_up)
			Log.Notice("Blua Engine", "%s...", itr->c_str());

		if(luaL_loadfile(L, filename) != 0)
		{
			//printf(" %sfailed. (could not load)\n",MSG_COLOR_RED);
			sLog.outColor(TRED, "failed. (could not run)\n");
			const char * msg = lua_tostring(L, -1);
			if(msg!=NULL&&lua_is_starting_up)
				//printf(" %s\t%s\n",msg,MSG_COLOR_RED);
				sLog.outColor(TRED, "\t%s\n",msg);
		}
		else
		{
			if(lua_pcall(L, 0, LUA_MULTRET, 0) != 0)
			{
				//printf("failed. (could not run)\n");
				sLog.outColor(TRED, "failed. (could not run)\n");
				const char * msg = lua_tostring(L, -1);
				if(msg!=NULL&&lua_is_starting_up)
					//printf(" %s\t%s\n",msg,MSG_COLOR_RED);
					sLog.outColor(TRED, "\t%s\n",msg);
			}
		}
	}
}

void LuaEngine::OnUnitEvent(Unit * pUnit, const char * FunctionName, uint32 EventType, Unit * pMiscUnit, uint32 Misc, int32 Misc2, float Misc3, bool Yes, ItemPrototype * Misc4)
{
	if(FunctionName==NULL)
		return;
	size_t pos1,pos2,pos3,pos4,pos5 = 0;
	string pString = string(FunctionName);
	char * key = ".";
	pos1 = pString.find(key);
	pos2 = pString.find(key,pos1+1);
	pos3 = pString.find(key,pos2+1);
	pos4 = pString.find(key,pos3+1);
	pos5 = pString.find(key,pos4+1);
	string tblname1 = pString.substr(0,pos1);
	string tblname2 = pString.substr(pos1+1,(pos2-pos1-1));
	string tblname3 = pString.substr(pos2+1,(pos3-pos2-1));
	string tblname4 = pString.substr(pos3+1,(pos4-pos3-1));
	string tblname5 = pString.substr(pos4+1,string::npos);
	if(pos1 != string::npos)
	{
		lua_pushstring(L,tblname1.c_str());
		lua_gettable(L,LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			sLog.outColor(TRED, "Tried to access invalid LUA table '%s' from Ascent (Unit)!\n",tblname1.c_str());
			return;
		}
		else if(lua_istable(L,-1))
		{
			if(pos2 != string::npos)
			{
				lua_pushstring(L,tblname2.c_str());
				lua_gettable(L,-2);
				if(lua_isnil(L,-1))
				{
					printf("Tried to access invalid LUA table '%s' from Ascent (Unit)!\n",tblname2);
					return;
				}
				else if(lua_istable(L,-1))
				{
					if(pos3 != string::npos)
					{
						lua_pushstring(L,tblname3.c_str());
						lua_gettable(L,-2);
						if(lua_isnil(L,-1))
						{
							sLog.outColor(TRED, "Tried to access invalid LUA table '%s' from Ascent (Unit)!\n",tblname3.c_str());
							return;
						}
						else if(lua_istable(L,-1))
						{
							if(pos4 != string::npos)
							{
								lua_pushstring(L,tblname4.c_str());
								lua_gettable(L,-2);
								if(lua_isnil(L,-1))
								{
									sLog.outColor(TRED, "Tried to access invalid LUA table '%s' from Ascent (Unit)!\n",tblname4.c_str());
									return;
								}
								if(lua_istable(L,-1))
								{
									if(pos5 != string::npos)
									{
										lua_pushstring(L,tblname5.c_str());
										lua_gettable(L,-2);
										if(lua_isnil(L,-1))
										{
											sLog.outColor(TRED, "Tried to access invalid LUA table '%s' from Ascent (Unit)!\n",tblname5.c_str());
											return;
										}
										else if(lua_istable(L,-1))
										{
											printf("Too many tables were specified, Aborting.....\n");
											return;
										}
									}
									else
									{
										string func = pString.substr(pos4+1,string::npos);
										m_Lock.Acquire();
										lua_pushstring(L,func.c_str());
										lua_gettable(L,-2);
										Lunar<Unit>::push(L, pUnit);
										lua_pushinteger(L,EventType);
										if(pMiscUnit!=NULL)
											Lunar<Unit>::push(L, pMiscUnit);
										else
											lua_pushnil(L);
										lua_pushinteger(L,Misc);
										lua_pushnumber(L,lua_Number(Misc2));
										lua_pushnumber(L,lua_Number(Misc3));
										( (Yes == true) ? lua_pushboolean(L,1) : lua_pushboolean(L,0) );
										( (Misc4 != NULL) ? lua_pushinteger(L,Misc4->ItemId) : lua_pushnil(L) );

										int r = lua_pcall(L,8,LUA_MULTRET,0);
										if(r)
											report(L);
										m_Lock.Release();
										return;
									}
								}
							}
							else
							{
								m_Lock.Acquire();
								string func = pString.substr(pos3+1,string::npos);
								lua_pushstring(L,func.c_str());
								lua_gettable(L,-2);
								Lunar<Unit>::push(L, pUnit);
								lua_pushinteger(L,EventType);
								if(pMiscUnit!=NULL)
									Lunar<Unit>::push(L, pMiscUnit);
								else
									lua_pushnil(L);
								lua_pushinteger(L,Misc);
								lua_pushnumber(L,lua_Number(Misc2));
								lua_pushnumber(L,lua_Number(Misc3));
								( (Yes == true) ? lua_pushboolean(L,1) : lua_pushboolean(L,0) );
								( (Misc4 != NULL) ? lua_pushinteger(L,Misc4->ItemId) : lua_pushnil(L) );

								int r = lua_pcall(L,8,LUA_MULTRET,0);
								if(r)
									report(L);
								m_Lock.Release();
								return;
							}
						}
					}
					else
					{
						m_Lock.Acquire();
						string func1 = pString.substr(pos2+1,string::npos);
						lua_pushstring(L,func1.c_str());
						lua_gettable(L,-2);
						Lunar<Unit>::push(L, pUnit);
						lua_pushinteger(L,EventType);
						if(pMiscUnit!=NULL)
							Lunar<Unit>::push(L, pMiscUnit);
						else
							lua_pushnil(L);
						lua_pushinteger(L,Misc);
						lua_pushnumber(L,lua_Number(Misc2));
						lua_pushnumber(L,lua_Number(Misc3));
						( (Yes == true) ? lua_pushboolean(L,1) : lua_pushboolean(L,0) );
						( (Misc4 != NULL) ? lua_pushinteger(L,Misc4->ItemId) : lua_pushnil(L) );

						int r = lua_pcall(L,8,LUA_MULTRET,0);
						if(r)
							report(L);
						m_Lock.Release();
						return;
					}
				}
			}
			else
			{
				m_Lock.Acquire();
				string func = pString.substr(pos1+1,string::npos);
				lua_pushstring(L,func.c_str());
				lua_gettable(L,-2);
				Lunar<Unit>::push(L, pUnit);
				lua_pushinteger(L,EventType);
				if(pMiscUnit!=NULL)
					Lunar<Unit>::push(L, pMiscUnit);
				else
					lua_pushnil(L);
				lua_pushinteger(L,Misc);
				lua_pushnumber(L,lua_Number(Misc2));
				lua_pushnumber(L,lua_Number(Misc3));
				( (Yes == true) ? lua_pushboolean(L,1) : lua_pushboolean(L,0) );
				( (Misc4 != NULL) ? lua_pushinteger(L,Misc4->ItemId) : lua_pushnil(L) );

				int r = lua_pcall(L,8,LUA_MULTRET,0);
				if(r)
					report(L);
				m_Lock.Release();
				return;
			}
		}
	}
	else
	{
		m_Lock.Acquire();
		lua_pushstring(L, FunctionName);
		lua_gettable(L, LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			printf("Tried to call invalid LUA function '%s' from Ascent (Unit)!\n", FunctionName);
			m_Lock.Release();
			return;
		}

		Lunar<Unit>::push(L, pUnit);
		lua_pushinteger(L,EventType);
		if(pMiscUnit!=NULL)
			Lunar<Unit>::push(L, pMiscUnit);
		else
			lua_pushnil(L);
		lua_pushinteger(L,Misc);
		lua_pushnumber(L,lua_Number(Misc2));
		lua_pushnumber(L,lua_Number(Misc3));
		( (Yes == true) ? lua_pushboolean(L,1) : lua_pushboolean(L,0) );
		( (Misc4 != NULL) ? lua_pushinteger(L,Misc4->ItemId) : lua_pushnil(L) );
		
		int r = lua_pcall(L,8,LUA_MULTRET,0);
		if(r) report(L);
		m_Lock.Release();
		return;
	}
}

void LuaEngine::OnQuestEvent(Player *  QuestOwner, const char * FunctionName, uint32 QuestID, uint32 EventType, Player *  QuestStarter,uint32 Misc, uint32 Misc2)
{
	if(FunctionName==NULL) return;
	string pString = string(FunctionName);
	size_t pos1,pos2,pos3,pos4,pos5;
	string name1,name2,name3,name4,name5;
	char * key = ".";
	pos1 = pString.find(key);
	pos2 = pString.find(key,pos1+1);
	pos3 = pString.find(key,pos2+1);
	pos4 = pString.find(key,pos3+1);
	pos5 = pString.find(key,pos4+1);
	name1 = pString.substr(0,pos1);
	name2 = pString.substr(pos1+1,pos2-pos1-1);
	name3 = pString.substr(pos2+1,pos3-pos2-1);
	name4 = pString.substr(pos3+1,pos3-pos2-1);
	name5 = pString.substr(pos4+1,string::npos);
	m_Lock.Acquire();
	if(pos1 != string::npos)
	{
		lua_pushstring(L,name1.c_str());
		lua_gettable(L,LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name1);
			return;
		}
		else if(lua_istable(L,-2))
		{
			if(pos2 != string::npos)
			{
				lua_pushstring(L,name2.c_str());
				lua_gettable(L,-2);
				if(lua_isnil(L,-1))
				{
					printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name2);
					return;
				}
				else if(lua_istable(L,-1))
				{
					if(pos3 != string::npos)
					{
						lua_pushstring(L,name3.c_str());
						lua_gettable(L,-2);
						if(lua_isnil(L,-1))
						{
							printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name3);
							return;
						}
						else if(lua_istable(L,-1))
						{
							if(pos4 != string::npos)
							{
								lua_pushstring(L,name4.c_str());
								lua_gettable(L,-2);
								if(lua_isnil(L,-1))
								{
									printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name4);
									return;
								}
								else if(lua_istable(L,-1))
								{
									if(pos5 != string::npos)
									{
										lua_pushstring(L,name5.c_str());
										lua_gettable(L,-2);
										if(lua_isnil(L,-1))
										{
											printf("Tried to access and invalid Lua function %s from Ascent(GO). \n",name5);
											return;
										}
										else if(lua_istable(L,-1))
										{
											printf("Reached maximum number of tables allowed. Aborting..... \n");
											return;
										}
									}
									else
									{
										m_Lock.Acquire();
										string func = pString.substr(pos4+1,string::npos);
										lua_pushstring(L,func.c_str());
										lua_gettable(L,-2);
										if (QuestOwner) Lunar<Unit>::push(L, (Unit *)QuestOwner);
										else lua_pushnil(L);
										lua_pushinteger(L,EventType);
										if(QuestStarter!=NULL && QuestStarter->GetTypeId() == TYPEID_UNIT) Lunar<Unit>::push(L, (Unit *)QuestStarter);
										else lua_pushnil(L);
										int r = lua_pcall(L,3,LUA_MULTRET,0);
										if(r) report(L);
										m_Lock.Release();
										return;
									}
								}
							}
							else
							{
								m_Lock.Acquire();
								string func = pString.substr(pos3+1,string::npos);
								lua_pushstring(L,func.c_str());
								lua_gettable(L,-2);
								if (QuestOwner) Lunar<Unit>::push(L, (Unit *)QuestOwner);
								else lua_pushnil(L);
								lua_pushinteger(L,EventType);
								if(QuestStarter!=NULL && QuestStarter->GetTypeId() == TYPEID_UNIT) Lunar<Unit>::push(L, (Unit *)QuestStarter);
								else lua_pushnil(L);
								int r = lua_pcall(L,3,LUA_MULTRET,0);
								if(r) report(L);
								m_Lock.Release();
								return;
							}
						}
					}
					else
					{
						m_Lock.Acquire();
						string func = pString.substr(pos2+1,string::npos);
						lua_pushstring(L,func.c_str());
						lua_gettable(L,-2);
						if (QuestOwner) Lunar<Unit>::push(L, (Unit *)QuestOwner);
						else lua_pushnil(L);
						lua_pushinteger(L,EventType);
						if(QuestStarter!=NULL && QuestStarter->GetTypeId() == TYPEID_UNIT) Lunar<Unit>::push(L, (Unit *)QuestStarter);
						else lua_pushnil(L);
						int r = lua_pcall(L,3,LUA_MULTRET,0);
						if(r) report(L);
						m_Lock.Release();
						return;
					}
				}
			}
			else
			{
				m_Lock.Acquire();
				string func = pString.substr(pos1+1,string::npos);
				lua_pushstring(L,func.c_str());
				lua_gettable(L,-2);
				if (QuestOwner) Lunar<Unit>::push(L, (Unit *)QuestOwner);
				else lua_pushnil(L);
				lua_pushinteger(L,EventType);
				if(QuestStarter!=NULL && QuestStarter->GetTypeId() == TYPEID_UNIT) Lunar<Unit>::push(L, (Unit *)QuestStarter);
				else lua_pushnil(L);
				int r = lua_pcall(L,3,LUA_MULTRET,0);
				if(r) report(L);
				m_Lock.Release();
				return;
			}
		}	
	}
	else
	{
		m_Lock.Acquire();
		lua_pushstring(L, FunctionName);
		lua_gettable(L, LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{	
			printf("Tried to call invalid LUA function '%s' from Ascent (Quest)!\n", FunctionName);
			m_Lock.Release();
			return;
		}
		if (QuestOwner) Lunar<Unit>::push(L, (Unit *)QuestOwner);
		else lua_pushnil(L);
		lua_pushinteger(L,EventType);
		if(QuestStarter!=NULL && QuestStarter->GetTypeId() == TYPEID_UNIT) Lunar<Unit>::push(L, (Unit *)QuestStarter);
		else lua_pushnil(L);
		int r = lua_pcall(L,3,LUA_MULTRET,0);
		if(r) report(L);
		m_Lock.Release();
		return;
	}
}
void LuaEngine::CallFunction(Unit * pUnit, const char * FuncName)
{
	if(FuncName == NULL) return;
	size_t pos1,pos2,pos3,pos4,pos5 = 0;
	string pString = string(FuncName);
	char * key = ".";
	pos1 = pString.find(key);
	pos2 = pString.find(key,pos1+1);
	pos3 = pString.find(key,pos2+1);
	pos4 = pString.find(key,pos3+1);
	pos5 = pString.find(key,pos4+1);
	string tblname1 = pString.substr(0,pos1);
	string tblname2 = pString.substr(pos1+1,(pos2-pos1-1));
	string tblname3 = pString.substr(pos2+1,(pos3-pos2-1));
	string tblname4 = pString.substr(pos3+1,(pos4-pos3-1));
	string tblname5 = pString.substr(pos4+1, string::npos);
	if(pos1 != string::npos)
	{
		lua_pushstring(L,tblname1.c_str());
		lua_gettable(L,LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			sLog.outColor(TRED, "Tried to access invalid LUA table '%s' from Ascent (Unit)!\n",tblname1.c_str());
			return;
		}
		else if(lua_istable(L,-1))
		{
			if(pos2 != string::npos) // another table string;
			{
				lua_pushstring(L,tblname2.c_str());
				lua_gettable(L,-2);
				if(lua_isnil(L,-1))
				{
					sLog.outColor(TRED, "Tried to access invalid LUA table '%s' from Ascent (Unit)!\n",tblname2);
					return;
				}
				else if(lua_istable(L,-1))
				{
					if(pos3 != string::npos)
					{
						lua_pushstring(L,tblname3.c_str());
						lua_gettable(L,-2);
						if(lua_isnil(L,-1))
						{
							sLog.outColor(TRED, "Tried to access invalid LUA table '%s' from Ascent (Unit)!\n",tblname3.c_str());
							return;
						}
						else if(lua_istable(L,-1))
						{
							if(pos4 != string::npos)
							{
								lua_pushstring(L,tblname4.c_str());
								lua_gettable(L,-2);
								if(lua_isnil(L,-1))
								{
									sLog.outColor(TRED, "Tried to access invalid LUA table '%s' from Ascent (Unit)!\n",tblname4.c_str());
									return;
								}
								else if(lua_istable(L,-1))
								{
									if(pos5 != string::npos)
									{
										lua_pushstring(L,tblname5.c_str());
										lua_gettable(L,-2);
										if(lua_istable(L,-1))
										{
											printf("Reached maximum tables allowed to be registered. Aborting... \n");
											return;
										}
										else if(lua_isnil(L,-1))
										{
											sLog.outColor(TRED, "Tried to access invalid LUA table '%s' from Ascent (Unit)!\n",tblname5.c_str());
											return;
										}
									}
									else
									{
										m_Lock.Acquire();
										string func = pString.substr(pos4+1,string::npos);
										lua_pushstring(L,func.c_str());
										lua_gettable(L,-2);
										Lunar<Unit>::push(L, pUnit);
										int r = lua_pcall(L,1,LUA_MULTRET,0);
										if(r)
											report(L);
										m_Lock.Release();
										return;
									}
								}

							}
							else
							{
								m_Lock.Acquire();
								string func = pString.substr(pos3+1,string::npos);
								lua_pushstring(L,func.c_str());
								lua_gettable(L,-2);
								Lunar<Unit>::push(L, pUnit);
								int r = lua_pcall(L,1,LUA_MULTRET,0);
								if(r)
									report(L);

								m_Lock.Release();
								return;
							}
						}
					}
					else
					{
						m_Lock.Acquire();
						string func = pString.substr(pos2+1,string::npos);
						lua_pushstring(L,func.c_str());
						lua_gettable(L,-2);
						Lunar<Unit>::push(L, pUnit);
						int r = lua_pcall(L,1,LUA_MULTRET,0);
						if(r)
							report(L);

						m_Lock.Release();
						return;
					}
				}
			}
			else
			{
				m_Lock.Acquire();
				string func = pString.substr(pos1+1,string::npos);
				lua_pushstring(L,func.c_str());
				lua_gettable(L,-2);
				Lunar<Unit>::push(L, pUnit);
				int r = lua_pcall(L,1,LUA_MULTRET,0);
				if(r)
					report(L);

				m_Lock.Release();
				return;
			}
		}
	}
	m_Lock.Acquire();
	lua_pushstring(L, FuncName);
	lua_gettable(L, LUA_GLOBALSINDEX);
	if(lua_isnil(L,-1))
	{
		printf("Tried to call invalid LUA function '%s' from Ascent (Unit)!\n", FuncName);
		m_Lock.Release();
		return;
	}

	Lunar<Unit>::push(L, pUnit);
	int r = lua_pcall(L,1,LUA_MULTRET,0);
	if(r)
		report(L);

	m_Lock.Release();
}

void LuaEngine::OnGameObjectEvent(GameObject *  pGameObject, const char * FunctionName, uint32 EventType, Unit * pMiscUnit, uint32 Misc, ItemPrototype* Misc2)
{
	if(FunctionName==NULL)
		return;
	string pString = string(FunctionName);
	size_t pos1,pos2,pos3,pos4,pos5;
	string name1,name2,name3,name4,name5;
	char * key = ".";
	pos1 = pString.find(key);
	pos2 = pString.find(key,pos1+1);
	pos3 = pString.find(key,pos2+1);
	pos4 = pString.find(key,pos3+1);
	pos5 = pString.find(key,pos4+1);
	name1 = pString.substr(0,pos1);
	name2 = pString.substr(pos1+1,pos2-pos1-1);
	name3 = pString.substr(pos2+1,pos3-pos2-1);
	name4 = pString.substr(pos3+1,pos3-pos2-1);
	name5 = pString.substr(pos4+1,string::npos);
	if(pos1 != string::npos)
	{
		lua_pushstring(L,name1.c_str());
		lua_gettable(L,LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name1);
			return;
		}
		else if(lua_istable(L,-2))
		{
			if(pos2 != string::npos)
			{
				lua_pushstring(L,name2.c_str());
				lua_gettable(L,-2);
				if(lua_isnil(L,-1))
				{
					printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name2);
					return;
				}
				else if(lua_istable(L,-1))
				{
					if(pos3 != string::npos)
					{
						lua_pushstring(L,name3.c_str());
						lua_gettable(L,-2);
						if(lua_isnil(L,-1))
						{
							printf("Tried to access an invalid Lua table/function %s from Ascent(GO). \n",name3);
							return;
						}
						else if(lua_istable(L,-1))
						{
							if(pos4 != string::npos)
							{
								lua_pushstring(L,name4.c_str());
								lua_gettable(L,-2);
								if(lua_isnil(L,-1))
								{
									printf("Tried to access an invalid Lua table/function %s from Ascent(GO). \n",name4);
									return;
								}
								else if(lua_istable(L,-1))
								{
									if(pos5 != string::npos)
									{
										lua_pushstring(L,name5.c_str());
										lua_gettable(L,-2);
										if(lua_isnil(L,-1))
										{
											printf("Tried to access and invalid Lua function/function %s from Ascent(GO). \n",name5);
											return;
										}
										else if(lua_istable(L,-1))
										{
											printf("Reached maximum number of tables allowed. Aborting..... \n");
											return;
										}
									}
									else
									{
										m_Lock.Acquire();
										string func = pString.substr(pos4+1,string::npos);
										lua_pushstring(L,func.c_str());
										lua_gettable(L,-2);
										Lunar<GameObject>::push(L, pGameObject);
										lua_pushinteger(L,EventType);
										if(!pMiscUnit)
											lua_pushnil(L);
										else
											Lunar<Unit>::push(L, pMiscUnit);

										int r = lua_pcall(L,3,LUA_MULTRET,0);
										if(r)
											report(L);

										m_Lock.Release();
										return;
									}
								}
							}
							else
							{
								m_Lock.Acquire();
								string func = pString.substr(pos3+1,string::npos);
								lua_pushstring(L,func.c_str());
								lua_gettable(L,-2);
								Lunar<GameObject>::push(L, pGameObject);
								lua_pushinteger(L,EventType);
								if(!pMiscUnit)
									lua_pushnil(L);
								else
									Lunar<Unit>::push(L, pMiscUnit);

								int r = lua_pcall(L,3,LUA_MULTRET,0);
								if(r)
									report(L);

								m_Lock.Release();
								return;
							}
						}
					}
					else
					{
						m_Lock.Acquire();
						string func = pString.substr(pos2+1,string::npos);
						lua_pushstring(L,func.c_str());
						lua_gettable(L,-2);
						Lunar<GameObject>::push(L, pGameObject);
						lua_pushinteger(L,EventType);
						if(!pMiscUnit)
							lua_pushnil(L);
						else
							Lunar<Unit>::push(L, pMiscUnit);

						int r = lua_pcall(L,3,LUA_MULTRET,0);
						if(r)
							report(L);

						m_Lock.Release();
						return;
					}
				}
			}
			else
			{
				m_Lock.Acquire();
				string func = pString.substr(pos1+1,string::npos);
				lua_pushstring(L,func.c_str());
				lua_gettable(L,-2);
				Lunar<GameObject>::push(L, pGameObject);
				lua_pushinteger(L,EventType);
				if(!pMiscUnit)
					lua_pushnil(L);
				else
					Lunar<Unit>::push(L, pMiscUnit);

				int r = lua_pcall(L,3,LUA_MULTRET,0);
				if(r)
					report(L);

				m_Lock.Release();
				return;
			}
		}	
	}
	else
	{
		m_Lock.Acquire();
		lua_pushstring(L, FunctionName);
		lua_gettable(L, LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			printf("Tried to call invalid LUA function '%s' from Ascent! (GO)\n", FunctionName);
			m_Lock.Release();
			return;
		}

		Lunar<GameObject>::push(L, pGameObject);
		lua_pushinteger(L,EventType);
		if(!pMiscUnit)
			lua_pushnil(L);
		else
			Lunar<Unit>::push(L, pMiscUnit);

		int r = lua_pcall(L,3,LUA_MULTRET,0);
		if(r)
			report(L);

		m_Lock.Release();
		return;
	}
}

void LuaEngine::OnGossipEvent(Object * pObject, const char * FunctionName, uint32 EventType, Player *  mPlayer, uint32 Id, uint32 IntId, const char * Code)
{
    if(FunctionName==NULL)
		return;

	string pString = string(FunctionName);
	size_t pos1,pos2,pos3,pos4,pos5;
	string name1,name2,name3,name4,name5;
	char * key = ".";
	pos1 = pString.find(key);
	pos2 = pString.find(key,pos1+1);
	pos3 = pString.find(key,pos2+1);
	pos4 = pString.find(key,pos3+1);
	pos5 = pString.find(key,pos4+1);
	name1 = pString.substr(0,pos1);
	name2 = pString.substr(pos1+1,pos2-pos1-1);
	name3 = pString.substr(pos2+1,pos3-pos2-1);
	name4 = pString.substr(pos3+1,pos3-pos2-1);
	name5 = pString.substr(pos4+1,string::npos);
	if(pos1 != string::npos)
	{
		lua_pushstring(L,name1.c_str());
		lua_gettable(L,LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name1);
			return;
		}
		else if(lua_istable(L,-2))
		{
			if(pos2 != string::npos)
			{
				lua_pushstring(L,name2.c_str());
				lua_gettable(L,-2);
				if(lua_isnil(L,-1))
				{
					printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name2);
					return;
				}
				else if(lua_istable(L,-1))
				{
					if(pos3 != string::npos)
					{
						lua_pushstring(L,name3.c_str());
						lua_gettable(L,-2);
						if(lua_isnil(L,-1))
						{
							printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name3);
							return;
						}
						else if(lua_istable(L,-1))
						{
							if(pos4 != string::npos)
							{
								lua_pushstring(L,name4.c_str());
								lua_gettable(L,-2);
								if(lua_isnil(L,-1))
								{
									printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name4);
									return;
								}
								else if(lua_istable(L,-1))
								{
									if(pos5 != string::npos)
									{
										lua_pushstring(L,name5.c_str());
										lua_gettable(L,-2);
										if(lua_isnil(L,-1))
										{
											printf("Tried to access and invalid Lua function %s from Ascent(GO). \n",name5);
											return;
										}
										else if(lua_istable(L,-1))
										{
											printf("Reached maximum number of tables allowed. Aborting..... \n");
											return;
										}
									}
									else
									{
										m_Lock.Acquire();
										string func = pString.substr(pos4+1,string::npos);
										lua_pushstring(L,func.c_str());
										lua_gettable(L,-2);
										if(pObject->GetTypeId() == TYPEID_UNIT) Lunar<Unit>::push(L, TO_UNIT(pObject));
										else if(pObject->GetTypeId() == TYPEID_ITEM) Lunar<Item>::push(L, TO_ITEM(pObject));
										else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT) Lunar<GameObject>::push(L, TO_GAMEOBJECT(pObject));
										lua_pushinteger(L, EventType);
										Lunar<Unit>::push(L, (Player * )mPlayer);
										lua_pushinteger(L, Id);
										lua_pushinteger(L, IntId);
										lua_pushstring(L, Code);
										int r = lua_pcall(L, 6, LUA_MULTRET, 0);
										if(r) report(L);
										m_Lock.Release();
										return;
									}
								}
							}
							else
							{
								m_Lock.Acquire();
								string func = pString.substr(pos3+1,string::npos);
								lua_pushstring(L,func.c_str());
								lua_gettable(L,-2);
								if(pObject->GetTypeId() == TYPEID_UNIT) Lunar<Unit>::push(L, TO_UNIT(pObject));
								else if(pObject->GetTypeId() == TYPEID_ITEM) Lunar<Item>::push(L, TO_ITEM(pObject));
								else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT) Lunar<GameObject>::push(L, TO_GAMEOBJECT(pObject));
								lua_pushinteger(L, EventType);
								Lunar<Unit>::push(L, (Player * )mPlayer);
								lua_pushinteger(L, Id);
								lua_pushinteger(L, IntId);
								lua_pushstring(L, Code);
								int r = lua_pcall(L, 6, LUA_MULTRET, 0);
								if(r) report(L);
								m_Lock.Release();
								return;
							}
						}
					}
					else
					{
						m_Lock.Acquire();
						string func = pString.substr(pos2+1,string::npos);
						lua_pushstring(L,func.c_str());
						lua_gettable(L,-2);
						if(pObject->GetTypeId() == TYPEID_UNIT) Lunar<Unit>::push(L, TO_UNIT(pObject));
						else if(pObject->GetTypeId() == TYPEID_ITEM) Lunar<Item>::push(L, TO_ITEM(pObject));
						else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT) Lunar<GameObject>::push(L, TO_GAMEOBJECT(pObject));
						lua_pushinteger(L, EventType);
						Lunar<Unit>::push(L, (Player * )mPlayer);
						lua_pushinteger(L, Id);
						lua_pushinteger(L, IntId);
						lua_pushstring(L, Code);
						int r = lua_pcall(L, 6, LUA_MULTRET, 0);
						if(r) report(L);
						m_Lock.Release();
						return;
					}
				}
			}
			else
			{
				m_Lock.Acquire();
				string func = pString.substr(pos1+1,string::npos);
				lua_pushstring(L,func.c_str());
				lua_gettable(L,-2);
				if(pObject->GetTypeId() == TYPEID_UNIT) Lunar<Unit>::push(L, TO_UNIT(pObject));
				else if(pObject->GetTypeId() == TYPEID_ITEM) Lunar<Item>::push(L, TO_ITEM(pObject));
				else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT) Lunar<GameObject>::push(L, TO_GAMEOBJECT(pObject));
				lua_pushinteger(L, EventType);
				Lunar<Unit>::push(L, (Player * )mPlayer);
				lua_pushinteger(L, Id);
				lua_pushinteger(L, IntId);
				lua_pushstring(L, Code);
				int r = lua_pcall(L, 6, LUA_MULTRET, 0);
				if(r) report(L);
				m_Lock.Release();
				return;
			}
		}	
	}
	else
	{
		m_Lock.Acquire();
		lua_pushstring(L, FunctionName);
		lua_gettable(L, LUA_GLOBALSINDEX);
		if(lua_isnil(L, -1))
		{
			printf("Tried to call invalid LUA function '%s' from Ascent (Gossip)!\n", FunctionName);
			m_Lock.Release();
			return;
		}
		if(pObject->GetTypeId() == TYPEID_UNIT) Lunar<Unit>::push(L, TO_UNIT(pObject));
		else if(pObject->GetTypeId() == TYPEID_ITEM) Lunar<Item>::push(L, TO_ITEM(pObject));
		else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT) Lunar<GameObject>::push(L, TO_GAMEOBJECT(pObject));
		lua_pushinteger(L, EventType);
		Lunar<Unit>::push(L, (Player * )mPlayer);
		lua_pushinteger(L, Id);
		lua_pushinteger(L, IntId);
		lua_pushstring(L, Code);
		int r = lua_pcall(L, 6, LUA_MULTRET, 0);
		if(r) report(L);
		m_Lock.Release();
		return;
	}
}
#ifdef ENABLE_LUA_HOOKS
void LuaEngine::OnServerHook(Player *  plr, uint32 evt, Object *  obj, const char * FunctionName, uint32 Misc = 0, uint32 Misc_ = 0)
{
	if(FunctionName == NULL) return;
	m_Lock.Acquire();
	string pString = string(FunctionName);
	size_t pos1,pos2,pos3,pos4,pos5;
	string name1,name2,name3,name4,name5;
	char * key = ".";
	pos1 = pString.find(key);
	pos2 = pString.find(key,pos1+1);
	pos3 = pString.find(key,pos2+1);
	pos4 = pString.find(key,pos3+1);
	pos5 = pString.find(key,pos4+1);
	name1 = pString.substr(0,pos1);
	name2 = pString.substr(pos1+1,pos2-pos1-1);
	name3 = pString.substr(pos2+1,pos3-pos2-1);
	name4 = pString.substr(pos3+1,pos3-pos2-1);
	name5 = pString.substr(pos4+1,string::npos);
	if(pos1 != string::npos)
	{
		lua_pushstring(L,name1.c_str());
		lua_gettable(L,LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name1);
			m_Lock.Release();
			return;
		}
		else if(lua_istable(L,-2))
		{
			if(pos2 != string::npos)
			{
				lua_pushstring(L,name2.c_str());
				lua_gettable(L,-2);
				if(lua_isnil(L,-1))
				{
					printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name2);
					m_Lock.Release();
					return;
				}
				else if(lua_istable(L,-1))
				{
					if(pos3 != string::npos)
					{
						lua_pushstring(L,name3.c_str());
						lua_gettable(L,-2);
						if(lua_isnil(L,-1))
						{
							printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name3);
							m_Lock.Release();
							return;
						}
						else if(lua_istable(L,-1))
						{
							if(pos4 != string::npos)
							{
								lua_pushstring(L,name4.c_str());
								lua_gettable(L,-2);
								if(lua_isnil(L,-1))
								{
									printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name4);
									m_Lock.Release();
									return;
								}
								else if(lua_istable(L,-1))
								{
									if(pos5 != string::npos)
									{
										lua_pushstring(L,name5.c_str());
										lua_gettable(L,-2);
										if(lua_isnil(L,-1))
										{
											printf("Tried to access and invalid Lua function %s from Ascent(GO). \n",name5);
											m_Lock.Release();
											return;
										}
										else if(lua_istable(L,-1))
										{
											printf("Reached maximum number of tables allowed. Aborting..... \n");
											m_Lock.Release();
											return;
										}
									}
									else
									{
										string func = pString.substr(pos4+1,string::npos);
										lua_pushstring(L,func.c_str());
										lua_gettable(L,-2);
										Lunar<Unit>::push(L, (Unit *)plr);
										lua_pushinteger(L, evt);
										if(obj == NULL) lua_pushnil(L);
										else switch(obj->GetTypeId())
										{
											case TYPEID_UNIT:
											case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
											case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
											default: lua_pushnil(L);
										}
										lua_pushinteger(L, Misc);
										lua_pushinteger(L, Misc_);
										int r = lua_pcall(L, 5, LUA_MULTRET, 0);
										if(r) report(L);
										m_Lock.Release();
										return;
									}
								}
							}
							else
							{
								string func = pString.substr(pos3+1,string::npos);
								lua_pushstring(L,func.c_str());
								lua_gettable(L,-2);
								Lunar<Unit>::push(L, (Unit *)plr);
								lua_pushinteger(L, evt);
								if(obj == NULL) lua_pushnil(L);
								else switch(obj->GetTypeId())
								{
									case TYPEID_UNIT:
									case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
									case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
									default: lua_pushnil(L);
								}
								lua_pushinteger(L, Misc);
								lua_pushinteger(L, Misc_);
								int r = lua_pcall(L, 5, LUA_MULTRET, 0);
								if(r) report(L);
								m_Lock.Release();
								return;
							}
						}
					}
					else
					{
						string func = pString.substr(pos2+1,string::npos);
						lua_pushstring(L,func.c_str());
						lua_gettable(L,-2);
						Lunar<Unit>::push(L, (Unit *)plr);
						lua_pushinteger(L, evt);
						if(obj == NULL) lua_pushnil(L);
						else switch(obj->GetTypeId())
						{
							case TYPEID_UNIT:
							case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
							case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
							default: lua_pushnil(L);
						}
						lua_pushinteger(L, Misc);
						lua_pushinteger(L, Misc_);
						int r = lua_pcall(L, 5, LUA_MULTRET, 0);
						if(r) report(L);
						m_Lock.Release();
						return;
					}
				}
			}
			else
			{
				string func = pString.substr(pos1+1,string::npos);
				lua_pushstring(L,func.c_str());
				lua_gettable(L,-2);
				Lunar<Unit>::push(L, (Unit *)plr);
				lua_pushinteger(L, evt);
				if(obj == NULL) lua_pushnil(L);
				else switch(obj->GetTypeId())
				{
					case TYPEID_UNIT:
					case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
					case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
					default: lua_pushnil(L);
				}
				lua_pushinteger(L, Misc);
				lua_pushinteger(L, Misc_);
				int r = lua_pcall(L, 5, LUA_MULTRET, 0);
				if(r) report(L);
				m_Lock.Release();
				return;
			}
		}	
	}
	else
	{
		lua_pushstring(L, FunctionName);
		lua_gettable(L, LUA_GLOBALSINDEX);
		if(lua_isnil(L, -1))
		{
			printf("Tried to call invalid LUA function '%s' from Ascent (Gossip)!\n", FunctionName);
			m_Lock.Release();
			return;
		}
		Lunar<Unit>::push(L, (Unit *)plr);
		lua_pushinteger(L, evt);
		if(obj == NULL) lua_pushnil(L);
		else switch(obj->GetTypeId())
		{
			case TYPEID_UNIT:
			case TYPEID_PLAYER:
				Lunar<Unit>::push(L, TO_UNIT(obj));
			case TYPEID_GAMEOBJECT:
				Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
			default:
				lua_pushnil(L);
		}
		lua_pushinteger(L, Misc);
		lua_pushinteger(L, Misc_);
		int r = lua_pcall(L, 5, LUA_MULTRET, 0);
		if(r) report(L);
		m_Lock.Release();
		return;
	}
}
void LuaEngine::OnServerHook(Player *  plr, uint32 evt, Object *  obj, const char * FunctionName, uint32 pmisc1, uint32 pmisc2, const char * misc1, const char * misc2)
{
	if(FunctionName==NULL)
		return;
	m_Lock.Acquire();
	string pString = string(FunctionName);
	size_t pos1,pos2,pos3,pos4,pos5;
	string name1,name2,name3,name4,name5;
	char * key = ".";
	pos1 = pString.find(key);
	pos2 = pString.find(key,pos1+1);
	pos3 = pString.find(key,pos2+1);
	pos4 = pString.find(key,pos3+1);
	pos5 = pString.find(key,pos4+1);
	name1 = pString.substr(0,pos1);
	name2 = pString.substr(pos1+1,pos2-pos1-1);
	name3 = pString.substr(pos2+1,pos3-pos2-1);
	name4 = pString.substr(pos3+1,pos3-pos2-1);
	name5 = pString.substr(pos4+1,string::npos);
	if(pos1 != string::npos)
	{
		lua_pushstring(L,name1.c_str());
		lua_gettable(L,LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name1);
			m_Lock.Release();
			return;
		}
		else if(lua_istable(L,-2))
		{
			if(pos2 != string::npos)
			{
				lua_pushstring(L,name2.c_str());
				lua_gettable(L,-2);
				if(lua_isnil(L,-1))
				{
					printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name2);
					m_Lock.Release();
					return;
				}
				else if(lua_istable(L,-1))
				{
					if(pos3 != string::npos)
					{
						lua_pushstring(L,name3.c_str());
						lua_gettable(L,-2);
						if(lua_isnil(L,-1))
						{
							printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name3);
							m_Lock.Release();
							return;
						}
						else if(lua_istable(L,-1))
						{
							if(pos4 != string::npos)
							{
								lua_pushstring(L,name4.c_str());
								lua_gettable(L,-2);
								if(lua_isnil(L,-1))
								{
									printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name4);
									m_Lock.Release();
									return;
								}
								else if(lua_istable(L,-1))
								{
									if(pos5 != string::npos)
									{
										lua_pushstring(L,name5.c_str());
										lua_gettable(L,-2);
										if(lua_isnil(L,-1))
										{
											printf("Tried to access and invalid Lua function %s from Ascent(GO). \n",name5);
											m_Lock.Release();
											return;
										}
										else if(lua_istable(L,-1))
										{
											printf("Reached maximum number of tables allowed. Aborting..... \n");
											m_Lock.Release();
											return;
										}
									}
									else
									{
										string func = pString.substr(pos4+1,string::npos);
										lua_pushstring(L,func.c_str());
										lua_gettable(L,-2);
										Lunar<Unit>::push(L, (Unit *)plr);
										lua_pushinteger(L, evt );
										if(obj == NULL) lua_pushnil(L);
										else switch(obj->GetTypeId())
										{
											case TYPEID_UNIT:
											case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
											case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
											default: lua_pushnil(L);
										}
										lua_pushinteger(L, pmisc1);
										lua_pushinteger(L, pmisc2);
										( (misc1 != NULL) ? lua_pushstring(L, misc1) : lua_pushnil(L) );
										( (misc2 != NULL) ? lua_pushstring(L, misc2) : lua_pushnil(L) );
										int r = lua_pcall(L, 7, LUA_MULTRET, 0);
										if(r)	report(L);
										m_Lock.Release();
									}
								}
							}
							else
							{
								string func = pString.substr(pos3+1,string::npos);
								lua_pushstring(L,func.c_str());
								lua_gettable(L,-2);
								Lunar<Unit>::push(L, (Unit *)plr);
								lua_pushinteger(L, evt );
								if(obj == NULL) lua_pushnil(L);
								else switch(obj->GetTypeId())
								{
									case TYPEID_UNIT:
									case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
									case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
									default: lua_pushnil(L);
								}
								lua_pushinteger(L, pmisc1);
								lua_pushinteger(L, pmisc2);
								( (misc1 != NULL) ? lua_pushstring(L, misc1) : lua_pushnil(L) );
								( (misc2 != NULL) ? lua_pushstring(L, misc2) : lua_pushnil(L) );
								int r = lua_pcall(L, 7, LUA_MULTRET, 0);
								if(r)	report(L);
								m_Lock.Release();
							}
						}
					}
					else
					{
						string func = pString.substr(pos2+1,string::npos);
						lua_pushstring(L,func.c_str());
						lua_gettable(L,-2);
						Lunar<Unit>::push(L, (Unit *)plr);
						lua_pushinteger(L, evt );
						if(obj == NULL) lua_pushnil(L);
						else switch(obj->GetTypeId())
						{
							case TYPEID_UNIT:
							case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
							case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
							default: lua_pushnil(L);
						}
						lua_pushinteger(L, pmisc1);
						lua_pushinteger(L, pmisc2);
						( (misc1 != NULL) ? lua_pushstring(L, misc1) : lua_pushnil(L) );
						( (misc2 != NULL) ? lua_pushstring(L, misc2) : lua_pushnil(L) );
						int r = lua_pcall(L, 7, LUA_MULTRET, 0);
						if(r)	report(L);
						m_Lock.Release();
					}
				}
			}
			else
			{
				string func = pString.substr(pos1+1,string::npos);
				lua_pushstring(L,func.c_str());
				lua_gettable(L,-2);
				Lunar<Unit>::push(L, (Unit *)plr);
				lua_pushinteger(L, evt );
				if(obj == NULL) lua_pushnil(L);
				else switch(obj->GetTypeId())
				{
					case TYPEID_UNIT:
					case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
					case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
					default: lua_pushnil(L);
				}
				lua_pushinteger(L, pmisc1);
				lua_pushinteger(L, pmisc2);
				( (misc1 != NULL) ? lua_pushstring(L, misc1) : lua_pushnil(L) );
				( (misc2 != NULL) ? lua_pushstring(L, misc2) : lua_pushnil(L) );
				int r = lua_pcall(L, 7, LUA_MULTRET, 0);
				if(r)	report(L);
				m_Lock.Release();
			}
		}	
	}
	else
	{
		lua_pushstring(L, FunctionName);
		lua_gettable(L, LUA_GLOBALSINDEX);
		if(lua_isnil(L, -1))
		{
			printf("Tried to call invalid LUA function '%s' from Ascent (Gossip)!\n", FunctionName);
			m_Lock.Release();
			return;
		}
		Lunar<Unit>::push(L, (Unit *)plr);
		lua_pushinteger(L, evt );
		if(obj == NULL) lua_pushnil(L);
		else switch(obj->GetTypeId())
		{
			case TYPEID_UNIT:
			case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
			case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
			default: lua_pushnil(L);
		}
		lua_pushinteger(L, pmisc1);
		lua_pushinteger(L, pmisc2);
		( (misc1 != NULL) ? lua_pushstring(L, misc1) : lua_pushnil(L) );
		( (misc2 != NULL) ? lua_pushstring(L, misc2) : lua_pushnil(L) );
		int r = lua_pcall(L, 7, LUA_MULTRET, 0);
		if(r)	report(L);
		m_Lock.Release();
	}
}
void LuaEngine::OnServerHook(Player *  plr, uint32 evt, Object *  obj, const char * FunctionName, ArenaTeam * pArena, bool misc1, bool misc2)
{
	if(FunctionName==NULL) return;

	m_Lock.Acquire();
	string pString = string(FunctionName);
	size_t pos1,pos2,pos3,pos4,pos5;
	string name1,name2,name3,name4,name5;
	char * key = ".";
	pos1 = pString.find(key);
	pos2 = pString.find(key,pos1+1);
	pos3 = pString.find(key,pos2+1);
	pos4 = pString.find(key,pos3+1);
	pos5 = pString.find(key,pos4+1);
	name1 = pString.substr(0,pos1);
	name2 = pString.substr(pos1+1,pos2-pos1-1);
	name3 = pString.substr(pos2+1,pos3-pos2-1);
	name4 = pString.substr(pos3+1,pos3-pos2-1);
	name5 = pString.substr(pos4+1,string::npos);
	if(pos1 != string::npos)
	{
		lua_pushstring(L,name1.c_str());
		lua_gettable(L,LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name1);
			m_Lock.Release();
			return;
		}
		else if(lua_istable(L,-2))
		{
			if(pos2 != string::npos)
			{
				lua_pushstring(L,name2.c_str());
				lua_gettable(L,-2);
				if(lua_isnil(L,-1))
				{
					printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name2);
					m_Lock.Release();
					return;
				}
				else if(lua_istable(L,-1))
				{
					if(pos3 != string::npos)
					{
						lua_pushstring(L,name3.c_str());
						lua_gettable(L,-2);
						if(lua_isnil(L,-1))
						{
							printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name3);
							m_Lock.Release();
							return;
						}
						else if(lua_istable(L,-1))
						{
							if(pos4 != string::npos)
							{
								lua_pushstring(L,name4.c_str());
								lua_gettable(L,-2);
								if(lua_isnil(L,-1))
								{
									printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name4);
									m_Lock.Release();
									return;
								}
								else if(lua_istable(L,-1))
								{
									if(pos5 != string::npos)
									{
										lua_pushstring(L,name5.c_str());
										lua_gettable(L,-2);
										if(lua_isnil(L,-1))
										{
											printf("Tried to access and invalid Lua function %s from Ascent(GO). \n",name5);
											m_Lock.Release();
											return;
										}
										else if(lua_istable(L,-1))
										{
											printf("Reached maximum number of tables allowed. Aborting..... \n");
											m_Lock.Release();
											return;
										}
									}
									else
									{
										string func = pString.substr(pos4+1,string::npos);
										lua_pushstring(L,func.c_str());
										lua_gettable(L,-2);
										Lunar<Unit>::push(L, (Unit *)plr);
										lua_pushinteger(L, evt);
										if(obj == NULL) lua_pushnil(L);
										else switch(obj->GetTypeId())
										{
											case TYPEID_UNIT:
											case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
											case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
											default: lua_pushnil(L);
										}
										( (pArena != NULL) ? lua_pushstring(L, pArena->m_name.c_str()) : lua_pushnil(L) );
										lua_pushinteger(L, (misc1 == true) ? 1 : 0);
										lua_pushinteger(L, (misc2 == true) ? 1 : 0);
										int r = lua_pcall(L,6,LUA_MULTRET,0);
										if(r) report(L);
										m_Lock.Release();
										return;
									}
								}
							}
							else
							{
								string func = pString.substr(pos3+1,string::npos);
								lua_pushstring(L,func.c_str());
								lua_gettable(L,-2);
								Lunar<Unit>::push(L, (Unit *)plr);
								lua_pushinteger(L, evt);
								if(obj == NULL) lua_pushnil(L);
								else switch(obj->GetTypeId())
								{
									case TYPEID_UNIT:
									case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
									case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
									default: lua_pushnil(L);
								}
								( (pArena != NULL) ? lua_pushstring(L, pArena->m_name.c_str()) : lua_pushnil(L) );
								lua_pushinteger(L, (misc1 == true) ? 1 : 0);
								lua_pushinteger(L, (misc2 == true) ? 1 : 0);
								int r = lua_pcall(L,6,LUA_MULTRET,0);
								if(r) report(L);
								m_Lock.Release();
								return;
							}
						}
					}
					else
					{
						string func = pString.substr(pos2+1,string::npos);
						lua_pushstring(L,func.c_str());
						lua_gettable(L,-2);
						Lunar<Unit>::push(L, (Unit *)plr);
						lua_pushinteger(L, evt);
						if(obj == NULL) lua_pushnil(L);
						else switch(obj->GetTypeId())
						{
							case TYPEID_UNIT:
							case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
							case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
							default: lua_pushnil(L);
						}
						( (pArena != NULL) ? lua_pushstring(L, pArena->m_name.c_str()) : lua_pushnil(L) );
						lua_pushinteger(L, (misc1 == true) ? 1 : 0);
						lua_pushinteger(L, (misc2 == true) ? 1 : 0);
						int r = lua_pcall(L,6,LUA_MULTRET,0);
						if(r) report(L);
						m_Lock.Release();
						return;
					}
				}
			}
			else
			{
				string func = pString.substr(pos1+1,string::npos);
				lua_pushstring(L,func.c_str());
				lua_gettable(L,-2);
				Lunar<Unit>::push(L, (Unit *)plr);
				lua_pushinteger(L, evt);
				if(obj == NULL) lua_pushnil(L);
				else switch(obj->GetTypeId())
				{
					case TYPEID_UNIT:
					case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
					case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
					default: lua_pushnil(L);
				}
				( (pArena != NULL) ? lua_pushstring(L, pArena->m_name.c_str()) : lua_pushnil(L) );
				lua_pushinteger(L, (misc1 == true) ? 1 : 0);
				lua_pushinteger(L, (misc2 == true) ? 1 : 0);
				int r = lua_pcall(L,6,LUA_MULTRET,0);
				if(r) report(L);
				m_Lock.Release();
				return;
			}
		}	
	}
	else
	{
		lua_pushstring(L, FunctionName);
		lua_gettable(L, LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			printf("Tried to call invalid LUA function '%s' from Ascent! (GO)\n", FunctionName);
			m_Lock.Release();
			return;
		}
		Lunar<Unit>::push(L, (Unit *)plr);
		lua_pushinteger(L, evt);
		if(obj == NULL) lua_pushnil(L);
		else switch(obj->GetTypeId())
		{
			case TYPEID_UNIT:
			case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
			case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
			default: lua_pushnil(L);
		}
		( (pArena != NULL) ? lua_pushstring(L, pArena->m_name.c_str()) : lua_pushnil(L) );
		lua_pushinteger(L, (misc1 == true) ? 1 : 0);
		lua_pushinteger(L, (misc2 == true) ? 1 : 0);
		int r = lua_pcall(L,6,LUA_MULTRET,0);
		if(r) report(L);
		m_Lock.Release();
		return;
	}
}
void LuaEngine::OnServerHook(Player *  plr, uint32 evt,Object *  obj, const char * FunctionName, Quest * pQuest, Guild * pGuild, SpellEntry * Sp)
{
	if(FunctionName==NULL) return;
	m_Lock.Acquire();
	string pString = string(FunctionName);
	size_t pos1,pos2,pos3,pos4,pos5;
	string name1,name2,name3,name4,name5;
	char * key = ".";
	pos1 = pString.find(key);
	pos2 = pString.find(key,pos1+1);
	pos3 = pString.find(key,pos2+1);
	pos4 = pString.find(key,pos3+1);
	pos5 = pString.find(key,pos4+1);
	name1 = pString.substr(0,pos1);
	name2 = pString.substr(pos1+1,pos2-pos1-1);
	name3 = pString.substr(pos2+1,pos3-pos2-1);
	name4 = pString.substr(pos3+1,pos3-pos2-1);
	name5 = pString.substr(pos4+1,string::npos);
	if(pos1 != string::npos)
	{
		lua_pushstring(L,name1.c_str());
		lua_gettable(L,LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name1);
			m_Lock.Release();
			return;
		}
		else if(lua_istable(L,-2))
		{
			if(pos2 != string::npos)
			{
				lua_pushstring(L,name2.c_str());
				lua_gettable(L,-2);
				if(lua_isnil(L,-1))
				{
					printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name2);
					m_Lock.Release();
					return;
				}
				else if(lua_istable(L,-1))
				{
					if(pos3 != string::npos)
					{
						lua_pushstring(L,name3.c_str());
						lua_gettable(L,-2);
						if(lua_isnil(L,-1))
						{
							printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name3);
							m_Lock.Release();
							return;
						}
						else if(lua_istable(L,-1))
						{
							if(pos4 != string::npos)
							{
								lua_pushstring(L,name4.c_str());
								lua_gettable(L,-2);
								if(lua_isnil(L,-1))
								{
									printf("Tried to access an invalid Lua table %s from Ascent(GO). \n",name4);
									m_Lock.Release();
									return;
								}
								else if(lua_istable(L,-1))
								{
									if(pos5 != string::npos)
									{
										lua_pushstring(L,name5.c_str());
										lua_gettable(L,-2);
										if(lua_isnil(L,-1))
										{
											printf("Tried to access and invalid Lua function %s from Ascent(GO). \n",name5);
											m_Lock.Release();
											return;
										}
										else if(lua_istable(L,-1))
										{
											printf("Reached maximum number of tables allowed. Aborting..... \n");
											m_Lock.Release();
											return;
										}
									}
									else
									{
										string func = pString.substr(pos4+1,string::npos);
										lua_pushstring(L,func.c_str());
										lua_gettable(L,-2);
										Lunar<Unit>::push(L, (Unit *)plr);
										lua_pushinteger(L, evt);
										if(obj == NULL) lua_pushnil(L);
										else switch(obj->GetTypeId())
										{
											case TYPEID_UNIT:
											case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
											case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
											default: lua_pushnil(L);
										}
										( (pQuest != NULL) ? lua_pushinteger(L, pQuest->id) : lua_pushnil(L));
										( (pGuild != NULL) ? lua_pushstring(L, pGuild->GetGuildName()) : lua_pushnil(L) );
										( (Sp != NULL) ? lua_pushinteger(L, Sp->Id) : lua_pushnil(L) );
										int r = lua_pcall(L,6,LUA_MULTRET,0);
										if(r)	report(L);
										m_Lock.Release();
										return;
									}
								}
							}
							else
							{
								string func = pString.substr(pos3+1,string::npos);
								lua_pushstring(L,func.c_str());
								lua_gettable(L,-2);
								Lunar<Unit>::push(L, (Unit *)plr);
								lua_pushinteger(L, evt);
								if(obj == NULL) lua_pushnil(L);
								else switch(obj->GetTypeId())
								{
									case TYPEID_UNIT:
									case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
									case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
									default: lua_pushnil(L);
								}
								( (pQuest != NULL) ? lua_pushinteger(L, pQuest->id) : lua_pushnil(L));
								( (pGuild != NULL) ? lua_pushstring(L, pGuild->GetGuildName()) : lua_pushnil(L) );
								( (Sp != NULL) ? lua_pushinteger(L, Sp->Id) : lua_pushnil(L) );
								int r = lua_pcall(L,6,LUA_MULTRET,0);
								if(r)	report(L);
								m_Lock.Release();
								return;
							}
						}
					}
					else
					{
						string func = pString.substr(pos2+1,string::npos);
						lua_pushstring(L,func.c_str());
						lua_gettable(L,-2);
						Lunar<Unit>::push(L, (Unit *)plr);
						lua_pushinteger(L, evt);
						if(obj == NULL) lua_pushnil(L);
						else switch(obj->GetTypeId())
						{
							case TYPEID_UNIT:
							case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
							case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
							default: lua_pushnil(L);
						}
						( (pQuest != NULL) ? lua_pushinteger(L, pQuest->id) : lua_pushnil(L));
						( (pGuild != NULL) ? lua_pushstring(L, pGuild->GetGuildName()) : lua_pushnil(L) );
						( (Sp != NULL) ? lua_pushinteger(L, Sp->Id) : lua_pushnil(L) );
						int r = lua_pcall(L,6,LUA_MULTRET,0);
						if(r)	report(L);
						m_Lock.Release();
						return;
					}
				}
			}
			else
			{
				string func = pString.substr(pos1+1,string::npos);
				lua_pushstring(L,func.c_str());
				lua_gettable(L,-2);
				Lunar<Unit>::push(L, (Unit *)plr);
				lua_pushinteger(L, evt);
				if(obj == NULL) lua_pushnil(L);
				else switch(obj->GetTypeId())
				{
					case TYPEID_UNIT:
					case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
					case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
					default: lua_pushnil(L);
				}
				( (pQuest != NULL) ? lua_pushinteger(L, pQuest->id) : lua_pushnil(L));
				( (pGuild != NULL) ? lua_pushstring(L, pGuild->GetGuildName()) : lua_pushnil(L) );
				( (Sp != NULL) ? lua_pushinteger(L, Sp->Id) : lua_pushnil(L) );
				int r = lua_pcall(L,6,LUA_MULTRET,0);
				if(r)	report(L);
				m_Lock.Release();
				return;
			}
		}	
	}
	else
	{
		lua_pushstring(L, FunctionName);
		lua_gettable(L, LUA_GLOBALSINDEX);
		if(lua_isnil(L,-1))
		{
			printf("Tried to call invalid LUA function '%s' from Ascent! (GO)\n", FunctionName);
			m_Lock.Release();
			return;
		}

		Lunar<Unit>::push(L, (Unit *)plr);
		lua_pushinteger(L, evt);
		if(obj == NULL) lua_pushnil(L);
		else switch(obj->GetTypeId())
		{
			case TYPEID_UNIT:
			case TYPEID_PLAYER: Lunar<Unit>::push(L, TO_UNIT(obj));
			case TYPEID_GAMEOBJECT: Lunar<GameObject>::push(L, TO_GAMEOBJECT(obj));
			default: lua_pushnil(L);
		}
		( (pQuest != NULL) ? lua_pushinteger(L, pQuest->id) : lua_pushnil(L));
		( (pGuild != NULL) ? lua_pushstring(L, pGuild->GetGuildName()) : lua_pushnil(L) );
		( (Sp != NULL) ? lua_pushinteger(L, Sp->Id) : lua_pushnil(L) );
		int r = lua_pcall(L,6,LUA_MULTRET,0);
		if(r)	report(L);
		m_Lock.Release();
	}
}
#endif

static int RegisterUnitEvent(lua_State * L);
static int ModifyUnitEvent(lua_State * L);
static int RemoveUnitEvent(lua_State * L);
static int RegisterQuestEvent(lua_State * L);
static int RegisterGameObjectEvent(lua_State * L);
static int RegisterUnitGossipEvent(lua_State * L);
static int RegisterItemGossipEvent(lua_State * L);
static int RegisterGOGossipEvent(lua_State * L);
#ifdef ENABLE_LUA_HOOKS
static int RegisterServerHookEvent(lua_State * L);
static int ModifyServerHookEvent(lua_State * L);
static int RemoveServerHookEvent(lua_State * L);
static int CheckBoolResult(lua_State * L);
#endif
static int WorldQuery(lua_State * L);

void LuaEngine::RegisterCoreFunctions()
{
	// Unit funcs //
	lua_pushcfunction(L, &RegisterUnitEvent);
	lua_setglobal(L, "RegisterUnitEvent");

	lua_pushcfunction(L,&ModifyUnitEvent);
	lua_setglobal(L,"ModifyUnitEvent");

	lua_pushcfunction(L,&RemoveUnitEvent);
	lua_setglobal(L,"RemoveUnitEvent");
	// GameObject funcs//

	lua_pushcfunction(L, &RegisterGameObjectEvent);
	lua_setglobal(L, "RegisterGameObjectEvent");

	// Quest funcs //
	lua_pushcfunction(L, &RegisterQuestEvent);
	lua_setglobal(L, "RegisterQuestEvent");
    // Unit, Item, GO gossip stuff
    lua_pushcfunction(L, &RegisterUnitGossipEvent);
	lua_setglobal(L, "RegisterUnitGossipEvent"); 
 
    lua_pushcfunction(L, &RegisterItemGossipEvent);
	lua_setglobal(L, "RegisterItemGossipEvent"); 

   	lua_pushcfunction(L, &RegisterGOGossipEvent);
	lua_setglobal(L, "RegisterGOGossipEvent");

#ifdef ENABLE_LUA_HOOKS
	lua_pushcfunction(L, &RegisterServerHookEvent);
	lua_setglobal(L, "RegisterServerHookEvent");

	lua_pushcfunction(L, &ModifyServerHookEvent);
	lua_setglobal(L, "ModifyServerHookEvent");

	lua_pushcfunction(L, &RemoveServerHookEvent);
	lua_setglobal(L, "RemoveServerHookEvent");

	lua_pushcfunction(L, &CheckBoolResult);
	lua_setglobal(L, "CheckBoolResult");
#endif

	lua_pushcfunction(L, &WorldQuery);
	lua_setglobal(L, "WorldQuery");
	
	Lunar<Unit>::Register(L);
	Lunar<Item>::Register(L);
	Lunar<GameObject>::Register(L);
	//Lunar<Quest>::Register(L); quest isn't a class
}
static int WorldQuery(lua_State * L)
{
	const char * query = luaL_checkstring(L,1);
	if(!query) return 0;
	QueryResult * qry = g_luaMgr.WorldQuery(query);
	if(qry != NULL) lua_pushboolean(L,1);
	else lua_pushboolean(L,0);
	return 1;
}

static int RegisterUnitEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

	g_luaMgr.RegisterUnitEvent(entry,ev,str);
	return 0;
}

static int RegisterQuestEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

	g_luaMgr.RegisterQuestEvent(entry,ev,str);
	return 0;
}

static int RegisterGameObjectEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

	g_luaMgr.RegisterGameObjectEvent(entry,ev,str);
	return 0;
}
// Gossip stuff
static int RegisterUnitGossipEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

    g_luaMgr.RegisterUnitGossipEvent(entry, ev, str);
	return 0;
}
static int ModifyUnitEvent(lua_State * L)
{
	int entry = luaL_checkint(L,1);
	int ev = luaL_checkint(L,2);
	const char * str = luaL_checkstring(L,3);
	const char * newstr = luaL_checkstring(L,4);
	if(!entry || !ev) return 0;
	g_luaMgr.ModifyUnitEvent(entry,ev,str,newstr);
	return 0;
}
static int RemoveUnitEvent(lua_State * L)
{
	int entry = luaL_checkint(L,1);
	int ev = luaL_checkint(L,2);
	const char * str = luaL_checkstring(L,3);
	if(!entry || !ev) return 0;
	g_luaMgr.RemoveUnitEvent(entry,ev,str);
	return 0;
}

static int RegisterItemGossipEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

    g_luaMgr.RegisterItemGossipEvent(entry, ev, str);
	return 0;
}

static int RegisterGOGossipEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

    g_luaMgr.RegisterGOGossipEvent(entry, ev, str);
	return 0;
}
#ifdef ENABLE_LUA_HOOKS
static int RegisterServerHookEvent(lua_State * L)
{
	uint32 evt = luaL_checkint(L,1);
	const char * str = luaL_checkstring(L,2);
	if(!evt || !lua_is_starting_up ) return 0;
	g_luaMgr.RegisterServerHookEvent(evt, str);
	return 0;
}
static int ModifyServerHookEvent(lua_State * L)
{
	uint32 evt = luaL_checkint(L,1);
	const char * str = luaL_checkstring(L,2);
	const char * newfunc = luaL_checkstring(L, 3);
	if(!evt) return 0;
	g_luaMgr.ModifyServerHookEvent(evt, str,newfunc);
	return 0;
}
static int RemoveServerHookEvent(lua_State * L)
{
	uint32 evt = luaL_checkint(L,1);
	const char * str = luaL_checkstring(L,2);
	if(!evt) return 0;
	g_luaMgr.RemoveServerHookEvent(evt, str);
	return 0;
}
static int CheckBoolResult(lua_State * L)
{
	uint32 evt = luaL_checkint(L,1);
	const char * func = luaL_checkstring(L,2);
	if(evt == 0 || func == NULL) return 0;
	bool choice = g_luaMgr.CheckBoolResult(evt,func);
	lua_pushboolean(L, (choice == true) ? 1 : 0);
	return 1;
}
#endif

/************************************************************************/
/* Manager Stuff                                                        */
/************************************************************************/
class LuaCreature : public CreatureAIScript
{
public:
	LuaCreature(shared_ptr<Creature> creature) : CreatureAIScript(creature) {};
	~LuaCreature() {};

	void OnCombatStart(shared_ptr<Unit> mTarget)
	{
		uint32 evt = CREATURE_EVENT_ON_ENTER_COMBAT;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mTarget,0);
			}
	}

	void OnCombatStop(shared_ptr<Unit> mTarget)
	{
		uint32 evt = CREATURE_EVENT_ON_LEAVE_COMBAT;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mTarget,0);
			}
	}
	void OnTargetDied(shared_ptr<Unit> mTarget)
	{
		uint32 evt = CREATURE_EVENT_ON_TARGET_DIED;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mTarget,0);
			}
	}

	void OnDied(shared_ptr<Unit> mKiller)
	{
		uint32 evt = CREATURE_EVENT_ON_DIED;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mKiller,0);
			}
	}
	void OnTargetParried(shared_ptr<Unit> mTarget)
	{
		uint32 evt = CREATURE_EVENT_ON_TARGET_PARRIED;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mTarget,0);
			}
	}
	void OnTargetDodged(shared_ptr<Unit> mTarget)
	{
		uint32 evt = CREATURE_EVENT_ON_TARGET_DODGED;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mTarget,0);
			}
	}
	void OnTargetBlocked(shared_ptr<Unit> mTarget, int32 iAmount)
	{
		uint32 evt = CREATURE_EVENT_ON_TARGET_BLOCKED;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mTarget,iAmount);
			}
	}
	void OnTargetCritHit(shared_ptr<Unit> mTarget, float fAmount)
	{
		uint32 evt = CREATURE_EVENT_ON_TARGET_CRIT_HIT;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mTarget,fAmount);
			}
	}
	void OnParried(shared_ptr<Unit> mTarget)
	{
		uint32 evt = CREATURE_EVENT_ON_PARRY;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mTarget,0);
			}
	}
	void OnDodged(shared_ptr<Unit> mTarget)
	{
		uint32 evt = CREATURE_EVENT_ON_DODGED;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mTarget,0);
			}
	}
	void OnBlocked(shared_ptr<Unit> mTarget, int32 iAmount)
	{
		uint32 evt = CREATURE_EVENT_ON_BLOCKED;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mTarget,iAmount);
			}
	}
	void OnCritHit(shared_ptr<Unit> mTarget, float fAmount)
	{
		uint32 evt = CREATURE_EVENT_ON_CRIT_HIT;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mTarget,fAmount);
			}
	}
	void OnHit(shared_ptr<Unit> mTarget, float fAmount)
	{
		uint32 evt = CREATURE_EVENT_ON_HIT;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mTarget,fAmount);
			}
	}
	void OnAssistTargetDied(shared_ptr<Unit> mAssistTarget)
	{
		uint32 evt = CREATURE_EVENT_ON_ASSIST_TARGET_DIED;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mAssistTarget,0);
			}
	}
	void OnFear(shared_ptr<Unit> mFeared, uint32 iSpellId)
	{
		uint32 evt = CREATURE_EVENT_ON_FEAR;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mFeared,iSpellId);
			}
	}
	void OnFlee(shared_ptr<Unit> mFlee)
	{
		uint32 evt = CREATURE_EVENT_ON_FLEE;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mFlee,0);
			}
	}
	void OnCallForHelp()
	{
		uint32 evt = CREATURE_EVENT_ON_CALL_FOR_HELP;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, NULLUNIT,0);
			}
	}
	void OnLoad()
	{
		uint32 evt = CREATURE_EVENT_ON_LOAD;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, NULLUNIT,0);
			}
	}
	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		uint32 evt = CREATURE_EVENT_ON_REACH_WP;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, NULLUNIT,iWaypointId,bForwards);
			}
	}
	void OnLootTaken(shared_ptr<Player> pPlayer, ItemPrototype *pItemPrototype)
	{
		uint32 evt = CREATURE_EVENT_ON_LOOT_TAKEN;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, pPlayer,pItemPrototype);
			}
	}
	void AIUpdate()
	{
		uint32 evt = CREATURE_EVENT_ON_AIUPDATE;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, NULLUNIT,0);
			}
	}
	void OnEmote(shared_ptr<Player> pPlayer, EmoteType Emote)
	{
		uint32 evt = CREATURE_EVENT_ON_EMOTE;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, NULLUNIT, uint32(Emote));
			}
	}
	void StringFunctionCall(const char * pFunction)
	{
		g_engine.CallFunction(_unit, pFunction );
	}
	void OnDamageTaken(shared_ptr<Unit> mAttacker, float fAmount)
	{
		uint32 evt = CREATURE_EVENT_ON_DAMAGE_TAKEN;
		if( m_binding[evt].size() != NULL)
			for(list<const char*>::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++)
			{
				if( (*itr) != NULL)
					g_engine.OnUnitEvent( _unit, (*itr), evt, mAttacker, fAmount);
			}
	}

	void Destroy()
	{
		delete this;
	}

	LuaUnitBinding * m_binding;
};


class LuaGameObject : public GameObjectAIScript
{
public:
	LuaGameObject(shared_ptr<GameObject> go) : GameObjectAIScript(go) {}
	~LuaGameObject() {}

		void OnCreate()
	{
		if( m_binding->Functions[GAMEOBJECT_EVENT_ON_CREATE] != NULL )
			g_engine.OnGameObjectEvent( _gameobject, m_binding->Functions[GAMEOBJECT_EVENT_ON_CREATE], GAMEOBJECT_EVENT_ON_CREATE, NULLUNIT, 0, NULL );
	}
	void OnSpawn()
	{
		if( m_binding->Functions[GAMEOBJECT_EVENT_ON_SPAWN] != NULL )
			g_engine.OnGameObjectEvent( _gameobject, m_binding->Functions[GAMEOBJECT_EVENT_ON_SPAWN], GAMEOBJECT_EVENT_ON_SPAWN, NULLUNIT, 0, NULL );
	}
	void OnDespawn()
	{
		if( m_binding->Functions[GAMEOBJECT_EVENT_ON_DESPAWN] != NULL )
			g_engine.OnGameObjectEvent( _gameobject, m_binding->Functions[GAMEOBJECT_EVENT_ON_DESPAWN], GAMEOBJECT_EVENT_ON_DESPAWN, NULLUNIT, 0, NULL );
	}
	void OnLootTaken(shared_ptr<Player> pLooter, ItemPrototype *pItemInfo)
	{
		if( m_binding->Functions[GAMEOBJECT_EVENT_ON_LOOT_TAKEN] != NULL )
			g_engine.OnGameObjectEvent( _gameobject, m_binding->Functions[GAMEOBJECT_EVENT_ON_LOOT_TAKEN], GAMEOBJECT_EVENT_ON_LOOT_TAKEN, pLooter,0, pItemInfo );
	}
	void OnActivate(shared_ptr<Player> pPlayer)
	{
		if( m_binding->Functions[GAMEOBJECT_EVENT_ON_USE] != NULL )
			g_engine.OnGameObjectEvent( _gameobject, m_binding->Functions[GAMEOBJECT_EVENT_ON_USE], GAMEOBJECT_EVENT_ON_USE, pPlayer, 0, NULL );
	}
	
	void AIUpdate()
	{
		if( m_binding->Functions[GAMEOBJECT_EVENT_AIUPDATE] != NULL )
			g_engine.OnGameObjectEvent( _gameobject, m_binding->Functions[GAMEOBJECT_EVENT_AIUPDATE], GAMEOBJECT_EVENT_AIUPDATE, NULLUNIT, 0, NULL );
	}
	void Destroy ()
	{
		delete this;
	}

	LuaGameObjectBinding * m_binding;
};

class LuaGossip : public GossipScript
{
public:
	LuaGossip() : GossipScript() {}
	~LuaGossip() {}

	void GossipHello(shared_ptr<Object> pObject, shared_ptr<Player> Plr, bool AutoSend) // Dont need GossipHello for GO's
	{
        if(pObject->GetTypeId() == TYPEID_UNIT)
        {
            if( m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_TALK] != NULL )
			    g_engine.OnGossipEvent( pObject, m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_TALK], GOSSIP_EVENT_ON_TALK, Plr, NULL, NULL, NULL );
        }
        else if(pObject->GetTypeId() == TYPEID_ITEM)
        {
            if( m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_TALK] != NULL )
			    g_engine.OnGossipEvent( pObject, m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_TALK], GOSSIP_EVENT_ON_TALK, Plr, NULL, NULL, NULL );
        }
	}

	void GossipSelectOption(shared_ptr<Object> pObject, shared_ptr<Player> Plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
        if(pObject->GetTypeId() == TYPEID_UNIT)
        {
            if( m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION] != NULL )
			    g_engine.OnGossipEvent( pObject, m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION], GOSSIP_EVENT_ON_SELECT_OPTION, Plr, Id, IntId, EnteredCode);
        }
        else if(pObject->GetTypeId() == TYPEID_ITEM)
        {
            if( m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION] != NULL )
                g_engine.OnGossipEvent( pObject, m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION], GOSSIP_EVENT_ON_SELECT_OPTION, Plr, Id, IntId, EnteredCode);
        }
        else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
        {
            if( m_go_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION] != NULL )
                g_engine.OnGossipEvent( pObject, m_go_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION], GOSSIP_EVENT_ON_SELECT_OPTION, Plr, Id, IntId, EnteredCode);
        }
	}

	void GossipEnd(shared_ptr<Object> pObject, shared_ptr<Player> Plr)
	{
        if(pObject->GetTypeId() == TYPEID_UNIT)
        {
		    if( m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_END] != NULL )
			    g_engine.OnGossipEvent( pObject, m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_END], GOSSIP_EVENT_ON_END, Plr, NULL, NULL, NULL );
        }
        else if(pObject->GetTypeId() == TYPEID_ITEM)
        {
            if( m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_END] != NULL )
			    g_engine.OnGossipEvent( pObject, m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_END], GOSSIP_EVENT_ON_END, Plr, NULL, NULL, NULL );
        }
        else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
        {
            if( m_go_gossip_binding->Functions[GOSSIP_EVENT_ON_END] != NULL )
			    g_engine.OnGossipEvent( pObject, m_go_gossip_binding->Functions[GOSSIP_EVENT_ON_END], GOSSIP_EVENT_ON_END, Plr, NULL, NULL, NULL );
        }
	}

	LuaUnitGossipBinding * m_unit_gossip_binding;
	LuaItemGossipBinding * m_item_gossip_binding;
    LuaGOGossipBinding * m_go_gossip_binding;
};

class LuaQuest : public QuestScript
{
public:
	LuaQuest() : QuestScript() {}
	~LuaQuest() {}

	void OnQuestStart(shared_ptr<Player> mTarget, QuestLogEntry *qLogEntry)
	{
		if( m_binding->Functions[QUEST_EVENT_ON_ACCEPT] != NULL )
			g_engine.OnQuestEvent( mTarget, m_binding->Functions[QUEST_EVENT_ON_ACCEPT], qLogEntry->GetQuest()->id, QUEST_EVENT_ON_ACCEPT, mTarget, 0, 0 );
	}

	void OnQuestComplete(shared_ptr<Player> mTarget, QuestLogEntry *qLogEntry)
	{
		if( m_binding->Functions[QUEST_EVENT_ON_COMPLETE] != NULL )
			g_engine.OnQuestEvent( mTarget, m_binding->Functions[QUEST_EVENT_ON_COMPLETE], qLogEntry->GetQuest()->id, QUEST_EVENT_ON_COMPLETE, mTarget, 0, 0 );
	}
	void OnQuestCancel(shared_ptr<Player> mTarget)
	{
		if( m_binding->Functions[QUEST_EVENT_ON_CANCEL] != NULL )
			g_engine.OnQuestEvent( mTarget, m_binding->Functions[QUEST_EVENT_ON_CANCEL], 0, QUEST_EVENT_ON_CANCEL, mTarget, 0 , 0 );
	}
	void OnGameObjectActivate(uint32 entry, shared_ptr<Player> mTarget, QuestLogEntry *qLogEntry)
	{
		if( m_binding->Functions[QUEST_EVENT_GAMEOBJECT_ACTIVATE] != NULL )
			g_engine.OnQuestEvent( mTarget, m_binding->Functions[QUEST_EVENT_GAMEOBJECT_ACTIVATE], qLogEntry->GetQuest()->id, QUEST_EVENT_GAMEOBJECT_ACTIVATE, mTarget, entry, 0 );
	}
	void OnCreatureKill(uint32 entry, shared_ptr<Player> mTarget, QuestLogEntry *qLogEntry)
	{
		if( m_binding->Functions[QUEST_EVENT_ON_CREATURE_KILL] != NULL )
			g_engine.OnQuestEvent( mTarget, m_binding->Functions[QUEST_EVENT_ON_CREATURE_KILL], qLogEntry->GetQuest()->id, QUEST_EVENT_ON_CREATURE_KILL, mTarget, entry , 0 );
	}
	void OnExploreArea(uint32 areaId, shared_ptr<Player> mTarget, QuestLogEntry *qLogEntry)
	{
		if( m_binding->Functions[QUEST_EVENT_ON_EXPLORE_AREA] != NULL )
			g_engine.OnQuestEvent( mTarget, m_binding->Functions[QUEST_EVENT_ON_EXPLORE_AREA], qLogEntry->GetQuest()->id, QUEST_EVENT_ON_EXPLORE_AREA, mTarget, areaId, 0 );
	}
	void OnPlayerItemPickup(uint32 itemId, uint32 totalCount, shared_ptr<Player> mTarget, QuestLogEntry *qLogEntry)
	{
		if( m_binding->Functions[QUEST_EVENT_ON_PLAYER_ITEMPICKUP] != NULL )
			g_engine.OnQuestEvent( mTarget, m_binding->Functions[QUEST_EVENT_ON_PLAYER_ITEMPICKUP], qLogEntry->GetQuest()->id, QUEST_EVENT_ON_PLAYER_ITEMPICKUP, mTarget, itemId, totalCount );
	}

	LuaQuestBinding * m_binding;
};
#ifdef ENABLE_LUA_HOOKS
class pHookInterface : public LuaHookInterface
{
protected:
	LuaSHBinding * m_binding;
	shared_ptr<CompleteMap> compmap;
public:
	pHookInterface() : compmap(new CompleteMap), m_binding(NULL) {}
	friend class LuaEngineMgr; 
	//accessors
	void SetBinding(LuaSHBinding* nBinding ) { m_binding = nBinding; }
	shared_ptr<CompleteMap> GetCompleteMap() { return compmap; }
	//LuaSHBinding GetBinding()  { return m_binding[NUM_SERVER_HOOKS]; }
	//end
	bool lOnNewCharacter(uint32 Race, uint32 Class, WorldSession * Session, const char * Name)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_NEW_CHARACTER;
		bool ret = holdbools[evt];
		if( m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook(NULLPLR,evt,NULLOBJ,(*itr), Race, Class, Name, NULL); }
		}
		return ret;
	}
	void lOnKillPlayer(shared_ptr<Player> pPlayer, shared_ptr<Player> pVictim)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_KILL_PLAYER;
		if( m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, pVictim,(*itr)); }
		}
	}
	void lOnFirstEnterWorld(shared_ptr<Player> pPlayer)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, NULLOBJ, (*itr)); }
		}
	}
	void lOnEnterWorld(shared_ptr<Player> pPlayer)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_ENTER_WORLD;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, NULLOBJ, (*itr)); }
		}
	}
	void lOnGuildCreate(shared_ptr<Player> pLeader, Guild * pGuild)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_GUILD_CREATE;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pLeader, evt, NULLOBJ, (*itr), NULL, pGuild, NULL); }
		}
	}
	void lOnGuildJoin(shared_ptr<Player> pPlayer, Guild * pGuild)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_GUILD_JOIN;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, NULLOBJ, (*itr), NULL, pGuild, NULL); }
		}
	}
	void lOnDeath(shared_ptr<Player> pPlayer)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_DEATH;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer,evt,NULLOBJ, (*itr)); }
		}
	}
	bool lOnRepop(shared_ptr<Player> pPlayer)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_REPOP;
		bool ret = holdbools[evt];
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer,evt,NULLOBJ, (*itr));
				pHoldBoolsByChar * nmap = &(*compmap.get())[evt];
				if(nmap != NULL) nmap->insert(make_pair((*itr),ret)); }
		}
		return ret;
	}
	void lOnEmote(shared_ptr<Player> pPlayer, uint32 Emote)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_EMOTE;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, NULLOBJ,(*itr), Emote); }
		}
	}
	void lOnEnterCombat(shared_ptr<Player> pPlayer, shared_ptr<Unit> pTarget)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_ENTER_COMBAT;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, pTarget, (*itr)); }
		}
	}
	bool lOnCastSpell(shared_ptr<Player> pPlayer, SpellEntry * pSpell)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_CAST_SPELL;
		bool ret = holdbools[evt];
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook(pPlayer, evt, NULLOBJ, (*itr), NULL, NULL, pSpell);
				pHoldBoolsByChar * nmap =  &(*compmap.get())[evt];
				if(nmap != NULL) nmap->insert(make_pair((*itr),ret)); }
		}
		return ret;
	}
	bool lOnLogoutRequest(shared_ptr<Player> pPlayer)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST;
		bool ret = holdbools[evt];
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) 
			{
				if( (*itr) != NULL)
				g_engine.OnServerHook(pPlayer, evt, NULLOBJ, (*itr));
				pHoldBoolsByChar * nmap =  &(*compmap.get())[evt];
				if(nmap != NULL) nmap->insert(make_pair((*itr),ret)); }
		}
		return ret;
	}
	void lOnLogout(shared_ptr<Player> pPlayer)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_LOGOUT;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook(pPlayer, evt, NULLOBJ, (*itr)); }

		}
	}
	void lOnQuestAccept(shared_ptr<Player> pPlayer, Quest * pQuest)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_QUEST_ACCEPT;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
					g_engine.OnServerHook(pPlayer, evt, NULLOBJ, (*itr), pQuest, NULL, NULL); }
		}
	}
	void lOnZone(shared_ptr<Player> pPlayer, uint32 Zone)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_ZONE;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, NULLOBJ, (*itr), Zone); }
		}
	}
	bool lOnChat(shared_ptr<Player> pPlayer, uint32 Type, uint32 Lang, const char * Message, const char * Misc)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_CHAT;
		bool ret = holdbools[evt];
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer,evt,NULLOBJ,(*itr),Type,Lang,Message,Misc);
				pHoldBoolsByChar * nmap = &(*compmap.get())[evt];
				if(nmap != NULL) nmap->insert(make_pair((*itr),ret)); }
		}
		return holdbools[evt];
	}
	/*void lOnLoot(shared_ptr<Player>  pPlayer, shared_ptr<Unit>  pTarget, uint32 Money, uint32 ItemId)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_AREATRIGGER;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, pTarget, (*itr), Money, ItemId); }
		}
	}*/
	void lOnEnterWorld2(shared_ptr<Player>  pPlayer)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_ENTER_WORLD_2;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, NULLOBJ, (*itr)); }
		}
	}
	void lOnCharacterCreate(shared_ptr<Player>  pPlayer)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_CHARACTER_CREATE;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, NULLOBJ, (*itr)); }
		}
	}
	void lOnQuestCancelled(shared_ptr<Player>  pPlayer, Quest * pQuest)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_QUEST_CANCELLED;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, NULLOBJ, (*itr), pQuest, NULL, NULL); }
		}
	}
	void lOnQuestFinished(shared_ptr<Player> pPlayer, Quest * pQuest)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_QUEST_FINISHED;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, NULLOBJ, (*itr), pQuest, NULL, NULL); }
		}
	}
	void lOnHonorableKill(shared_ptr<Player>  pPlayer, shared_ptr<Player>  pKilled)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_HONORABLE_KILL;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, pKilled, (*itr)); }
		}
	}
	void lOnArenaFinish(shared_ptr<Player>  pPlayer, ArenaTeam* pTeam, bool victory, bool rated)
	{
		int32 evt = SERVER_HOOK_EVENT_ON_ARENA_FINISH;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, NULLOBJ, (*itr), pTeam, victory, rated); }
		}
	}
	void lOnAreaTrigger(shared_ptr<Player> pPlayer, uint32 areaTrigger)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_AREATRIGGER;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
				g_engine.OnServerHook( pPlayer, evt, NULLOBJ, (*itr), areaTrigger); }
		}
	}
	void lOnAuraRemove(Player * pPlayer, uint32 spellID)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_AURA_REMOVE;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
					g_engine.OnServerHook(pPlayer,evt,NULLOBJ,(*itr),spellID,0);
			}
		}
	}
	void lOnPostSpellCast(Player * pPlayer, SpellEntry * pSpell, Unit * pTarget)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_POST_SPELL_CAST;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
					g_engine.OnServerHook(pPlayer,evt,pTarget,(*itr),NULL,NULL,pSpell);
			}
		}
	}
	void lOnContinentCreate(shared_ptr<MapMgr>pMgr)
	{
		uint32 evt = SERVER_HOOK_EVENT_ON_CONTIENT_CREATE;
		if(m_binding[evt].size())
		{
			for(LuaSHBinding::iterator itr = m_binding[evt].begin(); itr != m_binding[evt].end(); itr++) {
				if( (*itr) != NULL)
					g_engine.OnServerHook(NULLPLR,evt,NULLOBJ,(*itr),pMgr->GetMapId());
			}
		}
	}
};
#endif
CreatureAIScript * CreateLuaCreature(Creature * src)
{
	LuaUnitBinding * pBinding = g_luaMgr.GetUnitBinding( src->GetEntry() );

	LuaCreature * pLua = new LuaCreature(src);
	pLua->m_binding = pBinding;
	uint32 evt = CREATURE_EVENT_ON_CREATEAISCRIPT;
	for(LuaUnitBinding::iterator itr = pBinding[evt].begin(); itr != pBinding[evt].end(); itr++)
	{
		g_engine.OnUnitEvent( src,(*itr),evt,NULLUNIT,0);
	}
	return pLua;
}

GameObjectAIScript * CreateLuaGameObject(GameObject * src)
{
	LuaGameObjectBinding * pBinding = g_luaMgr.GetGameObjectBinding( src->GetEntry() );
	LuaGameObject * pLua = new LuaGameObject(src);
	pLua->m_binding = pBinding;
	return pLua;
}

QuestScript * CreateLuaQuestScript(uint32 id)
{
	LuaQuestBinding * pBinding = g_luaMgr.GetQuestBinding( id );
	LuaQuest * pLua = new LuaQuest();
	pLua->m_binding = pBinding;
	return pLua;
}
// Gossip stuff
GossipScript * CreateLuaUnitGossipScript(uint32 id)
{
    LuaUnitGossipBinding * pBinding = g_luaMgr.GetLuaUnitGossipBinding( id );
	LuaGossip * pLua = new LuaGossip();
	pLua->m_unit_gossip_binding = pBinding;
	return pLua;
}

GossipScript * CreateLuaItemGossipScript(uint32 id)
{
    LuaItemGossipBinding * pBinding = g_luaMgr.GetLuaItemGossipBinding( id );
	if( pBinding == NULL )
		return NULL;

	LuaGossip * pLua = new LuaGossip();
	pLua->m_item_gossip_binding = pBinding;
	return pLua;
}

GossipScript * CreateLuaGOGossipScript(uint32 id)
{
    LuaGOGossipBinding * pBinding = g_luaMgr.GetLuaGOGossipBinding( id );
	LuaGossip * pLua = new LuaGossip();
    pLua->m_go_gossip_binding = pBinding;
	return pLua;
}
/* LuaEngineMgr Constructor */
LuaEngineMgr::LuaEngineMgr() {
#ifdef ENABLE_LUA_HOOKS
	m_shBinding = new LuaSHBinding[NUM_SERVER_HOOKS];
	m_HookInterface = new pHookInterface;
#endif
}

#ifdef ENABLE_LUA_HOOKS
bool LuaEngineMgr::CheckBoolResult(uint32 evt, const char * functionname)
{
	ASSERT( evt < NUM_SERVER_HOOKS);
	pHoldBoolsByChar * nmap;
	switch(evt)
	{
	case 1:
	case 7:
	case 10:
	case 12:
	case 16:
		nmap = &(*m_HookInterface->GetCompleteMap().get())[evt];
		return (*nmap)[functionname];
	default:
		return true;
	}
}
#endif
void LuaEngineMgr::Startup()
{
	Log.Notice("Lua/BluaEngineMgr", "Spawning Lua Engine...");
	lua_is_starting_up = true;
	g_engine = m_engine;
	m_engine.LoadScripts();
	lua_is_starting_up = false;
#ifdef ENABLE_LUA_HOOKS
	m_HookInterface->SetBinding(m_shBinding);
#endif

	// stuff is registered, so lets go ahead and make our emulated C++ scripted lua classes.
	for(UnitBindingMap::iterator itr = m_unitBinding.begin(); itr != m_unitBinding.end(); ++itr)
	{
		m_scriptMgr->register_creature_script( itr->first, CreateLuaCreature );
	}

	for(GameObjectBindingMap::iterator itr = m_gameobjectBinding.begin(); itr != m_gameobjectBinding.end(); ++itr)
	{
		m_scriptMgr->register_gameobject_script( itr->first, CreateLuaGameObject );
	}

	for(QuestBindingMap::iterator itr = m_questBinding.begin(); itr != m_questBinding.end(); ++itr)
	{
		QuestScript * qs = CreateLuaQuestScript( itr->first );
		if( qs != NULL )
			m_scriptMgr->register_quest_script( itr->first, qs );
	}
    // Register Gossip Stuff
    for(GossipUnitScriptsBindingMap::iterator itr = m_unit_gossipBinding.begin(); itr != m_unit_gossipBinding.end(); ++itr)
	{
		GossipScript * gs = CreateLuaUnitGossipScript( itr->first );
		if( gs != NULL )
			m_scriptMgr->register_gossip_script( itr->first, gs );
    }

    for(GossipItemScriptsBindingMap::iterator itr = m_item_gossipBinding.begin(); itr != m_item_gossipBinding.end(); ++itr)
	{
		GossipScript * gs = CreateLuaItemGossipScript( itr->first );
		if( gs != NULL )
			m_scriptMgr->register_item_gossip_script( itr->first, gs );
    }

    for(GossipGOScriptsBindingMap::iterator itr = m_go_gossipBinding.begin(); itr != m_go_gossipBinding.end(); ++itr)
	{
		GossipScript * gs = CreateLuaGOGossipScript( itr->first );
		if( gs != NULL )
			m_scriptMgr->register_go_gossip_script( itr->first, gs );
    }
#ifdef ENABLE_LUA_HOOKS
	m_scriptMgr->register_lua_hook(m_HookInterface);
#endif
}

QueryResult * LuaEngineMgr::WorldQuery(const char * query)
{
	if(query == NULL) return NULL;
	QueryResult * Qr = WorldDatabase.Query(query);
	if(Qr != NULL) return Qr;
	else return NULL;
}

void LuaEngineMgr::RegisterUnitEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
	ASSERT( Event < CREATURE_EVENT_COUNT);
	UnitBindingMap::iterator itr = m_unitBinding.find(Id);
	if(itr == m_unitBinding.end())
	{
		LuaUnitBinding * ub = new LuaUnitBinding[CREATURE_EVENT_COUNT];
		ub[Event].push_back(strdup(FunctionName));
		m_unitBinding.insert(make_pair(Id,ub));
	}
	else
	{
		if( itr->second[Event].size() != NULL)
		{
			for(LuaUnitBinding::iterator it = itr->second[Event].begin(); it != itr->second[Event].end(); it++)
			{
				if( (*it) == FunctionName)
				{
					printf("LuaEngineMgr::RegisterUnitEvent tried to register duplicate function %s",(*it));
					free( (void*)(*it));
					itr->second[Event].erase( it);
				}
			}
		}
		itr->second[Event].push_back(strdup(FunctionName));
	}
}
void LuaEngineMgr::ModifyUnitEvent(uint32 id, uint32 evt, const char *oldfunc, const char *newfunc)
{
	ASSERT( evt < CREATURE_EVENT_COUNT);
	UnitBindingMap::iterator itr = m_unitBinding.find(id);
	if( itr != m_unitBinding.end())
	{
		uint8 count = 0;
		if(itr->second[evt].size() != 0)
		{
			for(LuaUnitBinding::iterator it = itr->second[evt].begin(); it != itr->second[evt].end(); it++)
			{
				count++;
				if( (*it) != NULL && !strcmp( (*it),oldfunc))
				{
					free((void*)(*it));
					itr->second[evt].erase(it);
					itr->second[evt].push_back(newfunc);
				}
			}
			if(count == itr->second[evt].size())
				itr->second[evt].push_back(newfunc);
		}
	}
}
void LuaEngineMgr::RemoveUnitEvent(uint32 id, uint32 evt, const char * FunctionName)
{
	UnitBindingMap::iterator itr = m_unitBinding.find(id);
	if(itr != m_unitBinding.end())
	{
		uint8 count = 0;
		if(itr->second[evt].size() != 0)
		{
			for(LuaUnitBinding::iterator it = itr->second[evt].begin(); it != itr->second[evt].end(); it++)
			{
				count++;
				if( (*it) != NULL && !strcmp( (*it), FunctionName))
				{
					free( (void*)(*it));
					itr->second[evt].erase(it);
				}
			}
			if(count == itr->second[evt].size())
				printf("LuaEngineMgr::RemoveUnitEvent : Failed to locate function %s in creature id %u and event %u. \n",FunctionName,id,evt);
		}
	}
}

void LuaEngineMgr::RegisterQuestEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
	QuestBindingMap::iterator itr = m_questBinding.find(Id);
	if(itr == m_questBinding.end())
	{
		LuaQuestBinding qb;
		memset(&qb,0,sizeof(LuaQuestBinding));
		qb.Functions[Event] = strdup(FunctionName);
		m_questBinding.insert(make_pair(Id,qb));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}
void LuaEngineMgr::RegisterGameObjectEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
	GameObjectBindingMap::iterator itr = m_gameobjectBinding.find(Id);
	if(itr == m_gameobjectBinding.end())
	{
		LuaGameObjectBinding ub;
		memset(&ub,0,sizeof(LuaGameObjectBinding));
		ub.Functions[Event] = strdup(FunctionName);
		m_gameobjectBinding.insert(make_pair(Id,ub));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}
// Gossip Events
void LuaEngineMgr::RegisterUnitGossipEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
    GossipUnitScriptsBindingMap::iterator itr = m_unit_gossipBinding.find(Id);
 
    if(itr == m_unit_gossipBinding.end())
	{
		LuaUnitGossipBinding gb;
		memset(&gb, 0, sizeof(LuaUnitGossipBinding));
		gb.Functions[Event] = strdup(FunctionName);
		m_unit_gossipBinding.insert(make_pair(Id, gb));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}

void LuaEngineMgr::RegisterItemGossipEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
    GossipItemScriptsBindingMap::iterator itr = m_item_gossipBinding.find(Id);

    if(itr == m_item_gossipBinding.end())
	{
		LuaItemGossipBinding gb;
		memset(&gb, 0, sizeof(LuaItemGossipBinding));
		gb.Functions[Event] = strdup(FunctionName);
		m_item_gossipBinding.insert(make_pair(Id, gb));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}

#ifdef ENABLE_LUA_HOOKS
void LuaEngineMgr::RegisterServerHookEvent(uint32 evt, const char *FunctionName)
{
	ASSERT( evt < NUM_SERVER_HOOKS);
	if(m_shBinding[evt].size() != 0)
	{
		for(LuaSHBinding::iterator itr = m_shBinding[evt].begin(); itr != m_shBinding[evt].end(); itr++)
			{
			if( (*itr) != NULL && !strcmp( (*itr),FunctionName))
			{
				free( (void*)(*itr));
				(*itr) = NULL;
				m_shBinding[evt].erase(itr);
			}
		}
	}
	m_shBinding[evt].push_back(strdup(FunctionName));
}
void LuaEngineMgr::ModifyServerHookEvent(uint32 evt, const char * func1, const char * func2)
{
	ASSERT( evt < NUM_SERVER_HOOKS);
	if(m_shBinding[evt].size() != 0)
	{
		uint8 count = 0;
		for(LuaSHBinding::iterator itr = m_shBinding[evt].begin(); itr != m_shBinding[evt].end(); itr++)
		{
			count++;
			if( (*itr) != NULL && !strcmp( (*itr), func1))
			{
				free( (void*)(*itr));
				(*itr) = NULL;
				m_shBinding[evt].erase(itr);
				m_shBinding[evt].push_back(strdup(func2));
			}
			if(count == m_shBinding[evt].size())
				m_shBinding[evt].push_back(strdup(func2));
		}
	}
}
void LuaEngineMgr::RemoveServerHookEvent(uint32 evt, const char * func)
{
	ASSERT( evt < NUM_SERVER_HOOKS);
	if(m_shBinding[evt].size() != 0)
	{
		for(LuaSHBinding::iterator itr = m_shBinding[evt].begin(); itr != m_shBinding[evt].end(); itr++)
		{
			if( (*itr) != NULL && !strcmp( (*itr),func))
			{
				delete (*itr);
				(*itr) = NULL;
				m_shBinding[evt].erase(itr);
			}
		}
	}
}
#endif

void LuaEngineMgr::RegisterGOGossipEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
    GossipGOScriptsBindingMap::iterator itr = m_go_gossipBinding.find(Id);

    if(itr == m_go_gossipBinding.end())
	{
		LuaGOGossipBinding gb;
		memset(&gb, 0, sizeof(LuaGOGossipBinding));
		gb.Functions[Event] = strdup(FunctionName);
		m_go_gossipBinding.insert(make_pair(Id, gb));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}

/*void LuaEngineMgr::ReloadScripts()
{
	m_lock.Acquire();

	// acquire the locks on all the luaengines so they don't do anything.
	for(LuaEngineMap::iterator itr = m_engines.begin(); itr != m_engines.end(); ++itr)
		itr->first->GetLock().Acquire();

	// remove all the function name bindings
	for(UnitBindingMap::iterator itr = m_unitBinding.begin(); itr != m_unitBinding.end(); ++itr)
	{
		for(uint32 i = 0; i < CREATURE_EVENT_COUNT; ++i)
			if(itr->second.Functions[i] != NULL)
				free((void*)itr->second.Functions[i]);
	}
	
	for(GameObjectBindingMap::iterator itr = m_gameobjectBinding.begin(); itr != m_gameobjectBinding.end(); ++itr)
	{
		for(uint32 i = 0; i < GAMEOBJECT_EVENT_COUNT; ++i)
			if(itr->second.Functions[i] != NULL)
				free((void*)itr->second.Functions[i]);
	}

	// clear the maps
	m_gameobjectBinding.clear();
	m_unitBinding.clear();

	// grab the first lua engine in the list, use it to re-create all the binding names.
	LuaEngine * l = m_engines.begin()->first;
	lua_is_starting_up = true;
	l->Restart();
	lua_is_starting_up = false;

	// all our bindings have been re-created, go through the lua engines and restart them all, and then release their locks.
	for(LuaEngineMap::iterator itr = m_engines.begin(); itr != m_engines.end(); ++itr)
	{
		if(itr->first != l)		// this one is already done
		{
			itr->first->Restart();
			itr->first->GetLock().Release();
		}
	}

	// release the big lock
	m_lock.Release();
}*/

void LuaEngineMgr::Unload()
{
	g_engine.GetLock().Acquire();

	for(UnitBindingMap::iterator itr = m_unitBinding.begin(); itr != m_unitBinding.end(); ++itr)
	{
		delete [] itr->second;
	}
	for(GameObjectBindingMap::iterator itr = m_gameobjectBinding.begin(); itr != m_gameobjectBinding.end(); ++itr)
	{
		for(uint32 i = 0; i < GAMEOBJECT_EVENT_COUNT; ++i)
			if(itr->second.Functions[i] != NULL)
				free((void*)itr->second.Functions[i]);
	}
	for(GossipUnitScriptsBindingMap::iterator itr = m_unit_gossipBinding.begin(); itr != m_unit_gossipBinding.end(); ++itr)
	{
		for(uint32 i = 0; i < GOSSIP_EVENT_COUNT; ++i)
			if(itr->second.Functions[i] != NULL)
				free((void*)itr->second.Functions[i]);
	}

	for(GossipItemScriptsBindingMap::iterator itr = m_item_gossipBinding.begin(); itr != m_item_gossipBinding.end(); ++itr)
	{
		for(uint32 i = 0; i < GOSSIP_EVENT_COUNT; ++i)
			if(itr->second.Functions[i] != NULL)
				free((void*)itr->second.Functions[i]);
	}
	for(GossipGOScriptsBindingMap::iterator itr = m_go_gossipBinding.begin(); itr != m_go_gossipBinding.end(); ++itr)
	{
		for(uint32 i = 0; i < GOSSIP_EVENT_COUNT; ++i)
			if(itr->second.Functions[i] != NULL)
				free((void*)itr->second.Functions[i]);
	}
#ifdef ENABLE_LUA_HOOKS
	for(int i = 0; i < NUM_SERVER_HOOKS; i++)
	{
		m_shBinding[i].clear();
	}
	m_scriptMgr->unregister_lua_hook(m_HookInterface);
#endif
	m_gameobjectBinding.clear();
	m_unitBinding.clear();
	m_unit_gossipBinding.clear();
	m_item_gossipBinding.clear();
	m_go_gossipBinding.clear();
	g_engine.Restart();
	ReloadClasses();
	g_engine.GetLock().Release();
}

void LuaEngine::Restart()
{
	sLog.outString("Reloading Scripts....");
	lua_is_starting_up = true;
	g_engine.LoadScripts();
	lua_is_starting_up = false;
}

void LuaEngineMgr::ReloadClasses()
{
#ifdef ENABLE_LUA_HOOKS
	m_HookInterface = new pHookInterface;
	m_HookInterface->SetBinding(m_shBinding);
#endif

	for(UnitBindingMap::iterator itr = m_unitBinding.begin(); itr != m_unitBinding.end(); ++itr)
	{
	m_scriptMgr->register_creature_script( itr->first, CreateLuaCreature );
	}
	for(GameObjectBindingMap::iterator itr = m_gameobjectBinding.begin(); itr != m_gameobjectBinding.end(); ++itr)
	{
	m_scriptMgr->register_gameobject_script( itr->first, CreateLuaGameObject );
	}
	for(QuestBindingMap::iterator itr = m_questBinding.begin(); itr != m_questBinding.end(); ++itr)
	{
		QuestScript * qs = CreateLuaQuestScript( itr->first );
		if( qs != NULL )
			m_scriptMgr->register_quest_script( itr->first, qs );
	}
    for(GossipUnitScriptsBindingMap::iterator itr = m_unit_gossipBinding.begin(); itr != m_unit_gossipBinding.end(); ++itr)
 	{
		GossipScript * gs = CreateLuaUnitGossipScript( itr->first );
 		if( gs != NULL )
 			m_scriptMgr->register_gossip_script( itr->first, gs );
	}
	for(GossipItemScriptsBindingMap::iterator itr = m_item_gossipBinding.begin(); itr != m_item_gossipBinding.end(); ++itr)
	{
		GossipScript * gs = CreateLuaItemGossipScript( itr->first );
		if( gs != NULL )
			m_scriptMgr->register_item_gossip_script( itr->first, gs );
    }

	for(GossipGOScriptsBindingMap::iterator itr = m_go_gossipBinding.begin(); itr != m_go_gossipBinding.end(); ++itr)
	{
		GossipScript * gs = CreateLuaGOGossipScript( itr->first );
		if( gs != NULL )
			m_scriptMgr->register_go_gossip_script( itr->first, gs );
    }
#ifdef ENABLE_LUA_HOOKS
	m_scriptMgr->register_lua_hook(m_HookInterface);
#endif
}

/************************************************************************/
/* SCRIPT FUNCTION IMPLEMENTATION                                       */
/************************************************************************/
#define CHECK_TYPEID(expected_type) if(!ptr || !ptr->IsInWorld() || ptr->GetTypeId() != expected_type) { return 0; }
#define CHECK_TYPEID_RET(expected_type) if(!ptr || !ptr->IsInWorld() || ptr->GetTypeId() != expected_type) { lua_pushboolean(L,0); return 0; }
#define CHECK_TYPEID_RET_INT(expected_type) if(!ptr || !ptr->IsInWorld() || ptr->GetTypeId() != expected_type) { lua_pushinteger(L,0); return 0; }
#define CHECK_PTR() if( ptr == NULL ||!ptr->IsInWorld() ) { return 0; }
#include "NameSpaces\GameObjectFunctions.cpp"
#include "NameSpaces\UnitFunctions.cpp"
#include "NameSpaces\ItemFunctions.cpp"
