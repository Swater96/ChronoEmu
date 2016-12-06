/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"

// Event of Hallowen control
//#define EVENT_HALLOWEEN		// Decomment this for enable the event on Innkeepers

#define SPELL_TRICK_OR_TREATED  24755
#define SPELL_TREAT             24715
#define TRICK_OR_TREAT			"Trick or Treat!"
// -------------------------

class InnkeeperGossip : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player * Plr, bool AutoSend);
    void GossipSelectOption(Object * pObject, Player * Plr, uint32 Id, uint32 IntId, const char * Code);
    void GossipEnd(Object * pObject, Player * Plr);
	void Destroy()
	{
		delete this;
	}
};

void InnkeeperGossip::GossipHello(Object * pObject, Player * Plr, bool AutoSend)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULLCREATURE;
	if(pCreature==NULLCREATURE)
		return;

    GossipMenu *Menu;

    uint32 TextID = 820;
	uint32 Text = objmgr.GetGossipTextForNpc(pCreature->GetEntry());
	if(Text != 0)
	{
		GossipText * text = NpcTextStorage.LookupEntry(Text);
		if(text != 0)
			TextID = Text;
	}
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextID, Plr);

	#ifdef	EVENT_HALLOWEEN
	  if(!Plr->HasAura(SPELL_TRICK_OR_TREATED))
	     Menu->AddItem( 0, TRICK_OR_TREAT, 4 );
	#endif

    if( pObject->GetUInt32Value(UNIT_NPC_FLAGS) & UNIT_NPC_FLAG_VENDOR )
        Menu->AddItem( 1, "I would like to browse your goods.", 1 );
    
    Menu->AddItem( 5, "Make this inn your home.", 2 );
    Menu->AddItem( 0, "What can I do at an inn?", 3 );
    if(AutoSend)
        Menu->SendTo(Plr);
}

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), textid, Plr); \
    Menu->SendTo(Plr);

void InnkeeperGossip::GossipSelectOption(Object * pObject, Player * Plr, uint32 Id, uint32 IntId, const char * Code)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULLCREATURE;
	if(pCreature==NULLCREATURE)
		return;

    switch(IntId)
    {
    case 1:     // VENDOR
        Plr->GetSession()->SendInventoryList(pCreature);
        break;
    case 2:     // BINDER
        Plr->GetSession()->SendInnkeeperBind(pCreature);
        break;
    case 3:     // WHAT CAN I DO ?
        // Prepare second menu
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 1853, Plr);
        Menu->AddItem(5, "Make this inn your home.", 2);
        Menu->SendTo(Plr);
        break;
	case 4:     // EVENT OF HALLOWEEN
		if(!Plr->HasAura(SPELL_TRICK_OR_TREATED))
		{
			pCreature->CastSpell(Plr, SPELL_TRICK_OR_TREATED, true);

			// either trick or treat, 50% chance
			if(rand()%2)
			{
				Plr->CastSpell(Plr, SPELL_TREAT, true);
			}
			else
			{
				int32 trickspell=0;
				switch (rand()%9)
				{
					case 0:
					    trickspell=24753;                       // cannot cast, random 30sec
						break;
					case 1:
						trickspell=24713;                       // lepper gnome costume
						break;
					case 2:
						if(Plr->getGender() == 0){
							trickspell=24735;                   // male ghost costume
						}else{
							trickspell=24736;                   // female ghostcostume
						}
	                    break;
	                case 3:
						if(Plr->getGender() == 0){
							trickspell=24711;                   // male ninja costume
						}else{
							trickspell=24710;                   // female ninja costume
						}
		                break;
	                case 4:
						if(Plr->getGender() == 0){
							trickspell=24708;                   // male pirate costume
						}else{
							trickspell=24709;                   // female pirate costume
						}
	                    break;
	                case 5:
	                    trickspell=24723;                       // skeleton costume
	                    break;
	            }
	            pCreature->CastSpell(Plr, trickspell, true);
			}	
		}
		Plr->Gossip_Complete();
        break;
    }
}

