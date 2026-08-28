#ifndef _CHAR_LIB_H_
#define _CHAR_LIB_H_

#include <string>
#include <set>
#include <singleton.hpp>
#include "logic/type_def.hpp"
#include "template/ServerTemplateHead.h"

typedef std::set<std::string>	WordSet;
typedef WordSet::iterator		WordSetIt;

/*
 *	游戏允许的有效字库
 */
namespace hld
{
	class CCharLib : public hld::singleton<CCharLib>
	{
	private:
		friend class hld::singleton< CCharLib >;

	private:
		WordSet		m_word_set;
	public:
		void LoadResource();
		WordSet& GetWordSet(void);
	};
}


#endif
