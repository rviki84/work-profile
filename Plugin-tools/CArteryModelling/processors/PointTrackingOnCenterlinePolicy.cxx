/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/


#include "PointTrackingOnCenterlinePolicy.h"
#include <coreReportExceptionMacros.h>
#include <CILabAssertMacros.h>
#include "itkImage.h"
#include "coreMultiRenderWindowOverlay.h"

//VTK
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkCellLocator.h>

using namespace Core;


PointTrackingOnCenterlinePolicy::PointTrackingOnCenterlinePolicy()
{
	m_CenterlinePoint = NULL;
}

void PointTrackingOnCenterlinePolicy::SetCenterline( VtkPolyDataPtr centerline)
{
	this->m_Centerline = centerline;
	m_CenterlinePoint = centerline->GetPoint(0);
}

void PointTrackingOnCenterlinePolicy::SetPointInformationHolder( 
	PointInformationHolder::Pointer pointInformationHolder) 
{
	cilabAssertMacro(pointInformationHolder && "Arg pointInformationHolder may not be NULL");
	this->m_PointInformationHolder = pointInformationHolder;
	if( !this->m_PointInformationHolder->GetSubject() )
	{
		this->m_PointInformationHolder->SetSubject( m_CenterlinePoint );
	}
}


void PointTrackingOnCenterlinePolicy::OnNewPoint(
	const Superclass::PointType& worldCoordinate, int timeStep)
{
	if ( this->m_Centerline )
	{
		mitk::Point3D point = worldCoordinate;
		for(unsigned int i=0; i<3; i++)
		{
			this->m_NewPoint[i] = point[i];
		}

		// find the closest centerline point to the new position of the mouse pointer
		vtkSmartPointer<vtkCellLocator> locator = vtkSmartPointer<vtkCellLocator>::New();
		locator->SetDataSet(this->m_Centerline);
		locator->BuildLocator();
		double t, closest_point[3];
		int subId;
		vtkIdType cellID;
		locator->FindClosestPoint(m_NewPoint, closest_point, cellID, subId, t);
		for(unsigned int i=0; i<3; i++)
		{
			this->m_CenterlinePoint[i] = closest_point[i];
		}
	}

	// Notify observers to update the visualization
	this->m_PointInformationHolder->NotifyObservers();
}


double* PointTrackingOnCenterlinePolicy::GetCenterlinePoint()
{
	return m_CenterlinePoint;
}
