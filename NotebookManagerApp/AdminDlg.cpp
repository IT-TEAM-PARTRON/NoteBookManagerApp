// AdminDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "NotebookManagerApp.h"
#include "AdminDlg.h"
#include "afxdialogex.h"
#include <mysql.h>
#include <iostream>
#include "CreatUserDlg.h"
#include "afxdialogex.h"
#include <winspool.h> 
#include <iostream>
#include <mysql.h>


using namespace std;
#pragma comment(lib, "shlwapi.lib") // shlwapi.lib 라이브러리를 링크합니다.

// AdminDlg dialog
#define WM_TRAYICON (WM_USER + 1)

IMPLEMENT_DYNAMIC(AdminDlg, CDialogEx)

AdminDlg::AdminDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADMIN, pParent)
	, m_strId(_T(""))
	, m_strName(_T(""))
	, m_strModel(_T(""))
	, m_strSn(_T(""))
	, m_strNote(_T(""))
	, m_cdtDateOS(COleDateTime::GetCurrentTime())
	, m_cdtDateOP(COleDateTime::GetCurrentTime())
	, m_strSearch(_T(""))
	, m_strOrigin(_T(""))
	, m_strCountRecord(_T(""))
{
	m_colorBackground = RGB(242, 243, 245);
	m_colorText = RGB(0, 0, 0);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_PARTRON);
	// Tạo bút vẽ với màu nền
	m_brushBackground.CreateSolidBrush(m_colorBackground);
}

AdminDlg::~AdminDlg()
{
	// Remove icon from system tray
	//Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

void AdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_pListData);
	DDX_Control(pDX, IDC_RADIO_WAITING, m_radioWaiting);
	DDX_Control(pDX, IDC_RADIO_USED, m_radioUsed);
	DDX_Control(pDX, IDC_RADIO_KR, m_radioKr);
	DDX_Control(pDX, IDC_RADIO_VN, m_radioVn);
	DDX_Control(pDX, IDC_EDIT_ID, m_editId);
	DDX_Text(pDX, IDC_EDIT_ID, m_strId);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Control(pDX, IDC_EDIT_MODEL, m_editModel);
	DDX_Text(pDX, IDC_EDIT_MODEL, m_strModel);
	DDX_Control(pDX, IDC_EDIT_SN, m_editSn);
	DDX_Text(pDX, IDC_EDIT_SN, m_strSn);
	DDX_Control(pDX, IDC_EDIT_NOTE, m_editNote);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_strNote);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SUPPLY, m_dtpDateOS);
	DDX_Control(pDX, IDC_DATETIMEPICKER_PURCHASE, m_dtpDateOP);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SUPPLY, m_cdtDateOS);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_PURCHASE, m_cdtDateOP);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_strSearch);
	DDX_Control(pDX, IDC_EDIT_ORIGIN, m_editOrigin);
	DDX_Text(pDX, IDC_EDIT_ORIGIN, m_strOrigin);
	DDX_Control(pDX, IDC_EDIT_COUNT_RECORD, m_editCountRecord);
	DDX_Text(pDX, IDC_EDIT_COUNT_RECORD, m_strCountRecord);
}


BEGIN_MESSAGE_MAP(AdminDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_WM_QUERYDRAGICON()
	//ON_MESSAGE(WM_USER + 1, &AdminDlg::OnTrayNotification)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &AdminDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &AdminDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &AdminDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CREAT, &AdminDlg::OnBnClickedButtonCreat)
	ON_BN_CLICKED(IDC_RADIO_WAITING, &AdminDlg::OnBnClickedRadioWaiting)
	ON_BN_CLICKED(IDC_RADIO_USED, &AdminDlg::OnBnClickedRadioUsed)
	ON_BN_CLICKED(IDC_RADIO_KR, &AdminDlg::OnBnClickedRadioKr)
	ON_BN_CLICKED(IDC_RADIO_VN, &AdminDlg::OnBnClickedRadioVn)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DATA, &AdminDlg::OnNMClickListData)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_DATA, &AdminDlg::OnNMCustomdrawListData)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, &AdminDlg::OnEnChangeEditSearch)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &AdminDlg::OnBnClickedButtonSearch)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &AdminDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &AdminDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()

