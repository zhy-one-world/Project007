#include "phantom.h"
#include "template/template_manager.h"
namespace hld
{
    cphantom::cphantom()
	{
		clear_data();
	}

	cphantom::~cphantom()
	{
	}
	void cphantom::clear_data()
	{
		m_phantom_info.reset();
		m_config_ptr = nullptr;
	}
	bool cphantom::init_phantom_by_template(int32 phantom_id)
	{
		IllusionTemplate *config_ptr = GET_TEMPLATE(IllusionTemplate, phantom_id);
		if (config_ptr == nullptr)
		{
			return false;
		}
		m_config_ptr = config_ptr;
		m_phantom_info.reset();
		set_inst_data(e_phm_config_id, phantom_id);
		return true;

	}
	bool cphantom::init_phantom_by_info( s_phantom_info phantom_info)
	{
		for (int32 idx = e_phm_config_id; idx < e_phm_max; idx++)
		{
			if (set_inst_data(idx, phantom_info.data_ary[idx]) == false)
			{
				return false;
			}
		}
		int32 phantom_id = get_inst_data(e_phm_config_id);
		IllusionTemplate *config_ptr = GET_TEMPLATE(IllusionTemplate, phantom_id);
		if (config_ptr == nullptr)
		{
			return false;
		}
		m_config_ptr = config_ptr;

		return true;
	}
	bool cphantom::set_inst_data(int32 idx, int32 value)
	{
		if (idx >= e_phm_config_id && idx < e_phm_max)
		{
			m_phantom_info.data_ary[idx] = value;
			return true;
		}
		return false;
	}
	int32 cphantom::get_inst_data(int32 idx) const
	{
		if (idx >= e_phm_config_id &&  idx < e_phm_max)
		{
			return m_phantom_info.data_ary[idx];
		}
		else return -1;
	}
	int32 cphantom::get_finish_count(int32 class_type)
	{
		int32 num = 0;
		if (m_config_ptr == nullptr)
		{
			return num;
		}
		switch (class_type)
		{
		case e_phm_class_warrior:
			num = m_config_ptr->WarriorItemSet.size();
			break;
		case e_phm_class_mage:
			num = m_config_ptr->MageItemSet.size();
			break;
		case e_phm_class_knight:
			num = m_config_ptr->KnightItemSet.size();
			break;
		case e_phm_class_assassin:
			num = m_config_ptr->AssassinItemSet.size();
			break;
		default:
			break;
		}
		return num;

	}

	bool cphantom::is_finish(int32 class_type)
	{
		int32 num = get_finish_count(class_type);
		if (num == 0)
		{
			return false;
		}

		int32 start = e_phm_item_1;
		for (int32 i = 0; i < num && i < MAX_PHANTOM_PER_NUM; i++)
		{
			if (this->get_inst_data(start + i) == 0)
			{
				return false;
			}
		}
		return true;
	}
	bool cphantom::is_acitve()
	{
		int32 value = this->get_inst_data(e_phm_state);
		if (value == 0)
		{
			return false;
		}
		return true;

	}
	void cphantom::set_item_value(int32 idx, int32 value)
	{
		if (idx < 0 || idx >= MAX_PHANTOM_PER_NUM)
		{
			return;
		}
		int32 start = e_phm_item_1;
		
		this->set_inst_data(start + idx, value);
	}
	int32 cphantom::get_skill_id()
	{
		if (m_config_ptr == nullptr)
		{
			return 0;
		}
		return m_config_ptr->SkillID;
	}
	bool cphantom::is_item_finish(int32 idx)
	{
		if (idx < 0 || idx >= MAX_PHANTOM_PER_NUM)
		{
			return false;
		}
		int32 start = e_phm_item_1;

		if (this->get_inst_data(start + idx) == 0)
			return false;
		return true;
	}
	bool cphantom::is_valid()
	{
		return get_inst_data(e_phm_config_id) != 0;
	}
}
