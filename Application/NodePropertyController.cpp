#include <Main/Common.h>
#include "NodePropertyController.h"
#include "MainFrame.h"

CNodePropertyController::CNodePropertyController()
{
	gEnv->PropCtrl = this;
	mTargetNode = 0;
	mOutdated = true;
}

CNodePropertyController::~CNodePropertyController()
{

}

void CNodePropertyController::SetTargetNode(NNode* node)
{
	mTargetNode = node;
	mOutdated = true;
}

void CNodePropertyController::UpdatePropertiesFromTarget()
{
	// You are losing all the changes!!!

	// if( mHasUI )
	CreateUI();
	mOutdated = false;
}

bool CNodePropertyController::UpdateTarget()
{
	if (mTargetNode)
	{
		mTargetNode->SetName(mNameCtrl->GetValue().ToStdString());
		for (TPropTable::iterator iter = mTargetNode->mPropTable.begin();
			iter != mTargetNode->mPropTable.end(); iter++)
		{
			if ((*iter).mType == std::string("T"))
			{
				wxTextCtrl* control = (wxTextCtrl*)FindInControlList((*iter).mName);
				(*iter).mValue = control->GetValue().ToStdString();
			}
			else if ((*iter).mType == std::string("D"))
			{
				// UNIMPLEMENTED!!!!!!!!!
			}
			else if ((*iter).mType == std::string("C"))
			{
				wxCheckBox* control = (wxCheckBox*)FindInControlList((*iter).mName);
				if (control->GetValue())
					(*iter).mValue = std::string("T");
				else
					(*iter).mValue = std::string("F");
			}
		}

		// Repaint the whole thing
		gEnv->MainFrame->PaintFlowEditor();
		CreateUI();
		return true;
	}
	return false;
}

TPropTable& CNodePropertyController::GetAvailableProps()
{
	if (mTargetNode)
	{
		return mTargetNode->mPropTable;
	}
	assert(1); // BUG!!!
	return mTargetNode->mPropTable;
}

std::string& CNodePropertyController::GetPropertyValue(const std::string& name)
{
	if (mTargetNode)
	{
		for (TPropTable::iterator iter = mTargetNode->mPropTable.begin();
			iter != mTargetNode->mPropTable.end(); iter++)
		{
			if (name == (*iter).mName)
			{
				return (*iter).mValue;
			}
		}
	}
	assert(1); // BUUUUGGGGG!!!
	return const_cast<std::string&>(name);
}

bool CNodePropertyController::SetPropertyValue(const std::string& name, const std::string& value)
{
	if (mTargetNode)
	{
		for (TPropTable::iterator iter = mTargetNode->mPropTable.begin();
			iter != mTargetNode->mPropTable.end(); iter++)
		{
			if (name == (*iter).mName)
			{
				(*iter).mValue = value;
				return true;
			}
		}
	}
	return false;
}

// Helpers for GUI (Creating ribbon elements to edit props)
void CNodePropertyController::CreateUI()
{
	// mHasUI = true;
	DestroyUI();
	if (mTargetNode)
	{
		mNameCtrl = CreateText("Name");
		mNameCtrl->SetValue(mTargetNode->GetName().c_str());
		mCtrlList.clear();
		for (TPropTable::iterator iter = mTargetNode->mPropTable.begin();
			iter != mTargetNode->mPropTable.end(); iter++)
		{
			if ((*iter).mType == std::string("T"))
			{
				wxTextCtrl* control = CreateText((*iter).mName);
				control->SetValue((*iter).mValue);
				
				TControlPair tCtrlPair;
				tCtrlPair.first = (*iter).mName;
				tCtrlPair.second = control;
				mCtrlList.push_back(tCtrlPair);
			}
			else if ((*iter).mType == std::string("D"))
			{
				// UNIMPLEMENTED!!!!!!!!!
				//wxTextCtrl* control = CreateText((*iter).mName);
				//control->SetValue((*iter).mValue);
			}
			else if ((*iter).mType == std::string("C"))
			{
				wxCheckBox* control = CreateCheckBox((*iter).mName);
				if ((*iter).mValue == std::string("T"))
					control->SetValue(true);
				else
					control->SetValue(false);

				TControlPair tCtrlPair;
				tCtrlPair.first = (*iter).mName;
				tCtrlPair.second = control;
				mCtrlList.push_back(tCtrlPair);
			}
		}
	}
	gEnv->MainFrame->mPropertyPage->Realize();
}

wxTextCtrl* CNodePropertyController::CreateText(const std::string& name)
{
	CreateUIPanel();
	wxRibbonPanel* panel = gEnv->MainFrame->mPropertyPanel;
	wxStaticText* namelable = new wxStaticText(panel, wxID_ANY, wxString(name.c_str()) + wxString(": "));
	wxTextCtrl* control = new wxTextCtrl(panel, wxID_ANY);
	wxSizer* bundle = new wxBoxSizer(wxHORIZONTAL);
	bundle->Add(namelable);
	bundle->Add(control);
	mPropPanelSizer->Add(bundle);
	return control;
}

wxChoice* CNodePropertyController::CreateChoice(const std::string& name)
{
	CreateUIPanel();
	wxRibbonPanel* panel = gEnv->MainFrame->mPropertyPanel;
	wxStaticText* namelable = new wxStaticText(panel, wxID_ANY, wxString(name.c_str()) + wxString(": "));
	wxChoice* control = new wxChoice(panel, wxID_ANY);
	wxSizer* bundle = new wxBoxSizer(wxHORIZONTAL);
	bundle->Add(namelable);
	bundle->Add(control);
	mPropPanelSizer->Add(bundle);
	return control;
}

wxCheckBox* CNodePropertyController::CreateCheckBox(const std::string& name, bool default_value)
{
	CreateUIPanel();
	wxRibbonPanel* panel = gEnv->MainFrame->mPropertyPanel;
	wxCheckBox* control = new wxCheckBox(panel, wxID_ANY, wxString(name.c_str()));
	mPropPanelSizer->Add(control, 0);
	return control;
}

void CNodePropertyController::CreateUIPanel()
{
	if (!gEnv->MainFrame->mPropertyPanel)
	{
		wxString name("Property");
		if (mTargetNode)
			name = name + " - " + mTargetNode->GetName().c_str();
		gEnv->MainFrame->mPropertyPanel = new wxRibbonPanel(gEnv->MainFrame->mPropertyPage, wxID_ANY, name);
		mPropPanelSizer = new wxBoxSizer(wxVERTICAL);
		gEnv->MainFrame->mPropertyPanel->SetSizer(mPropPanelSizer);
	}
}

void CNodePropertyController::DestroyUI()
{
	if (gEnv->MainFrame->mPropertyPanel)
	{
		delete gEnv->MainFrame->mPropertyPanel;
		gEnv->MainFrame->mPropertyPanel = 0;
	}
}

void* CNodePropertyController::FindInControlList(const std::string& propname)
{
	for (TControlList::iterator iter = mCtrlList.begin();
		iter != mCtrlList.end(); iter++)
	{
		if (propname == (*iter).first)
			return (*iter).second;
	}
	return 0;
}
