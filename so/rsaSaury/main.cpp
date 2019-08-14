#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#define PASS "" //口令

#define PUBLICKEY "publicKey.pem"
#define PRIVATEKEY "privateKey.pem"

using namespace std;

bool Base64Encode(const string& input, string* output) {
	typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<string::const_iterator, 6, 8> > Base64EncodeIterator;
	stringstream result;
	copy(Base64EncodeIterator(input.begin()), Base64EncodeIterator(input.end()), ostream_iterator<char>(result));
	size_t equal_count = (3 - input.length() % 3) % 3;
	for (size_t i = 0; i < equal_count; i++) {
		result.put('=');
	}
	*output = result.str();
	return output->empty() == false;
}

bool Base64Decode(const string& input, string* output) {
	typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<string::const_iterator>, 8, 6> Base64DecodeIterator;
	stringstream result;
	try {
		copy(Base64DecodeIterator(input.begin()), Base64DecodeIterator(input.end()), ostream_iterator<char>(result));
	}
	catch (...) {
		return false;
	}
	*output = result.str();
	return output->empty() == false;
}

// 私钥解密    
std::string rsa_pri_decrypt(const std::string& cipherText, std::string& priKey)
{
	std::string strRet;
	std::string temp_strRet;
	RSA* rsa = RSA_new();
	BIO* keybio;
	keybio = BIO_new_mem_buf((unsigned char*)priKey.c_str(), -1);

	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	int len = RSA_size(rsa);
	char* decryptedText = (char*)malloc(len + 1);
	memset(decryptedText, 0, len + 1);
	int step = 256;    //解密长度限制
	int olen = cipherText.length();
	int ret = 0;
	for (int i = 0; i < olen; i = i + step)
	{
		memset(decryptedText, '\0', len + 1);
		string tmp_str = cipherText.substr(i, step);
		ret = RSA_private_decrypt(tmp_str.length(), (const unsigned char*)tmp_str.c_str(), (unsigned char*)decryptedText, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0) {
			temp_strRet = std::string(decryptedText, ret);
			strRet.append(temp_strRet);
		}
	}

	// 释放内存  
	free(decryptedText);
	BIO_free_all(keybio);
	RSA_free(rsa);
	return strRet;
}

// 公钥加密    
std::string rsa_pub_encrypt(const std::string& clearText, std::string& pubKey)
{
	std::string strRet;
	std::string temp_strRet;

	bool with_new_line = false;
	BIO* keybio = BIO_new_mem_buf((unsigned char*)pubKey.c_str(), -1);
	RSA* rsa = RSA_new();
	rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	if (!rsa)
	{
		BIO_free_all(keybio);
		return NULL;
	}
	int len = RSA_size(rsa);
	char* encryptedText = (char*)malloc(len + 1);
	memset(encryptedText, 0, len + 1);

	// 加密函数  
	int step = 245;    //加密长度限制
	int olen = clearText.length();
	int ret = 0;
	for (int i = 0; i < olen; i = i + step)
	{
		memset(encryptedText, '\0', len + 1);
		string tmp_str = clearText.substr(i, step);
		ret = RSA_public_encrypt(tmp_str.length(), (const unsigned char*)tmp_str.c_str(), (unsigned char*)encryptedText, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0) {
			temp_strRet = std::string(encryptedText, ret);
			strRet.append(temp_strRet);
		}
	}

	// 释放内存  
	free(encryptedText);
	BIO_free_all(keybio);
	RSA_free(rsa);
	return strRet;
}

std::string ReadFile(const char* pFileName)
{
	ifstream inFile(pFileName);
	string contents("");
	if (inFile.is_open())
	{
		std::stringstream buffer;
		buffer << inFile.rdbuf();
		contents.append(buffer.str());
	}

	inFile.close();

	return contents.c_str();
}

extern "C" int RsaEncrypt(char* str, char*& outstr)//char* str
{
	bool with_new_line = false;
	std::string publicRsa;

	publicRsa = ReadFile(PUBLICKEY);
	//char* result;
	//std::string result;
	int cc = 0;


	std::string ret = "";
	//ret = rsa_pub_encrypt("aasdddtyu中tyu", publicRsa);
	ret = rsa_pub_encrypt(str, publicRsa);
	//cout << ret << endl;

	string base64_str;
	Base64Encode(ret, &base64_str);


	std::ofstream fout;
	fout.open("test.txt");
	fout << base64_str;
	fout.close();

	//strcpy(outstr, str_e_base64.c_str());

	//strcpy(outstr, ppp.c_str());
	strcpy(outstr, base64_str.c_str());
	cc = 1;
	return cc;
}

extern "C" int RsaDecrypt(char* str, char*& outstr)//char* str
{
	bool with_new_line = false;
	std::string privateRsa;

	privateRsa = ReadFile(PRIVATEKEY);
	//char* result;
	//std::string result;
	int cc = 0;

	std::string str_e_base64 = str;
	string output_str;
	Base64Decode(str_e_base64, &output_str);
	std::string ret = rsa_pri_decrypt(output_str, privateRsa);

	/*std::ofstream fout;
	fout.open("de_test.txt");
	fout << str_e_base64;
	fout.close();*/

	//strcpy(outstr, str_e_base64.c_str());

	//strcpy(outstr, ppp.c_str());
	strcpy(outstr, ret.c_str());
	cc = 1;
	return cc;
}
