/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef _MISC_  // include once
#define _MISC_

/**
 * This file provides general helper methods unrelated to Origin objects.
 */

/**
 * Remove special characters from a given string to serve as a layer name.
 *
 * @param string name the string to strip
 *
 * @return string name the stripped string
 */
string MISC_stripName(string name);

/**
 * Remove duplicate values from a string array.
 *
 * @param vector<string> rawData the raw stirng vector
 *
 * @return vector<string> unique the trimmed vector
 */
vector<string> MISC_arrayUnique(vector<string> rawData);

/**
 * Format and return a string (like printf()).
 *
 * This function is a wrapper function for the string.Format()
 * method. Its parameters must be hard-coded as long as OriginC
 * does not support <stdarg.h> with variadic argument list support.
 *
 * @param string base    the format string
 * @param string str1... the replacement values
 *
 * @return string result the formatted string
 */
string MISC_formatString(string base, string str1 = "", string str2 = "",
                         string str3 = "");

/**
 * Global storage for the timer.
 */
extern int lastClock;

/**
 * Output a timer from the last time the method was called.
 *
 * @param string label if empty, resets the global timer
 */
void timer(string label = "");

#endif