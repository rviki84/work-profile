/*
* Copyright (c) 2015,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* The University of Sheffield (TUoS), Sheffield, UK. All rights reserved.
* See license.txt file for details.
*/

#include "PatientInfoPluginPatientWorkingAreaWidget.h"
#include "PatientInfoIOMenuEventHandler.h"
#include "DicomPatientItemTreeData.h"
#include "DicomStudyItemTreeData.h"
#include "DicomSeriesItemTreeData.h"
#include "DicomTimepointItemTreeData.h"
#include "DicomSliceItemTreeData.h"

// DcmAPI
#include "dcmFile.h"
#include "dcmDataSetReader.h"
#include "dcmMultiSliceReader.h"
#include "dcmSearch.h"

// gdcm
#include <gdcmFile.h>
#include <gdcmDocEntry.h>
#include <gdcmBinEntry.h>
#include <gdcmUtil.h>
#include <gdcmSeqEntry.h>
#include <gdcmSQItem.h>

// GuiBridgeLib
#include "gblWxBridgeLib.h"
#include "gblWxButtonEventProxy.h"
// Core
#include "coreDataEntityHelper.h"
#include "coreUserHelperWidget.h"
#include "coreFactoryManager.h"
#include "coreProcessorManager.h"
#include "coreDataEntityReader.h"
#include "coreDataTreeHelper.h"
#include "coreReportExceptionMacros.h"

// wx
#include <wx/settings.h>
#include <wx/wupdlock.h>
#include "wx/dnd.h"
#include "wxEventHandlerHelper.h"

using namespace DicomPlugin;

namespace patientInfoPlugin
{

	// Check if orientation tag is present in the read dataset
	bool CheckOrientationTag( dcmAPI::DataSet::Pointer dataSet );


#if wxUSE_DRAG_AND_DROP
/**
\brief Drop target for Data entity list (i.e. user drags a file from 
explorer unto window and adds the file to entity list)

\author Xavi Planes
\date 05 Dec 2008
\ingroup gmWidgets
*/
class DICOMDropTarget : public wxFileDropTarget
{
public:
	DICOMDropTarget(PatientInfoIOMenuEventHandler *patientInfoIOMenuEventHandler) {
		m_PatientInfoIOMenuEventHandler = patientInfoIOMenuEventHandler;
	}
	~DICOMDropTarget(){}
	virtual bool OnDropFiles(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y),
		const wxArrayString& files)
	{

		if ( files.size() == 0 )
		{
			return false;
		}

		m_PatientInfoIOMenuEventHandler->Open( _U(files[ 0 ]) );

		m_PatientInfoIOMenuEventHandler->AddFileToHistory( _U(files[ 0 ]) );

		return true;
	}
private:
	//!
	PatientInfoIOMenuEventHandler *m_PatientInfoIOMenuEventHandler;
};
#endif


PatientWorkingAreaWidget::PatientWorkingAreaWidget(  wxWindow* parent, int id/*= wxID_ANY*/,
    const wxPoint&  pos /*= wxDefaultPosition*/, 
    const wxSize&  size /*= wxDefaultSize*/, 
    long style/* = 0*/ )
