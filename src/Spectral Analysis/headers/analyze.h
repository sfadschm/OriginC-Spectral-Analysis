/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _ANALYZE_  // include once
#define _ANALYZE_

/**
 * This file provides methods for analyzing spectral datasets.
 */

/**
 * Run spectral analysis on a worksheet layer.
 *
 * @param Worksheet wks the worksheet to analyze
 */
void ANALYZE_spectra(Worksheet wks);

/**
 * Extract abscissa data for analysis from column index or user label and paste
 * into result sheet.
 *
 * @param Worksheet wks         the worksheet to analyze
 * @param Worksheet evalWks     the worksheet holding the results
 * @param int       designation the intended designation of the abscissa
 * @param int       labelInt    the index of the user label holding abscissa
 * @param int       dataSize    the size of the evaluation results for indexing
 */
void ANALYZE_createAbscissa(Worksheet wks, Worksheet evalWks, int designation,
                            int labelInt, int dataSize);

/**
 * Collect and re-sort data columns from multiple peak analysis result sheets.
 *
 * @param WorksheetPage  wb         the active workbook page
 * @param vector<string> srcNames   the names of the data source sheets
 * @param string         columnName the name of the column to extract
 * @param string         identifier the base name of the result sheets
 */
void ANALYZE_collectPeaks(WorksheetPage wb, vector<string> srcNames,
                          string columnName, string identifier);

#endif