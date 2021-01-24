/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#ifndef _PatientInfoIOMenuEventHandler_H
#define _PatientInfoIOMenuEventHandler_H

#define wxID_ShowPACSWidget wxID("wxID_ShowPACSWidget")

namespace patientInfoPlugin{

class PatientWorkingAreaWidget;

/** 
Handler for MRU list and File menu entries

The wxEventHandler cannot be PatientWorkingAreaWidget because the parent is
the main window and creates an infinite loop

Is not possible to connect the menu event to a PatientWorkingAreaWidget
handler function because the handler function should be a member function
of wxEventHandler class

\note This class is a modified form of class DicomPlugin::DicomMenuEventHandler 

\ingroup PatientInfoPlugin
\author Vignesh Rammohan
\date 17 August 2015
*/
class PatientInfoIOMenuEventHandler : public wxEvtHandler
{
public:
	PatientInfoIOMenuEventHandler( PatientWorkingAreaWidget* workingArea );

	//!
	void Open( const std::string &filename );

	//!
	void AddFileToHistory( const std::string &filename );

protected:
	DECLARE_EVENT_TABLE( );

	//!
	void OnMRUFile(wxCommandEvent& event);

	//! Open directory with dicom files (2D images)
	void OnMenuOpenDirectory(wxCommandEvent& event);

	/** Open DICOMDIR file which contains information about dicom data
	or Open single dicom file
	*/
	void OnMenuOpenDicomFile(wxCommandEvent& event);

	//!
	void OnMenuShowPACSWidget(wxCommandEvent& event);

private:
	//!
	PatientWorkingAreaWidget* m_WorkingArea;
};

} // patientInfoPlugin

#endif //_PatientInfoIOMenuEventHandler_H