: PatientInfoPluginPatientWorkingAreaWidgetUI(parent, id,pos,size,style)
{
	try
	{
		m_Processor = patientInfoPlugin::PatientWorkingAreaProcessor::New();
		SetName( "Patient WorkingArea Widget" );

		Core::IO::BaseDataEntityReader::Pointer baseReader;
		baseReader = Core::IO::DataEntityReader::GetRegisteredReader( "Core::IO::DICOMFileReader" );
		if ( baseReader.IsNotNull() )
		{
			m_Reader = dynamic_cast<Core::IO::DICOMFileReader*> (baseReader->CreateAnother( ).GetPointer( ));
		}

		m_dcmImageDataHolder = Core::DataEntityHolder::New();
		
		Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
		gIface = Core::Runtime::Kernel::GetGraphicalInterface();
		wxWindow* mainWindow = dynamic_cast<wxWindow*> ( gIface->GetMainWindow( ) );
		m_PatientInfoIOMenuEventHandler = new PatientInfoIOMenuEventHandler( this );
		mainWindow->PushEventHandler( m_PatientInfoIOMenuEventHandler );


		#if wxUSE_DRAG_AND_DROP
			// Drag & Drop
			SetDropTarget( new DICOMDropTarget( m_PatientInfoIOMenuEventHandler ) );
		#endif

		m_SelectedSliceHolder = Core::DataHolder<int>::New( );
		m_SelectedSliceHolder->SetSubject( -1 );
		m_SelectedSliceHolder->AddObserver( this, &patientInfoPlugin::PatientWorkingAreaWidget::OnSelectedSliceModified );
		m_treeDicomView->SetSelectedSliceHolder( m_SelectedSliceHolder );

		// Add tree columns
		m_TreeListCtrl->AddColumn("Tag");
		m_TreeListCtrl->AddColumn("Name");
		m_TreeListCtrl->AddColumn("Value");
	}
	coreCatchExceptionsReportAndNoThrowMacro(PatientWorkingAreaWidget::PatientWorkingAreaWidget)
}

PatientWorkingAreaWidget::~PatientWorkingAreaWidget( )
{
	Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
	gIface = Core::Runtime::Kernel::GetGraphicalInterface();

	if ( m_PatientInfoIOMenuEventHandler )
	{
		wxWindow* mainWindow = dynamic_cast<wxWindow*> ( gIface->GetMainWindow( ) );
		wxPopEventHandler( mainWindow, m_PatientInfoIOMenuEventHandler );
		delete m_PatientInfoIOMenuEventHandler;
	}
}

void PatientWorkingAreaWidget::OnInit( )
{
	//------------------------------------------------------
	// Observers to data

	m_dcmImageDataHolder->AddObserver(this, &PatientWorkingAreaWidget::OnModifiedFilePath);
	/*m_Processor->GetOutputDataEntityHolder( 0 )->AddObserver( 
		this, 
		&PatientWorkingAreaWidget::OnModifiedOutputDataEntity );

	m_Processor->GetInputDataEntityHolder( PatientWorkingAreaProcessor::INPUT_0 )->AddObserver( 
		this, 
		&PatientWorkingAreaWidget::OnModifiedInputDataEntity );
	*/
	
	this->sbSizerAdditonalInfo1->Show(false);
	UpdateWidget();
	
}

void PatientWorkingAreaWidget::UpdateWidget()
{
	this->FitInside();
	GetSizer()->Layout();
	UpdateHelperWidget( );
}

void PatientWorkingAreaWidget::UpdateData()
{
	// Set parameters to processor. Pending
}


void PatientWorkingAreaWidget::OnSelectedSliceModified()
{
	int count = 0;
	wxTreeItemId item = m_treeDicomView->FindTreeItem( m_SelectedSliceHolder->GetSubject( ), count );
	if ( item != m_treeDicomView->GetCurrentSliceTreeItemId( ) )
	{
		UpdateDataEntity( item );
	}
}


void PatientWorkingAreaWidget::UpdateDataEntity(wxTreeItemId sliceTreeItemId)
{
	try
	{
		m_treeDicomView->SetCurrentSliceTreeItemId( sliceTreeItemId );

		//create data entity from selected slice
		wxArrayTreeItemIds treeItemIdsArray;
		treeItemIdsArray.Add( sliceTreeItemId );
		std::vector<Core::DataEntity::Pointer> dataEntities;
		dataEntities = BuildDataEntities( treeItemIdsArray, false, true );

		/* !currently not used
		//set filename as a label for currently rendered slice
		SliceItemTreeData* sliceItemTreeData = (SliceItemTreeData*)m_treeDicomView->GetItemData(sliceTreeItemId);
		if(sliceItemTreeData != NULL)
		{
			m_sliceName->SetValue(_U(sliceItemTreeData->GetSlicePath()));
		}
		*/

		Core::DataEntity::Pointer dataEntity;
		if ( !dataEntities.empty() && dataEntities[ 0 ].IsNotNull( ) )
		{
			dataEntity = dataEntities[ 0 ];
		}

		//set time slider position
		//SetTimeSliderPosition(sliceTreeItemId); // !currently not used

		//set space slider position
		//SetSpaceSliderPosition(sliceTreeItemId); // !currently not used

		//save current level window property
		//m_RenderWindow->SaveCurrentLevelWindow(); // !currently not used

		//set slice data in DataHolder
		m_dcmImageDataHolder->SetSubject(dataEntity, true);
	}
	coreCatchExceptionsReportAndNoThrowMacro(PatientWorkingAreaWidget::UpdateDataEntity)
}


