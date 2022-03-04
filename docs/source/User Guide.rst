.. role:: underline
    :class: underline

.. cssclass:: title

   |front-title|

.. cssclass:: title-sub

   |front-subtitle|

**Last Update:** 07.03.2022 (v1.2.2)

This is an *OriginC* library providing several scripts for importing,
handling, and evaluating spectroscopic data. No warranty is given for
functionality or correctness, please verify all results generated with
this script manually.

All methods have been tested with *OriginPro 2021-2021b*; older program
versions should be updated anyways.

.. contents::
   :depth: 3

.. raw:: pdf

    PageBreak docsPage

Installation
============

1. Copy the file :literal:`\  build/Spectral Analysis.opx \ ` to a location on your
   computer.

2. Run *OriginPro*.

3. Drag and drop :literal:`\  Spectral Analysis.opx \ ` into *OriginPro*. The
   installation process will start automatically and add all required
   files to the *User Folder Workspace*.

   If any errors occur, the script will show a prompt. Please contact
   the script provider with the displayed error code.

4. All scripts will now be available after each start of *OriginPro*.

Usage
=====

1. Open the *Script Window*.

   (*Window* / *Script Window* or Shortcut *Alt + Shift + 3*)

2. Type the name of a script (see `Methods <#methods>`_) into the window and confirm with *Enter*.

   (Each *Enter* inside the *Script Window* will be interpreted as a
   command call. If the current row does not contain a valid script
   name, an error will be printed.)

Extending
=========

This package can easily be extended by other code modules.

See the `Developer Guide <Developer-Guide>`_ and the
*Example Plugin* code in the *src* directory.

Debugging
=========

If you need to debug the runtime of any function from this library or
your own, you can use the :literal:`\  timer(string label) \ ` method.

1. Skipping the *label* parameter resets the timer without output::

       timer();
       // no output

2. To generate output, pass a descriptor to the *label* parameter::

       timer("Transpose Sheet");
       // output: "Transpose Sheet: 19 ms"

Methods
=========

All callable methods can be looked up in the file :literal:`\  main.c \ ` and do not
require any parameters to be passed when calling them.

Helper methods are in the header files (:literal:`\  *.h \ `) in the corresponding
subfolders. They can in principle also be called from the *Script Window*,
however, in this case the function parameters must be passed immediately.

Data Handling
-------------

import
^^^^^^

Automates data import from a variety of sources. The import functions
for spectra, 3D-maps and XY-|lambda|-I data support pushing header lines to the
OriginPro worksheet user parameters with the pseudo-syntax
*ParamName:{TAB}Value*.

:underline:`Parameters`
"""""""""""""""""""""""

  .. list-table::
     :widths: 30 70
     :header-rows: 1

     * - **Parameter**
       - **Description**

     * - :literal:`\  Target Workbook \ `
       - Name of the workbook to append the imported data to.
         If not set, will create a new workbook.

     * - :literal:`\  Target Worksheet \ `
       - Name of the worksheet to append the imported data
         to. If not set, will create a new worksheet.

     * - :literal:`\  Data Type \ `
       - The source type of the data (see *b*).

     * - :literal:`\  Delimiter \ `
       - The column value delimiter of the source file (usually
         *Tab*, sometimes *Whitespace*).

     * - :literal:`\  Dec. Separator \ `
       - The decimal separator of the source file.

     * - :literal:`\  Constant X-Axes \ `
       - If checked, removes the *X*-columns of all imported
         files except the first.

     * - :literal:`\  Create Sparklines? \ `
       - If checked, creates sparklines for all imported
         data columns. (Use with caution, creates heavy CPU load!)

:underline:`Datatypes`
""""""""""""""""""""""

  .. list-table::
     :widths: 25 75
     :header-rows: 1

     * - **Data Type**
       - **Description**

     * - :literal:`\  Spectra Files \ `
       - E.g., spectra from *LabSpec* or *UVProbe*. Imported
         file names will be written to the column comments.

     * - :literal:`\  3D-Maps \ `
       - E.g., time traces from *LabSpec*, peak maps from *NT-MDT* or
         Raman images generated with the *Raman Tool Set 2.0*. The file structure
         must be *XY-I* ... File names are used as worksheet names.
         *XY*-coordinates and other axes are written to the user parameters.

     * - :literal:`\  4D-Maps \ `
       - E.g., raw map data from *NT-MDT* (XY-|lambda|-I ...). File names
         are used as worksheet names. *XY*-coordinates are written to the user
         parameters. Note that *NT-MDT* data must be exported from *Image
         Analysis* as a *MATLAB* file (*\*.m*)!

     * - :literal:`\  Tracks \ `
       - XML-Files generated via *ImageJ TrackMate* containing particle
         tracking information. Metadata (e.g., *time axis* and *XY*-coordinates)
         are written to the user parameters.

