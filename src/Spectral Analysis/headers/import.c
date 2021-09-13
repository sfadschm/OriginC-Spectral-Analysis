/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#include <Origin.h>

#include "file.h"
#include "lang.h"
#include "misc.h"
#include "origin.h"
#include "user.h"
#include "worksheet.h"

Worksheet IMPORT_spectra(vector<string> params, vector<string> strFiles) {
    // user information
    printf(IMPORT_READING);

    // map required user parameters
    string wbName  = params[1];
    string wksName = params[2];
    int delim      = atoi(params[3]);
    int numSep     = atoi(params[4]) - 2;  // see definition of NF_IS_AUTO
    int removeX    = atoi(params[5]);
    int addSparks  = atoi(params[6]);
    int addSeries  = atoi(params[7]);

    // create target workbook/worksheet for import
    WorksheetPage wb = ORIGIN_createWb(wbName);
    Worksheet wks    = ORIGIN_createWks(wb, wksName);
    int existingCols = wks.GetNumCols();

    // loop through source files
    for (int i = 0; i < strFiles.GetSize(); i++) {
        ASCIMP ai;
        if (0 == AscImpReadFileStruct(strFiles[i], &ai)) {
            // setup headers
            ai.iMode = ASCIMP_MODE_APPEND_COLS;  // open new columns per file
            ai.iRenameWks  = 0;                  // do not automatically rename
            ai.iNonnumeric = 1;  // read nonnumeric values as missing
            ai.iDelimited  = 1;  // files must be delimited
            ai.iDelimiter  = delim;
            ai.nNumSep     = numSep;

            // ignore headers (will be handled later)
            ai.iAutoSubHeaderLines = 0;
            ai.iSubHeaderLines     = 0;

            // import file
            wks.ImportASCII(strFiles[i], ai);

            // set headers
            for (int j = existingCols; j < wks.GetNumCols(); j++) {
                if (wks.Columns(j).GetLongName() == "X" || j == existingCols) {
                    wks.Columns(j).SetType(OKDATAOBJ_DESIGNATION_X);
                } else {
                    wks.Columns(j).SetType(OKDATAOBJ_DESIGNATION_Y);
                    // wks.Columns(j).SetComments(MISC_stripName(strFiles[i]));
                }
                if (wks.Columns(j).GetLongName() == "") {
                    if (j == existingCols) {
                        wks.Columns(j).SetLongName(X_NAME_WAVELENGTH);
                        wks.Columns(j).SetUnits(X_UNIT_WAVELENGTH);
                    } else {
                        wks.Columns(j).SetLongName(Y_NAME_INTENSITY);
                        wks.Columns(j).SetUnits(Y_UNIT_INTENSITY);
                    }
                }
            }

            // get new column count
            existingCols = wks.GetNumCols();
        }
    }

    // handle header lines if any
    IMPORT_processHeaders(wks);

    // read labels from user
    if (addSeries == 1) {
        printf(IMPORT_LABELING);
        vector<string> labels;
        labels = USER_readLabels();

        // add column labels (for series)
        if (atoi(labels[0]) != -1) {
            // map user input
            string paramName = (labels[0] + " (" + labels[1] + ")");
            float paramStep  = atof(labels[2]);

            // add user parameter row
            WOKRSHEET_addUserParameter(wks, paramName);

            // write labels to columns
            for (int jj = existingCols - 1; jj < wks.GetNumCols(); jj++) {
                if (jj % 2 == 1) {
                    // Y-column
                    wks.Columns(jj).SetExtendedLabel(
                        ftoa((jj - 1) / 2 * paramStep), RCLT_UDL);
                }
            }
        }
    }

    // remove double x-axes
    if (removeX == 1) {
        for (int jjj = wks.GetNumCols() - 2; jjj > 0; jjj -= 2) {
            wks.DeleteCol(jjj);
        }
    }

    // generate sparklines
    if (addSparks == 1) {
        WORKSHEET_addSparklines(wks);
    }

    return wks;
}

