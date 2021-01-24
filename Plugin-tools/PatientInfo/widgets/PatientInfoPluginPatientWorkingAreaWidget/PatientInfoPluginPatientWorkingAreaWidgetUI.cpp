/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#include "PatientInfoPluginPatientWorkingAreaWidgetUI.h"

// begin wxGlade: ::extracode

// end wxGlade


PatientInfoPluginPatientWorkingAreaWidgetUI::PatientInfoPluginPatientWorkingAreaWidgetUI(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxScrolledWindow(parent, id, pos, size, wxTAB_TRAVERSAL)
{
    // begin wxGlade: PatientInfoPluginPatientWorkingAreaWidgetUI::PatientInfoPluginPatientWorkingAreaWidgetUI
	m_sbxDicomTree = new wxStaticBox(this, wxID_ANY, wxT("DICOM Tree Viewer"));
	m_treeDicomView = new DicomPlugin::DICOMTree(this, wxID_wxID_PatientWorkingAreaWidget_DICOM_DATA_TREE, wxDefaultPosition, wxDefaultSize
		, wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|wxTR_MULTIPLE|wxTR_MULTIPLE|wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER);

	m_patients = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_patient1 = new wxPanel( m_patients, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_patient2 = new wxPanel( m_patients, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_patient3 = new wxPanel( m_patients, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

	sbxPatientInfo1 = new wxStaticBox( m_patient1, wxID_ANY, wxT("Patient Information") );
	sbxScanInfo1 = new wxStaticBox( m_patient1, wxID_ANY, wxT("Scan Information") );
	sbxCardiacInfo1 = new wxStaticBox( m_patient1, wxID_ANY, wxT("Cardiac Information") );
	sbxPerformerInfo1 = new wxStaticBox( m_patient1, wxID_ANY, wxT("Performer Information") );
	sbxAdditonalInfo1 = new wxStaticBox( m_patient1, wxID_ANY, wxT("Additional Information") );
	sbSizerPatientInfo1 = new wxStaticBoxSizer( sbxPatientInfo1, wxHORIZONTAL );
	sbSizerScanInfo1 = new wxStaticBoxSizer( sbxScanInfo1, wxHORIZONTAL );
	sbSizerCardiacInfo1 = new wxStaticBoxSizer( sbxCardiacInfo1, wxHORIZONTAL );
	sbSizerPerformerInfo1 = new wxStaticBoxSizer( sbxPerformerInfo1, wxHORIZONTAL );
	sbSizerAdditonalInfo1 = new wxStaticBoxSizer( sbxAdditonalInfo1, wxHORIZONTAL );

	sbxPatientInfo2 = new wxStaticBox( m_patient2, wxID_ANY, wxT("Patient Information") );
	sbxScanInfo2 = new wxStaticBox( m_patient2, wxID_ANY, wxT("Scan Information") );
	sbxCardiacInfo2 = new wxStaticBox( m_patient2, wxID_ANY, wxT("Cardiac Information") );
	sbxPerformerInfo2 = new wxStaticBox( m_patient2, wxID_ANY, wxT("Performer Information") );
	//sbxAdditonalInfo2 = new wxStaticBox( m_patient2, wxID_ANY, wxT("Additional Information") );
	sbSizerPatientInfo2 = new wxStaticBoxSizer( sbxPatientInfo2, wxHORIZONTAL );
	sbSizerScanInfo2 = new wxStaticBoxSizer( sbxScanInfo2, wxHORIZONTAL );
	sbSizerCardiacInfo2 = new wxStaticBoxSizer( sbxCardiacInfo2, wxHORIZONTAL );
	sbSizerPerformerInfo2 = new wxStaticBoxSizer( sbxPerformerInfo2, wxHORIZONTAL );
	//sbSizerAdditonalInfo2 = new wxStaticBoxSizer( sbxAdditonalInfo2, wxHORIZONTAL );

	sbxPatientInfo3 = new wxStaticBox( m_patient3, wxID_ANY, wxT("Patient Information") );
	sbxScanInfo3 = new wxStaticBox( m_patient3, wxID_ANY, wxT("Scan Information") );
	sbxCardiacInfo3 = new wxStaticBox( m_patient3, wxID_ANY, wxT("Cardiac Information") );
	sbxPerformerInfo3 = new wxStaticBox( m_patient3, wxID_ANY, wxT("Performer Information") );
	//sbxAdditonalInfo3 = new wxStaticBox( m_patient3, wxID_ANY, wxT("Additional Information") );
	sbSizerPatientInfo3 = new wxStaticBoxSizer(sbxPatientInfo3, wxHORIZONTAL );
	sbSizerScanInfo3 = new wxStaticBoxSizer( sbxScanInfo3, wxHORIZONTAL );
	sbSizerCardiacInfo3 = new wxStaticBoxSizer( sbxCardiacInfo3, wxHORIZONTAL );
	sbSizerPerformerInfo3 = new wxStaticBoxSizer( sbxPerformerInfo3, wxHORIZONTAL );
	//sbSizerAdditonalInfo3 = new wxStaticBoxSizer( sbxAdditonalInfo3, wxHORIZONTAL );

	// Patient 1
	m_lblName1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtName1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPatID1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Patient ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtPatID1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDOB1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Date of birth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtDOB1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLength1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtLength1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMeter1 = new wxStaticText( m_patient1, wxID_ANY, wxT("meter"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblGender1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Gender"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtGender1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAge1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Age"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtAge1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblYears1 = new wxStaticText( m_patient1, wxID_ANY, wxT("years"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblWeight1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Weight"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtWeight1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblKg1 = new wxStaticText( m_patient1, wxID_ANY, wxT("kg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblModality1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Modality"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtModality1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblStudyDescr1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Study Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtStudyDescr1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblStudyDate1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Study date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtStudyDate1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblManufacturer1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Manufacturer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtManufacturer1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDisorder1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Disorder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtDisorder1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblBloodPress1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Blood pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtBloodPress1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMmhg1 = new wxStaticText( m_patient1, wxID_ANY, wxT("mmHg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblHeartRate1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Heart rate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtHeartRate1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblBpm1 = new wxStaticText( m_patient1, wxID_ANY, wxT("bpm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblNamePerf1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Name performer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtNamePerf1 = new wxTextCtrl( m_patient1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFunctionPerf1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Function performer"), wxDefaultPosition, wxDefaultSize, 0 );
	const wxString m_chcFunctionPerf1Choices[] = { wxT("Radiologist"), wxT("Specialist"), wxT("Radiographer"), wxT("Medical Student"), wxT("Medical Intern") };
	int m_chcFunctionPerf1NChoices = sizeof( m_chcFunctionPerf1Choices ) / sizeof( wxString ); 
	m_chcFunctionPerf1 = new wxChoice( m_patient1, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_chcFunctionPerf1NChoices, m_chcFunctionPerf1Choices, 0 );
	m_lblAnalysisDate1 = new wxStaticText( m_patient1, wxID_ANY, wxT("Analysis date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_dtpAnalysisDate1 = new wxDatePickerCtrl( m_patient1, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	m_cbxAdvanced1 = new wxCheckBox( m_patient1, wxID_PatientWorkingAreaWidget_AdvancedOpt, wxT("Advanced"), wxDefaultPosition, wxDefaultSize, 0 );

	// Patient 2
	m_lblName2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtName2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPatID2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Patient ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtPatID2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDOB2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Date of birth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtDOB2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLength2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtLength2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMeter2 = new wxStaticText( m_patient2, wxID_ANY, wxT("meter"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblGender2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Gender"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtGender2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAge2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Age"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtAge2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblYears2 = new wxStaticText( m_patient2, wxID_ANY, wxT("years"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblWeight2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Weight"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtWeight2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblKg2 = new wxStaticText( m_patient2, wxID_ANY, wxT("kg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblModality2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Modality"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtModality2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblStudyDescr2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Study Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtStudyDescr2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblStudyDate2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Study date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtStudyDate2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblManufacturer2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Manufacturer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtManufacturer2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDisorder2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Disorder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtDisorder2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblBloodPress2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Blood pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtBloodPress2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMmhg2 = new wxStaticText( m_patient2, wxID_ANY, wxT("mmHg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblHeartRate2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Heart rate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtHeartRate2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblBpm2 = new wxStaticText( m_patient2, wxID_ANY, wxT("bpm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblNamePerf2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Name performer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtNamePerf2 = new wxTextCtrl( m_patient2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFunctionPerf2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Function performer"), wxDefaultPosition, wxDefaultSize, 0 );
	const wxString m_chcFunctionPerf2Choices[] = { wxT("Radiologist"), wxT("Specialist"), wxT("Radiographer"), wxT("Medical Student"), wxT("Medical Intern") };
	int m_chcFunctionPerf2NChoices = sizeof( m_chcFunctionPerf2Choices ) / sizeof( wxString );
	m_chcFunctionPerf2 = new wxChoice( m_patient2, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_chcFunctionPerf2NChoices, m_chcFunctionPerf2Choices, 0 );
	m_lblAnalysisDate2 = new wxStaticText( m_patient2, wxID_ANY, wxT("Analysis date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_dtpAnalysisDate2 = new wxDatePickerCtrl( m_patient2, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	m_cbxAdvanced2 = new wxCheckBox( m_patient2, wxID_ANY, wxT("Advanced"), wxDefaultPosition, wxDefaultSize, 0 );

	// Patient 3
	m_lblName3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtName3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblPatID3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Patient ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtPatID3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDOB3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Date of birth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtDOB3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblLength3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtLength3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMeter3 = new wxStaticText( m_patient3, wxID_ANY, wxT("meter"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblGender3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Gender"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtGender3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblAge3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Age"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtAge3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblYears3 = new wxStaticText( m_patient3, wxID_ANY, wxT("years"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblWeight3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Weight"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtWeight3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblKg3 = new wxStaticText( m_patient3, wxID_ANY, wxT("kg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblModality3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Modality"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtModality3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblStudyDescr3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Study Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtStudyDescr3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblStudyDate3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Study date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtStudyDate3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblManufacturer3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Manufacturer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtManufacturer3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblDisorder3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Disorder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtDisorder3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblBloodPress3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Blood pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtBloodPress3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMmhg3 = new wxStaticText( m_patient3, wxID_ANY, wxT("mmHg"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblHeartRate3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Heart rate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtHeartRate3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblBpm3 = new wxStaticText( m_patient3, wxID_ANY, wxT("bpm"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblNamePerf3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Name performer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_txtNamePerf3 = new wxTextCtrl( m_patient3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_lblFunctionPerf3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Function performer"), wxDefaultPosition, wxDefaultSize, 0 );
	const wxString m_chcFunctionPerf3Choices[] = { wxT("Radiologist"), wxT("Specialist"), wxT("Radiographer"), wxT("Medical Student"), wxT("Medical Intern") };
	int m_chcFunctionPerf3NChoices = sizeof( m_chcFunctionPerf3Choices ) / sizeof( wxString );
	m_chcFunctionPerf3 = new wxChoice( m_patient3, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_chcFunctionPerf3NChoices, m_chcFunctionPerf3Choices, 0 );
	m_lblAnalysisDate3 = new wxStaticText( m_patient3, wxID_ANY, wxT("Analysis date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_dtpAnalysisDate3 = new wxDatePickerCtrl( m_patient3, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	m_cbxAdvanced3 = new wxCheckBox( m_patient3, wxID_ANY, wxT("Advanced"), wxDefaultPosition, wxDefaultSize, 0 );
	
	// Advanced screen
	m_additionalInformation = new wxNotebook( m_patient1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_pnlSerie1 = new wxPanel( m_additionalInformation, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	//m_dvcSerie1 = new wxDataViewCtrl( m_pnlSerie1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_pnlSerie2 = new wxPanel( m_additionalInformation, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_dvcSerie2 = new wxDataViewCtrl( m_pnlSerie2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_pnlSerie3 = new wxPanel( m_additionalInformation, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_dvcSerie3 = new wxDataViewCtrl( m_pnlSerie3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_pnlSerie4 = new wxPanel( m_additionalInformation, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_dvcSerie4 = new wxDataViewCtrl( m_pnlSerie4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_pnlSerie5 = new wxPanel( m_additionalInformation, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_dvcSerie5 = new wxDataViewCtrl( m_pnlSerie5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	m_TreeListCtrl = new wxTreeListCtrl(m_pnlSerie1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
    set_properties();
    do_layout();
    // end wxGlade
}


BEGIN_EVENT_TABLE(PatientInfoPluginPatientWorkingAreaWidgetUI, wxScrolledWindow)
    // begin wxGlade: PatientInfoPluginPatientWorkingAreaWidgetUI::event_table
	EVT_TREE_SEL_CHANGED(wxID_wxID_PatientWorkingAreaWidget_DICOM_DATA_TREE, PatientInfoPluginPatientWorkingAreaWidgetUI::OnTreeSelChanged)
	EVT_CHECKBOX(wxID_PatientWorkingAreaWidget_AdvancedOpt, PatientInfoPluginPatientWorkingAreaWidgetUI::OnChkAdvancedOpt)
    // end wxGlade
END_EVENT_TABLE();


void PatientInfoPluginPatientWorkingAreaWidgetUI::OnTreeSelChanged(wxTreeEvent &event)
{
    event.Skip();
    wxLogDebug(wxT("Event handler (PatientInfoPluginPatientWorkingAreaWidgetUI::OnTreeSelChanged) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

void PatientInfoPluginPatientWorkingAreaWidgetUI::OnChkAdvancedOpt(wxCommandEvent &event)
{
	event.Skip();
	wxLogDebug(wxT("Event handler (PatientInfoPluginPatientWorkingAreaWidgetUI::OnChkAdvancedOpt) not implemented yet")); //notify the user that he hasn't implemented the event handler yet
}

// wxGlade: add PatientInfoPluginPatientWorkingAreaWidgetUI event handlers


void PatientInfoPluginPatientWorkingAreaWidgetUI::set_properties()
{
    // begin wxGlade: PatientInfoPluginPatientWorkingAreaWidgetUI::set_properties
	SetScrollRate(20, 20);

	// Patient 1
	m_lblName1->Wrap( -1 );
	m_txtName1->Enable( false );
	m_lblPatID1->Wrap( -1 );
	m_txtPatID1->Enable( false );
	m_lblDOB1->Wrap( -1 );
	m_txtDOB1->Enable( false );
	m_lblLength1->Wrap( -1 );
	m_txtLength1->SetMaxLength( 4 ); 
	m_lblMeter1->Wrap( -1 );
	m_lblGender1->Wrap( -1 );
	m_txtGender1->Enable( false );
	m_lblAge1->Wrap( -1 );
	m_txtAge1->SetMaxLength( 3 ); 
	m_txtAge1->Enable( false );
	m_lblYears1->Wrap( -1 );
	m_lblWeight1->Wrap( -1 );
	m_txtWeight1->SetMaxLength( 3 ); 
	m_lblKg1->Wrap( -1 );
	m_lblModality1->Wrap( -1 );
	m_txtModality1->Enable( false );
	m_lblStudyDescr1->Wrap( -1 );
	m_txtStudyDescr1->Enable( false );
	m_lblStudyDate1->Wrap( -1 );
	m_txtStudyDate1->Enable( false );
	m_lblManufacturer1->Wrap( -1 );
	m_txtManufacturer1->Enable( false );
	m_lblDisorder1->Wrap( -1 );
	m_lblBloodPress1->Wrap( -1 );
	m_txtBloodPress1->SetMaxLength( 7 ); 
	m_lblMmhg1->Wrap( -1 );
	m_lblHeartRate1->Wrap( -1 );
	m_txtHeartRate1->SetMaxLength( 3 ); 
	m_lblBpm1->Wrap( -1 );
	m_lblNamePerf1->Wrap( -1 );
	m_lblFunctionPerf1->Wrap( -1 );
	m_chcFunctionPerf1->SetSelection( 0 );
	m_lblAnalysisDate1->Wrap( -1 );
	m_cbxAdvanced1->SetValue(false); 

	// Patient 2
	m_lblName2->Wrap( -1 );
	m_txtName2->Enable( false );
	m_lblPatID2->Wrap( -1 );
	m_txtPatID2->Enable( false );
	m_lblDOB2->Wrap( -1 );
	m_txtDOB2->Enable( false );
	m_lblLength2->Wrap( -1 );
	m_txtLength2->SetMaxLength( 4 ); 
	m_lblMeter2->Wrap( -1 );
	m_lblGender2->Wrap( -1 );
	m_txtGender2->Enable( false );
	m_lblAge2->Wrap( -1 );
	m_txtAge2->SetMaxLength( 3 ); 
	m_txtAge2->Enable( false );
	m_lblYears2->Wrap( -1 );
	m_lblWeight2->Wrap( -1 );
	m_txtWeight2->SetMaxLength( 3 ); 
	m_lblKg2->Wrap( -1 );
	m_lblModality2->Wrap( -1 );
	m_txtModality2->Enable( false );
	m_lblStudyDescr2->Wrap( -1 );
	m_txtStudyDescr2->Enable( false );
	m_lblStudyDate2->Wrap( -1 );
	m_txtStudyDate2->Enable( false );
	m_lblManufacturer2->Wrap( -1 );
	m_txtManufacturer2->Enable( false );
	m_lblDisorder2->Wrap( -1 );
	m_lblBloodPress2->Wrap( -1 );
	m_txtBloodPress2->SetMaxLength( 7 ); 
	m_lblMmhg2->Wrap( -1 );
	m_lblHeartRate2->Wrap( -1 );
	m_txtHeartRate2->SetMaxLength( 3 ); 
	m_lblBpm2->Wrap( -1 );
	m_lblNamePerf2->Wrap( -1 );
	m_lblFunctionPerf2->Wrap( -1 );
    m_chcFunctionPerf2->SetSelection( 0 );
	m_lblAnalysisDate2->Wrap( -1 );
	m_cbxAdvanced2->SetValue(false); 

	// Patient 3
	m_lblName3->Wrap( -1 );
	m_txtName3->Enable( false );
	m_lblPatID3->Wrap( -1 );
	m_txtPatID3->Enable( false );
	m_lblDOB3->Wrap( -1 );
	m_txtDOB3->Enable( false );
	m_lblLength3->Wrap( -1 );
	m_txtLength3->SetMaxLength( 4 ); 
	m_lblMeter3->Wrap( -1 );
	m_lblGender3->Wrap( -1 );
	m_txtGender3->Enable( false );
	m_lblAge3->Wrap( -1 );
	m_txtAge3->SetMaxLength( 3 ); 
	m_txtAge3->Enable( false );
	m_lblYears3->Wrap( -1 );
	m_lblWeight3->Wrap( -1 );
	m_txtWeight3->SetMaxLength( 3 ); 
	m_lblKg3->Wrap( -1 );
	m_lblModality3->Wrap( -1 );
	m_txtModality3->Enable( false );
	m_lblStudyDescr3->Wrap( -1 );
	m_txtStudyDescr3->Enable( false );
	m_lblStudyDate3->Wrap( -1 );
	m_txtStudyDate3->Enable( false );
	m_lblManufacturer3->Wrap( -1 );
	m_txtManufacturer3->Enable( false );
	m_lblDisorder3->Wrap( -1 );
	m_lblBloodPress3->Wrap( -1 );
	m_txtBloodPress3->SetMaxLength( 7 ); 
	m_lblMmhg3->Wrap( -1 );
	m_lblHeartRate3->Wrap( -1 );
	m_txtHeartRate3->SetMaxLength( 3 ); 
	m_lblBpm3->Wrap( -1 );
	m_lblNamePerf3->Wrap( -1 );
	m_lblFunctionPerf3->Wrap( -1 );
	m_chcFunctionPerf3->SetSelection( 0 );
	m_lblAnalysisDate3->Wrap( -1 );
	m_cbxAdvanced3->SetValue(false);

    // end wxGlade
}


void PatientInfoPluginPatientWorkingAreaWidgetUI::do_layout()
{
    // begin wxGlade: PatientInfoPluginPatientWorkingAreaWidgetUI::do_layout
    GlobalSizer = new wxBoxSizer(wxHORIZONTAL);
	sbSizerDicomTree = new wxStaticBoxSizer(m_sbxDicomTree, wxHORIZONTAL);
	sbSizerDicomTree->Add(m_treeDicomView, 2, wxALL|wxEXPAND, 0);

	// Patient 1
	bSizerPatient1 = new wxBoxSizer( wxVERTICAL );
	bSizerPatientInfoLeft1 = new wxBoxSizer( wxVERTICAL );
	bSizerName1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerPatID1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerDOB1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerLenght1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerPatientInfoRight1 = new wxBoxSizer( wxVERTICAL );
	bSizerSpacer1 = new wxBoxSizer( wxVERTICAL );
	bSizerGender1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerAge1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerWeight1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerScanInfo1 = new wxBoxSizer( wxVERTICAL );
	bSizerModality1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerStudyDescr1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerStudyDate1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerManufacturer1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerCardiacInfo1 = new wxBoxSizer( wxVERTICAL );
	bSizerDisorder1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerBloodPressure1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerHeartRate1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerPerformerInfo1 = new wxBoxSizer( wxVERTICAL );
	bSizerNamePerf1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerFunctionPerf1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerAnalysisDate1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerAdvanced1 = new wxBoxSizer( wxHORIZONTAL );

	bSizerName1->Add( m_lblName1, 1, wxEXPAND, 5 );
	bSizerName1->Add( m_txtName1, 1, wxALL, 5 );
	bSizerName1->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerPatientInfoLeft1->Add( bSizerName1, 1, wxEXPAND, 5 );
	bSizerPatID1->Add( m_lblPatID1, 1, wxEXPAND, 5 );
	bSizerPatID1->Add( m_txtPatID1, 1, wxALL, 5 );
	bSizerPatID1->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerPatientInfoLeft1->Add( bSizerPatID1, 1, wxEXPAND, 5 );
	bSizerDOB1->Add( m_lblDOB1, 1, wxEXPAND, 5 );
	bSizerDOB1->Add( m_txtDOB1, 1, wxALL, 5 );
	bSizerDOB1->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerPatientInfoLeft1->Add( bSizerDOB1, 1, wxEXPAND, 5 );
	bSizerLenght1->Add( m_lblLength1, 1, wxEXPAND, 5 );
	bSizerLenght1->Add( m_txtLength1, 1, wxALL, 5 );
	bSizerLenght1->Add( m_lblMeter1, 1, wxALL, 5 );
	bSizerPatientInfoLeft1->Add( bSizerLenght1, 1, wxEXPAND, 5 );
	sbSizerPatientInfo1->Add( bSizerPatientInfoLeft1, 0, wxALL|wxEXPAND, 5 );
	bSizerSpacer1->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerSpacer1->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerSpacer1->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerSpacer1->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerPatientInfoRight1->Add( bSizerSpacer1, 1, wxEXPAND, 5 );
	bSizerGender1->Add( m_lblGender1, 1, wxALL, 5 );
	bSizerGender1->Add( m_txtGender1, 1, wxALL, 5 );
	bSizerGender1->Add( 0, 0, 1, wxALL, 5 );
	bSizerPatientInfoRight1->Add( bSizerGender1, 1, wxEXPAND, 5 );
	bSizerAge1->Add( m_lblAge1, 1, wxALL, 5 );
	bSizerAge1->Add( m_txtAge1, 1, wxALL, 5 );
	bSizerAge1->Add( m_lblYears1, 1, wxALL, 5 );
	bSizerPatientInfoRight1->Add( bSizerAge1, 1, wxEXPAND, 5 );
	bSizerWeight1->Add( m_lblWeight1, 1, wxALL, 5 );
	bSizerWeight1->Add( m_txtWeight1, 1, wxALL, 5 );
	bSizerWeight1->Add( m_lblKg1, 1, wxALL, 5 );
	bSizerPatientInfoRight1->Add( bSizerWeight1, 1, wxEXPAND, 5 );
	sbSizerPatientInfo1->Add( bSizerPatientInfoRight1, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient1->Add( sbSizerPatientInfo1, 0, wxALL|wxEXPAND, 5 );

	bSizerModality1->Add( m_lblModality1, 1, wxALL, 5 );
	bSizerModality1->Add( 0, 0, 1, wxALL, 5 );
	bSizerModality1->Add( m_txtModality1, 1, wxALL, 5 );
	bSizerScanInfo1->Add( bSizerModality1, 1, wxEXPAND, 5 );
	bSizerStudyDescr1->Add( m_lblStudyDescr1, 1, wxALL, 5 );
	bSizerStudyDescr1->Add( 0, 0, 1, wxALL, 5 );
	bSizerStudyDescr1->Add( m_txtStudyDescr1, 1, wxALL, 5 );
	bSizerScanInfo1->Add( bSizerStudyDescr1, 1, wxEXPAND, 5 );
	bSizerStudyDate1->Add( m_lblStudyDate1, 1, wxALL, 5 );
	bSizerStudyDate1->Add( 0, 0, 1, wxALL, 5 );
	bSizerStudyDate1->Add( m_txtStudyDate1, 1, wxALL, 5 );
	bSizerScanInfo1->Add( bSizerStudyDate1, 1, wxEXPAND, 5 );
	bSizerManufacturer1->Add( m_lblManufacturer1, 1, wxALL, 5 );
	bSizerManufacturer1->Add( 0, 0, 1, wxALL, 5 );
	bSizerManufacturer1->Add( m_txtManufacturer1, 1, wxALL, 5 );
	bSizerScanInfo1->Add( bSizerManufacturer1, 1, wxEXPAND, 5 );
	sbSizerScanInfo1->Add( bSizerScanInfo1, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient1->Add( sbSizerScanInfo1, 0, wxALL|wxEXPAND, 5 );

	bSizerDisorder1->Add( m_lblDisorder1, 1, wxALL, 5 );
	bSizerDisorder1->Add( 0, 0, 1, wxALL, 5 );
	bSizerDisorder1->Add( m_txtDisorder1, 1, wxALL, 5 );
	bSizerDisorder1->Add( 0, 0, 1, wxALL, 5 );
	bSizerCardiacInfo1->Add( bSizerDisorder1, 1, wxEXPAND, 5 );
	bSizerBloodPressure1->Add( m_lblBloodPress1, 1, wxALL, 5 );
	bSizerBloodPressure1->Add( 0, 0, 1, wxALL, 5 );
	bSizerBloodPressure1->Add( m_txtBloodPress1, 1, wxALL, 5 );
	bSizerBloodPressure1->Add( m_lblMmhg1, 1, wxALL, 5 );
	bSizerCardiacInfo1->Add( bSizerBloodPressure1, 1, wxEXPAND, 5 );
	bSizerHeartRate1->Add( m_lblHeartRate1, 1, wxALL, 5 );
	bSizerHeartRate1->Add( 0, 0, 1, wxALL, 5 );
	bSizerHeartRate1->Add( m_txtHeartRate1, 1, wxALL, 5 );
	bSizerHeartRate1->Add( m_lblBpm1, 1, wxALL, 5 );
	bSizerCardiacInfo1->Add( bSizerHeartRate1, 1, wxEXPAND, 5 );
	sbSizerCardiacInfo1->Add( bSizerCardiacInfo1, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient1->Add( sbSizerCardiacInfo1, 0, wxALL|wxEXPAND, 5 );

	bSizerNamePerf1->Add( m_lblNamePerf1, 1, wxALL, 5 );
	bSizerNamePerf1->Add( 0, 0, 1, wxALL, 5 ); 
	bSizerNamePerf1->Add( m_txtNamePerf1, 1, wxALL, 5 );
	bSizerPerformerInfo1->Add( bSizerNamePerf1, 1, wxEXPAND, 5 );
	bSizerFunctionPerf1->Add( m_lblFunctionPerf1, 1, wxALL, 5 );
	bSizerFunctionPerf1->Add( 0, 0, 1, wxALL, 5 );
	bSizerFunctionPerf1->Add( m_chcFunctionPerf1, 1, wxALL, 5 );
	bSizerPerformerInfo1->Add( bSizerFunctionPerf1, 1, wxEXPAND, 5 );
	bSizerAnalysisDate1->Add( m_lblAnalysisDate1, 1, wxALL, 5 );
	bSizerAnalysisDate1->Add( 0, 0, 1, wxALL, 5 );
	bSizerAnalysisDate1->Add( m_dtpAnalysisDate1, 1, wxALL, 5 );
	bSizerPerformerInfo1->Add( bSizerAnalysisDate1, 1, wxEXPAND, 5 );
	sbSizerPerformerInfo1->Add( bSizerPerformerInfo1, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient1->Add( sbSizerPerformerInfo1, 0, wxALL|wxEXPAND, 5 );
	bSizerAdvanced1->Add( m_cbxAdvanced1, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient1->Add( bSizerAdvanced1, 0,  wxEXPAND, 5 );
	// Advanced box
	bSizerSerie1 = new wxBoxSizer( wxHORIZONTAL );
	bSizerSerie2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerSerie3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerSerie4 = new wxBoxSizer( wxHORIZONTAL );
	bSizerSerie5 = new wxBoxSizer( wxHORIZONTAL );
	//bSizerSerie1->Add( m_dvcSerie1, 0, wxALL, 5 );
	bSizerSerie1->Add( m_TreeListCtrl, 1, wxALL|wxEXPAND, 5 );
	bSizerSerie2->Add( m_dvcSerie2, 1, wxALL|wxEXPAND, 5 );
	bSizerSerie3->Add( m_dvcSerie3, 1, wxALL|wxEXPAND, 5 );
	bSizerSerie4->Add( m_dvcSerie4, 1, wxALL|wxEXPAND, 5 );
	bSizerSerie5->Add( m_dvcSerie5, 1, wxALL|wxEXPAND, 5 );
	m_pnlSerie1->SetSizer( bSizerSerie1 );
	m_pnlSerie1->Layout();
	bSizerSerie1->Fit( m_pnlSerie1 );
	m_additionalInformation->AddPage( m_pnlSerie1, wxT("Serie 1 (overview)"), true );
	m_pnlSerie2->SetSizer( bSizerSerie2 );
	m_pnlSerie2->Layout();
	bSizerSerie2->Fit( m_pnlSerie2 );
	m_additionalInformation->AddPage( m_pnlSerie2, wxT("Serie 2 (SA)"), false );
	m_pnlSerie3->SetSizer( bSizerSerie3 );
	m_pnlSerie3->Layout();
	bSizerSerie3->Fit( m_pnlSerie3 );
	m_additionalInformation->AddPage( m_pnlSerie3, wxT("Serie 3 (LA)"), false );
	m_pnlSerie4->SetSizer( bSizerSerie4 );
	m_pnlSerie4->Layout();
	bSizerSerie4->Fit( m_pnlSerie4 );
	m_additionalInformation->AddPage( m_pnlSerie4, wxT("Serie 4 (4CH)"), false );
	m_pnlSerie5->SetSizer( bSizerSerie5 );
	m_pnlSerie5->Layout();
	bSizerSerie5->Fit( m_pnlSerie5 );
	m_additionalInformation->AddPage( m_pnlSerie5, wxT("Serie 5 (3CH)"), false );

	sbSizerAdditonalInfo1->Add( m_additionalInformation, 1, wxALL|wxEXPAND, 0 );
	bSizerPatient1->Add( sbSizerAdditonalInfo1, 1, wxALL|wxEXPAND, 0 );
	m_patient1->SetSizer( bSizerPatient1 );
	m_patient1->Layout();
	bSizerPatient1->Fit( m_patient1 );
	m_patients->AddPage( m_patient1, wxT("Patient 1"), true );

	// Patient 2
	bSizerPatient2 = new wxBoxSizer( wxVERTICAL );
	bSizerPatientInfoLeft2 = new wxBoxSizer( wxVERTICAL );
	bSizerName2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerPatID2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerDOB2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerLenght2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerPatientInfoRight2 = new wxBoxSizer( wxVERTICAL );
	bSizerSpacer2 = new wxBoxSizer( wxVERTICAL );
	bSizerGender2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerAge2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerWeight2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerScanInfo2 = new wxBoxSizer( wxVERTICAL );
	bSizerModality2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerStudyDescr2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerStudyDate2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerManufacturer2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerDisorder2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerCardiacInfo2 = new wxBoxSizer( wxVERTICAL );
	bSizerBloodPressure2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerHeartRate2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerPerformerInfo2 = new wxBoxSizer( wxVERTICAL );
	bSizerNamePerf2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerFunctionPerf2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerAnalysisDate2 = new wxBoxSizer( wxHORIZONTAL );
	bSizerAdvanced2 = new wxBoxSizer( wxVERTICAL );

	bSizerName2->Add( m_lblName2, 1, wxEXPAND, 5 );
	bSizerName2->Add( m_txtName2, 1, wxALL, 5 );
	bSizerName2->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerPatientInfoLeft2->Add( bSizerName2, 1, wxEXPAND, 5 );
	bSizerPatID2->Add( m_lblPatID2, 1, wxEXPAND, 5 );
	bSizerPatID2->Add( m_txtPatID2, 1, wxALL, 5 );
	bSizerPatID2->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerPatientInfoLeft2->Add( bSizerPatID2, 1, wxEXPAND, 5 );
	bSizerDOB2->Add( m_lblDOB2, 1, wxEXPAND, 5 );
	bSizerDOB2->Add( m_txtDOB2, 1, wxALL, 5 );
	bSizerDOB2->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerPatientInfoLeft2->Add( bSizerDOB2, 1, wxEXPAND, 5 );
	bSizerLenght2->Add( m_lblLength2, 1, wxEXPAND, 5 );
	bSizerLenght2->Add( m_txtLength2, 1, wxALL, 5 );
	bSizerLenght2->Add( m_lblMeter2, 1, wxALL, 5 );
	bSizerPatientInfoLeft2->Add( bSizerLenght2, 1, wxEXPAND, 5 );
	sbSizerPatientInfo2->Add( bSizerPatientInfoLeft2, 0, wxALL|wxEXPAND, 5 );
	bSizerSpacer2->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerSpacer2->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerSpacer2->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerSpacer2->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerPatientInfoRight2->Add( bSizerSpacer2, 1, wxEXPAND, 5 );
	bSizerGender2->Add( m_lblGender2, 1, wxALL, 5 );
	bSizerGender2->Add( m_txtGender2, 1, wxALL, 5 );
	bSizerGender2->Add( 0, 0, 1, wxALL, 5 );
	bSizerPatientInfoRight2->Add( bSizerGender2, 1, wxEXPAND, 5 );
	bSizerAge2->Add( m_lblAge2, 1, wxALL, 5 );
	bSizerAge2->Add( m_txtAge2, 1, wxALL, 5 );
	bSizerAge2->Add( m_lblYears2, 1, wxALL, 5 );
	bSizerPatientInfoRight2->Add( bSizerAge2, 1, wxEXPAND, 5 );
	bSizerWeight2->Add( m_lblWeight2, 1, wxALL, 5 );
	bSizerWeight2->Add( m_txtWeight2, 1, wxALL, 5 );
	bSizerWeight2->Add( m_lblKg2, 1, wxALL, 5 );
	bSizerPatientInfoRight2->Add( bSizerWeight2, 1, wxEXPAND, 5 );
	sbSizerPatientInfo2->Add( bSizerPatientInfoRight2, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient2->Add( sbSizerPatientInfo2, 0, wxALL|wxEXPAND, 5 );
	bSizerModality2->Add( m_lblModality2, 1, wxALL, 5 );
	bSizerModality2->Add( 0, 0, 1, wxALL, 5 );
	bSizerModality2->Add( m_txtModality2, 1, wxALL, 5 );
	bSizerScanInfo2->Add( bSizerModality2, 1, wxEXPAND, 5 );
	bSizerStudyDescr2->Add( m_lblStudyDescr2, 1, wxALL, 5 );
	bSizerStudyDescr2->Add( 0, 0, 1, wxALL, 5 );
	bSizerStudyDescr2->Add( m_txtStudyDescr2, 1, wxALL, 5 );
	bSizerScanInfo2->Add( bSizerStudyDescr2, 1, wxEXPAND, 5 );
	bSizerStudyDate2->Add( m_lblStudyDate2, 1, wxALL, 5 );
	bSizerStudyDate2->Add( 0, 0, 1, wxALL, 5 );
	bSizerStudyDate2->Add( m_txtStudyDate2, 1, wxALL, 5 );
	bSizerScanInfo2->Add( bSizerStudyDate2, 1, wxEXPAND, 5 );
	bSizerManufacturer2->Add( m_lblManufacturer2, 1, wxALL, 5 );
	bSizerManufacturer2->Add( 0, 0, 1, wxALL, 5 );
	bSizerManufacturer2->Add( m_txtManufacturer2, 1, wxALL, 5 );
	bSizerScanInfo2->Add( bSizerManufacturer2, 1, wxEXPAND, 5 );
	sbSizerScanInfo2->Add( bSizerScanInfo2, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient2->Add( sbSizerScanInfo2, 0, wxALL|wxEXPAND, 5 );
	bSizerDisorder2->Add( m_lblDisorder2, 1, wxALL, 5 );
	bSizerDisorder2->Add( 0, 0, 1, wxALL, 5 );
	bSizerDisorder2->Add( m_txtDisorder2, 1, wxALL, 5 );
	bSizerDisorder2->Add( 0, 0, 1, wxALL, 5 );
	bSizerCardiacInfo2->Add( bSizerDisorder2, 1, wxEXPAND, 5 );
	bSizerBloodPressure2->Add( m_lblBloodPress2, 1, wxALL, 5 );
	bSizerBloodPressure2->Add( 0, 0, 1, wxALL, 5 );
	bSizerBloodPressure2->Add( m_txtBloodPress2, 1, wxALL, 5 );
	bSizerBloodPressure2->Add( m_lblMmhg2, 1, wxALL, 5 );
	bSizerCardiacInfo2->Add( bSizerBloodPressure2, 1, wxEXPAND, 5 );
	bSizerHeartRate2->Add( m_lblHeartRate2, 1, wxALL, 5 );
	bSizerHeartRate2->Add( 0, 0, 1, wxALL, 5 );
	bSizerHeartRate2->Add( m_txtHeartRate2, 1, wxALL, 5 );
	bSizerHeartRate2->Add( m_lblBpm2, 1, wxALL, 5 );
	bSizerCardiacInfo2->Add( bSizerHeartRate2, 1, wxEXPAND, 5 );
	sbSizerCardiacInfo2->Add( bSizerCardiacInfo2, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient2->Add( sbSizerCardiacInfo2, 0, wxALL|wxEXPAND, 5 );
	bSizerNamePerf2->Add( m_lblNamePerf2, 1, wxALL, 5 );
	bSizerNamePerf2->Add( 0, 0, 1, wxALL, 5 ); 
	bSizerNamePerf2->Add( m_txtNamePerf2, 1, wxALL, 5 );
	bSizerPerformerInfo2->Add( bSizerNamePerf2, 1, wxEXPAND, 5 );
	bSizerFunctionPerf2->Add( m_lblFunctionPerf2, 1, wxALL, 5 );
	bSizerFunctionPerf2->Add( 0, 0, 1, wxALL, 5 );
	bSizerFunctionPerf2->Add( m_chcFunctionPerf2, 1, wxALL, 5 );
	bSizerPerformerInfo2->Add( bSizerFunctionPerf2, 1, wxEXPAND, 5 );
	bSizerAnalysisDate2->Add( m_lblAnalysisDate2, 1, wxALL, 5 );
	bSizerAnalysisDate2->Add( 0, 0, 1, wxALL, 5 );
	bSizerAnalysisDate2->Add( m_dtpAnalysisDate2, 1, wxALL, 5 );
	bSizerPerformerInfo2->Add( bSizerAnalysisDate2, 1, wxEXPAND, 5 );
	sbSizerPerformerInfo2->Add( bSizerPerformerInfo2, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient2->Add( sbSizerPerformerInfo2, 0, wxALL|wxEXPAND, 5 );
	bSizerAdvanced2->Add( m_cbxAdvanced2, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient2->Add( bSizerAdvanced2, 0, wxALL|wxEXPAND, 5 );
	m_patient2->SetSizer( bSizerPatient2 );
	m_patient2->Layout();
	bSizerPatient2->Fit( m_patient2 );
	m_patients->AddPage( m_patient2, wxT("Patient 2"), false );


	// Patient 3
	bSizerPatient3 = new wxBoxSizer( wxVERTICAL );
	bSizerPatientInfoLeft3 = new wxBoxSizer( wxVERTICAL );
	bSizerName3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerPatID3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerDOB3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerLenght3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerPatientInfoRight3 = new wxBoxSizer( wxVERTICAL );
	bSizerSpacer3 = new wxBoxSizer( wxVERTICAL );
	bSizerGender3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerAge3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerWeight3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerScanInfo3 = new wxBoxSizer( wxVERTICAL );
	bSizerModality3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerStudyDescr3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerStudyDate3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerManufacturer3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerCardiacInfo3 = new wxBoxSizer( wxVERTICAL );
	bSizerDisorder3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerBloodPressure3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerHeartRate3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerPerformerInfo3 = new wxBoxSizer( wxVERTICAL );
	bSizerNamePerf3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerFunctionPerf3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerAnalysisDate3 = new wxBoxSizer( wxHORIZONTAL );
	bSizerAdvanced3 = new wxBoxSizer( wxVERTICAL );

	bSizerName3->Add( m_lblName3, 1, wxEXPAND, 5 );
	bSizerName3->Add( m_txtName3, 1, wxALL, 5 );
	bSizerName3->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerPatientInfoLeft3->Add( bSizerName3, 1, wxEXPAND, 5 );
	bSizerPatID3->Add( m_lblPatID3, 1, wxEXPAND, 5 );
	bSizerPatID3->Add( m_txtPatID3, 1, wxALL, 5 );
	bSizerPatID3->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerPatientInfoLeft3->Add( bSizerPatID3, 1, wxEXPAND, 5 );
	bSizerDOB3->Add( m_lblDOB3, 1, wxEXPAND, 5 );
	bSizerDOB3->Add( m_txtDOB3, 1, wxALL, 5 );
	bSizerDOB3->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerPatientInfoLeft3->Add( bSizerDOB3, 1, wxEXPAND, 5 );
	bSizerLenght3->Add( m_lblLength3, 1, wxEXPAND, 5 );
	bSizerLenght3->Add( m_txtLength3, 1, wxALL, 5 );
	bSizerLenght3->Add( m_lblMeter3, 1, wxALL, 5 );
	bSizerPatientInfoLeft3->Add( bSizerLenght3, 1, wxEXPAND, 5 );
	sbSizerPatientInfo3->Add( bSizerPatientInfoLeft3, 0, wxALL|wxEXPAND, 5 );
	bSizerSpacer3->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerSpacer3->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerSpacer3->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerSpacer3->Add( 0, 0, 1, wxEXPAND, 5 );
	bSizerPatientInfoRight3->Add( bSizerSpacer3, 1, wxEXPAND, 5 );
	bSizerGender3->Add( m_lblGender3, 1, wxALL, 5 );
	bSizerGender3->Add( m_txtGender3, 1, wxALL, 5 );
	bSizerGender3->Add( 0, 0, 1, wxALL, 5 );
	bSizerPatientInfoRight3->Add( bSizerGender3, 1, wxEXPAND, 5 );
	bSizerAge3->Add( m_lblAge3, 1, wxALL, 5 );
	bSizerAge3->Add( m_txtAge3, 1, wxALL, 5 );
	bSizerAge3->Add( m_lblYears3, 1, wxALL, 5 );
	bSizerPatientInfoRight3->Add( bSizerAge3, 1, wxEXPAND, 5 );
	bSizerWeight3->Add( m_lblWeight3, 1, wxALL, 5 );
	bSizerWeight3->Add( m_txtWeight3, 1, wxALL, 5 );
	bSizerWeight3->Add( m_lblKg3, 1, wxALL, 5 );
	bSizerPatientInfoRight3->Add( bSizerWeight3, 1, wxEXPAND, 5 );
	sbSizerPatientInfo3->Add( bSizerPatientInfoRight3, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient3->Add( sbSizerPatientInfo3, 0, wxALL|wxEXPAND, 5 );
	bSizerModality3->Add( m_lblModality3, 1, wxALL, 5 );
	bSizerModality3->Add( 0, 0, 1, wxALL, 5 );
	bSizerModality3->Add( m_txtModality3, 1, wxALL, 5 );
	bSizerScanInfo3->Add( bSizerModality3, 1, wxEXPAND, 5 );
	bSizerStudyDescr3->Add( m_lblStudyDescr3, 1, wxALL, 5 );
	bSizerStudyDescr3->Add( 0, 0, 1, wxALL, 5 );
	bSizerStudyDescr3->Add( m_txtStudyDescr3, 1, wxALL, 5 );
	bSizerScanInfo3->Add( bSizerStudyDescr3, 1, wxEXPAND, 5 );
	bSizerStudyDate3->Add( m_lblStudyDate3, 1, wxALL, 5 );
	bSizerStudyDate3->Add( 0, 0, 1, wxALL, 5 );
	bSizerStudyDate3->Add( m_txtStudyDate3, 1, wxALL, 5 );
	bSizerScanInfo3->Add( bSizerStudyDate3, 1, wxEXPAND, 5 );
	bSizerManufacturer3->Add( m_lblManufacturer3, 1, wxALL, 5 );
	bSizerManufacturer3->Add( 0, 0, 1, wxALL, 5 );
	bSizerManufacturer3->Add( m_txtManufacturer3, 1, wxALL, 5 );
	bSizerScanInfo3->Add( bSizerManufacturer3, 1, wxEXPAND, 5 );
	sbSizerScanInfo3->Add( bSizerScanInfo3, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient3->Add( sbSizerScanInfo3, 0, wxALL|wxEXPAND, 5 );
	bSizerDisorder3->Add( m_lblDisorder3, 1, wxALL, 5 );
	bSizerDisorder3->Add( 0, 0, 1, wxALL, 5 );
	bSizerDisorder3->Add( m_txtDisorder3, 1, wxALL, 5 );
	bSizerDisorder3->Add( 0, 0, 1, wxALL, 5 );
	bSizerCardiacInfo3->Add( bSizerDisorder3, 1, wxEXPAND, 5 );
	bSizerBloodPressure3->Add( m_lblBloodPress3, 1, wxALL, 5 );
	bSizerBloodPressure3->Add( 0, 0, 1, wxALL, 5 );
	bSizerBloodPressure3->Add( m_txtBloodPress3, 1, wxALL, 5 );
	bSizerBloodPressure3->Add( m_lblMmhg3, 1, wxALL, 5 );
	bSizerCardiacInfo3->Add( bSizerBloodPressure3, 1, wxEXPAND, 5 );
	bSizerHeartRate3->Add( m_lblHeartRate3, 1, wxALL, 5 );
	bSizerHeartRate3->Add( 0, 0, 1, wxALL, 5 );
	bSizerHeartRate3->Add( m_txtHeartRate3, 1, wxALL, 5 );
	bSizerHeartRate3->Add( m_lblBpm3, 1, wxALL, 5 );
	bSizerCardiacInfo3->Add( bSizerHeartRate3, 1, wxEXPAND, 5 );
	sbSizerCardiacInfo3->Add( bSizerCardiacInfo3, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient3->Add( sbSizerCardiacInfo3, 0, wxALL|wxEXPAND, 5 );
	bSizerNamePerf3->Add( m_lblNamePerf3, 1, wxALL, 5 );
	bSizerNamePerf3->Add( 0, 0, 1, wxALL, 5 ); 
	bSizerNamePerf3->Add( m_txtNamePerf3, 1, wxALL, 5 );
	bSizerPerformerInfo3->Add( bSizerNamePerf3, 1, wxEXPAND, 5 );
	bSizerFunctionPerf3->Add( m_lblFunctionPerf3, 1, wxALL, 5 );
	bSizerFunctionPerf3->Add( 0, 0, 1, wxALL, 5 );
	bSizerFunctionPerf3->Add( m_chcFunctionPerf3, 1, wxALL, 5 );
	bSizerPerformerInfo3->Add( bSizerFunctionPerf3, 1, wxEXPAND, 5 );
	bSizerAnalysisDate3->Add( m_lblAnalysisDate3, 1, wxALL, 5 );
	bSizerAnalysisDate3->Add( 0, 0, 1, wxALL, 5 );
	bSizerAnalysisDate3->Add( m_dtpAnalysisDate3, 1, wxALL, 5 );
	bSizerPerformerInfo3->Add( bSizerAnalysisDate3, 1, wxEXPAND, 5 );
	sbSizerPerformerInfo3->Add( bSizerPerformerInfo3, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient3->Add( sbSizerPerformerInfo3, 0, wxALL|wxEXPAND, 5 );
	bSizerAdvanced3->Add( m_cbxAdvanced3, 0, wxALL|wxEXPAND, 5 );
	bSizerPatient3->Add( bSizerAdvanced3, 0, wxALL|wxEXPAND, 5 );
	m_patient3->SetSizer( bSizerPatient3 );
	m_patient3->Layout();
	bSizerPatient3->Fit( m_patient3 );
	m_patients->AddPage( m_patient3, wxT("Patient 3"), false );
	
	bSizerMainWindow = new wxBoxSizer( wxVERTICAL );
	bSizerMainWindow->Add( m_patients, 2, wxEXPAND, 0 );
	GlobalSizer->Add(sbSizerDicomTree, 1, wxEXPAND, 0);
	GlobalSizer->Add(bSizerMainWindow, 3, wxEXPAND, 0);
	this->SetSizer(GlobalSizer);
	GlobalSizer->Fit(this);
    // end wxGlade
}

