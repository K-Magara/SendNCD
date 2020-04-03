// SendNCDSetup.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_SENDNCDSETUP_H__215820C0_F598_42D8_A738_E2E0331EDF81__INCLUDED_)
#define AFX_SENDNCDSETUP_H__215820C0_F598_42D8_A738_E2E0331EDF81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSendNCDSetup �_�C�A���O

class CSendNCDSetup : public CDialog
{
// �R���X�g���N�V����
public:
	CSendNCDSetup(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSendNCDSetup)
	enum { IDD = IDD_SENDNCD_SETUP };
	CComboBox	m_ctComPort;
	BOOL	m_bAutoRead;
	int		m_nComPort;
	//}}AFX_DATA
	CString	m_strSendCom;	// �ʐM�߰ĕ�����

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSendNCDSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSendNCDSetup)
	afx_msg void OnSendSetup();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SENDNCDSETUP_H__215820C0_F598_42D8_A738_E2E0331EDF81__INCLUDED_)
