// OsirisMfcDlg.h : 头文件
//

#pragma once

#include <iostream>

// COsirisMfcDlg 对话框
class COsirisMfcDlg : public CDialog
{
// 构造
public:
	COsirisMfcDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OSIRISMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
