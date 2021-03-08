/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _CORRECT_ // include once
#define _CORRECT_

/**
 * This file provides methods for manipulating (correcting) worksheet data.
 *
 * @toDo Outsource reference data collection.
 */
#include <Origin.h>
#include <XFBase.h>

/**
 * Subtracts reference background data from a datasheet.
 *
 * @param Worksheet tgtWks        the worksheet holding the data
 * @param Worksheet refWks        the worksheet holding the background data
 * @param string    userLabelName the user label to connect data and reference dataset
 */
void CORRECT_backgroundRef(Worksheet tgtWks, Worksheet refWks, string userLabelName)
{
	// user information
	out_str("Background Subtraction started ...");

	// get user labels of reference and source
	vector<string> refLabelData, tgtLabelData;
	refLabelData = WORKSHEET_getUserLabelData(refWks, userLabelName);
	tgtLabelData = WORKSHEET_getUserLabelData(tgtWks, userLabelName);

	// abort if no label data found
	if(refLabelData.GetSize() == 0 || tgtLabelData.GetSize() == 0)
	{
		out_str("No label data found. Aborting ...");
		return;
	}

	// set helpers
	Column xCol, xTempCol;
	int xInt = -1, yInt = -1, refXInt = -1, refYInt = -1, xTempInt = tgtWks.AddCol();
	tgtWks.Columns(xTempInt).SetType(OKDATAOBJ_DESIGNATION_X);

	// get column designations
	string colTypes    = tgtWks.GetColDesignations();
	string refColTypes = refWks.GetColDesignations();

	// loop through all columns
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++)
	{
		// set column indexes
		if(colTypes.GetAt(colInt) == 'Y')
		{
			yInt = colInt;
		}
		if(colTypes.GetAt(colInt) == 'X')
		{
			if(xInt >= 0)
			{
				// get X-data
				vector<double> xTempDataV = tgtWks.Columns(xTempInt).GetDataObject();

				// paste converted X-data
				vectorbase& xColObj = tgtWks.Columns(xInt).GetDataObject();
				xColObj = xTempDataV;
			}

			// set new X-data column index
			xInt = colInt;
			continue;
		}

		// get source data range
		XYRange data;
		data.Add(tgtWks, xInt, "X");
		data.Add(tgtWks, yInt, "Y");

		// set target data range
		XYRange dataTgt;
		dataTgt.Add(tgtWks, xTempInt, "X");
		dataTgt.Add(tgtWks, yInt, "Y");

		// get reference data range by user label
		refYInt = refLabelData.Find(tgtLabelData[yInt]);

		// skip if no reference data found
		if(refYInt < 0 || tgtLabelData[yInt] == "")
		{
			tgtWks.Columns(yInt).SetComments("No reference data found!");
			continue;
		}

		// search reference X-axis
		refXInt = refColTypes.Find('X');
		while(refColTypes.Find('X', refXInt + 1) < refYInt && refColTypes.Find('X', refXInt + 1) >= 0)
		{
			refXInt = refColTypes.Find('X', refXInt + 1);
		}

		// get reference data range
		XYRange ref;
		ref.Add(refWks, refXInt, "X");
		ref.Add(refWks, refYInt, "Y");

		// prepare X-Function
		XFBase xf("subtract_ref");
		if(!xf.SetArg("iy1", data) | !xf.SetArg("iy2", ref) | !xf.SetArg("common", 1) | !xf.SetArg("oy", dataTgt))
		{
			out_str("Failed to set parameters!");
			continue;
		}

		// run X-Function
		if(!xf.Evaluate())
		{
			out_str("Failed to evaluate the subtract_ref X-Function.");
			continue;
		}

		// set comment
		tgtWks.Columns(yInt).SetComments("Subtracted '" + refWks.GetName() + "!" + refXInt + "," + refYInt + "'.");
	}

	// remove temporary X-Column
	tgtWks.DeleteCol(xTempInt);

	// user information
	out_str("Background Subtraction finished.\n");
}


