/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#include <Origin.h>
#include <stdio.h>

#include "lang.h"

void FILE_openRead(stdioFile &fileObj, string fileName) {
    if (! fileObj.Open(fileName, file::modeRead)) {
        // invalid file
        printf(FILES_NOT_FOUND);
    }
}

void FILE_close(stdioFile &fileObj) {
    fileObj.Close();
}