/*------------------------------------------------------------------------------*
 * File Name:	MatrixPage.h													*
 * Creation: 	Alexander Schmitz												*
 * Purpose: 	Provides methods for Origin MatrixPage manipulation.			*
 * Copyright(c) 2019, Alexander Schmitz         								*
 * All Rights Reserved															*
 * 																				*
 * Last Modified::	14.11.2019													*
 * Tasks:           				    										*
 *------------------------------------------------------------------------------*/
#ifndef _MATRIXPAGE_ // include once
#define _MATRIXPAGE_

#include <Origin.h>

/**
 *  Function MATRIXPAGE_addMatrixLayer
 *  Adds a new matrix layer to a matrix page.
 *  @param WorksheetPage wb the target workbook
 *	@param string wksName() the name of the new worksheet
 *	@return int wksInt the index of the new worksheet in the workbook
 **/ 
int MATRIXPAGE_addMatrixLayer(MatrixPage mp, string mlName = ""){
	int mlInt = mp.AddLayer();
	MatrixLayer ml = mp.Layers(mlInt);
	
	// rename layer if applicable
	if(mlName != ""){
		ml.SetName(mlName);
	}

	return mlInt;
}

#endif