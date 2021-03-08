/*------------------------------------------------------------------------------*
 * File Name:	Worksheet.h                                                     *
 * Creation:	Alexander Schmitz                                               *
 * Purpose:		Provides methods for Origin Worksheet manipulation.             *
 * Copyright(c) 2021, Alexander Schmitz                                         *
 * All Rights Reserved                                                          *
 *                                                                              *
 * Last Modified:	08.03.2021                                                  *
 * Tasks:                                                                       *
 *------------------------------------------------------------------------------*/
#ifndef _WORKSHEET_ // include once
#define _WORKSHEET_

#include <Origin.h>
#include <XFBase.h>
#include "User.h"

/**
 * Add and show a user defined parameter row in a worksheet.
 *
 * @param Worksheet wks       the target worksheet
 * @param string    paramName the name of the user parameter
 * @param int       offset(0) offset of the user parameter row
 **/
void WOKRSHEET_addUserParameter(Worksheet wks, string paramName, int offset = 0)
{
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
	if(vnTypes.Find(vecIndex, RCLT_UDL + offset) == 0)
	{
		gg.SetUserDefinedLabelNames(userLabels);
		vnTypes.Add(RCLT_UDL + offset);
		gg.SetShowLabels(vnTypes);
	}

	// refresh to show labels
	wks.GetPage().Refresh();
}

/**
 * Create Sparklines for a single worksheet.
 *
 * @param Worksheet* wks the target worksheet (by reference)
 *
 * @return bool true/false the result of sparkline updating
 *
 * @warning: this function might take a lot of time to complete!
 **/
bool WORKSHEET_addSparklines(Worksheet &wks)
{
	// create an instance of the X-Function
	XFBase xf("sparklines");

	// abort if X-Function not loaded
	if (!xf)
	{
		return false;
	}

	// set parameters
	if (!xf.SetArg("iw", wks) || !xf.SetArg("sel", 0) || !xf.SetArg("c1", 1) || !xf.SetArg("c2", wks.GetNumCols()))
	{
		return false;
	}

	// execute the X-Function
	if (!xf.Evaluate())
	{
		return false;
	}

	//user information
	printf("\nDone.");

	return true;
}

/**
 * Get the lowest Y-value in a given worksheet.
 *
 * @param Worksheet wks the worksheet to read out
 **/
double WORKSHEET_getYMin(Worksheet wks)
{
	// setup variables
	double min = 1E37 , colMin, colMax;

	// loop through columns
	for(int colInt = 0; colInt < wks.GetNumCols(); colInt++)
	{
		// get source column
		Column colObj = wks.Columns(colInt);

		// skip x/z-data
		if(colObj.GetType() != OKDATAOBJ_DESIGNATION_Y)
		{
			continue;
		}

		// get column data
		vectorbase &colData = colObj.GetDataObject();

		// search column minimum
		colData.GetMinMax(colMin, colMax);

		// store minimum value
		if(colMin < min)
		{
			min = colMin;
		}
	}

	return min;
}

/**
 * Get user label data from a worksheet by label name.
 *
 * @param Worksheet wks       the worksheet
 * @param string    labelName the name of the user label to read
 **/
vector<string> WORKSHEET_getUserLabelData(Worksheet wks, string labelName)
{
	// get  label names
	Grid gg;
	gg.Attach(wks);	
	vector<string> labelNames;
	gg.GetUserDefinedLabelNames(labelNames);

	// find user label name in reference
	vector<string> labelData;
	int labelIndex = labelNames.Find(labelName);

	// abort if user label does not exist
	if(labelIndex < 0)
	{
		printf("User Label '" + labelName + "' not found in worksheet '" + wks.GetName() + "'.\n");
		return labelData;
	}

	// get reference label data
	gg.GetLabelsByType(labelData, RCLT_UDL + labelIndex);

	return labelData;
}

/**
 * Define lower bound for values in a worksheet.
 *
 * @param Worksheet wks        the worksheet
 * @param double    lowerBound the lower bound of allowed values in the worksheet
 **/
void WORKSHEET_setLowerBound(Worksheet wks, double lowerBound)
{
	// get column designations
	string colTypes = wks.GetColDesignations();

	// loop through all columns
	int xInt = 0, yInt = 0;
	Column dataCol;
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++)
	{
		// skip x/z-data
		if(colTypes.GetAt(colInt) != OKDATAOBJ_DESIGNATION_Y){
			continue;
		}

		// replace lower bounds
		dataCol = wks.Columns(colInt);
		vectorbase &dataVec = dataCol.GetDataObject();
		for(int rowInt = 0; rowInt < dataVec.GetSize(); rowInt++)
		{
			dataVec[rowInt] = (dataVec[rowInt] < lowerBound || dataVec[rowInt] == NULL) ? lowerBound : dataVec[rowInt];
		}
	}
}

#endif