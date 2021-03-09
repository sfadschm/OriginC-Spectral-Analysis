/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _MISC_ // include once
#define _MISC_

/**
 * This file provides general helper methods unrelated to Origin objects.
 */
#include <Origin.h>

/**
 * Remove special characters and other stuff from a given string to serve as a name.
 *
 * @param string name the string to strip
 *
 * @return string name the stripped string
 */
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
 */
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

/**
 * Format and return a string (like printf).
 *
 * @param string base    the format string
 * @param string str1... the replacement values
 *
 * @return string result the formatted string
 */
string MISC_formatString(string base, string str1 = "", string str2 = "", string str3 = "", string str4 = "")
{
	string result;
	result.Format(base, str1, str2, str3, str4);

	return result;
}

#endif