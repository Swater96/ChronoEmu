	// Full Credits To LuaAppArc
#ifndef ITEMFUNCTIONS_H
#define ITEMFUNCTIONS_H

namespace luaItem
{
	int GossipCreateMenu(lua_State * L, Item *  ptr);
	int GossipMenuAddItem(lua_State * L, Item *  ptr);
	int GossipSendMenu(lua_State * L, Item *  ptr);
	int GossipComplete(lua_State * L, Item *  ptr);
	int GossipSendPOI(lua_State * L, Item *  ptr);
}
#endif