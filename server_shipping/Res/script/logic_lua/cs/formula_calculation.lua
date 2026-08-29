local formula_calculation = {}
function formula_calculation:init()
	print("formula_calculation:init")
end
--装备基础属性计算
function formula_calculation.get_item_base_att_array(self_name, item_level, item_color)
	local attack_base = 5 --攻击常数1（基础值
	local attack_up = 2 --攻击常数2（成长值）
	local armor_base = 10 --防御常数1（基础值）
	local armor_up = 4 --防御常数2（成长值）
	local hp_base = 60 --生命常数1（基础值）
	local hp_up =24 --生命常数2（成长值）
	local color_ratio = {1,1.2,1.5,1.8,2.2,2.7,3.3,4,4.8,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6} -- 品质系数
	-------------------------------------计算--------------------------------------------------
	--攻击公式计算
	local att_attack_base = attack_base + (item_level - 1) * attack_up * (1 + item_level / 100 + item_level / 200)
	local att_attack_end = att_attack_base * color_ratio[item_color + 1]
	--防御公式计算
	local att_armor_base = armor_base + (item_level - 1) * armor_up * (1 + item_level / 100 + item_level / 200)
	local att_armor_end = att_armor_base * color_ratio[item_color + 1]
	--血量公式计算
	local att_hp_base = hp_base + (item_level - 1) * hp_up * (1 + item_level / 100 + item_level / 200)
	local att_hp_end = att_hp_base * color_ratio[item_color + 1]
	--组成最后的返回值
	local res = "4_6_"..att_attack_end.."_0_1"
	res = res.."4_7_"..att_armor_end.."_0_1"
	res = res.."4_8_"..att_hp_end.."_0_1"
	return 1, res
end
--装备分解计算
function formula_calculation.get_item_recovery_end(self_name, item_level, item_color)
	local recovery_money_type = 5 --分解货币类型
	local recovery_base = 1 --分解常数1（基础值）
	local color_ratio = {1,2,3,5,7,10,15,20,28,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40} -- 品质系数
	-------------------------------------计算--------------------------------------------------
	local recovery_end = recovery_base * color_ratio[item_color + 1]
	return 1, recovery_money_type, recovery_end
end
--装备强化消耗货币
function formula_calculation.get_upgrade_money_cost(self_name, equip_slot, upgrade_level)
	local item_upgrade_id = {0, 41000001, 41010001, 41020001, 41030001, 41040001, 41050001, 41060001, 41070001, 41080001, 41090001}
	local item_upgrade_money_type = 5
	local money_type = item_upgrade_money_type
	local money_cost = (upgrade_level + 1) * 5
	return 1, money_type, money_cost
end
--装备强化属性加成
function formula_calculation.get_upgrade_att_array(self_name, item_id, upgrade_level)
	print("get_upgrade_att_array item_template param item_id:"..item_id.." upgrade_level:"..upgrade_level.." self_name:"..self_name)
	local item_template = template_manager:get_line("ItemTemplate", item_id)
	if nil == item_template then
		print("get_upgrade_att_array item_template is null item_id："..item_id)
		return 1003, ""
	end
	local equip_template = template_manager:get_line("EquipTemplate", item_template.logic_id)
	if nil == equip_template then
		print("get_upgrade_att_array equip_template is null item_id:"..item_id.." logic_id:"..item_template.logic_id)
		return 1003, ""
	end
	
	local upgrade_att = {}
	for k,v in pairs(equip_template.AttArray) do
		table.insert(upgrade_att, v)
	end
	local att_len = (#upgrade_att) / 5
	for i = 0, att_len - 1 do
		local temp_att = upgrade_att[i * 5 + 3]
		upgrade_att[i * 5 + 3] = temp_att * upgrade_level * 0.1
	end
	local res = upgrade_att[1]
	for i = 2, (#upgrade_att) do
		res = res.."_"..upgrade_att[i]
	end
	return 1, res
end

formula_calculation_mgr = formula_calculation
return formula_calculation