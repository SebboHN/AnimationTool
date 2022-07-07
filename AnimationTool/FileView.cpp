
#include "pch.h"
#include "framework.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "AnimationTool.h"
#include "application.h"
#include "Playback.h"
#include "RenderWindow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView() noexcept
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle=WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::GetDirectories(CString sPath) {
	CFileFind finder;
	BOOL bFind=finder.FindFile(sPath);

	while (bFind)
	{
		bFind=finder.FindNextFileW();
		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			CollectionFolder.push_back(finder.GetFilePath());
			GetDirectories(finder.GetFilePath() + _T("\\*.*"));
		}
	}
	finder.Close();
}

void CFileView::FillFileView()
{	
	CString myPath=L"F:\\Steam\\steamapps\\common\\Fallout 4\\Data\\Meshes\\Actors\\Character\\Animations";
	GetDirectories(myPath);
	CFileFind findernew;
	bool hasFiles=false;
	hRoot=m_wndFileView.InsertItem(myPath, 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	for (int i=0; i < CollectionFolder.size(); i++) {

		if (CollectionFolder[i] == myPath) 
		{
			hSrc=m_wndFileView.InsertItem(CollectionFolder[i], i, i, hRoot);
		} 
		else
		{
			CString deletion=CollectionFolder[i];
			deletion.Delete(0, myPath.GetLength());
			hSrc=m_wndFileView.InsertItem(deletion, i, i, hRoot);
		}
	
		// start working for files
		BOOL bWorking=findernew.FindFile(CollectionFolder[i] + _T("\\*"));
		while (bWorking)
		{
			bWorking=findernew.FindNextFile();
			// skip . and .. files
			if (!findernew.IsDots())
			{
				CString Temp=findernew.GetFileName();
				int totalLength=Temp.GetLength();
				Temp.Delete(0, totalLength - 4);
				if (Temp == ".hkx") {
					m_wndFileView.InsertItem(findernew.GetFileName(), i, i, hSrc);
				}
			}
		}
	}
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree=(CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree=point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags=0;
		HTREEITEM hTreeItem=pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != nullptr)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndFileView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
	ozz::Application::ProcResize=true;

}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("Properties...."));

}

void CFileView::OnFileOpen()
{
	HTREEITEM selectedItem=m_wndFileView.GetSelectedItem();
	CString ItemText=m_wndFileView.GetItemText(selectedItem);
	
	HTREEITEM parentItem=m_wndFileView.GetParentItem(selectedItem);

	CString reconstructPath=CollectionFolder[0] + m_wndFileView.GetItemText(parentItem) + "\\" + ItemText;
	hkxConversion(reconstructPath, ItemText);
	std::wcout << reconstructPath << std::endl;
	// TODO: Add your command handler code here
}

void CFileView::OnFileOpenWith()
{
	// TODO: Add your command handler code here
}

void CFileView::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{

}


void CFileView::hkxConversion(CString FilePaths, CString newPath) {

	std::string copyPath="\\hkx\\";
	std::string tempstr(CW2A(newPath.GetString()));
	copyPath=copyPath.append(tempstr);
	std::filesystem::path cwd=std::filesystem::current_path();
	copyPath=cwd.string() + copyPath;
	std::string copyPathTmp=copyPath;
	if (copyPath.find(".hkx")) {
		copyPath.replace(copyPath.length() - 3, copyPath.length(), "hko");
	}
	std::filesystem::copy_file(LPCTSTR(FilePaths), copyPathTmp);
	std::string test=cwd.string() + "\\Data\\" + "PC32.hko";
	std::filesystem::copy_file(test, copyPath);

	CreateConversion(copyPath, copyPathTmp);
	const char* myChar=tempstr.c_str();
	std::string strCommand=cwd.string() + "\\Data\\HCT_2014\\hctStandAloneFilterManager.exe -s \"" + copyPath + "\" \"" + copyPathTmp + "\" -t -v -o \"" + copyPathTmp + "\"";



	const char* completeCommand=strCommand.c_str();

	system(completeCommand);

	std::string strCommand2nd=cwd.string() + "\\Data\\HavokBehaviorPostProcess.exe --platformWin32 " + "\"" + copyPathTmp + "\" \"" + copyPathTmp + "\"";
	const char* completeCommand2nd=strCommand2nd.c_str();

	system(completeCommand2nd);
	FbxConversion(copyPathTmp);
}

void CFileView::FbxConversion(std::string hkxPath) {
	std::filesystem::path cwdFbx = std::filesystem::current_path();
	//std::string str_SkeletonPath(CW2A(SkeletonPath.GetString()));
	//std::string str_HKXPath(CW2A(hkxPath.GetString()));
	std::string CMDCommand = cwdFbx.string() + "\\Data\\havok2fbx.exe" + " " + "-hk_skeleton " + "\"" + cwdFbx.string() + "\\Data\\skeleton.hkx" + "\"" + " " + "-hk_anim " + "\"" + hkxPath + "\"" + " " + "-fbx " +"\"" +cwdFbx.string()+"\\fbx\\" + "myFile.fbx" + "\"";
	const char* completeCommandFBX = CMDCommand.c_str();
	system(completeCommandFBX);

}

