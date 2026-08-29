// Code: 陈昆鹏
#ifndef __INVALID_WIDE_WORD_H__
#define __INVALID_WIDE_WORD_H__

#include "logic/type_def.hpp"
#include "CInvalidWord.hpp"
#include "CCharLib.hpp"

namespace faith
{
	/**
	 *	是否存在屏蔽字符串
	 *	@param	invalid_words	:	屏蔽字库
	 *	@param	str				:	被检查的字符串
	 */
	bool include_valid_wide_str(WideWordSet& invalid_words,	std::string& str);

	/**
	 *	是否存在屏蔽字符串
	 *	@param	invalid_words	:	屏蔽字库
	 *	@param	str				:	被检查的字符串
	 *	@param	len				:	被检查的字符串的长度
	 */
	bool include_valid_wide_str(WideWordSet& invalid_words, char* str, int32 len);

	/**
	 *	用*替换被屏蔽的字
	 *	@param	invalid_words	:	屏蔽字库
	 *	@param	str				:	被替换的字符串
	 */
	void replace_valid_wide_str(WideWordSet& invalid_words,	std::string& str);

	/**
	 *	用*替换被屏蔽的字
	 *	@param	invalid_words	:	屏蔽字库
	 *	@param	str				:	被替换的字符串
	 *	@param	len				:	被替换的字符串的长度
	 */
	void replace_valid_wide_str(WideWordSet& invalid_words, char* str, int32 len);

	/** 
	 *	检测是否存在非法字符(包含两个检测过程：是否属于字库，是否是非屏蔽字)
	 *	@return :  true:存在非法字符		false:不存在非法字符
	 */
	bool include_valid_wide_str(std::string str);

	/** 
	 *	有非法字符时，用*替换非法字符
	 */
	void replace_valid_wide_str(std::string& str);


	//检查输入的整句话是否是屏蔽字
	bool is_sentence_valid(WideWordSet& invalid_words, std::wstring& wstr);


	//屏蔽最大组合
	void shield_componet_valid(WideWordSet& invalid_words, std::wstring& wstr);

	//将检查后的屏蔽字恢复
	void resume_str(std::string& invalid, std::string& origo);

	//判断查找一句话中找到的指定位置后的n个字符组合是否为一个单词
	bool is_a_word(std::wstring& sentence, int32 pos, int32 n);
};

#endif