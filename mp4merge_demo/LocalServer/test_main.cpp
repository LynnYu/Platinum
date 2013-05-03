#include "StreamControl.h"
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	CStreamCtrl stream;
	char* name="������è2[�����]";
	int type=2;
	//char* name="D:\\�ز�\\�ֶκϲ�\\������è2\\������è2[�����].mp4";
	//int type=1;
	stream.Open(name,type);
	unsigned __int64 fileSize;
	stream.GetSize(&fileSize);
	const unsigned int Step=128*1024;
	unsigned int bytes_read=0;
	unsigned int pos=0;
	unsigned char* buffer=new unsigned char[Step];

	ofstream out(L"D:\\������è2[�����]����.mp4",ios::out|ios::binary);
	bool isheader=true;
	while(pos<fileSize)
	{
		stream.Read(buffer,Step,&bytes_read);
		if(bytes_read==0)
			break;
		pos+=bytes_read;
		out.write((char*)buffer,bytes_read);
	}

	out.close();
	delete []buffer;
	buffer=NULL;
	return 0;
}