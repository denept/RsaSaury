#include <dlfcn.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>

using namespace std;

int main()
{
	void* handle = dlopen("./librsaSaury.so", RTLD_LAZY);

	if (!handle)
	{
		printf("open lib error\n");
		cout << dlerror() << endl;
		return -1;
	}

	typedef int(*RsaEncrypt)(char* str, char*& outstr);
	RsaEncrypt re = (RsaEncrypt)dlsym(handle, "RsaEncrypt");
	if (!re)
	{
		cout << dlerror() << endl;
		dlclose(handle);
		return -1;
	}
	char* text = new char[4];
	strcpy(text, "asds");
	char* outstr = new char[80000];
	int rettext = re(text, outstr);

	cout << outstr << endl;

	dlclose(handle);
    return 0;
}