BOOL AdminDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetFontInit();
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_DATA);
	pListCtrl->GetHeaderCtrl()->SetFont(&font_data.pFontListFist);
	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_GRIDLINES);
	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	// Thiết lập số cột cho List Control
	pListCtrl->InsertColumn(0, _T("ID"), LVCFMT_LEFT, 120);
	pListCtrl->InsertColumn(1, _T("NAME"), LVCFMT_LEFT, 220);
	pListCtrl->InsertColumn(2, _T("MODEL"), LVCFMT_LEFT, 130);
	pListCtrl->InsertColumn(3, _T("SN"), LVCFMT_LEFT, 130);
	pListCtrl->InsertColumn(4, _T("ORIGIN"), LVCFMT_LEFT, 100);
	pListCtrl->InsertColumn(5, _T("DATEOFSUPPLY"), LVCFMT_LEFT, 150);
	pListCtrl->InsertColumn(6, _T("DATEOFPURCHASE"), LVCFMT_LEFT, 150);
	pListCtrl->InsertColumn(7, _T("USER"), LVCFMT_LEFT, 100);
	pListCtrl->InsertColumn(8, _T("STATUS"), LVCFMT_LEFT, 100);
	pListCtrl->InsertColumn(9, _T("NOTE"), LVCFMT_LEFT, 100);

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//ShowTrayIcon();

	// Thiết lập các điều khiển khác nếu cần thiết
	
	
	if (str_global_manageraccess == "user")
	{
		GetDlgItem(IDC_BUTTON_ADD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_UPDATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_DELETE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_CREAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_CLEAR)->ShowWindow(SW_HIDE);
	}
	
	
	m_radioUsed.SetCheck(BST_CHECKED);
	m_strStatus = _T("USED");
	m_radioVn.SetCheck(BST_CHECKED);
	m_strUser = _T("VN");
	ConnectToMariaDB();
	PopulateListCtrl();

	if (pListCtrl->GetItemCount() > 0)
	{
		// Chọn mục đầu tiên
		pListCtrl->SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

		// Đảm bảo rằng mục đầu tiên được hiển thị
		pListCtrl->EnsureVisible(0, FALSE);
	}
	return FALSE; // Trả về TRUE trừ khi bạn đặt focus vào điều khiển
}

void AdminDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//void AdminDlg::ShowTrayIcon()
//{
//	m_nid.cbSize = sizeof(NOTIFYICONDATA);
//	m_nid.hWnd = this->m_hWnd;
//	m_nid.uID = IDI_ICON_PARTRON;
//	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
//	m_nid.uCallbackMessage = WM_TRAYICON;
//	m_nid.hIcon = AfxGetApp()->LoadIcon(IDI_ICON_PARTRON);
//	_tcscpy_s(m_nid.szTip, _T("Notebook Application"));
//
//	Shell_NotifyIcon(NIM_ADD, &m_nid);
//}

//LRESULT AdminDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
//{
//	if (lParam == WM_LBUTTONDOWN || lParam == WM_RBUTTONDOWN)
//	{
//		// Hiển thị cửa sổ ứng dụng khi nhấp vào biểu tượng
//		ShowWindow(SW_RESTORE);
//		SetForegroundWindow();
//	}
//	return 0;
//}

