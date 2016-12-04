/*
 * ChronoEmu Scripts - Scholomance Header
 * Copyright (C) 2010 ForsakenGaming <http://forsakengaming.com/>
 * Developed by SideWays
 */

#include "StdAfx.h"
#include "Setup.h"
#include "../Common/Base.h"

static Coords KirtonosWP[]=
{
	{ 323.887756f, 93.449120f, 106.078842f, 3.147874f },
	{ 318.731384f, 93.563095f, 101.643738f, 3.120385f }
};

static Coords StudentSpawn[]=
{
	{ 94.8988f, 73.5054f, 97.7763f },
	{ 109.245f, 68.8032f, 98.9383f },
	{ 131.626f, 86.589f, 102.056f },
	{ 112.661f, 86.179f, 99.6562f },
	{ 110.322f, 84.9862f, 99.9234f },
	{ 111.239f, 64.8379f, 99.035f },
	{ 123.59f, 79.7615f, 101.698f },
	{ 120.956f, 97.073f, 101.359f },
	{ 123.5f, 99.3146f, 102.006f },
	{ 123.571f, 95.8893f, 101.615f },
	{ 126.328f, 81.184f, 101.868f },
	{ 113.776f, 68.9133f, 99.8338f },
	{ 130.442f, 73.1535f, 102.441f },
	{ 125.2f, 106.638f, 101.826f },
	{ 139.847f, 68.5642f, 103.207f },
	{ 144.228f, 68.1884f, 103.473f },
	{ 143.114f, 65.1004f, 103.226f },
	{ 133.769f, 82.9655f, 102.423f },
	{ 110.434f, 101.785f, 100.11f },
	{ 103.822f, 106.933f, 98.6173f },
	{ 105.327f, 110.351f, 98.8685f },
	{ 97.1261f, 97.5603f, 97.8059f },
	{ 95.2852f, 76.5092f, 97.7875f },
	{ 110.395f, 87.146f, 99.8255f },
	{ 118.181f, 113.647f, 101.827f },
	{ 110.053f, 98.8023f, 99.7995f },
	{ 99.9902f, 95.1306f, 97.7582f },
	{ 96.7977f, 93.9028f, 97.416f },
	{ 118.303f, 115.577f, 101.587f },
	{ 97.9246f, 75.0885f, 97.9626f },
	{ 107.033f, 106.829f, 99.1358f },
	{ 136.103f, 87.3239f, 102.13f }
};

#define GO_FLAG_UNCLICKABLE 0x00000010

#define CN_KIRTONOS						10506	
#define CURSE_OF_TONGUES				12889 	
#define DISARM							8379	
#define PIERCE_ARMOR					6016	
#define SHADOWBOLT_VOLLEY				17228
#define SWOOP							18144	
#define WING_FLAP						12882
#define KIRTONOS_TRANSFORM				16467
#define K_DOMINATE_MIND					14515

#define CN_DOCTOR_THEOLEN_KRASTINOV		11261	
#define REND							18106            
#define BACKHAND						18103
#define FRENZY							28371

#define CN_RATTLEGORE					11622
#define KNOCK_AWAY						10101            
#define STRIKE							18368
#define WAR_STOMP						16727

#define CN_MARDUK_BLACKPOOL				10433	
#define MB_CLEAVE						15284            
#define MB_DEFILING_AURA				17695
#define MB_SHADOWBOLT_V					17228
#define MB_SHADOW_SHIELD				12040

#define CN_INSTRUCTOR_MALICIA			10505	
#define CALL_OF_GRAVE					17831	
#define CORRUPTION						11672  
#define FLASH_HEAL						17843	
#define RENEW							8362 //-- this heals players too.. wtf?
#define HEAL							15586	
#define SLOW							13747

#define CN_THE_RAVENIAN					10507
#define TRAMPLE							15550 	
#define RAVENIANCLEAVE					20691 
#define SUNDERINCLEAVE					17963
#define KNOCKAWAY						10101   

#define CN_LADY_ILLUCIA_BAROV			10502	
#define CURSE_OF_AGONY					18671
#define SHADOW_SHOCK					17289	
#define SILENCE							12528	
#define ILLUCIA_FEAR					12542	
#define DOMINATE_MIND					14515	

#define CN_RAS_FROSTWHISPER				10508	
#define FROSTBOLT						21369	
#define ICE_ARMOR						18100	
#define FREEZE							18763	
#define RAS_FEAR						12096	
#define CHILL_NOVA						18099	
#define FROSTBOLT_VOLLEY				8398	
#define RAS_KNOCK						11130

#define CN_JANDICE_BAROV				10503
#define CURSE_OF_BLOOD					16098
#define SUMMON_ILLUSION					17773
#define BANISH							8994

/*#define CN_KORMOK 14491	

#define SHADOW_BOLT_VOLLEY	20741            
#define BONE_SHIELD			27688
#define SUMMON_RISEY_LACKEY 17618*/

#define CN_VECTUS						10432	
#define FLAMESTRIKE						18399       
#define BLAST_WAVE						16046
#define V_FRENZY						28371

#define CN_LORD_ALEXEI_BAROV			10504	
#define UNHOLY_AURA						17467
#define VEIL_OF_SHADOW					17820

#define CN_LOREKEEPER_POLKELT			10901	
#define VOLATILE_INFECTION				18149 	
#define CORROSIVE_ACID					8245	
#define NOXIOUS_CATALYST				18151

#define CN_DARKMASTER_GANDLING			1853	
#define ARCANE_MISSILES					15791
#define CURSE_OF_THE_DARKMASTER			18702
#define SHADOW_SHIELD					22417
#define SHADOW_PORTAL					17950


