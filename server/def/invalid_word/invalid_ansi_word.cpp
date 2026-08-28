
#include "invalid_ansi_word.h"
#include <algorithm>

#include "CInvalidWord.hpp"
#include "CCharLib.hpp"
#include "trie_filter.h"
#include "utility/globle_data.h"
#include <stdlib.h>


#include <string>

#include <iostream>

namespace hld
{
	WordSet invalid_ansi_word::m_invalid_db_chars;

	invalid_ansi_word::invalid_ansi_word()
	{
	}

	invalid_ansi_word::~invalid_ansi_word()
	{
	}

	void invalid_ansi_word::init_word_lib()
	{
		m_invalid_db_chars.clear();
		xchar invalid_chars[32] = { ';', '`' };
		for (int32 i = 0; i < 32; i++)
		{
			xchar _char = invalid_chars[i];
			if (_char == '\0')
			{
				break;
			}
			m_invalid_db_chars.insert(std::string(&_char, &_char + 1));
		}
	}

	WordSet& invalid_ansi_word::get_invalid_db_chars()
	{
		if (m_invalid_db_chars.empty())
		{
			init_word_lib();
		}
		return m_invalid_db_chars;
	}

	/**
	 *	字体是否在指定的字库中存在
	 *	@param	words	:	标准字库
	 *	@param	str		:	被检查的字符串
	 *	@param	len		:	被检查的字符串长度
	 */
	bool invalid_ansi_word::exist_in_char_lib(WordSet& words, const char* str, int32 len)
	{
		if(!words.size() || !str || len < 1) return true;

		for(int32 index=0; index < len; ++index)
		{
			std::string cur_word;
			if(str[index] > 0)
			{//读取ASII中的字符
				cur_word.push_back(str[index]);
			}
			else
			{//读取一个汉字字符
				cur_word.push_back(str[index++]);
				cur_word.push_back(str[index]);
			}

			if(words.end() == words.find(cur_word))
			{//不存在字库中
				return false;
			}
		}

		return true;
	}


	/**
	 *	字体是否在指定的字库中存在
	 *	@param	words	:	标准字库
	 *	@param	str		:	被检查的字符串
	 */
	bool invalid_ansi_word::exist_in_char_lib(WordSet& words, const std::string& str)
	{
		return exist_in_char_lib(words, (char*)str.c_str(), (int32)str.size());
	}

	/**
	 *	是否存在屏蔽字符串
	 *	@param	invalid_words	:	屏蔽字库
	 *	@param	str				:	被检查的字符串
	 */
	bool invalid_ansi_word::include_invalid_ansi_str(WordSet& invalid_words, std::string& str)
	{
		if (invalid_words.empty() || str.empty())
		{
			return false;
		}
		bool bWord = false;
		for( WordSetIt it=invalid_words.begin(); it!=invalid_words.end(); ++it )
		{
			if (str.find(*it) != std::string::npos)
			{
				bWord = true;
				break;
			}
		}
		return bWord;
	}


	/**
	 *	是否存在屏蔽字符串
	 *	@param	invalid_words	:	屏蔽字库
	 *	@param	str				:	被检查的字符串
	 *	@param	len				:	被检查的字符串的长度
	 */
	bool invalid_ansi_word::include_invalid_ansi_str(WordSet& invalid_words, const char* str, int32 len)
	{
		if (invalid_words.empty() || !str || len < 1)
		{
			return false;
		}
		std::string temp_str(str, str+len);

		return include_invalid_ansi_str(invalid_words, temp_str);
	}

	/**
	 *	用*替换被屏蔽的字
	 *	@param	invalid_words	:	屏蔽字库
	 *	@param	str				:	被替换的字符串
	 */
	void invalid_ansi_word::replace_invalid_ansi_str(WordSet& invalid_words, std::string& str)
	{
		if (!invalid_words.size() || !str.size())
		{
			return;
		}

		if( is_sentence_valid(invalid_words, str) )
		{//增加整句屏蔽逻辑
			std::fill(str.begin(), str.end(), '*');
			return;
		}

		shield_componet_valid(invalid_words, str);
	}

