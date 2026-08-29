local server_tool = {}
function server_tool:init()
end
function server_tool:merge_money_to_array(money_array, new_money_id, new_money_value)
	if (nil == money_array)then
		money_array = {}
	end
	if nil == money_array[new_money_id] then
		money_array[new_money_id] = new_money_value;
	else
		money_array[new_money_id] = money_array[new_money_id] + new_money_value;
	end
end
function server_tool:get_game_config_template_ptr()
	return template_manager:get_line("GameConfigTemplate", 99000001)
end

function server_tool:get_region_template_ptr()
	return template_manager:get_line("RegionTemplate", 150000000)
end

function server_tool:get_item_template(item_ptr)
	if nil == item_ptr then
		print("server_tool:get_item_template  item_ptr is null")
		return nil
	end
	return template_manager:get_line("ItemTemplate", item_ptr:get_item_template_id())
end
function server_tool:implode(str_array, sep)
	local ret = ""
	if nil == str_array then
		return ret
	end
	if nil == sep then
		sep = ""
	end
	for k,v in pairs(str_array) do
		ret = ret..sep..v
	end
	return ret
end
function server_tool:send_notice(player_ref, notice_info)
	if player_ref == nil then
		return
	end
	if notice_info.notice_id == nil or notice_info.notice_id <= 0 then
		return
	end
	hld.globle_data:get_instance():clear_char_notice_info();
	hld.globle_data:get_instance():set_notice_id(notice_info.notice_id);
	if notice_info ~= nil then
		if notice_info.other_name ~= nil then
			for k,v in pairs(notice_info.other_name) do
				hld.globle_data:get_instance():add_other_name(v);
			end
		end
		if notice_info.describe_str ~= nil then
			for k,v in pairs(notice_info.describe_str) do
				hld.globle_data:get_instance():add_describe_str(v);
			end
		end
		if notice_info.item_name ~= nil then
			for k,v in pairs(notice_info.item_name) do
				hld.globle_data:get_instance():add_item_name(v);
			end
		end
		if notice_info.num_type_data ~= nil then
			for k,v in pairs(notice_info.num_type_data) do
				hld.globle_data:get_instance():add_num_type_data(v);
			end
		end
		if notice_info.item_data ~= nil then
			for k,v in pairs(notice_info.item_data) do
				hld.globle_data:get_instance():add_item_data(v);
			end
		end
	end
	player_ref:send_notice();
	hld.globle_data:get_instance():clear_char_notice_info();
end
function server_tool:read_table_rand(table_value)
    local key_array={}
    local n=1
    for k in pairs(table_value) do
        key_array[n]=k
        n=n+1
    end
    return key_array[math.random(1,n-1)]
end

