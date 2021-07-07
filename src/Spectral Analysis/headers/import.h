/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _IMPORTER_  // include once
#define _IMPORTER_

/**
 * This file provides methods for importing data from various file formats.
 */

/**
 * Import and format a (consecutive) series of ASCII type spectra files.
 *
 * @param vector<string> params   the user parameters
 * @param vector<string> strFiles the names of the input data files
 *
 * @return Worksheet wks the worksheet containing the imported data
 */
Worksheet IMPORT_spectra(vector<string> params, vector<string> strFiles);

/**
 * Import and format spectral data from a one or more 3D Map ASCII files.
 * E.g. from Streak, LabSpec timetraces or evaluated NT-MDT maps.
 *
 * @param vector<string> params   the user parameters
 * @param vector<string> strFiles the names of the input data files
 *
 * @return WorksheetPage wb the workbook containing the imported data
 */
WorksheetPage IMPORT_3dMaps(vector<string> params, vector<string> strFiles);

/**
 * Import and format spectral data from a one or more 4D Map ASCII files.
 * E.g., NT-MDT maps (in MATLab format).
 *
 * @param vector<string> params the user parameters
 * @param vector<string> strFiles the names of the input data files
 *
 * @return WorksheetPage wb the workbook containing the imported data
 */
WorksheetPage IMPORT_4dMaps(vector<string> params, vector<string> strFiles);

/**
 * Import and format spectra data from a one or more 4D Map ASCII files.
 * E.g., LabView maps, XY-LI maps.
 *
 * @param vector<string> params the user parameters
 * @param vector<string> strFiles the names of the input data files
 *
 * @return WorksheetPage wb the workbook containing the imported data
 */
WorksheetPage IMPORT_LabViewMaps(vector<string> params,
                                 vector<string> strFiles);

/**
 * Import and format particle tracking data from one or more ImageJ XML files.
 *
 * @param vector<string> params the user parameters
 * @param vector<string> strFiles the names of the input data files
 *
 * @return WorksheetPage wb the workbook containing the imported data
 */
void IMPORT_Tracks(vector<string> params, vector<string> strFiles);

#endif