WorksheetPage IMPORT_3dMaps(vector<string> params, vector<string> strFiles) {
    // user information
    printf(IMPORT_READING);

    // map required user parameters
    string wbName  = params[1];
    string wksName = params[2];
    int delim      = atoi(params[3]);
    int numSep     = atoi(params[4]) - 2;  // see definition of NF_IS_AUTO

    // create target workbook for import
    WorksheetPage wb = ORIGIN_createWb(wbName);

    // loop through source files
    for (int i = 0; i < strFiles.GetSize(); i++) {
        // get source file extension
        string strExt = strFiles[i].Mid(strFiles[i].ReverseFind('.') + 1);

        // create target worksheet for import
        Worksheet wks = ORIGIN_createWks(wb, MISC_stripName(strFiles[i]), true);

        ASCIMP ai;
        if (0 == AscImpReadFileStruct(strFiles[i], &ai)) {
            // setup headers
            ai.iMode = ASCIMP_MODE_NEW_SHEETS;  // open new worksheet per file
            ai.iRenameWks  = 0;                 // do not automatically rename
            ai.iNonnumeric = 1;  // read nonnumeric values as missing
            ai.iDelimited  = 1;  // files must be delimited
            ai.iDelimiter  = delim;
            ai.nNumSep     = numSep;

            // ignore headers (will be handled later)
            ai.iAutoSubHeaderLines = 0;
            ai.iSubHeaderLines     = 0;

            // import file
            wks.ImportASCII(strFiles[i], ai);

            // transpose matrix if necessary (Streak and LabSpec)
            if (strExt.CompareNoCase("DAC") == 0 ||
                strExt.CompareNoCase("TXT") == 0) {
                int rowStart, rowEnd;
                wks.GetBounds(rowStart, 0, rowEnd, 0);
                wks.Transpose();
                wks.SetSize(wks.GetNumCols(), rowEnd + 1);
            }

            // define parameters and units
            string xParam, xUnit, yParam, yUnit;

            // Streak images
            if (strExt.CompareNoCase("DAC") == 0) {
                xParam = X_NAME_TIME;

                string cellText;
                wks.GetCell(0, 0, cellText);
                xUnit = cellText.Mid(0, cellText.ReverseFind('|'));

                yParam = X_NAME_WAVELENGTH;
                yUnit  = X_UNIT_WAVELENGTH;
            }

            // LabSpec TimeTrace
            if (strExt.CompareNoCase("TXT") == 0) {
                xParam = X_NAME_TIME;
                xUnit  = X_UNIT_TIME;
                yParam = X_NAME_WAVELENGTH;
                yUnit  = X_UNIT_WAVELENGTH;
            }

            // Raman image or CCD image
            if (strExt.CompareNoCase("TSV") == 0) {
                xParam = "X";
                yParam = "Y";

                // CCD image
                if (strFiles[i].Find("_img") > -1) {
                    xUnit = XYZ_MATRIX_STEPU_PX;
                    yUnit = XYZ_MATRIX_STEPU_PX;
                } else {
                    xUnit = XYZ_MATRIX_STEPU_PRE;
                    yUnit = XYZ_MATRIX_STEPU_PRE;
                }
            }

            // set basic column labels
            wks.Columns(0).SetType(OKDATAOBJ_DESIGNATION_X);
            wks.Columns(0).SetLongName(yParam);
            wks.Columns(0).SetUnits(yUnit);
            for (int ii = 1; ii < wks.GetNumCols(); ii++) {
                wks.Columns(ii).SetLongName(Y_NAME_INTENSITY);
                wks.Columns(ii).SetUnits(Y_UNIT_INTENSITY);
            }

            // handle header lines if any
            int paramCount = IMPORT_processHeaders(wks, 0, true);

            // add user parameter row
            WOKRSHEET_addUserParameter(wks, xParam + " (" + xUnit + ")",
                                       paramCount);

            // move x-axis to parameters
            string colLabel;
            for (int jj = 1; jj < wks.GetNumCols(); jj++) {
                wks.GetCell(0, jj, colLabel);
                wks.Columns(jj).SetExtendedLabel(colLabel,
                                                 RCLT_UDL + paramCount);
            }

            // remove x-axis from data matrix
            wks.DeleteRow(0);
        }
    }

    return wb;
}

