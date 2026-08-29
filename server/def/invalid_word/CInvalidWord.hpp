#ifndef _INVALID_WORD_H_
#define _INVALID_WORD_H_

#include "CCharLib.hpp"
#include "logic/type_def.hpp"

//add by leo
#include <string>
namespace faith
{
	typedef	std::vector<std::wstring>		WideWordSet;
	typedef	WideWordSet::iterator			WideWordSetIt;
	typedef	WideWordSet::reverse_iterator	WideWordSetReIt;
#include <locale>

	std::wstring	from_string_to_wstring(std::string& str);
	std::string		from_wstring_to_string(std::wstring& wstr);
	int32			get_wchar_num_in_string(std::string& str);
	/*
	*	ÓÎÏ·µÄÆÁ±Î×Ö¿â
	*/
	class CInvalidWord : public faith::singleton< CInvalidWord >
	{
	private:
		friend class faith::singleton< CInvalidWord >;


	private:
		WideWordSet	m_invalid_word_set;
		WideWordSet	m_invalid_name_set;
		bool		m_is_test;
		WordSet		m_origin_invalid_words;

	private:
		void AddWStr(std::wstring& wstr);
	public:
		void LoadResource();
		WideWordSet& GetInvalidWideWordSet(void);
		WordSet& GetInvalidWordSet(void) { return m_origin_invalid_words; }
	};
}
#endif
