#ifndef GOFUNCTIONS_H
#define GOFUNCTIONS_H

namespace luaGameObject
{
	// Begin - GO - Gossip Functions.
	int GossipCreateMenu(lua_State * L, GameObject * ptr);
	int GossipMenuAddItem(lua_State * L, GameObject * ptr);
	int GossipSendMenu(lua_State * L, GameObject * ptr);
	int GossipComplete(lua_State * L, GameObject * ptr);
	int GossipSendPOI(lua_State * L, GameObject * ptr);
	// End - Go - Gossip Functions.
	int PlaySoundToSet(lua_State * L, GameObject * ptr);
	int GetName(lua_State * L, GameObject * ptr);
	int Teleport(lua_State * L, GameObject * ptr);
	int GetClosestPlayer(lua_State * L, GameObject * ptr);
	int GetZoneId(lua_State *L, GameObject * ptr);
	int GetItemCount(lua_State * L, GameObject * ptr);
	int GetSpawnX(lua_State * L, GameObject * ptr);
	int GetSpawnY(lua_State * L, GameObject * ptr);
	int GetSpawnZ(lua_State * L, GameObject * ptr);
	int GetSpawnO(lua_State * L, GameObject * ptr);
	int GetInRangePlayersCount(lua_State * L, GameObject * ptr);
	int GetEntry(lua_State * L, GameObject * ptr);
	int GetX(lua_State * L, GameObject * ptr);
	int GetY(lua_State * L, GameObject * ptr);
	int GetZ(lua_State * L, GameObject * ptr);
	int GetO(lua_State * L, GameObject * ptr);
	int GetInRangePlayers(lua_State * L, GameObject * ptr);
	int GetInRangeGameObjects(lua_State * L, GameObject * ptr);
	int GetInstanceID(lua_State * L, GameObject * ptr);
	int GetUInt64Value(lua_State * L, GameObject * ptr);
	int GetUInt32Value(lua_State * L, GameObject * ptr);
	int GetFloatValue(lua_State * L, GameObject * ptr);
	int GetGUID(lua_State * L, GameObject *  ptr);
	int AddItem(lua_State * L, GameObject * ptr);
	int Despawn(lua_State * L, GameObject * ptr);
	int IsInWorld(lua_State * L, GameObject * ptr);
	int IsInBack(lua_State * L, GameObject * ptr);
	int IsInFront(lua_State * L, GameObject * ptr);
	int SpawnCreature(lua_State * L, GameObject * ptr);
	int SpawnGameObject(lua_State * L, GameObject * ptr);
	int CalcDistance(lua_State * L, GameObject * ptr);
	int SetOrientation(lua_State * L, GameObject * ptr);
	int RemoveFromWorld(lua_State * L, GameObject * ptr);
	int CalcRadAngle(lua_State * L, GameObject * ptr);
	int SetUInt32Value(lua_State * L, GameObject * ptr);
	int SetUInt64Value(lua_State * L, GameObject * ptr);
	int SetFloatValue(lua_State * L, GameObject * ptr);
	int ModUInt32Value(lua_State * L, GameObject * ptr);
	int CastSpell(lua_State * L, GameObject * ptr);
	int FullCastSpell(lua_State * L, GameObject * ptr);
	int CastSpellOnTarget(lua_State * L, GameObject * ptr);
	int FullCastSpellOnTarget(lua_State * L, GameObject * ptr);
	int Despawn(lua_State * L, GameObject * ptr);
	int RegisterAIUpdateEvent(lua_State * L, GameObject * ptr);
	int ModifyAIUpdateEvent(lua_State * L, GameObject * ptr);
	int RemoveAIUpdateEvent(lua_State * L, GameObject * ptr);
	int OnGameObjectEvent(lua_State * L, GameObject * ptr);
	int GetCreatureNearestCoords(lua_State * L, GameObject * ptr);
	int GetGameObjectNearestCoords(lua_State * L, GameObject * ptr);
	/* Date : 01/07/2009 */
	int SetSpawnPosition(lua_State * L, GameObject * ptr);
	int SaveToDB(lua_State * L, GameObject * ptr);
	int DeleteFromDB(lua_State * L, GameObject * ptr);
	int WorldQuery(lua_State * L, GameObject * ptr);
	/* End Date: 01/07/2009 */
}
#endif
