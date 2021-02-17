/*------------------------------------------------------------------------------*
 * File Name:	Graph.h  	 													*
 * Creation: 	Alexander Schmitz												*
 * Purpose: 	Provides basic methods for manipulating graph pages.			*
 * Copyright(c) 2019, Alexander Schmitz         								*
 * All Rights Reserved															*
 * 																				*
 * Last Modified::	14.11.2019											    	*
 * Tasks:           				    										*
 *------------------------------------------------------------------------------*/
#ifndef _GRAPH_ // include once
#define _GRAPH_

#include <Origin.h>

/**
 * Method GRAPH_alignText
 * Align text fields in graph windows.
 **/
void GRAPH_alignText(string strobj, uint ijustify){
  GraphLayer gl = Project.ActiveLayer();
  if(gl.IsValid()){
    GraphObject go = gl.GraphObjects(strobj);
    if(go.IsValid()){
      Tree tr;
      bool brelative = true;
      tr = go.GetFormat(FPB_ALL, FOB_ALL, true, brelative);
      tr.root.alignment.horizontal.nVal = ijustify;
      go.ApplyFormat(tr, true, brelative);
    } else {
      printf("%s is not a valid object in current layer.\n",strobj);
    }
  } else {
    printf("active layer is not a valid graph.\n");
  }
}

#endif