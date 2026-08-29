local item_def = {};
function item_def:init()
end

item_def.MAX_ITEM_BAG_NUM				    =200												-- 背包最大数量
item_def.max_proctect_mark_equip_slot_num   = 8													-- 守护印记装备栏上限
item_def.max_proctect_mark_bag_slot_num     = 36                                                -- 守护印记栏上限

item_def.max_enchant_type_num = 100																-- 附魔种类最大数量

--enum mount_type_read

item_def.mount_type_non                     = 0			--坐骑类型，无
item_def.mount_type_normal                  = 1			--普通坐骑
item_def.mount_type_special                 = 2			--特殊坐骑
item_def.mount_type_warlord                 = 3			--城主坐骑
item_def.mount_type_illusion                = 4		    --坐骑幻化
item_def.mount_type_lordlegionmem           = 5	        --霸主军团成员坐骑

ItemDef = item_def;
return ItemDef;