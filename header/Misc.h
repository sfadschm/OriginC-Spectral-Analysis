/*------------------------------------------------------------------------------*
 * File Name: Misc.h                                                            *
 * Creation:  Alexander Schmitz                                                 *
 * Purpose:   Provides basic variable/string handling methods.                  *
 * Copyright(c) 2021, Alexander Schmitz                                         *
 * All Rights Reserved                                                          *
 *                                                                              *
 * Last Modified: 08.03.2021                                                    *
 * Tasks:                                                                       *
 *------------------------------------------------------------------------------*/
#ifndef _MISC_ // include once
#define _MISC_

#include <Origin.h>

/**
 * Remove special characters and other stuff from a given string to serve as a name.
 *
 * @param string name the string to strip
 *
 * @return string name the stripped string
 **/
string MISC_stripName(string name)
{
	// strip path if name is derived from a file path
	if(name.ReverseFind('\\') >= 0)
	{
		// remove file folder hierarchy
		int splitPos = name.ReverseFind('\\') + 1;
		name = name.Mid(splitPos, name.GetLength() - splitPos);

		// remove file extension
		int extSize = name.GetLength() - name.ReverseFind('.');
		name = name.Mid(0, name.GetLength() - extSize);
	}

	return name;
}

/**
 * Remove duplicate values from a string array.
 *
 * @param vector<string> rawData the raw stirng vector
 *
 * @return vector<string> unique the trimmed vector
 **/
vector<string> MISC_arrayUnique(vector<string> rawData)
{
	// search duplicate data
	vector<int> dupI;
	vector<uint> R1s;
	rawData.FindDuplicates(dupI, R1s);

	// remove duplicates
	vector<string> unique;
	for(int i = 0; i < R1s.GetSize(); i++)
	{
		unique.Add(rawData[R1s[i]]);
	}

	return unique;
}

#endif