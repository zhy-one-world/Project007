#include "components/play/time_activity_component.h"
#include "template/template_manager.h"
#include "internet/time_activity.pb.h"

using namespace faith;
c_time_activity::c_time_activity()
{
	m_activity_id = 0;
	m_activity_end_time = 0;
	m_activity_template = nullptr;
	m_activity_data = nullptr;
}
void c_time_activity::set_data_info(int32 info_index, int32 value)
{
	m_activity_data->set_data_info(info_index, value);
}
void c_time_activity::add_data_info(int32 info_index, int32 value)
{
	m_activity_data->add_data_info(info_index, value);
}
int32 c_time_activity::get_data_info(int32 info_index)
{
	return m_activity_data->get_data_info(info_index);
}
void c_time_activity::clear_data()
{
	return m_activity_data->clear_data_info();
}
std::shared_ptr<s_time_activity_base> c_time_activity::create_data(e_time_activity_type activity_type)
{
	if (activity_type == e_time_activity_type_wheel_plate)
	{
		m_activity_data = std::make_shared<s_wheel_plate_info>();
	}
	clear_data();
	return m_activity_data;
}
void c_time_activity::set_activity_id(int32 activity_id, int64 end_time)
{
	m_activity_id = activity_id;
	m_activity_end_time = end_time;
	m_activity_template = GET_TEMPLATE(TimeActivityTemplate, m_activity_id);
	m_activity_data = create_data(e_time_activity_type(m_activity_template->ActivityType));
}
void c_time_activity::load_data(const time_activity_s2s_activity_info& msg, int64 end_time)
{
	set_activity_id(msg.activity_id(), end_time);
	if (msg.has_wheel_plate_data())
	{
		auto& msg_data = msg.wheel_plate_data();
		for (int32 i = 0; i < msg_data.data_array_size(); i++)
		{
			m_activity_data->set_data_info(i, msg_data.data_array(i));
		}
	}
}bool c_time_activity::is_end_del()
{
	if (m_activity_template->ActivityType == e_time_activity_type_wheel_plate)
	{
		if (m_activity_data->get_data_info(e_wheel_plate_info_open_index) >= m_activity_template->ActivityLogic.size() / 2)
		{
			return m_activity_template->CompleteHide != 0;
		}
		else
		{
			return false;
		}
	}
	return false;
}
void c_time_activity::generate_db_data(time_activity_s2s_activity_info* msg)
{
	msg->set_activity_id(m_activity_id);
	if (m_activity_template->ActivityType == e_time_activity_type_wheel_plate)
	{
		auto activity_data = std::dynamic_pointer_cast<s_wheel_plate_info>(m_activity_data);
		auto msg_data = msg->mutable_wheel_plate_data();
		for (int32 i = 0; i < e_wheel_plate_info_max; i++)
		{
			msg_data->add_data_array(activity_data->get_data_info(i));
		}
	}
}
void c_time_activity::generate_client_data(time_activity_s2c_activity_info* msg)
{
	msg->set_activity_id(m_activity_id);
	msg->set_end_time(m_activity_end_time);
	if (m_activity_template->ActivityType == e_time_activity_type_wheel_plate)
	{
		auto activity_data = std::dynamic_pointer_cast<s_wheel_plate_info>(m_activity_data);
		auto msg_data = msg->mutable_wheel_plate_data();
		for (int32 i = 0; i < e_wheel_plate_info_max; i++)
		{
			msg_data->add_data_array(activity_data->get_data_info(i));
		}
	}
}