/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#include <wx/wx.h>
#include <wx/image.h>

#ifndef PatientInfoPluginPatientWorkingAreaWidgetUI_H
#define PatientInfoPluginPatientWorkingAreaWidgetUI_H

// begin wxGlade: ::dependencies
#include <wx/treectrl.h>
#include <wx/dataview.h>
#include <wx/datectrl.h>
#include "wx/treelistctrl.h"
// end wxGlade

#include "DicomTree.h"

// begin wxGlade: ::extracode
#define wxID_PatientWorkingAreaWidget_AdvancedOpt wxID("wxID_PatientWorkingAreaWidget_AdvancedOpt")
#define wxID_wxID_PatientWorkingAreaWidget_DICOM_DATA_TREE wxID("wxID_wxID_PatientWorkingAreaWidget_DICOM_DATA_TREE")
// end wxGlade


/**
\brief The parent class PatientInfoPluginPatientWorkingAreaWidgetUI contains all the code for 
buttons, labels, layouts, etc. for interacting with patientInfoPlugin::PatientWorkingAreaProcessor.

\ingroup PatientInfoPlugin
\author Vignesh Rammohan
\date 17 August 2015
*/
class PatientInfoPluginPatientWorkingAreaWidgetUI: public wxScrolledWindow/*wxPanel*/ {
public:
    // begin wxGlade: PatientInfoPluginPatientWorkingAreaWidgetUI::ids
    // end wxGlade

