/*------------------------------------------------------------------------------*
 * File Name:	Analyse.h 														*
 * Creation: 	Alexander Schmitz												*
 * Purpose: 	Provides user functions to perform analysis on datasets.		*
 * Copyright(c) 2021, Alexander Schmitz         								*
 * All Rights Reserved															*
 * 																				*
 * Last Modified:	11.02.2021			    									* 
 * Tasks:           				    										*
 *------------------------------------------------------------------------------*/
#ifndef _ANALYSIS_ // include once
#define _ANALYSIS_

#include <Origin.h>
#include "Lang.h"
#include "User.h"
#include "Curve.h"
#include "Origin.h"
#include "Map.h"

/**
 * Method ANALYSE_spectra
 * Run analysis on a worksheet layer.
 * @param Worksheet wks the worksheet to analyse
 **/
void ANALYSE_spectra(Worksheet wks){
	// get analysis parameters only on first 
	vector<string> params;
	params = USER_analyseSpectra(wks);
	
	// user cancel
	if(atoi(params[0]) == -1){
		printf(USER_PARAMS_EMPTY);
		return;
	}
	
	// map user parameters
	int methodInt = atoi(params[0]);
	string strDataname = params[1];
	double startX = atof(params[2]);
	double stopX = atof(params[3]);
	int paramInt = atoi(params[4]);

	// create new result worksheet if necessary
	WorksheetPage wb = wks.GetPage();
	Worksheet evalWks = ORIGIN_createWks(wb, ANALYSIS_TARGET);

    // create new temporary sheet for calculations
	Worksheet tmpWks = ORIGIN_createWks(wb, "temp");
	tmpWks.AddCol();
	tmpWks.AddCol();

	// loop through all columns
	vector<double> result, yData;
	for(int colInt = 0; colInt < wks.GetNumCols(); colInt++){
		// only process y-Data
		if(wks.Columns(colInt).GetType() != OKDATAOBJ_DESIGNATION_Y ){
			continue;
		};

		// skip empty columns
		if(wks.Columns(colInt).GetUpperBound() == -1){
			continue;
		};
		
		// get curve
		Curve rawCurve(wks, colInt);

		// truncate curve
		Curve dataCurve;
		CURVE_truncate(rawCurve, dataCurve, tmpWks, startX, stopX);
			
		// evaluate worksheet
		switch(methodInt){
			case 0: // Peak Position
				result.Add(xatymax(dataCurve));
				break;
			
			case 1: // Mass Centre
				vector<double> xDataV, yDataV;
				dataCurve.CopyData(xDataV, yDataV);
				
				vector<double> weightedAreaV;
				weightedAreaV = xDataV * yDataV;

				double weightedY, totalY;
				weightedAreaV.Sum(weightedY);
				yDataV.Sum(totalY);

				result.Add(weightedY/totalY);
				break;

			case 2: // Peak Intensity
				result.Add(max(dataCurve));
				break;
					
			case 3: // Peak Area
				result.Add(area(dataCurve));
				break;

			case 4: // Peak FWHM
				result.Add(fwhm(dataCurve));
				break;

			default:
				printf(ANALYSIS_NO_METHOD);
		}
	}

	// remove temporary worksheet
	tmpWks.Delete();

	// active source worksheet
    set_active_layer(wks);
		
	// read result x-Data
	Grid gg;
	gg.Attach(wks);	
	string labelName;
	vector<double> labelData;
	if(paramInt == 0){ // create numeric index 
		labelName = "Index";
		labelData.Data(1, result.GetSize(), 1);
	} else { // get labels from parameter row
		// resolve label name
		vector<string> labelNames;
		gg.GetUserDefinedLabelNames(labelNames);
		labelName = labelNames[paramInt - 1];

		// get label data
		vector<string> labelDataStr;
		gg.GetLabelsByType(labelDataStr, RCLT_UDL + (paramInt-1));
		
		// remove x-axes
		for(int colInt = wks.Columns().GetSize()-1; colInt >= 0; colInt--){
			if(wks.Columns(colInt).GetType() != OKDATAOBJ_DESIGNATION_Y){
				labelDataStr.RemoveAt(colInt); // remove non-y-data
			}
		}

		// convert to numeric values
		convert_str_vector_to_num_vector(labelDataStr, labelData);
	}
		
	// paste rownames into worksheet
	int tgtXColumnInt = evalWks.AddCol();
	evalWks.Columns(tgtXColumnInt).SetLongName(labelName);
	evalWks.Columns(tgtXColumnInt).SetType(OKDATAOBJ_DESIGNATION_X);
	vectorbase &tgtXColumnData = evalWks.Columns(tgtXColumnInt).GetDataObject();
	tgtXColumnData = labelData;
		
	// paste results into worksheet
	int tgtColumnInt = evalWks.AddCol();
	evalWks.Columns(tgtColumnInt).SetLongName(strDataname);
	vectorbase &tgtColumnData = evalWks.Columns(tgtColumnInt).GetDataObject();
	tgtColumnData = result;

	// recursion until user cancel
	ANALYSE_spectra(wks);
}


