/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#include <Origin.h>

void CURVE_truncate(Curve rawCurve, Curve &dataCurve, Worksheet tmpWks,
                    double startX, double stopX) {
    // remove missing curve values
    int nMissing;
    int nSrcOffset;
    Curve strippedCurve(rawCurve, nMissing, nSrcOffset,
                        CURVECOPY_SKIP_MISSING_INSIDE);

    // sort curve (for inverted x-axis)
    strippedCurve.Sort();

    // get curve boundaries
    double xMin, xMax, yMin, yMax;
    Curve_MinMax(&strippedCurve, &xMin, &xMax, true, &yMin, &yMax);

    // set user boundaries
    if (startX != 0) {
        xMin = startX;
    }
    if (stopX != 0) {
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