#include "GoodsTemplate_S.h"

namespace faith
{
    void GoodsTemplateSize()
    {
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("template_base");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("AttributeId");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("ItemId");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("GoodsNum");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("ClassLimit");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("Levellimit");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("NeedLegionLevel");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("NeedContribution");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("IsLock");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("TimelimitOnType");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("TimelimitOnTime");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("DurationTime");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("AddTime");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("RemoveTime");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("OldPrice");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("NewPrice");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("vector<int32>");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("NeedItemId");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("LimitType");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("RoleMaxNum");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back(" EffectiveTime");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("int32");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("NeedVIPLevel");
        template_size::get_instance().template_size_map[e_GoodsTemplate].push_back("vector<string>");
        template_size::get_instance().template_key_map[e_GoodsTemplate].push_back("ChannelLimit");
    }
}
