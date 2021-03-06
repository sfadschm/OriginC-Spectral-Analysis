/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#include <Origin.h>
#include <XFBase.h>

#include "lang.h"
#include "user.h"

void WOKRSHEET_addUserParameter(Worksheet wks, string paramName,
                                int offset = 0) {
    // get current label names
    Grid gg;
    gg.Attach(wks);
    vector<int> vnTypes;
    gg.GetShowLabels(vnTypes, TRUE);

    // read existing user labels
    vector<string> userLabels;
    gg.GetUserDefinedLabelNames(userLabels);

    // add new parameter name
    userLabels.Add(paramName);

    // set and show parameters
    vector<uint> vecIndex;
    if (vnTypes.Find(vecIndex, RCLT_UDL + offset) == 0) {
        gg.SetUserDefinedLabelNames(userLabels);
        vnTypes.Add(RCLT_UDL + offset);
        gg.SetShowLabels(vnTypes);
    }

    // refresh to show labels
    wks.GetPage().Refresh();
}

bool WORKSHEET_addSparklines(Worksheet &wks) {
    // user information
    printf(SPARKS_START);

    // create an instance of the X-Function
    XFBase xf("sparklines");

    // abort if X-Function not loaded
    if (! xf) {
        return false;
    }

    // set parameters
    if (! xf.SetArg("iw", wks) || ! xf.SetArg("sel", 0) ||
        ! xf.SetArg("c1", 1) || ! xf.SetArg("c2", wks.GetNumCols())) {
        return false;
    }

    // execute the X-Function
    if (! xf.Evaluate()) {
        return false;
    }

    // user information
    printf(SPARKS_STOP);

    return true;
}

double WORKSHEET_getYMin(Worksheet wks) {
    // setup variables
    double min = 1E37, colMin, colMax;

    // loop through columns
    for (int colInt = 0; colInt < wks.GetNumCols(); colInt++) {
        // get source column
        Column colObj = wks.Columns(colInt);

        // skip x/z-data
        if (colObj.GetType() != OKDATAOBJ_DESIGNATION_Y) {
            continue;
        }

        // get column data
        vectorbase &colData = colObj.GetDataObject();

        // search column minimum
        colData.GetMinMax(colMin, colMax);

        // store minimum value
        if (colMin < min) {
            min = colMin;
        }
    }

    return min;
}

vector<string> WORKSHEET_getUserLabelData(Worksheet wks, string labelName) {
    // get  label names
    Grid gg;
    gg.Attach(wks);
    vector<string> labelNames;
    gg.GetUserDefinedLabelNames(labelNames);

    // find user label name in reference
    vector<string> labelData;
    int labelIndex = labelNames.Find(labelName);

    // abort if user label does not exist
    if (labelIndex < 0) {
        printf(CORRECT_NO_LABELS, labelName, wks.GetName());
        return labelData;
    }

    // get reference label data
    gg.GetLabelsByType(labelData, RCLT_UDL + labelIndex);

    return labelData;
}

void WORKSHEET_setLowerBound(Worksheet wks, double lowerBound) {
    // get column designations
    string colTypes = wks.GetColDesignations();

    // loop through all columns
    int xInt = 0, yInt = 0;
    Column dataCol;
    for (int colInt = 0; colInt < colTypes.GetLength(); colInt++) {
        // skip x/z-data
        if (colTypes.GetAt(colInt) != OKDATAOBJ_DESIGNATION_Y) {
            continue;
        }

        // replace lower bounds
        dataCol             = wks.Columns(colInt);
        vectorbase &dataVec = dataCol.GetDataObject();
        for (int rowInt = 0; rowInt < dataVec.GetSize(); rowInt++) {
            dataVec[rowInt] =
                (dataVec[rowInt] < lowerBound || dataVec[rowInt] == NULL)
                    ? lowerBound
                    : dataVec[rowInt];
        }
    }
}