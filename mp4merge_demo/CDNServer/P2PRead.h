#include "QtSequenceMerge.h"
#pragma once

typedef enum tagFlyfox_player_request_media_status_code
{
	FLYFOX_PLAYER_REQUEST_MEDIA_STATUS_CLOSED_PIECE  = 0,
	FLYFOX_PLAYER_REQUEST_MEDIA_STATUS_NETWORK_ERROR          //������ִ���
}Flyfox_player_request_media_status_code_e;

/****�������ݻص�****
* in_bNewPiece              :���ݶ�index
* in_pReceiveDataBuffer     :��������
* in_MediaPieceNum          :�������ݳ���
****/

typedef void(*flyfox_player_request_media_cb)(int nPieceIndex, unsigned char* in_pReceiveDataBuffer, int nBufferLen);
typedef void(*flyfox_player_request_media_status_cb)(int in_bPieceIndex, Flyfox_player_request_media_status_code_e e_StatusCode);

class CP2PRead
{
public:
	CP2PRead(const char* media_template_path, int in_start, int in_end);
	~CP2PRead(void);

public:
	//test
	void getSequenceInfo(SequenceInfo* out_psInfo);

private:
	SequenceInfo m_sInfo;
	vector<FILE*> m_vFileHandle;
	int m_iStartClipNumber;
	int m_iEndClipNumber;

public:
	/****�����ݻ���****
	****/
	void flyfox_media_data_cache_init(flyfox_player_request_media_cb RequestMedia_cb, 
		flyfox_player_request_media_status_cb Error_cb);

	/****�ر����ݻ���****
	****/
	void flyfox_media_data_cache_uninit();

	/****��������Ƭ��****
	*in_pPieceName :Ƭ�α�ʾ
	*in_nStartPos  :Ƭ�ο�ʼ������ʼ��  ��λ��byte
	****/
	bool flyfox_media_data_cache_request_piece(int in_nPieceIndex, int  in_nStartPos, int in_nEndPos); 
	bool flyfox_media_data_cache_cancel_request_piece(int in_nPieceIndex);

private:
	flyfox_player_request_media_cb m_cb;
	flyfox_player_request_media_status_cb m_status_cb;
};
