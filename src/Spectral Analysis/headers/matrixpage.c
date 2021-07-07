/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#include <Origin.h>

int MATRIXPAGE_addMatrixLayer(MatrixPage mp, string mlName = "") {
    int mlInt      = mp.AddLayer();
    MatrixLayer ml = mp.Layers(mlInt);

    // rename layer if applicable
    if (mlName != "") {
        ml.SetName(mlName);
    }

    return mlInt;
}