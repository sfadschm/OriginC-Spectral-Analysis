/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _IMPORTER_ // include once
#define _IMPORTER_

/**
 * This file provides methods for importing data from various file formats.
 */
#include <Origin.h>
#include "Lang.h"
#include "File.h"
#include "Misc.h"
#include "Worksheet.h"

/**
 * Import and format a (consecutive) series of ASCII type spectra files.
 *
 * @param vector<string> params   the user parameters
 * @param vector<string> strFiles the names of the input data files
 *
 * @return Worksheet wks the worksheet containing the imported data
 */
Worksheet IMPORT_spectra(vector<string> params, vector<string> strFiles){
	// user information
	printf(IMPORT_READING);

	// map required user parameters
	string wbName  = params[1];
	string wksName = params[2];
	int delim      = atoi(params[3]);
	int numSep     = atoi(params[4]) - 2; // see definition of NF_IS_AUTO
	int removeX    = atoi(params[5]);
	int addSparks  = atoi(params[6]);
	int addSeries  = atoi(params[7]);

	// create target workbook/worksheet for import
	WorksheetPage wb = ORIGIN_createWb(wbName); 
	Worksheet wks    = ORIGIN_createWks(wb, wksName); 
	int existingCols = wks.GetNumCols();

	// loop through source files
	for(int i = 0; i < strFiles.GetSize(); i++)
	{
		ASCIMP	ai;
		if(0 == AscImpReadFileStruct(strFiles[i], &ai))
		{
			// setup headers
			ai.iMode       = ASCIMP_MODE_APPEND_COLS; // open new columns per file
			ai.iRenameWks  = 0;                       // do not automatically rename
			ai.iNonnumeric = 1;                       // read nonnumeric values as missing 
			ai.iDelimited  = 1;                       // files must be delimited
			ai.iDelimiter  = delim;
			ai.nNumSep     = numSep;

			// import file
			wks.ImportASCII(strFiles[i], ai);

			// set headers and add filename to comments
			for(int j = existingCols; j < wks.GetNumCols(); j++)
			{
				if(wks.Columns(j).GetLongName() == "X" || j == existingCols)
				{
					wks.Columns(j).SetType(OKDATAOBJ_DESIGNATION_X);
				}
				else
				{
					wks.Columns(j).SetType(OKDATAOBJ_DESIGNATION_Y);
					wks.Columns(j).SetComments(MISC_stripName(strFiles[i]));
				}
				if(wks.Columns(j).GetLongName() == "")
				{
					if(j == existingCols)
					{
						wks.Columns(j).SetLongName(USER_IMPORT_XNAME_PRE);
						wks.Columns(j).SetUnits(USER_IMPORT_XUNIT_PRE);
					}
					else
					{
						wks.Columns(j).SetLongName(USER_IMPORT_YNAME_PRE);
						wks.Columns(j).SetUnits(USER_IMPORT_YUNIT_PRE);
					}
				}
			}

			// get new column count
			existingCols = wks.GetNumCols();
		}
	}

	// read labels from user
	if(addSeries == 1)
	{
		printf(IMPORT_LABELING);
		vector<string> labels;
		labels = USER_readLabels();

		// add column labels (for series)
		if(atoi(labels[0]) != -1)
		{
			// map user input
			string paramName = (labels[0] + " (" + labels[1] + ")");
			float paramStep = atof(labels[2]);

			// add user parameter row
			WOKRSHEET_addUserParameter(wks, paramName);

			// write labels to columns
			for(int jj = existingCols - 1; jj < wks.GetNumCols(); jj++)
			{
				if(jj%2 == 1)
				{
					// Y-column
					wks.Columns(jj).SetExtendedLabel(ftoa((jj-1)/2*paramStep), RCLT_UDL);
				}
			}
		}
	}

	// remove double x-axes
	if(removeX == 1)
	{
		for(int jjj = wks.GetNumCols() - 2; jjj > 0; jjj-=2)
		{
			wks.DeleteCol(jjj);
		}
	}

	// generate sparklines
	if(addSparks == 1)
	{
		WORKSHEET_addSparklines(wks);
	}

	return wks;
}

