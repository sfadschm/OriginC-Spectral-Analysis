/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _USER_ // include once
#define _USER_

/**
 * This file defines the user interaction interfaces (dialogues).
 */
#include <Origin.h>
#include <GetNBox.h>
#include "Lang.h"

/**
 * Open a single file selection dialog.
 *
 * @param string boxTitle the title of the dialog box
 *
 * @return string strFile the full path to the selected file
 */
vector<string> USER_selectFiles()
{
	// explode file types
	vector<string> fileTypes;
	str_separate(FILES_TYPES, "|", fileTypes);

	// open user dialog
	int numFiles;
	vector<string> strFiles;
	numFiles = GetMultiOpenBox(strFiles, fileTypes, NULL, NULL, FILES_TITLE);

	// check for user cancel
	if(numFiles == 0)
	{
		printf(FILES_EMPTY);
		strFiles.RemoveAt(0);
	}

	return strFiles;
}

/**
 * Open a multiple input box to setup data import.
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_importData()
{
	// name holders
	string wbNames, wksNames;

	// get Worksheet(page) names
	foreach(PageBase pb in Project.ActiveFolder().Pages)
	{
		// skip everything except workbook pages
		if(pb.GetType() != 2)
		{
			continue;
		}

		// extract name
		wbNames = wbNames + "|" + pb.GetName();

		// extract worksheet names from page
		Page p;
		p = (Page) pb;
		foreach(Layer lay in p.Layers)
		{
			wksNames = wksNames + "|" + pb.GetName() + " - " + lay.GetName();
		}
	}

	// setup N_BOX
	GETN_BOX(tr);
	GETN_STRLIST(wbName,      LABEL_WB_NAME,      "", wbNames);
	GETN_STRLIST(wksName,     LABEL_WKS_NAME,     "", wksNames);
	GETN_LIST(   Method,      IMPORT_METHOD, 0,  IMPORT_METHODS);
	GETN_LIST(   Delimiter,   IMPORT_DELIM,  2,  IMPORT_DELIMS);
	GETN_LIST(   Separator,   IMPORT_SEPAR,  3,  IMPORT_SEPARS);
	GETN_CHECK(  RemoveX,     IMPORT_XAXIS,  false);
	GETN_CHECK(  Sparklines,  IMPORT_SPARKL, false);
	GETN_CHECK(  SeriesLabel, IMPORT_SERIES, false);

	// store results
	vector<string> params;
	if(GetNBox(tr, IMPORT_TITLE, IMPORT_DESC))
	{
		params.Add(tr.Method.strVal);
		params.Add(tr.wbName.strVal);
		params.Add(tr.wksName.strVal);
		params.Add(tr.Delimiter.strVal);
		params.Add(tr.Separator.strVal);
		params.Add(tr.RemoveX.strVal);
		params.Add(tr.Sparklines.strVal);
		params.Add(tr.SeriesLabel.strVal);
	}
	else
	{
		// user input failed or cancelled
		params.Add("-1");
	}

	// secure worksheet name
	int nRet = params[2].Find(" - "); // wksName
	if(nRet > -1)
	{
		params[1] = params[2].Left(nRet); // wbName
		params[2] = params[2].Right(params[2].GetLength() - (nRet + 3)); // wksName
	}

	return params;
}

/**
 * Open a multi field input box to setup the worksheet labeling.
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_readLabels()
{
	// setup N_BOX
	GETN_BOX(tr);
	GETN_STR(Parameter, IMPORT_LABELS_NAME, X_NAME_TIME);
	GETN_STR(Unit,      IMPORT_LABELS_STEP, X_UNIT_TIME);
	GETN_NUM(Stepsize,  IMPORT_LABELS_STEP, 1);

	// store results
	vector<string> params;
	if(GetNBox(tr, IMPORT_LABELS_TITLE, IMPORT_LABELS_DESC))
	{
		params.Add(tr.Parameter.strVal);
		params.Add(tr.Unit.strVal);
		params.Add(tr.Stepsize.strVal);
	}
	else
	{
		// user input failed or cancelled
		params.Add("-1");
	}

	return params;
}

/**
 * Open a multiple input box to setup spectra analysis.
 *
 * @param Worksheet wks the worksheet to read the user labels from
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_analyzeSpectra(Worksheet wks)
{
	// read worksheet user labels
	Grid gg;
	gg.Attach(wks);
	vector<string> labelNames;
	gg.GetUserDefinedLabelNames(labelNames);

	// implode labels for GETN
	labelNames.InsertAt(0, ANALYZE_GENERIC_INDEX);
	string labelList = str_combine(labelNames, "|");

	// setup N_BOX
	GETN_BOX(tr);
	GETN_STR( DataName, ANALYZE_SPECTRA_LABEL_TARGET,    ANALYZE_SPECTRA_TARGET);
	GETN_LIST(Method,   ANALYZE_SPECTRA_LABEL_METHOD,    0, ANALYZE_SPECTRA_METHODS);
	
	GETN_SEPARATOR_LINE;
	GETN_STR(  STR,       ANALYZE_ABSCISSA_HINT,            "") GETN_HINT;
	GETN_CHECK(SkipParam, ANALYZE_SPECTRA_LABEL_SKIP,       false);
	GETN_LIST( XParam,    ANALYZE_SPECTRA_LABEL_ABSCISSA_X, 0, labelList);
	GETN_LIST( YParam,    ANALYZE_SPECTRA_LABEL_ABSCISSA_Y, 0, "none|" + labelList);
	
	GETN_SEPARATOR_LINE;
	GETN_STR(STR, ANALYZE_RANGE_HINT, "") GETN_HINT;
	GETN_NUM(StartX,   ANALYZE_SPECTRA_LABEL_XSTART,    0);
	GETN_NUM(StopX,    ANALYZE_SPECTRA_LABEL_XSTOP,     0);

	// store results
	vector<string> params;
	if(GetNBox(tr, ANALYZE_SPECTRA_TITLE, ANALYZE_SPECTRA_DESC + " (\"" + wks.GetName() + "\")"))
	{
		params.Add(tr.Method.strVal);
		params.Add(tr.DataName.strVal);
		params.Add(tr.StartX.strVal);
		params.Add(tr.StopX.strVal);
		params.Add(tr.SkipParam.strVal);		
		params.Add(tr.XParam.strVal);
		params.Add(tr.YParam.strVal);
	}
	else
	{
		// user input failed or cancelled
		params.Add("-1");
	}

	return params;
}

/**
 * Open a multi field input box to setup a 3D-Map linescan extraction.
 *
 * @param Worksheet wks the source data worksheet
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_map4dLinescan(Worksheet wks)
{
	// resolve worksheet label names
	Grid gg;
	gg.Attach(wks);
	vector<string> labelNamesV;
	gg.GetUserDefinedLabelNames(labelNamesV);
	string labelNames = str_combine(labelNamesV, "|");

	// setup NBOX
	GETN_BOX(tr);
	GETN_LIST(ScanAxis, "Axis", 1, "X|Y");
	GETN_LIST(XParam,   "X",    0, labelNames);
	GETN_LIST(YParam,   "Y",    1, labelNames);
	GETN_NUM(Precision, "Precision", 1);
	
	// store results
	vector<string> params;
	if(GetNBox(tr, MAP_LINESCAN_TITLE, MAP_LINESCAN_DESC))
	{
		params.Add(tr.ScanAxis.strVal);
		params.Add(tr.XParam.strVal);
		params.Add(tr.YParam.strVal);
		params.Add(tr.Precision.strVal);
	}
	else
	{
		// user input failed or cancelled
		params.Add("-1");
	}

	// get data from labels
	vector<string> xStrV, yStrV;
	gg.GetLabelsByType(xStrV, RCLT_UDL + (tr.XParam.nVal));
	gg.GetLabelsByType(yStrV, RCLT_UDL + (tr.YParam.nVal));

	// convert data to numbers
	vector<double> xNumV, yNumV;
	convert_str_vector_to_num_vector(xStrV, xNumV);
	convert_str_vector_to_num_vector(yStrV, yNumV);
	
	// round values to precision
	xNumV = round(xNumV, atof(tr.Precision.strVal));
	yNumV = round(yNumV, atof(tr.Precision.strVal));
	
	// sort values
	xNumV.Sort();
	yNumV.Sort();

	// recreate string vectors
	vector<string> xStrFmt(xNumV.GetSize()), yStrFmt(yNumV.GetSize());
	for(int ii = 0; ii < xNumV.GetSize(); ii++){
		xStrFmt[ii] = "." + tr.Precision.strVal;
	}
	for(ii = 0; ii < yNumV.GetSize(); ii++){
		yStrFmt[ii] = "*." + tr.Precision.strVal;
	}
	convert_double_vector_to_string_vector(xNumV, xStrV, xNumV.GetSize(), xStrFmt);
	convert_double_vector_to_string_vector(yNumV, yStrV, yNumV.GetSize(), yStrFmt);
	
	// remove duplicates
	vector<string> xStrVUnique, yStrVUnique;
	xStrVUnique = MISC_arrayUnique(xStrV);
	yStrVUnique = MISC_arrayUnique(yStrV);

	// prepare coordinates
	vector<string> coordStrV;
	if(tr.ScanAxis.nVal == 0)
	{
		// x-Axis scan
		coordStrV = yStrVUnique;
	}
	else
	{
		coordStrV = xStrVUnique;
	}

	// get coordinate and width
	vector<string> lineParams;
	lineParams = USER_linescan(coordStrV);

	// add coordinate
	if(lineParams[0] == "-1"){
		params[0] = "-1";
	} else {
		params.Add(lineParams[0]);
		//params.Add(lineParams[1]);
	}

	return params;
}

/**
 * Open a multiple input box to setup linescan parameters.
 *
 * @param vector<string> coordStrV the string values of available coordinates
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_linescan(vector<string> coordStrV)
{
	// combine coordinates
	string coordStr;
	coordStr = str_combine(coordStrV, "|");

	// calculate possible widths
	vector<string> widthsV;
	for(int i = 0; i < coordStrV.GetSize(); i++)
	{
		// only symmetric widths
		if((i-1) % 2 == 0)
		{
			widthsV.Add(ftoa(i));
		}
	}
	string widths = str_combine(widthsV, "|");

	// setup N_BOX
	GETN_BOX(tr);
	GETN_LIST(Coord, MAP_LINESCAN_COORD, 0, coordStr);
	//GETN_LIST(Width, MAP_LINESCAN_WIDTH, 0, widths);

	// store results
	vector<string> params;
	if(GetNBox(tr, MAP_LINESCAN_TITLE, MAP_LINESCAN_DESC))
	{
		params.Add(coordStrV[tr.Coord.nVal]);
		//params.Add(widthsV[tr.Width.nVal]);
	}
	else
	{
		params.Add("-1");
	}

	return params;
}

/**
 * Open a multiple input box to setup data manipulation.
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_correctData(WorksheetPage wb)
{
	// get Worksheet(page) names
	string wksNames;
	foreach(Layer lay in wb.Layers)
	{
		wksNames = wksNames + "|" + lay.GetName();
	}
	wksNames.TrimLeft("|");

	// setup N_BOX
	GETN_BOX(tr);
	GETN_STRLIST(wksName,       CORRECT_DATAWKS_LABEL, Project.ActiveLayer().GetName(), wksNames);
	GETN_CHECK(  Clean,         CORRECT_CLEAN,         false);
	GETN_CHECK(  Background,    CORRECT_BACKGROUND,    false);
	GETN_CHECK(  Spikes,        CORRECT_SPIKES,        false);
	GETN_CHECK(  Setup,         CORRECT_SETUP,         false);
	GETN_CHECK(  Filters,       CORRECT_FILTERS,       false);
	GETN_CHECK(  Integration,   CORRECT_INTEGRATION,   false);
	GETN_CHECK(  Transform,     CORRECT_TRANSFORM,     false);
	GETN_CHECK(  Normalise,     CORRECT_NORMALISE,     false);

	// store results
	vector<string> params;
	if(GetNBox(tr, CORRECT_TITLE, CORRECT_DESC))
	{
		params.Add(tr.wksName.strVal);
		params.Add(tr.Clean.strVal);
		params.Add(tr.Background.strVal);
		params.Add(tr.Spikes.strVal);
		params.Add(tr.Setup.strVal);
		params.Add(tr.Filters.strVal);
		params.Add(tr.Integration.strVal);
		params.Add(tr.Transform.strVal);
		params.Add(tr.Normalise.strVal);
	}
	else
	{
		// user input failed or cancelled
		params.Add("-1");
	}

	return params;
}

/**
 * Open a multiple input box to setup data manipulation.
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_correctDataSource(WorksheetPage wb, Worksheet dataWks, int method, string title, int step = 1)
{
	vector<string> params;

	// get Worksheet names
	string wksNames;
	foreach(Layer lay in wb.Layers)
	{
		wksNames = wksNames + "|" + lay.GetName();
	}
	wksNames.TrimLeft("|");

	// read worksheet user labels
	Grid gg;
	gg.Attach(dataWks);
	vector<string> labelNames;
	gg.GetUserDefinedLabelNames(labelNames);

	// implode labels for GETN
	labelNames.InsertAt(0, "Index");
	string labelList = str_combine(labelNames, "|");

	// setup N_BOX
	GETN_BOX(tr);
	switch(method)
	{
		case 1: // clean masked data
		case 7: // energy transformation
		case 8: // normalise data
			GETN_STR(STR, CORRECT_CONFIRM_HINT, "") GETN_HINT;
			break;

		case 2: // subtract background
			// explode methods
			vector<string> methods;
			str_separate(CORRECT_BACKGROUND_METHODS, "|", methods);

			switch(step)
			{
				case 1: // pre-select correction method
					GETN_STR(STR, CORRECT_BACKGROUND_HINT, "") GETN_HINT;
					GETN_RADIO_INDEX(bgMethod, 1, CORRECT_BACKGROUND_METHODS);
					GETN_OPTION_DISPLAY_FORMAT(DISPLAY_EDITOR_LEFT);

					// call second dialogue step with recursion
					if(GetNBox(tr, title, CORRECT_BACKGROUND)){
						return USER_correctDataSource(wb, dataWks, method, title, tr.bgMethod.dVal + 2);
					} else {
						// user input failed or cancelled
						params.Add("-1");
						return params;
					}
					break;

				case 2: // reference mode
					title = methods[step - 2];
					GETN_STR(STR, CORRECT_REFERENCE_HINT, "") GETN_HINT;
					GETN_STRLIST(wksName, ANALYZE_WKS,         "", wksNames);
					GETN_LIST(userParam,  LABEL_USERPARAMETER, -1, labelList);
					break;

				case 3: // median mode
					title = methods[step - 2];
					GETN_STR(STR, CORRECT_BACKGROUND_MEDIAN_HINT, "") GETN_HINT;
					GETN_NUM(bgStart, CORRECT_BACKGROUND_PARAM_START, 0);
					GETN_NUM(bgStop,  CORRECT_BACKGROUND_PARAM_STOP,  0);
					break;

				case 4: // constant mode
					title = methods[step - 2];
					GETN_STR(STR, CORRECT_BACKGROUND_CONSTANT_HINT, "") GETN_HINT;
					GETN_NUM(bgValue, CORRECT_BACKGROUND_PARAM_VALUE, 0);
					break;

			}
			break;

		case 3: // correct spikes
			GETN_STR(STR, CORRECT_SPIKES_HINT, "") GETN_HINT;
			GETN_NUM(spikeTh, CORRECT_SPIKES_PARAM_THRESHOLD, 5);
			GETN_NUM(spikeW,  CORRECT_SPIKES_PARAM_WIDTH,     5);
			break;

		case 4: // correct setup
		case 5: // correct filters
			GETN_STR(STR, CORRECT_REFERENCE_HINT, "") GETN_HINT;
			GETN_STRLIST(wksName,	ANALYZE_WKS,         "", wksNames);
			GETN_LIST(userParam,	LABEL_USERPARAMETER, -1, labelList);
			break;

		case 6: // correct integration time
			GETN_STR(STR, 			CORRECT_USERLABEL_HINT, "") GETN_HINT;
			GETN_LIST(userParam,	LABEL_USERPARAMETER, -1,  labelList);
			break;
	}

	// store results
	if(GetNBox(tr, title, title))
	{
		switch(method)
		{
			case 1: // clean masked data
			case 7: // energy transformation
			case 8: // normalise data
				params.Add(1);
				break;

			case 2: // subtract background
				params.Add(step - 2);
				switch(step)
				{
					case 2: // reference mode
						params.Add(tr.wksName.strVal);
						if(tr.userParam.dVal >= 0)
						{
							params.Add(labelNames[tr.userParam.dVal]);
						}
						else
						{
							params.Add(0);
						}
						break;

					case 3: // median mode
						params.Add(tr.bgStart.dVal);
						params.Add(tr.bgStop.dVal);
						break;

					case 4: // constant mode
						params.Add(tr.bgValue.dVal);
						break;
				}
				break;

			case 3: // remove spikes
				params.Add(tr.spikeW.dVal);
				params.Add(tr.spikeTh.dVal);
				break;

			case 4: // correct setup
			case 5: // correct filters
				params.Add(tr.wksName.strVal);
				if(tr.userParam.dVal >= 0)
				{
					params.Add(labelNames[tr.userParam.dVal]);
				}
				else
				{
					params.Add(-1);
				}
				break;

			case 6: // correct integration time
				if(tr.userParam.dVal >= 0)
				{
					params.Add(labelNames[tr.userParam.dVal]);
				}
				else
				{
					params.Add(-1);
				}
				break;
		}
	}
	else
	{
		// user input failed or cancelled
		params.Add("-1");
	}

	return params;
}

/**
 * Open a multiple input box to setup map data handling.
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_map()
{
	// setup N_BOX
	GETN_BOX(tr);
	GETN_LIST(Method, MAP_METHOD, 0, MAP_METHODS);

	// store results
	vector<string> params;
	if(GetNBox(tr, MAP_TITLE, MAP_DESC))
	{
		params.Add(tr.Method.strVal);
	}
	else
	{
		// user input failed or cancelled
		params.Add("-1");
	}

	return params;
}

/**
 * Open a multi field input box to setup a map dimension.
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_xyzMatrix()
{
	// setup N_BOX
	GETN_BOX(tr);
	GETN_INTERACTIVE(ZRange, XYZ_MATRIX_ZCOL, "")
	GETN_OPTION_INTERACTIVE_CONTROL(ICOPT_ALLOW_MULTIPLE_DATA_1);
	GETN_CHECK(UseCols,   XYZ_MATRIX_USECOLS, true)
	GETN_XYRANGE(XyRange, XYZ_MATRIX_XYCOL,   1, "") ;
	GETN_OPTION_INTERACTIVE_CONTROL(ICOPT_RESTRICT_TO_ONE_DATA);

	GETN_NUM(NumCols,   XYZ_MATRIX_COLC,  10) GETN_BEGIN_GROUP(XYZ_MATRIX_TITLE);;
	GETN_STR(StepUnit,  XYZ_MATRIX_STEPU, XYZ_MATRIX_STEPU_PRE);
	GETN_NUM(XStep,     XYZ_MATRIX_XSTEP, 1);
	GETN_NUM(YStep,     XYZ_MATRIX_YSTEP, 1)  GETN_END_GROUP;

	// store results
	vector<string> params;
	if(GetNBox(tr, XYZ_MATRIX_TITLE, XYZ_MATRIX_DESC))
	{
		params.Add(tr.ZRange.strVal);
		params.Add(tr.UseCols.strVal);
		params.Add(tr.XyRange.Range1.X.strVal);
		params.Add(tr.XyRange.Range1.Y.strVal);
		params.Add(tr.NumCols.strVal);
		params.Add(tr.StepUnit.strVal);
		params.Add(tr.XStep.strVal);
		params.Add(tr.YStep.strVal);
	}
	else
	{
		// user input failed or cancelled
		params.Add("-1");
	}

	return params;
}

/**
 * Open a multiple input box to setup peak collection.
 *
 * @param WorksheetPage  wb         the workbook to evaluate
 * @param vector<string> sheetNames an array of sheet names with valid data for collection
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_peaks(WorksheetPage wb, vector<string> sheetNames)
{
	// get first valid worksheet
	Worksheet tmpWks = wb.Layers(sheetNames[0]);

	// get possible column names
	string wksCols;
	foreach(Column col in tmpWks.Columns)
	{
		wksCols = wksCols + "|" + col.GetName() + " - " + col.GetLongName();
	}

	// setup N_BOX
	GETN_BOX(tr);
	GETN_STR(    Identifier, LABEL_TARGET_NAME, "");
	GETN_STRLIST(colName,    LABEL_COL_NAME,    "", wksCols);

	// store results
	vector<string> params;
	if(GetNBox(tr, PEAKS_TITLE, PEAKS_DESC))
	{
		params.Add(tr.Identifier.strVal);
		params.Add(tr.colName.strVal);
	}
	else
	{
		// user input failed or cancelled
		params.Add("-1");
	}

	// strip column long name
	if(params.GetSize() == 2)
	{
		int nRet = params[1].Find(" - "); // wcolName
		if(nRet > -1)
		{
			params[1] = params[1].Left(nRet);
		}
	}

	return params;
}

/**
 * Open a multiple input box to setup data interpolation.
 *
 * @param Worksheet wks the worksheet to evaluate
 * @param vector<string> sheetNames an array of sheet names with valid data for collection
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_interpolate(Worksheet wks)
{
	// get possible column names
	string wksCols;
	double i = 0;
	foreach(Column col in wks.Columns)
	{
		wksCols = wksCols + "|" + ftoa(i) + ": " + col.GetName() + " - " + col.GetLongName();
		i = i + 1;
	}

	// setup N_BOX
	GETN_BOX(tr);
	GETN_STRLIST(colName, INTERPOLATE_COL, "", wksCols);

	// store results
	vector<string> params;
	if(GetNBox(tr, INTERPOLATE_TITLE, INTERPOLATE_DESC))
	{
		params.Add(tr.colName.strVal);
	}
	else
	{
		// user input failed or cancelled
		params.Add("-1");
	}

	// extract column index
	if(params[0] != "-1")
	{
		int nRet = params[0].Find(":"); // wcolName
		if(nRet > -1)
		{
			params[0] = params[0].Left(nRet);
		}
	}

	return params;
}

#endif