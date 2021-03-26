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
 */
// general
#define LABEL_TARGET_NAME   "Name:"
#define LABEL_WB_NAME       "Target Workbook"
#define LABEL_WKS_NAME      "Target Worksheet"
#define LABEL_COL_NAME      "Target Column"
#define LABEL_USERPARAMETER "Parameter"
#define PARAMS_EMPTY        "\nNo Parameters were set!"

// file selection
#define FILES_TITLE     "\nSelect File(s)"
#define FILES_EMPTY     "\nNo File(s) selected!"
#define FILES_TYPES     "[Data Files (*.txt, *.tsv, *.dac, *.m, *.xml)] *.txt; *.tsv; *.dac; *.m; *.xml|[All Files (*.*)] *.*"
#define FILES_NOT_FOUND "File not found!"

// column names and units
#define X_NAME_WAVELENGTH     "Wavelength"
#define X_UNIT_WAVELENGTH     "nm"
#define X_NAME_RAMAN          "Raman Shift"
#define X_UNIT_RAMAN          "1/cm"
#define X_NAME_ENERGY         "Energy"
#define X_UNIT_ENERGY         "eV"
#define X_NAME_TIME           "Time"
#define X_UNIT_TIME           "s"
#define Y_NAME_INTENSITY      "Intensity"
#define Y_UNIT_INTENSITY      "cts"
#define Y_NAME_INTENSITY_TIME "Intensity"
#define Y_UNIT_INTENSITY_TIME "cts/s"
#define Y_NAME_INTENSITY_NORM "Normalised Intensity"
#define Y_UNIT_INTENSITY_NORM ""

// import
#define IMPORT_START    "\nImporting data ..."
#define IMPORT_READING  "\nReading files ..."
#define IMPORT_LABELING "\nCreating labels ..."
#define IMPORT_STOP     "\nImport complete.\n"
#define IMPORT_PROGRESS "\nFile %d of %d"

#define IMPORT_TITLE   "Data Import"
#define IMPORT_DESC    "Setup Data Import"
#define IMPORT_METHOD  "Data Type"
#define IMPORT_METHODS "Spectra Files|3D Maps (Streak, LabSpec, Raman)|4D Map (LabView, XY-LI)|4D Maps (NT-MDT)|Image Tracks (ImageJ)"
#define IMPORT_DELIM   "Delimiter"
#define IMPORT_DELIMS  "Unknown \"?\"|Comma \",\"|Tab \"  \"|Whitespace \" \"|Semicolon \";\""
#define IMPORT_SEPAR   "Dec. Separator"
#define IMPORT_SEPARS  "Auto|Origin Default|Windows Default|American|European"
#define IMPORT_XAXIS   "Constant X-Axes?"
#define IMPORT_SPARKL  "Create Spaklines? (Slow!!!)"
#define IMPORT_SERIES  "Add Series Labels (User)?"

#define IMPORT_LABELS_TITLE "Data Import Labels"
#define IMPORT_LABELS_DESC  "Define Data Column Labeling"
#define IMPORT_LABELS_NAME  "Parameter Name"
#define IMPORT_LABELS_STEP  "Stepsize"

// correct
#define CORRECT_NO_LABEL          "No label data found. Aborting ...\n\n"
#define CORRECT_NO_LABELS         "User label '%s' not found in worksheet '%s'.\n"
#define CORRECT_NO_LABEL_SELECTED "No user label was selected. Aborting ..."

#define CORRECT_MSG_START  "%s started ...\n"
#define CORRECT_MSG_NO_REF "No reference data found for '%s'.\n"
#define CORRECT_MSG_STOP   "%s finished.\n\n"

#define CORRECT_TITLE         "Data Manipulation"
#define CORRECT_DESC          "Run several manipulation methods on worksheets in active workbook."
#define CORRECT_NODATAWKS     "No valid worksheet selected."
#define CORRECT_DATAWKS_LABEL "Data Source"

