#include "AESDataConvert.h"
#include "logic/type_def.hpp"
#include "aes.h"
#include "md5/Base64.h"

#pragma warning(disable: 26400 26451)

namespace faith {
	xstring AESDataConvert::g_key = "test";
	xstring AESDataConvert::g_iv = "";

	const int32 max_aes_data_length = 2000;

	xstring AESDataConvert::DecryptionAES(const xstring& strSrc) {
		xstring strData = Base64::base64_decode(strSrc);
		size_t length = strData.length();
		if (length > max_aes_data_length) 
		{
			CONSOLE_INFO("DecryptionAES Failure, string length too long. string length = {} string:{}", length , strData);
			return xstring();
		}

		char szDataIn[max_aes_data_length + 1] = { 0 };
		memcpy(szDataIn, strData.c_str(), length + 1);
		szDataIn[length] = 0;

		char szDataOut[max_aes_data_length + 1] = { 0 };
		memcpy(szDataOut, strData.c_str(), length + 1);
		szDataOut[length] = 0;

		AES aes;
		xstring aes_key = SERVERCONFIG->http_aes;
		aes.MakeKey(aes_key.c_str(), g_iv.c_str(), 16, 16);
		aes.Decrypt(szDataIn, szDataOut, length, AES::ECB);

		// remove of PKCS7Padding filling
		if (0x00 < szDataOut[length - 1] && szDataOut[length - 1] <= 0x16)
		{
			int tmp = szDataOut[length - 1];
			for (int i = length - 1; i >= length - tmp; i--)
			{
				if (szDataOut[i] != tmp)
				{
					memset(szDataOut, 0, length);
					break;
				}
				else
					szDataOut[i] = 0;
			}
		}
		xstring strDest(szDataOut);
		return strDest;
	}

}