HCURSOR AdminDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
HBRUSH AdminDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// Thiết lập màu nền và màu chữ cho các điều khiển
	if (nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetBkColor(m_colorBackground);
		return m_brushBackground;
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(m_colorBackground);
		pDC->SetTextColor(m_colorText);
		return m_brushBackground;
	}

	return hbr;
}
void AdminDlg::DrawButton(CDC& dc, LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF bgColor, COLORREF textColor)
{
	// Lấy trạng thái của button
	UINT state = lpDrawItemStruct->itemState;
	CRect rect = lpDrawItemStruct->rcItem;

	// Vẽ nền
	if (state & ODS_SELECTED)
	{
		dc.FillSolidRect(&rect, RGB(101, 114, 143)); // Màu nền khi được chọn
	}
	else
	{
		dc.FillSolidRect(&rect, bgColor); // Màu nền bình thường
	}

	// Thiết lập màu chữ và vẽ văn bản
	dc.SetTextColor(textColor); // Màu chữ
	dc.SetBkMode(TRANSPARENT);

	CString strText;
	GetDlgItemText(lpDrawItemStruct->CtlID, strText);

	dc.DrawText(strText, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// Vẽ đường viền
	dc.DrawEdge(&rect, (state & ODS_SELECTED) ? EDGE_SUNKEN : EDGE_RAISED, BF_RECT);
}
void AdminDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	switch (nIDCtl)
	{
	case IDC_BUTTON_ADD:
		DrawButton(*CDC::FromHandle(lpDrawItemStruct->hDC), lpDrawItemStruct, RGB(36, 79, 173), RGB(255, 255, 255));
		break;
	case IDC_BUTTON_UPDATE:
		DrawButton(*CDC::FromHandle(lpDrawItemStruct->hDC), lpDrawItemStruct, RGB(36, 173, 36), RGB(255, 255, 255));
		break;
	case IDC_BUTTON_DELETE:
		DrawButton(*CDC::FromHandle(lpDrawItemStruct->hDC), lpDrawItemStruct, RGB(173, 36, 36), RGB(255, 255, 255));
		break;
	case IDC_BUTTON_CREAT:
		DrawButton(*CDC::FromHandle(lpDrawItemStruct->hDC), lpDrawItemStruct, RGB(242, 189, 119), RGB(255, 255, 255));
		break;
	case IDC_BUTTON_CLEAR:
		DrawButton(*CDC::FromHandle(lpDrawItemStruct->hDC), lpDrawItemStruct, RGB(191, 180, 163), RGB(255, 255, 255));
		break;
	default:
		CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
		break;
	}
}
void AdminDlg::OnNMCustomdrawListData(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	switch (pNMCD->dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;

	case CDDS_ITEMPREPAINT:
		// Check if it is the header item
		if (pNMCD->dwItemSpec == 0) // This checks if it is the first row
		{
			if (pNMCD->dwItemSpec == 0) // Column 1
			{
				//pNMCD->clrText = RGB(255, 0, 0); // Text color
				//pNMCD->clrTextBk = RGB(255, 255, 255); // Background color
				//SelectObject(pNMCD->hdc, font_data.pFontListFist); // Set font for column 1
			}
			else if (pNMCD->dwItemSpec == 1) // Column 2
			{
				//pNMCD->clrText = RGB(0, 0, 255); // Text color
				//pNMCD->clrTextBk = RGB(255, 255, 255); // Background color
				//SelectObject(pNMCD->hdc, font_data.pFontListFist); // Set font for column 2
			}
		}
		*pResult = CDRF_NEWFONT;
		return;
	}

	*pResult = 0;
}
void AdminDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	exit(1);
	CDialogEx::OnClose();
}
// AdminDlg message handlers
void AdminDlg::SetFontInit()
{
	font_data.pFontText.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Times New Roman"));
	font_data.pFontEdit.CreateFont(18, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	font_data.pFontButton.CreateFont(28, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Times New Roman"));
	font_data.pFontList.CreateFont(17, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	font_data.pFontListFist.CreateFont(19, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Times New Roman"));

	GetDlgItem(IDC_STATIC_ID)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_STATIC_NAME)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_STATIC_MODEL)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_STATIC_SN)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_STATIC_NOTE)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_STATIC_DATEOFSUPPLY)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_STATIC_DATEOFPURCHASE)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_STATIC_STATUS)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_STATIC_USER)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_STATIC_ORIGIN)->SetFont(&font_data.pFontText);

	GetDlgItem(IDC_BUTTON_ADD)->SetFont(&font_data.pFontButton);
	GetDlgItem(IDC_BUTTON_UPDATE)->SetFont(&font_data.pFontButton);
	GetDlgItem(IDC_BUTTON_DELETE)->SetFont(&font_data.pFontButton);
	GetDlgItem(IDC_BUTTON_CREAT)->SetFont(&font_data.pFontButton);
	GetDlgItem(IDC_BUTTON_CLEAR)->SetFont(&font_data.pFontButton);

	GetDlgItem(IDC_EDIT_ID)->SetFont(&font_data.pFontEdit);
	GetDlgItem(IDC_EDIT_NAME)->SetFont(&font_data.pFontEdit);
	GetDlgItem(IDC_EDIT_MODEL)->SetFont(&font_data.pFontEdit);
	GetDlgItem(IDC_EDIT_SN)->SetFont(&font_data.pFontEdit);
	GetDlgItem(IDC_EDIT_NOTE)->SetFont(&font_data.pFontEdit);
	GetDlgItem(IDC_EDIT_SEARCH)->SetFont(&font_data.pFontEdit);
	GetDlgItem(IDC_EDIT_ORIGIN)->SetFont(&font_data.pFontEdit);
	GetDlgItem(IDC_EDIT_COUNT_RECORD)->SetFont(&font_data.pFontEdit);

	GetDlgItem(IDC_LIST_DATA)->SetFont(&font_data.pFontList);
}
void AdminDlg::SetDataInit()
{
	m_strId = "";
	m_strName = "";
	m_strModel = "";
	m_strSn = "";
	m_strNote = "";
	m_strSearch = "";
	m_strOrigin = "";
	m_strCountRecord = "";

	CString strDateOfSupply  = COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d");
	m_cdtDateOS.ParseDateTime(strDateOfSupply, VAR_DATEVALUEONLY);
	CString strDateOfPurchase = COleDateTime::GetCurrentTime().Format(L"%Y-%m-%d");
	m_cdtDateOP.ParseDateTime(strDateOfPurchase, VAR_DATEVALUEONLY);
	m_radioUsed.SetCheck(BST_CHECKED);
	m_radioWaiting.SetCheck(BST_UNCHECKED);
	m_strStatus = _T("USED");
	m_radioVn.SetCheck(BST_CHECKED);
	m_radioKr.SetCheck(BST_UNCHECKED);
	m_strUser = _T("VN");
	UpdateData(FALSE);
}
BOOL AdminDlg::ConnectToMariaDB()
{
	conn = mysql_init(NULL);

	// Kết nối đến database
	if (!mysql_real_connect(conn, ip, uid, pwd, dbname, port, NULL, 0)) {
		AfxMessageBox(_T("Server Connection Fail"));
		return FALSE;
	}
	//mysql_close(conn);
	return TRUE;
}
void AdminDlg::PopulateListCtrl()
{
	// Kết nối đến cơ sở dữ liệu và thực hiện truy vấn

	if (ConnectToMariaDB() && conn != nullptr)
	{
		CString strSQL = _T("SELECT * FROM notebook_managerment");
		if (mysql_query(conn, CT2A(strSQL)))
		{
			AfxMessageBox(CString("Query execution failed: ") + CString(mysql_error(conn)));
		}

		res = mysql_store_result(conn);
		if (res == NULL)
		{
			AfxMessageBox(CString("Error storing result: ") + CString(mysql_error(conn)));
		}

		CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_DATA);
		pListCtrl->DeleteAllItems();

		int nItem = 0;
		while ((row = mysql_fetch_row(res)) != NULL)
		{
			// Thêm hàng mới vào List Control
			pListCtrl->InsertItem(nItem, CString(row[0]));
			pListCtrl->SetItemText(nItem, 1, CString(row[1]));
			pListCtrl->SetItemText(nItem, 2, CString(row[2]));
			pListCtrl->SetItemText(nItem, 3, CString(row[3]));
			pListCtrl->SetItemText(nItem, 4, CString(row[4]));
			pListCtrl->SetItemText(nItem, 5, CString(row[5]));
			pListCtrl->SetItemText(nItem, 6, CString(row[6]));
			pListCtrl->SetItemText(nItem, 7, CString(row[7]));
			pListCtrl->SetItemText(nItem, 8, CString(row[8]));
			pListCtrl->SetItemText(nItem, 9, CString(row[9]));

			m_pListData.SetItemState(-1, 0, LVIS_SELECTED);
			m_pListData.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
			m_pListData.EnsureVisible(nItem, FALSE);
			nItem++;
		}

		mysql_free_result(res);
		mysql_close(conn);

		m_strCountRecord.Format(_T("%d"), m_pListData.GetItemCount());
		UpdateData(FALSE);

		pListCtrl->Invalidate();
		pListCtrl->UpdateWindow();
	}
}