    PatientInfoPluginPatientWorkingAreaWidgetUI(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
    // begin wxGlade: PatientInfoPluginPatientWorkingAreaWidgetUI::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: PatientInfoPluginPatientWorkingAreaWidgetUI::attributes
	wxStaticBox* m_sbxDicomTree;
	DicomPlugin::DICOMTree* m_treeDicomView;

	wxNotebook* m_patients;
	// Patient 1
	wxPanel* m_patient1;
	wxStaticText* m_lblName1;
	wxTextCtrl* m_txtName1;
	wxStaticText* m_lblPatID1;
	wxTextCtrl* m_txtPatID1;
	wxStaticText* m_lblDOB1;
	wxTextCtrl* m_txtDOB1;
	wxStaticText* m_lblLength1;
	wxTextCtrl* m_txtLength1;
	wxStaticText* m_lblMeter1;
	wxStaticText* m_lblGender1;
	wxTextCtrl* m_txtGender1;
	wxStaticText* m_lblAge1;
	wxTextCtrl* m_txtAge1;
	wxStaticText* m_lblYears1;
	wxStaticText* m_lblWeight1;
	wxTextCtrl* m_txtWeight1;
	wxStaticText* m_lblKg1;
	wxStaticText* m_lblModality1;
	wxTextCtrl* m_txtModality1;
	wxStaticText* m_lblStudyDescr1;
	wxTextCtrl* m_txtStudyDescr1;
	wxStaticText* m_lblStudyDate1;
	wxTextCtrl* m_txtStudyDate1;
	wxStaticText* m_lblManufacturer1;
	wxTextCtrl* m_txtManufacturer1;
	wxStaticText* m_lblDisorder1;
	wxTextCtrl* m_txtDisorder1;
	wxStaticText* m_lblBloodPress1;
	wxTextCtrl* m_txtBloodPress1;
	wxStaticText* m_lblMmhg1;
	wxStaticText* m_lblHeartRate1;
	wxTextCtrl* m_txtHeartRate1;
	wxStaticText* m_lblBpm1;
	wxStaticText* m_lblNamePerf1;
	wxTextCtrl* m_txtNamePerf1;
	wxStaticText* m_lblFunctionPerf1;
	wxChoice* m_chcFunctionPerf1;
	wxStaticText* m_lblAnalysisDate1;
	wxDatePickerCtrl* m_dtpAnalysisDate1;
	wxCheckBox* m_cbxAdvanced1;

	// Patient 2
	wxPanel* m_patient2;
	wxStaticText* m_lblName2;
	wxTextCtrl* m_txtName2;
	wxStaticText* m_lblPatID2;
	wxTextCtrl* m_txtPatID2;
	wxStaticText* m_lblDOB2;
	wxTextCtrl* m_txtDOB2;
	wxStaticText* m_lblLength2;
	wxTextCtrl* m_txtLength2;
	wxStaticText* m_lblMeter2;
	wxStaticText* m_lblGender2;
	wxTextCtrl* m_txtGender2;
	wxStaticText* m_lblAge2;
	wxTextCtrl* m_txtAge2;
	wxStaticText* m_lblYears2;
	wxStaticText* m_lblWeight2;
	wxTextCtrl* m_txtWeight2;
	wxStaticText* m_lblKg2;
	wxStaticText* m_lblModality2;
	wxTextCtrl* m_txtModality2;
	wxStaticText* m_lblStudyDescr2;
	wxTextCtrl* m_txtStudyDescr2;
	wxStaticText* m_lblStudyDate2;
	wxTextCtrl* m_txtStudyDate2;
	wxStaticText* m_lblManufacturer2;
	wxTextCtrl* m_txtManufacturer2;
	wxStaticText* m_lblDisorder2;
	wxTextCtrl* m_txtDisorder2;
	wxStaticText* m_lblBloodPress2;
	wxTextCtrl* m_txtBloodPress2;
	wxStaticText* m_lblMmhg2;
	wxStaticText* m_lblHeartRate2;
	wxTextCtrl* m_txtHeartRate2;
	wxStaticText* m_lblBpm2;
	wxStaticText* m_lblNamePerf2;
	wxTextCtrl* m_txtNamePerf2;
	wxStaticText* m_lblFunctionPerf2;
	wxChoice* m_chcFunctionPerf2;
	wxStaticText* m_lblAnalysisDate2;
	wxDatePickerCtrl* m_dtpAnalysisDate2;
	wxCheckBox* m_cbxAdvanced2;

	// Patient 3
	wxPanel* m_patient3;
	wxStaticText* m_lblName3;
	wxTextCtrl* m_txtName3;
	wxStaticText* m_lblPatID3;
	wxTextCtrl* m_txtPatID3;
	wxStaticText* m_lblDOB3;
	wxTextCtrl* m_txtDOB3;
	wxStaticText* m_lblLength3;
	wxTextCtrl* m_txtLength3;
	wxStaticText* m_lblMeter3;
	wxStaticText* m_lblGender3;
	wxTextCtrl* m_txtGender3;
	wxStaticText* m_lblAge3;
	wxTextCtrl* m_txtAge3;
	wxStaticText* m_lblYears3;
	wxStaticText* m_lblWeight3;
	wxTextCtrl* m_txtWeight3;
	wxStaticText* m_lblKg3;
	wxStaticText* m_lblModality3;
	wxTextCtrl* m_txtModality3;
	wxStaticText* m_lblStudyDescr3;
	wxTextCtrl* m_txtStudyDescr3;
	wxStaticText* m_lblStudyDate3;
	wxTextCtrl* m_txtStudyDate3;
	wxStaticText* m_lblManufacturer3;
	wxTextCtrl* m_txtManufacturer3;
	wxStaticText* m_lblDisorder3;
	wxTextCtrl* m_txtDisorder3;
	wxStaticText* m_lblBloodPress3;
	wxTextCtrl* m_txtBloodPress3;
	wxStaticText* m_lblMmhg3;
	wxStaticText* m_lblHeartRate3;
	wxTextCtrl* m_txtHeartRate3;
	wxStaticText* m_lblBpm3;
	wxStaticText* m_lblNamePerf3;
	wxTextCtrl* m_txtNamePerf3;
	wxStaticText* m_lblFunctionPerf3;
	wxChoice* m_chcFunctionPerf3;
	wxStaticText* m_lblAnalysisDate3;
	wxDatePickerCtrl* m_dtpAnalysisDate3;
	wxCheckBox* m_cbxAdvanced3;

	// Advanced screen
	wxNotebook* m_additionalInformation;
	wxPanel* m_pnlSerie1;
	//wxDataViewCtrl* m_dvcSerie1;
	wxTreeListCtrl* m_TreeListCtrl;
	wxPanel* m_pnlSerie2;
	wxDataViewCtrl* m_dvcSerie2;
	wxPanel* m_pnlSerie3;
	wxDataViewCtrl* m_dvcSerie3;
	wxPanel* m_pnlSerie4;
	wxDataViewCtrl* m_dvcSerie4;
	wxPanel* m_pnlSerie5;
	wxDataViewCtrl* m_dvcSerie5;


	// sizers
	wxBoxSizer* GlobalSizer;
	wxStaticBoxSizer* sbSizerDicomTree;
	wxBoxSizer* bSizerMainWindow;
	// Patient 1
	wxBoxSizer* bSizerPatient1;
	wxStaticBox* sbxPatientInfo1;
	wxStaticBoxSizer* sbSizerPatientInfo1;
	wxBoxSizer* bSizerPatientInfoLeft1;
	wxBoxSizer* bSizerName1;
	wxBoxSizer* bSizerPatID1;
	wxBoxSizer* bSizerDOB1;
	wxBoxSizer* bSizerLenght1;
	wxBoxSizer* bSizerPatientInfoRight1;
	wxBoxSizer* bSizerSpacer1;
	wxBoxSizer* bSizerGender1;
	wxBoxSizer* bSizerAge1;
	wxBoxSizer* bSizerWeight1;
	wxStaticBox* sbxScanInfo1;
	wxStaticBoxSizer* sbSizerScanInfo1;
	wxBoxSizer* bSizerScanInfo1;
	wxBoxSizer* bSizerModality1;
	wxBoxSizer* bSizerStudyDescr1;
	wxBoxSizer* bSizerStudyDate1;
	wxBoxSizer* bSizerManufacturer1;
	wxStaticBox* sbxCardiacInfo1;
	wxStaticBoxSizer* sbSizerCardiacInfo1;
	wxBoxSizer* bSizerCardiacInfo1;
	wxBoxSizer* bSizerDisorder1; 
	wxBoxSizer* bSizerBloodPressure1;
	wxBoxSizer* bSizerHeartRate1;
	wxStaticBox* sbxPerformerInfo1;
	wxStaticBoxSizer* sbSizerPerformerInfo1;
	wxBoxSizer* bSizerPerformerInfo1;
	wxBoxSizer* bSizerNamePerf1;
	wxBoxSizer* bSizerFunctionPerf1;
	wxBoxSizer* bSizerAnalysisDate1;
	wxBoxSizer* bSizerAdvanced1;
	// Patient 2
	wxBoxSizer* bSizerPatient2;
	wxStaticBox* sbxPatientInfo2;
	wxStaticBoxSizer* sbSizerPatientInfo2;
	wxBoxSizer* bSizerPatientInfoLeft2;
	wxBoxSizer* bSizerName2;
	wxBoxSizer* bSizerPatID2;
	wxBoxSizer* bSizerDOB2;
	wxBoxSizer* bSizerLenght2;
	wxBoxSizer* bSizerPatientInfoRight2;
	wxBoxSizer* bSizerSpacer2;
	wxBoxSizer* bSizerGender2;
	wxBoxSizer* bSizerAge2;
	wxBoxSizer* bSizerWeight2;
	wxStaticBox* sbxScanInfo2;
	wxStaticBoxSizer* sbSizerScanInfo2;
	wxBoxSizer* bSizerScanInfo2;
	wxBoxSizer* bSizerModality2;
	wxBoxSizer* bSizerStudyDescr2;
	wxBoxSizer* bSizerStudyDate2;
	wxBoxSizer* bSizerManufacturer2;
	wxStaticBox* sbxCardiacInfo2;
	wxStaticBoxSizer* sbSizerCardiacInfo2;
	wxBoxSizer* bSizerCardiacInfo2;
	wxBoxSizer* bSizerDisorder2; 
	wxBoxSizer* bSizerBloodPressure2;
	wxBoxSizer* bSizerHeartRate2;
	wxStaticBox* sbxPerformerInfo2;
	wxStaticBoxSizer* sbSizerPerformerInfo2;
	wxBoxSizer* bSizerPerformerInfo2;
	wxBoxSizer* bSizerNamePerf2;
	wxBoxSizer* bSizerFunctionPerf2;
	wxBoxSizer* bSizerAnalysisDate2;
	wxBoxSizer* bSizerAdvanced2;
	// Patient 3
	wxBoxSizer* bSizerPatient3;
	wxStaticBox* sbxPatientInfo3;
	wxStaticBoxSizer* sbSizerPatientInfo3;
	wxBoxSizer* bSizerPatientInfoLeft3;
	wxBoxSizer* bSizerName3;
	wxBoxSizer* bSizerPatID3;
	wxBoxSizer* bSizerDOB3;
	wxBoxSizer* bSizerLenght3;
	wxBoxSizer* bSizerPatientInfoRight3;
	wxBoxSizer* bSizerSpacer3;
	wxBoxSizer* bSizerGender3;
	wxBoxSizer* bSizerAge3;
	wxBoxSizer* bSizerWeight3;
	wxStaticBox* sbxScanInfo3;
	wxStaticBoxSizer* sbSizerScanInfo3;
	wxBoxSizer* bSizerScanInfo3;
	wxBoxSizer* bSizerModality3;
	wxBoxSizer* bSizerStudyDescr3;
	wxBoxSizer* bSizerStudyDate3;
	wxBoxSizer* bSizerManufacturer3;
	wxStaticBox* sbxCardiacInfo3;
	wxStaticBoxSizer* sbSizerCardiacInfo3;
	wxBoxSizer* bSizerCardiacInfo3;
	wxBoxSizer* bSizerDisorder3; 
	wxBoxSizer* bSizerBloodPressure3;
	wxBoxSizer* bSizerHeartRate3; 
	wxStaticBox* sbxPerformerInfo3;
	wxStaticBoxSizer* sbSizerPerformerInfo3;
	wxBoxSizer* bSizerPerformerInfo3;
	wxBoxSizer* bSizerNamePerf3;
	wxBoxSizer* bSizerFunctionPerf3;
	wxBoxSizer* bSizerAnalysisDate3;
	wxBoxSizer* bSizerAdvanced3;
	// Advanced box
	wxStaticBox* sbxAdditonalInfo1;
	wxStaticBoxSizer* sbSizerAdditonalInfo1;
	wxStaticBox* sbxAdditonalInfo2;
	wxStaticBoxSizer* sbSizerAdditonalInfo2;
	wxStaticBox* sbxAdditonalInfo3;
	wxStaticBoxSizer* sbSizerAdditonalInfo3;
	wxBoxSizer* bSizerSerie1;
	wxBoxSizer* bSizerSerie2;
	wxBoxSizer* bSizerSerie3;
	wxBoxSizer* bSizerSerie4;
	wxBoxSizer* bSizerSerie5;

    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
	virtual void OnTreeSelChanged(wxTreeEvent &event); // wxGlade: <event_handler>
	virtual void OnChkAdvancedOpt(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


#endif // PatientInfoPluginPatientWorkingAreaWidgetUI_H