function server_tool:gen_drop_id_list(drop_template_id, item_drop_with_att, drop_weight, curstep, class_type, exp_level)
    --dump("----drop.0",drop_template_id,item_drop_with_att,drop_weight,curstep,class_type)
    class_type = class_type or hld.e_class_type_none;
    curstep=curstep+1;
    --dump("---drop.0end",curstep)
    if curstep > 10 then
        --dump("----drop.1")
		return;--强制保护,因为掉落包可以套掉落包,填错了就会递归包含,最终导致死循环,所以调用深度超过10,则强制返回;
    end
    --dump("----drop.....")
    --dump("----drop.2",#item_drop_with_att.item_id_arr)
	if #item_drop_with_att.item_id_arr > 100 then
		return;--一个包掉落了超过100个物品? 强制退出,很可能是出问题了;
	end
    --dump("----drop.222222")
    local rand_ret = server_tool:rand_trigger(drop_weight, ConstDefine.DROP_MAX_WEIGHT);
    --dump("----drop.2end",rand_ret)
	if false == rand_ret then
		return;
	end
    --dump("----drop.3")
    local DropTemplate=template_manager:get_line("DropTemplate", drop_template_id);
    --dump("----drop.3end")
	if DropTemplate == nil then
		return;
	end


    local result_drop_template_id = drop_template_id 

	local ChangeIds =  DropTemplate.LevelChangeId
	if #ChangeIds > 0 and #ChangeIds % 3 == 0  then
		for i=0,#ChangeIds/3 -1 do
			local MinLevel = ChangeIds[i * 3 + 1]
			local MaxLevel = ChangeIds[i * 3 + 2]
			local new_drop_temp_id = ChangeIds[i*3+ 3]
			if exp_level >= MinLevel and exp_level <= MaxLevel then
				DropTemplate = template_manager:get_line("DropTemplate", new_drop_temp_id);
                result_drop_template_id = new_drop_temp_id
				break;
			end
		end
	end
    --职业限制
	--dump(DropTemplate)
    --dump("----drop.4",DropTemplate.ClassType,hld.e_class_type_none)
	if DropTemplate.ClassType > hld.e_class_type_none and class_type > hld.e_class_type_none then
		if class_type ~= DropTemplate.ClassType then
			return;
		end
    end
    
    local IDs = DropTemplate.IDs
    local IDTypes = DropTemplate.IDTypes
    local Bounds = DropTemplate.Bounds
    local Weights = DropTemplate.Weights
    local Counts = DropTemplate.Counts
    
    local size_id = #IDs;
    --dump("----drop.4end",size_id)
	local drop_type = DropTemplate.DropType; --0为逐个掉落，可能掉落多个物品，每个物品的掉落概率=权重/1000000 1为归一掉落，圆桌理论，最多只会掉落一个物品，所有id总权重小于1000000时，可能出现不掉落物品的情况
	local size_idtype = #IDTypes;
	local size_count = #Counts;
	local size_bound = #Bounds;
	local size_weight = #Weights;
    --dump("-----drop.5",size_id,drop_type,size_idtype,size_count,size_bound,size_weight)
	local size_equ = (size_id == size_idtype) and (size_id * 2 == size_count) and (size_id == size_bound) and (size_id == size_weight);
	if size_equ == false then
		return;--表填错了,列的数量对不上
	end

	--新掉落逻辑
    if drop_type == hld.e_dropbox_ran_type_all then
        --dump("-----drop.6")
		server_tool:sub_drop_rand_all(result_drop_template_id, item_drop_with_att, curstep, class_type, exp_level);
    elseif drop_type == hld.e_dropbox_ran_type_one then
        --dump("-----drop.7")
		server_tool:sub_drop_rand_one(result_drop_template_id, item_drop_with_att, curstep, class_type, exp_level);
	else
	end
end

function server_tool:sub_drop_rand_all(drop_template_id, item_drop_with_att, curstep, class_type, exp_level)
    class_type = class_type or hld.e_class_type_none;
    local DropTemplate = template_manager:get_line("DropTemplate", drop_template_id);
    if DropTemplate == nil then
        return;
    end

    local IDs = DropTemplate.IDs
    local IDTypes = DropTemplate.IDTypes
    local Bounds = DropTemplate.Bounds
    local Weights = DropTemplate.Weights
    local Counts = DropTemplate.Counts
    local ConstAtt = DropTemplate.ConstAtt

    local size_id = #IDs;
	local size_idtype = #IDTypes;
	local size_count = #Counts;
	local size_bound = #Bounds;
	local size_weight = #Weights;
    local size_const_att = #ConstAtt;
    
    local size_equ = (size_id == size_idtype) and (size_id * 2 == size_count) and (size_id == size_bound) and (size_id == size_weight);
    if size_equ == false then
        return;--表填错了,列的数量对不上
    end

    for i=0,size_id-1 do

			local temp_id = tonumber(IDs[i+1]);
			local temp_type = tonumber(IDTypes[i+1]);
			local temp_count = 0;
			local temp_bound = tonumber(Bounds[i+1]);
			local temp_weight = tonumber(Weights[i+1]);

			local temp_count1 = tonumber(Counts[2*i+1]);
			local temp_count2 = tonumber(Counts[2*i+2]);

            local temp_const_att = 0;
            
            if size_const_att > i then
                temp_const_att = tonumber(ConstAtt[i+1]);
            end

            if temp_count2 > temp_count1 then
                temp_count = hld.random_gen:get_random(temp_count1, temp_count2);
            else
                temp_count = hld.random_gen:get_random(temp_count2, temp_count1);
            end

            repeat
                if temp_id == 0 then --物品ID不可能是0
                    break;
                end

                if hld.e_dropbox_typ_drop_box == temp_type then --掉落盒里的物品是另一个掉落盒，再随机一遍……
                    for ii=0,temp_count-1 do
                        server_tool:gen_drop_id_list(temp_id, item_drop_with_att, temp_weight, curstep, class_type, exp_level);--此时还不确定这个包掉不掉，所以要传真实权重做判断
                    end
                elseif hld.e_dropbox_typ_item == temp_type then --掉落盒里是物品，就加入掉落列表
                    local rand_temp = server_tool:rand_trigger(temp_weight, ConstDefine.DROP_MAX_WEIGHT);
                    if true == rand_temp then
                        for ii=0,temp_count-1 do
                            table.insert(item_drop_with_att.item_id_arr,temp_id);
                            table.insert(item_drop_with_att.item_lock_state_arr,temp_bound);
                            table.insert(item_drop_with_att.item_const_att_arr,temp_const_att);
                        end
                    end
                else
                    break;
                end
            until true
    end
end

function server_tool:sub_drop_rand_one(drop_template_id, item_drop_with_att, curstep, class_type, exp_level)
    class_type = class_type or hld.e_class_type_none;
    local DropTemplate=template_manager:get_line("DropTemplate", drop_template_id);
    if nil == DropTemplate then
        return;
    end

    local IDs = DropTemplate.IDs
    local IDTypes = DropTemplate.IDTypes
    local Bounds = DropTemplate.Bounds
    local Weights = DropTemplate.Weights
    local Counts = DropTemplate.Counts

    local size_id = #IDs;
	local size_idtype = #IDTypes;
	local size_count = #Counts;
	local size_bound = #Bounds;
    local size_weight = #Weights;
    
    local size_equ = (size_id == size_idtype) and (size_id * 2 == size_count) and (size_id == size_bound) and (size_id == size_weight);
    --dump("-----drop.7.3",size_equ,size_id,size_idtype,size_count,size_bound,size_weight)
    if false == size_equ then
        return;
    end
    
    local choosed_item_index = -1;
    local total_weight = 0;
    for i=1,size_weight do
        --dump("-----drop.7.3end",Weights[i])
        total_weight=total_weight+tonumber(Weights[i]);
    end
    --dump("-----drop.7.4",total_weight,ConstDefine.DROP_MAX_WEIGHT)
    if total_weight < ConstDefine.DROP_MAX_WEIGHT then
        total_weight = ConstDefine.DROP_MAX_WEIGHT;
    end

    --确定随到第几个
    --此时的随机上限已经做过处理，在实际总权重值不足“DROP_MAX_WEIGHT”时，已经将其设置为“DROP_MAX_WEIGHT”，所以可能出现随不到的情况
    local random_value = hld.random_gen:get_random(0, total_weight);
    local cur_weight = 0;
    --dump("-----drop.7.5",random_value)
    for i=1,size_weight do
        cur_weight = cur_weight + tonumber(Weights[i]);
        --dump("do",cur_weight,random_value)
        if cur_weight>= random_value then
            --dump("do")
            choosed_item_index = i;
			break;
        end
    end
    --dump("-----drop.7.6",choosed_item_index,size_id)
    if choosed_item_index < 1 then
        --没随到，啥都不干
        return;
    end

    if choosed_item_index > size_id then
        --dump("----drop.7.6end")
        return; --随到的物品下标越界了
    end

    local cur_id_typ = tonumber(IDTypes[choosed_item_index]);
    --dump("-----drop.7.7",cur_id_typ)
    if tonumber(cur_id_typ) == hld.e_dropbox_typ_non then
        return; --0可能就是表有问题……
    end

    local real_get_id = tonumber(IDs[choosed_item_index]);
    local item_lock_state = tonumber(Bounds[choosed_item_index]);
    --dump("-----drop.7.8",real_get_id,item_lock_state)
    if real_get_id == 0 then
        return;
    end

    --确认生成几个物品
	local total_item_num = 0;
	local temp_count1 = tonumber(Counts[2 * choosed_item_index -1]);
    local temp_count2 = tonumber(Counts[2 * choosed_item_index]);
    
    local temp_const_att = 0;
    local ConstAtt = DropTemplate.ConstAtt
    --dump("-----drop7.9",temp_count1,temp_count2,#ConstAtt)
    if #ConstAtt >= choosed_item_index then
        temp_const_att = tonumber(ConstAtt[choosed_item_index]);
    end
    if temp_count2 > temp_count1 then
        total_item_num = hld.random_gen:get_random(temp_count1, temp_count2);
    else
        total_item_num = hld.random_gen:get_random(temp_count2, temp_count1);
    end

    --dump("----drop7.10",total_item_num,cur_id_typ);
    for cur_item_num=0,total_item_num-1 do
        if cur_id_typ == hld.e_dropbox_typ_drop_box then
            --dump("----drop7.10end")
            server_tool:gen_drop_id_list(real_get_id, item_drop_with_att, ConstDefine.DROP_MAX_WEIGHT, curstep, class_type, exp_level);--因为此时已经确认该物品包必然掉落，所以递归时直接传入满权重
        elseif cur_id_typ == hld.e_dropbox_typ_item then --掉落盒里是物品，就加入掉落列表
            --dump("----drop7.11")
            table.insert(item_drop_with_att.item_id_arr,real_get_id);
            table.insert(item_drop_with_att.item_lock_state_arr,item_lock_state);
            table.insert(item_drop_with_att.item_const_att_arr,temp_const_att);
        end
    end
end

function server_tool:rand_trigger(drop_weight, drop_denominator)
    local random = hld.random_gen:get_random(1, drop_denominator);
    if random <= drop_weight then
        return true;
    end
    return false;
end

server_tool_mgr = server_tool
return server_tool_mgr;
