/*------------------------------------------------------------------------------*
 * File Name:	Misc.h  	 													*
 * Creation: 	Alexander Schmitz												*
 * Purpose: 	Provides basic variable/string handling methods.  				*
 * Copyright(c) 2019, Alexander Schmitz         								*
 * All Rights Reserved															*
 * 																				*
 * Last Modified:	16.02.2020											    	*
 * Tasks:           				    										*
 *------------------------------------------------------------------------------*/
#ifndef _MISC_ // include once
#define _MISC_

#include <Origin.h>

/**
 * Function MISC_stripName
 * Remove special characters and other stuff from a given string to serve as a name.
 * @param string name the string to strip
 * @return string name the stripped string
 **/
string MISC_stripName(string name){
	// strip path if name is derived from a file path
	if(name.ReverseFind('\\') >= 0){
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
 * Function MISC_arrayUnique
 * Remove duplicate values from a string array.
 * @param vector<string> rawData the raw stirng vector
 * @return vector<string> unique the trimmed vector
 **/
vector<string> MISC_arrayUnique(vector<string> rawData){
	// search duplicate data
	vector<int> dupI;
	vector<uint> R1s;
	rawData.FindDuplicates(dupI, R1s);
	
	// remove duplicates
	vector<string> unique;
	for(int i = 0; i < R1s.GetSize(); i++){
		unique.Add(rawData[R1s[i]]);
	}

	return unique;
}

/**
 * Function MISC_alignGraphText
 * Align a text element in a graph window (active layer).
 **/
 void MISC_alignGraphText(){
	GraphLayer gl = Project.ActiveLayer();
	if(gl.IsValid()){
		// get user selection	
		vector<string> params;
		params = USER_alignGlTexts(gl);

		// handle user cancel
		if(atoi(params[0]) > -1){
			// map source parameters
			string objName = params[0];
			int iJustify = atoi(params[1]);

			// get object
			GraphObject go = gl.GraphObjects(objName);
			if(go.IsValid()){ // align text
				Tree tr;
				tr = go.GetFormat(FPB_ALL, FOB_ALL, true, true);
				tr.root.alignment.horizontal.nVal = iJustify;
				go.ApplyFormat(tr, true, true);
			} else { // not a valid object
				printf(USER_ALIGN_NOOBJ, objName);
				return;
			}
		} else { // user cancel
			printf(USER_PARAMS_EMPTY);
			return;
		}
	} else { // not a graph layer
		printf(USER_ALIGN_NOGRAPH);
		return;
	}
}


#endif