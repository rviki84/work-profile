/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TuoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#ifndef _CoronaryArteryModelingWidgetCollective_H
#define _CoronaryArteryModelingWidgetCollective_H

#include "coreFrontEndPlugin.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"
#include "coreWidgetCollective.h"

#include "CoronaryArteryModelingProcessorCollective.h"

namespace cam
{

/**
This class instantiates all widgets used in the plugin. The widgets are used to operate the plugin processors
(see ProcessorCollective).
In the CoronaryArteryModeling, there is currently only one widget, but when the number of widgets grows, this class
ensures that the code remains maintainable.

\ingroup CoronaryArteryModeling
*/

class WidgetCollective : public Core::WidgetCollective
{
public:
	//!
	coreDeclareSmartPointerClassMacro(
		WidgetCollective, 
		Core::WidgetCollective );

private:
    //! The constructor instantiates all the widgets and registers them.
	WidgetCollective( );

};

} // namespace cam

#endif //_CoronaryArteryModelingWidgetCollective_H
