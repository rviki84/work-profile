/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#ifndef POINTTRACKINGONCENTERLINEPOLICY_H
#define POINTTRACKINGONCENTERLINEPOLICY_H

#include "corePointTrackingInteractorWithPolicy.h"
#include "coreImageInformation.h"
#include "CILabNamespaceMacros.h"
#include "mitkImage.h"

namespace Core{

/**
\brief This is a concrete implementation of PointTrackingInteractorPolicy that responds to the event
of a newly selected point in the 3D space and finds the closest centerline point to the new point.

\ingroup CoronaryArteryModeling
\author Vignesh Rammohan
\date 04 December 2015
*/

class PointTrackingOnCenterlinePolicy : public PointTrackingInteractorPolicy
{
public:
	typedef vtkSmartPointer<vtkPolyData> VtkPolyDataPtr;
	//Information holder type for the 3D point
	typedef Core::DataHolder< double* > 
		PointInformationHolder;

	coreDeclareSmartPointerClassMacro(
		Core::PointTrackingOnCenterlinePolicy, 
		PointTrackingInteractorPolicy);	

	//! Sets the centerline 
	void SetCenterline(VtkPolyDataPtr centerline);

	/**
	Sets the centerline information holder.
	\ param _pointInformationHolder - May not be NULL (or an assertion 
	is raised). If the holder has no
	subject, then a subject will be assigned to the holder automatically 
	using _pointInformationHolder->SetSubject.
	*/
	void SetPointInformationHolder(
		PointInformationHolder::Pointer pointInformationHolder);

	/** Respond to the event that a new point has been selected in 
	PointTrackingInteractorWithPolicy.
	*/
	void OnNewPoint(const Superclass::PointType& worldCoordinate, int timeStep);

	double* GetCenterlinePoint();

protected:
	//! Default Constructor
	PointTrackingOnCenterlinePolicy(void);

	//! The centerline
	VtkPolyDataPtr m_Centerline;
	//! The centerline information holder
	PointInformationHolder::Pointer m_PointInformationHolder;

	double m_NewPoint[3];

	double* m_CenterlinePoint;
	
	coreDeclareNoCopyConstructors(PointTrackingOnCenterlinePolicy);
};

} // Core

#endif //POINTTRACKINGONCENTERLINEPOLICY_H
