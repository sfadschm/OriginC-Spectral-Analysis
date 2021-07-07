/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _FILE_  // include once
#define _FILE_

/**
 * This file provides methods for manipulating text files.
 */

/**
 * Open a file in read mode.
 *
 * @param stdioFile* fileObj  the file object to open the file into
 * @param string     fileName the full path to the file to open
 */
void FILE_openRead(stdioFile &fileObj, string fileName);

/**
 * Close a file object.
 *
 * @param stdioFile fileObj the file object to close
 */
void FILE_close(stdioFile &fileObj);

#endif