void AdminDlg::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strModel == "" || m_strSn == "" || m_strId == "" || m_strOrigin == "")
	{
		AfxMessageBox(CString("Check Information : ' ID, Model, Sn, Origin ' Not Null "));
	}
	else
	{
		if (ConnectToMariaDB() && conn != nullptr)
		{
			CString strDateOS = m_cdtDateOS.Format(L"%Y-%m-%d");
			CString strDateOP = m_cdtDateOP.Format(L"%Y-%m-%d");
			CString strSQL;
			strSQL.Format(_T("INSERT INTO notebook_managerment VALUES('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')")
				, m_strId, m_strName, m_strModel, m_strSn, m_strOrigin, strDateOS, strDateOP, m_strUser, m_strStatus, m_strNote);
			if (mysql_query(conn, CT2A(strSQL)))
			{
				AfxMessageBox(CString("Query execution failed: ") + CString(mysql_error(conn)));
			}
			else
			{
				AfxMessageBox(CString("ADD DATA SUCCESS!"), MB_OK | MB_ICONINFORMATION);
				PopulateListCtrl();
				SetDataInit();
				return;
			}
			mysql_close(conn);
		}
	}
}

void AdminDlg::OnBnClickedButtonUpdate()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strModel == "" || m_strSn == "" || m_strId == "" || m_strOrigin == "")
	{
		AfxMessageBox(CString("Check Information : ' ID, Model, Sn, Origin ' Not Null "));
	}
	else
	{

		CString strDateOS = m_cdtDateOS.Format(L"%Y-%m-%d");
		CString strDateOP = m_cdtDateOP.Format(L"%Y-%m-%d");

		if (ConnectToMariaDB() && conn != nullptr)
		{
			CString strSQL;
			strSQL.Format(_T("SELECT * FROM notebook_managerment WHERE ID = '%s'"), m_strId);
			if (mysql_query(conn, CT2A(strSQL)))
			{
				AfxMessageBox(CString("Error storing result: ") + CString(mysql_error(conn)));
			}
			else
			{
				MYSQL_RES* res = mysql_store_result(conn); //luu ket qua truy van SELECT
				if (res == NULL)
				{
					AfxMessageBox(CString("Error storing result: ") + CString(mysql_error(conn)));
				}
				else
				{
					MYSQL_ROW row;
					if ((row = mysql_fetch_row(res)) != NULL)
					{
						CString strSqlUpdate;
						strSqlUpdate.Format(_T("UPDATE notebook_managerment SET NAME = '%s', MODEL = '%s', SN ='%s', ORIGIN ='%s', DATEOFSUPPLY = '%s', DATEOFPURCHASE = '%s', USER = '%s', STATUS = '%s' , NOTE = '%s' WHERE ID = '%s'")
							, m_strName, m_strModel, m_strSn, m_strOrigin, strDateOS, strDateOP, m_strUser, m_strStatus, m_strNote, m_strId);
						if (mysql_query(conn, CT2A(strSqlUpdate)))
						{
							AfxMessageBox(CString("Query execution failed: ") + CString(mysql_error(conn)));
						}
						else
						{
							AfxMessageBox(CString("UPDATE DATA SUCCESS!"), MB_OK | MB_ICONINFORMATION);
							PopulateListCtrl();
							SetDataInit();
							mysql_free_result(res);
							return;
						}
					}
					else
					{
						AfxMessageBox(CString("No Data ID Found To Update"));
					}

				}
				mysql_free_result(res);
			}
			mysql_close(conn);
		}
	}
}


