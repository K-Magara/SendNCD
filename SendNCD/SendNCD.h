// SendNCD.h : SENDNCD �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_SENDNCD_H__1D13A3CA_11A9_4714_AB0B_D6CD5AEAE8BF__INCLUDED_)
#define AFX_SENDNCD_H__1D13A3CA_11A9_4714_AB0B_D6CD5AEAE8BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��
#include "NCVCaddin.h"

/////////////////////////////////////////////////////////////////////////////
// CSendNCDApp
// ���̃N���X�̓���̒�`�Ɋւ��Ă� SendNCD.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CSendNCDApp : public CWinApp
{
	int		m_nComPort;		// �ʐM�߰�(��ڼ޽�ؓo�^����)
	BOOL	m_bAutoRead;	// �����}�����ꂽ�����ۂ�ϸۂ����M����
	CMapStringToPtr	m_mapDCB;	// �߰ĕ�����𷰂ɼر��߰Đݒ���(DCB)�i�[

public:
	CSendNCDApp();

public:
	int		GetComPort(void) {
		return m_nComPort;
	}
	void	SetComPort(int a) {
		m_nComPort = a>=0 ? a : 0;
	}
	void	GetDCB(LPCTSTR, LPDCB);
	void	SetDCB(LPCTSTR, LPDCB);

	BOOL	GetAutoRead(void) {
		return m_bAutoRead;
	}
	void	SetAutoRead(BOOL bAutoRead) {
		m_bAutoRead = bAutoRead;
	}

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSendNCDApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSendNCDApp)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SENDNCD_H__1D13A3CA_11A9_4714_AB0B_D6CD5AEAE8BF__INCLUDED_)