/**
 * Subtracts an averaged background from each column of a datasheet (using Median).
 *
 * @param Worksheet tgtWks        the worksheet holding the data
 * @param Worksheet refWks        the worksheet holding the background data
 * @param string    userLabelName the user label to connect data and reference dataset
 */
void CORRECT_backgroundMedian(Worksheet tgtWks, double xStart, double xStop)
{
	// user information
	out_str("Background Subtraction started ...");

	// get column designations
	string colTypes = tgtWks.GetColDesignations();

	// loop through all columns
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++)
	{
		// only process y-data
		if(colTypes.GetAt(colInt) != 'Y')
		{
			continue;
		}

		// get curve
		Curve dataCurve(tgtWks, colInt);

		// get curve boundaries
		double xMin,xMax,yMin,yMax;
		Curve_MinMax(&dataCurve, &xMin, &xMax, true, &yMin, &yMax);

		// set user boundaries
		if(xStart != 0)
		{
			xMin = xStart;
		}
		if(xStop != 0)
		{
			xMax = xStop;
		}

		// set rectangular boundaries
		fpoint fptTL(xMin, yMax);
		fpoint fptBR(xMax, yMin);

		// extract data from curve
		vector<double> vX, vY;
		vector<int> vIndex;
		dataCurve.GetRectPoints(fptTL, fptBR, vX, vY, vIndex);
		double test;

		// calculate median
		int nSize = vY.GetSize();
		QuantileOptions opt;
		QuantileResults res;
		opt.Median = true;
		ocmath_quantiles(vY, nSize, &opt, &res);    

		// subtract background
		dataCurve = dataCurve - res.Median;

		// set comment
		tgtWks.Columns(colInt).SetComments("Subtracted '" + res.Median + "'.");
	}

	// user information
	out_str("Background Subtraction finished.\n");
}

/**
 * Set all masked data to undefined and remove mask.
 *
 * @param Worksheet wks the worksheet
 */
void CORRECT_masked(Worksheet wks){
	// user information
	out_str("Cleaning masked data ...");

	// get column designations
	string colTypes = wks.GetColDesignations();

	// loop through all columns
	DataRange dr;
	Column dataCol;
	int xInt = 0, yInt = 0, dataSize;
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++)
	{
		// only manipulate Y-Columns
		if(colTypes.GetAt(colInt) != 'Y')
		{
			continue;
		}

		// Copy-paste data to remove masked data
		Dataset dsSrc(wks, colInt); 
		Data_copy(&dsSrc, &dsSrc, 0, -1, -1, FALSE);

		// remove mask from column (else cells are not writable
		dr.Add("CurRange", wks, 0, colInt, -1, colInt);
		dr.SetMask(FALSE, TRUE);
 
		// Create mean values for removed data
		dataCol = wks.Columns(colInt);
		vector &vData = dataCol.GetDataObject();
		dataSize = vData.GetSize();

		// Loop rows
		for(int n = 1; n < dataSize; n++)
		{
			// check if cell does not have a value
			if(is_missing_value(vData[n]))
			{
				// Initialize calculation helpers
				int seqCounter = 0;
				double seqSum = 0;
				for(int m = n - 5; m <= n + 5; m++)
				{
					// respect boundaries
					if(m > 0 && m < dataSize)
					{
						// only use "non-spikes"
						if(!is_missing_value(vData[m]))
						{
							seqCounter++;
							seqSum += vData[m];
						}
					}
				}

				// Write data to cell
				if(seqCounter > 0)
				{
					vData[n] =  round(seqSum / seqCounter, 0);
				}
				else
				{
					vData[n] = 0;
				}
			}
		}

		// add comment
		wks.Columns(colInt).SetComments("Cleaned.");
	}

	// user information
	out_str("Masked data have been cleaned.\n");
}


/**
 * Remove spikes from all y-Datasets in a worksheet (Whitaker Hayes Algorithm).
 *
 * @param Worksheet tgtWks    the worksheet holding the data
 * @param double    threshold the threshold for the "z-Value"
 * @param int       width     the step width to caluclate mean values from
 */
