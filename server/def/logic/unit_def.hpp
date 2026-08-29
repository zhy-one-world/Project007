#ifndef __OMP_DEF_UNITDEF_HEADER_FILE__
#define __OMP_DEF_UNITDEF_HEADER_FILE__

#include "logic/type_def.hpp"

namespace faith {
	namespace app {

#pragma pack(push,1)

		enum UnitType
		{
			UT_PLAYER,		//Íæ¼Ò
			UT_NPC			//NPC
		};

		struct UnitID
		{
			UnitType	type;
			union
			{
				struct 
				{
					uint32	charindex;		//½ÇÉ«Ë÷Òý
				} player;

				struct 
				{
				} npc;
			};

			bool operator < (const UnitID & o) const
			{
				if(type != o.type)
				{
					return type < o.type; 
				}
				switch(type)
				{
					case UT_PLAYER:
					{
						return player.charindex < o.player.charindex;
					}
					default:
						return false;
				}
			}

			template<class Archive>
			inline void serialize(Archive & ar, const unsigned int version)
			{
				ar & type;
				switch(type)
				{
				case UT_PLAYER:
					{
						ar & player.charindex;
						break;
					}
				default:
					break;
				}
			}
		};

#pragma pack(pop)

	}	// end of namespace app

	struct s_unit_identifier
	{
		s_unit_identifier() : runtime_id(-1) {}

		s_unit_identifier(guid_64 guid_param, int32 runtime_id_param) 
			: unit_guid(guid_param)
			, runtime_id(runtime_id_param) 
		{}

		void clear_data()
		{
			unit_guid.clear_data();
			runtime_id = -1;
		}

		bool is_valid() const { return unit_guid.is_valid(); }

		inline s_unit_identifier& operator () (guid_64 guid_param, int32 runtime_id_param)
		{
			this->unit_guid = guid_param;
			this->runtime_id = runtime_id_param;
			return *this;
		}
		bool operator != (const s_unit_identifier& _left) const
		{
			return _left.unit_guid != unit_guid || _left.runtime_id != runtime_id;

		}
		bool operator == (const s_unit_identifier& _left) const
		{
			return _left.unit_guid == unit_guid && _left.runtime_id == runtime_id;
		}
		guid_64 unit_guid;
		int32 runtime_id;
	};


}	// end of namespace faith


#endif//#define __OMP_DEF_UNITDEF_HEADER_FILE__
