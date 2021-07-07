/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _ORIGIN_  // include once
#define _ORIGIN_

/**
 * This file provides helper methods for Origin objects.
 */

/**
 * Create a new workbook and rename it.
 *
 * @param string wbName() the desired name of the new workbook
 *
 * @return WorksheetPage wb the workbook object
 */
WorksheetPage ORIGIN_createWb(string wbName = "");

/**
 * Create a new worksheet in a given workbook.
 *
 * @param WorksheetPage wb       the parent workbook of the worksheet to create
 * @param string        wksName  the desired name of the worksheet to create
 * @param bool          forceNew create new worksheet even if name exists
 *
 * @return Worksheet wks the worksheet object
 */
Worksheet ORIGIN_createWks(WorksheetPage wb, string wksName = "",
                           bool forceNew = false);

/**
 * Get the indexes of all worksheets in the active workbook.
 *
 * @param int            sourceType 0 if source is workbook, 1 if worksheet
 * @param WorksheetPage& wb         the reference to link the active workbook
 * @param Worksheet&     wks        the reference to link the active worksheet
 *
 * @return vector<int> sourceWksInts numeric indexes of the active worksheets
 */
vector<int> ORIGIN_getActiveWorksheets(int sourceType, WorksheetPage& wb,
                                       Worksheet& wks);

/**
 * Create a new matrixpage and rename it.
 *
 * @param string mpName() the name of the page to create
 *
 * @return MatrixPage mp the page object
 */
MatrixPage ORIGIN_createMp(string mpName = "");

/**
 * Create a new matrix layer in a given matrix page.
 *
 * @param MatrixPage wb       the parent page of the matrix layer to create
 * @param string     mlName   the desired name of the matrix layer to create
 * @param bool       forceNew create new layer even if name exists
 *
 * @return MatrixLayer ml the layer object
 */
MatrixLayer ORIGIN_createMl(MatrixPage mp, string mlName = "",
                            bool forceNew = false);

#endif