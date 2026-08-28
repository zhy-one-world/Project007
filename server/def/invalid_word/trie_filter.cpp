#include<string>
#include"trie_filter.h"
#include"template/template_manager.h"
namespace hld
{
	trie_filter::trie_filter()
	{
		m_sensitive_symbols_root.m_child.clear();
		load_invalid_word_lib();
		load_valid_word_lib();
	}

	trie_filter::~trie_filter()
	{
		m_invalid_word_root.m_child.clear();
		m_valid_word_root.m_child.clear();
		m_sensitive_symbols_root.m_child.clear();
	}

	void trie_filter::load_invalid_word_lib()
	{
		if (m_invalid_word_root.m_child.empty() == false)
		{
			return;
		}
		for (int32 i = start_of_invalid_word; i <= end_of_invalid_word; ++i)
		{
			const std::string& target_text = template_manager::get_instance().get_target_sensitive_text(i);
			if (target_text.empty() == false)
			{
				add_key(target_text, &m_invalid_word_root);
			}
		}
	}
	
	void trie_filter::load_valid_word_lib()
	{
		if (m_valid_word_root.m_child.empty() == false)
		{
			return;
		}
		for (int32 i = start_of_valid_word; i <= end_of_valid_word; ++i)
		{
			const std::string& target_text = template_manager::get_instance().get_target_sensitive_text(i);
			if (target_text.empty() == false)
			{
				add_key(target_text, &m_valid_word_root);
			}
		}
	}

	void trie_filter::load_sensitive_symbols_lib()
	{
		if (m_sensitive_symbols_root.m_child.empty() == false)
		{
			return;
		}
		add_key("/", &m_sensitive_symbols_root);
		add_key("\\", &m_sensitive_symbols_root);
		add_key("/", &m_sensitive_symbols_root);
		add_key("'", &m_sensitive_symbols_root);
		add_key("\"", &m_sensitive_symbols_root);
		add_key("%", &m_sensitive_symbols_root);
	}

	// add forbidden key to the trie
	void trie_filter::add_key(const std::string& key, trie_node* current_node_ptr)
	{
		if (key.empty() == true)
		{
			return;
		}

		for (int32 i = 0; i < key.size(); ++i)
		{
			//insert each char into the tree
			char current_char = key[i];
			if (current_node_ptr->m_child.find(current_char) != current_node_ptr->m_child.end())
			{
				// node exists
			}
			else
			{
				current_node_ptr->m_child[current_char] = new_node(current_char);
			}
			trie_node& child_node = current_node_ptr->m_child[current_char];
			current_node_ptr = &child_node;
		}
		current_node_ptr->m_child['\0'] = new_node('\0'); // indicate the end the word
	}

	//check whether the given string includes forbidden text
	//no change to the original string if not
	//replace forbidden part of string if includes
	void trie_filter::find_forbidden(std::string& str_to_be_checked, bool only_check_symbols)
	{
		if (str_to_be_checked.empty() == true )
		{
			return;
		}
		if (m_invalid_word_root.m_child.empty() == true)
		{
			load_invalid_word_lib();
		}
		if (m_sensitive_symbols_root.m_child.empty() == true)
		{
			load_sensitive_symbols_lib();
		}

		trie_node* use_word_root = nullptr;
		if (only_check_symbols)
		{
			use_word_root = &m_sensitive_symbols_root;
		}
		else
		{
			use_word_root = &m_invalid_word_root;
		}

		int32 forbidden_start, forbidden_end; // start and end position of the forbidden word in string
		std::string key = str_to_be_checked;
		str_to_lower_case(key);

		for (int32 i = 0; i < key.size(); ++i)
		{
			trie_node* current_node_ptr = use_word_root;
			forbidden_start = forbidden_end = -1;
			for (int32 j = i; j < key.size(); ++j)
			{
				char& current_char = key[j];

				//if exist possible child
				if (current_node_ptr->m_child.find(current_char) != current_node_ptr->m_child.end())
				{
					trie_node& child_node = current_node_ptr->m_child[current_char];
					current_node_ptr = &child_node;
					// found forbidden word
					if (current_node_ptr->m_child.find('\0') != current_node_ptr->m_child.end())
					{
						forbidden_start = i;
						forbidden_end = j;
					}
				}
				else
				{
					break;
				}
			}
			
			replace_str_with_asterisk(forbidden_start, forbidden_end, str_to_be_checked);
			i += forbidden_end - forbidden_start;
		}
	}

