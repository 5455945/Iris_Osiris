// OsirisMfcDlg.h : ͷ�ļ�
//

#pragma once

#include <iostream>

// COsirisMfcDlg �Ի���
class COsirisMfcDlg : public CDialog
{
// ����
public:
	COsirisMfcDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_OSIRISMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRecognition();
	afx_msg void OnCancel();
	afx_msg void OnBnClickedBtnSetDataDir();
	afx_msg void OnBnClickedBtnSetOutputDir();

private:
	std::string m_sInputDataDir;
	std::string m_sOutputDataDir;
	
	void SetDefaultDataDir();
	CString GetFolderDir();

};
