/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _MAP_ // include once
#define _MAP_

/**
 * This file provides helper methods for analyzing map data.
 */
#include <Origin.h>

/**
 * Extract a linescan from a map of XY-I data.
 *
 * @param WorksheetPage wb the workbook to extract the line from
 */
void MAP_4D_Linescan(Worksheet wks)
{
	// get user parameters
	vector<string> params;
	params = USER_map4dLinescan(wks);

	// abort if user dialog cancelled
	if(params[0] == "-1")
	{
		printf(PARAMS_EMPTY);
		return;
	}

	// map user parameters
	int    scanAxisInt = atof(params[0]); // scan axis (X, Y)
	int    xParamInt   = atof(params[1]); // user parameter index X
	int    yParamInt   = atof(params[2]); // user parameter index Y
	int    precision   = atof(params[3]); // the corrdinate precision
	string scanCoord   = params[4];       // scan center position string
	//int    scanWidth   = atof(params[5]); // scan width in lines/rows

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
	if(scanAxisInt == 0)
	{
		scanAxisData  = yStrV;
		labelAxisData = xStrV;
		scanAxisLabel = labelNamesV[0];
	}
	else
	{
		scanAxisData  = xStrV;
		labelAxisData = yStrV;
		scanAxisLabel = labelNamesV[1];
	}

	// create new result worksheetpage if neccessary
	WorksheetPage tgtWb = ORIGIN_createWb(MAP_LINESCAN_NAME);

	// create target woksheet
	vector<string> axes       = {"X", "Y"};
	string         tgtWksName = wks.GetName() + "_" + axes[scanAxisInt] + scanCoord;
	Worksheet      tgtWks     = ORIGIN_createWks(tgtWb, tgtWksName, true);

	// add user parameter
	WOKRSHEET_addUserParameter(tgtWks, scanAxisLabel);

	// get x-column
	int        tgtLColInt = tgtWks.AddCol();
	Column     tgtLCol    = tgtWks.Columns(tgtLColInt);

	// transfer x-data
	vectorbase &lData = tgtLCol.GetDataObject();
	lData = wks.Columns(0).GetDataObject();

	// set column labels
	tgtLCol.SetType(OKDATAOBJ_DESIGNATION_X);
	tgtLCol.SetLongName(wks.Columns(0).GetLongName());
	tgtLCol.SetUnits(wks.Columns(0).GetUnits());

	// loop source data
	int    tgtIColInt;
	Column tgtICol;
	for(int i = 1; i < wks.GetNumCols(); i++){
		if(round(atof(scanAxisData[i]), precision) == round(atof(scanCoord), precision)){ // part of the linescan
			// create column
			tgtIColInt = tgtWks.AddCol();
			tgtICol    = tgtWks.Columns(tgtIColInt);

			// transfer intensity data
			vectorbase &iData = tgtICol.GetDataObject();
			iData = wks.Columns(i).GetDataObject();

			// set column labels
			tgtICol.SetLongName(wks.Columns(i).GetLongName());
			tgtICol.SetUnits(wks.Columns(i).GetUnits());
			tgtICol.SetExtendedLabel(labelAxisData[i], RCLT_UDL);
		}
	}
}

/**
 * Generate matrixes from XYZ columns by defining the colspan.
 *
 * @param Worksheet wks the source worksheet
 */
void MAP_XYZtoMatrix(Worksheet wks)
{
	// get conversion parameters
	vector<string> params;
	params = USER_xyzMatrix();

	// abort if parameter dialogue cancelled
	if(params[0] == "-1"){
		printf(PARAMS_EMPTY);
		return;
	}	
	
	// dummy variable
	int empty;

	// map user parameters
	string zRangeName = params[0];
	int    useCols    = atoi(params[1]);
	string xRangeName = params[2];
	string yRangeName = params[3];
	int    numCols    = atoi(params[4]);
	string stepUnit   = params[5];
	double xStep      = atof(params[6]);
	double yStep      = atof(params[7]);

	// get Z data range
	DataRange zRange;
	okxf_init_range_from_string(&zRange, zRangeName);

	// get Z column indexes
	int         zColStart, zColStop;
	vector<int> zColNums;
	find_input_range_bounding_box(zRange, empty, zColStart, empty, zColStop, wks, "X");
	for(int kk = zColStart; kk <= zColStop; kk++)
	{
		zColNums.Add(kk);
	}

	// create new matrixpage
	MatrixPage mp;
	mp = ORIGIN_createMp(XYZ_MATRIX_SHEET_NAME);

	// create new matrix layer
	MatrixLayer ml;
	ml = ORIGIN_createMl(mp, wks.GetLongName());

	// prepare XY data from columns or user parameters
	int    numRows;
	double xMin, xMax, yMin, yMax;
	string xName, xUnit, yName, yUnit;
	if(useCols == 1)
	{
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
		vector<int>  xDupI, yDupI;
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
	}
	else
	{
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
	tr.Root.Dimensions.X.Unit.strVal     = xUnit;
	tr.Root.Dimensions.Y.LongName.strVal = yName;
	tr.Root.Dimensions.Y.Unit.strVal     = yUnit;
	if(ml.UpdateThemeIDs(tr.Root) == 0)
	{
		ml.ApplyFormat(tr, true, true);
	}

	// loop through Z columns
	for(int ii = 0; ii < zColNums.GetSize(); ii++)
	{
		// set current matrix object
		MatrixObject mo  = ml.MatrixObjects(ii);
		matrixbase   &mb = mo.GetDataObject();

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
}

#endif