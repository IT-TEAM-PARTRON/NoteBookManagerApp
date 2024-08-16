
// NotebookManagerAppDlg.h : header file
//

#pragma once

#include <afxwin.h>
#include <mysql.h>

// CNotebookManagerAppDlg dialog
class CNotebookManagerAppDlg : public CDialogEx
{
// Construction
public:
	CNotebookManagerAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NOTEBOOKMANAGERAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	CBrush m_brushBackground;
	COLORREF m_colorBackground;
	COLORREF m_colorText;
	
	typedef struct {
		CFont pFontEdit;
		CFont pFontButton;
		CFont pFontText;
	}font_data_t;
	font_data_t font_data;

	// Khai báo các hàm xử lý sự kiện
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void SetFontInit();
	BOOL ConnectToMariaDB();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLogin();
	BOOL PreTranslateMessage(MSG* pMsg);
	CEdit m_editUser;
	CString m_strUser;
	CEdit m_editPassword;
	CString m_strPassword;
	CString  managerAccess;
	MYSQL* conn;
};
