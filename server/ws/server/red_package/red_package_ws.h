#ifndef _WS_RED_PACKAGE_WS_H_
#define _WS_RED_PACKAGE_WS_H_

#include "logic/type_def.hpp"
#include "Logic/event_def.hpp"

namespace faith
{
	struct RedPackageTemplate;

	class red_package_ws
	{
	public:
		red_package_ws();
		~red_package_ws();
		void					tick(float dt);
		void					clear_data();

		int32					init_by_info(int32 red_package_template_id, guid_64 sender_guid, int32 sender_vip_level, xstring sender_name, xstring text);
		int32					draw_red_package(guid_64 receiver_guid, xstring receiver_name);
		int32					find_empty_receiver_list_slot();
		void					add_receiver(s_red_package_receiver receiver_info);
		bool					is_in_receiver_list(guid_64 receiver_guid);

		guid_64					get_guid() { return m_red_package_info.base_info.red_package_guid; };
		guid_64					get_sender_guid() { return m_red_package_info.base_info.sender_guid; };
		int32					get_template_id() { return m_red_package_info.base_info.red_package_template_id; };
		xstring					get_sender_name() { return m_red_package_info.base_info.sender_name; };
		xstring					get_send_text() { return m_red_package_info.base_info.text; };
		int32					get_create_time() { return m_red_package_info.base_info.create_time; };
		int32					get_remain_times() { return m_red_package_info.base_info.remain_times; };
		int32					get_remain_num() { return m_red_package_info.base_info.remain_money_num; };
		RedPackageTemplate*		get_template_ptr();
		int32					get_money_type();
		s_red_package_receiver*	get_receiver_list() { return m_red_package_info.receiver_info_list; };
		s_red_package			get_complete_data() { return m_red_package_info; };
		
		void					set_base_info(s_red_package_base temp_info);

		//rename																																		
		void					change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步
	private:
		int32					get_random_money_num();

		s_red_package			m_red_package_info;
		RedPackageTemplate*		m_red_package_template_ptr;
		int32					m_money_type;
	};
}



#endif
