/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#include <Origin.h>
#include <time.h>

string MISC_stripName(string name) {
    // strip path if name is derived from a file path
    if (name.ReverseFind('\\') >= 0) {
        // remove file folder hierarchy
        int splitPos = name.ReverseFind('\\') + 1;
        name         = name.Mid(splitPos, name.GetLength() - splitPos);

        // remove file extension
        int extSize = name.GetLength() - name.ReverseFind('.');
        name        = name.Mid(0, name.GetLength() - extSize);
    }

    return name;
}

vector<string> MISC_arrayUnique(vector<string> rawData) {
    // search duplicate data
    vector<int> dupI;
    vector<uint> R1s;
    rawData.FindDuplicates(dupI, R1s);

    // remove duplicates
    vector<string> unique;
    for (int i = 0; i < R1s.GetSize(); i++) {
        if (rawData[R1s[i]] != "") {
            unique.Add(rawData[R1s[i]]);
        }
    }

    return unique;
}

string MISC_formatString(string base, string str1 = "", string str2 = "",
                         string str3 = "") {
    string result;
    result.Format(base, str1, str2, str3);

    return result;
}

int lastClock = 0;
void timer(string label = "") {
    // get system time
    struct tm the2Time;
    SYSTEMTIME sys2Time;
    GetSystemTime(&sys2Time);

    // convert to ms value
    int milliTime =
        ((((sys2Time.wDay) * 24 + sys2Time.wHour) * 60 + sys2Time.wMinute) *
             60 +
         sys2Time.wSecond) *
            1000 +
        sys2Time.wMilliseconds;

    // no output without label
    if (label != "") {
        // output time since last call
        if (lastClock == 0) {
            printf("%s: %d ms\n", label, 0);
        } else {
            printf("%s: %d ms\n", label, (milliTime - lastClock));
        }
    }

    // set new clock
    lastClock = milliTime;

    return;
}