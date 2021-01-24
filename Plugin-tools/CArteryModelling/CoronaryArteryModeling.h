/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CoronaryArteryModeling_H
#define _CoronaryArteryModeling_H

#include "CoronaryArteryModelingProcessorCollective.h"
#include "CoronaryArteryModelingWidgetCollective.h"

// CoreLib
#include "coreFrontEndPlugin.h"

namespace cam // cam stands for CoronaryArteryModeling
{

/** 
\brief Creates all objects of the plug-in and connect them.

\note Nobody can get access to this class. This class is only for 
initialization of all components. 

\note Try to make all processors reusable for other plug-ins. Be aware 
of creating a dependency between the processor and any class of the rest 
of the plug-in.

\ingroup CoronaryArteryModeling
\author Vignesh Rammohan
\date 30 July 2015
*/
class PLUGIN_EXPORT CoronaryArteryModeling : public Core::FrontEndPlugin::FrontEndPlugin
{
// TYPE DEFINITIONS
public:
	coreDeclareSmartPointerClassMacro(CoronaryArteryModeling, Core::FrontEndPlugin::FrontEndPlugin);

// OPERATIONS
protected:
	//!
	CoronaryArteryModeling(void);

	//!
	virtual ~CoronaryArteryModeling(void);

private:
	//! Purposely not implemented
	CoronaryArteryModeling( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	//! Contains all the processors for the plugin
	ProcessorCollective::Pointer m_Processors;

	//! Contains all the widgets for the plugin
	WidgetCollective::Pointer m_Widgets;
}; // class CoronaryArteryModeling

} // namespace cam

#endif // CoronaryArteryModeling_H
