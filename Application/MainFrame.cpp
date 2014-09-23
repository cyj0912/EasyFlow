#include <Main\Common.h>
#include "MainFrame.h"
#include "ToolBox.h"
#include "FlowEditor.h"
#include "FlowGraph.h"
#include "NodePropertyController.h"
#include <wx/aboutdlg.h>
// #include <dwmapi.h>
#include <Main\XPM\ico.xpm>
#include <Main\XPM\ICO_NEW.xpm>
#include <Main\XPM\ICO_SAVE.xpm>
#include <Main\XPM\ICO_TIP.xpm>
#include <Main\XPM\Lite-Icon-icon.xpm>

BEGIN_EVENT_TABLE(CMainFrame, wxFrame)
	EVT_CLOSE(CMainFrame::OnClose)
	EVT_MENU(wxID_EXIT, CMainFrame::OnQuit)
	EVT_MENU(wxID_ABOUT, CMainFrame::OnAbout)
	EVT_RIBBONTOOLBAR_CLICKED(wxID_ABOUT, CMainFrame::OnRibbonAbout)
	EVT_RIBBONTOOLBAR_CLICKED(wxID_HELP_INDEX, CMainFrame::OnRibbonTips)
	EVT_RIBBONTOOLBAR_CLICKED(wxID_EXECUTE, CMainFrame::OnRibbonDump)
END_EVENT_TABLE()

CMainFrame::CMainFrame()
	: wxFrame(NULL, wxID_ANY, "myFlow", wxDefaultPosition, wxDefaultSize)
{
	gEnv->MainFrame = this;
	mPropertyPanel = 0;
	SetIcon(wxIcon(tip_xpm));
	Maximize(true);
	CreateControls();
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::CreateControls()
{
	// Start creating main UI
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	mMenubar = new wxMenuBar;
	wxMenu* file = new wxMenu;
	file->Append(wxID_EXIT, wxT("&Quit"));
	mMenubar->Append(file, wxT("&File"));
	wxMenu* help = new wxMenu;
	help->Append(wxID_ABOUT, wxT("&About"));
	mMenubar->Append(help, wxT("&Help"));
	SetMenuBar(mMenubar);

	mStatusbar = CreateStatusBar();
	mStatusbar->SetStatusText("Ready");

	mRibbon = new wxRibbonBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxRIBBON_BAR_DEFAULT_STYLE);
	mRibbon->SetTabCtrlMargins(10, 20);
	{
		wxRibbonPage* rpage = new wxRibbonPage(mRibbon, -1, "Common");
		mPropertyPage = rpage;
		wxRibbonPanel* rpanel = new wxRibbonPanel(rpage, wxID_ANY, "Common");
		wxRibbonToolBar* rtoolbar = new wxRibbonToolBar(rpanel);
		rtoolbar->AddTool(wxID_NEW, ICO_NEW, "New");
		rtoolbar->AddTool(wxID_SAVE, ICO_SAVE, "Save");
		rtoolbar->AddTool(wxID_HELP_INDEX, ICO_TIP, "Tips");
		rtoolbar->AddTool(wxID_ABOUT, tip_xpm, "About");
		//rtoolbar->SetRows(2, 2);

		rpanel = new wxRibbonPanel(rpage, wxID_ANY, "Program");
		rtoolbar = new wxRibbonToolBar(rpanel);
		rtoolbar->AddTool(wxID_OK, Lite_Icon_icon, "Generate");
		rtoolbar->AddTool(wxID_EXECUTE, Lite_Icon_icon, "Dump Graph Info");
		//rtoolbar->SetRows(2, 3);
	}
	{
		wxRibbonPage* rpage = new wxRibbonPage(mRibbon, wxID_ANY, "About");
		wxRibbonPanel* rpanel = new wxRibbonPanel(rpage, wxID_ANY, "About");
	}
	mRibbon->Realise();

	wxPanel* midpanel = new wxPanel(this, wxID_ANY);
	midpanel->SetBackgroundColour(wxColour(216, 236, 255));
	{
		CToolBox* toolbox = new CToolBox(midpanel, wxID_ANY);
		mFlowEditor = new CFlowEditor(midpanel, wxID_ANY);
		wxSizer* midsizer = new wxBoxSizer(wxHORIZONTAL);
		midsizer->Add(toolbox, 0, wxEXPAND | wxALL, 10);
		midsizer->Add(mFlowEditor, 1, wxEXPAND | wxALL, 10);
		midpanel->SetSizer(midsizer);
	}

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(mRibbon, 0, wxEXPAND);
	sizer->Add(midpanel, 1, wxEXPAND);
	SetSizer(sizer);

	//Connect(wxEVT_PAINT, wxPaintEventHandler(CMainFrame::OnPaint));

	//mOutput = new wxDialog(this, wxID_ANY, "Output");
	//mLog = new wxTextCtrl(mOutput, wxID_ANY, wxEmptyString,
	//	wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE |
	//	wxTE_LEFT | wxTE_BESTWRAP | wxBORDER_NONE);

	mFlowGraph = new CFlowGraph();
	mFlowGraph->BindEditor(mFlowEditor);
	new CNodePropertyController;
}

void CMainFrame::OnClose(wxCloseEvent& evt)
{
	this->Destroy();
	//mOutput->Destroy();
}

void CMainFrame::OnPaint(wxPaintEvent& evt)
{
}

void CMainFrame::OnQuit(wxCommandEvent& evt)
{
	this->Close();
}

void CMainFrame::OnAbout(wxCommandEvent& evt)
{
	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName(_(APP_NAME));
	aboutInfo.SetVersion(_(APP_VERSION));
	aboutInfo.SetDescription(_("Rethinking Programming!"));
	aboutInfo.SetCopyright("(C) 2014 Toby Chen");
	aboutInfo.SetWebSite("http://www.ncchristian.org");
	aboutInfo.AddDeveloper("Toby Chen");
	aboutInfo.SetLicence(wxT("Source code is available in GPLv3\n")
		wxT("https://github.com/cyj0912/EasyFlow"));
	wxAboutBox(aboutInfo);
}

void CMainFrame::OnRibbonAbout(wxRibbonToolBarEvent& evt)
{
	OnAbout(static_cast<wxCommandEvent>(0));
}

void CMainFrame::OnRibbonTips(wxRibbonToolBarEvent& evt)
{
	mStatusbar->SetStatusText(wxT("Nothing yet..."));
}

void CMainFrame::OnRibbonDump(wxRibbonToolBarEvent& evt)
{
	const char* statusText = gEnv->FlowGraph->GetStatus();
	wxMessageBox(statusText);
}

CFlowGraph* CMainFrame::GetFlowGraph()
{
	return mFlowGraph;
}

void CMainFrame::PaintFlowEditor()
{
	wxClientDC dc(mFlowEditor);
	mFlowEditor->Render(dc);
}