#define CORRECT_CLEAN       "Masked Data Removal"
#define CORRECT_BACKGROUND  "Background Subtraction"
#define CORRECT_SPIKES      "Spike Removal"
#define CORRECT_SETUP       "Setup Correction"
#define CORRECT_FILTERS     "Filter Correction"
#define CORRECT_INTEGRATION "Integration Correction"
#define CORRECT_TRANSFORM   "Jacobian Transformation"
#define CORRECT_NORMALISE   "Normalisation"
#define CORRECT_INTERPOLATE "Interpolation"

#define CORRECT_SHEET_CLEAN       "Cleaned"
#define CORRECT_SHEET_BACKGROUND  "Subtracted"
#define CORRECT_SHEET_SPIKES      "Despiked"
#define CORRECT_SHEET_SETUP       "Setup Corrected"
#define CORRECT_SHEET_FILTERS     "Filter Corrected"
#define CORRECT_SHEET_INTEGRATION "Integration Corrected"
#define CORRECT_SHEET_TRANSFORM   "Transformed"
#define CORRECT_SHEET_NORMALISE   "Normalised"
#define CORRECT_SHEET_INTERPOLATE "Interpolated"

#define CORRECT_COMMENT_NO_REF         "No reference data found!"
#define CORRECT_COMMENT_CLEAN          "Cleaned."
#define CORRECT_COMMENT_NORMALISE      "Normalised."
#define CORRECT_COMMENT_BACKGROUND_REF "Subtracted '%s!%s,%s'."
#define CORRECT_COMMENT_BACKGROUND_MED "Subtracted '%s'."
#define CORRECT_COMMENT_SPIKES         "Removed spikes (%s, %s)."
#define CORRECT_COMMENT_REF            "Corrected with '%s!%s,%s'."
#define CORRECT_COMMENT_INTEGRATION    "Divided by '%s'."
#define CORRECT_COMMENT_TRANSFORM      "Transformed."
#define CORRECT_COMMENT_INTERPOLATE    "Interpolated to new X-axis."

#define CORRECT_CONFIRM_HINT   "Confirm with OK or abort with Cancel!"
#define CORRECT_REFERENCE_HINT "Choose reference data source:"
#define CORRECT_USERLABEL_HINT "Choose reference label:"

#define CORRECT_SPIKES_HINT            "Set parameters for Whitaker Hayes Algorithm:"
#define CORRECT_SPIKES_PARAM_THRESHOLD "Z-Threshold"
#define CORRECT_SPIKES_PARAM_WIDTH     "Averaging Width"

#define CORRECT_BACKGROUND_HINT        "Choose background correction method:"
#define CORRECT_BACKGROUND_PARAM_START "Start"
#define CORRECT_BACKGROUND_PARAM_STOP  "Stop"

#define CORRECT_BACKGROUND_METHODS        "Reference|Median"
#define CORRECT_BACKGROUND_MEDIAN_HINT    "Set median calculation boundaries:"

#define CORRECT_SETUP_DEFAULTING "No label data found for '%s'. Defaulting to (0,1)...\n"

// analyse
#define ANALYSE_START     "\nAnalysing data ..."
#define ANALYSE_STOP      "\nAnalysis complete.\n"
#define ANALYSE_NO_WKS    "Active layer is not a worksheet!"
#define ANALYSE_NO_METHOD "Method does not exist!"

#define ANALYSE_TITLE   "Data Analysis"
#define ANALYSE_DESC    "Setup Data Analysis of Active Window"
#define ANALYSE_WKS     "Worksheet"
#define ANALYSE_TARGET  "Source Type"
#define ANALYSE_TARGETS "Workbook|Worksheet"
#define ANALYSE_METHOD  "Method"
#define ANALYSE_METHODS "Spectra|4D-Linescan"

#define ANALYSE_GENERIC_INDEX "Index"

