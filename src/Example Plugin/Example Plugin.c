/**
 * This file is part of the OriginC Spectral Analysis package.
 *
 * (c) Alexander Schmitz <alexander.schmitz@uni-due.de>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * This is an exemplary main file for a package plugin.
 * All methods defined here should be called directly from the command window.
 */
#include <Origin.h>

#include "headers\plugin.h"
#include "..\Spectral Analysis\headers\lang.h"
#include "..\Spectral Analysis\headers\misc.h"

/**
 * Some example output to show the plugin working.
 */
void EXAMPLE_plugin() {
    // some direct output
    printf("\nHi, I am a main function in the Example Plugin!");

    // some output from the plugin header
    PLUGIN_HEADER_talk();
    
    // demonstrate usage of a main package header
    printf("\n\nMISC_formatString fills placeholders in a string dynamically:\nE.g., '%s' becomes '%s'.", "Starting %s ...", MISC_formatString("Starting %s ...", "Example Plugin"));
}