void PatientWorkingAreaWidget::OnModifiedFilePath()
{
	// get the file path
	const std::string filePath = GetSelectedSlicePath();
	if ( m_Filename == filePath )
	{
		return;
	}

	// read the file
	m_File.SetFileName( filePath );
	if(!m_File.Load())
	{
		return;
	}

	// read the previous file
	m_PreviousFile.SetFileName( m_Filename );
	m_PreviousFile.Load( );

	m_Filename = filePath;

	GetDICOMEntries(&m_File);

	// update the grid with new values
	UpdateTagsGrid();

	this->FitInside();
	GetSizer()->Layout();
}


void PatientWorkingAreaWidget::UpdateTagsGrid()
{
	wxWindowUpdateLocker lock( this );

	// Backup scroll position
	int scrollPosY = m_TreeListCtrl->GetScrollPos( wxVERTICAL );
	// clear the grid
	m_TreeListCtrl->DeleteRoot( );
	m_TreeListCtrl->AddRoot( m_Filename );
	m_TreeListCtrl->Collapse( m_TreeListCtrl->GetRootItem( ) );

	// Fill all tags 
	UpdateSeqEntry( &m_File, m_TreeListCtrl->GetRootItem( ) );

	// Update tree control
	m_TreeListCtrl->Expand( m_TreeListCtrl->GetRootItem( ) );
	m_TreeListCtrl->SetScrollPos( wxVERTICAL, scrollPosY );
}

void PatientWorkingAreaWidget::UpdateSeqEntry( gdcm::DocEntrySet* docEntrySet, wxTreeItemId parent )
{
	// Fill all tags 
	gdcm::DocEntry* docEntry = docEntrySet->GetFirstEntry();
	while(docEntry)
	{
		UpdateEntry( docEntry, parent );

		docEntry = docEntrySet->GetNextEntry();
	}
}

void PatientWorkingAreaWidget::UpdateEntry( gdcm::DocEntry* docEntry, wxTreeItemId parent )
{
	// Get key, name
	std::string key = docEntry->GetKey();
	std::string name = docEntry->GetName();
	std::string value = GetEntryValue( docEntry );

	/*
	// Check search
	// get the text to find (use it lower case to find)
	std::string searchTxt = std::string(text_ctrl_search->GetValue().mb_str());
	searchTxt = Core::StringHelper::ToLowerCase( searchTxt );

	bool checkSearch = 
		Core::StringHelper::ToLowerCase( key ).find(searchTxt) != std::string::npos
		|| Core::StringHelper::ToLowerCase( name ).find(searchTxt) != std::string::npos
		|| Core::StringHelper::ToLowerCase( value ).find(searchTxt) != std::string::npos;
	*/
	// Check compare
	bool checkCompare = true;
	//if ( m_Toolbar->GetToolToggled( wxID_BTN_COMPARE ) == true )
	//{
		//uint16_t group = docEntry->GetDictEntry()->GetGroup();
		//uint16_t elem  = docEntry->GetDictEntry()->GetElement();

		//gdcm::DocEntry* prevDocEntry = m_PreviousFile.GetDocEntry( group, elem );
		//std::string previousValue = GetEntryValue( prevDocEntry );
		//checkCompare = previousValue != value;
	//}


	// Add tag
	if( /*checkSearch &&*/ checkCompare )
	{
		wxTreeItemId treeItem = m_TreeListCtrl->AppendItem( parent, wxT( key ) );
		m_TreeListCtrl->SetItemText(treeItem, 1, wxT( name ));
		m_TreeListCtrl->SetItemText(treeItem, 2, wxT( value ));

		// If seq entry -> Update it
		if ( gdcm::SeqEntry *sqi = dynamic_cast<gdcm::SeqEntry *>(docEntry) )
		{
			m_TreeListCtrl->SetItemText(treeItem, 2, wxT( "" ));
			for(unsigned int pd = 0; pd < sqi->GetNumberOfSQItems() ; ++pd)
			{
				gdcm::SQItem* item = sqi->GetSQItem( pd );

				std::stringstream stream;
				stream << "Item " << pd;
				wxTreeItemId childtreeItem = m_TreeListCtrl->AppendItem( treeItem, wxT( stream.str( ).c_str( ) ) );

				UpdateSeqEntry( item, childtreeItem );
			}
		}
	}
}

