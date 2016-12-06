		// Full Credits to LUAppArc

#include "StdAfx.h"
#include "LUAEngine.h"
#include "ItemFunctions.h"

namespace luaItem
{
	int GossipCreateMenu(lua_State * L, Item *  ptr)
	{
		int text_id = luaL_checkint(L, 1);
		Unit * target = Lunar<Unit>::check(L, 2);
 		int autosend = luaL_checkint(L, 3);
		if(target->GetTypeId() != TYPEID_PLAYER) return 0;

		shared_ptr<Player> plr = TO_PLAYER(target);
	    
		objmgr.CreateGossipMenuForPlayer(&Menu, ptr->GetGUID(), text_id, plr);
		if(autosend)
			Menu->SendTo(plr);
		return 1;
	}
	int GossipMenuAddItem(lua_State * L, Item *  ptr)
	{
		int icon = luaL_checkint(L, 1);
		const char * menu_text = luaL_checkstring(L, 2);
		int IntId = luaL_checkint(L, 3);
		int extra = luaL_checkint(L, 4);

		Menu->AddItem(icon, menu_text, IntId, (extra > 0) ? true : false);
		return 1;
	}
	int GossipSendMenu(lua_State * L, Item *  ptr)
	{
		Unit * target = Lunar<Unit>::check(L, 1);
		if(target->GetTypeId() != TYPEID_PLAYER) return 0;
		shared_ptr<Player> plr = TO_PLAYER(target);
		Menu->SendTo(plr);
		return 1;
	}
	int GossipComplete(lua_State * L, Item *  ptr)
	{
		Unit * target = Lunar<Unit>::check(L, 1);
		if(target->GetTypeId() != TYPEID_PLAYER) return 0;
		shared_ptr<Player> plr = TO_PLAYER(target);
		plr->Gossip_Complete();
		return 1;
	}
	int GossipSendPOI(lua_State * L, Item *  ptr)
	{
		Unit * target = Lunar<Unit>::check(L, 1);
		Player *  plr = TO_PLAYER(target);
		float x = (float)luaL_checknumber(L, 2);
		float y = (float)luaL_checknumber(L, 3);
		int icon = luaL_checkint(L, 4);
		int flags = luaL_checkint(L, 5);
		int data = luaL_checkint(L, 6);
		const char * name = luaL_checkstring(L, 7);
		if(target->GetTypeId() != TYPEID_PLAYER) return 0;

		plr->Gossip_SendPOI(x, y, icon, flags, data, name);
		return 1;
	}
}
