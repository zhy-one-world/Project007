#include "CityWarTerritoryTemplate_S.h"

namespace hld
{
    void CityWarTerritoryTemplateSize()
    {
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("TerrName");
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("TerrImage");
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("TerrLevel");
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("Score");
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("BidMoney");
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("BidAddMoney");
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("MaintainGlow");
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("MaintainGold");
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("ContainZoneId");
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("DailyRewards");
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("float");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("TerritoryAddition");
        template_size::get_instance().template_size_map[e_CityWarTerritoryTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_CityWarTerritoryTemplate].push_back("TerritoryBuff");
    }
}
