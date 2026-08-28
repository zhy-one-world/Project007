
#include "invalid_wide_word.h"
#include <algorithm>

#include "CInvalidWord.hpp"
#include "CCharLib.hpp"
#include <stdlib.h>

#include <string>

#include <iostream>

namespace hld
{
	//是否为一个英文字符
	bool is_an_english_charactor(wchar_t ch)
	{
		bool bSmallHalf = ( (ch>=L'a')	&& (ch<=L'z') );	//小写半角
		bool bBigHalf	= ( (ch>=L'A')	&& (ch<=L'Z') );	//大写半角
		bool bSmallAll	= ( (ch>=L'ａ') && (ch<=L'ｚ') );	//小写全角
		bool bBigAll	= ( (ch>=L'Ａ') && (ch<=L'Ｚ') );	//大写全角

		return (bSmallHalf || bBigHalf || bSmallAll	|| bBigAll);
	}

	//判断是否为一个英文单词
	bool is_an_english_word(std::wstring& word)
	{
		bool bAnWord = false;

		if(word.size())
		{
			for(size_t pos=0; pos<word.size(); ++pos)
			{			
				bAnWord = is_an_english_charactor(word[pos]);
				if(!bAnWord) return false;
			}
		}
		return bAnWord;
	}

	/**
	 *	是否存在屏蔽字符串
	 *	@param	invalid_words	:	屏蔽字库
	 *	@param	str				:	被检查的字符串
	 */
	bool include_valid_wide_str(WideWordSet& invalid_words, std::string& str)
	{
		if( !invalid_words.size() || !str.size() ) return false;

		std::wstring temp_str = from_string_to_wstring(str);
		_wcslwr_s(const_cast<wchar_t*>(temp_str.c_str()), (temp_str.size()+1));

		if( is_sentence_valid(invalid_words, temp_str) ) return true;

		for( WideWordSetIt it=invalid_words.begin(); it!=invalid_words.end(); ++it )
		{
			//if( std::wstring::npos != temp_str.find(*it) )
			//	return true;

			bool bWord = true;
			std::wstring::size_type it_pos = temp_str.find(it->c_str(), 0, it->length()-1);
			if( std::wstring::npos != it_pos )
			{
				if(is_an_english_word(*it))
				{
					if(it_pos)
					{//前向判断
						if(is_an_english_charactor(temp_str[it_pos-1]))
							bWord = false;
					}

					//if((it_pos+it->size()) != temp_str.size())
					//{//后向判断
					//	if(is_an_english_charactor(temp_str[it_pos+it->size()]))
					//		bWord = false;
					//}
				}

				if(bWord) return true;
			}
		}

		return false;
	}


	/**
	 *	是否存在屏蔽字符串
	 *	@param	invalid_words	:	屏蔽字库
	 *	@param	str				:	被检查的字符串
	 *	@param	len				:	被检查的字符串的长度
	 */
	bool include_valid_wide_str(WideWordSet& invalid_words, char* str, int32 len)
	{
		if( !invalid_words.size() || !str || len<1 ) return false;

		std::string temp_str(str, str+len);

		return include_valid_wide_str(invalid_words, temp_str);
	}

	/**
	 *	用*替换被屏蔽的字
	 *	@param	invalid_words	:	屏蔽字库
	 *	@param	str				:	被替换的字符串
	 */
	void replace_valid_wide_str(WideWordSet& invalid_words, std::string& str)
	{
		if( !invalid_words.size() || !str.size() ) return;

		std::wstring temp_str = from_string_to_wstring(str);
		_wcslwr_s(const_cast<wchar_t*>(temp_str.c_str()), (temp_str.size()+1));

		if( is_sentence_valid(invalid_words, temp_str) )
		{//增加整句屏蔽逻辑
			std::fill(str.begin(), str.end(), '*');
			return;
		}

		shield_componet_valid(invalid_words, temp_str);

		for( WideWordSetIt it=invalid_words.begin(); it!=invalid_words.end(); ++it )
		{
			int32 start_pos = 0;
			for(start_pos = temp_str.find(it->c_str(),start_pos, it->length() - 1); std::wstring::npos!=start_pos; start_pos=temp_str.find(it->c_str(), start_pos,it->length() - 1))
			{
				if( !is_a_word(temp_str, start_pos, (int32)it->size()) )
				{
					start_pos += (int32)it->size();
					continue;
				}

				int32 size_word = (*it).size();
				int32 end_pos = start_pos + size_word - 1;
				for( ; start_pos<end_pos ; ++ start_pos )
				{
					temp_str[start_pos] = (L'*');
				}
			}
		}

		std::string invalid = from_wstring_to_string(temp_str);
		resume_str(invalid, str);

	}

