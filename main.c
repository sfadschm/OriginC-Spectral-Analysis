/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#include <Origin.h>
#include "header\Analyze.h"
#include "header\Correct.h"
#include "header\Import.h"
#include "header\Lang.h"
#include "header\Map.h"
#include "header\User.h"

/**
 * This is the package's main file.
 * All methods defined here should be called directly from the command window.
 */

/**
 * Import various datasets based on user input.
 */
void import()
{
	// get parameters from user
	vector<string> params;
	params = USER_importData();

	// abort if user dialog cancelled
	if(params[0] == "-1")
	{
		printf(PARAMS_EMPTY);
		return;
	}

	// user information
	printf(IMPORT_START);

	// map user parameters
	int methodInt = atoi(params[0]);

	// read source files from user
	vector<string> strFiles;
	strFiles = USER_selectFiles();

	// abort if no file selected
	if(strFiles.GetSize() == 0)
	{
		return;
	}

	// call import method
	switch(methodInt)
	{
		case 0: // Spectra
			IMPORT_spectra(params, strFiles);
			break;

		case 1: // 3D Map (Streak)
			IMPORT_3dMaps(params, strFiles);
			break;

		case 2: // 4D Map (LabView)
			IMPORT_LabViewMaps(params, strFiles);
			break;

			case 3: // 4D Map (NT-MDT)
			IMPORT_4dMaps(params, strFiles);
			break;

		case 4: // ImageJ Tracks
			IMPORT_Tracks(params, strFiles);
			break;
	}

	// user information
	printf(IMPORT_STOP);
}

/**
 * Run different manipulation routines on worksheets in current workbook.
 */
void correct()
{
	// get targets
	Worksheet     activeWks;
	WorksheetPage activeWb;
	ORIGIN_getActiveWorksheets(0, activeWb, activeWks);

	// get parameters from user
	vector<string> params;
	params = USER_correctData(activeWb);

	// abort if user dialog cancelled
	if(params[0] == "-1")
	{
		printf(PARAMS_EMPTY);
		return;
	}

	// map source parameters
	Worksheet dataWks = activeWb.Layers(params[0]);

	// abort if not a worksheet
	if(!dataWks)
	{
		printf(CORRECT_NODATAWKS);
		return;
	}

	// prepare dialog titles/texts
	vector<string> dialogTitles = {CORRECT_CLEAN,       CORRECT_BACKGROUND,       CORRECT_SPIKES,       CORRECT_SETUP,       CORRECT_FILTERS,       CORRECT_INTEGRATION,       CORRECT_TRANSFORM,       CORRECT_NORMALISE};
	vector<string> sheetTitles  = {CORRECT_SHEET_CLEAN, CORRECT_SHEET_BACKGROUND, CORRECT_SHEET_SPIKES, CORRECT_SHEET_SETUP, CORRECT_SHEET_FILTERS, CORRECT_SHEET_INTEGRATION, CORRECT_SHEET_TRANSFORM, CORRECT_SHEET_NORMALISE};

	// run through methods
	Worksheet tgtWks;
	for(int i = 1; i <= dialogTitles.GetSize(); i++)
	{
		if(atoi(params[i]) == 1)
		{
			// read user parameters for current step
			vector<string> userParams;
			userParams = USER_correctDataSource(activeWb, dataWks, i, dialogTitles[i - 1]);

			// abort if parameter dialogue cancelled
			if(userParams[0] == "-1")
			{
				printf(PARAMS_EMPTY + " (" + dialogTitles[i - 1] + ")");
				return;
			}

			// duplicate data sheet
			tgtWks = ORIGIN_createWks(activeWb, sheetTitles[i - 1], true);
			wks_copy(tgtWks, dataWks, CREATE_VISIBLE_SAME, DCTRL_COPY_GRID | DCTRL_COPY_DATA);

			// run correction method
			switch(i)
			{
				case 1: // clean masked data
					CORRECT_masked(tgtWks);
					break;

				case 2: // subtract background
					switch(atof(userParams[0]))
					{
						case 0: // reference mode
							CORRECT_backgroundRef(tgtWks, activeWb.Layers(userParams[1]), userParams[2]);
							break;
									
						case 1: // median mode
							CORRECT_backgroundMedian(tgtWks, atof(userParams[1]), atof(userParams[2]));
							break;

						case 2: // constant mode
							CORRECT_backgroundConstant(tgtWks, atof(userParams[1]));
							break;
					}
					break;

				case 3: // automatically remove spikes
					CORRECT_spikes(tgtWks, atof(userParams[0]), atoi(userParams[1]));
					break;

				case 4: // correct setup efficiency
					CORRECT_setup(tgtWks, activeWb.Layers(userParams[0]), userParams[1]);
					break;

				case 5: // correct filter transmission
					CORRECT_filters(tgtWks, activeWb.Layers(userParams[0]), userParams[1]);
					break;

				case 6: // correct integration time
					CORRECT_integrationTime(tgtWks, userParams[0]);
					break;

				case 7: // perform energy transformation
					CORRECT_transform(tgtWks);
					break;

				case 8: // perform data normalisation
					CORRECT_normalise(tgtWks);
					break;
			}

			// ensure that comments are shown
			Grid gg;
			gg.Attach(tgtWks);
			gg.ShowLabels(RCLT_COMMENT);

			// assign new data sheet (for recursion)
			dataWks = tgtWks;
		}
	}
}

