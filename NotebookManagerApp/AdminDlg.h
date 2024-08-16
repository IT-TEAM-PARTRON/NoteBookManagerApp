#pragma once


// AdminDlg dialog
#include <winspool.h>
#include <afxwin.h>
#include <mysql.h>
#include "NotebookManagerApp.h"
#include "NotebookManagerAppDlg.h"
#include <vector>

class AdminDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AdminDlg)

public:
	AdminDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AdminDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADMIN };
#endif

protected:
	HICON m_hIcon;
	NOTIFYICONDATA m_nid; // Biến cho icon trên taskbar


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CBrush m_brushBackground;
	COLORREF m_colorBackground;
	COLORREF m_colorText;

	typedef struct {
		CFont pFontEdit;
		CFont pFontButton;
		CFont pFontText;
		CFont pFontList;
		CFont pFontListFist;
	}font_data_t;
	font_data_t font_data;

	BOOL OnInitDialog();

	void OnPaint();

	//void ShowTrayIcon();

	//LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);

	HCURSOR OnQueryDragIcon();

	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void DrawButton(CDC& dc, LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF bgColor, COLORREF textColor);

	void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	void SetFontInit();

	void SetDataInit();

	BOOL ConnectToMariaDB();

	void PopulateListCtrl();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonCreat();
	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;
	CListCtrl m_pListData;
	CButton m_radioWaiting;
	CButton m_radioUsed;
	CButton m_radioKr;
	CButton m_radioVn;
	CEdit m_editId;
	CString m_strId;
	CEdit m_editName;
	CString m_strName;
	CEdit m_editModel;
	CString m_strModel;
	CEdit m_editSn;
	CString m_strSn;
	CEdit m_editNote;
	CString m_strNote;
	CDateTimeCtrl m_dtpDateOS;
	CDateTimeCtrl m_dtpDateOP;
	COleDateTime m_cdtDateOS;
	COleDateTime m_cdtDateOP;
	CString m_strStatus;
	CString m_strUser;
	afx_msg void OnBnClickedRadioWaiting();
	afx_msg void OnBnClickedRadioUsed();
	afx_msg void OnBnClickedRadioKr();
	afx_msg void OnBnClickedRadioVn();
	afx_msg void OnNMClickListData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawListData(NMHDR* pNMHDR, LRESULT* pResult);
	CString m_strSearch;
	afx_msg void OnEnChangeEditSearch();

	void StoreAllItems();

	void AddItemsToListCtrl(const std::vector<std::vector<CString>>& items);

	void SearchAndDisplayInListCtrl(const CString& searchText);

	std::vector<std::vector<CString>> m_AllItems;
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonClear();
	CEdit m_editOrigin;
	CString m_strOrigin;
	CEdit m_editCountRecord;
	CString m_strCountRecord;
};
