#include "NptDynamicRequestHandler.h"

/*----------------------------------------------------------------------
|   NPT_HttpDynamicRequestHandler::SetupResponse
+---------------------------------------------------------------------*/
NPT_Result 
	NPT_HttpDynamicRequestHandler::SetupResponse(NPT_HttpRequest& request, 
	const NPT_HttpRequestContext& context, 
	NPT_HttpResponse& response)
{
	NPT_HttpEntity* entity = response.GetEntity();
	if (entity == NULL) return NPT_ERROR_INVALID_STATE;

	// check the method
	if (request.GetMethod() != NPT_HTTP_METHOD_GET &&
		request.GetMethod() != NPT_HTTP_METHOD_HEAD) {
			response.SetStatus(405, "Method Not Allowed");
			return NPT_SUCCESS;
	}

	// set some default headers
	response.GetHeaders().SetHeader(NPT_HTTP_HEADER_ACCEPT_RANGES, "bytes");

	// declare HTTP/1.1 if the client asked for it
	if (request.GetProtocol() == NPT_HTTP_PROTOCOL_1_1) {
		response.SetProtocol(NPT_HTTP_PROTOCOL_1_1);
	}

	// check for range requests
	const NPT_String* range_spec = request.GetHeaders().GetHeaderValue(NPT_HTTP_HEADER_RANGE);

	NPT_UInt64 range_start  = 0;
	NPT_UInt64 range_end    = 0;

	if (range_spec) {    
		const NPT_String* accept_range = response.GetHeaders().GetHeaderValue(NPT_HTTP_HEADER_ACCEPT_RANGES);

		if (response.GetEntity()->GetTransferEncoding() == NPT_HTTP_TRANSFER_ENCODING_CHUNKED ||
			(accept_range && accept_range->Compare("bytes"))) {
				NPT_LOG_FINE("range request not supported");
				response.SetStatus(416, "Requested Range Not Satisfiable");
				return NPT_SUCCESS;            
		}

		// measure the stream size
		bool has_stream_size = true;
		NPT_LargeSize stream_size = m_FileSize;
		NPT_Result result;

		if (!range_spec->StartsWith("bytes=")) {
			NPT_LOG_FINE("unknown range spec");
			response.SetStatus(400, "Bad Request");
			return NPT_SUCCESS;
		}
		NPT_String valid_range;
		NPT_String range(range_spec->GetChars()+6);
		if (range.Find(',') >= 0) {
			NPT_LOG_FINE("multi-range requests not supported");
			if (has_stream_size) {
				valid_range = "bytes */";
				valid_range += NPT_String::FromInteger(stream_size);
				response.GetHeaders().SetHeader(NPT_HTTP_HEADER_CONTENT_RANGE, valid_range.GetChars());
			}
			response.SetStatus(416, "Requested Range Not Satisfiable");
			return NPT_SUCCESS;            
		}
		int sep = range.Find('-');
		bool has_start = false;
		bool has_end   = false;
		bool satisfied = false;
		if (sep < 0) {
			NPT_LOG_FINE("invalid syntax");
			response.SetStatus(400, "Bad Request");
			return NPT_SUCCESS;
		} else {
			if ((unsigned int)sep+1 < range.GetLength()) {
				result = NPT_ParseInteger64(range.GetChars()+sep+1, range_end);
				if (NPT_FAILED(result)) {
					NPT_LOG_FINE("failed to parse range end");
					return result;
				}
				range.SetLength(sep);
				has_end = true;
			}
			if (sep > 0) {
				result = range.ToInteger64(range_start);
				if (NPT_FAILED(result)) {
					NPT_LOG_FINE("failed to parse range start");
					return result;
				}
				has_start = true;
			}

			if (has_start) {
				// some clients sends incorrect range_end equal to size
				// we try to handle it
				if (!has_end || range_end == stream_size) range_end = stream_size-1; 
			} else {
				if (has_end) {
					if (range_end <= stream_size) {
						range_start = stream_size-range_end;
						range_end = stream_size-1;
					}
				}
			}
			NPT_LOG_FINE_2("final range: start=%lld, end=%lld", range_start, range_end);
			if (range_start > range_end) {
				NPT_LOG_FINE("invalid range");
				response.SetStatus(400, "Bad Request");
				satisfied = false;
			} else if (range_end >= stream_size) {
				response.SetStatus(416, "Requested Range Not Satisfiable");
				NPT_LOG_FINE("out of range");
				satisfied = false;
			} else {
				satisfied = true;
			}
		} 

		if (!satisfied) {
			if (!valid_range.IsEmpty()) response.GetHeaders().SetHeader(NPT_HTTP_HEADER_CONTENT_RANGE, valid_range.GetChars());
			response.SetStatus(416, "Requested Range Not Satisfiable");
			return NPT_SUCCESS;            
		}

		// use a portion of the file stream as a body
		entity->SetContentLength(range_end-range_start+1);
		response.SetStatus(206, "Partial Content");
		valid_range = "bytes ";
		valid_range += NPT_String::FromInteger(range_start);
		valid_range += "-";
		valid_range += NPT_String::FromInteger(range_end);
		valid_range += "/";
		valid_range += NPT_String::FromInteger(stream_size);
		response.GetHeaders().SetHeader(NPT_HTTP_HEADER_CONTENT_RANGE, valid_range.GetChars());
	} else {
		//bool update_content_length = (entity->GetTransferEncoding() != NPT_HTTP_TRANSFER_ENCODING_CHUNKED);
		// use the whole file stream as a body
		range_end    = m_FileSize-1;
	}

	typedef void (*SetRangeCBFun)(NPT_LargeSize start,NPT_LargeSize end);
	SetRangeCBFun SetRange;
	SetRange(range_start,range_end);

	NPT_InputStreamReference stream(new NPT_DynamicInputStream());
	entity->SetInputStream(stream);

	return NPT_SUCCESS;
}