#include <iostream>
#include<Windows.h>
#include<string>

using namespace std;

typedef int(*testFunc)(char* str, char*& outstr);
typedef const wchar_t* LPCWSTR;

int main()
{
	const char* dllName = "./rsaSaury.dll";
	HMODULE hDll = LoadLibrary(dllName);
	testFunc re = (testFunc)GetProcAddress(hDll, "RsaEncrypt");
	testFunc rd = (testFunc)GetProcAddress(hDll, "RsaDecrypt");
	if (!re || !rd)
	{
		cout << "Error" << endl;
	}
	else
	{
		char* text = new char[200];
		strcpy(text, "test中文");
		char* outstr = new char[80000];
		int ret = re(text, outstr);
		//cout << std::string(rettext, strlen((char*)rettext)) << endl;
		cout << outstr << endl;
		ret = rd(outstr, outstr);
		cout << outstr << endl;
	}
	FreeLibrary(hDll);

	system("pause");
	return 0;
}