/********************************************************************
created:	2015/03/27
created:	27:3:2015   18:00
file base:	TemplateServerBase
file ext:	h
author:		zhy

purpose:
*********************************************************************/
#pragma once
#include "logic/type_def.hpp"
#include "template/ServerTemplateEnum.h"
namespace hld
{
#pragma pack(push, 1)
	struct template_base
	{
		int32   attribute_id;						// ID
		template_base& operator= (const template_base & temp)
		{
			attribute_id = temp.attribute_id;
			return *this;
		}
	};
#pragma pack(pop)
	class template_size
	{
	public:
		void init_template_size();
		void init_template_size_for_dp();
	private:
		void init_template_base()
		{
			template_size_map.resize(e_template_max);
			template_key_map.resize(e_template_max);
			for (int32 i = 0; i < e_template_max; i++)
			{
				template_size_map[i].clear();
				template_key_map[i].clear();
			}
			template_size_map[e_template_base].push_back("int32");
			template_size_map[e_template_base].push_back("template_base&");
			template_size_map[e_template_base].push_back("attribute_id");
			template_size_map[e_template_base].push_back("return");
			template_size_map[e_template_base].push_back("}");
		}
	public:
		static template_size& get_instance()
		{
			static template_size s_template_size;
			return s_template_size;
		}
	public:
		std::vector<std::vector<std::string>> template_size_map;
		std::vector<std::vector<std::string>> template_key_map;
	};
}
