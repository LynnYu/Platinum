#include "NPTDynamicInputStream.h"
/*----------------------------------------------------------------------
|   NPT_DynamicInputStream::Read
+---------------------------------------------------------------------*/
NPT_Result
NPT_DynamicInputStream::Read(void* buffer, 
	                         NPT_Size bytes_to_read, 
	                         NPT_Size* bytes_read /* = NULL */)
{
	NPT_Byte* data=new NPT_Byte[bytes_to_read];
	NPT_Size size;

	NPT_CopyMemory(buffer,data,size);
	bytes_read=&size;
	return NPT_SUCCESS;
}
