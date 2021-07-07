/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _CURVE_  // include once
#define _CURVE_

/**
 * This file provides basic helper methods for handling curve data.
 */

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
 */
void CURVE_truncate(Curve rawCurve, Curve &dataCurve, Worksheet tmpWks,
                    double startX, double stopX);
#endif