WorksheetPage IMPORT_4dMaps(vector<string> params, vector<string> strFiles) {
    // user information
    printf(IMPORT_READING);

    // map required user parameters
    string wbName  = params[1];
    string wksName = params[2];
    int delim      = atoi(params[3]);
    int numSep     = atoi(params[4]) - 2;  // see definition of NF_IS_AUTO
    int addSparks  = atoi(params[6]);

    // create target workbook for import
    WorksheetPage wb = ORIGIN_createWb(wbName);

    // loop through source files
    for (int i = 0; i < strFiles.GetSize(); i++) {
        // progress info
        printf(IMPORT_PROGRESS, i + 1, strFiles.GetSize());

        // create target worksheet for import
        Worksheet wks = ORIGIN_createWks(wb, MISC_stripName(strFiles[i]), true);

        // set worksheet size (avoid creating new columns)
        wks.SetSize(-1, 1024);

        // open file
        stdioFile srcFile;                    // create file object
        FILE_openRead(srcFile, strFiles[i]);  // open source file

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

        // resize worksheet to fit data
        wks.SetSize(-1, (xCalib.GetSize() * yCalib.GetSize()) + 2);

        // add user parameter rows
        WOKRSHEET_addUserParameter(wks, "X (" + XYZ_MATRIX_STEPU_PRE + ")", 0);
        WOKRSHEET_addUserParameter(wks, "Y (" + XYZ_MATRIX_STEPU_PRE + ")", 1);

        // write calibration to parameters
        for (int jj = 2; jj < xCalib.GetSize() * yCalib.GetSize() + 2; jj++) {
            wks.Columns(jj).SetExtendedLabel(
                ftoa(xCalib[(jj - 2) % xCalib.GetSize()]), RCLT_UDL);
            wks.Columns(jj).SetExtendedLabel(
                ftoa(yCalib[((jj - 2) - (jj - 2) % xCalib.GetSize()) /
                            xCalib.GetSize()]),
                RCLT_UDL + 1);
        }

        // read data
        int lineInt = 3;
        string strLine, dataStr;
        while (srcFile.ReadString(strLine)) {
            // progress info
            if (mod(lineInt, 256) == 0) {
                printf("\n%d%%", (lineInt / 256 * 25));
            }

            // strip line
            int dataStart, dataEnd;
            dataStart = strLine.Find("[") + 1;
            dataEnd   = strLine.GetLength() - 3;
            dataStr   = strLine.Mid(dataStart, dataEnd - dataStart);
            dataStr.Replace(";", "");

            // import data
            switch (lineInt) {
                case 1:     // x-cordinates
                case 2:     // y-cordinates
                case 1030:  // EOF
                    break;

                case 5:  // map-info
                    // transpose calibration data
                    wks.Transpose();
                    break;

                default:
                    // split line into vector
                    vector<string> dataStrVector;
                    str_separate(dataStr, " ", dataStrVector);

                    // copy vector into column
                    if (lineInt < 5) {
                        // calibration data
                        // clang-format off
                        wks.Columns(lineInt - 3).PutStringArray(dataStrVector, 0, true);
                        // clang-format on
                    } else {
                        // spectra
                        // clang-format off
                        wks.Columns(lineInt - 6).PutStringArray(dataStrVector, 2, true);
                        // clang-format on
                    }
                    break;
            }
            lineInt++;  // next line
        }

        // close source file
        FILE_close(srcFile);

        // transpose matrix
        int rowStart, rowEnd;
        wks.GetBounds(rowStart, 0, rowEnd, 0);
        wks.Transpose();
        wks.SetSize(1024, rowEnd + 1);

        // set wavelength as X
        wks.Columns(0).SetType(OKDATAOBJ_DESIGNATION_X);
        wks.Columns(0).SetLongName(X_NAME_WAVELENGTH);
        wks.Columns(0).SetUnits(X_UNIT_WAVELENGTH);

        // set raman shift as X
        wks.Columns(1).SetType(OKDATAOBJ_DESIGNATION_X);
        wks.Columns(1).SetLongName(X_NAME_RAMAN);
        wks.Columns(1).SetUnits(X_UNIT_RAMAN);

        // set intensities
        for (int ii = 2; ii < wks.GetNumCols(); ii++) {
            wks.Columns(ii).SetLongName(Y_NAME_INTENSITY);
            wks.Columns(ii).SetUnits(Y_UNIT_INTENSITY);
        }

        // generate sparklines
        if (addSparks == 1) {
            WORKSHEET_addSparklines(wks);
        }
    }

    return wb;
}

