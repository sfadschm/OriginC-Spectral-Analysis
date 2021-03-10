/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _LANG_ // include once
#define _LANG_

/**
 * This file provides text constants for user and status messages.
 *
 * @toDo Group and rearrange headers/units.
 * @toDo Use MISC_formatString for IMPORT messages.
 */

/****************************** General *****************************/
#define LABEL_TARGET_NAME "Name:"
#define LABEL_WB_NAME     "Target Workbook"
#define LABEL_WKS_NAME    "Target Worksheet"
#define LABEL_COL_NAME    "Target Column"
#define USER_PARAMS_EMPTY "\nNo Parameters were set!"
/****************************** General End *****************************/

/****************************** Progress Information *****************************/
#define IMPORT_START     "\nImporting data ..."
#define IMPORT_READING   "\nReading files ..."
#define IMPORT_LABELING  "\nCreating labels ..."
#define IMPORT_STOP      "\nImport complete.\n"
#define ANALYSIS_START   "\nAnalysing data ..."
#define ANALYSIS_STOP    "\nAnalysis complete.\n"
#define SPARKS_START     "\nCreating sparklines (might take some time) ..."
#define SPARKS_STOP      "\nSparklines created.\n"
#define CORRECT_START    "\nModifying datasets ..."
#define CORRECT_STOP     "\nModificaton complete.\n"
#define CONVERSION_START "\nProcessing data ..."
#define CONVERSION_STOP  "\nConversion completed.\n"
#define RENAME_STOP      "\nWorkbooks renamed.\n"
#define LOWERBOUND_START "\nSetting worksheet lower bound ..."
#define LOWERBOUND_STOP  "\nLower bound set to %.3f.\n"
#define REDUCE_STOP      "\nAll sparklines have been deleted.\n"
#define PEAKS_START      "\nCollecting peak data ..."
#define PEAKS_STOP       "\nPeak data completed.\n"
/****************************** Progress Information End *****************************/

/****************************** User Dialogs *****************************/
// file selection
#define USER_FILES_TITLE "\nSelect File(s)"
#define USER_FILES_EMPTY "\nNo File(s) selected!"
#define USER_FILES_TYPES "[Data Files (*.txt, *.tsv, *.dac, *.m)] *.txt; *.tsv; *.dac; *.m|[All Files (*.*)] *.*"

// data import
#define USER_IMPORT_TITLE   "Data Import"
#define USER_IMPORT_DESC    "Setup Data Import"
#define USER_IMPORT_METHOD  "Data Type"
#define USER_IMPORT_METHODS "Spectra Files|3D Maps (Streak, LabSpec, Raman)|4D Map (LabView, XY-LI)|4D Maps (NT-MDT)|Image Tracks (ImageJ)"
#define USER_IMPORT_DELIM   "Delimiter"
#define USER_IMPORT_DELIMS  "Unknown \"?\"|Comma \",\"|Tab \"  \"|Whitespace \" \"|Semicolon \";\""
#define USER_IMPORT_SEPAR   "Dec. Separator"
#define USER_IMPORT_SEPARS  "Auto|Origin Default|Windows Default|American|European"
#define USER_IMPORT_XAXIS   "Constant X-Axes?"
#define USER_IMPORT_SPARKL  "Create Spaklines? (Slow!!!)"
#define USER_IMPORT_SERIES  "Add Series Labels (User)?"

#define USER_IMPORT_PROGRESS_INFO "\n\nFile %d of %d"

#define USER_IMPORT_XNAME_PRE  "Wavelength"
#define USER_IMPORT_XUNIT_PRE  "nm"
#define USER_IMPORT_YNAME_PRE  "Intensity"
#define USER_IMPORT_YUNIT_PRE  "cts"
#define USER_IMPORT_X2NAME_PRE "Raman Shift"
#define USER_IMPORT_X2UNIT_PRE "1/cm"

// peak collection
#define USER_PEAKS_TITLE "Peak Collection"
#define USER_PEAKS_DESC  "Collect peaks from fitting results."

// data interpolation
#define USER_INTERPOLATE_TITLE "Interpolate XY from X"
#define USER_INTERPOLATE_DESC  "Project datasets on new X-axis."
#define USER_INTERPOLATE_COL   "New X-Axis:"

