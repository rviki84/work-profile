/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/


#include "CArteryModellingSegmentCenterlineWidgetUI.h"

// begin wxGlade: ::extracode

// end wxGlade


CArteryModellingSegmentCenterlineWidgetUI::CArteryModellingSegmentCenterlineWidgetUI(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxScrolledWindow(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: CArteryModellingSegmentCenterlineWidgetUI::CArteryModellingSegmentCenterlineWidgetUI
	m_stxCardiacPhase = new wxStaticText(this, wxID_ANY, wxT("Current Cardiac Phase"));
	m_txtCardiacPhase = new wxTextCtrl(this, wxID_ANY, wxT("0.0"));
	m_btnSelectArtery = new wxToggleButton(this, wxID_SegmentCenterlineWidget_SelectArtery, wxT("Select Artery"));
	m_btnHelperPoint = new wxButton(this, wxID_SegmentCenterlineWidget_HelperPoint, wxT("Get Helper Point"));
	m_stxFrameNum = new wxStaticText(this, wxID_ANY, wxT("Frame #"));
	m_txtFrameNum = new wxTextCtrl(this, wxID_SegmentCenterlineWidget_FrameNum, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_stxBranchNum = new wxStaticText(this, wxID_ANY, wxT("Branch #"));
	m_txtBranchNum = new wxTextCtrl(this, wxID_SegmentCenterlineWidget_BranchNum, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_btnSegment = new wxButton(this, wxID_SegmentCenterlineWidget_Segment, wxT("Segment Centerline"));
	m_btnDummySegment = new wxButton(this, wxID_SegmentCenterlineWidget_DummySegment, wxT("Dummy Segment"));
	m_btnMerge = new wxButton(this, wxID_SegmentCenterlineWidget_Merge, wxT("Merge Segments"));
	m_btnDeleteSegment = new wxButton(this, wxID_SegmentCenterlineWidget_DeleteSegment, wxT("Delete Segment"));

	m_cbxAdvancedOpt = new wxCheckBox(this, wxID_SegmentCenterlineWidget_AdvancedOpt, wxT("Advanced Options"));
	m_sbxAdvancedOpt = new wxStaticBox(this, wxID_ANY, wxT(""));
	m_stxStartRad = new wxStaticText(this, wxID_ANY, wxT("Start Radius"));
	m_txtStartRad = new wxTextCtrl(this, wxID_SegmentCenterlineWidget_StartRad, wxT("-1.0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_stxEndRad = new wxStaticText(this, wxID_ANY, wxT("End Radius"));
	m_txtEndRad = new wxTextCtrl(this, wxID_SegmentCenterlineWidget_EndRad, wxT("-1.0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_btnSampleRad = new wxButton(this, wxID_SegmentCenterlineWidget_SampleRad, wxT("Sample Radius"));
	m_stxResample = new wxStaticText(this, wxID_ANY, wxT("Resampling Rate"));
	m_txtResample = new wxTextCtrl(this, wxID_SegmentCenterlineWidget_Resample, wxT("0.2"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_stxSmooth = new wxStaticText(this, wxID_ANY, wxT("Smoothing Window Width"));
	m_txtSmooth = new wxTextCtrl(this, wxID_SegmentCenterlineWidget_Smooth, wxT("1.0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    set_properties();
    do_layout();
    // end wxGlade
}


BEGIN_EVENT_TABLE(CArteryModellingSegmentCenterlineWidgetUI, wxScrolledWindow)
    // begin wxGlade: CArteryModellingSegmentCenterlineWidgetUI::event_table
	EVT_TOGGLEBUTTON(wxID_SegmentCenterlineWidget_SelectArtery, CArteryModellingSegmentCenterlineWidgetUI::OnBtnSelectArtery)
	EVT_BUTTON(wxID_SegmentCenterlineWidget_HelperPoint, CArteryModellingSegmentCenterlineWidgetUI::OnBtnGetHelperPoint)
	EVT_BUTTON(wxID_SegmentCenterlineWidget_Segment, CArteryModellingSegmentCenterlineWidgetUI::OnBtnSegment)
	EVT_BUTTON(wxID_SegmentCenterlineWidget_DummySegment, CArteryModellingSegmentCenterlineWidgetUI::OnBtnDummySegment)
	EVT_TEXT_ENTER(wxID_SegmentCenterlineWidget_FrameNum, CArteryModellingSegmentCenterlineWidgetUI::OnTxtFrameNum)
	EVT_TEXT_ENTER(wxID_SegmentCenterlineWidget_BranchNum, CArteryModellingSegmentCenterlineWidgetUI::OnTxtBranchNum)
	EVT_BUTTON(wxID_SegmentCenterlineWidget_Merge, CArteryModellingSegmentCenterlineWidgetUI::OnBtnMerge)
	EVT_BUTTON(wxID_SegmentCenterlineWidget_DeleteSegment, CArteryModellingSegmentCenterlineWidgetUI::OnBtnDeleteSegment)
	EVT_CHECKBOX(wxID_SegmentCenterlineWidget_AdvancedOpt, CArteryModellingSegmentCenterlineWidgetUI::OnChkAdvancedOpt)
	EVT_BUTTON(wxID_SegmentCenterlineWidget_SampleRad, CArteryModellingSegmentCenterlineWidgetUI::OnBtnSampleRadius)
	EVT_TEXT_ENTER(wxID_SegmentCenterlineWidget_StartRad, CArteryModellingSegmentCenterlineWidgetUI::OnTxtStartRadius)
	EVT_TEXT_ENTER(wxID_SegmentCenterlineWidget_EndRad, CArteryModellingSegmentCenterlineWidgetUI::OnTxtEndRadius)
	EVT_TEXT_ENTER(wxID_SegmentCenterlineWidget_Resample, CArteryModellingSegmentCenterlineWidgetUI::OnTxtResample)
	EVT_TEXT_ENTER(wxID_SegmentCenterlineWidget_Smooth, CArteryModellingSegmentCenterlineWidgetUI::OnTxtSmooth)
    // end wxGlade
END_EVENT_TABLE();



void CArteryModellingSegmentCenterlineWidgetUI::OnBtnSelectArtery(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnBtnSelectArtery) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnBtnGetHelperPoint(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnBtnGetHelperPoint) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnBtnSegment(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnBtnSegment) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnBtnDummySegment(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnBtnDummySegment) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnTxtFrameNum(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnTxtFrameNum) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnTxtBranchNum(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnTxtBranchNum) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnBtnMerge(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnBtnMerge) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnBtnDeleteSegment(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnBtnDeleteSegment) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnChkAdvancedOpt(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnChkAdvancedOpt) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnBtnSampleRadius(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnBtnSampleRadius) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnTxtStartRadius(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnTxtStartRadius) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnTxtEndRadius(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnTxtEndRadius) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnTxtResample(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnTxtResample) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void CArteryModellingSegmentCenterlineWidgetUI::OnTxtSmooth(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (CArteryModellingSegmentCenterlineWidgetUI::OnTxtSmooth) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

// wxGlade: add CArteryModellingSegmentCenterlineWidgetUI event handlers


void CArteryModellingSegmentCenterlineWidgetUI::set_properties()
{
    // begin wxGlade: CArteryModellingSegmentCenterlineWidgetUI::set_properties
    SetScrollRate(10, 10);
	m_txtCardiacPhase->SetEditable(false);
	m_btnSelectArtery->SetBackgroundColour(wxColour(255, 215, 0)); // gold
	m_btnHelperPoint->SetBackgroundColour(wxColour(255, 215, 0)); // gold
	m_btnSegment->SetBackgroundColour(wxColour(221,160,221)); // plum
	m_btnDummySegment->SetBackgroundColour(wxColour(230,230,250)); // lavender
	m_btnDeleteSegment->SetBackgroundColour(wxColour(240,128,128)); // light coral
	m_btnMerge->SetBackgroundColour(wxColour(64, 216, 128)); // green
	m_btnSelectArtery->SetToolTip("Select the Start-End Points or Cancel the selection");
	m_btnHelperPoint->SetToolTip("Display the helper point on the Segment-1 closest to the mouse pointer");
	m_btnSegment->SetToolTip("Segments the centerline based on the input Start-End points");
	m_btnDummySegment->SetToolTip("Adds a dummy centerline segment to the 'Centerline Set' data");
	m_txtFrameNum->SetToolTip("Type in image frame number (0 to n)");
	m_txtBranchNum->SetToolTip("Type in artery branch number (0 to n)");
    m_btnMerge->SetToolTip("Segment-2 will be merged into Segment-1");
	m_btnDeleteSegment->SetToolTip("Deletes the last segmented centerline data from the 'Centerline Set'");
	m_btnSampleRad->SetToolTip("Get the sample radius at Start-End points");
	// end wxGlade
}


void CArteryModellingSegmentCenterlineWidgetUI::do_layout()
{
    // begin wxGlade: CArteryModellingSegmentCenterlineWidgetUI::do_layout
    wxBoxSizer* GlobalSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    
	wxBoxSizer* sizer_11 = new wxBoxSizer(wxHORIZONTAL);
	sizer_11->Add(m_stxCardiacPhase, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_11->Add(10, 20, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_11->Add(m_txtCardiacPhase, 0, wxALIGN_CENTER_VERTICAL, 0);

	wxBoxSizer* sizer_12 = new wxBoxSizer(wxHORIZONTAL);
	sizer_12->Add(m_btnSelectArtery, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_12->Add(10, 20, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_12->Add(m_btnHelperPoint, 0, wxALIGN_CENTER_VERTICAL, 0);

	wxBoxSizer* sizer_13 = new wxBoxSizer(wxHORIZONTAL);
	sizer_13->Add(m_stxFrameNum, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_13->Add(m_txtFrameNum, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_13->Add(20, 20, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_13->Add(m_stxBranchNum, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_13->Add(m_txtBranchNum, 0, wxALIGN_CENTER_VERTICAL, 0);
	
	wxBoxSizer* sizer_14 = new wxBoxSizer(wxHORIZONTAL);
	sizer_14->Add(m_btnSegment, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_14->Add(10, 20, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_14->Add(m_btnDummySegment, 0, wxALIGN_CENTER_VERTICAL, 0);

	wxBoxSizer* sizer_15 = new wxBoxSizer(wxHORIZONTAL);
	sizer_15->Add(m_btnMerge, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_15->Add(10, 20, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_15->Add(m_btnDeleteSegment, 0, wxALIGN_CENTER_VERTICAL, 0);

	sizer_1->Add(20, 20, 0, wxALIGN_LEFT, 0);
	sizer_1->Add(sizer_11, 0, wxTOP|wxBOTTOM|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_1->Add(20, 20, 0, wxALIGN_LEFT, 0);
	sizer_1->Add(sizer_12, 0, wxTOP|wxBOTTOM|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_1->Add(20, 20, 0, wxALIGN_LEFT, 0);
	sizer_1->Add(sizer_13, 0, wxEXPAND|wxADJUST_MINSIZE, 0);
	sizer_1->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sizer_1->Add(sizer_14, 0, wxTOP|wxBOTTOM|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 0);
	sizer_1->Add(20, 20, 0, wxALIGN_LEFT, 0);
	sizer_1->Add(sizer_15, 0, wxTOP|wxBOTTOM|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 0);

	wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
	
	wxBoxSizer* sizer_21 = new wxBoxSizer(wxHORIZONTAL);
	sizer_21->Add(m_stxStartRad, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_21->Add(m_txtStartRad, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_21->Add(20, 20, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_21->Add(m_stxEndRad, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_21->Add(m_txtEndRad, 0, wxALIGN_CENTER_VERTICAL, 0);

	wxBoxSizer* sizer_22 = new wxBoxSizer(wxHORIZONTAL);
	sizer_22->Add(m_stxResample, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_22->Add(m_txtResample, 0, wxALIGN_CENTER_VERTICAL, 0);

	wxBoxSizer* sizer_23 = new wxBoxSizer(wxHORIZONTAL);
	sizer_23->Add(m_stxSmooth, 0, wxALIGN_CENTER_VERTICAL, 0);
	sizer_23->Add(m_txtSmooth, 0, wxALIGN_CENTER_VERTICAL, 0);

	sbx_sizer = new wxStaticBoxSizer(m_sbxAdvancedOpt, wxVERTICAL);
	sbx_sizer->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbx_sizer->Add(sizer_21, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	sbx_sizer->Add(20, 10, 0, wxALIGN_LEFT, 0);
	sbx_sizer->Add(m_btnSampleRad, 0, wxTOP|wxBOTTOM|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 0);
	sbx_sizer->Add(20, 20, 0, wxALIGN_LEFT, 0);
	sbx_sizer->Add(sizer_22, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	sbx_sizer->Add(20, 20, 0, wxALIGN_LEFT, 0);
	sbx_sizer->Add(sizer_23, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);

	sizer_2->Add(20, 20, 0, wxALIGN_LEFT, 0);
	sizer_2->Add(m_cbxAdvancedOpt, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);
	sizer_2->Add(sbx_sizer, 0, wxALIGN_LEFT|wxADJUST_MINSIZE, 0);

    GlobalSizer->Add(sizer_1, 0, wxALIGN_CENTER_HORIZONTAL, 0);
	GlobalSizer->Add(sizer_2, 0, wxALIGN_LEFT, 0);
    SetSizer(GlobalSizer);
    GlobalSizer->Fit(this);
    // end wxGlade
}