std::string PatientWorkingAreaWidget::GetEntryValue( gdcm::DocEntry* docEntry )
{
	if ( docEntry == NULL )
	{
		return "";
	}

	// Get entry value
	uint16_t group = docEntry->GetDictEntry()->GetGroup();
	uint16_t elem  = docEntry->GetDictEntry()->GetElement();
	std::string value = gdcm::GDCM_NOTASCII;

	// This function crashes when a is NULL
	//value = file.GetEntryForcedAsciiValue(group, elem);
	if (gdcm::ValEntry *v = dynamic_cast<gdcm::ValEntry *>(docEntry))
	{
		value = v->GetValue();
	}
	else if (gdcm::BinEntry *b = dynamic_cast<gdcm::BinEntry *>(docEntry))
	{
		uint8_t *a = b->GetBinArea();
		if (!b)
		{
			value = gdcm::GDCM_NOTLOADED;
		}
		else if (!a)
		{
			value = gdcm::GDCM_NOTLOADED;
		}
		else if (gdcm::Util::IsCleanArea(a, b->GetLength()) )
		{
			value = gdcm::Util::CreateCleanString(a, b->GetLength());
		}
	}

	return value;
}

void PatientWorkingAreaWidget::GetDICOMEntries(gdcm::DocEntrySet* docEntrySet)
{
	std::string entryVal;
	entryVal = docEntrySet->GetEntryValue(dcmAPI::tags::PatientName.m_group, dcmAPI::tags::PatientName.m_element);
	m_txtName1->SetValue(entryVal);
	entryVal = docEntrySet->GetEntryValue(dcmAPI::tags::PatientRealID.m_group, dcmAPI::tags::PatientRealID.m_element);
	m_txtPatID1->SetValue(entryVal);
	entryVal = docEntrySet->GetEntryValue(dcmAPI::tags::PatientDate.m_group, dcmAPI::tags::PatientDate.m_element);
	m_txtDOB1->SetValue(entryVal);
	entryVal = docEntrySet->GetEntryValue(dcmAPI::tags::PatientSex.m_group, dcmAPI::tags::PatientSex.m_element);
	m_txtGender1->SetValue(entryVal);
	entryVal = docEntrySet->GetEntryValue(dcmAPI::tags::PatientAge.m_group, dcmAPI::tags::PatientAge.m_element);
	m_txtAge1->SetValue(entryVal);
	entryVal = docEntrySet->GetEntryValue(dcmAPI::tags::PatientWeight.m_group, dcmAPI::tags::PatientWeight.m_element);
	m_txtWeight1->SetValue(entryVal);
	entryVal = docEntrySet->GetEntryValue(dcmAPI::tags::Modality.m_group, dcmAPI::tags::Modality.m_element);
	m_txtModality1->SetValue(entryVal);
	entryVal = docEntrySet->GetEntryValue(dcmAPI::tags::StudyDescription.m_group, dcmAPI::tags::StudyDescription.m_element);
	m_txtStudyDescr1->SetValue(entryVal);
	entryVal = docEntrySet->GetEntryValue(dcmAPI::tags::StudyDate.m_group, dcmAPI::tags::StudyDate.m_element);
	m_txtStudyDate1->SetValue(entryVal);
	entryVal = docEntrySet->GetEntryValue(dcmAPI::tags::Manufacturer.m_group, dcmAPI::tags::Manufacturer.m_element);
	m_txtManufacturer1->SetValue(entryVal);
	entryVal = docEntrySet->GetEntryValue(dcmAPI::tags::SeriesDescription.m_group, dcmAPI::tags::SeriesDescription.m_element);
	m_txtDisorder1->SetValue(entryVal);
	entryVal = docEntrySet->GetEntryValue(0x0018, 0x1088); // Heart Rate
	m_txtHeartRate1->SetValue(entryVal);
}


