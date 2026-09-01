// Code: 陈昆鹏
#ifndef __INVALID_ANSI_WORD_H__
#define __INVALID_ANSI_WORD_H__

#include "logic/type_def.hpp"

#include "CInvalidWord.hpp"
#include "CCharLib.hpp"

namespace faith
{
	class invalid_ansi_word
	{
	public:
		invalid_ansi_word();
		~invalid_ansi_word();
	public:
		static void init_word_lib();
		static WordSet& get_invalid_db_chars();

		/**
		 *	字体是否在指定的字库中存在
		 *	@param	words	:	标准字库
		 *	@param	str		:	被检查的字符串
		 *	@param	len		:	被检查的字符串长度
		 */
		static bool exist_in_char_lib(WordSet& words, const char* str, int32 len);

		/**
		 *	字体是否在指定的字库中存在
		 *	@param	words	:	标准字库
		 *	@param	str		:	被检查的字符串
		 */
		static bool exist_in_char_lib(WordSet& words, const std::string& str);

		/**
		 *	是否存在屏蔽字符串
		 *	@param	invalid_words	:	屏蔽字库
		 *	@param	str				:	被检查的字符串
		 */
		static bool include_invalid_ansi_str(WordSet& invalid_words, std::string& str);

		/**
		 *	是否存在屏蔽字符串
		 *	@param	invalid_words	:	屏蔽字库
		 *	@param	str				:	被检查的字符串
		 *	@param	len				:	被检查的字符串的长度
		 */
		static bool include_invalid_ansi_str(WordSet& invalid_words, const char* str, int32 len);

		/**
		 *	用*替换被屏蔽的字
		 *	@param	invalid_words	:	屏蔽字库
		 *	@param	str				:	被替换的字符串
		 */
		static void replace_invalid_ansi_str(WordSet& invalid_words, std::string& str);

		/**
		 *	用*替换被屏蔽的字
		 *	@param	invalid_words	:	屏蔽字库
		 *	@param	str				:	被替换的字符串
		 *	@param	len				:	被替换的字符串的长度
		 */
		static void replace_invalid_ansi_str(WordSet& invalid_words, char* str, int32 len);

		/**
		 *	检测是否存在非法字符
		 *	@return :  true:存在非法字符		false:不存在非法字符
		 */
		static bool include_invalid_ansi_str(std::string str);

		/**
		 *	检测是否每个字都属于可用字库
		 *	@return :  true:都是可用字符		false:有非可用字符
		 */
		static bool is_valid_ansi_str(std::string str);
		/**
		*	检查这地区是否需要检测可用字符
		*	@return :  true:需要检测		false:不需要检测
		*/
		static bool is_region_need_check_ansi_str();
		/**
		 *	有非法字符时，用*替换非法字符
		 */
		static void replace_invalid_ansi_str(std::string& str, bool only_check_symbols = false);


		//检查输入的整句话是否是屏蔽字
		static bool is_sentence_valid(WordSet& invalid_words, std::string& wstr);


		//屏蔽最大组合
		static void shield_componet_valid(WordSet& invalid_words, std::string& wstr);


		//判断查找一句话中找到的指定位置后的n个字符组合是否为一个单词
		static bool is_a_word(std::string& sentence, int32 pos, int32 n);


		//存入数据库之前 检查非法字符
		static void check_invalid_char_for_db(char* sz_str, int32 sz_len);

	private:
		static WordSet	m_invalid_db_chars;
	};
};

#endif