// worksheet labels
#define USER_LABELS_TITLE     "Data Import Labels"
#define USER_LABELS_DESC      "Define Data Column Labeling"
#define USER_LABELS_NAME      "Parameter Name"
#define USER_LABELS_UNIT      "Parameter Unit"
#define USER_LABELS_STEP      "Stepsize"
#define USER_LABELS_NAME_PRE  "Time"
#define USER_LABELS_UNIT_PRE  "s"
#define USER_LABELS_NOT_FOUND "User Label '%s' not found in worksheet '%s'.\n"

// general analysis
#define USER_ANALYSE_TITLE   "Data Analysis"
#define USER_ANALYSE_DESC    "Setup Data Analysis of Active Window"
#define USER_ANALYSE_WB      "Workbook"
#define USER_ANALYSE_WKS     "Worksheet"
#define USER_ANALYSE_TARGET  "Source Type"
#define USER_ANALYSE_TARGETS "Workbook|Worksheet"
#define USER_ANALYSE_METHOD  "Method"
#define USER_ANALYSE_METHODS "Spectra|4D-Linescan"

#define ANALYSE_GENERIC_INDEX "Index"

// noise reduction
#define USER_NOISE_VALUE_LABEL  "Noise Level"
#define USER_NOISE_USEMIN_LABEL "Use current sheet min value?"
#define USER_NOISE_TITLE        "Noise Subtraction"
#define USER_NOISE_DESCR        "Define Noise Level (enter 0 to skip)."

// general analysis
#define ANALYSIS_TITLE           "Evaluation"
#define ANALYSIS_DESC            "Define Calculation Parameters"
#define ANALYSIS_LABEL_TARGET    "Dataset Name"
#define ANALYSIS_TARGET          "Evaluation"
#define ANALYSIS_LABEL_METHOD    "Method"
#define ANALYSIS_LABEL_PARAMETER "Parameter"
#define ANALYSIS_LABEL_XSTART    "X Start Value"
#define ANALYSIS_LABEL_XSTOP     "X Stop Value"
#define ANALYSIS_NO_WKS          "Active layer is not a worksheet!"
#define ANALYSIS_NO_WB           "Could not grab workbook!"
#define ANALYSIS_NO_METHOD       "Method does not exist!"

// spectra analysis
#define ANALYSIS_SPECTRA_METHODS "Peak Position|Mass Centre|Peak Intensity|Peak Area|Peak FWHM"

// data conversion
#define USER_CONVERT_TITLE   "Data Conversion"
#define USER_CONVERT_DESC    "Choose Processing Method"
#define USER_CONVERT_METHOD  USER_ANALYSE_METHOD
#define USER_CONVERT_METHODS "XYZ-Data to Matrix"
#define USER_CONVERT_SELCOL  "Please select a single range of data in the active worksheet first!"

// convert XYZ to matrix
#define USER_XYZ_MATRIX_TITLE   "Setup Map Dimensions"
#define USER_XYZ_MATRIX_DESC    "Select XY-datarange or setup map dimensions."
#define USER_XYZ_MATRIX_USECOLS "Use XY-Columns?"
#define USER_XYZ_MATRIX_ZCOL    "Choose Datasets"
#define USER_XYZ_MATRIX_XYCOL   "Choose XY-Range"
#define USER_XYZ_MATRIX_INVDR   "Invalid Data Range"

#define USER_XYZ_MATRIX_COLC      "Column Count"
#define USER_XYZ_MATRIX_STEPU     "Step Unit"
#define USER_XYZ_MATRIX_STEPU_PRE "µm"
#define USER_XYZ_MATRIX_XSTEP     "X-Step"
#define USER_XYZ_MATRIX_YSTEP     "Y-Step"

#define USER_XYZ_TARGET_NAME "Maps"

// data manipulation/correction
#define USER_CORRECT_TITLE         "Data Manipulation"
#define USER_CORRECT_DESC          "Run several manipulation methods on worksheets in active workbook."
#define USER_CORRECT_NODATAWKS     "No valid worksheet selected."
#define USER_CORRECT_DATAWKS_LABEL "Data Source"

#define USER_CORRECT_METHODS     "Clean Sheet|Background Subtraction|Spike Removal|Setup Correction|Filter Correction|Integration Time|Energy Transformation|Data Normalisation"
#define USER_CORRECT_CLEAN       "Masked Data Removal"
#define USER_CORRECT_BACKGROUND  "Background Subtraction"
#define USER_CORRECT_SPIKES      "Spike Removal"
#define USER_CORRECT_SETUP       "Setup Correction"
#define USER_CORRECT_FILTERS     "Filter Correction"
#define USER_CORRECT_INTEGRATION "Integration Correction"
#define USER_CORRECT_TRANSFORM   "Jacobian Transformation"
#define USER_CORRECT_NORMALISE   "Normalisation"
#define USER_CORRECT_INTERPOLATE "Interpolation"

