/********************************************************************
created: 2019年03月27日
file base: csv_synchronization_mgr
file ext: cpp
author: wangdi

purpose: csv同步信息管理器
*********************************************************************/

#include "csv_synchronization_mgr.h"
#include "net.pb.h"
#include "template/template_manager.h"

namespace hld
{

	csv_synchronization_mgr::csv_synchronization_mgr()
	{

	}

	csv_synchronization_mgr::~csv_synchronization_mgr()
	{

	}

	void csv_synchronization_mgr::clear_data()
	{

	}

	void csv_synchronization_mgr::send_template(client_session* session_ptr)
	{
		send_title_template("TitleTemplate", session_ptr);
		send_oracle_trial_template("OracleTrialTemplate", session_ptr);
	}

	void csv_synchronization_mgr::send_title_template(xstring table_name, client_session* session_ptr)
	{
		template_manager::template_type* template_table = template_manager::get_instance().get_templates(e_TitleTemplate);
		if (nullptr == template_table)
		{
			return;
		}
		if (nullptr == session_ptr)
		{
			return;
		}
		bool if_first_send = true;
		game_proto_set_template_table resp;
		resp.set_tabel_name(table_name);
		for (auto ite = template_table->begin(); ite != template_table->end(); ++ite)
		{
			TitleTemplate* template_line = (TitleTemplate*)(ite->second);
			if (nullptr == template_line)
			{
				continue;
			}
			if (resp.ByteSize() > packege_length_max)
			{

				if (session_ptr->is_vaild())
				{
					if (if_first_send)
					{
						resp.set_if_first_send(1);
						if_first_send = false;
					}
					else
					{
						resp.set_if_first_send(0);
					}
					session_ptr->send_to_client(&resp, e_msgindex_s2c_time_limit_activity_template);
				}
				resp.clear_template_data();
			}
			game_proto_template_table_data* activity_template_data = resp.add_template_data();
			fill_title_template_one_msg(template_line, activity_template_data);
		}
		if (if_first_send)
		{
			resp.set_if_first_send(1);
		}
		else
		{
			resp.set_if_first_send(0);
		}
		if (session_ptr->is_vaild())
		{
			session_ptr->send_to_client(&resp, e_msgindex_s2c_time_limit_activity_template);
		}
	}

