/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _WORKBOOK_  // include once
#define _WORKBOOK_

/**
 * This file provides helper methods for workbook manipulation.
 */

/**
 * Add a new worksheet to a workbook.
 *
 * @param WorksheetPage wb        the target workbook
 * @param string        wksName() the name of the new worksheet
 *
 * @return int wksInt the index of the new worksheet in the workbook
 */
int WORKBOOK_addWorksheet(WorksheetPage wb, string wksName = "");

#endif