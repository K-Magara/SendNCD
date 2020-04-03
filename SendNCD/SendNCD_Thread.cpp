// Thread Function
// �رّ��M
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SendNCD.h"
#include "SendNCDInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
	�{���Ȃ� SendThread() ���璼�� NCVC_GetNCStrData() ���Ăяo��
	NC���ނ��擾���Ȃ��瑗�M����\�肾�������CSendThread() ����
	NCVC_GetNCStrData() ���ĂԂƁC�A�N�Z�X�ᔽ�̃G���[�ɂȂ�̂ŁC
	CSendNCDInfo �׽�\�z���� m_strGcode �֓o�^���C������Q�Ƃ���D
*/
UINT SendThread(LPVOID pParam)
{
#ifdef _DEBUG
	printf("SendThread() Start\n");
#endif
	CSendNCDInfo*	pParent = (CSendNCDInfo *)pParam;
	ASSERT(pParent);
//	NCVCHANDLE		hDoc    = pParent->m_hDoc;
	HANDLE			hCom    = pParent->m_hCom;
//	ASSERT(hDoc);
	ASSERT(hCom);
	DWORD	dwWrite;
	BOOL	fWrite;
//	char	szBuf[256];
//	int		nLen;

	::PurgeComm(hCom, PURGE_TXCLEAR);	// �ޯ̧�ر
	// �S�đ��M���I��邩�e�گ�ނ̌p���׸ނ��^�̂������J��Ԃ�
	for ( int i=0; i<pParent->m_strGcode.GetSize() && pParent->m_fThread; i++ ) {
		// NC���ގ󂯎��
/*
		nLen = NCVC_GetNCStrData(hDoc, i, szBuf, sizeof(szBuf));
		if ( nLen < 0 ) {
			AfxMessageBox(IDS_ERR_WRITECOM, MB_OK|MB_ICONSTOP);
			fWrite = FALSE;
			break;
		}
*/
#ifdef _DEBUG
		printf("Line=%d Gcode=%s\n", i, LPCTSTR(pParent->m_strGcode[i]));
#endif
		// �رّ��M
		fWrite = ::WriteFile(hCom, pParent->m_strGcode[i], pParent->m_strGcode[i].GetLength(),
						&dwWrite, NULL);
		if ( !fWrite || dwWrite != (DWORD)(pParent->m_strGcode[i].GetLength()) ) {
			AfxMessageBox(IDS_ERR_WRITECOM, MB_OK|MB_ICONSTOP);
			fWrite = FALSE;
			break;
		}
		// ���s
		fWrite = ::WriteFile(hCom, "\x0d\x0a", 2, &dwWrite, NULL);
		if ( !fWrite || dwWrite != 2 ) {
			AfxMessageBox(IDS_ERR_WRITECOM, MB_OK|MB_ICONSTOP);
			fWrite = FALSE;
			break;
		}
		// ��۸�ڽ�ް
		pParent->m_ctSendProgress.SetPos(i+1);
		Sleep(0);
	}
	// EOF
	if ( fWrite ) {
		::WriteFile(hCom, "\x1a", 1, &dwWrite, NULL);
		::FlushFileBuffers(hCom);
#ifdef _DEBUG
		printf("WriteFile success\n");
#endif
	}
	else {
		DWORD	dwErrorMask;
		COMSTAT	comstat;
		::ClearCommError(hCom, &dwErrorMask, &comstat);
#ifdef _DEBUG
		printf("WriteFile NG\n");
#endif
	}
	::CloseHandle(hCom);

	// ٰ�ߐ���I���Ȃ�
	if ( pParent->m_fThread ) {
		pParent->SendMessage(WM_CLOSE);	// pParent->DestroyWindow() �łͱ����ݴװ
#ifdef _DEBUG
		printf("SendMessage(WM_CLOSE) End\n");
#endif
	}

	return 0;	// AfxEndThread(0);
}
