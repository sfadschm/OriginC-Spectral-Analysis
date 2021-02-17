/*------------------------------------------------------------------------------*
 * File Name:	File.h		 													*
 * Creation: 	Alexander Schmitz												*
 * Purpose: 	Provides methods for text file manipulation.    				*
 * Copyright(c) 2019, Alexander Schmitz         								*
 * All Rights Reserved															*
 * 																				*
 * Last Modified::	14.11.2019		    										*
 * Tasks:           				    										*
 *------------------------------------------------------------------------------*/
#ifndef _FILE_ // include once
#define _FILE_

#include <Origin.h>
#include <stdio.h>

/**
 * Method FILE_openRead
 * Open a file in read mode.
 * @param stdioFile* fileObj the file object to open the file into
 * @param string fileName the full path to the file to open
 **/
void FILE_openRead(stdioFile &fileObj, string fileName){
    if(!fileObj.Open(fileName, file::modeRead))
        printf("File not found!"); // invalid file

    return;
}

/**
 * Method FILE_close
 * Close a file object.
 * @param stdioFile fileObj the file object to close
 **/
void FILE_close(stdioFile &fileObj){
	fileObj.Close();

	return;
}

#endif