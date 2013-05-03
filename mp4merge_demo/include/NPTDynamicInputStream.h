#ifndef _NPT_DYNAMIC_STREAM_H_
#define _NPT_DYNAMIC_STREAM_H_
#include "NptStreams.h"

/*----------------------------------------------------------------------
|   NPT_DynamicInputStream
+---------------------------------------------------------------------*/
class NPT_DynamicInputStream : public NPT_InputStream
{
public:
	// methods
	NPT_DynamicInputStream(){}
	virtual ~NPT_DynamicInputStream() {}

	NPT_Result Read(void*     buffer, 
		NPT_Size  bytes_to_read, 
		NPT_Size* bytes_read = NULL);
	NPT_Result Seek(NPT_Position offset)
	{
		return NPT_ERROR_NOT_SUPPORTED;
	}
	NPT_Result Tell(NPT_Position& offset)
	{
		offset=0;
		return NPT_SUCCESS;
	}
	NPT_Result GetSize(NPT_LargeSize& size)
	{
		return NPT_ERROR_NOT_SUPPORTED;
	}
	NPT_Result GetAvailable(NPT_LargeSize& available)
	{
		return NPT_ERROR_NOT_SUPPORTED;
	}
};

typedef NPT_Reference<NPT_DynamicInputStream> NPT_DynamicInputStreamReference;

#endif
