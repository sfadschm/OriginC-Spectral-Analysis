/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _USER_  // include once
#define _USER_

/**
 * This file defines the user interaction interfaces (dialogues).
 */

/**
 * Open a single file selection dialog.
 *
 * @param string boxTitle the title of the dialog box
 *
 * @return string strFile the full path to the selected file
 */
vector<string> USER_selectFiles();

/**
 * Open a multiple input box to setup data import.
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_importData();

/**
 * Open a multi field input box to setup the worksheet labeling.
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_readLabels();

/**
 * Open a multiple input box to setup spectra analysis.
 *
 * @param Worksheet wks the worksheet to read the user labels from
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_analyzeSpectra(Worksheet wks);

/**
 * Open a multi field input box to setup a 3D-Map linescan extraction.
 *
 * @param Worksheet wks the source data worksheet
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_map4dLinescan(Worksheet wks);

/**
 * Open a multiple input box to setup linescan parameters.
 *
 * @param vector<string> coordStrV the string values of available coordinates
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_linescan(vector<string> coordStrV);

/**
 * Open a multiple input box to setup data manipulation.
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_correctData(WorksheetPage wb);

/**
 * Open a multiple input box to setup data manipulation.
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_correctDataSource(WorksheetPage wb, Worksheet dataWks,
                                      int method, string title, int step = 1);

/**
 * Open a multiple input box to setup map data handling.
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_map();

/**
 * Open a multi field input box to setup a map dimension.
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_xyzMatrix();

/**
 * Open a multiple input box to setup peak collection.
 *
 * @param WorksheetPage  wb         the workbook to evaluate
 * @param vector<string> sheetNames sheet names with valid data for collection
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_peaks(WorksheetPage wb, vector<string> sheetNames);

/**
 * Open a multiple input box to setup data interpolation.
 *
 * @param Worksheet wks the worksheet to evaluate
 *
 * @return vector<string> params the user input parameters
 */
vector<string> USER_interpolate(Worksheet wks);

/**
 * Extracts all non-hidden user labels from a worksheet
 *
 * @param Worksheet wks the worksheet to extract labels from
 */
vector<string> USER_getLabelNames(Worksheet wks);

/**
 * Extracts the index of a workshee user label from its name.
 *
 * @param Worksheet wks       the worksheet holding the label
 * @param string    labelName the name of the userlabel
 */
int USER_getUserLabelIndex(Worksheet wks, string labelName);

#endif