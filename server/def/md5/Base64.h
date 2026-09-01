/*base_64.h文件*/
#ifndef BASE_64_H
#define BASE_64_H
/**
* Base64 编码/解码
* @author liruixing
*/
#include <singleton.hpp>
#include "logic/type_def.hpp"
#include <string.h>
namespace faith {
	class Base64 {
	private:
		static std::string _base64_table;
		static const char base64_pad = '=';
	public:
		/**
		* 这里必须是unsigned类型，否则编码中文的时候出错
		*/
		static std::string Encode(const unsigned char * str, int bytes);
		static std::string Decode(const char *str, int bytes);

		static std::string base64_encode(unsigned char const* bytes_to_encode, uint32 in_len);
		static std::string base64_decode(std::string const& encoded_string);
	};
}
#endif