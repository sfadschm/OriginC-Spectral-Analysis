/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _MATRIXPAGE_  // include once
#define _MATRIXPAGE_

/**
 * This file provides methods for Origin MatrixPage manipulation.
 */

/**
 *  Adds a new matrix layer to a matrix page.
 *
 *  @param WorksheetPage wb        the target workbook
 *  @param string        wksName() the name of the new worksheet
 *
 *  @return int wksInt the index of the new worksheet in the workbook
 **/
int MATRIXPAGE_addMatrixLayer(MatrixPage mp, string mlName = "");

#endif