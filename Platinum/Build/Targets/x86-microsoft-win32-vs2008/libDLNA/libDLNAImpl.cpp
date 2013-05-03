#include "stdafx.h"
#include "libdlnaimpl.h"

#include "FlyfoxMediaServer.h"
#include "FlyfoxMediaController.h"
#include "MP4Merge/FlyfoxStreamCtrl.h"

#define IP_LOCAL_HOST	"127.0.0.1"

//NPT_SET_LOCAL_LOGGER("libdlna.libdlnaimpl")

IOCallbacks* CLibDLNA::m_spIO = NULL;

CFlyfoxStreamCtrl* CLibDLNA::m_StreamCtrl = NULL;

/*----------------------------------------------------------------------
|   CLibDLNA::CLibDLNA
+---------------------------------------------------------------------*/
CLibDLNA::CLibDLNA()
: m_UPnP(NULL)
, m_DLNAServer(NULL)
, m_MediaController(NULL)
{

}

/*----------------------------------------------------------------------
|   CLibDLNA::~CLibDLNA
+---------------------------------------------------------------------*/
CLibDLNA::~CLibDLNA()
{
	Uninit();
}

/*----------------------------------------------------------------------
|   CLibDLNA::Init
+---------------------------------------------------------------------*/
NPT_Result CLibDLNA::Init()
{
	m_UPnP = new PLT_UPnP;

	m_DLNAServer = new CFlyfoxMediaServer("Platinum UPnP Media Server");
	m_DLNAServer->m_ModelDescription = "Platinum Media Server";
	m_DLNAServer->m_ModelURL = "http://www.plutinosoft.com/";
	m_DLNAServer->m_ModelNumber = "1.0";
	m_DLNAServer->m_ModelName = "Platinum File Media Server";
	m_DLNAServer->m_Manufacturer = "Plutinosoft";
	m_DLNAServer->m_ManufacturerURL = "http://www.plutinosoft.com/";

	// add device
	m_UPnP->AddDevice(m_DLNAServer);
	//m_UPnP->SetIgnoreLocalUUIDs(false);

	PLT_CtrlPointReference ctrlPoint(new PLT_CtrlPoint());

	m_MediaController = new CFlyfoxMediaController(ctrlPoint);

	m_UPnP->AddCtrlPoint(ctrlPoint);
	m_UPnP->Start();

	m_StreamCtrl = new CFlyfoxStreamCtrl;

	return NPT_SUCCESS;
}

/*----------------------------------------------------------------------
|   CLibDLNA::Uninit
+---------------------------------------------------------------------*/
NPT_Result CLibDLNA::Uninit()
{
	if (!m_UPnP.IsNull())
		m_UPnP->Stop();

	if (m_MediaController)
	{
		delete m_MediaController;
		m_MediaController = NULL;
	}

	if (m_StreamCtrl)
	{
		delete m_StreamCtrl;
		m_StreamCtrl = NULL;
	}
	return NPT_SUCCESS;
}

/*----------------------------------------------------------------------
|   
+---------------------------------------------------------------------*/
NPT_Result CLibDLNA::DiscoverDevices(char devices[DLNA_MAX_DEVICE_COUNT][DLNA_MAX_DEVICE_NAME_LEN], int* count)
{
	if (m_MediaController)
	{
		return m_MediaController->DiscoverDevices(devices, count);
	}
	return NPT_SUCCESS;
}

/*----------------------------------------------------------------------
|   
+---------------------------------------------------------------------*/
NPT_Result CLibDLNA::ChooseDevice(int device_index)
{
	if (m_MediaController)
	{
		return m_MediaController->SetCurMediaRenderer(device_index);
	}
	return NPT_SUCCESS;
}

/*----------------------------------------------------------------------
|   
+---------------------------------------------------------------------*/
NPT_Result CLibDLNA::Open(const char* url, int file_type)
{
	NPT_Result result = NPT_SUCCESS;
	if (m_MediaController)
	{
		PLT_DeviceDataReference device;
		m_MediaController->GetCurMediaRenderer(device);
		if (!device.IsNull())
		{
			NPT_String http_url;
			NPT_String ip = device->GetLocalIP().ToString();
			int port = m_DLNAServer->GetPort();
			NPT_String file_name = NPT_FilePath::BaseName(url);
			//http_url = NPT_String::Format("http://%s:%s/%s?type=%d", ip, port, name, type);

			NPT_HttpUrl base_uri(IP_LOCAL_HOST, port, NPT_HttpUrl::PercentEncode("/", NPT_Uri::PathCharsToEncode));
			http_url = CFlyfoxMediaServerDelegate::BuildSafeResourceUri(base_uri, ip, /*url*/url, file_type);

			NPT_LOG_INFO("SetAVTransportURI http_url=%s");//, http_url.GetChars());
			result = m_MediaController->SetAVTransportURI(device, 0, http_url/*url*/, "", NULL);
		}
	}
	return result;
}

/*----------------------------------------------------------------------
|   
+---------------------------------------------------------------------*/
NPT_Result CLibDLNA::Close()
{
	return NPT_SUCCESS;
}

/*----------------------------------------------------------------------
|   
+---------------------------------------------------------------------*/
NPT_Result CLibDLNA::Play()
{
	if (m_MediaController)
	{
		PLT_DeviceDataReference device;
		m_MediaController->GetCurMediaRenderer(device);
		if (!device.IsNull()) {
			m_MediaController->Play(device, 0, "1", NULL);
		}
	}
	return NPT_SUCCESS;
}

/*----------------------------------------------------------------------
|   
+---------------------------------------------------------------------*/
NPT_Result CLibDLNA::Pause()
{
	if (m_MediaController)
	{
		PLT_DeviceDataReference device;
		m_MediaController->GetCurMediaRenderer(device);
		if (!device.IsNull()) {
			m_MediaController->Pause(device, 0, NULL);
		}
	}
	return NPT_SUCCESS;
}

/*----------------------------------------------------------------------
|   
+---------------------------------------------------------------------*/
NPT_Result CLibDLNA::Stop()
{
	if (m_MediaController)
	{
		PLT_DeviceDataReference device;
		m_MediaController->GetCurMediaRenderer(device);
		if (!device.IsNull()) {
			m_MediaController->Stop(device, 0, NULL);
		}
	}
	return NPT_SUCCESS;
}

/*----------------------------------------------------------------------
|   
+---------------------------------------------------------------------*/
NPT_Result CLibDLNA::Seek(const char* time_pos/*long play_pos_ms*/)
{
	if (m_MediaController)
	{
		PLT_DeviceDataReference device;
		m_MediaController->GetCurMediaRenderer(device);
		if (!device.IsNull()) 
		{
			NPT_String target(time_pos);
			m_MediaController->Seek(device, 0, "REL_TIME", target, NULL);
		}
	}
	return NPT_SUCCESS;
}

/*----------------------------------------------------------------------
|   
+---------------------------------------------------------------------*/
NPT_Result CLibDLNA::GetVolume(int* volume)
{
	return NPT_SUCCESS;	
}

/*----------------------------------------------------------------------
|   
+---------------------------------------------------------------------*/
NPT_Result CLibDLNA::SetVolume(int volume)
{
	return NPT_SUCCESS;
}
