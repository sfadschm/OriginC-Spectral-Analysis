/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _MAP_  // include once
#define _MAP_

/**
 * This file provides helper methods for analyzing map data.
 */

/**
 * Extract a linescan from a map of XY-I data.
 *
 * @param WorksheetPage wb the workbook to extract the line from
 */
void MAP_4D_Linescan(Worksheet wks);

/**
 * Generate matrices from XYZ columns by defining the colspan.
 *
 * @param Worksheet wks the source worksheet
 */
void MAP_XYZtoMatrix(Worksheet wks);

#endif