void CORRECT_spikes(Worksheet wks, double threshold, int width)
{
	// get column designations
	string colTypes = wks.GetColDesignations();

	// loop through all columns
	int dataSize;
	Column dataCol;
	double median, mad;
	vector vDiff, vZ, vSpikes;
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++)
	{
		// skip x/z-data
		if(colTypes.GetAt(colInt) != 'Y')
		{
			continue;
		}

		// get spectrum
		dataCol = wks.Columns(colInt);
		vector &vData = dataCol.GetDataObject();
		dataSize = vData.GetSize();

		// calculate diff series
		vector vDiff(dataSize - 1);
		for(int i = 1; i < dataSize; i++)
		{
			vDiff[i-1] = vData[i] - vData[i - 1];
		}

		// caluculate median
		QuantileOptions opt;
		QuantileResults res;
		int nRet;
		opt.Median      = true;
		opt.Interpolate = INTERPOLATE_WEIGHT_AVER_RIGHT;        
		nRet            = ocmath_quantiles(vDiff, dataSize - 1, &opt, &res);
		double median   = res.Median;

		// calculate median absolute deviation
		mad = 0;
		for(int rowInt = 0; rowInt < dataSize - 1; rowInt++)
		{
			//mad = mad + 1.4826*median*abs(vDiff[rowInt]-median);
			mad = mad + abs(vDiff[rowInt]-median)/(dataSize - 1);
		}

		// calculate z-values
		vZ = (vDiff - median) / mad;

		// despike
		for(int n = 1; n < dataSize; n++)
		{
			if(abs(vZ[n - 1]) > threshold)
			{
				// calculate mean of "non-spikes"
				int seqCounter = 0;
				double seqSum = 0;
				for(int m = n - width; m <= n + width; m++)
				{
					// respect boundaries
					if(m > 0 && m < dataSize)
					{
						// only use "non-spikes"
						if(abs(vZ[m-1]) <= threshold)
						{
							seqCounter++;
							seqSum += vData[m];
						}
					}
				}

				// insert mean value of sequence
				if(seqCounter > 0)
				{
					vData[n] = round(seqSum / seqCounter, 0);
				}
			}
		}

		// set comment
		dataCol.SetComments("Removed spikes (" + ftoa(threshold) + ", " + ftoa(width) + ").");
	}
}

/**
 * Correct measurement data for a recorded instrument response function (by dividing).
 *
 * @param Worksheet tgtWks the worksheet holding the data
 * @param Worksheet refWks the worksheet holding the setup correction data
 *
 * @toDo Use User Label?
 */
void CORRECT_setup(Worksheet tgtWks, Worksheet refWks)
{
	// user information
	out_str("Setup Correction started ...");

	// set reference data range (assume only one dataset)
	XYRange ref;
	ref.Add(refWks, 0, "X");
	ref.Add(refWks, 1, "Y");

	// set helpers
	Column xCol, xTempCol;
	int xInt = -1, yInt = -1, xTempInt = tgtWks.AddCol();
	tgtWks.Columns(xTempInt).SetType(OKDATAOBJ_DESIGNATION_X);

	// get column designations
	string colTypes = tgtWks.GetColDesignations();

	// loop through all columns
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++)
	{
		// set column indexes
		if(colTypes.GetAt(colInt) == 'Y')
		{
			yInt = colInt;
		}
		if(colTypes.GetAt(colInt) == 'X')
		{
			if(xInt >= 0)
			{
				// get X-data
				vector<double> xTempDataV = tgtWks.Columns(xTempInt).GetDataObject();

				// paste converted X-data
				vectorbase& xColObj = tgtWks.Columns(xInt).GetDataObject();
				xColObj = xTempDataV;
			}

			// set new X-data column index
			xInt = colInt;
			continue;
		}

		// get source data range
		XYRange data;
		data.Add(tgtWks, xInt, "X");
		data.Add(tgtWks, yInt, "Y");

		// set target data range
		XYRange dataTgt;
		dataTgt.Add(tgtWks, xTempInt, "X");
		dataTgt.Add(tgtWks, yInt, "Y");

		// prepare X-Function
		XFBase xf("mathtool");
		if(!xf.SetArg("iy1", data) || !xf.SetArg("operator", 2) || !xf.SetArg("operand", 1) || !xf.SetArg("iy2", ref) || !xf.SetArg("oy", dataTgt) || !xf.SetArg("common", 1) || !xf.SetArg("rescale", 0))
		{
			out_str("Failed to set parameters!");
			continue;
		}

		// run X-Function
		if(!xf.Evaluate())
		{
			out_str("Failed to evaluate the mathtool X-Function.");
			continue;
		}

		// set comment
		tgtWks.Columns(yInt).SetComments("Corrected with '" + refWks.GetName() + "!0,1'.");
	}

	// remove temporary x-column
	tgtWks.DeleteCol(xTempInt);

	// user information
	out_str("Setup Correction finished.\n");
}

