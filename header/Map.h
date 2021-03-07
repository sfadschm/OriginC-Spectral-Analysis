/*------------------------------------------------------------------------------*
 * File Name:	Map.h 															*
 * Creation: 	Alexander Schmitz												*
 * Purpose: 	Provides user functions to analyse map data.					*
 * Copyright(c) 2021, Alexander Schmitz         								*
 * All Rights Reserved															*
 * 																				*
 * Last Modified:	14.11.2019			    									* 
 * Tasks:           				    										*
 *------------------------------------------------------------------------------*/
#ifndef _MAP_ // include once
#define _MAP_

#include <Origin.h>

/**
 * Function MAP_4D_Linescan
 * Extract a linescan from a map of XY-I data.
 * @param WorksheetPage wb the workbook to extract the line from
 **/
void MAP_4D_Linescan(Worksheet wks){
	vector<string> params;	
	params = USER_map4dLinescan(wks);	

	if(atoi(params[0]) > -1){
		// map user parameters
		int scanAxisInt = atof(params[0]); // scan axis (X, Y)
		int xParamInt 	= atof(params[1]); // user parameter index X
		int yParamInt	= atof(params[2]); // user parameter index Y
		string scanCoord = params[3];      // scan center position string
		int scanWidth	= atof(params[4]); // scan width in lines/rows

		// resolve worksheet label names
		Grid gg;
		gg.Attach(wks);	
		vector<string> labelNamesV;
		gg.GetUserDefinedLabelNames(labelNamesV);

		
		// get XY data from labels
		vector<string> xStrV, yStrV;
		gg.GetLabelsByType(xStrV, RCLT_UDL + xParamInt);
		gg.GetLabelsByType(yStrV, RCLT_UDL + yParamInt);

		// remove duplicates
		vector<string> xStrVUnique, yStrVUnique;
		xStrVUnique = MISC_arrayUnique(xStrV);
		yStrVUnique = MISC_arrayUnique(yStrV);

		// set scan axis
		string scanAxisLabel;
		vector<string> scanAxisData, labelAxisData;
		if(scanAxisInt == 0){ // X
			scanAxisData = yStrV;
			labelAxisData = xStrV;
			scanAxisLabel = labelNamesV[1];
		} else {
			scanAxisData = xStrV;
			labelAxisData = yStrV;
			scanAxisLabel = labelNamesV[0];
		}

		// create new result worksheetpage if neccessary
		WorksheetPage tgtWb = ORIGIN_createWb("Linescans");

		// create target woksheet
		vector<string> axes = {"X", "Y"};
		string tgtWksName = wks.GetName() + "_" + axes[scanAxisInt] + scanCoord + "_" + ftoa(scanWidth);
		Worksheet tgtWks = ORIGIN_createWks(tgtWb, tgtWksName, true);

		// add user parameter
		WOKRSHEET_addUserParameter(tgtWks, scanAxisLabel);
		
		// transfer wavelength axis
		int tgtLColInt = tgtWks.AddCol();
		Column tgtLCol = tgtWks.Columns(tgtLColInt);
		vectorbase &lData = tgtLCol.GetDataObject();
		lData = wks.Columns(0).GetDataObject();
		tgtLCol.SetType(OKDATAOBJ_DESIGNATION_X);
		tgtLCol.SetLongName(wks.Columns(0).GetLongName());
		tgtLCol.SetUnits(wks.Columns(0).GetUnits());

		// loop source data
		int tgtIColInt;
		Column tgtICol;
		for(int i = 1; i < wks.GetNumCols(); i++){
			if(scanAxisData[i] == scanCoord){ // part of the linescan
				tgtIColInt = tgtWks.AddCol(); // create column
				tgtICol = tgtWks.Columns(tgtIColInt);
				vectorbase &iData = tgtICol.GetDataObject();
				iData = wks.Columns(i).GetDataObject(); // transfer intensity data
				tgtICol.SetLongName(wks.Columns(i).GetLongName());
				tgtICol.SetUnits(wks.Columns(i).GetUnits());
				tgtICol.SetExtendedLabel(labelAxisData[i], RCLT_UDL);
			}
		}
	} else { // user cancel
		printf(USER_PARAMS_EMPTY);
	}
}

#endif