	/**
	 *	用*替换被屏蔽的字
	 *	@param	invalid_words	:	屏蔽字库
	 *	@param	str				:	被替换的字符串
	 *	@param	len				:	被替换的字符串的长度
	 */
	void invalid_ansi_word::replace_invalid_ansi_str(WordSet& invalid_words, char* str, int32 len)
	{
		if( !invalid_words.size() || !str || len<1 ) return;

		std::string	out_temp(str);
		replace_invalid_ansi_str(invalid_words, out_temp);

		std::copy(str, str+len, out_temp.begin());
	}

	//检测是否存在非法字符
	bool invalid_ansi_word::include_invalid_ansi_str(std::string str)
	{
		return trie_filter::get_instance().exist_forbidden(str);
		//return (include_invalid_ansi_str(CInvalidWord::getInstance().GetInvalidWordSet(), str));
	}

	//检测是否每个字都属于可用字库
	bool invalid_ansi_word::is_valid_ansi_str(std::string str)
	{
		if (false == is_region_need_check_ansi_str())
		{
			return true;
		}
		return trie_filter::get_instance().is_str_all_valid_char(str);
	}

	bool invalid_ansi_word::is_region_need_check_ansi_str()
	{
		RegionTemplate* region_temp_ptr = globle_data::get_instance().get_region_template_ptr();
		if (nullptr == region_temp_ptr)
		{
			return true;
		}
		switch (region_temp_ptr->RegionCode)
		{
		case e_version_region_type_tha:
		case e_version_region_type_jp:
		case e_version_region_type_rus:
		case e_version_region_type_sg:
		{
			return false;
		}
		break;
		default:
			break;
		}
		return true;
	}

	/** 
	 *	有非法字符时，用*替换非法字符
	 */ 
	void invalid_ansi_word::replace_invalid_ansi_str(std::string& str, bool only_check_symbols)
	{
		return trie_filter::get_instance().find_forbidden(str, only_check_symbols);
		//replace_invalid_ansi_str(CInvalidWord::getInstance().GetInvalidWordSet(), str);
	}


	//检查输入的整句话是否是屏蔽字
	bool invalid_ansi_word::is_sentence_valid(WordSet& invalid_words, std::string& str)
	{
		WordSet::iterator it = invalid_words.begin();
		while (it != invalid_words.end())
		{
			if (str == (*it))
			{
				return true;
			}
			++it;
		}
		return false;
	}

	//屏蔽最大组合
	void invalid_ansi_word::shield_componet_valid(WordSet& invalid_words, std::string& str)
	{
		for(WordSetIt it=invalid_words.begin(); it!=invalid_words.end(); )
		{
			int32 start_pos	= str.find(it->c_str(), 0, it->length());
			if (std::string::npos == start_pos)
			{
				++it;
				continue;
			}
			if (!is_a_word(str, start_pos, (int32)it->size()))
			{
				++it;
				continue;
			}
			int32 end_pos = start_pos + it->size();
			for(  ;start_pos<end_pos; ++start_pos )
			{
				str[start_pos] = '*';
			}
		}
	}


	//判断查找一句话中找到的指定位置后的n个字符组合是否为一个单词
	bool invalid_ansi_word::is_a_word(std::string& sentence, int32 pos, int32 n)
	{
		return true;

		//std::string allowed_chars(L"\t\v\b\f\a\n\r\\\"\'\? <>[](){}_=+-|/!@#$%^&*~`., \0");

		//if( !sentence.size() || n<1 || pos<0 ) return false;

		////前向判断
		//if( 0!=pos && 255>sentence[pos-1] )
		//{
		//	if(std::string::npos==allowed_chars.find(sentence[pos-1]))
		//		return false;
		//}

		////后向判断
		//if(((pos+n)<(int32)sentence.size()) && 255>sentence[pos+n])
		//	return (std::string::npos!=allowed_chars.find(sentence[pos+n]));

		//return true;
	}

	void invalid_ansi_word::check_invalid_char_for_db(char* sz_str, int32 sz_len)
	{
		//exist_in_char_lib(m_invalid_db_chars, sz_str, sz_len);
		for (int32 i = 0; i < sz_len; i++)
		{
			if ('\0' == sz_str[i])
			{
				break;
			}
			if (include_invalid_ansi_str(get_invalid_db_chars(), &(sz_str[i]), 1))
			{
				sz_str[i] = ' ';
			}
		}
	}
}
