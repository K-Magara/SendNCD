// SendNCDSetup.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "SendNCD.h"
#include "SendNCDSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CSendNCDSetup, CDialog)
	//{{AFX_MSG_MAP(CSendNCDSetup)
	ON_BN_CLICKED(IDC_SEND_SETUP, OnSendSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendNCDSetup �_�C�A���O

CSendNCDSetup::CSendNCDSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CSendNCDSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendNCDSetup)
	//}}AFX_DATA_INIT
	m_nComPort = AfxGetSendNCDApp()->GetComPort();
	m_bAutoRead = AfxGetSendNCDApp()->GetAutoRead();
}

void CSendNCDSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendNCDSetup)
	DDX_Control(pDX, IDC_SEND_COM, m_ctComPort);
	DDX_Check(pDX, IDC_SEND_AUTOREAD, m_bAutoRead);
	DDX_CBIndex(pDX, IDC_SEND_COM, m_nComPort);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSendNCDSetup ���b�Z�[�W �n���h��

BOOL CSendNCDSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// �g�p�ł���ر����޲���ڼ޽�؂���擾
	HKEY	hKey;
	if ( ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM",
				0, KEY_READ, &hKey) != ERROR_SUCCESS ) {
		AfxMessageBox(IDS_ERR_NOSERIAL, MB_OK|MB_ICONSTOP);
		EndDialog(IDCANCEL);
		return FALSE;
	}
	TCHAR	szEntry[_MAX_PATH];
	BYTE	szValue[_MAX_PATH];
	DWORD	dwIndex, dwEntry, dwValue, dwType;
	LONG	lResult = ERROR_SUCCESS;
	for ( dwIndex=0; lResult!=ERROR_NO_MORE_ITEMS; dwIndex++ ) {
		dwEntry = sizeof(szEntry);
		dwValue = sizeof(szValue);
		lResult = ::RegEnumValue(hKey, dwIndex, szEntry, &dwEntry, NULL,
							&dwType, szValue, &dwValue);
		if ( lResult==ERROR_SUCCESS && dwType==REG_SZ )
			m_ctComPort.AddString((LPCTSTR)szValue);
	}
	::RegCloseKey(hKey);
	if ( m_ctComPort.GetCount() <= 0 ) {
		AfxMessageBox(IDS_ERR_NOSERIAL, MB_OK|MB_ICONSTOP);
		EndDialog(IDCANCEL);
		return FALSE;
	}
	m_ctComPort.SetCurSel(m_nComPort);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CSendNCDSetup::OnSendSetup() 
{
	UpdateData();

	m_ctComPort.GetLBText(m_nComPort, m_strSendCom);
#ifdef _DEBUG
	printf("CSendNCDSetup::OnSendSetup() CommPort=%s\n", LPCTSTR(m_strSendCom));
#endif

	COMMCONFIG	cc;
	::ZeroMemory(&cc, sizeof(COMMCONFIG));
	cc.dwSize	= sizeof(COMMCONFIG);
/*
	cc.wVersion	= 1;
	cc.wVersion	= WINDOWS95_COMMCONFIG_VERSION;
*/
	// �ر���ײ�ސݒ��޲�۸ތĂяo��
	AfxGetSendNCDApp()->GetDCB(m_strSendCom, &(cc.dcb));
	if ( ::CommConfigDialog(m_strSendCom, m_hWnd, &cc) )
		AfxGetSendNCDApp()->SetDCB(m_strSendCom, &(cc.dcb));
}

void CSendNCDSetup::OnOK() 
{
	UpdateData();

	m_ctComPort.GetLBText(m_nComPort, m_strSendCom);
	AfxGetSendNCDApp()->SetComPort(m_nComPort);
	AfxGetSendNCDApp()->SetAutoRead(m_bAutoRead);

	// ComPort, AutoRead �̕ۑ�
	CString	strAppName;
	strAppName.LoadString(IDR_APPNAME);
	HKEY	hKey;
	if ( NCVC_CreateRegKey(strAppName, &hKey) == ERROR_SUCCESS ) {
		CString	strEntry;
		strEntry.LoadString(IDS_REG_COM);
		::RegSetValueEx(hKey, strEntry, NULL, REG_DWORD, (const LPBYTE)(&m_nComPort), sizeof(int));
		strEntry.LoadString(IDS_REG_AUTOREAD);
		::RegSetValueEx(hKey, strEntry, NULL, REG_DWORD, (const LPBYTE)(&m_bAutoRead), sizeof(BOOL));
		::RegCloseKey(hKey);
	}
#ifdef _DEBUG
	else
		printf("CSendNCDSetup::OnOK() NCVC_CreateRegKey() Error\n");
#endif

	CDialog::OnOK();
}
