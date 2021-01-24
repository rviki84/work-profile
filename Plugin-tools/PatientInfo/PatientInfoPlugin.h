/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#ifndef _PatientInfoPlugin_H
#define _PatientInfoPlugin_H

#include "PatientInfoPluginProcessorCollective.h"
#include "PatientInfoPluginWidgetCollective.h"

// CoreLib
#include "coreFrontEndPlugin.h"

namespace patientInfoPlugin
{

/** 
\brief Creates all objects of the plug-in and connect them.

\note Nobody can get access to this class. This class is only for 
initialization of all components. 

\note Try to make all processors reusable for other plug-ins. Be aware 
of creating a dependency between the processor and any class of the rest 
of the plug-in.

\ingroup PatientInfoPlugin
*/
class PLUGIN_EXPORT PatientInfoPlugin : public Core::FrontEndPlugin::FrontEndPlugin
{
// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerClassMacro(PatientInfoPlugin, Core::FrontEndPlugin::FrontEndPlugin);

// OPERATIONS
protected:
	//!
	PatientInfoPlugin(void);

	//!
	virtual ~PatientInfoPlugin(void);

private:
	//! Purposely not implemented
	PatientInfoPlugin( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//! Contains all the processors for the plugin
	ProcessorCollective::Pointer m_Processors;

	//! Contains all the widgets for the plugin
	WidgetCollective::Pointer m_Widgets;
}; // class PatientInfoPlugin

} // namespace patientInfoPlugin

#endif // PatientInfoPlugin_H