/**
 * Method collectPeaks
 * Collect and resort specific data columns from multiple peak analysis.
 * @param Worksheet wks the worksheet to analyse
 **/
void ANALYSE_collectPeaks(WorksheetPage wb, vector<string> srcNames, string columnName, string identifier){	
	// get workbook name
	string wbName = wb.GetName();
	
	// create worksheet for appended data
	Worksheet colWks = ORIGIN_createWks(wb, identifier + "Cols");
	
	// generate source data range
    DataRange srcRange;
    string rangeName;
    for(int ii = 0; ii < srcNames.GetSize(); ii++){
		// generate range name from sheet name (number)
		rangeName = srcNames[ii];
		rangeName.Replace("PeakProperties", "");
			
		// append range to source
		srcRange.Add(rangeName, "[" + wbName + "]" + srcNames[ii] + "!" + columnName);
	}

	// extract peak cols with LabTalk
	string str_range;
	srcRange.GetRangeString(str_range);
	string str_append = "wAppend -r 1 irng:=" + str_range + " ow:=\"" + colWks.GetName() + "\" id:=0;";
	LT_execute(str_append);
	
	// focus on result sheet
	set_active_layer(colWks);
	
	// create worksheet for transposed data
	Worksheet rowWks = ORIGIN_createWks(wb, identifier + "Rows");

	// transpose peak data
	string str_transpose = "wTranspose -r 1 iw:=\"" + colWks.GetName() + "\" ow:=\"" + rowWks.GetName() + "\" exchange:=0;";
	LT_execute(str_transpose);

	// focus on result sheet
	set_active_layer(rowWks);
	
	// create worksheet for reusing data
	Worksheet tgtWks = ORIGIN_createWks(wb, identifier);
	tgtWks.AddCol("Peak Index");
	tgtWks.AddCol("Mean");
	tgtWks.AddCol("Std. Deviation");

	// set column designations and names
	tgtWks.SetColDesignations("XYE");
	tgtWks.Columns(0).SetLongName("Peak Index");
	tgtWks.Columns(1).SetLongName("Mean");
	tgtWks.Columns(2).SetLongName("Std. Deviation");

	// fill data
	int srcCol = 1;
	foreach(Column rowCol in rowWks.Columns){
		// each column is its own peak index
		tgtWks.SetCell(srcCol - 1, 0, srcCol);

		// set mean and stddev by formula
		tgtWks.SetCell(srcCol - 1, 1, "=mean("+ identifier + "Rows" + "!" + srcCol + ")");
		tgtWks.SetCell(srcCol - 1, 2, "=stddev("+ identifier + "Rows" + "!" + srcCol + ")");

		srcCol++;
	}
	
	// focus on result sheet
	set_active_layer(tgtWks);
}

#endif