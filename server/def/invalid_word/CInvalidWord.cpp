#include "CInvalidWord.hpp"
#include "invalid_ansi_word.h"
#include "invalid_wide_word.h"
#include "trie_filter.h"
#include <stdlib.h>
#include <stdio.h>
#include "common\random.hpp"
#include "template/template_manager.h"

namespace hld
{
	WideWordSet& CInvalidWord::GetInvalidWideWordSet(void)
	{
		return m_invalid_word_set;
	}

	void CInvalidWord::LoadResource()
	{
		//trie_filter::get_instance().load_invalid_word_lib();
	}


	void CInvalidWord::AddWStr(std::wstring& wstr)
	{
		int32 start = 0;
		int32 end = (int32)m_invalid_word_set.size();
		int32 mid = 0;
		int32 cur_len = (int32)wstr.size();

		for (mid = (start + end) / 2; start < end; mid = (start + end) / 2)
		{
			((int32)(m_invalid_word_set[mid].size()) < cur_len) ? (start = mid + 1) : (end = mid);
		}

		m_invalid_word_set.insert(m_invalid_word_set.begin() + mid, 1, wstr);
	}

	int32 get_wchar_num_in_string(std::string& str)
	{
		int32 wlen = 0;
		for (int32 index = 0; index < str.size(); ++index)
		{
			if (str[index] < 0) ++index;
			wlen++;
		}
		return wlen + 1;
	}

	std::wstring from_string_to_wstring(std::string& str)
	{
		if (!str.size()) return std::wstring(L"");

		setlocale(LC_ALL, "chs");

		std::wstring wstr;
		wstr.resize(get_wchar_num_in_string(str), 0);
		int32 copy_end = mbstowcs_s(NULL, const_cast<wchar_t*>(wstr.c_str()), wstr.size(), str.c_str(), _TRUNCATE);
		if (0 != copy_end)
		{
			return std::wstring(L"");
		}

		setlocale(LC_ALL, "utf8");

		if (wstr.empty())
		{
			return std::wstring(L"");
		}

		//for( ;L'\0'== *wstr.rbegin();  )
		//	wstr.erase(wstr.end()-1);

		return wstr;
	}
	std::string from_wstring_to_string(std::wstring& wstr)
	{
		setlocale(LC_ALL, "chs");

		std::string str(2 * wstr.size() + 1, '\0');
		wcstombs_s(NULL, const_cast<char*>(str.c_str()), str.size(), wstr.c_str(), _TRUNCATE);

		setlocale(LC_ALL, "utf8");
		return str;
	}
}