// spectral analysis
#define ANALYSE_SPECTRA_TITLE           "Evaluation"
#define ANALYSE_SPECTRA_DESC            "Define Calculation Parameters"
#define ANALYSE_SPECTRA_METHODS         "Peak Position|Mass Centre|Peak Intensity|Peak Area|Peak FWHM"
#define ANALYSE_SPECTRA_LABEL_TARGET    "Dataset Name"
#define ANALYSE_SPECTRA_TARGET          "Evaluation"
#define ANALYSE_SPECTRA_LABEL_METHOD    "Method"
#define ANALYSE_SPECTRA_LABEL_XSTART    "X Start Value"
#define ANALYSE_SPECTRA_LABEL_XSTOP     "X Stop Value"

// 4D-Map analysis
#define ANALYSE_LINESCAN_TITLE "Linescan Coordinate"
#define ANALYSE_LINESCAN_DESC  "Set Linescan Parameters"
#define ANALYSE_LINESCAN_COORD "Coordinate"
#define ANALYSE_LINESCAN_WIDTH "Width"
#define ANALYSE_LINESCAN_NAME  "Linescans"

// convert
#define CONVERT_START "\nProcessing data ..."
#define CONVERT_STOP  "\nConversion completed.\n"

#define CONVERT_TITLE   "Data Conversion"
#define CONVERT_DESC    "Choose Processing Method"
#define CONVERT_METHOD  ANALYSE_METHOD
#define CONVERT_METHODS "XYZ-Data to Matrix"
#define CONVERT_SELCOL  "Please select a single range of data in the active worksheet first!"

// convert XYZ to matrix
#define XYZ_MATRIX_TITLE   "Setup Map Dimensions"
#define XYZ_MATRIX_DESC    "Select XY-datarange or setup map dimensions."
#define XYZ_MATRIX_USECOLS "Use XY-Columns?"
#define XYZ_MATRIX_ZCOL    "Choose Datasets"
#define XYZ_MATRIX_XYCOL   "Choose XY-Range"

#define XYZ_MATRIX_COLC       "Column Count"
#define XYZ_MATRIX_STEPU      "Step Unit"
#define XYZ_MATRIX_STEPU_PRE  "µm"
#define XYZ_MATRIX_XSTEP      "X-Step"
#define XYZ_MATRIX_YSTEP      "Y-Step"
#define XYZ_MATRIX_SHEET_NAME "Maps"

// peaks
#define PEAKS_START    "\nCollecting peak data ..."
#define PEAKS_STOP     "\nPeak data completed.\n"
#define PEAKS_NO_WKS   "\nNo valid worksheets found in current workbook!"
#define PEAKS_TOO_LONG "\nCommand string is too long (%d), please reduce the number of worksheets!\nAborting ..."

#define PEAKS_TITLE "Peak Collection"
#define PEAKS_DESC  "Collect peaks from fitting results."

#define PEAKS_COLUMN_SHEET_SUFFIX "Cols"
#define PEAKS_ROW_SHEET_SUFFIX    "Rows"

#define PEAKS_HEADER_INDEX  "Peak Index"
#define PEAKS_HEADER_MEAN   "Mean"
#define PEAKS_HEADER_STDDEV "Std. Deviation"

// interpolation
#define INTERPOLATE_TITLE "Interpolate XY from X"
#define INTERPOLATE_DESC  "Project datasets on new X-axis."
#define INTERPOLATE_COL   "New X-Axis:"

// sparklines
#define SPARKS_START "\nCreating sparklines (might take some time) ..."
#define SPARKS_STOP  "\nSparklines created.\n"

// lowerbound
#define LOWERBOUND_START "\nSetting worksheet lower bound ..."
#define LOWERBOUND_STOP  "\nLower bound set to %.3f.\n"

// rename
#define RENAME_STOP "\nWorkbooks renamed.\n"

// reduce
#define REDUCE_STOP "\nAll sparklines have been deleted.\n"

// X-Functions
#define X_PARAM_FAILED "Could not set parameter for X-Function!\n"
#define X_EVAL_FAILED  "Could not evaluate X-Function!\n"

#endif