	//create new node with key
	trie_node trie_filter::new_node(char key)
	{
		child_map new_child_map;
		trie_node node;
		node.m_key = key;
		node.m_child = new_child_map;

		return node;
	}

	void trie_filter::replace_str_with_asterisk(int32 start, int32 end, std::string& original_string)
	{
		if (start < 0 || start > end || end >= original_string.size())
		{
			return;
		}
		for (int32 i = start; i <= end; ++i)
		{
			original_string[i] = '*';
		}
	}

	void trie_filter::str_to_lower_case(std::string&  original_string)
	{
		for (int32 i = 0; i < original_string.size(); ++i)
		{
			char& original_char = original_string[i];
			if ('A' <= original_char && original_char <= 'Z')
			{
				original_string[i] =  tolower(original_char);
			}
		}
	}
	void trie_filter::str_to_upper_case(std::string&  original_string)
	{
		for (int32 i = 0; i < original_string.size(); ++i)
		{
			char& original_char = original_string[i];
			if ('a' <= original_char && original_char <= 'z')
			{
				original_string[i] = toupper(original_char);
			}
		}
	}
	bool trie_filter::exist_forbidden(const std::string& str_to_be_checked)
	{
		if (str_to_be_checked.empty() == true)
		{
			return false;
		}
		if (m_invalid_word_root.m_child.empty() == true)
		{
			load_invalid_word_lib();
		}
		std::string key = str_to_be_checked;
		str_to_upper_case(key);
		trie_node* current_node_ptr = &m_invalid_word_root;
		for (int32 i = 0; i < key.size(); ++i)
		{
			current_node_ptr = &m_invalid_word_root;
			for (int32 j = i; j < key.size(); ++j)
			{
				const char& current_char = key[j];

				//if exist possible child
				if (current_node_ptr->m_child.find(current_char) != current_node_ptr->m_child.end())
				{
					trie_node& child_node = current_node_ptr->m_child[current_char];
					current_node_ptr = &child_node;
					// found forbidden word
					if (current_node_ptr->m_child.find('\0') != current_node_ptr->m_child.end())
					{
						return true;
					}
				}
				else
				{
					break;
				}
			}
		}
		return false;
	}

	bool trie_filter::is_str_all_valid_char(const std::string& str_to_be_checked)
	{
		if (str_to_be_checked.empty() == true)
		{
			return false;
		}
		if (m_valid_word_root.m_child.empty() == true)
		{
			load_valid_word_lib();
		}


		RegionTemplate* region_template_ptr = GET_TEMPLATE(RegionTemplate, 150000000);
		if (region_template_ptr == nullptr)
		{
			return false;
		}
		int32 region_code = region_template_ptr->RegionCode;

		std::string key = str_to_be_checked;

		switch (region_code)
		{
		case e_version_region_type::e_version_region_type_china_main:
		case e_version_region_type::e_version_region_type_kr:
		case e_version_region_type::e_version_region_type_china_tw:
		case e_version_region_type::e_version_region_type_tha:
		case e_version_region_type::e_version_region_type_jp:
		case e_version_region_type::e_version_region_type_rus:
		case e_version_region_type::e_version_region_type_sg:
		{
			str_to_upper_case(key);
		}
		break;
		case e_version_region_type::e_version_region_type_en:
		{
			str_to_lower_case(key);
		}
		break;
		default:
			str_to_upper_case(key);
			break;
		}
		//str_to_lower_case(key);
		//str_to_upper_case(key);
		trie_node* current_node_ptr = &m_valid_word_root;
		for (int32 i = 0; i < key.size();)
		{
			current_node_ptr = &m_valid_word_root;
			bool cur_word_find = false;
			int32 j = i;
			for (; j < key.size(); ++j)
			{
				const char& current_char = key[j];

				if (current_node_ptr->m_child.find(current_char) == current_node_ptr->m_child.end())
				{
					cur_word_find = false;
					break;
				}

				trie_node& child_node = current_node_ptr->m_child[current_char];
				current_node_ptr = &child_node;

				if (current_node_ptr->m_child.find('\0') != current_node_ptr->m_child.end())
				{
					cur_word_find = true;
					break;
				}
			}

			if (false == cur_word_find)
			{
				return false;
			}

			i = j + 1;
		}

		return true;
	}
}