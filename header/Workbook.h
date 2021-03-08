/*------------------------------------------------------------------------------*
 * File Name:	Workbook.h                                                      *
 * Creation:	Alexander Schmitz                                               *
 * Purpose:		Provides methods for Origin Workbook manipulation.              *
 * Copyright(c) 2021, Alexander Schmitz                                         *
 * All Rights Reserved                                                          *
 *                                                                              *
 * Last Modified::	08.03.2021                                                  *
 * Tasks:                                                                       *
 *------------------------------------------------------------------------------*/
#ifndef _WORKBOOK_ // include once
#define _WORKBOOK_

#include <Origin.h>

/**
 * Add a new worksheet to a workbook.
 *
 * @param WorksheetPage wb        the target workbook
 * @param string        wksName() the name of the new worksheet
 *
 * @return int wksInt the index of the new worksheet in the workbook
 **/ 
int WORKBOOK_addWorksheet(WorksheetPage wb, string wksName = "")
{
	// create new layer
	int wksInt = wb.AddLayer();
	Worksheet wks  = wb.Layers(wksInt);

	// clear worksheet
	while(wks.DeleteCol(0));
	
	// rename worksheet if applicable
	if(wksName != "")
	{
		wks.SetName(wksName);
	}

	return wksInt;
}

#endif