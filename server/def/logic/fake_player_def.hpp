/********************************************************************
  created: 2016/10/27
  file base: fake_player_def
  file ext: hpp
  author: lxy
  
  purpose: 
*********************************************************************/
#ifndef _FAKE_PLAYER_DEF_HPP_
#define _FAKE_PLAYER_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "utility/init_unit.h"
#include "internet/st.pb.h"
#include "buff_def.hpp"
#include "arena_def.hpp"
#include "belief_def.hpp"

namespace hld
{
#pragma pack(push,1)

	struct s_big_player_db
	{
		int32 player_type;
		guid_64 role_guid;
		void to_proto(hld::st_proto::st_big_player_db* st_big_ptr)
		{
			if (st_big_ptr == nullptr)
			{
				return;
			}
			st_big_ptr->set_player_type(player_type);
			st_big_ptr->set_role_guid(role_guid.server_64);
		}
		void from_proto(const hld::st_proto::st_big_player_db& st_big_ref)
		{
			player_type = st_big_ref.player_type();
			role_guid.server_64 = st_big_ref.role_guid();
		}

	};

	struct s_fake_player_info
	{
		enum
		{
			efpi_none = 0,
			efpi_info,
			efpi_att,
			efpi_skill,
			efpi_item,
			efpi_buff,
			efpi_spirit,
			efpi_fight_att,
			efpi_belief,
			efpi_max
		};

		//typedef std::bitset<efpi_max> data_block_mask_type;

		s_fake_player_info()
		{
			clear();
		};

		void clear()
		{
			memset(this, 0, sizeof(*this));
		}

		void mask_data_block(int32 id)
		{
			if (id >= efpi_info && id < efpi_max)
				init_unit::mark_flag_data(data_block_mask, id);
		}

		bool all_data_block_masked() const
		{ 
			bool all_masked = true;
			for (int32 i = efpi_info; i < efpi_max; i++)
			{
				if (0 == init_unit::get_flag_data(data_block_mask, i))
				{
					all_masked = false;
					break;
				}
			}
			return all_masked;
		}

		void reset_data_block_mask() { data_block_mask = 0; }

		int32					data_block_mask;
		guid_64					guid;
		s_unit_info				role_info;
		s_base_att_info			att_info;
		int32					skill_num;
		s_skill_info			skill_data[MAX_SKILL_NUM];
		int32					item_num;
		s_item_info				item_data[fake_item_num];
		int32					buff_num;
		s_buff_info				buff_data[MAN_UNIT_BUFF_INST];
		s_arena_char_fight_att	m_fight_att;
		s_spirit_info			spirit_data;
		s_belief_info			belief_data[e_belief_series_type_max];
		int32					belief_num;
		bool to_proto(hld::st_proto::st_fake_player_info *st_fake_ptr)
		{
			if (st_fake_ptr == nullptr)
			{
				return false;
			}
			st_fake_ptr->set_data_block_mask(data_block_mask);
			st_fake_ptr->set_guid(guid.server_64);
			hld::st_proto::st_unit_info* st_unit_ptr = st_fake_ptr->mutable_role_info();
			if (st_unit_ptr == nullptr)
			{
				return false;
			}
			role_info.to_proto(st_unit_ptr);


			hld::st_proto::st_base_att_info* st_base_ptr = st_fake_ptr->mutable_att_info();
			if (st_base_ptr == nullptr)
			{
				return false;
			}
			att_info.to_proto(st_base_ptr);


			st_fake_ptr->set_skill_num(skill_num);
			for (int32 i = 0 ; i < skill_num ; i++)
			{
				hld::st_proto::st_skill_info *st_skill_ptr = st_fake_ptr->add_skill_data();
				if (st_skill_ptr == nullptr)
				{
					return false;
				}
				skill_data[i].to_proto(st_fake_ptr->add_skill_data());
			}
			
			st_fake_ptr->set_item_num(item_num);
			for (int32 i = 0; i < item_num; i++)
			{
				hld::st_proto::st_item_info *st_item_ptr = st_fake_ptr->add_item_data();
				if (st_item_ptr == nullptr)
				{
					return false;
				}
				//item_data[i].to_proto(st_item_ptr);
			}

			st_fake_ptr->set_buff_num(buff_num);
			for (int32 i = 0; i < buff_num; i++)
			{
				hld::st_proto::st_buff_info *st_buff_ptr = st_fake_ptr->add_buff_data();
				if (st_buff_ptr == nullptr)
				{
					return false;
				}
				buff_data[i].to_proto(st_buff_ptr);
			}

			hld::st_proto::st_arena_char_fight_att* st_arena_ptr = st_fake_ptr->mutable_m_fight_att();
			if (st_arena_ptr == nullptr)
			{
				return false;
			}
			m_fight_att.to_proto(st_arena_ptr);

			hld::st_proto::st_spirit_info* st_spirit_ptr = st_fake_ptr->mutable_spirit_data();
			if (st_spirit_ptr == nullptr)
			{
				return false;
			}
			spirit_data.to_proto(st_spirit_ptr);

			st_fake_ptr->set_belief_num(belief_num);
			for (int32 i = 0; i < e_belief_series_type_max; i++)
			{
				hld::st_proto::st_belief_info *st_belief_ptr = st_fake_ptr->add_belief_data();
				if (st_belief_ptr == nullptr)
				{
					return false;
				}
				belief_data[i].to_proto(st_belief_ptr);
			}
			return true;
		}
		void from_proto(const hld::st_proto::st_fake_player_info & st_fake_ref)
		{

			data_block_mask = st_fake_ref.data_block_mask();
			guid.server_64 = st_fake_ref.guid();

			role_info.from_proto(st_fake_ref.role_info());



			att_info.from_proto(st_fake_ref.att_info());


			skill_num = st_fake_ref.skill_num();
			for (int32 i = 0; i < skill_num && i < st_fake_ref.skill_data_size() ; i++)
			{
				skill_data[i].from_proto(st_fake_ref.skill_data(i));
			}

			item_num = st_fake_ref.item_num();
			for (int32 i = 0; i < item_num && i < st_fake_ref.item_data_size(); i++)
			{

				//item_data[i].from_proto(st_fake_ref.item_data(i));
			}

			buff_num = st_fake_ref.buff_num();
			for (int32 i = 0; i < buff_num && i < st_fake_ref.belief_data_size(); i++)
			{
	
				buff_data[i].from_proto(st_fake_ref.buff_data(i));
			}


			m_fight_att.from_proto(st_fake_ref.m_fight_att());


			spirit_data.from_proto(st_fake_ref.spirit_data());

			belief_num =  st_fake_ref.belief_num();
			for (int32 i = 0; i < e_belief_series_type_max && st_fake_ref.belief_data_size(); i++)
			{
				belief_data[i].from_proto(st_fake_ref.belief_data(i));
			}
		}
	};

#pragma pack(pop)
}

#endif
