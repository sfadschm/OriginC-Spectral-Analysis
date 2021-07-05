/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _GRAPH_  // include once
#define _GRAPH_

/**
 * This file provides basic methods for manipulating graph pages.
 */
#include <Origin.h>

/**
 * Align text fields in graph windows.
 **/
void GRAPH_alignText(string strobj, uint ijustify) {
    GraphLayer gl = Project.ActiveLayer();

    if (gl.IsValid()) {
        GraphObject go = gl.GraphObjects(strobj);

        if (go.IsValid()) {
            Tree tr;
            bool brelative = true;
            tr             = go.GetFormat(FPB_ALL, FOB_ALL, true, brelative);
            tr.root.alignment.horizontal.nVal = ijustify;
            go.ApplyFormat(tr, true, brelative);
        } else {
            printf("%s is not a valid object in current layer.\n", strobj);
        }
    } else {
        printf("active layer is not a valid graph.\n");
    }
}

#endif