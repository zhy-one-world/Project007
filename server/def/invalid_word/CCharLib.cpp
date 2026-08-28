#include "CCharLib.hpp"
#include "template/template_manager.h"
namespace hld
{
	WordSet& CCharLib::GetWordSet(void)
	{
		return m_word_set;
	}


	void CCharLib::LoadResource()
	{
		for (int32 i = 94020001; i <= 94036797; ++i)
		{
			const std::string& target_text = template_manager::get_instance().get_target_sensitive_text(i);
			if (target_text.empty()==false)
			{
				m_word_set.insert(target_text);
			}
		}
	}
}