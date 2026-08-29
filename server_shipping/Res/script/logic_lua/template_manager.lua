
local template_manager_lua = {}
local m_TemplateTable = {}
--[[local m_TemplateName =
{
    "VIPTemplate",
	"WeddingRingTemplate",
	"ElementHeartTemplate",
	"ItemTemplate",
	"ElementHeartUpgradeTemplate",
	"QuestionMarryTemplate",
	"MapTemplate",
	"DropTemplate",
	"OracleTrialTemplate",
	"GameConfigTemplate",
	"PlayerUpgradeTemplate",
	"ActivityCommonConfigTemplate",
	"WelfareTemplate",
    "AwakenTemplate",
    "NoticeTemplate",
    "EquipTemplate",
	"SpiritTemplate",
    "ItemUpgradeTemplate",
	"ItemAddonTemplate",
    "PropBasicAttributeTemplate",
    "GodHeartTemplate",
    "GodHeartUpgradeTemplate",
    "FeatherTemplate",
    "PassiveSkillTemplate",
    "RegionTemplate",
    "GoodsTemplate",
	"ConvertTemplate",
	"BeliefTemplate",
	"BeliefRunesTemplate",
	"BeliefRunesUpgradeTemplate",
	"GrowUpFundTemplate",
    "ProtectSpiritTemplate",
    "TinderTemplate",
    "MissionTemplate",
    "JewelTemplate",
    "VIPTemplate",
    "RecycleTemplate",
   	"WingTemplate",
    "BeastSpiritTemplate",
	"CompositTemplate",
    "MountTemplate",
    "PlayerInitTemplate",
    "GradeGodHoodTemplate",
    "MissionChapterTemplate",
    "DeityTemplate",
    "PokedexTemplate",
    "PsycheTemplate",
    "FasionTemplate",
    "SpecialNameTemplate",
    "StoreTemplate",
    "AttributeTemplate",
    "ServiceGoalTemplate",
    "ItemSuccinctTemplate",
    "AdditionBuffTemplate",
    "EquipEnchantTemplate",
	"MoneyTemplate",
    "DragonTripTemplate",
    "ArenaRankRewardTemplate",
    "TalentTemplate",
    "TalentListTemplate",
    "AchievementTemplate",
    "DailyActivitiesTemplate",
    "LegionConstructionsTemplate",
	"BreakWillTemplate",
}]]
function template_manager_lua:init()
    m_TemplateTable = {}
    for k,v in pairs(g_TemplateName) do
		print(v)
		package.loaded[v] = nil
        m_TemplateTable[v] = require(v);
	end

 end

function template_manager_lua:get_field(TemplateName, TemplateKey, FieldValue)
    local key = tonumber(TemplateKey)
    if nil == key then
        return ''
    end
	local lineData = self:get_line(TemplateName, key)
	if lineData ~= nil then
		local ValueData = lineData[FieldValue]
		if ValueData ~= nil then
			return ValueData
		else
			return ''
		end
	else
		return ''
	end
    return ''
end

function template_manager_lua:get_line(TemplateName, TemplateKey)
    --print "template_manager_lua:get_line"
    local key = tonumber(TemplateKey)
    if nil == key then
        return nil
    end
    if m_TemplateTable[TemplateName] ~= nil then
        return m_TemplateTable[TemplateName][key]
    end
    return nil
end

function template_manager_lua:GetLineByTypeAndTarget(WelfareType, WelfareTargetValue)
    local WelfareTemplateTable = self:get_template("WelfareTemplate");
	if WelfareTemplateTable == nil then
		return nil;
    end
    for key,value in pairs(WelfareTemplateTable) do
        local Template = value;
		repeat
			if nil == Template then
				break;
			end
			if WelfareType == Template.Type and WelfareTargetValue == Template.TargetValue then
				return Template,key;
			end
		until true
	end
	return nil;
end
function template_manager_lua:get_template(TemplateName)
	return m_TemplateTable[TemplateName]
end
function template_manager_lua:get_str_id_by_notice_id(notice_id)
	local notice_template_ptr = self:get_line("NoticeTemplate", notice_id);
	if (nil == notice_template_ptr)then
		return "";
	end
	return notice_template_ptr.ContentText;
end

function template_manager_lua:get_template_by_vip_level(vip_level)
	local vip_template_table = self:get_template("VIPTemplate")
	if nil == vip_template_table then
        return nil
    end
	local region_code = self:get_field("RegionTemplate", 150000000, "RegionCode");
    for key,value in pairs(vip_template_table) do
        local Template = value
        if nil == Template then
            break
        end
		for k, v in pairs(Template.RegionArr) do
			if v == region_code and vip_level == Template.Level then
				return Template
			end
		end
    end
end

function template_manager_lua:get_spirit_excellent_target_color_array(spirit_template_ptr, target_color)

    if spirit_template_ptr == nil or target_color <= hld.e_item_color_white or target_color >= hld.e_item_color_max then
        return {}
    end

    if target_color == hld.e_item_color_green then
        return spirit_template_ptr.GreenExcellentRatio
    end

    if target_color == hld.e_item_color_blue then
        return spirit_template_ptr.BlueExcellentRatio
    end
    if target_color == hld.e_item_color_purple then
        return spirit_template_ptr.PurpleExcellentRatio
    end
    if target_color == hld.e_item_color_orange then
        return spirit_template_ptr.OrangeExcellentRatio
    end
    if target_color == hld.e_item_color_red then
        return spirit_template_ptr.RedExcellentRatio
    end
	if target_color == hld.e_item_color_pink then
        return spirit_template_ptr.PinkExcellentRatio
    end
    return {}
end

function template_manager_lua:get_template_by_type_and_level_and_star(name_type,  name_level,  name_star)
    local special_name_tmpl_table = self:get_template("SpecialNameTemplate")
    if nil == special_name_tmpl_table then
        return nil,nil;
    end

    for key,value in pairs(special_name_tmpl_table) do
        repeat
        local special_name_tmpl_ptr = value
        if nil == special_name_tmpl_ptr then
            break
        end
        if name_type == special_name_tmpl_ptr.NameType and name_level == special_name_tmpl_ptr.NameLevel and name_star == special_name_tmpl_ptr.NameStar then
            return key,special_name_tmpl_ptr;
        end
        until true
    end
    return nil,nil;
end
function template_manager_lua:get_excellent_target_color_array(equip_template_ptr, target_color)
	if (equip_template_ptr == nil
		or target_color <= hld.e_item_color_white
		or target_color >= hld.e_item_color_max)then
		return nil
	end

	local color_ratio = {}
	color_ratio[hld.e_item_color_green] = equip_template_ptr.GreenExcellentRatio
	color_ratio[hld.e_item_color_blue] = equip_template_ptr.BlueExcellentRatio
	color_ratio[hld.e_item_color_purple] = equip_template_ptr.PurpleExcellentRatio
	color_ratio[hld.e_item_color_orange] = equip_template_ptr.OrangeExcellentRatio
	color_ratio[hld.e_item_color_red] = equip_template_ptr.RedExcellentRatio
	color_ratio[hld.e_item_color_pink] = equip_template_ptr.PinkExcellentRatio
	return color_ratio[target_color]
end


function template_manager_lua:get_map_template_map_id(map_template_id)
    if 0 > map_template_id then
        return  nil;
    end
    local map_template = self:get_template("MapTemplate");
    if  map_template == nil then
        return  nil;
    end
    for k, v in pairs(map_template) do
        if k == map_template_id then
            return  v;
        end
    end
    return v
end
template_manager = template_manager_lua
return template_manager