std::vector<Core::DataEntity::Pointer> 
	PatientWorkingAreaWidget::BuildDataEntities( 
	wxArrayTreeItemIds treeItemIdsArray,
	bool postProcessData,
	bool forRendering )
{
	std::vector<Core::DataEntity::Pointer> dataEntities;

	m_Reader->ClearSelection();
	for(int i=0; i<treeItemIdsArray.GetCount(); i++)
	{
		wxTreeItemId treeItemId = treeItemIdsArray.Item(i);

		SliceItemTreeData* sliceItemTreeData;
		sliceItemTreeData = dynamic_cast<SliceItemTreeData*>(m_treeDicomView->GetItemData(treeItemId));
		if( sliceItemTreeData != NULL)
		{
			m_Reader->SetSelectedSliceID( sliceItemTreeData->GetSliceId() );

			// Get time point
			treeItemId = m_treeDicomView->GetItemParent( treeItemId );
		}

		TimepointItemTreeData* timepointItemTreeData;
		timepointItemTreeData = dynamic_cast<TimepointItemTreeData*>(m_treeDicomView->GetItemData(treeItemId));
		if( timepointItemTreeData != NULL)
		{
			m_Reader->SetSelectedTimePointID( timepointItemTreeData->GetTiempointId() );

			// Get serie
			treeItemId = m_treeDicomView->GetItemParent( treeItemId );
		}

		SeriesItemTreeData* seriesDataItemTreeData;
		seriesDataItemTreeData = dynamic_cast<SeriesItemTreeData*>(m_treeDicomView->GetItemData(treeItemId));
		if ( seriesDataItemTreeData != NULL ) 
		{
			m_Reader->SetSelectedSerieID( seriesDataItemTreeData->GetSeriesId() );

			// Get serie
			treeItemId = m_treeDicomView->GetItemParent( treeItemId );
		}

		StudyItemTreeData* studyItemTreeData;
		studyItemTreeData = dynamic_cast<StudyItemTreeData*>(m_treeDicomView->GetItemData(treeItemId));
		if ( studyItemTreeData != NULL ) 
		{
			m_Reader->SetSelectedStudyID( studyItemTreeData->GetStudyId() );
		}

	}

	m_Reader->SetLoadFilter( Core::IO::DICOMFileReader::LOAD_FILTER_ALL );
	m_Reader->SetPostProcessData( postProcessData );
	bool prevValue = m_Reader->GetExportDICOMSlices( );
	// Set it to false for rendering
	if ( forRendering )
	{
		m_Reader->SetExportDICOMSlices( false );
	}
	m_Reader->Update( );
	// Restore value
	m_Reader->SetExportDICOMSlices( prevValue );

	for ( int i = 0 ; i < m_Reader->GetNumberOfOutputs( ) ; i++ )
	{
		// Add FilePath
		if ( m_Reader->GetOutputDataEntity( i ).IsNotNull() )
		{
			m_Reader->GetOutputDataEntity( i )->GetMetadata( )->AddTag( "FilePath", m_Reader->GetFileNames( )[ 0 ] );
		}
		dataEntities.push_back( m_Reader->GetOutputDataEntity( i ) );
	}

	return dataEntities;
}


