/*------------------------------------------------------------------------------*
 * File Name:	main.c 															*
 * Creation: 	Alexander Schmitz												*
 * Purpose:		Data Handling/Evaluation										*
 * Copyright(c) 2020, Alexander Schmitz         								*
 * All Rights Reserved															*
 * 																				*
 * Last Modified:	26.01.2021													*
 * Tasks:           				    										*
 *------------------------------------------------------------------------------*/
#include <Origin.h>
#include "header\Lang.h"
#include "header\User.h"
#include "header\Import.h"
#include "header\Analyse.h"
#include "header\Convert.h"
#include "header\Correct.h"

/**
 * Method import
 * Import various datasets from user input.
 **/
void import(){
	// get parameters from user
	vector<string> params;
	params = USER_importData();
	
	// import data
	if(atoi(params[0]) > -1){
		printf(IMPORT_START);

		// map user parameters
		int methodInt = atoi(params[0]);

		// read source files from user
		vector<string> strFiles;
		strFiles = USER_selectFiles();
		if(strFiles.GetSize() == 0)
			return;
			
		// call import method
		switch(methodInt){ // method switch
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
			
		case 4: // ImageJ Trackings
			IMPORT_Tracks(params, strFiles);
			break;
		}
		
		printf(IMPORT_STOP);
	} else { // user cancel
		printf(USER_PARAMS_EMPTY);
	}
}

/**
 * Method correct
 * Run different manipulation routines on worksheets in current workbook.
 **/
void correct(){
	// get targets
	Worksheet activeWks;
	WorksheetPage activeWb;
	ORIGIN_getActiveWorksheets(0, activeWb, activeWks);

	// get parameters from user
	vector<string> params;
	params = USER_correctData(activeWb);
	
	// correct data
	if(atoi(params[0]) > -1){
		// map source parameters
        Worksheet dataWks = activeWb.Layers(params[0]);

		if(dataWks){ // existing worksheet
			// run through methods
			Worksheet tgtWks;
			vector<string> dialogTitles = {USER_CORRECT_CLEAN, USER_CORRECT_BACKGROUND, USER_CORRECT_SPIKES, USER_CORRECT_SETUP, USER_CORRECT_FILTERS, USER_CORRECT_INTEGRATION, USER_CORRECT_TRANSFORM, USER_CORRECT_NORMALISE};
			vector<string> sheetTitles = {USER_CORRECT_SHEET_CLEAN, USER_CORRECT_SHEET_BACKGROUND, USER_CORRECT_SHEET_SPIKES, USER_CORRECT_SHEET_SETUP, USER_CORRECT_SHEET_FILTERS, USER_CORRECT_SHEET_INTEGRATION, USER_CORRECT_SHEET_TRANSFORM, USER_CORRECT_SHEET_NORMALISE};
			for(int i = 1; i <= 8; i++) {
				if(atoi(params[i]) == 1){
					vector<string> userParams;
					userParams = USER_correctDataSource(activeWb, dataWks, i, dialogTitles[i - 1]);
					if(atoi(userParams[0]) > -1){
						// duplicate data sheet
						tgtWks = ORIGIN_createWks(activeWb, sheetTitles[i - 1], true);
						wks_copy(tgtWks, dataWks, CREATE_VISIBLE_SAME, DCTRL_COPY_GRID | DCTRL_COPY_DATA);

						// run method
						switch(i){
							case 1: // clean masked data
								CORRECT_masked(tgtWks);
								break;

							case 2: // background
								if(atof(userParams[0]) == 0){ // reference
									CORRECT_backgroundRef(tgtWks, activeWb.Layers(userParams[1]), userParams[2]);
								} else { // average
									CORRECT_backgroundAve(tgtWks, atof(userParams[3]), atof(userParams[4]));
								}
								break;

							case 3: // spikes
								CORRECT_spikes(tgtWks, atof(userParams[0]), atoi(userParams[1]));
								break;

							case 4: // setup
								CORRECT_setup(tgtWks, activeWb.Layers(userParams[0]));
								break;

							case 5: // filters
								CORRECT_filters(tgtWks, activeWb.Layers(userParams[0]), userParams[1]);
								break;

							case 6: // integration time
								CORRECT_integrationTime(tgtWks, userParams[0]);
								break;

							case 7: // energy transformation
								CORRECT_transform(tgtWks);
								break;

							case 8: // normalisation
								CORRECT_normalise(tgtWks);
								break;
						}

						// make sure comments are shown
						Grid gg;
						gg.Attach(tgtWks);	
						gg.ShowLabels(RCLT_COMMENT);

						// assign new data sheet
						dataWks = tgtWks;
					} else {
						printf(USER_PARAMS_EMPTY + " (" + dialogTitles[i - 1] + ")");
					}
				}
			}
		} else { // wrong input
			printf(USER_CORRECT_NODATAWKS);
		}
	} else { // user cancel
		printf(USER_PARAMS_EMPTY);
	}
}

