/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _ANALYZE_ // include once
#define _ANALYZE_

/**
 * This file provides methods for analyzing spectral datasets.
 */
#include <Origin.h>
#include "Lang.h"
#include "User.h"
#include "Curve.h"
#include "Origin.h"
#include "Map.h"

/**
 * Run spectral analysis on a worksheet layer.
 *
 * @param Worksheet wks the worksheet to analyze
 */
void ANALYZE_spectra(Worksheet wks)
{
	// get analysis parameters
	vector<string> params;
	params = USER_analyzeSpectra(wks);

	// abort if dialogue cancelled
	if(params[0] == "-1")
	{
		printf(PARAMS_EMPTY);
		return;
	}

	// map user parameters
	int methodInt      = atoi(params[0]);
	string strDataname = params[1];
	double startX      = atof(params[2]);
	double stopX       = atof(params[3]);
	int skipParams     = atoi(params[4]);
	int xParamInt      = atoi(params[5]);
	int yParamInt      = atoi(params[6]);

	// create new result worksheet if necessary
	WorksheetPage wb  = wks.GetPage();
	Worksheet evalWks = ORIGIN_createWks(wb, ANALYZE_SPECTRA_TARGET);

	// create new temporary sheet for calculations
	Worksheet tmpWks = ORIGIN_createWks(wb, "temp");
	tmpWks.AddCol();
	tmpWks.AddCol();

	// loop through all columns
	vector<double> result, yData;
	for(int colInt = 0; colInt < wks.GetNumCols(); colInt++)
	{
		// skip x/z-data and empty columns
		if(wks.Columns(colInt).GetType() != OKDATAOBJ_DESIGNATION_Y || wks.Columns(colInt).GetUpperBound() == -1)
		{
			continue;
		}

		// get curve
		Curve rawCurve(wks, colInt);

		// truncate curve
		Curve dataCurve;
		CURVE_truncate(rawCurve, dataCurve, tmpWks, startX, stopX);

		// evaluate worksheet
		switch(methodInt)
		{
			case 0: // peak position
				result.Add(xatymax(dataCurve));
				break;

			case 1: // mass centre
				vector<double> xDataV, yDataV;
				dataCurve.CopyData(xDataV, yDataV);

				vector<double> weightedAreaV;
				weightedAreaV = xDataV * yDataV;

				double weightedY, totalY;
				weightedAreaV.Sum(weightedY);
				yDataV.Sum(totalY);

				result.Add(weightedY/totalY);
				break;

			case 2: // peak intensity
				result.Add(max(dataCurve));
				break;

			case 3: // peak area
				result.Add(area(dataCurve));
				break;

			case 4: // peak FWHM
				result.Add(fwhm(dataCurve));
				break;

			default: // user cansel
				printf(ANALYZE_NO_METHOD);
		}
	}

	// remove temporary worksheet
	tmpWks.Delete();

	// activate source worksheet
	set_active_layer(wks);

	// do we want to add abscissa data?
	if(skipParams == 0){
		// create x-data
		ANALYZE_createAbscissa(wks, evalWks, OKDATAOBJ_DESIGNATION_X, xParamInt, result.GetSize());

		// create y-data if desired
		if(yParamInt > 0){
			ANALYZE_createAbscissa(wks, evalWks, OKDATAOBJ_DESIGNATION_Y, yParamInt - 1, result.GetSize());
		}
	} else {
		// fake y-abscissa if last column is a z-column
		string colTypes = evalWks.GetColDesignations();
		if(colTypes.Right(1) == "Z"){
			yParamInt = 1;
		} else {
			yParamInt = 0;
		}
	}
	
	// create ordinate column
	int tgtColumnInt = evalWks.AddCol();
	evalWks.Columns(tgtColumnInt).SetLongName(strDataname);
	if(yParamInt > 0){
		evalWks.Columns(tgtColumnInt).SetType(OKDATAOBJ_DESIGNATION_Z);
	} else {
		evalWks.Columns(tgtColumnInt).SetType(OKDATAOBJ_DESIGNATION_Y);
	}

	// assign new dataobject
	vectorbase &tgtColumnData = evalWks.Columns(tgtColumnInt).GetDataObject();

	// paste ordinates
	tgtColumnData = result;

	// recursion until user cancel
	ANALYZE_spectra(wks);
}

/**
 * Extract abscissa data for analysis from column index or user label and paste into result sheet.
 *
 * @param Worksheet wks         the worksheet to analyze
 * @param Worksheet evalWks     the worksheet holding the results
 * @param int       designation the intended designation of the abscissa
 * @param int       labelInt    the index of the user label holding abscissa data
 * @param int       dataSize    the size of the evaluation results (for indexing)
 */
