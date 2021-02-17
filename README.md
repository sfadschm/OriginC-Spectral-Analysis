Handling Data with OriginC

# Functionality

**Last Update:** 17.02.2021

This is a medium-size *OriginC* library providing several scripts for
importing, handling, and evaluating spectroscopic data.

No warranty is given for functionality or correctness, so please verify
all results generated with this script manually.

All methods have been tested with *OriginPro 2018-2021*; older program
versions should be updated anyways.

# Installation

1.  Copy the folder *OriginC* to a permanent location on your computer.

2.  Run *OriginPro* and open the *Code Builder*.

    (*View* → *Code Builder* or Shortcut *Alt + 4*)

3.  Rightclick the folder *User \[AutoLoad\]* in the *Workspace* and
    choose *Add files \...*.

4.  Navigate to your local copy of the *OriginC* folder and select the
    *main.c* file.

5.  Click *Rebuild All* from the toolbar (or Shortcut *Alt+ F8*).

6.  If the output window on the bottom left displays an error, please
    consult the suggestion box at Fr. Merker.

7.  Rightclick the *Origin C Workspace* in the *Workspace* tree and
    disable the *Build on Startup* option.

8.  All scripts will now be available after each start of *OriginPro*.
    The *Code Builder* can now be closed.

# Usage

1.  Open the *Script Window*.

    (*Window* → *Script Window* or Shortcut *Alt + Shift + 3*)

2.  Type the name of a script (see *D*) into the window and confirm with
    > *Enter*.

    (Each *Enter* inside the *Script Window* will be interpreted as a
    command call. If the current row does not contain a valid script
    name, an error will be printed.)

# Functions

All callable functions can be looked up in the file *main.c* and do not
require any parameters to \<\<be passed when calling them.

Subfunctions are in the header files (*\*.h*) in the corresponding
subfolders. These functions can in principle also be called from the
*Script Window*, however, in this case the function parameters must be
passed immediately.

## import

Automates data import from a variety of sources.

### Parameters

*Target Workbook*: Name of the workbook to append the imported data to.
If not set, will create a new workbook.

*Target Worksheet*: Name of the worksheet to append the imported data
to. If not set, will create a new worksheet.

*Data Type*: The source type of the data (see *b*).

*Delimiter*: The column value delimiter of the source file (usually
*Tab*, sometimes *Whitespace*).

*Dec. Separator*: The decimal separator of the source file.

*Constant X-Axes*: If checked, removes the *X*-Columns of all imported
files except the first.

*Create Sparklines?*: If checked, creates sparklines for all imported
data columns. (Use with caution! Creates heavy CPU load!)

### Datatypes

*Spectra Files*: E.g., spectra from *LabSpec* or *UVProbe*. Imported
file names will be written to the column comments.

*3D-Maps*: E.g., time traces from *LabSpec*, peak maps from *NT-MDT* or
Raman images generated with the *Raman Tool Set 2.0*. The file structure
must be *XY-I*... File names are used as worksheet names.
*XY*-coordinates and other axes are written to the user parameters.

*4D-Maps*: E.g., raw map data from *NT-MDT* (*XY-λ-I*...). File names
are used as worksheet names. *XY*-coordinates are written to the user
parameters. Note that *NT-MDT* data must be exported from *Image
Analysis* as a *MATLAB* file (*\*.m*)!

*Tracks*: XML-Files generated via the *ImageJ TrackMate* analysis
containing particle tracking information. The map metadata (e.g., *time
axis* and *XY*-coordinates) are written to the user parameters.

## correct

Manipulates all datasets in a selectable worksheet of the active
workbook. For each selected method, the results are stored in a new
worksheet, which is then used as the data source worksheet for the
subsequent correction method.

### Parameters (vary depending on the chosen method)

*Data Source*: Dropdown field to select the source data worksheet.

*Worksheet*: Dropdown field to select the reference data worksheet (only
in selected methods).

*Parameter*: Dropdown field to select the user parameter row containing
the reference data (in the current worksheet) or connecting the data and
reference worksheets (only in selected methods).