WorksheetPage IMPORT_LabViewMaps(vector<string> params,
                                 vector<string> strFiles) {
    // user information
    printf(IMPORT_READING);

    // map required user parameters
    string wbName  = params[1];
    string wksName = params[2];
    int delim      = atoi(params[3]);
    int numSep     = atoi(params[4]) - 2;  // see definition of NF_IS_AUTO

    // create target workbook for import
    WorksheetPage wb = ORIGIN_createWb(wbName);

    // loop through source files
    for (int i = 0; i < strFiles.GetSize(); i++) {
        // create target worksheet for import
        Worksheet wks = ORIGIN_createWks(wb, MISC_stripName(strFiles[i]), true);

        ASCIMP ai;
        if (0 == AscImpReadFileStruct(strFiles[i], &ai)) {
            // setup headers
            ai.iMode = ASCIMP_MODE_NEW_SHEETS;  // open new worksheet per file
            ai.iRenameWks  = 0;                 // do not automatically rename
            ai.iNonnumeric = 1;  // read nonnumeric values as missing
            ai.iDelimited  = 1;  // files must be delimited
            ai.iDelimiter  = delim;
            ai.nNumSep     = numSep;

            // ignore headers (will be handled later)
            ai.iAutoSubHeaderLines = 0;
            ai.iSubHeaderLines     = 0;

            // import file
            wks.ImportASCII(strFiles[i], ai);

            // set basic column headers
            wks.Columns(0).SetType(
                OKDATAOBJ_DESIGNATION_X);  // set wavelength as X
            wks.Columns(0).SetLongName(X_NAME_WAVELENGTH);
            wks.Columns(0).SetUnits(X_UNIT_WAVELENGTH);
            for (int ii = 1; ii < wks.GetNumCols(); ii++) {
                wks.Columns(ii).SetLongName(Y_NAME_INTENSITY);
                wks.Columns(ii).SetUnits(Y_UNIT_INTENSITY);
            }

            // handle header lines if any
            int paramCount = IMPORT_processHeaders(wks, 0, true);

            // add user parameter rows
            WOKRSHEET_addUserParameter(wks, "X (" + XYZ_MATRIX_STEPU_PRE + ")",
                                       paramCount);
            WOKRSHEET_addUserParameter(wks, "Y (" + XYZ_MATRIX_STEPU_PRE + ")",
                                       paramCount + 1);

            // write coordinates to parameters
            string xLabel, yLabel;
            for (int jj = 1; jj < wks.GetNumCols(); jj++) {
                wks.GetCell(0, jj, xLabel);
                wks.GetCell(1, jj, yLabel);
                wks.Columns(jj).SetExtendedLabel(xLabel, RCLT_UDL + paramCount);
                wks.Columns(jj).SetExtendedLabel(yLabel,
                                                 RCLT_UDL + paramCount + 1);
            }

            // remove axes
            wks.DeleteRow(0);  // remove x axis
            wks.DeleteRow(0);  // remove y axis
        }
    }

    return wb;
}