void AdminDlg::OnBnClickedButtonDelete()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strModel == "" || m_strSn == "" || m_strId == "" || m_strOrigin == "")
	{
		AfxMessageBox(CString("Check Information : ' ID, Model, Sn, Origin ' Not Null "));
	}
	else
	{
		if (ConnectToMariaDB() && conn != nullptr)
		{
			CString strSQL;
			strSQL.Format(_T("SELECT ID FROM notebook_managerment WHERE ID = '%s'"), m_strId);
			if (mysql_query(conn, CT2A(strSQL)))
			{
				AfxMessageBox(CString("Error storing result: ") + CString(mysql_error(conn)));
			}
			else
			{
				MYSQL_RES* res = mysql_store_result(conn); //luu ket qua truy van SELECT
				if (res == NULL)
				{
					AfxMessageBox(CString("Error storing result: ") + CString(mysql_error(conn)));
				}
				else
				{
					MYSQL_ROW row;
					if ((row = mysql_fetch_row(res)) != NULL)
					{
						CString strSqlDelete;
						strSqlDelete.Format(_T("DELETE FROM notebook_managerment WHERE ID = '%s'"), m_strId);
						if (mysql_query(conn, CT2A(strSqlDelete)))
						{
							AfxMessageBox(CString("Query execution failed: ") + CString(mysql_error(conn)));
						}
						else
						{
							AfxMessageBox(CString("DELETE DATA SUCCESS!"), MB_OK | MB_ICONINFORMATION);
							PopulateListCtrl();
							SetDataInit();
							mysql_free_result(res);
							return;
						}
					}
					else
					{
						AfxMessageBox(CString("No Data ID "));
					}

				}
				mysql_free_result(res);
			}
			mysql_close(conn);
		}
	}
	}
	
