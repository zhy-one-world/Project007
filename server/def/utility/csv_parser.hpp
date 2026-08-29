#pragma  once
# include <string>
#include <map>
#include <memory>

namespace faith
{
	class csv_row
	{
	public:
		void set_value(const std::string& key, const std::string& value);
		const std::string get_value(const std::string& key) const;
	private:
		std::map<std::string, std::string> m_values;
	};



	class csv_parser
	{
	public:
		static void load(const std::string& data);

		static std::shared_ptr<csv_row> get_row(int32_t row);
		static int32_t get_row_count(void);
	protected:
		static void remove_double_quot_escape(std::string& str);
	};
}
