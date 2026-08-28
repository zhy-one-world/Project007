#pragma once
#include "logic/type_def.hpp"




namespace hld
{
	struct trie
	{
		char m_key;
		std::map<char, trie> m_child;
	};

	typedef struct trie							trie_node;
	typedef std::map<char, trie>				child_map;

	const int32 start_of_invalid_word = 94000001;
	const int32 end_of_invalid_word = 94199999;
	const int32 start_of_valid_word = 94200001;
	const int32 end_of_valid_word = 94209999;


	class trie_filter
	{
	private:
		trie_node m_invalid_word_root;
		trie_node m_valid_word_root;
		trie_node m_sensitive_symbols_root;
	public:
		static trie_filter&	get_instance()
		{
			static trie_filter instance;
			return instance;
		}
	public:
		trie_filter();
		~trie_filter();
	public:
		void							load_invalid_word_lib();
		void							load_valid_word_lib();
		void							load_sensitive_symbols_lib();
		void							add_key(const std::string& key, trie_node* current_node_ptr);	//add word to the tree
		void							find_forbidden(std::string& text, bool only_check_symbols = false);								//find all forbidden word and replace with asterisk(*)			
		void							replace_str_with_asterisk(int32 start, int32 end, std::string& original_string);  // replace forbidden word with asterisk(*)
		void							str_to_lower_case(std::string& original_string);				//return the lower case string
		void							str_to_upper_case(std::string& original_string);				//return the upper case string
		bool							exist_forbidden(const std::string& str_to_be_checked);
		bool							is_str_all_valid_char(const std::string& str_to_be_checked);
	private:
		trie_node						new_node(char key);
	};














}