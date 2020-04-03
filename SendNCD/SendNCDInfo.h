// SendNCDInfo.h : �w�b�_�[ �t�@�C��
//
#if !defined(AFX_SENDNCDINFO_H__4BA86C2B_5BC7_45AD_865E_64F158D1CD99__INCLUDED_)
#define AFX_SENDNCDINFO_H__4BA86C2B_5BC7_45AD_865E_64F158D1CD99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// �رّ��M�گ��
UINT SendThread(LPVOID);

/////////////////////////////////////////////////////////////////////////////
// CSendNCDInfo �_�C�A���O

class CSendNCDInfo : public CDialog
{
// �رّ��M�گ�ނ͗F�B
friend UINT SendThread(LPVOID);

	NCVCHANDLE	m_hDoc;		// NCVC�����
	HANDLE		m_hCom;		// �ر��߰�OPEN�����
	CWinThread*	m_pThread;	// �رّ��M�گ���߲��
	BOOL		m_fThread;	// �رّ��M�گ�ތp���׸�

/*
	�{���Ȃ� SendThread() ���璼�� NCVC_GetNCStrData() ���Ăяo��
	NC���ނ��擾���Ȃ��瑗�M����\�肾�������CSendThread() ����
	NCVC_GetNCStrData() ���ĂԂƁC�A�N�Z�X�ᔽ�̃G���[�ɂȂ�̂ŁC
	CSendNCDInfo �׽�\�z���� m_strGcode �֓o�^
	(�����r�����޷���Ă������Ă����邵�l)
*/
	CStringArray	m_strGcode;	// NC����

// �R���X�g���N�V����
public:
	CSendNCDInfo(NCVCHANDLE, LPCTSTR);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSendNCDInfo)
	enum { IDD = IDD_SENDNCD };
	CProgressCtrl	m_ctSendProgress;
	CAnimateCtrl	m_ctSendAvi;
	CString	m_strFileName;
	CString	m_strComPort;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSendNCDInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSendNCDInfo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg LRESULT OnNcHitTest(CPoint);	// �ײ��ė̈�ų���޳����ׯ��
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SENDNCDINFO_H__4BA86C2B_5BC7_45AD_865E_64F158D1CD99__INCLUDED_)
