#pragma once
#ifndef AESDATACONVERT_H_
#define AESDATACONVERT_H_

#include "logic/type_def.hpp"

namespace faith {

	class AESDataConvert {

	public:
		AESDataConvert() {};
		static void InitSecretKey(const xstring secretkey);
		static xstring DecryptionAES(const xstring& strSrc);

	private:
		static xstring g_key;
		static xstring g_iv;
	};

}

#endif
