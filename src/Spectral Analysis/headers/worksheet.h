/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _WORKSHEET_  // include once
#define _WORKSHEET_

/**
 * This file provides methods for manipulating worksheet objects.
 */

/**
 * Add and show a user defined parameter row in a worksheet.
 *
 * @param Worksheet wks       the target worksheet
 * @param string    paramName the name of the user parameter
 * @param int       offset(0) offset of the user parameter row
 */
void WOKRSHEET_addUserParameter(Worksheet wks, string paramName,
                                int offset = 0);

/**
 * Create Sparklines for a single worksheet.
 *
 * @param Worksheet* wks the target worksheet (by reference)
 *
 * @return bool true/false the result of sparkline updating
 *
 * @warning this function might take a lot of time to complete!
 */
bool WORKSHEET_addSparklines(Worksheet &wks);

/**
 * Get the lowest Y-value in a given worksheet.
 *
 * @param Worksheet wks the worksheet to read out
 */
double WORKSHEET_getYMin(Worksheet wks);

/**
 * Get user label data from a worksheet by label name.
 *
 * @param Worksheet wks       the worksheet
 * @param string    labelName the name of the user label to read
 *
 * @return vector<string> labelData the data included in the target label row
 */
vector<string> WORKSHEET_getUserLabelData(Worksheet wks, string labelName);

/**
 * Define lower bound for values in a worksheet.
 *
 * @param Worksheet wks        the worksheet
 * @param double    lowerBound the lower bound of allowed values in the sheet
 */
void WORKSHEET_setLowerBound(Worksheet wks, double lowerBound);

#endif