correct
^^^^^^^

Manipulates all datasets in a selectable worksheet of the active
workbook. For each selected method, the results are stored in a new
worksheet, which is then used as the data source worksheet for the
subsequent correction method.

:underline:`Parameters` (vary depending on the chosen method)
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

  .. list-table::
     :widths: 20 80
     :header-rows: 1

     * - **Parameter**
       - **Description**

     * - :literal:`\  Data Source \ `
       - Dropdown field to select the source data worksheet.

     * - :literal:`\  Worksheet \ `
       - Dropdown field to select the reference data worksheet (only
         in selected methods).

     * - :literal:`\  Parameter \ `
       - Dropdown field to select the user parameter containing the
         reference data (in the current worksheet) or connecting the data and
         reference worksheets (only in selected methods).

:underline:`Methods` (selected via checkboxes in initial dialog)
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

  .. list-table::
     :widths: 37 63
     :header-rows: 1

     * - **Method**
       - **Description**

     * - :literal:`\  Masked Data Removal \ `
       - Clears all cells of the source worksheet that have
         been *masked* (e.g., in a graph window) and fills them with mean values
         (averaging window: 10 data points).

     * - :literal:`\  Background Subtraction \ `
       - Subtracts reference data from all datasets.

         The *Reference* mode uses reference spectra connected via a selectable
         user parameter.

         **Parameters:** *Worksheet*, *Parameter*

         The *Median* mode uses the median value of the user defined spectral
         subregion.

         **Parameters:** *Start*, *Stop*

         The *Constant* mode uses a fixed value.

         **Parameters:** *Value*

     * - :literal:`\  Spike Removal \ `
       - Automatically removes spikes from the worksheet. This
         method is based on the Whitaker-Hayer algorithm, see `Chemolab
         2018 <https://dx.doi.org/10.1016/j.chemolab.2018.06.009>`__.

         **Parameters:** *Z-Threshold*, *Averaging Width*

     * - :literal:`\  Setup Correction \ `
       - Applies a setup calibration curve to the source data
         by division. If no reference user label is selected, this method
         defaults to the first two columns of the reference worksheet.

         **Parameters:** *Worksheet, Parameter*

     * - :literal:`\  Filter Correction \ `
       - Corrects the source data with a set of neutral
         density filter transmission curves by division. The transmission data of
         the filters must be absolute values.

         **Parameters:** *Worksheet*, *Parameter*

     * - :literal:`\  Integration Correction \ `
       - Divides all datasets by the integration time
         provided in a user parameter of the source worksheet. The selected user
         label must only contain numerical data!

         **Parameters:** *Parameter*

     * - :literal:`\  Jacobian Transformation \ `
       - Performs a Jacobian Transformation to the
         energy space on all source datasets (Caution! Not to be used for PLE or
         absorption data.).

         **Parameters**: *none*

     * - :literal:`\  Normalisation \ `
       - Normalises all source datasets by dividing them by
         their respective maximum value.

         **Parameters:** *none*

analyze
^^^^^^^

Executes simple spectroscopic analytics on the current worksheet.

:underline:`Parameters`
"""""""""""""""""""""""

  .. list-table::
     :widths: 27 73
     :header-rows: 1

     * - **Parameter**
       - **Description**

     * - :literal:`\  Dataset Name \ `
       - The name that will appear in the long name of the result
         column in the evaluation sheet.

     * - :literal:`\  Method \ `
       - The mathematical evaluation method to apply to the data.

     * - :literal:`\  Skip Abscissae? \ `
       - If checked, does not generate new abscissa in the
         result sheet, e.g., if they have already been generated with the last
         run of the method.

     * - :literal:`\  X-Abscissa \ `
       - The name of the user label that holds the data
         representing the new *X* values of the evaluation result.

     * - :literal:`\  Y-Abscissa \ `
       - See :literal:`\  X-Abscissa \ `. If set to *none*, values in the
         result sheet will be assigned *Y*, if set, values
         will be assigned *Z* (e.g., for map analysis).

     * - :literal:`\  X Start Value \ `
       - The lower bound of *X* values to evaluate; if *0*, will
         default to the first data row.

     * - :literal:`\  X Stop Value \ `
       - The upper bound of *X* values to evaluate; if *0*, will
         default to the last data row.

:underline:`Methods`
""""""""""""""""""""

  .. list-table::
     :widths: 25 75
     :header-rows: 1

     * - **Method**
       - **Description**

     * - :literal:`\  Peak Position \ `
       - Finds the point with the highest *Y* value in the given
         range.

     * - :literal:`\  Mass Centre \ `
       - Calculates the centroid in the given range.

     * - :literal:`\  Peak Intensity \ `
       - Returns the highest *Y* value in the given range.

     * - :literal:`\  Peak Area \ `
       - Summarizes all *Y* values in the given range (pseudo-area)

     * - :literal:`\  Peak FWHM \ `
       - Calculates the *full width at half maximum* by finding the
         *Peak Position* *Y*\ max in the given range and iterating to
         higher and lower *X* values until *Y* |lteq| 0.5 |times| *Y*\ max in both
         directions.

map
^^^

Transforms the datasets in the currently active worksheet to a different
representation (e.g., generate *3D-Map* from *XYZ* columns).

:underline:`Parameters`
"""""""""""""""""""""""

  .. list-table::
     :widths: 20 80
     :header-rows: 1

     * - **Parameter**
       - **Description**

     * - :literal:`\  Method \ `
       - Dropdown field to select the data conversion method.


