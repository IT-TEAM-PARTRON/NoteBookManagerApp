#pragma once

#include <mysql.h>
#include <afxwin.h>
// CreatUserDlg dialog

class CreatUserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreatUserDlg)

public:
	CreatUserDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CreatUserDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CREATER };
#endif

protected:
	CBrush m_brushBackground;
	COLORREF m_colorBackground;
	COLORREF m_colorText;

	typedef struct {
		CFont pFontEdit;
		CFont pFontButton;
		CFont pFontText;
	}font_data_t;
	font_data_t font_data;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	BOOL OnInitDialog();

	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	void SetFontInit();

	BOOL ConnectToMariaDB();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioCtadmin();
	afx_msg void OnBnClickedRadioCtuser();
private:
	CString m_strCtuser;
	CString m_strCtPassword;
	CString m_strCtCfpassword;
	CButton m_radioCadmin;
	CButton m_radioCuser;
	MYSQL* conn;
	CString m_strManageraccess;
public:
	afx_msg void OnBnClickedButtonCtsignUp();
};