/**
 * Correct measurement data for neutral density filter attenuation (from transmittance).
 *
 * @param Worksheet tgtWks        the worksheet holding the data
 * @param Worksheet refWks        the worksheet holding the filter transmittance data
 * @param string    userLabelName the user label to connect data and reference dataset
 */
void CORRECT_filters(Worksheet tgtWks, Worksheet refWks, string userLabelName)
{
	// user information
	out_str("Filter Correction started ...");

	// get user labels of reference and source
	vector<string> refLabelData, tgtLabelData;
	refLabelData = WORKSHEET_getUserLabelData(refWks, userLabelName);
	tgtLabelData = WORKSHEET_getUserLabelData(tgtWks, userLabelName);

	// abort if no label data found
	if(refLabelData.GetSize() == 0 || tgtLabelData.GetSize() == 0)
	{
		out_str("No label data found. Aborting ...");
		return;
	}

	// set helpers
	Column xCol, xTempCol;
	int xInt = -1, yInt = -1, refXInt = -1, refYInt = -1, xTempInt = tgtWks.AddCol();
	tgtWks.Columns(xTempInt).SetType(OKDATAOBJ_DESIGNATION_X);

	// get column designations
	string colTypes    = tgtWks.GetColDesignations();
	string refColTypes = refWks.GetColDesignations();

	// loop through all columns
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++)
	{
		// set column indexes
		if(colTypes.GetAt(colInt) == 'Y')
		{
			yInt = colInt;
		}
		if(colTypes.GetAt(colInt) == 'X')
		{
			if(xInt >= 0)
			{
				// get X-data
				vector<double> xTempDataV = tgtWks.Columns(xTempInt).GetDataObject();

				// paste converted X-data
				vectorbase& xColObj = tgtWks.Columns(xInt).GetDataObject();
				xColObj = xTempDataV;
			}

			// set new X-data column index
			xInt = colInt;
			continue;
		}

		// get source data range
		XYRange data;
		data.Add(tgtWks, xInt, "X");
		data.Add(tgtWks, yInt, "Y");

		// set target data range
		XYRange dataTgt;
		dataTgt.Add(tgtWks, xTempInt, "X");
		dataTgt.Add(tgtWks, yInt, "Y");

		// get reference data range by user label
		refYInt = refLabelData.Find(tgtLabelData[yInt]);

		// abort if no reference data found
		if(refYInt < 0 && tgtLabelData[yInt] == "")
		{
			// set comment
			tgtWks.Columns(yInt).SetComments("No reference data found!");
			continue;
		}

		// find reference X-axis
		refXInt = refColTypes.Find('X');
		while(refColTypes.Find('X', refXInt + 1) < refYInt && refColTypes.Find('X', refXInt + 1) >= 0)
		{
			refXInt = refColTypes.Find('X', refXInt + 1);
		}

		// get reference data range
		XYRange ref;
		ref.Add(refWks, refXInt, "X");
		ref.Add(refWks, refYInt, "Y");

		// prepare X-Function
		XFBase xf("mathtool");
		if(!xf.SetArg("iy1", data) || !xf.SetArg("operator", 2) || !xf.SetArg("operand", 1) || !xf.SetArg("iy2", ref) || !xf.SetArg("oy", dataTgt) || !xf.SetArg("common", 1))
		{
			out_str("Failed to set parameters!");
			continue;
		}

		// run X-Function
		if(!xf.Evaluate())
		{
			out_str("Failed to evaluate the mathtool X-Function.");
			continue;
		}

		// set comment
		tgtWks.Columns(yInt).SetComments("Corrected with '" + refWks.GetName() + "!" + refXInt + "," + refYInt + "'.");
	}

	// remove temporary X-Column
	tgtWks.DeleteCol(xTempInt);

	// user information
	out_str("Filter Correction finished.\n");
}

