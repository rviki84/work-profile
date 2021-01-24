/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/


#include "CArteryModellingLumenReconstructionWidgetUI.h"

// begin wxGlade: ::extracode

// end wxGlade


CArteryModellingLumenReconstructionWidgetUI::CArteryModellingLumenReconstructionWidgetUI(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxScrolledWindow(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: CArteryModellingLumenReconstructionWidgetUI::CArteryModellingLumenReconstructionWidgetUI
	m_btnComputeSurface = new wxButton(this, wxID_LumenReconstructionWidget_ComputeSurface, wxT("Compute Surface"));
	m_cbxAdvancedOpt = new wxCheckBox(this, wxID_LumenReconstructionWidget_AdvancedOpt, wxT("Advanced Options"));
	m_sbx3DRecon = new wxStaticBox(this, wxID_ANY, wxT("3D Reconstruction of Centerlines"));
	
	m_sbxRegistration = new wxStaticBox(this, wxID_ANY, wxT("Registration"));
	m_stxOutlierWt = new wxStaticText(this, wxID_ANY, wxT("Outlier Weight"));
	m_txtOutlierWt = new wxTextCtrl(this, wxID_LumenReconstructionWidget_OutlierWeight, wxT("0.1"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_stxNoOfItersCPD = new wxStaticText(this, wxID_ANY, wxT("No. of Iterations"));
	m_txtNoOfItersCPD = new wxTextCtrl(this, wxID_LumenReconstructionWidget_NoOfItersCPD, wxT("300"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_stxTolerance = new wxStaticText(this, wxID_ANY, wxT("Tolerance"));
	m_txtTolerance = new wxTextCtrl(this, wxID_LumenReconstructionWidget_Tolerance, wxT("1.0e-8"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	
	m_sbxOSB = new wxStaticBox(this, wxID_ANY, wxT("Optimal Subsequence Bijection"));
	m_stxEpiDistThres = new wxStaticText(this, wxID_ANY, wxT("Epipolar Distance Threshold"));
	m_txtEpiDistThres = new wxTextCtrl(this, wxID_LumenReconstructionWidget_EpiDistThres, wxT("20.0,15.0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_stxEpiDistNote = new wxStaticText(this, wxID_ANY, wxT("(comma separated double)"));
	m_stxSkipDist = new wxStaticText(this, wxID_ANY, wxT("Skip Distance"));
	m_txtSkipDist = new wxTextCtrl(this, wxID_LumenReconstructionWidget_SkipDist, wxT("5"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_stxSkipCost = new wxStaticText(this, wxID_ANY, wxT("Skip Cost"));
	m_txtSkipCost = new wxTextCtrl(this, wxID_LumenReconstructionWidget_SkipCost, wxT("0.01"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_stxLengthCost = new wxStaticText(this, wxID_ANY, wxT("Lenght Cost"));
	m_txtLengthCost = new wxTextCtrl(this, wxID_LumenReconstructionWidget_LengthCost, wxT("20"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_stxNoOfItersOSB = new wxStaticText(this, wxID_ANY, wxT("No. of Iterations"));
	m_txtNoOfItersOSB = new wxTextCtrl(this, wxID_LumenReconstructionWidget_NoOfItersOSB, wxT("10,5"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_stxNoOfItersNote = new wxStaticText(this, wxID_ANY, wxT("(comma separated integer)"));
	m_stxResLevel = new wxStaticText(this, wxID_ANY, wxT("Resolution Levels"));
	m_txtResLevel = new wxTextCtrl(this, wxID_LumenReconstructionWidget_ResLevel, wxT("4,2"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_stxResLevelNote = new wxStaticText(this, wxID_ANY, wxT("(comma separated integer)"));

	m_sbxSurfRecon = new wxStaticBox(this, wxID_ANY, wxT("Surface Reconstruction"));
	m_stxReconDepth = new wxStaticText(this, wxID_ANY, wxT("Reconstruction Depth"));
	m_txtReconDepth = new wxTextCtrl(this, wxID_LumenReconstructionWidget_ReconDepth, wxT("7"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_stxPointWt = new wxStaticText(this, wxID_ANY, wxT("Interpolation Weight"));
	m_txtPointWt = new wxTextCtrl(this, wxID_LumenReconstructionWidget_PointWt, wxT("4"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    set_properties();
    do_layout();
    // end wxGlade
}


BEGIN_EVENT_TABLE(CArteryModellingLumenReconstructionWidgetUI, wxScrolledWindow)
    // begin wxGlade: CArteryModellingLumenReconstructionWidgetUI::event_table
	EVT_BUTTON(wxID_LumenReconstructionWidget_ComputeSurface, CArteryModellingLumenReconstructionWidgetUI::OnBtnComputeSurface)
	EVT_CHECKBOX(wxID_LumenReconstructionWidget_AdvancedOpt, CArteryModellingLumenReconstructionWidgetUI::OnChkAdvancedOpt)
	EVT_TEXT_ENTER(wxID_LumenReconstructionWidget_OutlierWeight, CArteryModellingLumenReconstructionWidgetUI::OnTxtOutlierWeight)
	EVT_TEXT_ENTER(wxID_LumenReconstructionWidget_NoOfItersCPD, CArteryModellingLumenReconstructionWidgetUI::OnTxtNoOfItersCPD)
	EVT_TEXT_ENTER(wxID_LumenReconstructionWidget_Tolerance, CArteryModellingLumenReconstructionWidgetUI::OnTxtTolerance)
	EVT_TEXT_ENTER(wxID_LumenReconstructionWidget_EpiDistThres, CArteryModellingLumenReconstructionWidgetUI::OnTxtEpiDistThres)
	EVT_TEXT_ENTER(wxID_LumenReconstructionWidget_SkipDist, CArteryModellingLumenReconstructionWidgetUI::OnTxtSkipDist)
	EVT_TEXT_ENTER(wxID_LumenReconstructionWidget_SkipCost, CArteryModellingLumenReconstructionWidgetUI::OnTxtSkipCost)
	EVT_TEXT_ENTER(wxID_LumenReconstructionWidget_LengthCost, CArteryModellingLumenReconstructionWidgetUI::OnTxtLengthCost)
	EVT_TEXT_ENTER(wxID_LumenReconstructionWidget_NoOfItersOSB, CArteryModellingLumenReconstructionWidgetUI::OnTxtNoOfItersOSB)
	EVT_TEXT_ENTER(wxID_LumenReconstructionWidget_ResLevel, CArteryModellingLumenReconstructionWidgetUI::OnTxtResLevel)
	EVT_TEXT_ENTER(wxID_LumenReconstructionWidget_ReconDepth, CArteryModellingLumenReconstructionWidgetUI::OnTxtReconDepth)
	EVT_TEXT_ENTER(wxID_LumenReconstructionWidget_PointWt, CArteryModellingLumenReconstructionWidgetUI::OnTxtPointWt)
    // end wxGlade
END_EVENT_TABLE();


void CArteryModellingLumenReconstructionWidgetUI::OnBtnComputeSurface(wxCommandEvent &event)
{
    event.Skip();
    wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnBtnComputeSurface) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingLumenReconstructionWidgetUI::OnChkAdvancedOpt(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnChkAdvancedOpt) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingLumenReconstructionWidgetUI::OnTxtOutlierWeight(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnTxtOutlierWeight) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingLumenReconstructionWidgetUI::OnTxtNoOfItersCPD(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnTxtNoOfItersCPD) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingLumenReconstructionWidgetUI::OnTxtTolerance(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnTxtTolerance) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingLumenReconstructionWidgetUI::OnTxtEpiDistThres(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnTxtEpiDistThres) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingLumenReconstructionWidgetUI::OnTxtSkipDist(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnTxtSkipDist) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingLumenReconstructionWidgetUI::OnTxtSkipCost(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnTxtSkipCost) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingLumenReconstructionWidgetUI::OnTxtLengthCost(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnTxtLengthCost) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingLumenReconstructionWidgetUI::OnTxtNoOfItersOSB(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnTxtNoOfItersOSB) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingLumenReconstructionWidgetUI::OnTxtResLevel(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnTxtResLevel) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}


void CArteryModellingLumenReconstructionWidgetUI::OnTxtReconDepth(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnTxtReconDepth) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingLumenReconstructionWidgetUI::OnTxtPointWt(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingLumenReconstructionWidgetUI::OnTxtPointWt) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}
// wxGlade: add CArteryModellingLumenReconstructionWidgetUI event handlers


void CArteryModellingLumenReconstructionWidgetUI::set_properties()
{
    // begin wxGlade: CArteryModellingLumenReconstructionWidgetUI::set_properties
    SetScrollRate(10, 10);
	m_btnComputeSurface->SetBackgroundColour(wxColour(64, 192, 64));
	m_cbxAdvancedOpt->SetToolTip("Advanced options setting for matching, motion compensation, \
		3D reconstruction of centerlines & Poisson surface reconstruction");
	m_sbx3DRecon->SetForegroundColour(wxColour(64, 192, 128));
	m_sbxSurfRecon->SetForegroundColour(wxColour(192, 64, 128));
	m_sbxRegistration->SetForegroundColour(wxColour(128, 64, 192));
	m_sbxOSB->SetForegroundColour(wxColour(128, 64, 192));
    // end wxGlade
}


void CArteryModellingLumenReconstructionWidgetUI::do_layout()
{
    // begin wxGlade: CArteryModellingLumenReconstructionWidgetUI::do_layout    
	sbxSizer3DRecon = new wxStaticBoxSizer(m_sbx3DRecon, wxVERTICAL);
	sbxSizerRegistration = new wxStaticBoxSizer(m_sbxRegistration, wxVERTICAL);
	sbxSizerOSB = new wxStaticBoxSizer(m_sbxOSB, wxVERTICAL);
	sbxSizerSurfRecon = new wxStaticBoxSizer(m_sbxSurfRecon, wxVERTICAL);
	
	wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
	sizer_1->Add(20, 20, 0, wxALIGN_LEFT, 0);
	sizer_1->Add(m_btnComputeSurface, 0, wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_1->Add(20, 20, 0, wxALIGN_LEFT, 0);
	sizer_1->Add(m_cbxAdvancedOpt, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);

	wxBoxSizer* sizer_21 = new wxBoxSizer(wxHORIZONTAL);
	sizer_21->Add(m_stxOutlierWt, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_21->Add(m_txtOutlierWt, 0, wxALIGN_CENTER_VERTICAL, 0);
	wxBoxSizer* sizer_22 = new wxBoxSizer(wxHORIZONTAL);
	sizer_22->Add(m_stxNoOfItersCPD, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_22->Add(m_txtNoOfItersCPD, 0, wxALIGN_CENTER_VERTICAL, 0);
	wxBoxSizer* sizer_23 = new wxBoxSizer(wxHORIZONTAL);
	sizer_23->Add(m_stxTolerance, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_23->Add(m_txtTolerance, 0, wxALIGN_CENTER_VERTICAL, 0);
	sbxSizerRegistration->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizerRegistration->Add(sizer_21, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	sbxSizerRegistration->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizerRegistration->Add(sizer_22, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	sbxSizerRegistration->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizerRegistration->Add(sizer_23, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);

	wxBoxSizer* sizer_24 = new wxBoxSizer(wxHORIZONTAL);
	sizer_24->Add(m_stxEpiDistThres, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_24->Add(m_txtEpiDistThres, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_24->Add(m_stxEpiDistNote, 0, wxALIGN_CENTER_VERTICAL, 0);
	wxBoxSizer* sizer_25 = new wxBoxSizer(wxHORIZONTAL);
	sizer_25->Add(m_stxSkipDist, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_25->Add(m_txtSkipDist, 0, wxALIGN_CENTER_VERTICAL, 0);
	wxBoxSizer* sizer_26 = new wxBoxSizer(wxHORIZONTAL);
	sizer_26->Add(m_stxSkipCost, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_26->Add(m_txtSkipCost, 0, wxALIGN_CENTER_VERTICAL, 0);
	wxBoxSizer* sizer_27 = new wxBoxSizer(wxHORIZONTAL);
	sizer_27->Add(m_stxLengthCost, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_27->Add(m_txtLengthCost, 0, wxALIGN_CENTER_VERTICAL, 0);
	wxBoxSizer* sizer_28 = new wxBoxSizer(wxHORIZONTAL);
	sizer_28->Add(m_stxNoOfItersOSB, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_28->Add(m_txtNoOfItersOSB, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_28->Add(m_stxNoOfItersNote, 0, wxALIGN_CENTER_VERTICAL, 0);
	wxBoxSizer* sizer_29 = new wxBoxSizer(wxHORIZONTAL);
	sizer_29->Add(m_stxResLevel, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_29->Add(m_txtResLevel, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_29->Add(m_stxResLevelNote, 0, wxALIGN_CENTER_VERTICAL, 0);
	sbxSizerOSB->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizerOSB->Add(sizer_24, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	sbxSizerOSB->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizerOSB->Add(sizer_25, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	sbxSizerOSB->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizerOSB->Add(sizer_26, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	sbxSizerOSB->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizerOSB->Add(sizer_27, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	sbxSizerOSB->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizerOSB->Add(sizer_28, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	sbxSizerOSB->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizerOSB->Add(sizer_29, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);

	sbxSizer3DRecon->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizer3DRecon->Add(sbxSizerRegistration, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	sbxSizer3DRecon->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizer3DRecon->Add(sbxSizerOSB, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);

	wxBoxSizer* sizer_33 = new wxBoxSizer(wxHORIZONTAL);
	sizer_33->Add(m_stxReconDepth, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_33->Add(m_txtReconDepth, 0, wxALIGN_CENTER_VERTICAL, 0);
	wxBoxSizer* sizer_34 = new wxBoxSizer(wxHORIZONTAL);
	sizer_34->Add(m_stxPointWt, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_34->Add(m_txtPointWt, 0, wxALIGN_CENTER_VERTICAL, 0);
	sbxSizerSurfRecon->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizerSurfRecon->Add(sizer_33, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	sbxSizerSurfRecon->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbxSizerSurfRecon->Add(sizer_34, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	
	wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
	sizer_2->Add(20, 10, 0, wxEXPAND|wxADJUST_MINSIZE, 0);
	sizer_2->Add(sbxSizer3DRecon, 0, wxEXPAND|wxADJUST_MINSIZE, 0);
	sizer_2->Add(20, 20, 0, wxEXPAND|wxADJUST_MINSIZE, 0);
	sizer_2->Add(sbxSizerSurfRecon, 0, wxEXPAND|wxADJUST_MINSIZE, 0);

	wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
    GlobalSizer->Add(sizer_1, 0, wxEXPAND|wxADJUST_MINSIZE, 0);
	GlobalSizer->Add(sizer_2, 0, wxEXPAND|wxADJUST_MINSIZE, 0);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}