void ANALYZE_createAbscissa(Worksheet wks, Worksheet evalWks, int designation, int labelInt, int dataSize){
	// prepare grid for extracting abscissa data
	Grid gg;
	gg.Attach(wks);
	
	// extract label names
	vector<string> labelNames;
	gg.GetUserDefinedLabelNames(labelNames);

	// prepare abscissa holders
	string labelName, labelUnit = "";
	vector<double> labelData;

	// generate abscissa from index
	if(labelInt == 0)
	{
		// create numeric index 
		labelName = ANALYZE_GENERIC_INDEX;
		labelData.Data(1, dataSize, 1);
	} 
	
	// extract abscissa from labels
	if(labelInt > 0)
	{
		// resolve label name
		labelName = labelNames[labelInt - 1];
		
		// try to extract label unit
		int unitStart = labelName.Find("(");
		int unitEnd = labelName.Find(")");
		if(unitStart > -1 && unitEnd >  unitStart){
			unitStart = unitStart + 1;
			labelUnit = labelName.Mid(unitStart, unitEnd - unitStart);
			labelName = labelName.Left(unitStart - 1);
		}

		// get label data
		vector<string> labelDataStr;
		gg.GetLabelsByType(labelDataStr, RCLT_UDL + (labelInt - 1));

		// remove x-axes
		for(int colInt = wks.Columns().GetSize() - 1; colInt >= 0; colInt--)
		{
			// skip y-data
			if(wks.Columns(colInt).GetType() == OKDATAOBJ_DESIGNATION_Y)
			{
				continue;
			}

			labelDataStr.RemoveAt(colInt);
		}

		// convert to numeric values
		convert_str_vector_to_num_vector(labelDataStr, labelData);
	}

	// paste abscicca
	int tgtColumnInt = evalWks.AddCol();
	evalWks.Columns(tgtColumnInt).SetLongName(labelName);
	evalWks.Columns(tgtColumnInt).SetUnits(labelUnit);
	evalWks.Columns(tgtColumnInt).SetType(designation);
	vectorbase &tgtColumnData = evalWks.Columns(tgtColumnInt).GetDataObject();
	tgtColumnData = labelData;
}

/**
 * Collect and re-sort specific data columns from multiple peak analysis result sheets.
 *
 * @param WorksheetPage  wb         the active workbook page
 * @param vector<string> srcNames   the names of the data source sheets
 * @param string         columnName the name of the column to extract from the source sheets
 * @param string         identifier the base name of the result sheets to be created
 */
void ANALYZE_collectPeaks(WorksheetPage wb, vector<string> srcNames, string columnName, string identifier)
{
	// get workbook name
	string wbName = wb.GetName();

	// create worksheet for appended data
	Worksheet colWks = ORIGIN_createWks(wb, identifier + PEAKS_COLUMN_SHEET_SUFFIX);

	// generate source data range
	DataRange srcRange;
	for(int ii = 0; ii < srcNames.GetSize(); ii++)
	{
		// generate range name from sheet name (number)
		string rangeName = srcNames[ii];
		rangeName.Replace("PeakProperties", "");

		// append range to source
		srcRange.Add(rangeName, "[" + wbName + "]" + srcNames[ii] + "!" + columnName);
	}

	// extract range string from source range
	string str_range;
	srcRange.GetRangeString(str_range);

	// extract peak columns with LabTalk
	string str_append = "wAppend -r 1 irng:=" + str_range + " ow:=\"" + colWks.GetName() + "\" id:=0;";
	
	// abort if command string is too long for execution
	if(str_append.GetLength() >= 3000)
	{
		printf(PEAKS_TOO_LONG, str_append.GetLength());
		colWks.Delete();
		return;
	}
	LT_execute(str_append);

	// focus on result sheet
	set_active_layer(colWks);

	// create worksheet for transposed data
	Worksheet rowWks = ORIGIN_createWks(wb, identifier + PEAKS_ROW_SHEET_SUFFIX);

	// transpose peak data with LabTalk
	string str_transpose = "wTranspose -r 1 iw:=\"" + colWks.GetName() + "\" ow:=\"" + rowWks.GetName() + "\" exchange:=0;";
	LT_execute(str_transpose);

	// focus on transposed sheet
	set_active_layer(rowWks);

	// create worksheet for re-using data
	Worksheet tgtWks = ORIGIN_createWks(wb, identifier);
	tgtWks.AddCol();
	tgtWks.AddCol();
	tgtWks.AddCol();

	// set column designations and names
	tgtWks.SetColDesignations("XYE");
	tgtWks.Columns(0).SetLongName(PEAKS_HEADER_INDEX);
	tgtWks.Columns(1).SetLongName(PEAKS_HEADER_MEAN);
	tgtWks.Columns(2).SetLongName(PEAKS_HEADER_STDDEV);

	// fill result sheet with basic statistical data
	int srcCol = 1;
	foreach(Column rowCol in rowWks.Columns)
	{
		// each column is its own peak index
		tgtWks.SetCell(srcCol - 1, 0, srcCol);

		// set mean and stddev by formula
		tgtWks.SetCell(srcCol - 1, 1, "=mean("   + identifier + PEAKS_ROW_SHEET_SUFFIX + "!" + srcCol + ")");
		tgtWks.SetCell(srcCol - 1, 2, "=stddev(" + identifier + PEAKS_ROW_SHEET_SUFFIX + "!" + srcCol + ")");

		srcCol++;
	}

	// focus on result sheet
	set_active_layer(tgtWks);
}

#endif