/**
 * Import and format spectral data from a one or more 3D Map ASCII files (e.g., Streak).
 *
 * @param vector<string> params   the user parameters
 * @param vector<string> strFiles the names of the input data files
 *
 * @return WorksheetPage wb the workbook containing the imported data
 */
WorksheetPage IMPORT_3dMaps(vector<string> params, vector<string> strFiles){
	//user information
	printf(IMPORT_READING);

	// map required user parameters
	string wbName  = params[1];
	string wksName = params[2];
	int delim      = atoi(params[3]);
	int numSep     = atoi(params[4]) - 2; // see definition of NF_IS_AUTO

	// create target workbook for import
	WorksheetPage wb = ORIGIN_createWb(wbName); 

	// loop through source files
	for(int i = 0; i < strFiles.GetSize(); i++)
	{
		// get source file extension
		string strExt = strFiles[i].Mid(strFiles[i].ReverseFind('.')+1);

		// create target worksheet for import
		Worksheet wks = ORIGIN_createWks(wb, MISC_stripName(strFiles[i]), true); 

		ASCIMP ai;
		if(0 == AscImpReadFileStruct(strFiles[i], &ai))
		{
			// setup headers
			ai.iMode       = ASCIMP_MODE_NEW_SHEETS; // open new worksheet per file
			ai.iRenameWks  = 0;                      // do not automatically rename
			ai.iNonnumeric = 1;                      // read nonnumeric values as missing 
			ai.iDelimited  = 1;                      // files must be delimited
			ai.iDelimiter  = delim;
			ai.nNumSep     = numSep;

			// import file
			wks.ImportASCII(strFiles[i], ai);

			// transpose matrix if necessary (Streak and LabSpec)
			if(strExt.CompareNoCase("DAC") == 0 || strExt.CompareNoCase("TXT") == 0)
			{
				int rowStart,rowEnd;
				wks.GetBounds(rowStart, 0, rowEnd, 0);
				wks.Transpose();
				wks.SetSize(wks.GetNumCols(), rowEnd + 1);
			}

			// define parameters and units
			string xParam, xUnit, yParam, yUnit;

			// Streak images
			if(strExt.CompareNoCase("DAC") == 0)
			{
				xParam = USER_LABELS_NAME_PRE;

				string cellText;
				wks.GetCell(0, 0, cellText);
				xUnit = cellText.Mid(0, cellText.ReverseFind('|'));

				yParam = USER_IMPORT_XNAME_PRE;
				yUnit  = USER_IMPORT_XUNIT_PRE;
			}

			// LabSpec TimeTrace
			if(strExt.CompareNoCase("TXT") == 0)
			{
				xParam = USER_LABELS_NAME_PRE;
				xUnit  = USER_LABELS_UNIT_PRE;
				yParam = USER_IMPORT_XNAME_PRE;
				yUnit  = USER_IMPORT_XUNIT_PRE;
			}

			// Raman image
			if(strExt.CompareNoCase("TSV") == 0){
				xParam = "X";
				xUnit  = USER_XYZ_MATRIX_STEPU_PRE;
				yParam = "Y";
				yUnit  = USER_XYZ_MATRIX_STEPU_PRE;
			}

			// add user parameter row
			WOKRSHEET_addUserParameter(wks, xParam + " (" + xUnit + ")");

			// move x-axis to parameters
			string colLabel;
			for(int jj = 1; jj < wks.GetNumCols(); jj++)
			{
				wks.GetCell(0, jj, colLabel);
				wks.Columns(jj).SetExtendedLabel(colLabel, RCLT_UDL);
			}

			// remove x-axis from data matrix
			wks.DeleteRow(0);

			// finalize worksheet
			wks.Columns(0).SetType(OKDATAOBJ_DESIGNATION_X); // set wavelength as X
			wks.Columns(0).SetLongName(yParam);
			wks.Columns(0).SetUnits(yUnit);
			for(int ii = 1; ii < wks.GetNumCols(); ii++)
			{
				wks.Columns(ii).SetLongName(USER_IMPORT_XNAME_PRE);
				wks.Columns(ii).SetUnits(USER_IMPORT_XUNIT_PRE);
			}
		}
	}

	return wb;
}