void AdminDlg::OnBnClickedButtonCreat()
{
	// TODO: Add your control notification handler code here
	CreatUserDlg dlgNew;
	dlgNew.DoModal();
}


void AdminDlg::OnBnClickedRadioWaiting()
{
	// TODO: Add your control notification handler code here
	m_strStatus = _T("NOT USED");
}
void AdminDlg::OnBnClickedRadioUsed()
{
	// TODO: Add your control notification handler code here
	m_strStatus = _T("USED");
}
void AdminDlg::OnBnClickedRadioKr()
{
	// TODO: Add your control notification handler code here
	m_strUser = _T("KR");
}
void AdminDlg::OnBnClickedRadioVn()
{
	// TODO: Add your control notification handler code here
	m_strUser = _T("VN");
}


void AdminDlg::OnNMClickListData(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nItem = pNMItemActivate->iItem;

	if (nItem >= 0) // Kiểm tra chỉ mục hợp lệ
	{
		// Lấy giá trị của các cột trong hàng được chọn
		m_strId = m_pListData.GetItemText(nItem, 0); // Giá trị của cột 1
		m_strName = m_pListData.GetItemText(nItem, 1); // Giá trị của cột 2
		m_strModel = m_pListData.GetItemText(nItem, 2);
		m_strSn = m_pListData.GetItemText(nItem, 3);
		m_strOrigin = m_pListData.GetItemText(nItem, 4);

		CString strDateOfSupply = m_pListData.GetItemText(nItem, 5);
		m_cdtDateOS.ParseDateTime(strDateOfSupply, VAR_DATEVALUEONLY);
		CString strDateOfPurchase = m_pListData.GetItemText(nItem, 6);
		m_cdtDateOP.ParseDateTime(strDateOfPurchase, VAR_DATEVALUEONLY);


		CString strUser = m_pListData.GetItemText(nItem, 7);
		if (strUser == "KR")
		{
			m_radioKr.SetCheck(BST_CHECKED);
			m_radioVn.SetCheck(BST_UNCHECKED);
		}
		else if (strUser == "VN")
		{
			m_radioVn.SetCheck(BST_CHECKED);
			m_radioKr.SetCheck(BST_UNCHECKED);
		}


		CString strStatus = m_pListData.GetItemText(nItem, 8);
		if (strStatus == "NOT USED")
		{
			m_radioWaiting.SetCheck(BST_CHECKED);
			m_radioUsed.SetCheck(BST_UNCHECKED);
		}
		else if (strStatus == "USED")
		{
			m_radioUsed.SetCheck(BST_CHECKED);
			m_radioWaiting.SetCheck(BST_UNCHECKED);
		}

		m_strNote = m_pListData.GetItemText(nItem, 9);
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void AdminDlg::OnBnClickedButtonSearch()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strSearch == "")
	{
		PopulateListCtrl();
	}
	else
	{
		SearchAndDisplayInListCtrl(m_strSearch);
		m_strCountRecord.Format(_T("%d"), m_pListData.GetItemCount());
		UpdateData(FALSE);
	}
}

void AdminDlg::OnEnChangeEditSearch()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strSearch == "")
	{
		PopulateListCtrl();
	}
	else
	{
		SearchAndDisplayInListCtrl(m_strSearch);
		m_strCountRecord.Format(_T("%d"), m_pListData.GetItemCount());
		UpdateData(FALSE);
	}
}
CString ToLower(const CString& str)
{
	CString lowerStr = str;
	lowerStr.MakeLower();
	return lowerStr;
}

