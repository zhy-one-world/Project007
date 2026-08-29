local role_summary_class = {}
function role_summary_class:init()
end

define_C[13201] = function(message_data, player_ref)
	local msg_data = decode("hld.title_proto.title_equip_operate", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
	end
	role_summary_class:equip_operate(player_ref,msg_data.template_id,msg_data.operate_type)
end

function role_summary_class:equip_operate(player_ref,title_template_id, operate_type)
    if title_template_id <= 0 then
        return;
    end
    if operate_type < 0 or operate_type >= hld.e_title_operate_type_max then
        return;
    end
    if operate_type == hld.e_title_operate_type_equip_on then
        role_summary_class:equip_on_title(player_ref,title_template_id);
    else 
        role_summary_class:equip_off_title(player_ref);
    end
end

function role_summary_class:equip_on_title_lua(player_index,title_template_id)
    local player_ref = hld.get_player_by_index(player_index)
    if nil == player_ref then
        return;
    end
    role_summary_class:equip_on_title(player_ref,title_template_id)
end

function role_summary_class:equip_on_title(player_ref,title_template_id)
    if title_template_id <= 0 then
			return;
    end
        local n_title = player_ref:get_title_mgr():get_title_by_template_id(title_template_id)
		if n_title == nil then
			return;
        end
		role_summary_class:equip_off_title(player_ref);
		player_ref:get_pawn_att():set_unit_base_att(hld.e_base_att_info_equip_title_id, title_template_id);
		player_ref:get_pawn_att():send_base_att_one(hld.e_base_att_info_equip_title_id);
		n_title:set_title_equiping_att(player_ref:get_array_index(), true);
end

function role_summary_class:equip_off_title_lua(player_index)
    local player_ref = hld.get_player_by_index(player_index)
    if nil == player_ref then
        return;
    end
    role_summary_class:equip_off_title(player_ref)
end

function role_summary_class:equip_off_title(player_ref)

    local title_template_id = player_ref:get_pawn_att():get_unit_base_att(hld.e_base_att_info_equip_title_id);
    if title_template_id <= 0 then
        return;
    end
    local n_title = player_ref:get_title_mgr():get_title_by_template_id(title_template_id)
    n_title:set_title_equiping_att(player_ref:get_array_index(), false);
    player_ref:get_pawn_att():set_unit_base_att(hld.e_base_att_info_equip_title_id, -1);
    player_ref:get_pawn_att():send_base_att_one(hld.e_base_att_info_equip_title_id);
end

define_C[10525] = function(message_data, player_ref)
	local msg_data = decode("hld.character_proto.modify_person_information", message_data)
	if msg_data == nil then
		return
	end
	if player_ref:is_valid() == false  or player_ref:get_session_state() ~= hld.e_session_status_in_gaming then
		return;
    end
	role_summary_class:mofify_person_information(player_ref,msg_data.gender,msg_data.personalized_signature,msg_data.birthday_year,msg_data.birthday_mouth,msg_data.birthday_day,msg_data.city,msg_data.hometown,msg_data.online_state,msg_data.tags)
end

function role_summary_class:mofify_person_information(player_ref, gender,personalized_signature,birthday_year,birthday_mouth,birthday_day,city,hometown,online_state,tags)
    local check_result = player_ref:get_person_infor_mgr():check_person_information( personalized_signature, city,  hometown)
    if check_result == false then
        return
    end
    player_ref:get_person_infor_mgr():mofify_person_information_lua(gender,  personalized_signature,  birthday_year,  birthday_mouth,  birthday_day,  city,  hometown,  online_state)
    player_ref:get_person_infor_mgr():clear_person_information_tags()
    
    local m_size = #tags
   
    for i = 1,m_size do
        local tag = tags[i]
        delog(tag)
        player_ref:get_person_infor_mgr():add_person_information_tag(tag)
    end

    player_ref:get_person_infor_mgr():sync_to_client();
    player_ref:get_person_infor_mgr():sync_to_ws();

    --修改成功提示
    player_ref:send_notice("90096905");
end

role_summary = role_summary_class

return role_summary
