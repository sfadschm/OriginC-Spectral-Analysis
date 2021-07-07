/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef CORRECT_  // include once
#define CORRECT_

/**
 * This file provides methods for manipulating (correcting) worksheet data.
 */

/**
 * Collect reference data from another worksheet by matching user parameters.
 *
 * @param Worksheet tgtWks        the worksheet holding the data
 * @param Worksheet refWks        the worksheet holding the background data
 * @param string    userLabelName the user label holding the reference data
 *
 * @return maxtrix<int> refData a (x, 2) matrix containing the x-/y-indices
 */
matrix<int> CORRECT_getRefData(Worksheet tgtWks, Worksheet refWks,
                               string userLabelName);

/**
 * Subtracts reference background data from a datasheet.
 *
 * @param Worksheet tgtWks        the worksheet holding the data
 * @param Worksheet refWks        the worksheet holding the background data
 * @param string    userLabelName the user label holding the reference data
 */
void CORRECT_backgroundRef(Worksheet tgtWks, Worksheet refWks,
                           string userLabelName);

/**
 * Subtracts a median background from each column of a datasheet.
 *
 * @param Worksheet tgtWks the worksheet holding the data
 * @param double    xStart the start value of the median area
 * @param double    xStop  the end value of the median area
 */
void CORRECT_backgroundMedian(Worksheet tgtWks, double xStart, double xStop);

/**
 * Subtracts a constant background from each column of a datasheet.
 *
 * @param Worksheet tgtWks  the worksheet holding the data
 * @param double    bgValue the background level
 */
void CORRECT_backgroundConstant(Worksheet tgtWks, double bgValue);

/**
 * Set all masked data to undefined and remove mask.
 *
 * @param Worksheet wks the worksheet
 */
void CORRECT_masked(Worksheet wks);

/**
 * Remove spikes from all y-Datasets in a worksheet (Whitaker Hayes Algorithm).
 *
 * @param Worksheet tgtWks    the worksheet holding the data
 * @param double    threshold the threshold for the "z-Value"
 * @param int       width     the step width to caluclate mean values from
 */
void CORRECT_spikes(Worksheet wks, double threshold, int width);

/**
 * Correct measurement data with an instrument response function (by division).
 *
 * @param Worksheet tgtWks the worksheet holding the data
 * @param Worksheet refWks the worksheet holding the setup correction data
 * @param string    userLabelName the user label connecting the reference data
 */
void CORRECT_setup(Worksheet tgtWks, Worksheet refWks, string userLabelName);

/**
 * Correct measurement data for neutral density filter attenuation.
 * Filter data must be provided on an absolute transmittance scale.
 *
 * @param Worksheet tgtWks        the worksheet holding the data
 * @param Worksheet refWks        the worksheet holding the filter data
 * @param string    userLabelName the user label connecting the reference data
 */
void CORRECT_filters(Worksheet tgtWks, Worksheet refWks, string userLabelName);

/**
 * Divide measurement data by corresponding integration times.
 *
 * @param Worksheet tgtWks        the worksheet holding the data
 * @param string    userLabelName the user label holding integration times
 */
void CORRECT_integrationTime(Worksheet tgtWks, string userLabelName);

/**
 * Perfom a Jacobian energy transformation on spectroscopic data.
 *
 * @param Worksheet tgtWks the worksheet holding the data
 */
void CORRECT_transform(Worksheet tgtWks);

/**
 * Normalise data in a worksheet (divide by maximum).
 *
 * @param Worksheet tgtWks the worksheet holding the data
 */
void CORRECT_normalise(Worksheet tgtWks);

/**
 * Interpolate xy-datasets over new x-axis.
 *
 * @param WorksheetPage wb      the source worksheetpage
 * @param Worksheet     wks     the source worksheet
 * @param int           newXInt the column index of the new x-axis
 */
void CORRECT_interpolate(WorksheetPage wb, Worksheet wks, int newXInt);

#endif