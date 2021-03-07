/*------------------------------------------------------------------------------*
 * File Name:	Convert.h 														*
 * Creation: 	Alexander Schmitz												*
 * Purpose: 	Provide user functions to convert matrix data.					*
 * Copyright(c) 2021, Alexander Schmitz         								*
 * All Rights Reserved															*
 * 																				*
 * Last Modified:	14.11.2019			    									* 
 * Tasks:           				    										*
 *------------------------------------------------------------------------------*/
#ifndef _CONVERT_ // include once
#define _CONVERT_

#include <Origin.h>

/**
 * Function CONVERT_XYZtoMatrix
 * Creates matrixes from XYZ columns by defining the colspan.
 * @param Worksheet wks the source worksheet
 * @param vector<string> params the mapping parameters (user defined)
 **/
void CONVERT_XYZtoMatrix(Worksheet wks, vector<string> params){
    int empty; // dummy variable

	// start evaluation if not cancelled
	if(atoi(params[0]) > -1){
		// map user parameters
		string zRangeName = params[0];
		int useCols = atoi(params[1]);
		string xRangeName = params[2];
		string yRangeName = params[3];
		int numCols = atoi(params[4]);
		string stepUnit = params[5];
		double xStep = atof(params[6]);
		double yStep = atof(params[7]);

		// get Z data range
		DataRange zRange;
		okxf_init_range_from_string(&zRange, zRangeName);

		// get Z column indexes
		vector<int> zColNums;
		int zColStart, zColStop;
		find_input_range_bounding_box(zRange, empty, zColStart, empty, zColStop, wks, "X");
		for(int kk = zColStart; kk <= zColStop; kk++)
			zColNums.Add(kk);

		// create new matrixpage
		MatrixPage mp;
		mp = ORIGIN_createMp("Maps");

		// create new matrix layer
		MatrixLayer ml;
		ml = ORIGIN_createMl(mp, wks.GetLongName());

		// prepare XY data
		int numRows;
		double xMin, xMax, yMin, yMax;
		string xName, xUnit, yName, yUnit;
		if(useCols == 1){ // use xy-Columns
			// get XY data ranges
			DataRange xRange, yRange;
			okxf_init_range_from_string(&xRange, xRangeName);
			okxf_init_range_from_string(&yRange, yRangeName);

			// get XY column indexes
			int xColNum, yColNum;
			find_input_range_bounding_box(xRange, empty, xColNum, empty, empty, wks, "X");
			find_input_range_bounding_box(yRange, empty, yColNum, empty, empty, wks, "X");
			
			// get XY columns
			Column xCol = wks.Columns(xColNum);
			Column yCol = wks.Columns(yColNum);

			// get XY data as strings
			StringArray xColStr, yColStr;
			xCol.GetStringArray(xColStr);
			yCol.GetStringArray(yColStr);

			// find duplicate values
			vector<int> xDupI, yDupI;
			vector<uint> xR1s, yR1s;
			xColStr.FindDuplicates(xDupI, xR1s);
			yColStr.FindDuplicates(yDupI, yR1s);

			// set matrix size
			numCols = xR1s.GetSize();
			numRows = yR1s.GetSize();

			// set XY bounds
			xMin = atof(xColStr[xR1s[0]]);
			xMax = atof(xColStr[xR1s[xR1s.GetSize() - 1]]);
			yMin = atof(yColStr[yR1s[0]]);
			yMax = atof(yColStr[yR1s[yR1s.GetSize() - 1]]);

			// set matrix axes
			xName = xCol.GetLongName();
			xUnit = xCol.GetUnits();
			yName = yCol.GetLongName();
			yUnit = yCol.GetUnits();
		} else { // use user dimensions
			// set matrix size
			numRows = numCols;

			// set XY bounds
			xMin = 0;
			yMin = 0;
			xMax = numCols * xStep;
			yMax = numRows * yStep;

			// set matrix axes	
			xName = "X";
			yName = "Y";
			xUnit = yUnit = stepUnit;
		}
	
		// resize matrix layer
		ml.SetSize(zColNums.GetSize()); 

		// format matrix layer
		Tree tr;
		tr.Root.Dimensions.X.LongName.strVal = xName;
		tr.Root.Dimensions.X.Unit.strVal = xUnit;
		tr.Root.Dimensions.Y.LongName.strVal = yName;
		tr.Root.Dimensions.Y.Unit.strVal = yUnit;
		if(ml.UpdateThemeIDs(tr.Root) == 0){
			ml.ApplyFormat(tr, true, true);
		}

		// loop through Z columns
		for(int ii = 0; ii < zColNums.GetSize(); ii++){
			// set current matrix object
			MatrixObject mo = ml.MatrixObjects(ii);
			matrixbase &mb = mo.GetDataObject();

			// get column data
			Column dataCol = wks.Columns(zColNums[ii]);

			// get Z data
			vector<double> data = dataCol.GetDataObject();

			// set matrix size
			mo.SetLongName(dataCol.GetLongName());
			mo.SetUnits(dataCol.GetUnits());
			mo.SetComments(dataCol.GetComments());
			mb.SetSize(numRows, numCols);
			mo.SetXY(xMin, yMin, xMax, yMax);

			// fill matrix from vector
			mb.SetByVector(data);
		}
	} else { // user cancel
		printf(USER_PARAMS_EMPTY);
	}
}

#endif