/**
 * Divide measurement data by corresponding integration times.
 *
 * @param Worksheet tgtWks        the worksheet holding the data
 * @param string    userLabelName the user label holding integration times
 */
void CORRECT_integrationTime(Worksheet tgtWks, string userLabelName)
{
	// user information
	out_str("Integration Time Correction started ...");

	// get user labels of source
	vector<string> tgtLabelData;
	tgtLabelData = WORKSHEET_getUserLabelData(tgtWks, userLabelName);

	// abort if no label data found
	if(tgtLabelData.GetSize() == 0)
	{
		out_str("No label data found. Aborting ...");
		return;
	}

	// get column designations
	string colTypes = tgtWks.GetColDesignations();

	// loop through all columns
	int xInt = 0, yInt = 0;
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++)
	{
		// set column indexes
		if(colTypes.GetAt(colInt) == 'Y')
		{
			yInt = colInt;
		} 
		else
		{
			if(colTypes.GetAt(colInt) == 'X')
			{
				xInt = colInt;
			}
			continue;
		}

		// get source data range
		XYRange data;
		data.Add(tgtWks, xInt, "X");
		data.Add(tgtWks, yInt, "Y");

		// skip empty labels
		if(tgtLabelData[yInt] == "")
		{
			tgtWks.Columns(yInt).SetComments("No reference data found!");
			continue;
		}

		// get source integration time
		double intTime = atof(tgtLabelData[yInt]);

		// prepare X-Function
		XFBase xf("mathtool");
		if(!xf.SetArg("iy1", data) || !xf.SetArg("operator", 2) || !xf.SetArg("operand", 0) || !xf.SetArg("const", intTime) || !xf.SetArg("oy", data))
		{
			out_str("Failed to set parameters!\n");
			continue;
		}

		// run X-Function
		if(!xf.Evaluate())
		{
			out_str("Failed to evaluate the mathtool X-Function.\n");
			continue;
		}

		// set units
		tgtWks.Columns(yInt).SetUnits("cts./s");

		// set comment
		tgtWks.Columns(yInt).SetComments("Divided by '" +  tgtLabelData[yInt] + "'.");
	}

	// user information
	out_str("Integration Time Correction finished.\n");
}

/**
 * Perfom a Jacobian energy transformation on spectroscopic data.
 *
 * @param Worksheet tgtWks the worksheet holding the data
 */
void CORRECT_transform(Worksheet tgtWks)
{
	// user information
	out_str("Jacobian Transformation started ...");

	// get column designations
	string colTypes = tgtWks.GetColDesignations();

	// loop through all columns
	int xStore = -1;
	Column xCol, yCol;
	vector<double> xDataV, yDataV;
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++)
	{
		// set column indexes
		if(colTypes.GetAt(colInt) == 'Y')
		{
			// convert Y-data
			yCol = tgtWks.Columns(colInt);
			yDataV = yCol.GetDataObject();
			yDataV = yDataV * xDataV * xDataV / 1239.841857;

			// paste new Y-data
			vectorbase& yDataO = yCol.GetDataObject();
			yDataO = yDataV;

			// set comments
			tgtWks.Columns(colInt).SetComments("Transformed.");
		}
		if(colTypes.GetAt(colInt) == 'X')
		{
			// get X-data
			xStore = colInt;
			xCol = tgtWks.Columns(colInt);
			xDataV = xCol.GetDataObject();

			// paste converted X-data
			vectorbase& xDataO = xCol.GetDataObject();
			xDataO = 1239.841857 / xDataV;

			// set labels
			tgtWks.Columns(colInt).SetLongName("Energy");
			tgtWks.Columns(colInt).SetUnits("eV");
		}
	}

	// user information
	out_str("Jacobian Transformation finished.\n");
}