void CFileView::CreateConversion(std::string newFile, std::string copyPathTemp) {


	std::ofstream ofs;
	ofs.open(newFile, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	ofs.open(newFile,
		std::ios::out |   // output file stream
		std::ios::app |   // can append to a existing file
		std::ios::ate);  // set file cursor at the end

	ofs << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
	ofs << "<hkoptions>" << std::endl;
	ofs << "<hkobject class=\"hctConfigurationSetData\">" << std::endl;
	ofs << "<hkparam name=\"filterManagerVersion\">65536</hkparam>" << std::endl;
	ofs << "<hkparam name=\"activeConfiguration\">0</hkparam>" << std::endl;
	ofs << "</hkobject>" << std::endl;
	ofs << "<hkobject class=\"hctConfigurationData\">" << std::endl;
	ofs << "<hkparam name=\"configurationName\">ConvertAnimation_x32</hkparam>" << std::endl;
	ofs << "<hkparam name=\"numFilters\">3</hkparam>" << std::endl;
	ofs << "</hkobject>" << std::endl;
	ofs << "<hkobject name=\"Merge Asset\" class=\"hctFilterData\">" << std::endl;
	ofs << "<hkparam name=\"id\">3033372433</hkparam>" << std::endl;
	ofs << "<hkparam name=\"ver\">65536</hkparam>" << std::endl;
	ofs << "<hkparam name=\"hasOptions\">true</hkparam>" << std::endl;
	ofs << "</hkobject>" << std::endl;
	ofs << "<hkobject name=\"Merge Asset\" class=\"hctMergeAssetOptions\">" << std::endl;
	ofs << "<hkparam name=\"filename\">" << copyPathTemp << "</hkparam>" << std::endl;
	ofs << "</hkobject>" << std::endl;
	ofs << "<hkobject name=\"Prune Types\" class=\"hctFilterData\">" << std::endl;
	ofs << "<hkparam name=\"id\">2496164476</hkparam>" << std::endl;
	ofs << "<hkparam name=\"ver\">66048</hkparam>" << std::endl;
	ofs << "<hkparam name=\"hasOptions\">true</hkparam>" << std::endl;
	ofs << "</hkobject>" << std::endl;
	ofs << "<hkobject name=\"Prune Types\" class=\"hctPruneTypesOptions\">" << std::endl;
	ofs << "<hkparam name=\"pruneSceneData\">true</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneMeshData\">false</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneSkeletonData\">true</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneAnimationData\">false</hkparam>" << std::endl;
	ofs << "<hkparam name=\"customClasses\"></hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneAllSceneData\">true</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneEnvironmentData\">true</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneResourceData\">true</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneDestructionData\">false</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneAnimationTracks\">false</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneAnnotations\">false</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneIdentityBindingIndices\">true</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneQuantizedBindings\">true</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneAttributes\">false</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneMeshUserChannels\">false</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneAttachments\">true</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneSelectionSets\">false</hkparam>" << std::endl;
	ofs << "<hkparam name=\"selectionSets\"></hkparam>" << std::endl;
	ofs << "<hkparam name=\"selectionDeletionMode\">HK_SELECTION_DELETE_SELECTED</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneAllAnimationData\">false</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneMeshBindingData\">true</hkparam>" << std::endl;
	ofs << "<hkparam name=\"pruneRagdollAndMapperData\">true</hkparam>" << std::endl;
	ofs << "</hkobject>" << std::endl;
	ofs << "<hkobject name=\"Write to Platform\" class=\"hctFilterData\">" << std::endl;
	ofs << "<hkparam name=\"id\">2876798309</hkparam>" << std::endl;
	ofs << "<hkparam name=\"ver\">66048</hkparam>" << std::endl;
	ofs << "<hkparam name=\"hasOptions\">true</hkparam>" << std::endl;
	ofs << "</hkobject>" << std::endl;
	ofs << "<hkobject name=\"Write to Platform\" class=\"hctPlatformWriterOptions\">" << std::endl;
	ofs << "<hkparam name=\"filename\">" << copyPathTemp << "</hkparam>" << std::endl;
	ofs << "<hkparam name=\"tagfile\">false</hkparam>" << std::endl;
	ofs << "<hkparam name=\"preset\">MSVC_WIN32</hkparam>" << std::endl;
	ofs << "<hkparam name=\"bytesInPointer\">4</hkparam>" << std::endl;
	ofs << "<hkparam name=\"littleEndian\">1</hkparam>" << std::endl;
	ofs << "<hkparam name=\"reusePaddingOptimized\">0</hkparam>" << std::endl;
	ofs << "<hkparam name=\"emptyBaseClassOptimized\">1</hkparam>" << std::endl;
	ofs << "<hkparam name=\"removeMetadata\">true</hkparam>" << std::endl;
	ofs << "<hkparam name=\"userTag\">0</hkparam>" << std::endl;
	ofs << "<hkparam name=\"saveEnvironmentData\">false</hkparam>" << std::endl;
	ofs << "<hkparam name=\"xmlFormat\">false</hkparam>" << std::endl;
	ofs << "</hkobject>" << std::endl;
	ofs << "</hkoptions>" << std::endl;

	ofs.close();

}