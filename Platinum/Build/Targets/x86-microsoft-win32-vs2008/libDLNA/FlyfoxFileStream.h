#ifndef _FLYFOX_FILE_STREAM_H_
#define _FLYFOX_FILE_STREAM_H_

// enum {
// 	FILE_TYPE_LOCAL		= 1,
// 	FILE_TYPE_ONLINE	= 2,
// };

class CFlyfoxFileInputStream : public NPT_InputStream
{
public:
	CFlyfoxFileInputStream(){};
	~CFlyfoxFileInputStream(){};
};

class CFlyfoxFileOutpuStream : public NPT_OutputStream
{
public:
	CFlyfoxFileOutpuStream() {};
	~CFlyfoxFileOutpuStream() {};
};

class CFlyfoxFileStream : public CFlyfoxFileInputStream
						/*, public CFlyfoxFileOutpuStream*/
{
public:
	CFlyfoxFileStream(IOCallbacks* io)
	{
		m_IOCallbacks = io;/*CLibDLNA::GetIOCallbacks();*/
	}

	~CFlyfoxFileStream() {
	}

	// NPT_FileInterface methods
	virtual NPT_Result Open(const char* path, int type) {
		m_Path = path;
		if (m_IOCallbacks && m_IOCallbacks->Open)
			return m_IOCallbacks->Open(path, type);
		else
			return NPT_ERROR_NOT_IMPLEMENTED;
	};

	virtual NPT_Result Close() {
		if (m_IOCallbacks && m_IOCallbacks->Close)
			return m_IOCallbacks->Close();
		else
			return NPT_ERROR_NOT_IMPLEMENTED;
	};

	virtual NPT_Result GetInputStream(NPT_InputStreamReference& stream) {
		stream = this;
		return NPT_SUCCESS;
	};

// 	virtual NPT_Result GetOutputStream(NPT_OutputStreamReference& stream) {
// 		stream = NULL;
// 		return NPT_SUCCESS;
// 	};

	// NPT_InputStream methods
	virtual NPT_Result Read(void* buffer, NPT_Size bytes_to_read, NPT_Size* bytes_read = NULL) {
		if (m_IOCallbacks && m_IOCallbacks->Read)
			return m_IOCallbacks->Read(buffer, bytes_to_read, bytes_read);
		else 
			return NPT_ERROR_NOT_IMPLEMENTED;
	}
	virtual NPT_Result Seek(NPT_Position offset) {
		if (m_IOCallbacks && m_IOCallbacks->Seek)
			return m_IOCallbacks->Seek(offset);
		return NPT_ERROR_NOT_IMPLEMENTED;
	}
	virtual NPT_Result Tell(NPT_Position& offset) {
		if (m_IOCallbacks && m_IOCallbacks->Tell)
			return m_IOCallbacks->Tell(&offset);
		return NPT_ERROR_NOT_IMPLEMENTED;
	}
	virtual NPT_Result GetSize(NPT_LargeSize& size) {
		if (m_IOCallbacks && m_IOCallbacks->GetSize)
			return m_IOCallbacks->GetSize(&size);
		return NPT_ERROR_NOT_IMPLEMENTED;
	}
	virtual NPT_Result GetAvailable(NPT_LargeSize& available) {
		if (m_IOCallbacks && m_IOCallbacks->GetAvailable)
			return m_IOCallbacks->GetAvailable(&available);
		return NPT_ERROR_NOT_IMPLEMENTED;
	}

	// methods
	const NPT_String&   GetPath() { return m_Path; }

protected:
	NPT_String			m_Path;
	IOCallbacks*		m_IOCallbacks;
};


#endif