/**
 * Run basic data analysis on a workbook or worksheet.
 */
void analyze()
{
	// user information
	printf(ANALYZE_START);

	// get targets
	Worksheet     activeWks;
	WorksheetPage activeWb;
	vector<int>   sourceWksInts;
	sourceWksInts = ORIGIN_getActiveWorksheets(1, activeWb, activeWks);

	// loop through source layers
	Worksheet wks;
	for(int i = 0; i < sourceWksInts.GetSize(); i++)
	{
		wks = activeWb.Layers(sourceWksInts[i]);

		// skip existing result sheets
		if(wks.GetName() == ANALYZE_SPECTRA_TARGET)
		{
			continue;
		}

		// spectral analysis
		ANALYZE_spectra(wks);
	}

	// user information
	printf(ANALYZE_STOP);
}

/**
 * Alias for 'analyze'
 *
 * @deprecated use analyze
 */
void analyse()
{
	analyze();
}

/**
 * Handle, convert or extract data from maps.
 */
void map(){
	// get parameters from user
	vector<string> params;
	params = USER_map();

	// abort if user dialog cancelled
	if(params[0] == "-1")
	{
		printf(PARAMS_EMPTY);
		return;
	}

	// user information
	printf(MAP_START);

	// map user parameters
	int methodInt = atoi(params[0]);

	// get targets
	Worksheet activeWks = Project.ActiveLayer();

	// abort if not a worksheet
	if(!activeWks)
	{
		printf(ANALYZE_NO_WKS);
		return;
	}

	// call conversion method
	switch(methodInt)
	{
		case 0: // convert XYZ data to matrix
			// run conversion
			MAP_XYZtoMatrix(activeWks);
			break;
			
		case 1: // 4D linescan
			MAP_4D_Linescan(activeWks);
			break;

	}

	// user information
	printf(MAP_STOP);
}

/**
 * Project all data in the current worksheet onto a new x-axis.
 */
void interpolate(){
	// get active page
	Worksheet     activeWks;
	WorksheetPage activeWb;
	ORIGIN_getActiveWorksheets(0, activeWb, activeWks);

	// get parameters from user
	vector<string> params;
	params = USER_interpolate(activeWks);

	// abort if user dialog cancelled
	if(params[0] == "-1")
	{
		printf(PARAMS_EMPTY);
		return;
	}

	// map user parameters
	int newXInt = atoi(params[0]);

	// interpolate data
	CORRECT_interpolate(activeWb, activeWks, newXInt);
}

/**
 * Collect and resort selected data columns from multiple peak analysis result sheets.
 */
void peaks(){
	// get active page
	Worksheet     activeWks;
	WorksheetPage activeWb;
	ORIGIN_getActiveWorksheets(0, activeWb, activeWks);

	// collect valid sheets in active page
	vector<string> sheetNames;
	foreach(Layer tempWks in activeWb.Layers)
	{
		// only peak summary sheets are valid
		string tmpName = tempWks.GetName();
		if(tmpName.Match("PeakProperties*"))
		{
			sheetNames.Add(tmpName);
		}
	}

	// abort if no valid sheets available
	if(sheetNames.GetSize() < 1){
		printf(PEAKS_NO_WKS);
		return;
	}

	// get parameters from user
	vector<string> params;
	params = USER_peaks(activeWb, sheetNames);

	// abort if user dialog cancelled
	if(params[0] == "-1")
	{
		printf(PARAMS_EMPTY);
		return;
	}

	// user information
	printf(PEAKS_START);

	// map source parameters
	string identifier = params[0];
	string columnName = params[1];

	// evaluate peaks
	ANALYZE_collectPeaks(activeWb, sheetNames, columnName, identifier);

	// user information
	printf(PEAKS_STOP);
}

/**
 * Rename all Workbooks in active folder with their long names.
 */
void renameWbs(){
	// get active folder
	Folder fld = Project.ActiveFolder();

	// loop through workbook pages in folder
	PageBase pb;
	foreach(pb in fld.Pages)
	{
		pb.Rename(page_get_display_name(pb, FALSE), TRUE, TRUE);
	}

	// user information
	printf(RENAME_STOP);
}


/**
 * Set the lower bound of allowed values in the current worksheet to a defined value.
 *
 * @param double lowerBound the lowest allowed value
 */
void setLowerBound(double lowerBound = 0){
	// get targets
	Worksheet     activeWks;
	WorksheetPage activeWb;
	ORIGIN_getActiveWorksheets(0, activeWb, activeWks);

	// user information
	printf(LOWERBOUND_START);

	// run method
	WORKSHEET_setLowerBound(activeWks, lowerBound);

	// user information
	printf(LOWERBOUND_STOP, lowerBound);
}

/**
 * Reduce CPU load by removing all sparklines from the project and hiding all page windows.
 */
void reduce(){
	// loop through all pages
	foreach(PageBase pb in Project.Pages){
		if(pb.GetName().Find("sparkline") > -1)
		{
			// delete sparkline
			pb.Destroy();
		}
		else
		{
			// hide other pages
			pb.Show = false;
		}
	}

	// save project
	Project.Save();

	// user information
	printf(REDUCE_STOP);
}