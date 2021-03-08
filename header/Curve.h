/*------------------------------------------------------------------------------*
 * File Name:	Curve.h                                                         *
 * Creation:	Alexander Schmitz                                               *
 * Purpose:		Provides basic curve handling methods.                          *
 * Copyright(c) 2021, Alexander Schmitz                                         *
 * All Rights Reserved                                                          *
 *                                                                              *
 * Last Modified::	08.03.2021                                                  *
 * Tasks:                                                                       *
 *------------------------------------------------------------------------------*/
#ifndef _CURVE_ // include once
#define _CURVE_

#include <Origin.h>

/**
 * Remove missing data from curve and set boundaries.
 *
 * @param Curve     rawCurve the unprepared curve
 * @param Curve&    data     the reference to the finalised curve object
 * @param Worksheet tmpWks   the temporary worksheet to attach the curves to
 * @param double    startX   the lower x boundary
 * @param double    stopX    the upper x boundary
 *
 * @return Curve truncCurve the truncated curve
 **/
void CURVE_truncate(Curve rawCurve, Curve& dataCurve, Worksheet tmpWks, double startX, double stopX)
{
	// remove missing curve values
	int nMissing;
	int nSrcOffset;
	Curve strippedCurve(rawCurve, nMissing, nSrcOffset, CURVECOPY_SKIP_MISSING_INSIDE);

	// sort curve (for inverted x-axis)
	strippedCurve.Sort();

	// get curve boundaries
	double xMin,xMax,yMin,yMax;
	Curve_MinMax(&strippedCurve, &xMin, &xMax, true, &yMin, &yMax);

	// set user boundaries
	if(startX != 0)
	{
		xMin = startX;
	}
	if(stopX != 0)
	{
		xMax = stopX;
	}

	// set rectangular boundaries
	fpoint fptTL(xMin, yMax);
	fpoint fptBR(xMax, yMin);

	// extract data from curve
	vector<double> vX, vY;
	vector<int> vIndex;
	strippedCurve.GetRectPoints(fptTL, fptBR, vX, vY, vIndex);

	// prepare temporary storage
	vectorbase &tmpX = tmpWks.Columns(0).GetDataObject();
	vectorbase &tmpY = tmpWks.Columns(1).GetDataObject();

	// copy truncated data into temporary sheet
	tmpX = vX;
	tmpY = vY;

	// attach new data to target curve
	dataCurve.Attach(tmpWks, 0, 1);
}

#endif