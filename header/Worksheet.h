/*------------------------------------------------------------------------------*
 * File Name:	Worksheet.h	 													*
 * Creation: 	Alexander Schmitz												*
 * Purpose: 	Provides methods for Origin Worksheet manipulation.				*
 * Copyright(c) 2019, Alexander Schmitz         								*
 * All Rights Reserved															*
 * 																				*
 * Last Modified:	16.02.2020													*
 * Tasks:           				    										*
 *------------------------------------------------------------------------------*/
#ifndef _WORKSHEET_ // include once
#define _WORKSHEET_

#include <Origin.h>
#include <XFBase.h>
#include "User.h"

/**
 *  Function WORKSHEET_addUserParameter
 *  Adds and shows a user defined parameter row in a worksheet.
 *  @param Worksheet wks the target worksheet
 *	@param string paramName the name of the user parameter
 *  @param int offset(0) offset of the user parameter row
 **/ 
void WOKRSHEET_addUserParameter(Worksheet wks, string paramName, int offset = 0){
	// get current label names
	Grid gg;
	gg.Attach(wks);
    vector<int> vnTypes;
    gg.GetShowLabels(vnTypes, TRUE);

	// read existing user labels
	vector<string> userLabels;
	gg.GetUserDefinedLabelNames(userLabels);

	// add new parameter name
	userLabels.Add(paramName);
    
	// set and show parameters
	vector<uint> vecIndex;
    if(vnTypes.Find(vecIndex, RCLT_UDL + offset) == 0){
		gg.SetUserDefinedLabelNames(userLabels);
		vnTypes.Add(RCLT_UDL + offset);
		gg.SetShowLabels(vnTypes);
    }

    // refresh to show labels
    wks.GetPage().Refresh();
}

/**
 *  Function WORKSHEET_addSparklines
 *  Create Sparklines for a single worksheet (adapted from XFunctions).
 *  @param Worksheet* wks the target worksheet (by reference)
 *	@return bool true/false the result of sparkline updating
 *  @warning: this function might take a lot of time to complete!
 **/ 
bool WORKSHEET_addSparklines(Worksheet &wks){
    XFBase xf("sparklines"); // create an instance of the X-Function
    if (!xf)
        return false;
 
    if (!xf.SetArg("iw", wks)) // set the input worksheet
        return false;
 
    if (!xf.SetArg("sel", 0)) // ignore selected columns
        return false;
 
    if (!xf.SetArg("c1", 1)) // start at column 1
        return false;
 
    if (!xf.SetArg("c2", wks.GetNumCols())) // use all columns in sheet
        return false;
 
    if (!xf.Evaluate()) // execute the X-Function
        return false;
 
    printf("\nDone.");
    return true;
}

/**
 * Function WORKSHEET_getYMin
 * Returns the lowest Y-value in a given worksheet. 
 * @param Worksheet wks the worksheet to read out
 **/
double WORKSHEET_getYMin(Worksheet wks){
	double min = 1E37 , colMin, colMax;
	
	// loop through columns
	for(int colInt = 0; colInt < wks.GetNumCols(); colInt++){
		Column colObj = wks.Columns(colInt);
		if(colObj.GetType() == OKDATAOBJ_DESIGNATION_Y){
			// get column data
			vectorbase &colData = colObj.GetDataObject();
			
			// find column minimum
			colData.GetMinMax(colMin, colMax);

			// store minimum value
			if(colMin < min){
				min = colMin;
			}
		}
	}

	return min;
}

/**
 * Method WORKSHEET_getUserLabelData
 * Get user label data from a worksheet by label name.
 * @param Worksheet wks the worksheet
 * @param string labelName the name of the user label to read
 **/
vector<string> WORKSHEET_getUserLabelData(Worksheet wks, string labelName){
    // get  label names
	Grid gg;
	gg.Attach(wks);	
	vector<string> labelNames;
	gg.GetUserDefinedLabelNames(labelNames);
	
	// find user label name in reference
	vector<string> labelData;
	int labelIndex = labelNames.Find(labelName);
	if(labelIndex < 0){	// cancel if user label not exists
		printf("User Label '" + labelName + "' not found in worksheet '" + wks.GetName() + "'.\n");
	} else {
		// get reference label data
		gg.GetLabelsByType(labelData, RCLT_UDL + labelIndex);
	}

    return labelData;
}

/**
 * Method WORKSHEET_setLowerBound
 * Define lower bound for values in a worksheet.
 * @param Worksheet wks the worksheet
 * @param double lowerBound the lower bound of allowed values in the worksheet
 **/
void WORKSHEET_setLowerBound(Worksheet wks, double lowerBound){
	// get column designations
    string colTypes = wks.GetColDesignations();

	// loop through all columns
	int xInt = 0, yInt = 0;
    Column dataCol;
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++){
		// only manipulate Y-Columns
		if(colTypes.GetAt(colInt) == 'Y'){
			// Replace lower bounds
            dataCol = wks.Columns(colInt);
			vectorbase &dataVec = dataCol.GetDataObject();
			for(int rowInt = 0; rowInt < dataVec.GetSize(); rowInt++){
  			    dataVec[rowInt] = (dataVec[rowInt] < lowerBound || dataVec[rowInt] == NULL) ? lowerBound : dataVec[rowInt];
			}
		} else {
			continue;
		}
	}
}

#endif