void AdminDlg::StoreAllItems()
{
	PopulateListCtrl();
	int itemCount = m_pListData.GetItemCount();
	int colCount = m_pListData.GetHeaderCtrl()->GetItemCount();
	m_AllItems.clear();

	for (int i = 0; i < itemCount; ++i)
	{
		std::vector<CString> rowData;
		for (int j = 0; j < colCount; ++j)
		{
			rowData.push_back(m_pListData.GetItemText(i, j));
		}
		m_AllItems.push_back(rowData);
	}
}
void AdminDlg::AddItemsToListCtrl(const std::vector<std::vector<CString>>& items)
{
	for (const auto& row : items)
	{
		int nItem = m_pListData.InsertItem(m_pListData.GetItemCount(), row[0]);
		for (size_t i = 1; i < row.size(); ++i)
		{
			m_pListData.SetItemText(nItem, i, row[i]);
		}
		
	}
}
void AdminDlg::SearchAndDisplayInListCtrl(const CString& searchText)
{
	StoreAllItems(); // Lưu trữ tất cả các mục ban đầu
	m_pListData.DeleteAllItems(); // Xóa tất cả các mục hiện tại

	CString lowerSearchText = ToLower(searchText);

	std::vector<std::vector<CString>> foundItems;

	for (const auto& row : m_AllItems)
	{
		bool found = false;
		for (const auto& col : row)
		{
			if (ToLower(col).Find(lowerSearchText) != -1)
			{
				foundItems.push_back(row);
				found = true;
				break;
			}
		}
	}

	AddItemsToListCtrl(foundItems); // Thêm lại các mục tìm kiếm được
}
inline CString GetExecutablePath() {
	WCHAR buffer[MAX_PATH];
	DWORD result = GetModuleFileName(NULL, buffer, MAX_PATH);

	if (result == 0) {
		// Xử lý lỗi nếu cần thiết
		return _T("");
	}

	std::wstring path(buffer);
	std::wstring::size_type pos = path.find_last_of(L"\\/");
	if (pos != std::wstring::npos) {
		return CString(path.substr(0, pos).c_str());
	}
	return CString(path.c_str());
}
// xuat file csv tu control list
void ExportListCtrlToCSV(CListCtrl& listCtrl, const CString& filePath)
{
	// Mở file để ghi
	CStdioFile file;
	if (!file.Open(filePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		AfxMessageBox(_T("Unable to open file for writing."));
		return;
	}

	// Ghi tiêu đề cột vào file CSV
	CString strLine;
	int nColumnCount = listCtrl.GetHeaderCtrl()->GetItemCount();
	LVCOLUMN lvColumn;
	lvColumn.mask = LVCF_TEXT;
	TCHAR szText[256];
	lvColumn.pszText = szText;
	lvColumn.cchTextMax = 256;

	for (int i = 0; i < nColumnCount; i++)
	{
		listCtrl.GetColumn(i, &lvColumn);
		strLine += lvColumn.pszText;
		if (i < nColumnCount - 1)
		{
			strLine += _T(",");
		}
	}
	strLine += _T("\n");
	file.WriteString(strLine);

	// Ghi dữ liệu hàng vào file CSV
	int nItemCount = listCtrl.GetItemCount();
	for (int i = 0; i < nItemCount; i++)
	{
		strLine.Empty();
		for (int j = 0; j < nColumnCount; j++)
		{
			strLine += listCtrl.GetItemText(i, j);
			if (j < nColumnCount - 1)
			{
				strLine += _T(",");
			}
		}
		strLine += _T("\n");
		file.WriteString(strLine);
	}

	file.Close();
	AfxMessageBox(_T("Data successfully exported to file."),MB_OK | MB_ICONINFORMATION);
	
}

void AdminDlg::OnBnClickedButtonExport()
{
	// TODO: Add your control notification handler code here
	CTime currentTime = CTime::GetCurrentTime();
	CString strTime = currentTime.Format(_T("%Y%m%d_%H%M%S"));

	CString executablePath = GetExecutablePath();
	CString DataFile = executablePath + L"\\NOTEBOOK_LIST_" + strTime + L".csv";

	ExportListCtrlToCSV(m_pListData, DataFile);
}


void AdminDlg::OnBnClickedButtonClear()
{
	// TODO: Add your control notification handler code here
	SetDataInit();
	OnBnClickedButtonSearch();
}
