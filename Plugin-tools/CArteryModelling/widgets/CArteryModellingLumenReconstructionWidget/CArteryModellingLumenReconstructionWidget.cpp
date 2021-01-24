/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#include "CArteryModellingLumenReconstructionWidget.h"

// GuiBridgeLib
#include "gblWxBridgeLib.h"
#include "gblWxButtonEventProxy.h"
// Core
#include "coreDataEntityHelper.h"
#include "coreUserHelperWidget.h"
#include "coreFactoryManager.h"
#include "coreProcessorManager.h"
#include "coreProcessorInputWidget.h"

// Core
#include "coreDataTreeHelper.h"
#include "coreReportExceptionMacros.h"
#include "coreDataTreeMITKHelper.h"
#include "coreSettings.h"

#include <wx/busyinfo.h>

#include <fstream>

namespace cam
{

LumenReconstructionWidget::LumenReconstructionWidget(  wxWindow* parent, int id/*= wxID_ANY*/,
    const wxPoint&  pos /*= wxDefaultPosition*/, 
    const wxSize&  size /*= wxDefaultSize*/, 
    long style/* = 0*/ )
: CArteryModellingLumenReconstructionWidgetUI(parent, id,pos,size,style)
{
	m_Processor = cam::LumenReconstructionProcessor::New();

	SetName( "Lumen Reconstruction Widget" );
}

LumenReconstructionWidget::~LumenReconstructionWidget( )
{
	// We don't need to destroy anything because all the child windows 
	// of this wxWindow are destroyed automatically
	
}

void LumenReconstructionWidget::OnInit( )
{
	//------------------------------------------------------
	// Observers to data
	m_Processor->GetOutputDataEntityHolder( LumenReconstructionProcessor::LUMEN_SURFACE )->AddObserver( 
		this, 
		&LumenReconstructionWidget::OnModifiedOutputDataEntity );

	/*m_Processor->GetInputDataEntityHolder( LumenReconstructionProcessor::RA_IMAGE )->AddObserver( 
		this, 
		&LumenReconstructionWidget::OnModifiedInputDataEntity );
		*/
	
	GetInputWidget( LumenReconstructionProcessor::CENTERLINE_SET )->SetAutomaticSelection(false);
	GetInputWidget( LumenReconstructionProcessor::CENTERLINE_SET )->SetDefaultDataEntityFlag( false );
	
	this->sbxSizer3DRecon->Show(false);
	this->sbxSizerSurfRecon->Show(false);
	UpdateWidget();
}

void LumenReconstructionWidget::UpdateWidget()
{
	this->FitInside();
	
	UpdateHelperWidget( );
}

void LumenReconstructionWidget::UpdateData()
{
	// Set parameters to processor.
	m_Processor->SetOutlierWeight(m_txtOutlierWt->GetValue().c_str());
	m_Processor->SetMaxIterationsCPD(m_txtNoOfItersCPD->GetValue().c_str());
	m_Processor->SetTolerance(m_txtTolerance->GetValue().c_str());
	m_Processor->SetEpipolarDistanceThreshold(m_txtEpiDistThres->GetValue().c_str());
	m_Processor->SetSkip(m_txtSkipDist->GetValue().c_str());
	m_Processor->SetSkipCost(m_txtSkipCost->GetValue().c_str());
	m_Processor->SetLengthCost(m_txtLengthCost->GetValue().c_str());
	m_Processor->SetMaxIterationsOSB(m_txtNoOfItersOSB->GetValue().c_str());
	m_Processor->SetResamplingLevels(m_txtResLevel->GetValue().c_str());
	m_Processor->SetReconstructionDepth(m_txtReconDepth->GetValue().c_str());
	m_Processor->SetInterpolationWeight(m_txtPointWt->GetValue().c_str());
}

void LumenReconstructionWidget::OnBtnComputeSurface(wxCommandEvent& event)
{
	try
	{
		if (m_Processor->GetInputDataEntity(LumenReconstructionProcessor::CENTERLINE_SET).IsNull())
		{
			wxMessageBox("No centerline set is selected, please provide by using the dropdown menu");
			return;
		}

		// Update the scale values from widget to processor
		UpdateData();

        // Set multithreading to false if you want to execute it directly by main GUI thread
        GetProcessor()->SetMultithreading( false );
        Core::Runtime::Kernel::GetProcessorManager()->Execute( GetProcessor() );
	}
	coreCatchExceptionsReportAndNoThrowMacro( "LumenReconstructionWidget::OnBtnComputeSurface" );
}


void LumenReconstructionWidget::OnChkAdvancedOpt(wxCommandEvent &event)
{
	if (m_cbxAdvancedOpt->IsChecked())
	{
		this->sbxSizer3DRecon->Show(true);
		this->sbxSizerSurfRecon->Show(true);

	}
	else
	{
		this->sbxSizer3DRecon->Show(false);
		this->sbxSizerSurfRecon->Show(false);
	}
	this->FitInside();
	this->Layout();
}


void LumenReconstructionWidget::OnTxtOutlierWeight(wxCommandEvent &event)
{
	m_Processor->SetOutlierWeight(m_txtOutlierWt->GetValue().c_str());
}

void LumenReconstructionWidget::OnTxtNoOfItersCPD(wxCommandEvent &event)
{
	m_Processor->SetMaxIterationsCPD(m_txtNoOfItersCPD->GetValue().c_str());
}

void LumenReconstructionWidget::OnTxtTolerance(wxCommandEvent &event)
{
	m_Processor->SetTolerance(m_txtTolerance->GetValue().c_str());
}

void LumenReconstructionWidget::OnTxtEpiDistThres(wxCommandEvent &event)
{
	m_Processor->SetEpipolarDistanceThreshold(m_txtEpiDistThres->GetValue().c_str());
}

void LumenReconstructionWidget::OnTxtSkipDist(wxCommandEvent &event)
{
	m_Processor->SetSkip(m_txtSkipDist->GetValue().c_str());
}

void LumenReconstructionWidget::OnTxtSkipCost(wxCommandEvent &event)
{
	m_Processor->SetSkipCost(m_txtSkipCost->GetValue().c_str());
}
void LumenReconstructionWidget::OnTxtLengthCost(wxCommandEvent &event)
{
	m_Processor->SetLengthCost(m_txtLengthCost->GetValue().c_str());
}
void LumenReconstructionWidget::OnTxtNoOfItersOSB(wxCommandEvent &event)
{
	m_Processor->SetMaxIterationsOSB(m_txtNoOfItersOSB->GetValue().c_str());
}
void LumenReconstructionWidget::OnTxtResLevel(wxCommandEvent &event)
{
	m_Processor->SetResamplingLevels(m_txtResLevel->GetValue().c_str());
}

void LumenReconstructionWidget::OnTxtReconDepth(wxCommandEvent &event)
{
	m_Processor->SetReconstructionDepth(m_txtReconDepth->GetValue().c_str());
}
void LumenReconstructionWidget::OnTxtPointWt(wxCommandEvent &event)
{
	m_Processor->SetInterpolationWeight(m_txtPointWt->GetValue().c_str());
}

void LumenReconstructionWidget::OnModifiedOutputDataEntity()
{
//	try
//    {
//		Core::DataEntity::Pointer inputDataEntity;
//		inputDataEntity = m_Processor->GetInputDataEntity(LumenReconstructionProcessor::RA_IMAGE );
//
//		// Hide input if is different from output and output is not empty
//		if ( m_Processor->GetOutputDataEntity( 0 ).IsNotNull() && 
//			 m_Processor->GetOutputDataEntity( 0 ) != inputDataEntity )
//		{
//			GetRenderingTree()->Show( inputDataEntity, false );
//		}
//
//		// Add output to the data list and render it
//		// After adding the output, the input will automatically be changed to
//		// this one
//	/*	Core::DataTreeHelper::PublishOutput( 
//			m_Processor->GetOutputDataEntityHolder( 0 ), 
//			GetRenderingTree( ),
//			m_selectedDataEntityHolder );*/
//	
//	}
//	coreCatchExceptionsLogAndNoThrowMacro( 
//		"LumenReconstructionWidget::OnModifiedOutputDataEntity")
}

void LumenReconstructionWidget::UpdateHelperWidget()
{
	if ( GetHelperWidget( ) == NULL )
	{
		return;
	}
    GetHelperWidget( )->SetInfo( 
        Core::Widgets::HELPER_INFO_LEFT_BUTTON, 
        " info that is useful in order to use the processor" );
}

bool LumenReconstructionWidget::Enable( bool enable /*= true */ )
{
	bool bReturn = CArteryModellingLumenReconstructionWidgetUI::Enable( enable );

	// If this panel widget is selected -> Update the widget
	if ( enable )
	{
		UpdateWidget();
	}

	return bReturn;
}

void LumenReconstructionWidget::OnModifiedInputDataEntity()
{
	UpdateWidget();
}

Core::BaseProcessor::Pointer LumenReconstructionWidget::GetProcessor()
{
	return m_Processor.GetPointer( );
}



} //namespace cam
