#pragma once

#include <cstdlib>
#include <cstring>
#include <string>

#include <tinyxml/tinyxml.h>

namespace faith
{
	inline TiXmlElement* xml_child(TiXmlElement* parent, const char* name)
	{
		return parent ? parent->FirstChildElement(name) : nullptr;
	}

	inline std::string xml_text(TiXmlElement* element, const char* default_value = "")
	{
		if (element == nullptr || element->GetText() == nullptr)
		{
			return default_value ? default_value : "";
		}
		return element->GetText();
	}

	inline std::string xml_child_text(TiXmlElement* parent, const char* name, const char* default_value = "")
	{
		return xml_text(xml_child(parent, name), default_value);
	}

	inline int xml_child_int(TiXmlElement* parent, const char* name, int default_value = 0)
	{
		const std::string text = xml_child_text(parent, name, "");
		if (text.empty())
		{
			return default_value;
		}
		return std::atoi(text.c_str());
	}

	inline bool xml_child_bool(TiXmlElement* parent, const char* name, bool default_value = false)
	{
		const std::string text = xml_child_text(parent, name, "");
		if (text.empty())
		{
			return default_value;
		}
		return text == "1" || text == "true" || text == "TRUE" || text == "True";
	}

	inline std::string xml_child_text_or_attr(
		TiXmlElement* parent,
		const char* name,
		const char* attr_name,
		const char* default_value = "")
	{
		TiXmlElement* child = xml_child(parent, name);
		if (child == nullptr)
		{
			return default_value ? default_value : "";
		}
		if (attr_name != nullptr)
		{
			const char* attr = child->Attribute(attr_name);
			if (attr != nullptr && attr[0] != '\0')
			{
				return attr;
			}
		}
		return xml_text(child, default_value);
	}
}