#define USER_CORRECT_BACKGROUND_METHODS "Reference|Median"
#define USER_CORRECT_INTEGRATION_UNIT "cts/s"

#define USER_CORRECT_TRANSFORM_XNAME "Energy"
#define USER_CORRECT_TRANSFORM_XUNIT "eV"

#define USER_CORRECT_NORMALISE_YNAME "Normalised Intensity"
#define USER_CORRECT_NORMALISE_YUNIT ""

#define USER_CORRECT_SHEET_CLEAN       "Cleaned"
#define USER_CORRECT_SHEET_BACKGROUND  "Subtracted"
#define USER_CORRECT_SHEET_SPIKES      "Despiked"
#define USER_CORRECT_SHEET_SETUP       "Setup Corrected"
#define USER_CORRECT_SHEET_FILTERS     "Filter Corrected"
#define USER_CORRECT_SHEET_INTEGRATION "Integration Corrected"
#define USER_CORRECT_SHEET_TRANSFORM   "Transformed"
#define USER_CORRECT_SHEET_NORMALISE   "Normalised"
#define USER_CORRECT_SHEET_INTERPOLATE "Interpolated"

#define USER_CORRECT_SPIKES_PARAM_THRESHOLD "Z-Threshold"
#define USER_CORRECT_SPIKES_PARAM_WIDTH     "Averaging Width"

#define USER_CORRECT_BACKGROUND_PARAM_START "Start"
#define USER_CORRECT_BACKGROUND_PARAM_STOP  "Stop"

#define USER_CORRECT_MSG_CONFIRM  "Confirm with OK or abort with Cancel!"
#define USER_CORRECT_MSG_SOURCE   "Please select reference source."
#define USER_CORRECT_MSG_NO_LABEL "No label data found. Aborting ..."

#define USER_CORRECT_COMMENT_NO_REF         "No reference data found!"
#define USER_CORRECT_COMMENT_CLEAN          "Cleaned."
#define USER_CORRECT_COMMENT_NORMALISE      "Normalised."
#define USER_CORRECT_COMMENT_BACKGROUND_REF "Subtracted '%s!%s,%s'."
#define USER_CORRECT_COMMENT_BACKGROUND_MED "Subtracted '%s'."
#define USER_CORRECT_COMMENT_SPIKES         "Removed spikes (%s, %s)."
#define USER_CORRECT_COMMENT_REF            "Corrected with '%s!%s,%s'."
#define USER_CORRECT_COMMENT_INTEGRATION    "Divided by '%s'."
#define USER_CORRECT_COMMENT_TRANSFORM      "Transformed."
#define USER_CORRECT_COMMENT_INTERPOLATE    "Interpolated to new X-axis."

#define USER_CORRECT_MSG_START "%s started ...\n"
#define USER_CORRECT_MSG_STOP  "%s finished.\n\n"

#define USER_CORRECT_MSG_BACKGROUND_METHOD "Choose background correction method:"

// 4D-map linescan
#define USER_LINESCAN_TITLE "Linescan Coordinate"
#define USER_LINESCAN_DESC  "Set Linescan Parameters"
#define USER_LINESCAN_COORD "Coordinate"
#define USER_LINESCAN_WIDTH "Width"
#define USER_LINESCAN_NAME  "Linescans"

// peaks
#define PEAKS_NO_WKS "No valid worksheets found in current workbook!"

#define PEAKS_COLUMN_SHEET_SUFFIX "Cols"
#define PEAKS_ROW_SHEET_SUFFIX    "Rows"

#define PEAKS_HEADER_INDEX  "Peak Index"
#define PEAKS_HEADER_MEAN   "Mean"
#define PEAKS_HEADER_STDDEV "Std. Deviation"

// X-Functions
#define X_INIT_FAILED  "Could not initialise X-Function!\n"
#define X_PARAM_FAILED "Could not set parameter for X-Function!\n"
#define X_EVAL_FAILED  "Could not evaluate X-Function!\n"

// file handling
#define FILE_NOT_FOUND "File not found!"
/****************************** User Dialogs End *****************************/

#endif