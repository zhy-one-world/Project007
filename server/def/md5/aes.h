///////////////////////////////
// http://mingcn.cnblogs.com //
//  xelz CopyRight (c) 2010  //
///////////////////////////////


#if !defined(AFX_AES_H__6BDD3760_BDE8_4C42_85EE_6F7A434B81C4__INCLUDED_)
#define AFX_AES_H__6BDD3760_BDE8_4C42_85EE_6F7A434B81C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "openssl/aes.h"
#include "string"
#include <exception>
#include <cstring>
#include "logic/type_def.hpp"
using namespace std;

namespace hld {
	class AES
	{
	public:
		AES();
		virtual ~AES();
	public:
		static std::string aes_128_ecb_encrypt(const std::string& source, const std::string& key);
		static std::string aes_128_ecb_decrypt(const std::string& ciphertext, const std::string& key);

	public:
		enum
		{
			ECB = 0, CBC = 1, CFB = 2
		};

	private:
		enum
		{
			DEFAULT_BLOCK_SIZE = 16
		};
		enum
		{
			MAX_BLOCK_SIZE = 32, MAX_ROUNDS = 14, MAX_KC = 8, MAX_BC = 8
		};

	private:
		//Key Initialization Flag
		bool m_bKeyInit;
		//Encryption (m_Ke) round key
		int32 m_Ke[MAX_ROUNDS + 1][MAX_BC];
		//Decryption (m_Kd) round key
		int32 m_Kd[MAX_ROUNDS + 1][MAX_BC];
		//Key Length
		int32 m_keylength;
		//Block Size
		int32 m_blockSize;
		//Number of Rounds
		int32 m_iROUNDS;
		//Chain Block
		char m_chain0[MAX_BLOCK_SIZE];
		char m_chain[MAX_BLOCK_SIZE];
		//Auxiliary private use buffers
		int32 tk[MAX_KC];
		int32 a[MAX_BC];
		int32 t[MAX_BC];
	private:
		void Xor(char* buff, char const* chain);
		void DefEncryptBlock(char const* in, char* result);
		void DefDecryptBlock(char const* in, char* result);
		void EncryptBlock(char const* in, char* result);
		void DecryptBlock(char const* in, char* result);
	public:
		void MakeKey(char const* key, char const* chain, int32 keylength =
			DEFAULT_BLOCK_SIZE, int32 blockSize = DEFAULT_BLOCK_SIZE);
		void Encrypt(char const* in, char* result, size_t n, int32 iMode = ECB);
		void Decrypt(char const* in, char* result, size_t n, int32 iMode = ECB);
	};
}
#endif // !defined(AFX_AES_H__6BDD3760_BDE8_4C42_85EE_6F7A434B81C4__INCLUDED_)