void PatientWorkingAreaWidget::OnTreeSelChanged(wxTreeEvent& event)
{	
	try
	{
		wxArrayTreeItemIds treeItemIdsArray;
		if(m_treeDicomView->GetSelections(treeItemIdsArray) < 1)
			return;

		// get the last selected tree item
		wxTreeItemId treeItemId = treeItemIdsArray.Last();

		// If patient -> select first study
		if(dynamic_cast<PatientItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL)
		{
			if(m_treeDicomView->GetChildrenCount(treeItemId) > 0)
			{
				wxTreeItemIdValue cookie;
				treeItemId = m_treeDicomView->GetFirstChild(treeItemId, cookie);
			}
		}

		// If study -> select first timepoint
		if(dynamic_cast<StudyItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL)
		{	
			if(m_treeDicomView->GetChildrenCount(treeItemId) > 0)
			{
				wxTreeItemIdValue cookie;
				treeItemId = m_treeDicomView->GetFirstChild(treeItemId, cookie);
			}
		}

		// If study -> select first timepoint
		if(dynamic_cast<SeriesItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL)
		{	
			if(m_treeDicomView->GetChildrenCount(treeItemId) > 0)
			{
				wxTreeItemIdValue cookie;
				treeItemId = m_treeDicomView->GetFirstChild(treeItemId, cookie);
			}
		}

		//select the first slice in this timepoint
		if(dynamic_cast<TimepointItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL)
		{	
			if(m_treeDicomView->GetChildrenCount(treeItemId) > 0)
			{
				//select the middle slice
				wxTreeItemIdValue cookie;
				int middleSlicePos = m_treeDicomView->GetChildrenCount(treeItemId)/2;
				wxTreeItemId sliceItemId = m_treeDicomView->GetFirstChild(treeItemId, cookie);
				while(middleSlicePos > 0 && sliceItemId.IsOk())
				{
					sliceItemId = m_treeDicomView->GetNextChild(treeItemId, cookie);
					middleSlicePos--;
				}
				treeItemId = sliceItemId;
			}
		}


		//checking if we selected tree item is a slice item
		if(dynamic_cast<SliceItemTreeData*>(m_treeDicomView->GetItemData(treeItemId)) != NULL)
		{
			//update the data entity holder
			UpdateDataEntity(treeItemId);
		}
		
	}
	coreCatchExceptionsReportAndNoThrowMacro(PatientWorkingAreaWidget::OnTreeSelChanged)
}


void PatientWorkingAreaWidget::OnChkAdvancedOpt(wxCommandEvent &event)
{
	if (m_cbxAdvanced1->IsChecked())
	{
		this->sbSizerAdditonalInfo1->Show(true);
	}
	else
	{
		this->sbSizerAdditonalInfo1->Show(false);
	}
	this->FitInside();
	GetSizer()->Layout();
}

std::string PatientWorkingAreaWidget::GetSelectedSlicePath() const
{
	SliceItemTreeData* sliceData = dynamic_cast<SliceItemTreeData*> ( m_treeDicomView->GetItemData(m_treeDicomView->GetCurrentSliceTreeItemId()) );
	if ( sliceData == NULL )
	{
		return "";
	}

	return sliceData->GetSlicePath();
}

void PatientWorkingAreaWidget::ReadDataSet( const std::string &filename )
{
	try
	{
		// Register readers when all plugins are loaded
		RegisterReaders( );

		std::vector<std::string> filenames;
		filenames.push_back( filename );
		m_Reader->SetFileNames( filenames );
		m_Reader->ReadDataSet( filename );

		Core::Runtime::wxMitkGraphicalInterface::Pointer gIface;
		gIface = Core::Runtime::Kernel::GetGraphicalInterface();


		//load patients into tree
		m_treeDicomView->SetDcmData( GetDataSet() );
		m_treeDicomView->LoadPatientsIntoTree();

		// Update movie toolbar
		//m_MovieToolbar->SetTimeRange( 0, m_treeDicomView->GetNumberOfSlices( ) ); // !currently not used

		bool orientationTag = CheckOrientationTag( GetDataSet() );

		if ( orientationTag )
		{
			gIface->GetMainWindow()->ReportMessage("DICOM file imported successfully", false);
		}
		else
		{
			gIface->GetMainWindow()->ReportMessage("DICOM file imported without orientation information", true);
		}

		/*if the data is a multislice (3D or 4D) it has to be loaded to cached memory, 
		it means that single file contains 3D or 4D image and will be represented as SeriesCollective object
		*/
		// build series collective map
		if (m_Reader->GetDataSet()->GetStorageType() == dcmAPI::DataSet::MULTI_SLICE_PER_FILE )
		{
			BuildSeriesCollectiveFromSeriesDataset();
		}

		//show start preview
		DoStartStep();
	}
	coreCatchExceptionsReportAndNoThrowMacro(PatientWorkingAreaWidget::ReadDataSet)
}


