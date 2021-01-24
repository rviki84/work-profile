/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#include "PatientInfoPluginWidgetCollective.h"
#include "PatientInfoPluginPatientWorkingAreaWidget.h"
const long wxID_PatientWorkingAreaWidget = wxNewId();


#include "wxID.h"

#include "coreFrontEndPlugin.h"
#include "corePluginTab.h"

namespace patientInfoPlugin
{

WidgetCollective::WidgetCollective( ) 
{
	Core::Runtime::Kernel::GetGraphicalInterface()->CreatePluginTab( "Patient Info" );

	// Panel widgets
	/*Core::WindowConfig config;
	config.ProcessorObservers().TabPage( "Patient Info" ).CommandPanel();
   Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory(
    PatientWorkingAreaPanelWidget::Factory::NewBase(), 
    config.Caption("PatientWorkingAreaPanelWidget").
    Id(wxID_PatientWorkingAreaWidget) );*/
	
	Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory( 
		PatientWorkingAreaWidget::Factory::NewBase( ), 
		Core::WindowConfig( ).WorkingArea()
		.Id( wxID_PatientWorkingAreaWidget ).Caption( "Patient Info Working Area" ) );
		
	
	// disable unwanted functionality
	if ( GetPluginTab( ) != NULL )
	{
		GetPluginTab( )->EnableWindow( wxID_CommandPanel, false);
		GetPluginTab( )->EnableAllToolbars( true );
		//GetPluginTab( )->ShowWindow( wxID_IOToolbar );
		GetPluginTab( )->ShowWindow( wxID_PatientWorkingAreaWidget );
	}

}

} // namespace patientInfoPlugin