/**
 * Import and format spectral data from a one or more 4D Map ASCII files (e.g., NT-MDT).
 *
 * @param vector<string> params the user parameters
 * @param vector<string> strFiles the names of the input data files
 *
 * @return WorksheetPage wb the workbook containing the imported data
 */
WorksheetPage IMPORT_4dMaps(vector<string> params, vector<string> strFiles){
	// user information
	printf(IMPORT_READING);

	// map required user parameters
	string wbName  = params[1];
	string wksName = params[2];
	int delim      = atoi(params[3]);
	int numSep     = atoi(params[4]) - 2; // see definition of NF_IS_AUTO
	int addSparks  = atoi(params[6]);

	// create target workbook for import
	WorksheetPage wb = ORIGIN_createWb(wbName); 

	// loop through source files
	for(int i = 0; i < strFiles.GetSize(); i++)
	{
		// progress info
		printf(USER_IMPORT_PROGRESS_INFO, i + 1, strFiles.GetSize());

		// create target worksheet for import
		Worksheet wks = ORIGIN_createWks(wb, MISC_stripName(strFiles[i]), true); 

		// set worksheet size (avoid creating new columns)
		wks.SetSize(-1, 1024);

		// open file
		stdioFile srcFile; // create file object
		FILE_openRead(srcFile, strFiles[i]); // open source file

		// read parameters
		string xCalibStr, yCalibStr;
		srcFile.ReadString(xCalibStr);
		srcFile.ReadString(yCalibStr);

		// strip lines
		xCalibStr.Replace("X = [", "");
		xCalibStr.Replace(" ];", "");
		yCalibStr.Replace("Y = [", "");
		yCalibStr.Replace(" ];", "");

		// split lines into vector
		vector<string> xCalibStrV, yCalibStrV;
		str_separate(xCalibStr, " ", xCalibStrV);
		str_separate(yCalibStr, " ", yCalibStrV);

		// convert to double (for manipulation)
		vector<double> xCalib, yCalib;
		convert_str_vector_to_num_vector(xCalibStrV, xCalib);
		convert_str_vector_to_num_vector(yCalibStrV, yCalib);

		// remove axis bias
		xCalib = xCalib - xCalib[0];
		yCalib = yCalib - yCalib[0];

		// add user parameter rows
		WOKRSHEET_addUserParameter(wks, "X (" + USER_XYZ_MATRIX_STEPU_PRE + ")", 0);
		WOKRSHEET_addUserParameter(wks, "Y (" + USER_XYZ_MATRIX_STEPU_PRE + ")", 1);

		// write calibration to parameters
		for(int jj = 2; jj < xCalib.GetSize() * yCalib.GetSize() + 2; jj++)
		{
			wks.Columns(jj).SetExtendedLabel(ftoa(xCalib[(jj - 2) % xCalib.GetSize()]), RCLT_UDL);
			wks.Columns(jj).SetExtendedLabel(ftoa(yCalib[((jj - 2) - (jj - 2) % xCalib.GetSize()) /  xCalib.GetSize()]), RCLT_UDL + 1);
		}

		// read data
		int lineInt = 3;
		string strLine, dataStr;
		while(srcFile.ReadString(strLine))
		{
			// progress info
			if(mod(lineInt, 256) == 0)
			{
				printf("\n%d%%",(lineInt/256*25));
			}

			// strip line
			int dataStart, dataEnd;
			dataStart = strLine.Find("[") + 1;
			dataEnd = strLine.GetLength()-3;
			dataStr = strLine.Mid(dataStart, dataEnd-dataStart);
			dataStr.Replace(";","");

			// import data
			switch(lineInt)
			{
				case 1: // x-cordinates
				case 2: // y-cordinates
				case 1030: // EOF
					break;

				case 5: // map-info
					// transpose calibration data
					wks.Transpose();
					break;

				default:
					// split line into vector
					vector<string> dataStrVector;
					str_separate(dataStr," ", dataStrVector);

					// copy vector into column
					if(lineInt < 5)
					{
						// calibration data
						wks.Columns(lineInt-3).PutStringArray(dataStrVector, 0, true);
					}
					else
					{
						// spectra
						wks.Columns(lineInt-6).PutStringArray(dataStrVector, 2, true);
					}
					break;
			}
			lineInt++; // next line
		} 

		// close source file
		FILE_close(srcFile);

		// transpose matrix
		int rowStart,rowEnd;
		wks.GetBounds(rowStart, 0, rowEnd, 0);
		wks.Transpose();
		wks.SetSize(1024, rowEnd + 1);

		// set wavelength as X
		wks.Columns(0).SetType(OKDATAOBJ_DESIGNATION_X);
		wks.Columns(0).SetLongName(USER_IMPORT_XNAME_PRE);
		wks.Columns(0).SetUnits(USER_IMPORT_XUNIT_PRE);

		// set raman shift as X
		wks.Columns(1).SetType(OKDATAOBJ_DESIGNATION_X);
		wks.Columns(1).SetLongName(USER_IMPORT_X2NAME_PRE);
		wks.Columns(1).SetUnits(USER_IMPORT_X2UNIT_PRE);

		// set intensities
		for(int ii = 2; ii < wks.GetNumCols(); ii++ )
		{
			wks.Columns(ii).SetLongName(USER_IMPORT_YNAME_PRE);
			wks.Columns(ii).SetUnits(USER_IMPORT_YUNIT_PRE);
		}

		// generate sparklines
		if(addSparks == 1){
			WORKSHEET_addSparklines(wks);
		}
	}

	return wb;
}

