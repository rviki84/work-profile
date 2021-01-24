/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/


#include <wx/wx.h>
#include <wx/image.h>

#ifndef CArteryModellingLumenReconstructionWidgetUI_H
#define CArteryModellingLumenReconstructionWidgetUI_H

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode
#define wxID_LumenReconstructionWidget_ComputeSurface wxID("wxID_LumenReconstructionWidget_ComputeSurface")
#define wxID_LumenReconstructionWidget_AdvancedOpt wxID("wxID_LumenReconstructionWidget_AdvancedOpt")
#define wxID_LumenReconstructionWidget_OutlierWeight wxID("wxID_LumenReconstructionWidget_OutlierWeight")
#define wxID_LumenReconstructionWidget_NoOfItersCPD wxID("wxID_LumenReconstructionWidget_NoOfItersCPD")
#define wxID_LumenReconstructionWidget_Tolerance wxID("wxID_LumenReconstructionWidget_Tolerance")
#define wxID_LumenReconstructionWidget_EpiDistThres wxID("wxID_LumenReconstructionWidget_EpiDistThres")
#define wxID_LumenReconstructionWidget_SkipDist wxID("wxID_LumenReconstructionWidget_SkipDist")
#define wxID_LumenReconstructionWidget_SkipCost wxID("wxID_LumenReconstructionWidget_SkipCost")
#define wxID_LumenReconstructionWidget_LengthCost wxID("wxID_LumenReconstructionWidget_LengthCost")
#define wxID_LumenReconstructionWidget_NoOfItersOSB wxID("wxID_LumenReconstructionWidget_NoOfItersOSB")
#define wxID_LumenReconstructionWidget_ResLevel wxID("wxID_LumenReconstructionWidget_ResLevel")
#define wxID_LumenReconstructionWidget_ReconDepth wxID("wxID_LumenReconstructionWidget_ReconDepth")
#define wxID_LumenReconstructionWidget_PointWt wxID("wxID_LumenReconstructionWidget_PointWt")
// end wxGlade

/**
\brief The parent class CArteryModellingLumenReconstructionWidgetUI contains all the code for 
buttons, labels, layouts, etc. for interacting with cam::LumenReconstructionProcessor.

\ingroup CoronaryArteryModeling
\author Vignesh Rammohan
\date 10 November 2015
*/
class CArteryModellingLumenReconstructionWidgetUI: public wxScrolledWindow {
public:
    // begin wxGlade: CArteryModellingLumenReconstructionWidgetUI::ids
    // end wxGlade

    CArteryModellingLumenReconstructionWidgetUI(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
    // begin wxGlade: CArteryModellingLumenReconstructionWidgetUI::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: CArteryModellingLumenReconstructionWidgetUI::attributes
	
	wxButton* m_btnComputeSurface;
	wxCheckBox* m_cbxAdvancedOpt;

	wxStaticBox* m_sbx3DRecon; //3D Reconstruction of Centerlines
	// Registration Parameters
	wxStaticBox* m_sbxRegistration;
	wxStaticText* m_stxOutlierWt;
	wxTextCtrl* m_txtOutlierWt; //Determines CPD's robustness to outlier points (values between 0 and 1, 0.1 default)
	wxStaticText* m_stxNoOfItersCPD;
	wxTextCtrl* m_txtNoOfItersCPD; //Iteration number for CPD algorithm (300 default)
	wxStaticText* m_stxTolerance;
	wxTextCtrl* m_txtTolerance; //Stopping criteria for CPD (small number, 1.0e-8 default)

	// Optimal Subsequence Bijection (OSB) Parameters
	wxStaticBox* m_sbxOSB;
	wxStaticText* m_stxEpiDistThres;
	wxTextCtrl* m_txtEpiDistThres; //determines the feasible pairs/triplets for OSB algorithm (values between 0 and N, N=20 default, N=50 max)
	wxStaticText* m_stxEpiDistNote;
	wxStaticText* m_stxSkipDist;
	wxTextCtrl* m_txtSkipDist; //the max number of allowed skips for the algorithm (values between 0 and N, N=5 default, N=10 max)
	wxStaticText* m_stxSkipCost;
	wxTextCtrl* m_txtSkipCost; //cost of the skip (values between 0 and N, N=0.01 default)
	wxStaticText* m_stxLengthCost;
	wxTextCtrl* m_txtLengthCost; //cost on the length of centerline (values between 0 and N, N=20 default)
	wxStaticText* m_stxNoOfItersOSB;
	wxTextCtrl* m_txtNoOfItersOSB; //max number of iterations for matching-reconstruction cycle
	wxStaticText* m_stxNoOfItersNote;
	wxStaticText* m_stxResLevel;
	wxTextCtrl* m_txtResLevel; //multi-resolution levels
	wxStaticText* m_stxResLevelNote;

	// Surface Reconstruction Parameters (Poisson Surface Reconstruction)
	wxStaticBox* m_sbxSurfRecon;
	wxStaticText* m_stxReconDepth;
	/* This integer is the maximum depth of the tree that will be used for surface reconstruction.
	Running at depth d corresponds to solving on a voxel grid whose resolution is no larger than 
	2^d x 2^d x 2^d. Note that since the reconstructor adapts the octree to the sampling density, 
	the specified reconstruction depth is only an upper bound.
	The default value for this parameter is 8. */
	wxTextCtrl* m_txtReconDepth; //<reconstruction depth>
	wxStaticText* m_stxPointWt;
	/* This floating point value specifies the importance that interpolation of the point samples is
	given in the formulation of the screened Poisson equation. The results of the original 
	(unscreened) Poisson Reconstruction can be obtained by setting this value to 0.
	The default value for this parameter is 4. */
	wxTextCtrl* m_txtPointWt; //<interpolation weight>

	wxStaticBoxSizer* sbxSizer3DRecon;
	wxStaticBoxSizer* sbxSizerRegistration;
	wxStaticBoxSizer* sbxSizerOSB;
	/*wxStaticBoxSizer* sbxSizerOthers;*/
	wxStaticBoxSizer* sbxSizerSurfRecon;

    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void OnBtnComputeSurface(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnChkAdvancedOpt(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtOutlierWeight(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtNoOfItersCPD(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtTolerance(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtEpiDistThres(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtSkipDist(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtSkipCost(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtLengthCost(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtNoOfItersOSB(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtResLevel(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtReconDepth(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnTxtPointWt(wxCommandEvent &event); // wxGlade: <event_handler>

}; // wxGlade: end class


#endif // CArteryModellingLumenReconstructionWidgetUI_H
