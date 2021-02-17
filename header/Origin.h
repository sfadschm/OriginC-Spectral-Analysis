/*------------------------------------------------------------------------------*
 * File Name:	Origin.h	 													*
 * Creation: 	Alexander Schmitz												*
 * Purpose: 	Provides methods to manipulate Origin objects.  				*
 * Copyright(c) 2019, Alexander Schmitz         								*
 * All Rights Reserved															*
 * 																				*
 * Last Modified:	14.11.2019										    		*
 * Tasks:           				    										*
 *------------------------------------------------------------------------------*/
#ifndef _ORIGIN_ // include once
#define _ORIGIN_

#include <Origin.h>
#include "Lang.h"
#include "Misc.h"
#include "Workbook.h"
#include "MatrixPage.h"

/**
 * Function ORIGIN_createWb
 * Create a new workbook and rename it.
 * @param string wbName() the desired name of the new workbook
 * @return WorksheetPage wb the workbook object
 **/
WorksheetPage ORIGIN_createWb(string wbName = ""){
    WorksheetPage wb;
	string strippedName = MISC_stripName(wbName);
	
    if(!strippedName.IsEmpty()){
		// try using use existing workbook
		WorksheetPage wb2(strippedName);
		if(wb2){ // existing workbook
			wb = wb2;
		} else { // new workbook
			wb.Create();
		    wb.Layers(0).Delete();
			wb.SetName(strippedName);		
		}
    } else {
		wb.Create();
	    wb.Layers(0).Delete();
	}
        
    return wb;
}

/**
 * Function ORIGIN_createWks
 * Create a new worksheet in a given workbook. 
 * @param WorksheetPage wb the parent workbook of the worksheet to create
 * @param string wksName() the desired name of the worksheet to create
 * @param bool forceNew(false) defines if a new worksheet must be created even if name exists
 * @return Worksheet wks the worksheet object
 **/
Worksheet ORIGIN_createWks(WorksheetPage wb, string wksName = "", bool forceNew = false){
	int wksInt;
	string strippedName = MISC_stripName(wksName);
	
	if(!strippedName.IsEmpty()){
		// try using existing worksheet
		Worksheet wks = wb.Layers(strippedName);
		if(wks){ // existing worksheet
			if(forceNew){ // modify new worksheet name (avoid overwriting)
				int i = 2;
				string newName;
				while(wks){ // wait until no worksheet found
					newName = strippedName + "_" + ftoa(i++);
					wks = wb.Layers(newName);
				}

				// create new worksheet
				wksInt = WORKBOOK_addWorksheet(wb, newName);
			} else { // use existing worksheet
				wksInt = wks.GetIndex();
			}
		} else { // new worksheet
			wksInt = WORKBOOK_addWorksheet(wb, strippedName);
		}
	} else { // new worksheet
		wksInt = WORKBOOK_addWorksheet(wb);
	}
	
	return wb.Layers(wksInt);
}

/**
 * Function ORIGIN_getActiveWorksheets
 * Get the indexes of all worksheets in the active workbook.
 * @param int sourceType 0 if source is workbook, 1 if worksheet
 * @param WorksheetPage& wb the workbook reference to link the active workbook to
 * @param Worksheet& wks the worksheet reference to link the active worksheet to
 * @return vector<int> sourceWksInts the numeric indexes of the worksheet in the active workbook
 **/
vector<int> ORIGIN_getActiveWorksheets(int sourceType, WorksheetPage& wb, Worksheet& wks){
	vector<int> sourceWksInts;

	// get active layer/page
	wks = Project.ActiveLayer();
	if(!wks){ // cancel if no worksheet
		printf(ANALYSIS_NO_WKS);
		return sourceWksInts;
	}
	wb = wks.GetPage();

	// get source layers
	if(sourceType == 0){ // workbook
		foreach(Layer tempWks in wb.Layers){
			sourceWksInts.Add(tempWks.GetIndex());
		}
	} else { // worksheet
		sourceWksInts.Add(wks.GetIndex());
	}

	return sourceWksInts;
}

/**
 * Function ORIGIN_createMp
 * Create a new matrixpage and rename it.
 * @param string mpName() the name of the page to create
 * @return MatrixPage mp the page object
 **/
MatrixPage ORIGIN_createMp(string mpName = ""){
    MatrixPage mp;
	string strippedName = MISC_stripName(mpName);
	
    if(!strippedName.IsEmpty()){
		// try using use existing matrix
		MatrixPage mp2(strippedName);
		if(mp2){ // existing worksheet
			mp = mp2;
		} else { // new worksheet
			mp.Create();
		    mp.Layers(0).Delete();
			mp.SetName(strippedName);		
		}
    } else {
		mp.Create();
	    mp.Layers(0).Delete();
	}

    return mp;
}

/**
 * Function ORIGIN_createMl
 * Create a new matrix layer in a given matrix page. 
 * @param MatrixPage wb the parent matrixpage of the matrix layer to create
 * @param string mlName() the desired name of the matrix layer to create
 * @param bool forceNew(false) defines if a new layer must be created even if name exists
 * @return MatrixLayer ml the layer object
 **/
MatrixLayer ORIGIN_createMl(MatrixPage mp, string mlName = "", bool forceNew = false){
	int mlInt;
	string strippedName = MISC_stripName(mlName);
	
	if(!strippedName.IsEmpty()){
		// try using existing layer
		Worksheet ml = mp.Layers(strippedName);
		if(ml){ // existing layer
			if(forceNew){ // modify new layer name (avoid overwriting)
				int i = 2;
				string newName;
				while(ml){ // wait until no worksheet found
					newName = strippedName + "_" + ftoa(i++);
					ml = mp.Layers(newName);
				}

				// create new matrix layer
				mlInt = MATRIXPAGE_addMatrixLayer(mp, newName);
			} else { // use existing worksheet
				mlInt = ml.GetIndex();
			}
		} else { // new matrix layer
			mlInt = MATRIXPAGE_addMatrixLayer(mp, strippedName);
		}
	} else { // new worksheet
		mlInt = MATRIXPAGE_addMatrixLayer(mp);
	}
	
	return mp.Layers(mlInt);
}

#endif