void IMPORT_Tracks(vector<string> params, vector<string> strFiles) {
    // user information
    printf(IMPORT_READING);

    // map required user parameters
    string wbName  = params[1];
    string wksName = params[2];

    // create target workbook/worksheet for import
    WorksheetPage wb = ORIGIN_createWb(wbName);
    Worksheet wks    = ORIGIN_createWks(wb, wksName);

    // loop through source files
    for (int i = 0; i < strFiles.GetSize(); i++) {
        // read XML
        Tree tr;
        if (tr.Load(strFiles[i])) {
            // progress info
            printf(IMPORT_PROGRESS, i + 1, strFiles.GetSize());

            // create target worksheet for import
            Worksheet wks =
                ORIGIN_createWks(wb, MISC_stripName(strFiles[i]), true);

            // set wks size
            wks.SetSize(-1, 4 * tr.GetNodeCount());

            // get params
            string xyUnits, tUnits;
            double tScale, numParts;
            tr.GetAttribute("spaceUnits", xyUnits);
            tr.GetAttribute("timeUnits", tUnits);
            tr.GetAttribute("frameInterval", tScale);
            tr.GetAttribute("nTracks", numParts);

            // loop particles
            int iCol = 0;
            foreach (TreeNode tParticle in tr.Children) {
                // loop spots
                vector<double> partT, partX, partY, partD;
                foreach (TreeNode tSpot in tParticle.Children) {
                    double tempVal;
                    tSpot.GetAttribute("t", tempVal);
                    partT.Add(tempVal);
                    tSpot.GetAttribute("x", tempVal);
                    partX.Add(tempVal);
                    tSpot.GetAttribute("y", tempVal);
                    partY.Add(tempVal);
                }

                // recalculate data
                partT = partT * tScale;
                partX = -(partX - partX[0]);
                partY = -(partY - partY[0]);
                partD = sqrt(partX ^ 2 + partY ^ 2);

                // inset values
                for (int iAttr = 0; iAttr < 4; iAttr++) {
                    // next column
                    Column curCol = wks.Columns(iCol++);

                    // set column properties
                    vector<string> curData;
                    int colType    = OKDATAOBJ_DESIGNATION_Y;
                    string colName = "", colUnit = xyUnits;
                    switch (iAttr) {
                        case 0:
                            colType = OKDATAOBJ_DESIGNATION_X;
                            colName = X_NAME_TIME;
                            colUnit = tUnits;
                            convert_double_vector_to_string_vector(
                                partT, curData, partT.GetSize());
                            break;

                        case 1:
                            colName = "X";
                            convert_double_vector_to_string_vector(
                                partX, curData, partX.GetSize());
                            break;

                        case 2:
                            colName = "Y";
                            convert_double_vector_to_string_vector(
                                partY, curData, partY.GetSize());
                            break;

                        case 3:
                            colName = "d";
                            convert_double_vector_to_string_vector(
                                partD, curData, partD.GetSize());
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

    // remove default worksheet
    wb.Layers("Sheet1").Destroy();
}

int IMPORT_processHeaders(Worksheet wks, int offset = 0, bool isMap = false) {
    // count header rows in data (first column)
    string cellValue;
    int iHeaders = 0;
    for (int i = 0; i < wks.Columns(1).GetNumRows(); i++) {
        wks.GetCell(i, 0, cellValue);
        if (is_numeric(cellValue)) {
            break;
        } else {
            iHeaders++;
        }
    }

    // extract parameter names
    int paramCount = offset;
    string paramName;
    vector<string> paramNames;
    for (int ii = 0; ii < iHeaders; ii++) {
        // get cell value
        wks.GetCell(0, 0, paramName);

        // skip section headers
        if (paramName.Find(":") > -1) {
            // strip spaces and colon
            paramName.TrimLeft();
            paramName.TrimRight(':');

            // create new user parameter
            WOKRSHEET_addUserParameter(wks, paramName, paramCount);

            // write parameter values to parameters
            string labelData;
            for (int jj = 0; jj < wks.GetNumCols(); jj++) {
                // skip repeating X columns
                if (wks.Columns(jj).GetType() == OKDATAOBJ_DESIGNATION_X) {
                    continue;
                }

                // extract label value
                wks.GetCell(0, jj, labelData);

                // strip space
                labelData.TrimLeft();
                labelData.TrimRight();

                // paste value to user parameter
                int tgtColInt = isMap ? 0 : jj;
                wks.Columns(tgtColInt).SetExtendedLabel(labelData,
                                                        RCLT_UDL + paramCount);

                // abort if only one parameter column
                if (isMap) {
                    break;
                }
            }

            // count user parameters
            paramCount++;
        }

        // remove header from data
        wks.DeleteRow(0);
    }

    // adjust column widths
    wks.AutoSize();

    return paramCount;
}