// CreatUserDlg.cpp : implementation file
//

#include "pch.h"
#include "NotebookManagerApp.h"
#include "CreatUserDlg.h"
#include "afxdialogex.h"
#include <mysql.h>
#include <iostream>


// CreatUserDlg dialog

IMPLEMENT_DYNAMIC(CreatUserDlg, CDialogEx)

CreatUserDlg::CreatUserDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CREATER, pParent)
	, m_strCtuser(_T(""))
	, m_strCtPassword(_T(""))
	, m_strCtCfpassword(_T(""))
{
	m_colorBackground = RGB(242, 243, 245);
	m_colorText = RGB(0, 0, 0);

	// Tạo bút vẽ với màu nền
	m_brushBackground.CreateSolidBrush(m_colorBackground);
}

CreatUserDlg::~CreatUserDlg()
{
}

void CreatUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CTUSER, m_strCtuser);
	DDX_Text(pDX, IDC_EDIT_CTPASSWORD, m_strCtPassword);
	DDX_Text(pDX, IDC_EDIT_CTCFPASSWORD, m_strCtCfpassword);
	DDX_Control(pDX, IDC_RADIO_CTADMIN, m_radioCadmin);
	DDX_Control(pDX, IDC_RADIO_CTUSER, m_radioCuser);
}


BEGIN_MESSAGE_MAP(CreatUserDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_RADIO_CTADMIN, &CreatUserDlg::OnBnClickedRadioCtadmin)
	ON_BN_CLICKED(IDC_RADIO_CTUSER, &CreatUserDlg::OnBnClickedRadioCtuser)
	ON_BN_CLICKED(IDC_BUTTON_CTSIGN_UP, &CreatUserDlg::OnBnClickedButtonCtsignUp)
END_MESSAGE_MAP()


// CreatUserDlg message handlers
BOOL CreatUserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetFontInit();
	m_radioCuser.SetCheck(BST_CHECKED);
	m_strManageraccess = _T("user");
	return FALSE; // Trả về TRUE trừ khi bạn đặt focus vào điều khiển
}
HBRUSH CreatUserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CreatUserDlg::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

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
		dc.FillSolidRect(&rect, RGB(36, 79, 173)); // Màu nền bình thường
	}

	// Thiết lập màu chữ và vẽ văn bản
	dc.SetTextColor(RGB(255, 255, 255)); // Màu chữ trắng
	dc.SetBkMode(TRANSPARENT);

	CString strText;
	GetDlgItemText(lpDrawItemStruct->CtlID, strText);

	dc.DrawText(strText, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// Vẽ đường viền
	dc.DrawEdge(&rect, (state & ODS_SELECTED) ? EDGE_SUNKEN : EDGE_RAISED, BF_RECT);

	dc.Detach();
}
void CreatUserDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (nIDCtl == IDC_BUTTON_CTSIGN_UP) // Thay IDC_MY_BUTTON bằng ID thực tế của button của bạn
	{
		DrawItem(lpDrawItemStruct);
	}
	else
	{
		CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}
void CreatUserDlg::SetFontInit()
{
	font_data.pFontText.CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Times New Roman"));
	font_data.pFontEdit.CreateFont(17, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	font_data.pFontButton.CreateFont(30, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Times New Roman"));

	GetDlgItem(IDC_STATIC_CTUSER)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_STATIC_CTPASSWORD)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_STATIC_CTCFPASSWORD)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_RADIO_CTADMIN)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_RADIO_CTUSER)->SetFont(&font_data.pFontText);

	GetDlgItem(IDC_BUTTON_CTSIGN_UP)->SetFont(&font_data.pFontButton);

	GetDlgItem(IDC_EDIT_CTUSER)->SetFont(&font_data.pFontEdit);
	GetDlgItem(IDC_EDIT_CTPASSWORD)->SetFont(&font_data.pFontEdit);
	GetDlgItem(IDC_EDIT_CTCFPASSWORD)->SetFont(&font_data.pFontEdit);

}
BOOL CreatUserDlg::ConnectToMariaDB()
{
	conn = mysql_init(NULL);

	// Kết nối đến database
	if (!!mysql_real_connect(conn, ip, uid, pwd, dbname, port, NULL, 0)) {
		AfxMessageBox(_T("Server Connection Fail"));
		return FALSE;
	}
	//mysql_close(conn);
	return TRUE;
}
void CreatUserDlg::OnBnClickedRadioCtadmin()
{
	// TODO: Add your control notification handler code here
	m_radioCuser.SetCheck(BST_UNCHECKED);
	m_radioCadmin.SetCheck(BST_CHECKED);
	m_strManageraccess = _T("admin");
}


void CreatUserDlg::OnBnClickedRadioCtuser()
{
	// TODO: Add your control notification handler code here
	m_radioCuser.SetCheck(BST_CHECKED);
	m_radioCadmin.SetCheck(BST_UNCHECKED);
	m_strManageraccess = _T("user");
}


void CreatUserDlg::OnBnClickedButtonCtsignUp()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_strCtuser == "" || m_strCtPassword == "" || m_strCtCfpassword == "")
	{
		AfxMessageBox(CString("Info Null !"));
	}
	else
	{
		if (m_strCtPassword != m_strCtCfpassword)
		{
			AfxMessageBox(CString("Confirm Password Fail! "));
		}
		else
		{
			if (ConnectToMariaDB() && conn != nullptr)
			{
				CString strSQL;
				strSQL.Format(_T("SELECT * FROM user WHERE user_name = '%s'"), m_strCtuser);
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
							AfxMessageBox(CString("USER NAME EXIT"));
						}
						else
						{
							CString strSQL;
							strSQL.Format(_T("INSERT INTO user VALUES('%s','%s','%s')")
								, m_strCtuser, m_strCtPassword, m_strManageraccess);
							if (mysql_query(conn, CT2A(strSQL)))
							{
								AfxMessageBox(CString("Query execution failed: ") + CString(mysql_error(conn)));
							}
							else
							{
								AfxMessageBox(CString("CREATER USER SUCCESS!"), MB_OK | MB_ICONINFORMATION);
								mysql_free_result(res);
								mysql_close(conn);
								return;
							}
						}

					}
					mysql_free_result(res);
				}
				mysql_close(conn);
			}
		}
	}
	
}