dcmAPI::DataSet::Pointer PatientWorkingAreaWidget::GetDataSet()
{
	return m_Reader->GetDataSet();
}


void PatientWorkingAreaWidget::RegisterReaders()
{
	// Register specific readers
	Core::IO::BaseDataEntityReader* baseReader;
	baseReader = Core::IO::DataEntityReader::GetRegisteredReader( "Core::IO::DICOMFileReader" );
	Core::IO::DICOMFileReader* DICOMReader = dynamic_cast<Core::IO::DICOMFileReader*>( baseReader );
	if ( DICOMReader )
	{
		std::list<dcmAPI::AbstractImageReader::Pointer> readers = DICOMReader->GetRegisteredReaders();
		m_Reader->ClearReaderList( );
		std::list<dcmAPI::AbstractImageReader::Pointer>::const_reverse_iterator rit;
		for ( rit = readers.rbegin() ; rit != readers.rend() ; rit++ )
		{
			m_Reader->RegisterReader( *rit );
		}
	}

}

void PatientWorkingAreaWidget::DoStartStep()
{
	try
	{
		wxTreeItemIdValue cookie;

		// expand first Patient->Study->Series
		wxTreeItemId timepointId = m_treeDicomView->GetFirstSpecifiedTreeItem( DICOMTree::TIMEPOINT, cookie, true);
		if(!timepointId.IsOk())
			return;

		// render the middle slice in the first timepoint
		int middleSlicePos = m_treeDicomView->GetChildrenCount(timepointId)/2;
		wxTreeItemId sliceId = m_treeDicomView->GetFirstChild(timepointId, cookie);
		while(middleSlicePos > 0 && sliceId.IsOk())
		{
			sliceId = m_treeDicomView->GetNextChild(timepointId, cookie);
			middleSlicePos--;
		}

		//wxTreeItemId sliceId = m_treeDicomView->GetFirstChild(timepointId, cookie);
		if(!sliceId.IsOk())
			return;

		//select a slice and render it
		m_treeDicomView->UnselectAll();
		m_treeDicomView->SelectItem(timepointId);
		//m_treeDicomView->SelectItemByIndex(sliceId);
		//LoadDefaultLevelWindow(sliceId);

		/* !currently not used
		m_RenderWindow->SetUseFixedLevelWindow(false);
		m_RenderWindow->SetUseFixedLevelWindow(true);
		*/
		UpdateDataEntity(sliceId);
	}
	coreCatchExceptionsReportAndNoThrowMacro(PatientWorkingAreaWidget::DoStartStep)
}

void PatientWorkingAreaWidget::BuildSeriesCollectiveFromSeriesDataset()
{
	try
	{
		m_Reader->SetLoadFilter( Core::IO::DICOMFileReader::LOAD_FILTER_MULTISLICE );
		m_Reader->SetPostProcessData( false );
		m_Reader->Update( );

		m_Reader->SetNumberOfInputs( m_Reader->GetNumberOfOutputs( ) );
		for ( int i = 0 ; i < m_Reader->GetNumberOfOutputs( ) ; i++ )
		{
			Core::DataEntity::Pointer dataEntity = m_Reader->GetOutputDataEntity( i );
			m_Reader->SetInputDataEntity( i, dataEntity );

			// Try with signal data
			blSignalCollective::Pointer signalCollective;
			bool success = dataEntity->GetProcessingData( signalCollective );
			if ( success )
			{
				wxTreeItemIdValue cookie;
				wxTreeItemId seriesTreeId = m_treeDicomView->GetFirstSpecifiedTreeItem(DICOMTree::SERIES, cookie);

				if ( signalCollective->GetNumberOfSignals() > 0 && seriesTreeId.IsOk() )
				{
					//attach ECG signal if it was in the data
					m_treeDicomView->AppendItem( seriesTreeId, _U("ECG"), 5, 5, NULL );
				}
			}

		}
	}
	coreCatchExceptionsReportAndNoThrowMacro( PatientWorkingAreaWidget::BuildSeriesCollectiveFromSeriesDataset )
}