/**
 * Normalise data in a worksheet (divide by maximum).
 *
 * @param Worksheet tgtWks the worksheet holding the data
 */
void CORRECT_normalise(Worksheet tgtWks)
{
	// user information
	out_str("Normalisation started ...");

	// get column designations
	string colTypes = tgtWks.GetColDesignations();

	// loop through all columns
	int xInt = 0, yInt = 0;
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++)
	{
		// set column indexes
		if(colTypes.GetAt(colInt) == 'Y')
		{
				yInt = colInt;
		} 
		else
		{
			if(colTypes.GetAt(colInt) == 'X')
			{
				xInt = colInt;
			}
			continue;
		}

		// get source data range
		XYRange data;
		data.Add(tgtWks, xInt, "X");
		data.Add(tgtWks, yInt, "Y");

		// prepare X-Function
		XFBase xf("normalize ");
		if(!xf.SetArg("iy", data) || !xf.SetArg("method", 3) || !xf.SetArg("oy", data))
		{
			out_str("Failed to set parameters!");
			continue;
		}

		// run X-Function
		if(!xf.Evaluate())
		{
			out_str("Failed to evaluate the normalize X-Function.");
			continue;
		}  

		// set labels
		tgtWks.Columns(yInt).SetLongName("Normalised Intensity");
		tgtWks.Columns(yInt).SetUnits("");
		tgtWks.Columns(yInt).SetComments("Normalised.");
	}

	// user information
	out_str("Normalisation finished.\n");
}

void CORRECT_interpolate(WorksheetPage wb, Worksheet wks, int newXInt)
{
	// user information
	printf("Starting data interpolation...");

	// generate x-range string
	string str_xRange = wks.GetName() + "!" + newXInt;

	// store new x-data for replication
	vector<double> newXData;
	newXData = wks.Columns(newXInt).GetDataObject();

	// duplicate data sheet
	Worksheet tgtWks;
	tgtWks = ORIGIN_createWks(wb, "Interpolated", true);
	wks_copy(tgtWks, wks, CREATE_VISIBLE_SAME, DCTRL_COPY_GRID | DCTRL_COPY_DATA);

	// store sheet name
	string tgtSheet = tgtWks.GetName();

	// loop through all columns
	int numCols = tgtWks.GetNumCols();
	for(int colInt = 0; colInt < numCols ; colInt++)
	{
		// skip x/z-data, new x-axis and empty columns
		if(tgtWks.Columns(colInt).GetType() != OKDATAOBJ_DESIGNATION_Y || colInt == newXInt || tgtWks.Columns(colInt).GetUpperBound() == -1)
		{
			continue;
		}

		// perform interpolation with LabTalk
		string src_str         = tgtSheet + "!" + (colInt + 1);
		string str_interpolate = "interp1 -r 0 ix:=" + str_xRange + " iy:=" + src_str + " method:=linear option:=1 ox:=" + src_str;
		LT_execute(str_interpolate);

		// add comment to interpolated data
		tgtWks.Columns(colInt).SetComments("Interpolated to new X-axis.");
	}

	// replace old x-data with new axis
	for(colInt = 0; colInt < numCols ; colInt++)
	{
		// skip y/z-data and source of new x-data
		if(tgtWks.Columns(colInt).GetType() != OKDATAOBJ_DESIGNATION_X || colInt == newXInt)
		{
			continue;
		}

		// get target data object
		vectorbase& xColObj = tgtWks.Columns(colInt).GetDataObject();

		// paste new x-data
		xColObj = newXData;
	}

	// remove old x-data column
	tgtWks.DeleteCol(newXInt);

	// user information
	printf("Data interpolation finished.");
}

#endif