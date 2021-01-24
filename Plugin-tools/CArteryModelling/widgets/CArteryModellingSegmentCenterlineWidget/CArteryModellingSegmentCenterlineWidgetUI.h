/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/


#include <wx/wx.h>
#include <wx/image.h>

#ifndef CArteryModellingSegmentCenterlineWidgetUI_H
#define CArteryModellingSegmentCenterlineWidgetUI_H

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode
#define wxID_SegmentCenterlineWidget_SelectArtery wxID("wxID_SegmentCenterlineWidget_SelectArtery")
#define wxID_SegmentCenterlineWidget_HelperPoint wxID("wxID_SegmentCenterlineWidget_HelperPoint")
#define wxID_SegmentCenterlineWidget_Segment wxID("wxID_SegmentCenterlineWidget_Segment")
#define wxID_SegmentCenterlineWidget_DummySegment wxID("wxID_SegmentCenterlineWidget_DummySegment")
#define wxID_SegmentCenterlineWidget_FrameNum wxID("wxID_SegmentCenterlineWidget_FrameNum")
#define wxID_SegmentCenterlineWidget_BranchNum wxID("wxID_SegmentCenterlineWidget_BranchNum")
#define wxID_SegmentCenterlineWidget_Merge wxID("wxID_SegmentCenterlineWidget_Merge")
#define wxID_SegmentCenterlineWidget_DeleteSegment wxID("wxID_SegmentCenterlineWidget_DeleteSegment")
#define wxID_SegmentCenterlineWidget_AdvancedOpt wxID("wxID_SegmentCenterlineWidget_AdvancedOpt")
#define wxID_SegmentCenterlineWidget_StartRad wxID("wxID_SegmentCenterlineWidget_StartRad")
#define wxID_SegmentCenterlineWidget_EndRad wxID("wxID_SegmentCenterlineWidget_EndRad")
#define wxID_SegmentCenterlineWidget_SampleRad wxID("wxID_SegmentCenterlineWidget_SampleRad")
#define wxID_SegmentCenterlineWidget_Resample wxID("wxID_SegmentCenterlineWidget_Resample")
#define wxID_SegmentCenterlineWidget_Smooth wxID("wxID_SegmentCenterlineWidget_Smooth")
// end wxGlade

/**
\brief The parent class CArteryModellingSegmentCenterlineWidgetUI contains all the code for 
buttons, labels, layouts, etc. for interacting with coronaryArteryModeling::SegmentCenterlineProcessor.

\ingroup CoronaryArteryModeling
\author Vignesh Rammohan
\date 30 July 2015
*/
class CArteryModellingSegmentCenterlineWidgetUI: public wxScrolledWindow {
public:
    // begin wxGlade: CArteryModellingSegmentCenterlineWidgetUI::ids
    // end wxGlade

    CArteryModellingSegmentCenterlineWidgetUI(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
    // begin wxGlade: CArteryModellingSegmentCenterlineWidgetUI::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: CArteryModellingSegmentCenterlineWidgetUI::attributes
	wxStaticText* m_stxCardiacPhase;
	wxTextCtrl* m_txtCardiacPhase;
	wxToggleButton* m_btnSelectArtery;
	wxButton* m_btnHelperPoint;
	wxButton* m_btnSegment;
	wxButton* m_btnDummySegment;

	wxStaticText* m_stxFrameNum;
	wxTextCtrl* m_txtFrameNum;
	wxStaticText* m_stxBranchNum;
	wxTextCtrl* m_txtBranchNum;
	wxButton* m_btnMerge;
	wxButton* m_btnDeleteSegment;

	wxCheckBox* m_cbxAdvancedOpt;
	wxStaticBox* m_sbxAdvancedOpt;
	wxStaticText* m_stxStartRad;
	wxTextCtrl* m_txtStartRad;
	wxStaticText* m_stxEndRad;
	wxTextCtrl* m_txtEndRad;
	wxButton* m_btnSampleRad;
	wxStaticText* m_stxResample;
	wxTextCtrl* m_txtResample;
	wxStaticText* m_stxSmooth;
	wxTextCtrl* m_txtSmooth;

	wxStaticBoxSizer* sbx_sizer;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
	virtual void OnBtnSelectArtery(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnBtnGetHelperPoint(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnBtnSegment(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnBtnDummySegment(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtFrameNum(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtBranchNum(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnBtnMerge(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnBtnDeleteSegment(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnChkAdvancedOpt(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnBtnSampleRadius(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtStartRadius(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtEndRadius(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtResample(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtSmooth(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


#endif // CArteryModellingSegmentCenterlineWidgetUI_H