bool CheckOrientationTag( dcmAPI::DataSet::Pointer dataSet )
{

	bool orientationTag = true;
	dcmAPI::PatientIdVectorPtr patiendIdVector = dataSet->GetPatientIds();
	for(unsigned i=0; i < patiendIdVector->size(); i++)
	{
		dcmAPI::Patient::Pointer patient = dataSet->GetPatient( patiendIdVector->at(i) );
		dcmAPI::StudyIdVectorPtr studiesIdVector = patient->StudyIds();
		for(unsigned i=0; i < studiesIdVector->size(); i++)
		{
			dcmAPI::Study::Pointer study = patient->Study(studiesIdVector->at(i));
			dcmAPI::SeriesIdVectorPtr seriesIdVector = study->SeriesIds();
			for(unsigned i=0; i < seriesIdVector->size(); i++)
			{
				dcmAPI::Series::Pointer series = study->Series(seriesIdVector->at(i));
				dcmAPI::TimePointIdVectorPtr timePointIdVector = series->TimePointIds();
				for(unsigned i=0; i < timePointIdVector->size(); i++)
				{
					dcmAPI::TimePoint::Pointer timePoint = series->TimePoint(timePointIdVector->at(i));
					dcmAPI::SliceIdVectorPtr sliceIdVector = timePoint->SliceIds();
					for(unsigned i=0; i < sliceIdVector->size(); i++)
					{
						dcmAPI::Slice::Pointer slice = timePoint->Slice( sliceIdVector->at(i) );

						dcmAPI::TagsMap::iterator it;
						it = slice->GetTagsMap( )->find( dcmAPI::tags::ImageOrientationPatient );
						if ( it == slice->GetTagsMap( )->end() || it->second.empty( ) )
						{
							orientationTag = false;
						}
					}
				}
			}
		}
	}

	return orientationTag;
}

void PatientWorkingAreaWidget::OnModifiedOutputDataEntity()
{
	try
    {
		Core::DataEntity::Pointer inputDataEntity;
		inputDataEntity = m_Processor->GetInputDataEntity(PatientWorkingAreaProcessor::INPUT_0 );

		// Hide input if is different from output and output is not empty
		if ( m_Processor->GetOutputDataEntity( 0 ).IsNotNull() && 
			 m_Processor->GetOutputDataEntity( 0 ) != inputDataEntity )
		{
			GetRenderingTree()->Show( inputDataEntity, false );
		}

		// Add output to the data list and render it
		// After adding the output, the input will automatically be changed to
		// this one
	/*	Core::DataTreeHelper::PublishOutput( 
			m_Processor->GetOutputDataEntityHolder( 0 ), 
			GetRenderingTree( ),
			m_selectedDataEntityHolder );*/
	
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		"PatientWorkingAreaWidget::OnModifiedOutputDataEntity")
}

void PatientWorkingAreaWidget::UpdateHelperWidget()
{
	if ( GetHelperWidget( ) == NULL )
	{
		return;
	}
    GetHelperWidget( )->SetInfo( 
        Core::Widgets::HELPER_INFO_LEFT_BUTTON, 
        " info that is useful in order to use the processor" );
}

bool PatientWorkingAreaWidget::Enable( bool enable /*= true */ )
{
	/*
	bool bReturn = PatientInfoPluginPatientWorkingAreaWidgetUI::Enable( enable );

	// If this panel widget is selected -> Update the widget
	if ( enable )
	{
		UpdateWidget();
	}

	return bReturn;
	*/

	// If this panel widget is selected -> Update the widget
	if ( enable )
	{
		UpdateWidget();
	}
	m_PatientInfoIOMenuEventHandler->SetEvtHandlerEnabled( enable );
	return PatientInfoPluginPatientWorkingAreaWidgetUI::Enable( enable );
}

void PatientWorkingAreaWidget::OnModifiedInputDataEntity()
{
	UpdateWidget();
}

Core::BaseProcessor::Pointer PatientWorkingAreaWidget::GetProcessor()
{
	//return m_Processor.GetPointer( );
	return Core::BaseProcessor::Pointer(m_Reader);

}

} //namespace patientInfoPlugin
