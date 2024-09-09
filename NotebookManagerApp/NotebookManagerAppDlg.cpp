
// NotebookManagerAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "NotebookManagerApp.h"
#include "NotebookManagerAppDlg.h"
#include "afxdialogex.h"
#include <mysql.h>
#include <iostream>
#include "ConfigManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "AdminDlg.h"


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNotebookManagerAppDlg dialog



CNotebookManagerAppDlg::CNotebookManagerAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NOTEBOOKMANAGERAPP_DIALOG, pParent)
	, m_strUser(_T(""))
	, m_strPassword(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_PARTRON);

	m_colorBackground = RGB(242, 243, 245);
	m_colorText = RGB(0, 0, 0);

	// Tạo bút vẽ với màu nền
	m_brushBackground.CreateSolidBrush(m_colorBackground);
}

void CNotebookManagerAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_USER, m_editUser);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
}

BEGIN_MESSAGE_MAP(CNotebookManagerAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CNotebookManagerAppDlg::OnBnClickedButtonLogin)
END_MESSAGE_MAP()


// CNotebookManagerAppDlg message handlers

BOOL CNotebookManagerAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HANDLE hEvent;
	hEvent = CreateEvent(NULL, FALSE, TRUE, AfxGetAppName());
	if (GetLastError() == ERROR_ALREADY_EXISTS)	//중복 실행 방지
	{
		AfxMessageBox(_T("This Application is already running or Endig"), MB_ICONSTOP);
		exit(1);
	}
	SetFontInit();
	GetDlgItem(IDC_EDIT_USER)->SetFocus();
	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CNotebookManagerAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();

	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNotebookManagerAppDlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNotebookManagerAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
HBRUSH CNotebookManagerAppDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// Thiết lập màu nền và màu chữ cho các điều khiển
	if (nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetBkColor(m_colorBackground);
		return m_brushBackground;
	}
	else if (nCtlColor == CTLCOLOR_STATIC )
	{
		pDC->SetBkColor(m_colorBackground);
		pDC->SetTextColor(m_colorText);
		return m_brushBackground;
	}

	return hbr;
}
void CNotebookManagerAppDlg::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
void CNotebookManagerAppDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (nIDCtl == IDC_BUTTON_LOGIN) // Thay IDC_MY_BUTTON bằng ID thực tế của button của bạn
	{
		DrawItem(lpDrawItemStruct);
	}
	else
	{
		CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}
void CNotebookManagerAppDlg::SetFontInit()
{
	font_data.pFontText.CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Times New Roman"));
	font_data.pFontEdit.CreateFont(17, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	font_data.pFontButton.CreateFont(30, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Times New Roman"));

	GetDlgItem(IDC_STATIC_USER_NAME)->SetFont(&font_data.pFontText);
	GetDlgItem(IDC_STATIC_PASSWORD)->SetFont(&font_data.pFontText);

	GetDlgItem(IDC_BUTTON_LOGIN)->SetFont(&font_data.pFontButton);

	GetDlgItem(IDC_EDIT_USER)->SetFont(&font_data.pFontEdit);
	GetDlgItem(IDC_EDIT_PASSWORD)->SetFont(&font_data.pFontEdit);

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
BOOL CNotebookManagerAppDlg::ConnectToMariaDB()
{

	CString executablePath = GetExecutablePath();
	CString configFile = executablePath + L"\\Config.ini";
	ConfigManager configManager(configFile);

	 ip = configManager.GetIP();
	 port = configManager.GetPort();
	 uid = configManager.GetUID();
	 pwd = configManager.GetPWD();
	 dbname = configManager.GetDBNAME();

	conn = mysql_init(NULL);

	// Kết nối đến database
	if (!mysql_real_connect(conn, ip, uid, pwd, dbname,port, NULL, 0)) {
		AfxMessageBox(_T("Server Connection Fail"));
		return FALSE;
	}
	//mysql_close(conn);
	return TRUE;
}

void CNotebookManagerAppDlg::OnBnClickedButtonLogin()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (  ConnectToMariaDB() &&  conn != nullptr)
	{
		CString strSQL;
		strSQL.Format(_T("SELECT password, manager_access FROM user WHERE user_name = '%s'"), m_strUser);
		if (mysql_query(conn, CT2A(strSQL)))
		{
			AfxMessageBox(CString("Query execution failed: ") + CString(mysql_error(conn)));
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
					CString  passsword = CString(row[0]);
					         managerAccess = CString(row[1]);
							 str_global_manageraccess = managerAccess;
					if (passsword != m_strPassword)
					{
						GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextW(L"");
						UpdateData(TRUE);
						AfxMessageBox(CString("Your password was incorrect. Please try again."));
					}
					else
					{
						if (managerAccess == "admin" || managerAccess == "user")
						{
							mysql_free_result(res); // Giải phóng bộ nhớ
							mysql_close(conn); // Đóng kết nối
							SetWindowPos(NULL, -10000, -10000, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
							AdminDlg dlgNew;
							dlgNew.DoModal();
							// Trả về để thoát khỏi hàm
							return;

						}
						else
						{
							AfxMessageBox(CString("Not value manager access"));
						}
					}
					
				}
				else
				{
					GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextW(L"");
					GetDlgItem(IDC_EDIT_USER)->SetFocus();
					UpdateData(TRUE);
					AfxMessageBox(CString("Your user name was incorrect"));
				}
				mysql_free_result(res);
			}
		}
		mysql_close(conn);
	}
	
}
BOOL CNotebookManagerAppDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			OnBnClickedButtonLogin();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}