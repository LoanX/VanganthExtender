#include "stdafx.h"
#include "NpcDb.h"

CNpcDb g_NpcDb;

void CNpcDb::Init()
{
	m_DB.clear();
	LoadData();
	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] npc(s).", __FUNCTION__);
}

void CNpcDb::LoadData()
{
	//load npcdata
	wstringstream npcDataStream(ReadFileW(L"..\\Script\\NpcData.txt"));
	wstring line;
	while(getline(npcDataStream, line))
	{
		//npc_begin	warrior	20446	[utuku_orc]	level=6	acquire_exp_rate=5.9167	acquire_sp=6	unsowing=0	clan={@orc_clan}	ignore_clan_list={}	clan_help_range=300	slot_chest=[]	slot_rhand=[hand_axe]	slot_lhand=[]	shield_defense_rate=0	shield_defense=0	skill_list={@s_race_humanoids}	npc_ai={[utuku_orc];{[MoveAroundSocial]=138};{[MoveAroundSocial1]=138};{[MoveAroundSocial2]=138}}	category={}	race=humanoid	sex=male	undying=0	can_be_attacked=1	corpse_time=7	no_sleep_mode=0	agro_range=1000	ground_high={110;0;0}	ground_low={45;0;0}	exp=6039	org_hp=93.54158	org_hp_regen=2	org_mp=77.2	org_mp_regen=0.9	collision_radius={10;10}	collision_height={21;21}	str=40	int=21	dex=30	wit=20	con=43	men=20	base_attack_type=blunt	base_attack_range=40	base_damage_range={0;0;80;120}	base_rand_dam=50	base_physical_attack=13.52471	base_critical=1	physical_hit_modify=9	base_attack_speed=253	base_reuse_delay=0	base_magic_attack=9.23562	base_defend=53.53373	base_magic_defend=35.6433609604933	physical_avoid_modify=0	soulshot_count=0	spiritshot_count=0	hit_time_factor=0.48	item_make_list={}	corpse_make_list={{[iron_ore];1;1;7.7741};{[coal];1;1;7.7741};{[bow_shaft];1;1;0.5553}}	additional_make_list={}	additional_make_multi_list={{{{[adena];41;58;100}};70};{{{[leather_shirt];1;1;38.4772};{[leather_pants];1;1;61.5228}};0.4912};{{{[rp_brandish];1;1;5.80429};{[stem];1;1;62.7971};{[iron_ore];1;1;31.3986}};4.8757}}	hp_increase=0	mp_increase=0	safe_height=100	npc_end
		if(line.find(L"npc_begin") == 0)
		{
			NpcInfo ni;
			wstringstream dataStream(line);
			wstring temp;
			INT32 id = 0;
			wstring name;
			if(dataStream >> temp >> temp >> id >> name)
			{
				name = Utils::ReplaceString(name, L"[", L"", true);
				name = Utils::ReplaceString(name, L"]", L"", true);
				ni.id = id;
				ni.classId = 1000000 + id;
				ni.name = name;
				m_DB.push_back(ni);
			}
		}
	}

	//load NpcName.txt
	wstringstream nameStream(ReadFileW(L"..\\Script\\NpcName.txt"));
	while(getline(nameStream, line))
	{
		//20001	Gremlin		9C	E8	A9	-1
		if(line.size() > 0 && line.find(L"/") != 0)
		{
			vector<wstring> cols = Parser::Split(line, L"\t");
			if(cols.size() > 1)
			{
				INT32 id = _wtoi(cols[0].c_str());
				wstring clientName = cols[1];
				if(id > 0 && clientName.size() > 0)
				{
					for(UINT n=0;n<m_DB.size();n++)
					{
						if(m_DB[n].id == id)
						{
							m_DB[n].clientName = clientName;
							break;
						}
					}
				}
			}
		}
	}
}

wstring CNpcDb::GetClientNameByName(std::wstring name)
{
	guard;
	wstring clientName = L"";
	for(UINT n=0;n<m_DB.size();n++)
	{
		if(m_DB[n].name == name)
		{
			clientName = m_DB[n].clientName;
			break;
		}
	}
	unguard;
	return clientName;
}

wstring CNpcDb::GetClientNameByClassId(UINT classId)
{
	guard;
	wstring clientName = L"";
	for(UINT n=0;n<m_DB.size();n++)
	{
		if(m_DB[n].classId == classId)
		{
			clientName = m_DB[n].clientName;
			break;
		}
	}
	unguard;
	return clientName;
}