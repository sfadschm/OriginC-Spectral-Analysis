/*------------------------------------------------------------------------------*
 * File Name:	Correct.c 														*
 * Creation: 	Alexander Schmitz												*
 * Purpose: 	Provides user functions for data correction.					*
 * Copyright(c) 2020, Alexander Schmitz         								*
 * All Rights Reserved															*
 * 																				*
 * Last Modified:	12.11.2020													*
 * Tasks:	CORRECT_setup - Use User Label?										*
 *			Merge functions into one handler (avoid redundance)?				*
 *------------------------------------------------------------------------------*/
#ifndef _CORRECT_ // include once
#define _CORRECT_

#include <Origin.h>
#include <XFBase.h>

/**
 * Method CORRECT_backgroundRef
 * Subtracts reference background data from a datasheet.
 * @param Worksheet tgtWks the worksheet holding the data
 * @param Worksheet refWks the worksheet holding the background data
 * @param string userLabelName the user label to connect data and reference dataset
 **/
void CORRECT_backgroundRef(Worksheet tgtWks, Worksheet refWks, string userLabelName){
	// user information
	out_str("Background Subtraction started ...");

	// get user labels of reference and source
	vector<string> refLabelData, tgtLabelData;
	refLabelData = WORKSHEET_getUserLabelData(refWks, userLabelName);
	tgtLabelData = WORKSHEET_getUserLabelData(tgtWks, userLabelName);

	// abort if no label data found
	if(refLabelData.GetSize() == 0 || tgtLabelData.GetSize() == 0){
		out_str("No label data found. Aborting ...");
		return;
	}

	// set helpers
	Column xCol, xTempCol;
	int xInt = -1, yInt = -1, refXInt = -1, refYInt = -1, xTempInt = tgtWks.AddCol();
	tgtWks.Columns(xTempInt).SetType(OKDATAOBJ_DESIGNATION_X);

	// get column designations
    string colTypes = tgtWks.GetColDesignations();
	string refColTypes = refWks.GetColDesignations();

	// loop through all columns
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++){
		// set column indexes
		if(colTypes.GetAt(colInt) == 'Y'){
				yInt = colInt;
		} else {
			if(colTypes.GetAt(colInt) == 'X'){
				if(xInt >= 0){
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
		if(refYInt >= 0 && tgtLabelData[yInt] != ""){ // reference found
			// find reference X-axis
			refXInt = refColTypes.Find('X');
			while(refColTypes.Find('X', refXInt + 1) < refYInt && refColTypes.Find('X', refXInt + 1) >= 0){
				refXInt = refColTypes.Find('X', refXInt + 1);
			}

			// get reference data range
			XYRange ref;
			ref.Add(refWks, refXInt, "X");
			ref.Add(refWks, refYInt, "Y");

			// prepare X-Function
			XFBase xf("subtract_ref");
			if(!xf.SetArg("iy1", data) | !xf.SetArg("iy2", ref) | !xf.SetArg("common", 1) | !xf.SetArg("oy", dataTgt)){
				out_str("Failed to set parameters!");
				continue;
			}

			// run X-Function
			if(!xf.Evaluate()){
				out_str("Failed to evaluate the subtract_ref X-Function.");
				continue;
			}

			// set comment
			tgtWks.Columns(yInt).SetComments("Subtracted '" + refWks.GetName() + "!" + refXInt + "," + refYInt + "'.");

 		} else { // no reference found
			// set comment
			tgtWks.Columns(yInt).SetComments("No reference data found!");
		}
	}

	// remove temporary X-Column
	tgtWks.DeleteCol(xTempInt);

	// user information
	out_str("Background Subtraction finished.\n");
}


/**
 * Method CORRECT_backgroundAve
 * Subtracts an averaged background from each column of a datasheet.
 * @param Worksheet tgtWks the worksheet holding the data
 * @param Worksheet refWks the worksheet holding the background data
 * @param string userLabelName the user label to connect data and reference dataset
 **/
void CORRECT_backgroundAve(Worksheet tgtWks, double xStart, double xStop){
	// user information
	out_str("Background Subtraction started ...");

	// get column designations
    string colTypes = tgtWks.GetColDesignations();

	// loop through all columns
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++){
		// only process y-data
		if(colTypes.GetAt(colInt) != 'Y'){
			continue;
		}

		// get curve
		Curve dataCurve(tgtWks, colInt);

		// get curve boundaries
		double xMin,xMax,yMin,yMax;
		Curve_MinMax(&dataCurve, &xMin, &xMax, true, &yMin, &yMax);

		// set user boundaries
		if(xStart != 0)
			xMin = xStart;
		if(xStop != 0)
			xMax = xStop;

		// set rectangular boundaries
		fpoint fptTL(xMin, yMax);
		fpoint fptBR(xMax, yMin);

		// extract data from curve
		vector<double> vX, vY;
		vector<int> vIndex;
		dataCurve.GetRectPoints(fptTL, fptBR, vX, vY, vIndex);
		double test;

		// calculate average
		double sum, background;
		vY.Sum(sum);
		background = round(sum / vY.GetSize(), 0);
		
		// subtract background
		dataCurve = dataCurve - background;
		
		// set comment
		tgtWks.Columns(colInt).SetComments("Subtracted '" + background + "'.");		
	}

	// user information
	out_str("Background Subtraction finished.\n");
}

/**
 * Method CORRECT_masked
 * Set all masked data to undefined and remove mask.
 * @param Worksheet wks the worksheet
 **/
void CORRECT_masked(Worksheet wks){
    // user information
    out_str("Cleaning masked data ...");

	// get column designations
    string colTypes = wks.GetColDesignations();
    
	// loop through all columns
    DataRange dr;
    Column dataCol;
	int xInt = 0, yInt = 0, dataSize;
 	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++){
		// only manipulate Y-Columns
		if(colTypes.GetAt(colInt) == 'Y'){

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
			for(int n = 1; n < dataSize; n++){
				// check if cell does not have a value
				if(is_missing_value(vData[n])){
					// Initialize calculation helpers
					int seqCounter = 0;
					double seqSum = 0;
					for(int m = n - 5; m <= n + 5; m++){
						// respect boundaries
						if(m > 0 && m < dataSize){
							// only use "non-spikes"
							if(!is_missing_value(vData[m])){
								seqCounter++;
								seqSum += vData[m];
							}
						}
					}
					// Write data to cell
					if(seqCounter > 0){
						vData[n] =  round(seqSum / seqCounter, 0);
					} else {out_str("HI");
						vData[n] = 0;
					}
				}
			}
	
			wks.Columns(colInt).SetComments("Cleaned.");
		} else {
			continue;
		}
	}
	   
   	// user information
	out_str("Masked data have been cleaned.\n");
}


