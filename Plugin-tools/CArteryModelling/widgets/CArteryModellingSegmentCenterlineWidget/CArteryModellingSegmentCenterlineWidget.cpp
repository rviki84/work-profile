/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#include "CArteryModellingSegmentCenterlineWidget.h"

// GuiBridgeLib
#include "gblWxBridgeLib.h"
#include "gblWxButtonEventProxy.h"
// Core
#include "coreDataEntityHelper.h"
#include "coreUserHelperWidget.h"
#include "coreFactoryManager.h"
#include "coreProcessorManager.h"
#include "coreImageDataEntityMacros.h"
#include "coreProcessorInputWidget.h"

// Core
#include "coreDataTreeHelper.h"
#include "coreReportExceptionMacros.h"
#include "coreDataTreeMITKHelper.h"
#include "coreSettings.h"
#include "coreMovieToolbar.h"

#include "mitkGlobalInteraction.h"
#include <wx/busyinfo.h>
#include <fstream>
#include "vtkStringArray.h"

namespace cam
{

SegmentCenterlineWidget::SegmentCenterlineWidget(  wxWindow* parent, int id/*= wxID_ANY*/,
    const wxPoint&  pos /*= wxDefaultPosition*/, 
    const wxSize&  size /*= wxDefaultSize*/, 
    long style/* = 0*/ )
: CArteryModellingSegmentCenterlineWidgetUI(parent, id,pos,size,style)
{
	m_Processor = cam::SegmentCenterlineProcessor::New();

	SetName( "Segment Centerline Widget" );
}

SegmentCenterlineWidget::~SegmentCenterlineWidget( )
{	
	// Stop the landmark selector interaction
	this->StopInteraction();

	//m_PointInformationHolder->RemoveObserver(
	//	this, 
	//	&SegmentCenterlineWidget::OnModifiedMousePoint);
	
	// Stop the mouse (helper point) interaction
	this->RemoveMouseInteractor();

	// Disconnect the connected signal
	if(m_SignalOnChangeInTimeStep.connected()) {
		m_SignalOnChangeInTimeStep.disconnect();
	}
}

void SegmentCenterlineWidget::OnInit( )
{
	//------------------------------------------------------
	// Observers to data
	//m_Processor->GetOutputDataEntityHolder( SegmentCenterlineProcessor::CENTERLINE )->AddObserver( 
	//	this, 
	//	&SegmentCenterlineWidget::OnModifiedOutputDataEntity );

	m_Processor->GetInputDataEntityHolder( SegmentCenterlineProcessor::RA_IMAGE )->AddObserver( 
		this, 
		&SegmentCenterlineWidget::OnModifiedInputDataEntity );

	m_Processor->GetInputDataEntityHolder( SegmentCenterlineProcessor::CENTERLINE1 )->AddObserver( 
		this, 
		&SegmentCenterlineWidget::OnModifiedInputCenterline1 );

	m_Processor->GetInputDataEntityHolder( SegmentCenterlineProcessor::START_END_POINTS )->AddObserver( 
		this, 
		&SegmentCenterlineWidget::OnSelectedPoints,
		Core::DataHolderEventType::DH_SUBJECT_MODIFIED);

	GetInputWidget( SegmentCenterlineProcessor::CENTERLINE1 )->SetAutomaticSelection(false);
	GetInputWidget( SegmentCenterlineProcessor::CENTERLINE1 )->SetDefaultDataEntityFlag( false );
	GetInputWidget( SegmentCenterlineProcessor::CENTERLINE2 )->SetAutomaticSelection(false);
	GetInputWidget( SegmentCenterlineProcessor::CENTERLINE2 )->SetDefaultDataEntityFlag( false );
	GetProcessorOutputObserver(SegmentCenterlineProcessor::HELPER_POINT)->SetHideInput(false);
	GetProcessorOutputObserver(SegmentCenterlineProcessor::CENTERLINE)->SetHideInput(false);
	GetProcessorOutputObserver(SegmentCenterlineProcessor::CENTERLINE_SET)->SetHideInput(false);
	GetProcessorOutputObserver(SegmentCenterlineProcessor::CENTERLINE_SET)->SelectDataEntity(false);

	Core::Widgets::MovieToolbar* movieToolbar;
	GetPluginTab()->GetWidget( wxID_MovieToolbar, movieToolbar );
	// If MovieToolbar widget is available, add the observer for the timestep change
	if ( movieToolbar )
	{
		m_SignalOnChangeInTimeStep = movieToolbar->GetCurrentTimeStep()->
			AddObserverOnModified<SegmentCenterlineWidget>(
			this, &SegmentCenterlineWidget::OnChangeInTimeStep);
	}

	m_interactionEnabled = false;
	this->sbx_sizer->Show(false);
	m_PointInformationHolder = PointInformationHolder::New();
	m_PointInformationHolder->AddObserver(
		this, 
		&SegmentCenterlineWidget::OnModifiedMousePoint);
	m_PointValuePolicy = NULL;
	m_MouseTracker = NULL;
	m_IsLandmarkModified = false;
	m_SphereSource = vtkSmartPointer<vtkSphereSource>::New();
	UpdateWidget();
}

void SegmentCenterlineWidget::UpdateWidget()
{
	m_btnSelectArtery->SetValue( m_interactionEnabled );
	std::string label = m_interactionEnabled ? "Cancel Selection" : "Select Artery";
	m_btnSelectArtery->SetLabel(_U(label));
	this->FitInside();
	
	UpdateHelperWidget( );
}

void SegmentCenterlineWidget::UpdateData()
{
	// Set parameters to processor.
	m_Processor->SetCurrTimeStep(GetTimeStep());
	m_Processor->SetImageFrameNumber(m_txtFrameNum->GetValue().c_str());
	m_Processor->SetArteryBranchNumber(m_txtBranchNum->GetValue().c_str());
	m_Processor->SetResamplingRate(m_txtResample->GetValue().c_str());
	m_Processor->SetSmoothWindowSize(m_txtSmooth->GetValue().c_str());
	signed long startRad = atol( m_txtStartRad->GetValue().c_str() );
	m_Processor->SetStartRadius(startRad);
	signed long endRad = atol( m_txtEndRad->GetValue().c_str() );
	m_Processor->SetEndRadius(endRad);
}


void SegmentCenterlineWidget::OnChkAdvancedOpt(wxCommandEvent &event)
{
	if (m_cbxAdvancedOpt->IsChecked())
	{
		this->sbx_sizer->Show(true);
	}
	else
	{
		this->sbx_sizer->Show(false);
	}
	this->FitInside();
	this->Layout();
}


void SegmentCenterlineWidget::OnBtnSelectArtery(wxCommandEvent &event)
{
	try
	{
		if (m_Processor->GetInputDataEntity(SegmentCenterlineProcessor::RA_IMAGE).IsNull())
		{
			wxMessageBox("Please provide the input RA Image");
			return;
		}

		if ( m_btnSelectArtery->GetValue() )
		{
			this->ConnectInteractor();
		}
		else
		{
			this->StopInteraction();
			this->RemoveMouseInteractor();
			m_Processor->SetCurrTimeStep(-1);
			m_IsLandmarkModified = false;
		}
		UpdateWidget();
	}
	coreCatchExceptionsReportAndNoThrowMacro( "SegmentCenterlineWidget::OnBtnSelectArtery" )
}

void SegmentCenterlineWidget::OnBtnGetHelperPoint(wxCommandEvent &event)
{
	try
	{
		if (this->m_MouseTracker.IsNull( ))
		{
			this->AddMouseInteractor();
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro( "SegmentCenterlineWidget::OnBtnGetHelperPoint" )
}

void SegmentCenterlineWidget::OnBtnSegment(wxCommandEvent &event)
{
	try
	{
		if (m_Processor->GetCurrTimeStep() == -1)
		{
			wxMessageBox("No 'Start' & 'End' points available, please provide by clicking on 'Select Artery' button");
			return;
		}

		// User Confirmation before the execution of segmentation process
		wxString confirmSegment(std::string("Do you want to perform segmentation on,") + 
			"\n\nImage Frame Number: " + m_txtFrameNum->GetValue().c_str() +
			"\nArtery Branch Number: " + m_txtBranchNum->GetValue().c_str() +
			"\n\n\nPress 'YES' to confirm,'NO'or 'CANCEL' to quit.");
		int buttonPressed = wxID_CANCEL;
		wxMessageDialog dialogRun( 
			NULL, confirmSegment, "Segmentation Process - ***Confirmation***"
			, wxYES_NO | wxCANCEL | wxICON_QUESTION );
		buttonPressed = dialogRun.ShowModal();
		// Pressing NO or CANCEL quits the execution at this point
		if (buttonPressed != wxID_YES)
		{
			return;
		}

		// Update the GUI components' values from widget to processor
		UpdateData();
		m_Processor->SetProcessType( SegmentCenterlineProcessor::SEGMENT );
		// Set multithreading to false if you want to execute it directly by main GUI thread
		GetProcessor()->SetMultithreading( false );
        Core::Runtime::Kernel::GetProcessorManager()->Execute( GetProcessor() );

		Core::DataEntity::Pointer outputDataEntity = m_Processor->GetOutputDataEntity(
			SegmentCenterlineProcessor::CENTERLINE_SET );
		GetRenderingTree()->Show(outputDataEntity, false);
	}
	coreCatchExceptionsReportAndNoThrowMacro( "SegmentCenterlineWidget::OnBtnSegment" )
}

void SegmentCenterlineWidget::OnBtnDummySegment(wxCommandEvent &event)
{
	try
	{
		if (m_Processor->GetInputDataEntity(SegmentCenterlineProcessor::RA_IMAGE).IsNull())
		{
			wxMessageBox("Please provide the input RA Image");
			return;
		}
		// User Confirmation before the execution of dummy segmentation process
		wxString confirmSegment(std::string("Do you want to add a dummy segment for,") + 
			"\n\nImage Frame Number: " + m_txtFrameNum->GetValue().c_str() +
			"\nArtery Branch Number: " + m_txtBranchNum->GetValue().c_str() +
			"\n\n\nPress 'YES' to confirm,'NO'or 'CANCEL' to quit.");
		int buttonPressed = wxID_CANCEL;
		wxMessageDialog dialogRun( 
			NULL, confirmSegment, "Add Dummy Segment - ***Confirmation***"
			, wxYES_NO | wxCANCEL | wxICON_QUESTION );
		buttonPressed = dialogRun.ShowModal();
		// Pressing NO or CANCEL quits the execution at this point
		if (buttonPressed != wxID_YES)
		{
			return;
		}

		// Update the GUI components' values from widget to processor
		UpdateData();
		m_Processor->SetProcessType( SegmentCenterlineProcessor::DUMMY );
		// Set multithreading to false if you want to execute it directly by main GUI thread
		GetProcessor()->SetMultithreading( false );
		Core::Runtime::Kernel::GetProcessorManager()->Execute( GetProcessor() );

		Core::DataEntity::Pointer outputDataEntity = m_Processor->GetOutputDataEntity(
			SegmentCenterlineProcessor::CENTERLINE_SET );
		GetRenderingTree()->Show(outputDataEntity, false);
	}
	coreCatchExceptionsReportAndNoThrowMacro( "SegmentCenterlineWidget::OnBtnDummySegment" )
}

void SegmentCenterlineWidget::OnBtnMerge(wxCommandEvent &event)
{
	try
	{
		if (m_Processor->GetInputDataEntity(SegmentCenterlineProcessor::CENTERLINE1).IsNull())
		{
			wxMessageBox("No parent branch is selected, please provide by using the dropdown menu");
			return;
		}
		if (m_Processor->GetInputDataEntity(SegmentCenterlineProcessor::CENTERLINE2).IsNull())
		{
			wxMessageBox("No child branch is selected, please provide by using the dropdown menu");
			return;
		}
		
		// User Confirmation before the execution of merge process
		wxString confirmSegment(std::string("Do you want to perform merge on,") +
			"\n\nImage Frame Number: " + m_txtFrameNum->GetValue().c_str() +
			"\nArtery Branch Number: " + m_txtBranchNum->GetValue().c_str() +
			"\n\n\nPress 'YES' to confirm,'NO'or 'CANCEL' to quit.");
		int buttonPressed = wxID_CANCEL;
		wxMessageDialog dialogRun( 
			NULL, confirmSegment, "Merge Segments - ***Confirmation***"
			, wxYES_NO | wxCANCEL | wxICON_QUESTION );
		buttonPressed = dialogRun.ShowModal();
		// Pressing NO or CANCEL quits the execution at this point
		if (buttonPressed != wxID_YES)
		{
			return;
		}

		// Update the GUI components' values from widget to processor
		UpdateData();
		m_Processor->SetProcessType( SegmentCenterlineProcessor::MERGE );
		// Set multithreading to false if you want to execute it directly by main GUI thread
		GetProcessor()->SetMultithreading( false );
        Core::Runtime::Kernel::GetProcessorManager()->Execute( GetProcessor() );

		Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
		Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
		if ( m_Processor->GetInputDataEntity( SegmentCenterlineProcessor::CENTERLINE1 ).IsNotNull() )
		{
			list->Remove( m_Processor->GetInputDataEntity( SegmentCenterlineProcessor::CENTERLINE1 ) );
		}
		if ( m_Processor->GetInputDataEntity( SegmentCenterlineProcessor::CENTERLINE2 ).IsNotNull() )
		{
			list->Remove( m_Processor->GetInputDataEntity( SegmentCenterlineProcessor::CENTERLINE2 ) );
		}

		Core::DataEntity::Pointer outputDataEntity = m_Processor->GetOutputDataEntity(
			SegmentCenterlineProcessor::CENTERLINE_SET );
		GetRenderingTree()->Show(outputDataEntity, false);
	}
	coreCatchExceptionsReportAndNoThrowMacro( "SegmentCenterlineWidget::OnBtnMerge" )
}

void SegmentCenterlineWidget::OnBtnDeleteSegment(wxCommandEvent &event)
{
	try
	{
		if (m_Processor->GetOutputDataEntity(SegmentCenterlineProcessor::CENTERLINE_SET).IsNull())
		{
			wxMessageBox("No segment is available to delete!");
			return;
		}
		// User Confirmation before the execution of dummy segmentation process
		wxString confirmSegment(std::string("Do you want to delete last segment with,") + 
			"\n\nImage Frame Number: " + m_txtFrameNum->GetValue().c_str() +
			"\nArtery Branch Number: " + m_txtBranchNum->GetValue().c_str() +
			"\n\n\nPress 'YES' to confirm,'NO'or 'CANCEL' to quit.");
		int buttonPressed = wxID_CANCEL;
		wxMessageDialog dialogRun( 
			NULL, confirmSegment, "Delete Segment - ***Confirmation***"
			, wxYES_NO | wxCANCEL | wxICON_QUESTION );
		buttonPressed = dialogRun.ShowModal();
		// Pressing NO or CANCEL quits the execution at this point
		if (buttonPressed != wxID_YES)
		{
			return;
		}

		// Update the GUI components' values from widget to processor
		UpdateData();
		m_Processor->SetProcessType( SegmentCenterlineProcessor::DELETE_SEGMENT );
		// Set multithreading to false if you want to execute it directly by main GUI thread
		GetProcessor()->SetMultithreading( false );
		Core::Runtime::Kernel::GetProcessorManager()->Execute( GetProcessor() );

		Core::DataEntity::Pointer outputDataEntity = m_Processor->GetOutputDataEntity(
			SegmentCenterlineProcessor::CENTERLINE_SET );
		GetRenderingTree()->Show(outputDataEntity, false);
	}
	coreCatchExceptionsReportAndNoThrowMacro( "SegmentCenterlineWidget::OnBtnDeleteSegment" )
}

void SegmentCenterlineWidget::OnBtnSampleRadius(wxCommandEvent &event)
{
	try
	{
		if (m_Processor->GetCurrTimeStep() == -1)
		{
			wxMessageBox("No 'Start' & 'End' points available, please provide by clicking on 'Select Artery' button");
			return;
		}
		//wxWindowDisabler disableAll;
		//wxBusyInfo info(wxT("Processing, please wait..."));
		
		UpdateData();
		m_Processor->SetProcessType( SegmentCenterlineProcessor::SAMPLE );
		GetProcessor()->SetMultithreading( false );
        Core::Runtime::Kernel::GetProcessorManager()->Execute( GetProcessor() );

		std::ifstream readFile;
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		std::string filePath = settings->GetProjectHomePath() + "\\tmp\\SampleRadius.txt";
		std::string startRadius, endRadius;
		readFile.open( filePath, std::ios::in );
		readFile >> startRadius >> endRadius;
		m_txtStartRad->SetValue(startRadius);
		m_txtEndRad->SetValue(endRadius);
		readFile.close();
	}
	coreCatchExceptionsReportAndNoThrowMacro( "SegmentCenterlineWidget::OnBtnSampleRadius" )
}

void SegmentCenterlineWidget::OnTxtFrameNum(wxCommandEvent &event)
{
	m_Processor->SetImageFrameNumber(m_txtFrameNum->GetValue().c_str());
}

void SegmentCenterlineWidget::OnTxtBranchNum(wxCommandEvent &event)
{
	m_Processor->SetArteryBranchNumber(m_txtBranchNum->GetValue().c_str());	
}

void SegmentCenterlineWidget::OnTxtStartRadius(wxCommandEvent &event)
{
	signed long startRad = atol( m_txtStartRad->GetValue().c_str() );
	m_Processor->SetStartRadius(startRad);
}

void SegmentCenterlineWidget::OnTxtEndRadius(wxCommandEvent &event)
{
	signed long endRad = atol( m_txtEndRad->GetValue().c_str() );
	m_Processor->SetEndRadius(endRad);
}

void SegmentCenterlineWidget::OnTxtResample(wxCommandEvent &event)
{
	m_Processor->SetResamplingRate(m_txtResample->GetValue().c_str());
}

void SegmentCenterlineWidget::OnTxtSmooth(wxCommandEvent &event)
{
	m_Processor->SetSmoothWindowSize(m_txtSmooth->GetValue().c_str());
}


void SegmentCenterlineWidget::OnModifiedOutputDataEntity()
{
	try
    {
		/*
		Core::DataEntity::Pointer inputDataEntity;
		inputDataEntity = m_Processor->GetInputDataEntity(SegmentCenterlineProcessor::RA_IMAGE );

		// Hide input if is different from output and output is not empty
		if ( m_Processor->GetOutputDataEntity( SegmentCenterlineProcessor::CENTERLINE ).IsNotNull() && 
			 m_Processor->GetOutputDataEntity( SegmentCenterlineProcessor::CENTERLINE ) != inputDataEntity )
		{
			GetRenderingTree()->Show( inputDataEntity, false );
		}
		*/

		// Add output to the data list and render it
		// After adding the output, the input will automatically be changed to
		// this one
	/*	Core::DataTreeHelper::PublishOutput( 
			m_Processor->GetOutputDataEntityHolder( 0 ), 
			GetRenderingTree( ),
			m_selectedDataEntityHolder );*/
	
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		"SegmentCenterlineWidget::OnModifiedOutputDataEntity")
}

void SegmentCenterlineWidget::UpdateHelperWidget()
{
	if ( GetHelperWidget( ) == NULL )
	{
		return;
	}
    GetHelperWidget( )->SetInfo( 
        Core::Widgets::HELPER_INFO_LEFT_BUTTON, 
        " info that is useful in order to use the processor" );
}

bool SegmentCenterlineWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = CArteryModellingSegmentCenterlineWidgetUI::Enable( enable );

	// If this panel widget is selected -> Update the widget
	if ( enable )
	{
		UpdateWidget();
	}

	return bReturn;
}

void SegmentCenterlineWidget::OnModifiedInputDataEntity()
{
	if (m_Processor->GetInputDataEntity(SegmentCenterlineProcessor::RA_IMAGE ).IsNotNull())
	{
		// Get and set image folder for the processor
		blTag::Pointer tag = m_Processor->GetInputDataEntity(
			SegmentCenterlineProcessor::RA_IMAGE)->GetMetadata()->GetTag( "ImageDataPath" );
		m_Processor->SetImagePath(tag->GetValueAsString());

		blTag::Pointer tagCardiacPhase = m_Processor->GetInputDataEntity(
			SegmentCenterlineProcessor::RA_IMAGE)->GetMetadata()->GetTag( "CardiacPhases" );
		m_CardiacPhases.clear();
		tagCardiacPhase->GetValue< std::vector<double> >( m_CardiacPhases );
		this->OnChangeInTimeStep();
	}
}

void SegmentCenterlineWidget::OnModifiedInputCenterline1()
{
	if (this->m_MouseTracker.IsNotNull( ))
	{
		this->RemoveMouseInteractor();
		this->AddMouseInteractor();
	}
}

void SegmentCenterlineWidget::OnChangeInTimeStep()
{
	// Set the cardiac phase
	if (!m_CardiacPhases.empty())
	{
		double cardiacPhase = m_CardiacPhases[GetTimeStep()];
		m_txtCardiacPhase->SetValue(wxString::Format(wxT("%f"), cardiacPhase));
	}
	// Set the image frame number
	m_txtFrameNum->SetValue(wxString::Format(wxT("%i"), GetTimeStep()));
}

Core::BaseProcessor::Pointer SegmentCenterlineWidget::GetProcessor()
{
	return m_Processor.GetPointer( );
}


void SegmentCenterlineWidget::OnSelectedPoints()
{
	try
	{
		if (m_Processor->GetInputDataEntity(SegmentCenterlineProcessor::START_END_POINTS ).IsNotNull())
		{
			Core::vtkPolyDataPtr selectedPoints;
			m_Processor->GetProcessingData( SegmentCenterlineProcessor::START_END_POINTS, selectedPoints, GetTimeStep());
			Core::Widgets::LandmarkSelectorWidget* widget;
			widget = GetSelectionToolWidget< Core::Widgets::LandmarkSelectorWidget>( "Landmark selector" );
			if (widget == NULL || selectedPoints == NULL)
			{
				return;
			}

			const std::string STR_LANDMARK_VECTOR_NAME = "LandmarksName"; //! see code in coreLandMarkSelectorWidget
			Core::DataEntity::Pointer dataEntityLandmarks;
			dataEntityLandmarks = m_Processor->GetInputDataEntity(SegmentCenterlineProcessor::START_END_POINTS );
			
			if (selectedPoints->GetNumberOfPoints() == 1)
			{
				// Update the values in the view
				vtkStringArray *landmarksName = blVTKHelperTools::GetPointDataArray<vtkStringArray>( selectedPoints,  STR_LANDMARK_VECTOR_NAME.c_str() );
				landmarksName->SetValue(0, vtkStdString("Start Point"));
				selectedPoints->Update();

				if ( this->m_MouseTracker.IsNotNull( ) && !m_IsLandmarkModified)
				{
					vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
					points->InsertNextPoint(m_HelperPoint[0], m_HelperPoint[1], m_HelperPoint[2]);
					points->Modified();
					selectedPoints->SetPoints(points);
					selectedPoints->Modified();
					selectedPoints->Update();
				
					m_IsLandmarkModified = true;
					dataEntityLandmarks->Modified();
					GetMultiRenderWindow()->RequestUpdateAll();					
				}	
			}
			else if (selectedPoints->GetNumberOfPoints() == 2)
			{
				if (this->m_MouseTracker.IsNotNull( ))
				{
					vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
					points->InsertNextPoint(m_HelperPoint[0], m_HelperPoint[1], m_HelperPoint[2]);
					points->Modified();
					points->InsertNextPoint(selectedPoints->GetPoint(1));
					points->Modified();
					selectedPoints->SetPoints(points);
					selectedPoints->Modified();
					selectedPoints->Update();

					//dataEntityLandmarks->Modified();
					GetMultiRenderWindow()->RequestUpdateAll();
				}

				// Update the values in the view i.e., the point names
				vtkAbstractArray *abstractArray = selectedPoints->GetPointData()->GetAbstractArray( STR_LANDMARK_VECTOR_NAME.c_str() );
				vtkStringArray *dataArray = vtkStringArray::SafeDownCast( abstractArray );
				dataArray->Reset();
				dataArray->InsertNextValue( vtkStdString("Start Point") );
				dataArray->InsertNextValue( vtkStdString("End Point") );
				selectedPoints->Update();

				m_Processor->SetCurrTimeStep(GetTimeStep());
				m_Processor->UpdatePixelPts();
			}
			else if (selectedPoints->GetNumberOfPoints() == 0)
			{
				m_IsLandmarkModified = false;
			}
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro(SegmentCenterlineWidget::OnSelectedPoints)
}

void SegmentCenterlineWidget::AddMouseInteractor()
{
	// Create mouse tracker
	try
	{
		Core::DataEntity::Pointer CenterlineDataEntity; 
		CenterlineDataEntity = m_Processor->GetInputDataEntity( SegmentCenterlineProcessor::CENTERLINE1 );
		if ( CenterlineDataEntity.IsNull( ) )
		{
			wxMessageBox("Please select a Centerline to be tracked at the input Segment-1");
			return;
		}
		VtkPolyDataPtr centerline;
		CenterlineDataEntity->GetProcessingData( centerline, GetTimeStep() );
		if ( !centerline )
		{
			return;
		}

		this->InstantiateCenterlinePointPolicy( centerline );
		if (this->m_PointValuePolicy)
		{		
			this->m_MouseTracker = Core::PointTrackingInteractorWithPolicy::New("PositionTracker",
				this->m_PointValuePolicy);
		}

		if (this->m_MouseTracker)
		{
			//The interactor must be registered as a listener
			mitk::GlobalInteraction::GetInstance()->AddListener(
				static_cast<mitk::StateMachine::Pointer>(this->m_MouseTracker)
				);
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro("SegmentCenterlineWidget::AddMouseInteractor")
}


void SegmentCenterlineWidget::RemoveMouseInteractor()
{
	try
	{
		if ( this->m_MouseTracker.IsNotNull( ) )
		{
			mitk::GlobalInteraction::GetInstance()->RemoveListener
				(
				static_cast<mitk::StateMachine::Pointer>(this->m_MouseTracker)
				);
			this->m_MouseTracker = NULL;
			this->m_PointValuePolicy = NULL;
			//this->m_PointInformationHolder->NotifyObservers();

			Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
			Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
			if ( m_Processor->GetOutputDataEntity( SegmentCenterlineProcessor::HELPER_POINT ).IsNotNull() )
			{
				list->Remove( m_Processor->GetOutputDataEntity( SegmentCenterlineProcessor::HELPER_POINT ) );
			}
		}
	}
	coreCatchExceptionsReportAndNoThrowMacro("SegmentCenterlineWidget::RemoveMouseInteractor")
}

void SegmentCenterlineWidget::InstantiateCenterlinePointPolicy( VtkPolyDataPtr centerline )
{
	Core::PointTrackingOnCenterlinePolicy::Pointer policy; 
	policy = Core::PointTrackingOnCenterlinePolicy::New();
	policy->SetCenterline( centerline );
	this->m_PointInformationHolder->SetSubject( centerline->GetPoint(0));
	policy->SetPointInformationHolder(this->m_PointInformationHolder);
	this->m_PointValuePolicy = static_cast<Core::PointTrackingInteractorPolicy::Pointer>(policy);
}


void SegmentCenterlineWidget::OnModifiedMousePoint()
{	
	if (m_PointInformationHolder->GetSubject() != NULL)
	{
		// Create a sphere
		m_SphereSource->SetCenter(m_PointInformationHolder->GetSubject());
		m_SphereSource->SetRadius(2.0);
		m_SphereSource->Update();

		if (!m_IsLandmarkModified)
		{
			for(unsigned int i=0;i<3;i++)
			{
				m_HelperPoint[i] = m_PointInformationHolder->GetSubject()[i];
			}
		}

		Core::DataEntity::Pointer RAImgDataEntity;
		RAImgDataEntity = m_Processor->GetInputDataEntity(SegmentCenterlineProcessor::RA_IMAGE );
		size_t timestep = RAImgDataEntity->GetNumberOfTimeSteps();
		m_Processor->GetOutputPort( SegmentCenterlineProcessor::HELPER_POINT )->SetTotalTimeSteps(
			timestep);
		// Update the output HELPER_POINT
		m_Processor->GetOutputPort( SegmentCenterlineProcessor::HELPER_POINT )->UpdateOutput( 
			m_SphereSource->GetOutput(), GetTimeStep(), RAImgDataEntity );
	}
}

void SegmentCenterlineWidget::ConnectInteractor()
{
	Core::Widgets::LandmarkSelectorWidget* widget;
	widget = GetSelectionToolWidget< Core::Widgets::LandmarkSelectorWidget>( "Landmark selector" );

	if (!m_interactionEnabled && widget != NULL)
	{
		// Connect interactor
		widget->SetAllowedInputDataTypes( Core::ImageTypeId );
		widget->SetInputDataEntity( 
			m_Processor->GetInputDataEntity(SegmentCenterlineProcessor::RA_IMAGE) );
		widget->SetInteractorType( Core::PointInteractor::POINT_SET);
		widget->SetDataName("Start-End Points");

		widget->StartInteractor();	
		widget->SetLandmarksName("Start Point");

		Core::PointInteractorPointSet* pointSetInteractor;
		pointSetInteractor = static_cast<Core::PointInteractorPointSet*> (
			widget->GetPointInteractor( ).GetPointer( ));

		//sets max number of selected points
		pointSetInteractor->SetMaxPoints(2);
		pointSetInteractor->SetEnableContinuousMoveEvent(true);

		m_Processor->SetInputDataEntity( SegmentCenterlineProcessor::START_END_POINTS, 
			pointSetInteractor->GetSelectedPointsDataEntity() );

		/*Core::DataTreeMITKHelper::ChangeShowLabelsProperty(  
			pointSetInteractor->GetSelectedPointsDataEntity(),
			GetRenderingTree( ),  
			false );

		// Disable
		blMITKUtils::SetScalarMode( pointSetInteractor->GetSelectedDataEntityNode(), "", 2 );
		*/
		m_interactionEnabled = true;
	}

}

void SegmentCenterlineWidget::DisconnectInteractor()
{
	Core::Widgets::LandmarkSelectorWidget* widget;
	widget = GetSelectionToolWidget< Core::Widgets::LandmarkSelectorWidget>( "Landmark selector" );
	if ( widget == NULL  || !m_interactionEnabled)
	{
		return;
	}
	// Disconnect interactor
	widget->StopInteraction();

	widget->SetDefaultAllowedInputDataTypes( );
	m_interactionEnabled = false;
}

void SegmentCenterlineWidget::StopInteraction()
{
	try
	{
		DisconnectInteractor();
		
		Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
		Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
		
		if ( m_Processor->GetInputDataEntity( SegmentCenterlineProcessor::START_END_POINTS ).IsNotNull() )
		{
			list->Remove( m_Processor->GetInputDataEntity( SegmentCenterlineProcessor::START_END_POINTS ) );
		}
		
	}
	coreCatchExceptionsReportAndNoThrowMacro(SegmentCenterlineWidget::StopInteraction)
}

} //namespace cam
