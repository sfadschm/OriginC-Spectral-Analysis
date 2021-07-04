/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _ORIGIN_ // include once
#define _ORIGIN_

/**
 * This file provides helper methods for Origin objects.
 */
#include <Origin.h>
#include "Lang.h"
#include "Misc.h"
#include "Workbook.h"
#include "MatrixPage.h"

/**
 * Create a new workbook and rename it.
 *
 * @param string wbName() the desired name of the new workbook
 *
 * @return WorksheetPage wb the workbook object
 */
WorksheetPage ORIGIN_createWb(string wbName = "")
{
	WorksheetPage wb;

	// strip workbook name
	string strippedName = MISC_stripName(wbName);

	// create new book if invalid name
	if(strippedName.IsEmpty())
	{
		wb.Create();
		wb.Layers(0).Delete();
		return wb;
	}

	// try using use existing workbook
	WorksheetPage wb2(strippedName);
	if(wb2)
	{
		// use existing workbook
		return wb2;
	}

	// create new workbook with name
	wb.Create();
	wb.Layers(0).Delete();
	wb.SetName(strippedName);

	return wb;
}

/**
 * Create a new worksheet in a given workbook. 
 *
 * @param WorksheetPage wb              the parent workbook of the worksheet to create
 * @param string        wksName()       the desired name of the worksheet to create
 * @param bool          forceNew(false) defines if a new worksheet must be created even if name exists
 
 * @return Worksheet wks the worksheet object
 */
Worksheet ORIGIN_createWks(WorksheetPage wb, string wksName = "", bool forceNew = false)
{
	// index of new worksheet
	int wksInt;

	// strip worksheet name
	string strippedName = MISC_stripName(wksName);

	// return new sheet if invalid name
	if(strippedName.IsEmpty())
	{
		wksInt = WORKBOOK_addWorksheet(wb);
		return wb.Layers(wksInt);
	}

	// try to get existing worksheet
	Worksheet wks = wb.Layers(strippedName);

	// return new sheet if sheet does not exist
	if(!wks)
	{
		wksInt = WORKBOOK_addWorksheet(wb, strippedName);
		return wb.Layers(wksInt);
	}

	// use existing worksheet if not forced
	if(forceNew)
	{
		// rename until no existing layer found
		int i = 2;
		string newName;
		while(wks)
		{
			newName = strippedName + "_" + ftoa(i++);
			wks = wb.Layers(newName);
		}

		// create new worksheet
		wksInt = WORKBOOK_addWorksheet(wb, newName);
	}
	else
	{
		// use existing worksheet
		wksInt = wks.GetIndex();
	}

	return wb.Layers(wksInt);
}

/**
 * Get the indexes of all worksheets in the active workbook.
 *
 * @param int            sourceType 0 if source is workbook, 1 if worksheet
 * @param WorksheetPage& wb         the workbook reference to link the active workbook to
 * @param Worksheet&     wks        the worksheet reference to link the active worksheet to
 *
 * @return vector<int> sourceWksInts the numeric indexes of the worksheet in the active workbook
 */
vector<int> ORIGIN_getActiveWorksheets(int sourceType, WorksheetPage& wb, Worksheet& wks)
{
	// storage for worksheet indices
	vector<int> sourceWksInts;

	// get active layer/page
	wks = Project.ActiveLayer();

	// abort if not a valid worksheet
	if(!wks)
	{
		printf(ANALYZE_NO_WKS);
		return sourceWksInts;
	}

	// get workbook page
	wb = wks.GetPage();

	// get source layers
	if(sourceType == 0)
	{
		// loop through workbook layers
		foreach(Layer tempWks in wb.Layers)
		{
			sourceWksInts.Add(tempWks.GetIndex());
		}
	}
	else
	{
		// get worksheet index
		sourceWksInts.Add(wks.GetIndex());
	}

	return sourceWksInts;
}

/**
 * Create a new matrixpage and rename it.
 *
 * @param string mpName() the name of the page to create
 *
 * @return MatrixPage mp the page object
 */
MatrixPage ORIGIN_createMp(string mpName = "")
{
	MatrixPage mp;

	// strip layer name
	string strippedName = MISC_stripName(mpName);

	// create new page if invalid name
	if(strippedName.IsEmpty())
	{
		mp.Create();
		mp.Layers(0).Delete();
		return mp;
	}

	// try getting existing page
	MatrixPage mp2(strippedName);

	// use exsiting page
	if(mp2)
	{
		return mp2;
	}

	// create new named page
	mp.Create();
	mp.Layers(0).Delete();
	mp.SetName(strippedName);

	return mp;
}

/**
 * Create a new matrix layer in a given matrix page. 
 *
 * @param MatrixPage wb              the parent matrixpage of the matrix layer to create
 * @param string     mlName()        the desired name of the matrix layer to create
 * @param bool       forceNew(false) defines if a new layer must be created even if name exists
 *
 * @return MatrixLayer ml the layer object
 */
MatrixLayer ORIGIN_createMl(MatrixPage mp, string mlName = "", bool forceNew = false)
{
	// layer index
	int mlInt = -1;

	// strip layer name
	string strippedName = MISC_stripName(mlName);
	if(strippedName == "")
	{
		strippedName = "MSheet1";
	}
	
	// create new layer if invalid name
	if(!strippedName.IsEmpty())
	{
		mlInt = MATRIXPAGE_addMatrixLayer(mp);
	}

	// try getting existing layer
	Worksheet ml = mp.Layers(strippedName);

	// create new named layer if not exists
	if(!ml)
	{
		mlInt = MATRIXPAGE_addMatrixLayer(mp, strippedName);
	}

	// use existing worksheet if not forced
	if(forceNew)
	{
		// rename until no existing layer found
		int i = 2;
		string newName;
		while(ml)
		{
			newName = strippedName + "_" + ftoa(i++);
			ml = mp.Layers(newName);
		}

		// create new matrix layer
		mlInt = MATRIXPAGE_addMatrixLayer(mp, newName);
	}
	else
	{
		// use existing worksheet
		if(mlInt == -1)
		{
			mlInt = ml.GetIndex();
		}
	}

	return mp.Layers(mlInt);
}

#endif