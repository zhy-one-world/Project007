local feather_def = {};
function feather_def:init()
end

feather_def.feather_heart_error_sucess                  = 0;
feather_def.feather_heart_error_unknow                  = -1;
feather_def.feather_heart_error_not_money               = -2;
feather_def.feather_heart_error_max_upgrade             = -3;
feather_def.feather_heart_error_equip_on_type           = -4;
feather_def.feather_heart_error_equip_on_slot_type      = -5;
feather_def.feather_slot_core                           = hld.e_item_info_succinct_level;
feather_def.feather_slot_start                          = hld.e_item_info_jewel_slot_0;
feather_def.feather_slot_end                            = feather_def.feather_slot_start + 5;
feather_def.feather_heart_slot_max                      = 4;

--enum e_feather_heart_oper
feather_def.e_feather_heart_oper_grade = 0;
feather_def.e_feather_heart_oper_equip_on = 1;
feather_def.e_feather_heart_oper_piece = 2;

--enum e_feather_heart
feather_def.e_feather_heart_0 = 0;
feather_def.e_feather_heart_1 = 1;
feather_def.e_feather_heart_2 = 2;
feather_def.e_feather_heart_3 = 3;
feather_def.e_feather_heart_max = 4;

--e_feather_have_skill
feather_def.e_feather_have_skill_0 = 0;
feather_def.e_feather_have_skill_1 = 1;
feather_def.e_feather_have_skill_2 = 2;
feather_def.e_feather_have_skill_max = 3;

--e_feather_core_add_level
feather_def.e_feather_core_add_level_start = hld.e_item_info_forge_property1;
feather_def.e_feather_core_add_level_0 = feather_def.e_feather_core_add_level_start;
feather_def.e_feather_core_add_level_1 = feather_def.e_feather_core_add_level_start + 1;
feather_def.e_feather_core_add_level_2 = feather_def.e_feather_core_add_level_start + 2; 
feather_def.e_feather_core_add_level_end = feather_def.e_feather_core_add_level_start + 3;

FeatherDef = feather_def;
return FeatherDef;