void InnkeeperGossip::GossipEnd(Object * pObject, Player * Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupInnkeepers(ScriptMgr * mgr)
{
	GossipScript * gs = (GossipScript*) new InnkeeperGossip();
    /* Innkeeper List */
    mgr->register_gossip_script(15174, gs);     //Calandrath
    mgr->register_gossip_script(11116, gs);     //Innkeeper Abeqwa
    mgr->register_gossip_script(9501, gs);      //Innkeeper Adegwa
    mgr->register_gossip_script(6740, gs);      //Innkeeper Allison
    mgr->register_gossip_script(2352, gs);      //Innkeeper Anderson
    mgr->register_gossip_script(6739, gs);      //Innkeeper Bates
    mgr->register_gossip_script(1247, gs);      //Innkeeper Belm
    mgr->register_gossip_script(3934, gs);      //Innkeeper Boorand Plainswind
    mgr->register_gossip_script(6727, gs);      //Innkeeper Brianna
    mgr->register_gossip_script(7714, gs);      //Innkeeper Byula
    mgr->register_gossip_script(16458, gs);     //Innkeeper Faralia
    mgr->register_gossip_script(295, gs);       //Innkeeper Farley
    mgr->register_gossip_script(5111, gs);      //Innkeeper Firebrew
    mgr->register_gossip_script(7733, gs);      //Innkeeper Fizzgrimble
    mgr->register_gossip_script(7737, gs);      //Innkeeper Greul
    mgr->register_gossip_script(6928, gs);      //Innkeeper Grosk
    mgr->register_gossip_script(6929, gs);      //Innkeeper Gryshka
    mgr->register_gossip_script(6734, gs);      //Innkeeper Hearthstove
    mgr->register_gossip_script(8931, gs);      //Innkeeper Heather
    mgr->register_gossip_script(1464, gs);      //Innkeeper Helbrek
    mgr->register_gossip_script(6272, gs);      //Innkeeper Janene
    mgr->register_gossip_script(7731, gs);      //Innkeeper Jayka
    mgr->register_gossip_script(6930, gs);      //Innkeeper Karakul
    mgr->register_gossip_script(6747, gs);      //Innkeeper Kauth
    mgr->register_gossip_script(12196, gs);     //Innkeeper Kaylisk
    mgr->register_gossip_script(6736, gs);      //Innkeeper Keldamyr
    mgr->register_gossip_script(6738, gs);      //Innkeeper Kimlya
    mgr->register_gossip_script(11103, gs);     //Innkeeper Lyshaerya
    mgr->register_gossip_script(6741, gs);      //Innkeeper Norman
    mgr->register_gossip_script(6746, gs);      //Innkeeper Pala
    mgr->register_gossip_script(5688, gs);      //Innkeeper Renee
    mgr->register_gossip_script(6735, gs);      //Innkeeper Saelienne
    mgr->register_gossip_script(6737, gs);      //Innkeeper Shaussiy
    mgr->register_gossip_script(2388, gs);      //Innkeeper Shay
    mgr->register_gossip_script(9356, gs);      //Innkeeper Shul'kar
    mgr->register_gossip_script(7736, gs);      //Innkeeper Shyria
    mgr->register_gossip_script(11106, gs);     //Innkeeper Sikewa
    mgr->register_gossip_script(6807, gs);      //Innkeeper Skindle
    mgr->register_gossip_script(5814, gs);      //Innkeeper Thulbek
    mgr->register_gossip_script(7744, gs);      //Innkeeper Thulfram
    mgr->register_gossip_script(6790, gs);      //Innkeeper Trelayne
    mgr->register_gossip_script(11118, gs);     //Innkeeper Vizzie
    mgr->register_gossip_script(6791, gs);      //Innkeeper Wiley
    mgr->register_gossip_script(16256, gs);     //Jessica Chambers
    mgr->register_gossip_script(14731, gs);     //Lard
    mgr->register_gossip_script(6778, gs);      //Melika Isenstrider
    mgr->register_gossip_script(6806, gs);      //Tannok Frosthammer
}
