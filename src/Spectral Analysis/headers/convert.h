/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _CONVERT_  // include once
#define _CONVERT_

/**
 * This file provides helper methods for converting matrix data.
 */

/**
 * Generate matrixes from XYZ columns by defining the colspan.
 *
 * @param Worksheet      wks    the source worksheet
 * @param vector<string> params the mapping parameters (user defined)
 */
void CONVERT_XYZtoMatrix(Worksheet wks, vector<string> params);

#endif