	void csv_synchronization_mgr::fill_title_template_one_msg(TitleTemplate* template_line, game_proto_template_table_data* activity_template_data)
	{
		activity_template_data->set_attribute_id(template_line->attribute_id);
		game_proto_template_table_param_int*  activity_template_param_int = NULL;
		game_proto_template_table_param_string*  activity_template_param_string = NULL;
		int32	time_limit_activity_data_int;
		std::string	time_limit_activity_data_string;

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->attribute_id;
		activity_template_param_int->set_param_name("attribute_id");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->HighTitleType;
		activity_template_param_int->set_param_name("HighTitleType");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->EndTimeType;
		activity_template_param_int->set_param_name("EndTimeType");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->HighTitleName;
		activity_template_param_int->set_param_name("HighTitleName");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->HighTitleString;
		activity_template_param_int->set_param_name("HighTitleString");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->HighTitleIcon;
		activity_template_param_int->set_param_name("HighTitleIcon");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->HighTitleChatIcon;
		activity_template_param_int->set_param_name("HighTitleChatIcon");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->PlayerType;
		activity_template_param_int->set_param_name("PlayerType");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->AdditionArray.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = boost::to_string(template_line->AdditionArray[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += boost::to_string(template_line->AdditionArray[i]);
			}
		}
		activity_template_param_string->set_param_name("AdditionArray");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->StaticAttArray.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = boost::to_string(template_line->StaticAttArray[i]);
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += boost::to_string(template_line->StaticAttArray[i]);
			}
		}
		activity_template_param_string->set_param_name("StaticAttArray");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->LastTime;
		activity_template_param_int->set_param_name("LastTime");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->DeadActiveLine;
		activity_template_param_int->set_param_name("DeadActiveLine");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->NeedValue1;
		activity_template_param_int->set_param_name("NeedValue1");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->NeedValue2;
		activity_template_param_int->set_param_name("NeedValue2");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_int = activity_template_data->add_param_int();
		time_limit_activity_data_int = template_line->Noticeld;
		activity_template_param_int->set_param_name("Noticeld");
		activity_template_param_int->set_param_date(time_limit_activity_data_int);

		activity_template_param_string = activity_template_data->add_param_string();
		time_limit_activity_data_string.clear();
		for (int32 i = 0; i < template_line->ChannelLimit.size(); i++)
		{
			if (i == 0)
			{
				time_limit_activity_data_string = template_line->ChannelLimit[i];
			}
			else
			{
				time_limit_activity_data_string += ",";
				time_limit_activity_data_string += template_line->ChannelLimit[i];
			}
		}
		activity_template_param_string->set_param_name("ChannelLimit");
		activity_template_param_string->set_param_date(time_limit_activity_data_string);
	}

	void csv_synchronization_mgr::send_oracle_trial_template(xstring table_name, client_session* session_ptr)
	{
		template_manager::template_type* template_table = template_manager::get_instance().get_templates(e_OracleTrialTemplate);
		if (nullptr == template_table)
		{
			return;
		}
		if (nullptr == session_ptr)
		{
			return;
		}
		bool if_first_send = true;
		game_proto_set_template_table resp;
		resp.set_tabel_name(table_name);
		for (auto ite = template_table->begin(); ite != template_table->end(); ++ite)
		{
			OracleTrialTemplate* template_line = (OracleTrialTemplate*)(ite->second);
			if (nullptr == template_line)
			{
				continue;
			}

			if (resp.ByteSize() > packege_length_max)
			{
				if (if_first_send)
				{
					resp.set_if_first_send(1);
					if_first_send = false;
				}
				else
				{
					resp.set_if_first_send(0);
				}
				if (session_ptr->is_vaild())
				{
					session_ptr->send_to_client(&resp, e_msgindex_s2c_time_limit_activity_template);
				}
				resp.clear_template_data();
			}
			game_proto_template_table_data* template_data = resp.add_template_data();
			template_data->set_attribute_id(template_line->attribute_id);
			game_proto_template_table_param_int*	 template_param_int = NULL;
			game_proto_template_table_param_string*  template_param_string = NULL;
			int32	data_int;
			std::string	data_string;

			template_param_int = template_data->add_param_int();
			data_int = template_line->attribute_id;
			template_param_int->set_param_name("attribute_id");
			template_param_int->set_param_date(data_int);

			template_param_string = template_data->add_param_string();
			data_string.clear();
			for (int32 i = 0; i < template_line->MapId.size(); i++)
			{
				if (i == 0)
				{
					data_string = boost::to_string(template_line->MapId[i]);
				}
				else
				{
					data_string += ",";
					data_string += boost::to_string(template_line->MapId[i]);
				}
			}
			template_param_string->set_param_name("MapId");
			template_param_string->set_param_date(data_string);

			template_param_string = template_data->add_param_string();
			data_string.clear();
			for (int32 i = 0; i < template_line->AwardNum.size(); i++)
			{
				if (i == 0)
				{
					data_string = boost::to_string(template_line->AwardNum[i]);
				}
				else
				{
					data_string += ",";
					data_string += boost::to_string(template_line->AwardNum[i]);
				}
			}
			template_param_string->set_param_name("AwardNum");
			template_param_string->set_param_date(data_string);

			template_param_string = template_data->add_param_string();
			data_string.clear();
			for (int32 i = 0; i < template_line->MapId.size(); i++)
			{
				if (i == 0)
				{
					data_string = boost::to_string(template_line->MapId[i]);
				}
				else
				{
					data_string += ",";
					data_string += boost::to_string(template_line->MapId[i]);
				}
			}
			template_param_string->set_param_name("AwardId");
			template_param_string->set_param_date(data_string);
			if (if_first_send)
			{
				resp.set_if_first_send(1);
			}
			else
			{
				resp.set_if_first_send(0);
			}
			if (session_ptr->is_vaild())
			{
				session_ptr->send_to_client(&resp, e_msgindex_s2c_time_limit_activity_template);
			}
		}
	}

}