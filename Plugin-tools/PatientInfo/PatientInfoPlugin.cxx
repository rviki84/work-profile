/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
       #include <wx/wx.h>
#endif

#include "PatientInfoPlugin.h"

// CoreLib
#include "coreReportExceptionMacros.h"
#include "coreWxMitkGraphicalInterface.h"
#include "corePluginTab.h"

// Declaration of the plugin
coreBeginDefinePluginMacro(patientInfoPlugin::PatientInfoPlugin)
coreEndDefinePluginMacro()

namespace patientInfoPlugin
{

PatientInfoPlugin::PatientInfoPlugin(void) : FrontEndPlugin()
{
	try
	{
		m_Processors = ProcessorCollective::New();
		m_Widgets = WidgetCollective::New();
	}
	coreCatchExceptionsReportAndNoThrowMacro(PatientInfoPlugin::PatientInfoPlugin)
}

PatientInfoPlugin::~PatientInfoPlugin(void)
{
}

} // namespace patientInfoPlugin