	/**
	 *	用*替换被屏蔽的字
	 *	@param	invalid_words	:	屏蔽字库
	 *	@param	str				:	被替换的字符串
	 *	@param	len				:	被替换的字符串的长度
	 */
	void replace_valid_wide_str(WideWordSet& invalid_words, char* str, int32 len)
	{
		if( !invalid_words.size() || !str || len<1 ) return;

		std::string	out_temp(str);
		replace_valid_wide_str(invalid_words, out_temp);

		std::copy(str, str+len, out_temp.begin());
	}


	//检测是否存在非法字符(包含两个检测过程：是否属于字库，是否是非屏蔽字)
	bool include_valid_wide_str(std::string str)
	{
		return (include_valid_wide_str(CInvalidWord::getInstance().GetInvalidWideWordSet(), str));
	}

	/** 
	 *	有非法字符时，用*替换非法字符
	 */ 
	void replace_valid_wide_str(std::string& str)
	{
		replace_valid_wide_str(CInvalidWord::getInstance().GetInvalidWideWordSet(), str);
	}


	//检查输入的整句话是否是屏蔽字
	bool is_sentence_valid(WideWordSet& invalid_words, std::wstring& wstr)
	{
// 		boost::regex expression("^select ([a-zA-Z]*) from ([a-zA-Z]*)");
// 
// 		if (boost::regex_match())
// 		{
// 		}

		//if (wstr.length() > 2)
		{
			WideWordSet::iterator it = invalid_words.begin();
			while (it != invalid_words.end())
			{
				if (wstr == (*it))
				{
					return true;
				}
				++it;
			}
			
			return false;
		}
// 		else
// 		{
// 			return (invalid_words.end() != std::find(invalid_words.begin(), invalid_words.end(), wstr));
// 		}		
	}

	//屏蔽最大组合
	void shield_componet_valid(WideWordSet& invalid_words, std::wstring& wstr)
	{
		for(WideWordSetReIt it=invalid_words.rbegin(); it!=invalid_words.rend(); ++it)
		{
			int32 start_pos	= wstr.find(it->c_str(), 0, it->length() - 1);
			if( std::wstring::npos == start_pos) continue;

			if( !is_a_word(wstr, start_pos, (int32)it->size()) ) continue;

			int32 end_pos	= start_pos + it->size() -1;
			for(  ;start_pos<end_pos; ++start_pos )
			{
				wstr[start_pos] = L'*';
			}
		}
	}

	//将检查后的屏蔽字恢复
	void resume_str(std::string& invalid, std::string& origo)
	{
		int32 cur_pos = 0;
		for(int32 index=0; index<(int32)invalid.size(); ++index)
		{
			if('*' != invalid[index])
			{
				cur_pos++;
			}
			else
			{
				if(origo[cur_pos]<0) origo[cur_pos++] = '*';
				origo[cur_pos++] = '*';
			}
		}
	}

	//判断查找一句话中找到的指定位置后的n个字符组合是否为一个单词
	bool is_a_word(std::wstring& sentence, int32 pos, int32 n)
	{
		//std::wstring allowed_chars(L"\t\v\b\f\a\n\r\\\"\'\? <>[](){}_=+-|/!@#$%^&*~`., \0");

		//if( !sentence.size() || n<1 || pos<0 ) return false;

		////前向判断
		//if( 0!=pos && 255>sentence[pos-1] )
		//{
		//	if(std::wstring::npos==allowed_chars.find(sentence[pos-1]))
		//		return false;
		//}

		////后向判断
		//if(((pos+n)<(int32)sentence.size()) && 255>sentence[pos+n])
		//	return (std::wstring::npos!=allowed_chars.find(sentence[pos+n]));

		return true;
	}

}