/**
 * Import and format spectra data from a one or more 3D Map ASCII files (e.g., Streak).
 *
 * @param vector<string> params the user parameters
 * @param vector<string> strFiles the names of the input data files
 *
 * @return WorksheetPage wb the workbook containing the imported data
 */
WorksheetPage IMPORT_LabViewMaps(vector<string> params, vector<string> strFiles){
	// user information
	printf(IMPORT_READING);

	// map required user parameters
	string wbName  = params[1];
	string wksName = params[2];
	int delim      = atoi(params[3]);
	int numSep     = atoi(params[4]) - 2; // see definition of NF_IS_AUTO

	// create target workbook for import
	WorksheetPage wb = ORIGIN_createWb(wbName); 

	// loop through source files
	for(int i = 0; i < strFiles.GetSize(); i++)
	{
		// create target worksheet for import
		Worksheet wks = ORIGIN_createWks(wb, MISC_stripName(strFiles[i]), true); 

		ASCIMP ai;
		if(0 == AscImpReadFileStruct(strFiles[i], &ai))
		{
			// setup headers
			ai.iMode       = ASCIMP_MODE_NEW_SHEETS; // open new worksheet per file
			ai.iRenameWks  = 0;                      // do not automatically rename
			ai.iNonnumeric = 1;                      // read nonnumeric values as missing 
			ai.iDelimited  = 1;                      // files must be delimited
			ai.iDelimiter  = delim;
			ai.nNumSep     = numSep;

			// import file
			wks.ImportASCII(strFiles[i], ai);

			// add user parameter rows
			string paramUnit;
			WOKRSHEET_addUserParameter(wks, "X (" + USER_XYZ_MATRIX_STEPU_PRE + ")", 0);
			WOKRSHEET_addUserParameter(wks, "Y (" + USER_XYZ_MATRIX_STEPU_PRE + ")", 1);

			// write coordinates to parameters
			string xLabel, yLabel;
			for(int jj = 1; jj < wks.GetNumCols(); jj++)
			{
				wks.GetCell(0, jj, xLabel);
				wks.GetCell(1, jj, yLabel);
				wks.Columns(jj).SetExtendedLabel(xLabel, RCLT_UDL);
				wks.Columns(jj).SetExtendedLabel(yLabel, RCLT_UDL+1);
			}

			// remove time axis
			wks.DeleteRow(0); // remove x axis
			wks.DeleteRow(0); // remove y axis

			// finalize worksheet
			wks.Columns(0).SetType(OKDATAOBJ_DESIGNATION_X); // set wavelength as X
			wks.Columns(0).SetLongName(USER_IMPORT_XNAME_PRE);
			wks.Columns(0).SetUnits(USER_IMPORT_XUNIT_PRE);
			for(int ii = 1; ii < wks.GetNumCols(); ii++ )
			{
				wks.Columns(ii).SetLongName(USER_IMPORT_YNAME_PRE);
				wks.Columns(ii).SetUnits(USER_IMPORT_YUNIT_PRE);
			}
		}
	}

	return wb;
}

