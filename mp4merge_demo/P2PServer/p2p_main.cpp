#include <iostream>
#include <cstdio>
#include "..\include\Neptune.h"
#include "..\include\Platinum.h"
#include "..\include\NptLogging.h"
#include "..\include\NptDynamicRequestHandler.h"

#include <string>
#include <Windows.h>

#ifdef _DEBUG
#pragma comment( lib, "..\\lib\\debug\\Neptune.lib")
#pragma comment( lib, "..\\lib\\debug\\Platinum.lib")
#else
#pragma comment( lib, "..\\lib\\release\\Neptune.lib")
#pragma comment( lib, "..\\lib\\release\\Platinum.lib")
#endif

using namespace std;

int main()
{
	NPT_LogManager::GetDefault().Configure("plist:.level=ALL;.handlers=FileHandler;"
		".FileHandler.filename=C:\\log\\p2pserver.log;"
		".FileHandler.filter=9;.FileHandler.append=false");

	PLT_HttpServer cdn_server(NPT_IpAddress::Any,8082);
	char* url="/������è2[�����]";
	NPT_HttpDynamicRequestHandler dynamic_handle(true,NULL,"Z:\\Shared\\Media\\������è2");
	cdn_server.AddRequestHandler(&dynamic_handle,url);
	NPT_String url2="/���ه�;̩֮��[�����]";
	cdn_server.AddRequestHandler(&dynamic_handle,url2.GetChars());

	cdn_server.Start();
	cout<<"The P2P server is running..."<<endl;

	getc(stdin);

	cdn_server.Stop();
	cout<<"The P2P server is turned off."<<endl;

	return 0;
}