
// AnimationTool.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "AnimationTool.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "AnimationToolDoc.h"
#include "AnimationToolView.h"
#include "Playback.h"
#include "RenderWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool ozz::Application::closeProc;
static DWORD dwExitCode;
// CAnimationToolApp
static UINT __cdecl ResizeExtern(LPVOID pParam);



BEGIN_MESSAGE_MAP(CAnimationToolApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CAnimationToolApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()

UINT __cdecl OpenGLWindow(LPVOID pParam);
// CAnimationToolApp construction

CAnimationToolApp::CAnimationToolApp() noexcept
{
	m_bHiColorIcons = TRUE;


	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("AnimationTool.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CAnimationToolApp object

CAnimationToolApp theApp;


// CAnimationToolApp initialization

BOOL CAnimationToolApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();



	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	SetRegistryKey(_T("Animation Tool"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_AnimationToolTYPE,
		RUNTIME_CLASS(CAnimationToolDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CAnimationToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	ThreadData StartDataNew;
	StartDataNew._ProcId = 1;
	StartDataNew._ProcTitle = "Animation Tool";
	StartDataNew._ProcVersion = "1.0";
	StartDataNew._ProcPath = &StartDataNew._ProcTitle;



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	CAnimationToolApp::myWindowDoc = theApp.GenericGetActiveDocument();
	POSITION pos = CAnimationToolApp::myWindowDoc->GetFirstViewPosition();
	CView* pView = CAnimationToolApp::myWindowDoc->GetNextView(pos);
	RECT myRect;
	pView->GetClientRect(&myRect);
	HWND read = pView->GetSafeHwnd();
	StartDataNew._DocWidth = myRect.right;
	StartDataNew._DocHeight = myRect.bottom;

	// This function creates a new thread and modifies the 'achieved' global variable at some point in the future
	RenderLoop = AfxBeginThread(OpenGLWindow, &StartDataNew);

	//OpenGLWindow(&StartDataNew);
	// Wait the event to be signalled for 4 seconds!
	WaitForSingleObject(CreationReached, 1000);
	HWND embRendWindow = glfwGetWin32Window(SingletonRenderWindow::RenderWindowGet());
	ResizeLoop = AfxBeginThread(ResizeExtern, NULL);

	SetParent(embRendWindow, pView->GetSafeHwnd());
	pMainFrame->ShowWindow(m_nCmdShow);
	glfwFocusWindow(SingletonRenderWindow::RenderWindowGet());
	glfwShowWindow(SingletonRenderWindow::RenderWindowGet());
	pMainFrame->UpdateWindow();


	return TRUE;
}

int CAnimationToolApp::ExitInstance()
{
	//RenderLoop
	dwExitCode = 1;
	TerminateThread(&ResizeLoop, 0);
	ozz::Application::closeProc = true;
	Sleep(1000);
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CAnimationToolApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

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

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CAnimationToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CAnimationToolApp customization load/save methods

void CAnimationToolApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CAnimationToolApp::LoadCustomState()
{
}

void CAnimationToolApp::SaveCustomState()
{
}

// CAnimationToolApp message handlers
UINT __cdecl OpenGLWindow(LPVOID pParam) {

	PlaybackSampleApplication().Run(pParam);
	return 0;
}

CDocument* CAnimationToolApp::GenericGetActiveDocument()
{
	CDocument* pDoc = NULL;
	CWnd* pWndMain = AfxGetMainWnd();
	if (NULL != pWndMain)
	{
		if (pWndMain->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
		{
			// MDI application, so first we have to get the active MDI child frame.
			CFrameWnd* pFrame = ((CMDIFrameWnd*)pWndMain)->MDIGetActive();
			if (NULL != pFrame)
			{
				pDoc = pFrame->GetActiveDocument();
			}
		}
		else if (pWndMain->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		{
			// SDI appllication so main window is the active frame. 
			pDoc = ((CFrameWnd*)pWndMain)->GetActiveDocument();
		}
		else
		{
			ASSERT(FALSE); // Neither MDI nor SDI application.
		}
	}
	return pDoc;
}

UINT __cdecl ResizeExtern(LPVOID pParam) {
	RECT myRect;
	CView* pView = nullptr;
	POSITION pos;

	int i = 0;
	while (i == 0)
	{
		if (ozz::Application::ProcResize == true) {
			Sleep(2000);
			if (dwExitCode == 1) {
				i++;
			}
			//bool exitcode = GetExitCodeThread(CAnimationToolApp::ResizeLoop, &dwExitCode);
			else if (CAnimationToolApp::myWindowDoc != nullptr)
			{
				if (dwExitCode != 1) {
					pos = CAnimationToolApp::myWindowDoc->GetFirstViewPosition();
				}
				if (dwExitCode != 1) {
					pView = CAnimationToolApp::myWindowDoc->GetNextView(pos);
				}
				if (dwExitCode != 1) {
					pView->GetClientRect(&myRect);
				}
				if (dwExitCode != 1) {
					glfwSetWindowSize(SingletonRenderWindow::RenderWindowGet(), myRect.right, myRect.bottom);
				}
				if (dwExitCode != 1) {
					glfwShowWindow(SingletonRenderWindow::RenderWindowGet());

					//glfwGetFramebufferSize(SingletonRenderWindow::RenderWindowGet(), myRect.right, myRect.bottom);
				}
			}
		ozz::Application::ProcResize = false;
		}
		//CAnimationToolApp::myWindowDoc = theApp.GenericGetActiveDocument();
	}
	return 0;
}