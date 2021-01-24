/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TuoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#include "CoronaryArteryModelingWidgetCollective.h"

#include "CArteryModellingSegmentCenterlineWidget.h"
const long wxID_SegmentCenterlineWidget = wxNewId();

#include "CArteryModellingLumenReconstructionWidget.h"
const long wxID_LumenReconstructionWidget = wxNewId();

#include "wxID.h"

#include "coreFrontEndPlugin.h"
#include "corePluginTab.h"

namespace cam
{

WidgetCollective::WidgetCollective( ) 
{
	Core::Runtime::Kernel::GetGraphicalInterface()->CreatePluginTab( "Coronary Artery Modeling" );

	// Panel widgets
	Core::WindowConfig config;
	config.ProcessorObservers().TabPage( "Coronary Artery Modeling" ).CommandPanel();

    Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory(
	   SegmentCenterlineWidget::Factory::NewBase(), 
	   config.Caption("Segment Centerline").
	   Id(wxID_SegmentCenterlineWidget) );


	Core::Runtime::Kernel::GetGraphicalInterface()->RegisterFactory(
		LumenReconstructionWidget::Factory::NewBase(), 
		config.Caption("Lumen Reconstruction").
		Id(wxID_LumenReconstructionWidget) );
}

} // namespace cam
