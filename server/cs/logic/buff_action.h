/********************************************************************
	created:	2010/03/02
	created:	2:3:2010   17:33
	filename: 	d:\worksite\skill\lowserver\octopus\app\cs\low\new_buff\buff_action.h
	file path:	d:\worksite\skill\lowserver\octopus\app\cs\low\new_buff
	file base:	buff_action
	file ext:	h
	author:		zhangjianyu
	
	purpose:	
*********************************************************************/
#ifndef _BUFF_ACTION_H_
#define _BUFF_ACTION_H_

#include "Logic/att_def.hpp"
#include "Logic/buff_def.hpp"

namespace faith
{
	namespace buff_str
	{
		//清楚指定的字符
		void 
		clear_char( 
			char* szaction, 
			char cone, 
			char ctwo = 0 );

		//寻找指定的分割字符
		int32	
		find_delimter( 
			char* szaction, 
			char cone, 
			char ctwo = 0 );

		//转换小写
		void 
		conv_lower_case( 
			char* szaction );

		int32	
		check_act( 
			char* szaction );

		pbuffaction	find_act(const std::string& szaction);
	};		

	/************************************************************************/
	/*                     class buff_action                                */
	/************************************************************************/
	class buff_action
	{
		struct _function
		{
			_function()
			{
				func_ptr = nullptr;
			}
			pbuffaction	func_ptr;
		};
			


		enum _cond_op
		{
			op_none,
			op_and,
			op_or,
			op_neg,
		};
			

		struct _condition
		{
			_condition()
			{
				m_len = 0;
				m_pos = 0;
			}
			bool parse(char* szcondition, uint32 len);
			bool parse_func();
			bool parse_param(std::string& strparam);
			bool parse_operator();
			void trim();
			bool parse_over();
			bool valid() {return !m_functions.empty();}
			bool is_condition_right(buff_env_param& env, BuffActionTemplate* action_template_ptr);

			std::string				m_strcondition;
			int32						m_len;
			uint32					m_pos;
			std::vector<_function>	m_functions;
			std::vector<_cond_op>	m_operators;
		};

	public:

		buff_action( )
		{
			clear_data( );
		}

		bool operator( )(buff_env_param&);

		void set_template(BuffActionTemplate* action_template_ptr);

		bool is_auto( void )
		{
			return m_bauto;
		}

		bool is_condition_right( buff_env_param& env );
	private:

		void parse_action(const std::string& szaction);

		void clear_data( )
		{
			m_bauto = false;
 			memset(&m_condition, 0, sizeof(m_condition));
 			memset(&m_action, 0, sizeof(m_action));
			m_buff_action_template = nullptr;
		}
	private:
		bool					m_bauto;
 		BuffActionTemplate*		m_buff_action_template;	
		_function				m_action;
		_function				m_condition;
		_condition				m_condition_exp;
	};

	//=====================================================================================
}

#endif // buff_action_h_