:underline:`Methods`
""""""""""""""""""""

  .. list-table::
     :widths: 30 70
     :header-rows: 1

     * - **Method**
       - **Description**

     * - :literal:`\  XYZ-Data to Matrix \ `
       - Transforms a *Z* dataset into an *XYZ*-Map. *XY*
         data can be extracted from the column designations or from user input.

     * - :literal:`\  4D-Linescan \ `
       - Extracts a line scan along one axis from an imported
         XY-|lambda|-I map (e.g., from *NT-MDT* or *LabView*).

interpolate
^^^^^^^^^^^

Interpolates all *XY*-datasets in the currently active worksheet onto a
new *X*-axis. The new axis must be included in the same worksheet.

:underline:`Parameters`
"""""""""""""""""""""""

  .. list-table::
     :widths: 20 80
     :header-rows: 1

     * - **Parameter**
       - **Description**

     * - :literal:`\  New X-Axis \ `
       - The source column containing the new *X*-axis data.

peaks
^^^^^

Convenience method for peak fitting that collects a selected column from
all *PeakProperties\** worksheets in the currently active workbook. The
data are collected in a new sheet, transposed and some basic statistics
(mean and standard deviation) are calculated. All results are
auto-updated such that adjustments to individual fittings will directly
be reflected.

:underline:`Parameters`
"""""""""""""""""""""""

  .. list-table::
     :widths: 25 75
     :header-rows: 1

     * - **Parameter**
       - **Description**

     * - :literal:`\  Name \ `
       - The dataset identifier used to name the result sheets.

     * - :literal:`\  Target Column \ `
       - The source column from the *PeakProperties* sheet.

setLowerBound
^^^^^^^^^^^^^

Replaces all values in the active worksheet smaller than *lowerBound*
with *0*.

*lowerBound* defaults to *0* if omitted (all negative data are set to *0*).

:underline:`Parameters`
"""""""""""""""""""""""

  .. list-table::
     :widths: 25 75
     :header-rows: 1

     * - **Parameter**
       - **Description**

     * - :literal:`\  lowerBound (0) \ `
       - This parameter must be passed when calling the
         method, there is no user dialog.

Miscellaneous
-------------

renameWbs
^^^^^^^^^

Renames the *Short Name* field of all workbooks in the current folder
based on their *Long Names* (e.g., to enable sorting in the *Horiba
Fluoressence* software).

reduce
^^^^^^

Reduces the project's CPU load by deleting (!) all sparklines in the
project and hiding all windows.

.. ######################### Definitions ######################### ..

.. sectnum::
    :depth: 3

.. header::

   .. rst-class:: headertable

   .. list-table::
      :width: 100%
      :header-rows: 0

      * - .. rst-class:: headerbody-sup

          |title-sup|

          .. rst-class:: headerbody

          |title|

.. footer::

   .. rst-class:: footertable

   .. list-table::
      :width: 100%
      :widths: 80 20
      :header-rows: 0

      * - .. rst-class:: footerbody-left

          |copy| 2017-2022 Alexander Schmitz-Wunderlich, University of Duisburg-Essen

        - .. rst-class:: footerbody-right

          ###Page### of ###Total###

.. |front-title|     replace:: OriginC Spectral Analysis
.. |front-subtitle|  replace:: User Guide

.. |title-sup|  replace:: Using
.. |title|      replace:: Spectral Analysis with OriginC

.. |copy|   unicode:: U+000A9 .. COPYRIGHT SIGN
.. |times|  unicode:: U+000D7 .. MULTIPLICATION SIGN
.. |lambda| unicode:: U+003BB .. GREEK SMALL LAMBDA
.. |lteq|   unicode:: U+02264 .. LESS THAN OR EQUAL