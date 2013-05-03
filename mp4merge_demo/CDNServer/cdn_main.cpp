#include <iostream>
#include <cstdio>
#include "..\include\Neptune.h"
#include "..\include\platinum.h"

#include "..\include\NptLogging.h"
#include "..\include\NptDynamicRequestHandler.h"

#include "P2PRead.h"

#ifdef _DEBUG
#pragma comment( lib, "..\\lib\\debug\\Neptune.lib")
#pragma comment( lib, "..\\lib\\debug\\Platinum.lib")
#else
#pragma comment( lib, "..\\lib\\release\\Neptune.lib")
#pragma comment( lib, "..\\lib\\release\\Platinum.lib")
#endif

using namespace std;

void GetMP4HeaderData(const char* name,NPT_Byte* &data,unsigned int* size)
{
	NPT_String path_name=name;
	path_name+="��%d��.mp4";
	FILE* file_t = NULL;
	unsigned int icount(0),istart(0);
	do 
	{
		char* tmp_path=const_cast<char*>(path_name.GetChars());
		char media_path[266];
		sprintf_s(media_path, tmp_path, icount);
		errno_t err = fopen_s(&file_t, media_path, "rb");
		if (file_t)
		{
			icount++;
			fclose(file_t);
		}
	} while (file_t);

	CP2PRead p2p_read(path_name.GetChars(),istart,icount);
	SequenceInfo sInfo;
	p2p_read.getSequenceInfo(&sInfo);
	//calculate the length of buffer
	unsigned int length=4;
	unsigned int inum=sInfo.nSectionCount;
	for (unsigned int i=0;i<inum;++i)
	{
		length=length+4+sInfo.pSectioninfo[i].ui32SectionInfoBufferSize;
	}
	*size=length;
	data=new NPT_Byte[*size];
	NPT_Byte* pdata=data;
	NPT_CopyMemory(pdata,&sInfo.nSectionCount,4);
	pdata+=4;
	for (unsigned int i=0;i<inum;++i)
	{
		unsigned int buffer_size=sInfo.pSectioninfo[i].ui32SectionInfoBufferSize;
		NPT_CopyMemory(pdata,&buffer_size,4);
		pdata+=4;
		NPT_CopyMemory(pdata,sInfo.pSectioninfo[i].pSectionInfoBuffer,buffer_size);
		pdata+=buffer_size;
	}
}

int main()
{
	NPT_LogManager::GetDefault().Configure("plist:.level=ALL;.handlers=FileHandler;"
		".FileHandler.filename=C:\\log\\cdnserver.log;"
		".FileHandler.filter=9;.FileHandler.append=false");

	PLT_HttpServer cdn_server(NPT_IpAddress::Any,8080);

	NPT_String url="/������è2[�����]";
	GetDataBufferCallBack func=GetMP4HeaderData;
	NPT_HttpDynamicRequestHandler dynamic_handle(false,func,"Z:\\Shared\\Media\\������è2");
	cdn_server.AddRequestHandler(&dynamic_handle,url.GetChars());
	NPT_String url2="/���ه�;̩֮��[�����]";
	cdn_server.AddRequestHandler(&dynamic_handle,url2.GetChars());

	cdn_server.Start();
	cout<<"The CDN server is running..."<<endl;

	getc(stdin);

	cdn_server.Stop();
	cout<<"The CDN server is turned off."<<endl;

	return 0;
}