/**
 * Method CORRECT_spikes
 * Removes spikes from all y-Datasets in a worksheet (Whitaker Hayes Algorithm).
 * @param Worksheet tgtWks the worksheet holding the data
 * @param double threshold the threshold for the "z-Value"
 * @param int width the step width to caluclate mean values from
 **/
void CORRECT_spikes(Worksheet wks, double threshold, int width){
	// get column designations
    string colTypes = wks.GetColDesignations();

	// loop through all columns
	int dataSize;
    Column dataCol;
	double median, mad;
    vector vDiff, vZ, vSpikes;
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++){
		// only manipulate Y-Columns
		if(colTypes.GetAt(colInt) == 'Y'){
			// get spectrum
			dataCol = wks.Columns(colInt);
			vector &vData = dataCol.GetDataObject();
			dataSize = vData.GetSize();

			// calculate diff series
			vector vDiff(dataSize - 1);
			for(int i = 1; i < dataSize; i++){
				vDiff[i-1] = vData[i] - vData[i - 1];
			}

			// caluculate median
			QuantileOptions opt;
			QuantileResults res;
			int nRet;
			opt.Median = true;
			opt.Interpolate = INTERPOLATE_WEIGHT_AVER_RIGHT;        
			nRet = ocmath_quantiles(vDiff, dataSize - 1, &opt, &res);
			double median = res.Median;

			// calculate median absolute deviation
			mad = 0;
			for(int rowInt = 0; rowInt < dataSize - 1; rowInt++){
				//mad = mad + 1.4826*median*abs(vDiff[rowInt]-median);
				mad = mad + abs(vDiff[rowInt]-median)/(dataSize - 1);
			}

			// calculate z-values
			vZ = (vDiff - median) / mad;

			// despike
			for(int n = 1; n < dataSize; n++){
				if(abs(vZ[n - 1]) > threshold){
					// calculate mean of "non-spikes"
					int seqCounter = 0;
					double seqSum = 0;
					for(int m = n - width; m <= n + width; m++){
						// respect boundaries
						if(m > 0 && m < dataSize){
							// only use "non-spikes"
							if(abs(vZ[m-1]) <= threshold){
								seqCounter++;
								seqSum += vData[m];
							}
						}
					}
					// insert mean value of sequence
					if(seqCounter > 0)
						vData[n] = round(seqSum / seqCounter, 0);
				}
			}
		} else {
			continue;
		}
	}
}

