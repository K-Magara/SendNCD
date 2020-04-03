// SendNCD.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "SendNCD.h"
#include "SendNCDSetup.h"
#include "SendNCDInfo.h"

#include <memory.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendNCDApp

BEGIN_MESSAGE_MAP(CSendNCDApp, CWinApp)
	//{{AFX_MSG_MAP(CSendNCDApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// �B��� CSendNCDApp �I�u�W�F�N�g

CSendNCDApp theApp;
#ifdef _DEBUG
DbgConsole	theDebug;	// ���ޯ�ޗp�ݿ��
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendNCDApp �̍\�z

CSendNCDApp::CSendNCDApp()
{
	m_nComPort = 0;
	m_bAutoRead = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CSendNCDApp �N���X�̏�����

BOOL CSendNCDApp::InitInstance() 
{
#ifdef _DEBUG
	printf("CSendNCDApp::InitInstance() Start\n");
#endif

	// �����ݒ��ڼ޽�؂���擾
	HKEY	hKey;
	CString	strAppName, strEntry;
	DWORD	dwValue, dwEntry, dwType, dwIndex;
	TCHAR	szEntry[_MAX_PATH];
	BYTE	szValue[sizeof(DCB)];
	LONG	lResult = ERROR_SUCCESS;
	ULONG	lSize;
	LPDCB	lpDCB;

	strAppName.LoadString(IDR_APPNAME);
	if ( NCVC_CreateRegKey(strAppName, &hKey) != ERROR_SUCCESS ) {
#ifdef _DEBUG
		printf("NCVC_CreateRegKey() != ERROR_SUCCESS\n");
#endif
		return FALSE;
	}

	// DCB
	try {
		for ( dwIndex=0; lResult!=ERROR_NO_MORE_ITEMS; dwIndex++ ) {
			dwEntry = sizeof(szEntry);
			dwValue = sizeof(szValue);
			lResult = ::RegEnumValue(hKey, dwIndex, szEntry, &dwEntry, NULL,
								&dwType, szValue, &dwValue);
			if ( lResult==ERROR_SUCCESS && dwType==REG_BINARY && dwValue==sizeof(DCB) ) {
				lpDCB = new DCB;
				memcpy(lpDCB, szValue, sizeof(DCB));
				m_mapDCB.SetAt(szEntry, lpDCB);
			}
		}
	}
	catch (CMemoryException* e) {
		e->Delete();
		AfxMessageBox(IDS_ERR_OUTOFMEM, MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	CRegKey	reg;
	reg.Attach(hKey);

	// ComPort
	strEntry.LoadString(IDS_REG_COM);
	lSize = sizeof(DWORD);
	lResult = reg.QueryValue(strEntry, &dwType, &dwValue, &lSize);
	if ( lResult==ERROR_SUCCESS && dwType==REG_DWORD && lSize==sizeof(DWORD) )
		SetComPort( (int)dwValue );
	// AutoRead
	strEntry.LoadString(IDS_REG_AUTOREAD);
	lResult = reg.QueryValue(strEntry, &dwType, &dwValue, &lSize);
	if ( lResult==ERROR_SUCCESS && dwType==REG_DWORD && lSize==sizeof(DWORD) )
		m_bAutoRead = (BOOL)dwValue;

#ifdef _DEBUG
	printf("ComPort=%d\n", GetComPort());
	printf("AutoRead=%d\n", GetAutoRead());
#endif

	reg.Close();

	return CWinApp::InitInstance();
}

int CSendNCDApp::ExitInstance() 
{
	CString	strKey;
	LPDCB	lpDCB;
	for ( POSITION pos = m_mapDCB.GetStartPosition(); pos; ) {
		m_mapDCB.GetNextAssoc(pos, strKey, (LPVOID &)lpDCB);
		delete	lpDCB;
	}

	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CSendNCDApp ���ފ֐�

void CSendNCDApp::GetDCB(LPCTSTR lpszCom, LPDCB lpDCB)
{
	::ZeroMemory(lpDCB, sizeof(DCB));
	CString	strKey;
	strKey.Format(IDS_REG_COMDCB, lpszCom);
	LPDCB	lpmapDCB;
	if ( m_mapDCB.Lookup(strKey, (LPVOID &)lpmapDCB) ) {
		memcpy(lpDCB, lpmapDCB, sizeof(DCB));
		return;
	}

	// ϯ�߂ɐݒ��񂪂Ȃ��ꍇ�͌��ݒl��ǂݍ���
	strKey = lpszCom;
	HANDLE	hCom;
	if ( (hCom=::CreateFile("\\\\.\\"+strKey, GENERIC_WRITE, 0, NULL,
					OPEN_EXISTING, NULL, NULL)) != INVALID_HANDLE_VALUE ) {
		::GetCommState(hCom, lpDCB);
		::CloseHandle(hCom);
	}
}

void CSendNCDApp::SetDCB(LPCTSTR lpszCom, LPDCB lpDCB)
{
	LPDCB	lpmapDCB;
	CString	strAppName, strKey;
	strKey.Format(IDS_REG_COMDCB, lpszCom);
	HKEY	hKey;

	try {
		if ( m_mapDCB.Lookup(strKey, (LPVOID &)lpmapDCB) )
			memcpy(lpmapDCB, lpDCB, sizeof(DCB));
		else {
			lpmapDCB = new DCB;
			memcpy(lpmapDCB, lpDCB, sizeof(DCB));
			m_mapDCB.SetAt(strKey, lpmapDCB);
		}
	}
	catch (CMemoryException* e) {
		e->Delete();
		AfxMessageBox(IDS_ERR_OUTOFMEM, MB_OK|MB_ICONSTOP);
	}

#ifdef _DEBUG
	printf("CSendNCDApp::SetDCB()\n");
	printf("  BaudRate=%d\n", lpmapDCB->BaudRate);
	printf("  ByteSize=%d\n", lpmapDCB->ByteSize);
	printf("  Parity  =%d\n", lpmapDCB->Parity);
	printf("  StopBits=%d\n", lpmapDCB->StopBits);
#endif

	// DCB�ݒ�̕ۑ�
	strAppName.LoadString(IDR_APPNAME);
	if ( NCVC_CreateRegKey(strAppName, &hKey) == ERROR_SUCCESS ) {
		::RegSetValueEx(hKey, strKey, NULL, REG_BINARY, (const LPBYTE)lpmapDCB, sizeof(DCB));
		::RegCloseKey(hKey);
	}
#ifdef _DEBUG
	else 
		printf("NCVC_CreateRegKey() Error\n");
#endif
}

/////////////////////////////////////////////////////////////////////////////
// NCVC ��޲݊֐�

NCADDIN BOOL NCVC_Initialize(NCVCINITIALIZE* nci)
{
	static	LPCTSTR		szFuncName = "SendNCD";

#ifdef _DEBUG
	printf("SendNCD.dll NCVC_Initialize Start!!\n");
#endif

	// ��޲݂̕K�v���
	nci->dwSize = sizeof(NCVCINITIALIZE);
	nci->dwType = NCVCADIN_FLG_NCDFILE;
//	nci->nToolBar = 0;
	nci->lpszMenuName[NCVCADIN_ARY_NCDFILE]	= "�رّ��M...";
	nci->lpszFuncName[NCVCADIN_ARY_NCDFILE]	= szFuncName;
	nci->lpszAddinName	= szFuncName;
	nci->lpszCopyright	= "MNCT-S K.Magara";
	nci->lpszSupport	= "http://s-gikan2.maizuru-ct.ac.jp/";

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// NCVC ����Ċ֐�

NCADDIN void SendNCD(void)
{
#ifdef _DEBUG
	printf("SendNCD.dll SendNCD() Start!!\n");
#endif
	static	CSendNCDInfo*	pDlgSendInfo;

	// ���ݱ�è�ނ��޷��������ق��擾
	NCVCHANDLE	hDoc = NCVC_GetDocument(NULL);
	if ( hDoc == NULL ) {
#ifdef _DEBUG
		printf("NCVC_GetDocument() NULL\n");
#endif
		return;
	}

	// DLL ��ؿ��ID���� �޲�۸ނ�\��
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// �ر��߰Đݒ��޲�۸�
	CSendNCDSetup	dlgSelectComm;
	if ( dlgSelectComm.DoModal() != IDOK )
		return;
	// �ر��߰đ��M��ݾ�Ӱ��ڽ�޲�۸�
	// (�گ�ސ������đ��M)
	pDlgSendInfo = new CSendNCDInfo(hDoc, dlgSelectComm.m_strSendCom);
	pDlgSendInfo->Create(IDD_SENDNCD);
	// pDlgSendInfo �� delete �� CSendNCDInfo::PostNcDestroy()
}
