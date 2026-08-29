#pragma once
#ifndef AESDATACONVERT_H_
#define AESDATACONVERT_H_

#include <singleton.hpp>
#include "logic/type_def.hpp"
#include <string.h>
#include "server_log.hpp"

namespace faith {
	
	class AESDataConvert {

	public:
		AESDataConvert() {};
		static void InitSecretKey(const xstring secretkey) { 
			g_key = secretkey; 
			console_info("AESDataConvert::InitSecretKey secretkey: {}", secretkey);
		}
		//static xstring EncryptionAES(const xstring& strSrc);
		static xstring DecryptionAES(const xstring& strSrc);

	private:
		static xstring g_key;
		static xstring g_iv;		//ECB MODE不需要关心chain，可以填空
	};

}

#endif AESDATACONVERT_H_