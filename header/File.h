/*------------------------------------------------------------------------------*
 * File Name:	File.h                                                          *
 * Creation:	Alexander Schmitz                                               *
 * Purpose:		Provides methods for text file manipulation.                    *
 * Copyright(c) 2021, Alexander Schmitz                                         *
 * All Rights Reserved                                                          *
 *                                                                              *
 * Last Modified::	08.03.2021                                                  *
 * Tasks:                                                                       *
 *------------------------------------------------------------------------------*/
#ifndef _FILE_ // include once
#define _FILE_

#include <Origin.h>
#include <stdio.h>

/**
 * Open a file in read mode.
 *
 * @param stdioFile* fileObj  the file object to open the file into
 * @param string     fileName the full path to the file to open
 **/
void FILE_openRead(stdioFile &fileObj, string fileName)
{
	if(!fileObj.Open(fileName, file::modeRead))
	{
		// invalid file
		printf("File not found!");
	}
}

/**
 * Close a file object.
 *
 * @param stdioFile fileObj the file object to close
 **/
void FILE_close(stdioFile &fileObj)
{
	fileObj.Close();
}

#endif