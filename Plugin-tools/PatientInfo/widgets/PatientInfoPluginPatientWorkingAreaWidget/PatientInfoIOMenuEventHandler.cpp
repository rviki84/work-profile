/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#include "PatientInfoIOMenuEventHandler.h"
#include "PatientInfoPluginPatientWorkingAreaWidget.h"
#include "DicomPluginWidgetCollective.h"

#include "coreWxMitkGraphicalInterface.h"
#include "coreKernel.h"
#include "coreMainMenu.h"
#include "coreSettings.h"
#include "corePluginTab.h"
#include "coreFile.h"
#include "coreBaseMainWindow.h"

#include "blMitkUnicode.h"

#include "wx/busyinfo.h"

using namespace patientInfoPlugin;

// Event the widget
BEGIN_EVENT_TABLE(PatientInfoIOMenuEventHandler, wxEvtHandler)
	EVT_MENU	(wxID_OpenDataMenuItem, PatientInfoIOMenuEventHandler::OnMenuOpenDicomFile)	
	EVT_MENU	(wxID_OpenDirectoryMenuItem, PatientInfoIOMenuEventHandler::OnMenuOpenDirectory)	
	EVT_MENU	(wxID_ShowPACSWidget, PatientInfoIOMenuEventHandler::OnMenuShowPACSWidget)	
	EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, PatientInfoIOMenuEventHandler::OnMRUFile)
END_EVENT_TABLE()

PatientInfoIOMenuEventHandler::PatientInfoIOMenuEventHandler( PatientWorkingAreaWidget* workingArea ) : wxEvtHandler()
{
	m_WorkingArea = workingArea;
}

void PatientInfoIOMenuEventHandler::OnMRUFile( wxCommandEvent& event )
{

	if ( !GetEvtHandlerEnabled() )
	{
		event.Skip();
		return;
	}

	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();

	wxFrame* mainFrame = dynamic_cast<wxFrame*> ( gIface->GetMainWindow( ) );
	wxMenuBar* menuBar = mainFrame->GetMenuBar( );
	Core::Widgets::MainMenu* mainMenu = dynamic_cast<Core::Widgets::MainMenu*> ( menuBar );
	wxString f(mainMenu->GetFileHistory()->GetHistoryFile(event.GetId() - wxID_FILE1));

	Open( f.c_str( ) );
}

void patientInfoPlugin::PatientInfoIOMenuEventHandler::OnMenuOpenDirectory( wxCommandEvent& event )
{
	if ( !GetEvtHandlerEnabled() )
	{
		event.Skip();
		return;
	}

	std::string fileName(event.GetString().mb_str());

	wxDirDialog openDirectoryDialog( m_WorkingArea, wxT("Open Dicom directory"), wxT(""));

	try
	{
		if(fileName.empty())
		{
			Core::Runtime::Settings::Pointer settings;
			settings = Core::Runtime::Kernel::GetApplicationSettings();
			std::string dataPath = settings->GetCurrentDataPath();

			openDirectoryDialog.SetPath(_U(dataPath));
			if(openDirectoryDialog.ShowModal() == wxID_OK)
			{
				std::string dirPath(openDirectoryDialog.GetPath().mb_str(wxConvUTF8));
				if(dirPath.compare("") == 0)
				{
					throw Core::Exceptions::Exception( 
						"OnMenuOpenDirectory",
						"Provide a valid directory path" );
				}

				wxTheApp->Yield(true);

				Open(dirPath);

				AddFileToHistory( dirPath );
			}
		}
		else
		{
            wxTheApp->Yield(true);
            Open(fileName);
            AddFileToHistory( fileName );
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(PatientInfoIOMenuEventHandler::OnMenuOpenDirectory)

}

void patientInfoPlugin::PatientInfoIOMenuEventHandler::OnMenuOpenDicomFile(wxCommandEvent& event)
{
	if ( !GetEvtHandlerEnabled() )
	{
		event.Skip();
		return;
	}

	wxFileDialog* openFileDialog = new wxFileDialog
		(
		m_WorkingArea,
		wxT("Open a DICOMDIR/DICOM file"),
		wxT(""), wxT(""), wxT(""),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
		);

	try
	{	
		Core::Runtime::wxMitkGraphicalInterface::Pointer gIface = Core::Runtime::Kernel::GetGraphicalInterface();
		coreAssertMacro(gIface.IsNotNull());
		std::string filetypes = "DICOMDIR Files |DICOMDIR|DICOM Files (dcm)|*.dcm|All files (*.*)|*.*";
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		std::string dataPath = settings->GetLastOpenedPath();

		//process all the pending messages
		wxTheApp->Yield(true);

		openFileDialog->SetDirectory(_U(dataPath));
		openFileDialog->SetWildcard(_U(filetypes));
		if(openFileDialog->ShowModal() == wxID_OK)
		{
			std::string filePath(openFileDialog->GetPath().mb_str(wxConvUTF8));
			if(filePath.compare("") == 0)
			{
				throw Core::Exceptions::Exception( 
					"OnMenuOpenDirectory",
					"Provide a valid file name for opening a data file" );
			}

			wxTheApp->Yield(true);

			Open(filePath);

			AddFileToHistory( filePath );

		}

	}
	coreCatchExceptionsReportAndNoThrowMacro(PatientInfoIOMenuEventHandler::OnMenuOpenDicomFile)

	openFileDialog->Destroy();
}

void patientInfoPlugin::PatientInfoIOMenuEventHandler::AddFileToHistory( const std::string &filename )
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();

	wxFrame* mainFrame = dynamic_cast<wxFrame*> ( gIface->GetMainWindow( ) );
	wxMenuBar* menuBar = mainFrame->GetMenuBar( );
	Core::Widgets::MainMenu* mainMenu = dynamic_cast<Core::Widgets::MainMenu*> ( menuBar );
	mainMenu->GetFileHistory()->AddFileToHistory( _U( filename ) );

}

void patientInfoPlugin::PatientInfoIOMenuEventHandler::Open( const std::string &filename )
{
	wxTheApp->Yield(true);
	wxBusyInfo info(wxT("Loading DICOM data, please wait..."), m_WorkingArea);

	m_WorkingArea->ReadDataSet( filename );

	Core::Runtime::Settings::Pointer settings;
	settings = Core::Runtime::Kernel::GetApplicationSettings();
	Core::IO::File::Pointer file = Core::IO::File::New( );
	file->SetFileNameFullPath( filename );
	Core::Runtime::Kernel::GetApplicationSettings()->SetLastOpenedPath(
		file->GetPathToFile() );

}

void patientInfoPlugin::PatientInfoIOMenuEventHandler::OnMenuShowPACSWidget( wxCommandEvent& event )
{
	m_WorkingArea->GetPluginTab()->ShowWindow( wxID_ConnectToPacsDialogWidget );
}