/**
 * Method analyse
 * Run data analyse on a workbook or worksheet.
 **/
void analyse(){
	// get parameters from user
	vector<string> params;
	params = USER_analyse();
	
	// analyse data
	if(atoi(params[0]) > -1){
		printf(ANALYSIS_START);

		// map source parameters
		int methodInt = atoi(params[0]);
		int sourceTypeInt = atoi(params[1]);
	
		// get targets
		Worksheet activeWks;
		WorksheetPage activeWb;
		vector<int> sourceWksInts;
		sourceWksInts = ORIGIN_getActiveWorksheets(sourceTypeInt, activeWb, activeWks);

		// loop through source layers
		Worksheet wks;
		for(int i = 0; i < sourceWksInts.GetSize(); i++){
			wks = activeWb.Layers(sourceWksInts[i]);
			if(wks.GetName() == ANALYSIS_TARGET)
				continue;

			// call analysis method
			switch(methodInt){ // method switch
			case 0: // Spectra
				ANALYSE_spectra(wks);
				break;
			
			case 1: // 4D Linescan
				MAP_4D_Linescan(wks);
				break;
			}
		}
		printf(ANALYSIS_STOP);
	} else { // user cancel
		printf(USER_PARAMS_EMPTY);
	}
}

/**
 * Method convert
 * Convert data to or from maps (3D/4D).
 **/
void convert(){
	// get parameters from user
	vector<string> params;
	params = USER_convert();
	
	// handle data
	if(atoi(params[0]) > -1){
		printf(CONVERSION_START);

		// map user parameters
		int methodInt = atoi(params[0]);
			
		// get targets
		Worksheet activeWks = Project.ActiveLayer();
		if(!activeWks){
			printf(ANALYSIS_NO_WKS);
			return;
		}

		// call conversion method
		switch(methodInt){ // method switch
		case 0: // convert XYZ to matrix
			// get conversion parameters
			vector<string> mapParams;	
			mapParams = USER_xyzMatrix();
			if(atoi(mapParams[0]) > -1){
				CONVERT_XYZtoMatrix(activeWks, mapParams);
			} else{ // user cancel
				printf(USER_PARAMS_EMPTY);
				return;
			}
			break;
		}
		printf(CONVERSION_STOP);
	} else { // user cancel
		printf(USER_PARAMS_EMPTY);
	}
}

/**
 * Method peaks
 * Collect and resort specific data columns from multiple peak analysis.
 **/
void peaks(){
	// get active page
	Worksheet activeWks;
	WorksheetPage activeWb;
	ORIGIN_getActiveWorksheets(0, activeWb, activeWks);

	// collect valid sheets in page
	vector<string> sheetNames;
	foreach(Layer tempWks in activeWb.Layers){
		// only peak summary sheets are valid
		string tmpName = tempWks.GetName();
		if(tmpName.Match("PeakProperties*")){
			sheetNames.Add(tmpName);
		}
	}
	
	// exit if no valid sheets
	if(sheetNames.GetSize() < 1){
		printf(PEAKS_NO_WKS);
		return;
	}
	
	// get parameters from user
	vector<string> params;
	params = USER_peaks(activeWb, sheetNames);
	
	// analyse data
	if(atoi(params[0]) > -1){
		printf(PEAKS_START);

		// map source parameters
		string identifier = params[0];
		string columnName = params[1];

		// evaluate peaks
		ANALYSE_collectPeaks(activeWb, sheetNames, columnName, identifier);

		printf(PEAKS_STOP);
	} else { // user cancel
		printf(USER_PARAMS_EMPTY);
	}
}

/**
 * Method alignText
 * Align a text field in the active graph window.
 **/
void alignText(){
	// call method
	MISC_alignGraphText();
	
	// user information
	printf(ALIGN_STOP);
}

/**
 * Method renameWbs
 * Rename all Workbooks in active folder with their long names.
 **/
void renameWbs(){
    Folder fld = Project.ActiveFolder();
    PageBase pb;

    foreach(pb in fld.Pages){
      pb.Rename(page_get_display_name(pb, FALSE), TRUE, TRUE);
    }

    printf(RENAME_STOP);
}


/**
 * Method setLowerBound
 * Sets the lower bound of allowed values in the current worksheet.
 * @param double lowerBound the lowest allowed value
 **/
void setLowerBound(double lowerBound = 0){
	// get targets
	Worksheet activeWks;
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
 * Method reduce
 * Reduces CPU load by removing all sparklines from project and hiding all page windows.
 **/
void reduce(){
	// loop all pages
    foreach(PageBase pb in Project.Pages){
    	if(pb.GetName().Find("sparkline") > -1){
			// delete sparkline
		    pb.Destroy();
		} else {
			// hide other pages
			pb.Show = false;
        }
    }
    Project.Save();
}