### Methods (selected via checkboxes in initial dialog)

*Clean Masked Data*: Clears all cells of the source worksheet that have
been *masked* (e.g., in a graph window) and fills them with mean values
(averaging window: 10 data points).

*Background Subtraction*: Subtracts reference data from all datasets.

The *Reference* mode uses reference spectra connected via a selectable
user parameter.

[Parameters:]{.ul} *Worksheet*, *Parameter*

The *Average* mode uses the mean value of the user defined spectral
subregion.

[Parameter]{.ul}s: *Start*, *Stop*

*Spike Removal*: Automatically removes spikes from the work­sheet. This
method is based on the Whitaker-Hayer algorithm, see [Chemolab
2018](https://dx.doi.org/10.1016/j.chemolab.2018.06.009).

[Parameters:]{.ul} *Z-Threshold*, *Averaging Width*

*Setup Correction*: Applies a setup calibration curve to the source data
by division. Currently, only the first two columns of the reference
worksheet are used as a single *XY* dataset.

[Parameters:]{.ul} *Worksheet*

*Filter Correction*: Corrects the source data with a set of neutral
density filter transmission curves by division. The transmission data of
the filters must be absolute values.

[Parameters]{.ul}: *Worksheet*, *Parameter*

*Integration Time*: Divides all datasets by the integration time
provided in a user parameter of the source worksheet. The selected user
label must only contain numerical data!

[Parameters]{.ul}: *Parameter*

*Energy Transformation*: Performs a Jacobian Transformation to the
energy space on all source datasets (Caution! Not to be used for PLE or
absorption data.).

[Parameters]{.ul}: *none*

*Data Normalisation*: Normalises all source datasets by dividing them by
their respective maximum value.

[Parameters]{.ul}: *none*

## analyse

Executes simple spectroscopic analytics on the current worksheet or
workbook.

### Parameters

*Method*: Dropdown field to select the evaluation method.

*Source Type*: Dropdown field to select the data source type.

### Methods

*Spectra*: Basic spectral analysis of all *XY*-datasets in the worksheet
or workbook (e.g., *Peak Position*, *Area*, *FWHM*, ...).

*4D-Linescan*: Extracts a line scan along one axis from an imported
*XY-λ-I m*ap (e.g., *NT-MDT*). Note that the parameter *width* is not
implemented yet.

## convert

Transforms the datasets in the currently active worksheet to a different
representation (e.g., generate *3D-Map* from *XYZ* columns).

### Parameters

*Method*: Dropdown field to select the data conversion method.

### Methods

*XYZ-Data to Matrix*: Transforms a *Z* dataset into an *XYZ*-Map. *XY*
data can be extracted from the column designations or from user input.

## peaks

Convenience method for peak fitting that collects a selected column from
all *PeakProperties\** worksheets in the currently active workbook. The
data are collected in a new sheet, transposed and some basic statistics
(mean and standard deviation) are calculated. All results are
auto-updated such that adjustments to individual fittings will directly
be reflected.

### Parameters

*Name*: The dataset identifier used to name the result sheets.

*Target Column*: The source column from the *PeakProperties* sheet.

## alignText

Aligns a selectable text field in the currently active graph layer. The
active window must be a graph window!

### Parameters

*Text Object*: Dropdown field to select the text element that is to be
aligned (e.g., *XB* = X-Bottom, *YL* = Y-Left, ...)

*Alignment*: Dropdown field to select the desired text field alignment.

## renameWbs

Renames the *Short Name* field of all workbooks in the current folder
based on their *Long Names* (e.g., to enable sorting).

## setLowerBound (*lowerBound* = 0)

Replaces all values in the active worksheet smaller than *lowerBound*
with *0*. *lowerBound* defaults to *0* if omitted (all negative data are
set to *0*).

Note that the *lowerBound* parameter must be passed when calling the
method.

## reduce

Reduces the project's CPU load by deleting (!) all sparklines in the
project and hiding all windows.