/**
 * Import and format particle tracking data from one or more ImageJ XML files.
 *
 * @param vector<string> params the user parameters
 * @param vector<string> strFiles the names of the input data files
 *
 * @return WorksheetPage wb the workbook containing the imported data
 */
void IMPORT_Tracks(vector<string> params, vector<string> strFiles){
	// user information
	printf(IMPORT_READING);

	// map required user parameters
	string wbName  = params[1];
	string wksName = params[2];

	// create target workbook/worksheet for import
	WorksheetPage wb  = ORIGIN_createWb(wbName); 
	Worksheet     wks = ORIGIN_createWks(wb, wksName); 

	// loop through source files
	for(int i = 0; i < strFiles.GetSize(); i++)
	{
		// read XML	
		Tree tr;
		if( tr.Load(strFiles[i]))
		{
			// progress info
			printf(USER_IMPORT_PROGRESS_INFO, i + 1, strFiles.GetSize());

			// create target worksheet for import
			Worksheet wks = ORIGIN_createWks(wb, MISC_stripName(strFiles[i]), true); 

			// set wks size
			wks.SetSize(-1, 4*tr.GetNodeCount());

			// get params
			string xyUnits, tUnits; 
			double tScale, numParts;
			tr.GetAttribute("spaceUnits",    xyUnits);
			tr.GetAttribute("timeUnits",     tUnits);
			tr.GetAttribute("frameInterval", tScale);
			tr.GetAttribute("nTracks",       numParts);

			// loop particles
			int iCol = 0;
			foreach(TreeNode tParticle in tr.Children)
			{
				// loop spots
				vector<double> partT, partX, partY, partD;
				foreach(TreeNode tSpot in tParticle.Children)
				{
					double tempVal;
					tSpot.GetAttribute("t", tempVal);
					partT.Add(tempVal);
					tSpot.GetAttribute("x", tempVal);
					partX.Add(tempVal);
					tSpot.GetAttribute("y", tempVal);
					partY.Add(tempVal);
				}

				//recalculate data
				partT = partT * tScale;
				partX = -(partX - partX[0]);
				partY = -(partY - partY[0]);
				partD = sqrt(partX^2 + partY^2);

				// inset values
				for(int iAttr = 0; iAttr < 4; iAttr++)
				{
					// next column
					Column curCol = wks.Columns(iCol++);

					// set column properties
					vector<string> curData;
					int colType    = OKDATAOBJ_DESIGNATION_Y;
					string colName = "", colUnit = xyUnits;
					switch(iAttr)
					{
						case 0:
							colType = OKDATAOBJ_DESIGNATION_X;
							colName = USER_LABELS_NAME_PRE;
							colUnit = tUnits;
							convert_double_vector_to_string_vector(partT, curData, partT.GetSize());
							break;

						case 1:
							colName = "X";
							convert_double_vector_to_string_vector(partX, curData, partX.GetSize());
							break;

						case 2:
							colName = "Y";
							convert_double_vector_to_string_vector(partY, curData, partY.GetSize());
							break;

						case 3:
							colName = "d";
							convert_double_vector_to_string_vector(partD, curData, partD.GetSize());
							break;
					}

					curCol.SetLongName(colName);
					curCol.SetUnits(colUnit);
					curCol.SetType(colType);
					curCol.PutStringArray(curData);
					curCol.SetFormat(OKCOLTYPE_NUMERIC);
				}
			}
		}
	}
}

#endif