/**
 * Method CORRECT_setup
 * Corrects measurement data for a recorded instrument response function (by dividing).
 * @param Worksheet tgtWks the worksheet holding the data
 * @param Worksheet refWks the worksheet holding the setup correction data
 **/
void CORRECT_setup(Worksheet tgtWks, Worksheet refWks){
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
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++){
		// set column indexes
		if(colTypes.GetAt(colInt) == 'Y'){
				yInt = colInt;
		} else {
			if(colTypes.GetAt(colInt) == 'X'){
				if(xInt >= 0){
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
		if(!xf.SetArg("iy1", data) || !xf.SetArg("operator", 2) || !xf.SetArg("operand", 1) || !xf.SetArg("iy2", ref) || !xf.SetArg("oy", dataTgt) || !xf.SetArg("common", 1) || !xf.SetArg("rescale", 0)){
			out_str("Failed to set parameters!");
			continue;
		}

		// run X-Function
		if(!xf.Evaluate()){
			out_str("Failed to evaluate the mathtool X-Function.");
			continue;
		}

		// set comment
		tgtWks.Columns(yInt).SetComments("Corrected with '" + refWks.GetName() + "!0,1'.");
	}

	// remove temporary X-Column
	tgtWks.DeleteCol(xTempInt);

	// user information
	out_str("Setup Correction finished.\n");
}

/**
 * Method CORRECT_filters
 * Corrects measurement data for neutral density filter attenuation (from transmittance).
 * @param Worksheet tgtWks the worksheet holding the data
 * @param Worksheet refWks the worksheet holding the filter transmittance data
 * @param string userLabelName the user label to connect data and reference dataset
 **/
void CORRECT_filters(Worksheet tgtWks, Worksheet refWks, string userLabelName){
	// user information
	out_str("Filter Correction started ...");

	// get user labels of reference and source
	vector<string> refLabelData, tgtLabelData;
	refLabelData = WORKSHEET_getUserLabelData(refWks, userLabelName);
	tgtLabelData = WORKSHEET_getUserLabelData(tgtWks, userLabelName);

	// abort if no label data found
	if(refLabelData.GetSize() == 0 || tgtLabelData.GetSize() == 0){
		out_str("No label data found. Aborting ...");
		return;
	}

	// set helpers
	Column xCol, xTempCol;
	int xInt = -1, yInt = -1, refXInt = -1, refYInt = -1, xTempInt = tgtWks.AddCol();
	tgtWks.Columns(xTempInt).SetType(OKDATAOBJ_DESIGNATION_X);

	// get column designations
    string colTypes = tgtWks.GetColDesignations();
	string refColTypes = refWks.GetColDesignations();

	// loop through all columns
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++){
		// set column indexes
		if(colTypes.GetAt(colInt) == 'Y'){
				yInt = colInt;
		} else {
			if(colTypes.GetAt(colInt) == 'X'){
				if(xInt >= 0){
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
		if(refYInt >= 0 && tgtLabelData[yInt] != ""){ // reference found
			// find reference X-axis
			refXInt = refColTypes.Find('X');
			while(refColTypes.Find('X', refXInt + 1) < refYInt && refColTypes.Find('X', refXInt + 1) >= 0){
				refXInt = refColTypes.Find('X', refXInt + 1);
			}

			// get reference data range
			XYRange ref;
			ref.Add(refWks, refXInt, "X");
			ref.Add(refWks, refYInt, "Y");

			// prepare X-Function
			XFBase xf("mathtool");
			if(!xf.SetArg("iy1", data) || !xf.SetArg("operator", 2) || !xf.SetArg("operand", 1) || !xf.SetArg("iy2", ref) || !xf.SetArg("oy", dataTgt) || !xf.SetArg("common", 1)){
				out_str("Failed to set parameters!");
				continue;
			}

			// run X-Function
			if(!xf.Evaluate()){
				out_str("Failed to evaluate the mathtool X-Function.");
				continue;
			}

			// set comment
			tgtWks.Columns(yInt).SetComments("Corrected with '" + refWks.GetName() + "!" + refXInt + "," + refYInt + "'.");
 		} else { // no reference found
			// set comment
			tgtWks.Columns(yInt).SetComments("No reference data found!");
		}
	}

	// remove temporary X-Column
	tgtWks.DeleteCol(xTempInt);

	// user information
	out_str("Filter Correction finished.\n");
}

/**
 * Method CORRECT_integrationTime
 * Divides measurement data by corresponding integration times.
 * @param Worksheet tgtWks the worksheet holding the data
 * @param string userLabelName the user label holding integration times
 **/
void CORRECT_integrationTime(Worksheet tgtWks, string userLabelName){
	// user information
	out_str("Integration Time Correction started ...");

	// get user labels of source
	vector<string> tgtLabelData;
	tgtLabelData = WORKSHEET_getUserLabelData(tgtWks, userLabelName);

	// abort if no label data found
	if(tgtLabelData.GetSize() == 0){
		out_str("No label data found. Aborting ...");
		return;
	}

	// get column designations
    string colTypes = tgtWks.GetColDesignations();

	// loop through all columns
	int xInt = 0, yInt = 0;
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++){
		// set column indexes
		if(colTypes.GetAt(colInt) == 'Y'){
				yInt = colInt;
		} else {
			if(colTypes.GetAt(colInt) == 'X')
				xInt = colInt;
			continue;
		}

		// get source data range
		XYRange data;
		data.Add(tgtWks, xInt, "X");
		data.Add(tgtWks, yInt, "Y");

		// skip empty labels
		if(tgtLabelData[yInt] != ""){
			// get source integration time
			double intTime = atof(tgtLabelData[yInt]);

			// prepare X-Function
			XFBase xf("mathtool");
			if(!xf.SetArg("iy1", data) || !xf.SetArg("operator", 2) || !xf.SetArg("operand", 0) || !xf.SetArg("const", intTime) || !xf.SetArg("oy", data)){
				out_str("Failed to set parameters!\n");
				continue;
			}

			// run X-Function
			if(!xf.Evaluate()){
				out_str("Failed to evaluate the mathtool X-Function.\n");
				continue;
			}  

			// set units
			tgtWks.Columns(yInt).SetUnits("cts./s");

			// set comment
			tgtWks.Columns(yInt).SetComments("Divided by '" +  tgtLabelData[yInt] + "'.");
		} else { // reference label empty
			// set comment
			tgtWks.Columns(yInt).SetComments("No reference data found!");
		}
	}

	// user information
	out_str("Integration Time Correction finished.\n");
}

/**
 * Method CORRECT_transform
 * Perfoms a Jacobian energy transformation on spectroscopic data.
 * @param Worksheet tgtWks the worksheet holding the data
 **/
void CORRECT_transform(Worksheet tgtWks){
	// user information
	out_str("Jacobian Transformation started ...");

	// get column designations
    string colTypes = tgtWks.GetColDesignations();

	// loop through all columns
    int xStore = -1;
	Column xCol, yCol;
	vector<double> xDataV, yDataV;
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++){
		// set column indexes
		if(colTypes.GetAt(colInt) == 'Y'){
			// convert Y-data
			yCol = tgtWks.Columns(colInt);
			yDataV = yCol.GetDataObject();
			yDataV = yDataV * xDataV * xDataV / 1239.841857;
			
			// paste new Y-data
			vectorbase& yDataO = yCol.GetDataObject();
			yDataO = yDataV;

			// set comments
			tgtWks.Columns(colInt).SetComments("Transformed.");
		} else {
			if(colTypes.GetAt(colInt) == 'X'){
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
	}

	// user information
	out_str("Jacobian Transformation finished.\n");
}

/**
 * Method CORRECT_normalise
 * Normalises data in a worksheet (divide by maximum).
 * @param Worksheet tgtWks the worksheet holding the data
 **/
void CORRECT_normalise(Worksheet tgtWks){
	// user information
	out_str("Normalisation started ...");

	// get column designations
    string colTypes = tgtWks.GetColDesignations();

	// loop through all columns
	int xInt = 0, yInt = 0;
	for(int colInt = 0; colInt < colTypes.GetLength(); colInt++){
		// set column indexes
		if(colTypes.GetAt(colInt) == 'Y'){
				yInt = colInt;
		} else {
			if(colTypes.GetAt(colInt) == 'X')
				xInt = colInt;
			continue;
		}

		// get source data range
		XYRange data;
		data.Add(tgtWks, xInt, "X");
		data.Add(tgtWks, yInt, "Y");
    
		// prepare X-Function
		XFBase xf("normalize ");
		if(!xf.SetArg("iy", data) || !xf.SetArg("method", 3) || !xf.SetArg("oy", data)){
			out_str("Failed to set parameters!");
			continue;
		}

		// run X-Function
		if(!xf.Evaluate()){
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

#endif