
#include "csv_parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "boost/algorithm/string.hpp"
#include <vector>
#include <forward_list>
#include "boost/algorithm/string/erase.hpp"

using namespace hld;

/*
** csv_row
*/

void csv_row::set_value(const std::string& key, const std::string& value)
{
	m_values[key] = value;
}

const std::string csv_row::get_value(const std::string& key) const
{
	auto it = m_values.find(key);
	if (it != m_values.end())
	{
		return it->second;
	}
	return "0";
}
/*
** csv_parser
*/

std::vector<std::shared_ptr<csv_row>> _content;

void csv_parser::load(const std::string& data)
{
	std::ifstream ifile(data.c_str());

	if (ifile.is_open())
	{
		std::string line;
		bool is_begin = true;
		std::vector<std::string>	header;//first row of file
		_content.clear();

		while (ifile.good())
		{
			getline(ifile, line);
			if (is_begin)
			{
				is_begin = false;
				boost::split(header, line, boost::is_any_of(","));
				for (auto& it : header)
				{
					boost::algorithm::erase_head(it, 1);
					boost::algorithm::erase_tail(it, 1);
				}
			}
			else
			{
				bool quoted = false;
				int token_start = 0;
				int32_t i = 0;
				bool need_filter_quoted = false;
				int32_t row_index = 0;

				auto row = std::make_shared<csv_row>();

				for (; i != line.length(); i++)
				{
					if (line.at(i) == '"')
					{
						quoted = ((quoted) ? (false) : (true));
						need_filter_quoted = true;
					}
					else if (line.at(i) == ',' && !quoted)
					{
						if (need_filter_quoted)
						{
							need_filter_quoted = false;
							std::string temp_value = line.substr(token_start + 1, i - token_start - 2);
							remove_double_quot_escape(temp_value);
							row->set_value(header[row_index++], temp_value);
						}
						else
						{
							row->set_value(header[row_index++], line.substr(token_start, i - token_start));
						}

						token_start = i + 1;
					}
				}
				if (need_filter_quoted)
				{
					need_filter_quoted = false;
					std::string temp_value = line.substr(token_start + 1, line.length() - token_start - 2);
					remove_double_quot_escape(temp_value);
					row->set_value(header[row_index++], temp_value);
				}
				else
				{
					row->set_value(header[row_index++], line.substr(token_start, line.length() - token_start));
				}
				_content.push_back(row);
			}
		}
		ifile.close();
	}
	else
	{
		std::cout << "Error: file not found file:"<< data << std::endl;
	}
}

std::shared_ptr<csv_row> csv_parser::get_row(int32_t row_position)
{
	if (row_position >= _content.size())
	{
		return nullptr;
	}

	return _content[row_position];
}

int32_t csv_parser::get_row_count(void)
{
	return _content.size();
}
void csv_parser::remove_double_quot_escape(std::string& str)
{
	std::string str_src = "\"\"";
	std::string str_des = "\"";
	std::string::size_type pos = 0;
	std::string::size_type src_len = str_src.size();
	std::string::size_type des_len = str_des.size();
	pos = str.find(str_src, pos);
	while (pos != std::string::npos)
	{
		str.replace(pos, src_len, str_des);
		pos = str.find(str_src, pos + des_len);
	}
}