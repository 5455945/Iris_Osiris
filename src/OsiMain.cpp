/*******************************************************
* Open Source for Iris : OSIRIS
* Version : 4.0
* Date : 2011
* Author : Guillaume Sutra, Telecom SudParis, France
* License : BSD
********************************************************/

/** @mainpage
*
* @ref section1 \n
* @ref section2 \n
* @ref section3 \n
*
* \n\n
* @section section1 I. Introduction
*
* Osiris is an Open Source software implementing iris recognition algorithm.
* Version 4.0 was developed in 2011 by Guillaume Sutra,
* from Intermedia team, Telecom SudParis, Evry, France.
* The goal is to provide to the scientific community a tool for various experiments
* in iris recognition. For this purpose, Osiris can process \b one \b or \b more steps among:
* - segmentation
* - normalization
* - features extraction
* - matching
*
* The first step is a new approach to iris segmentation.
* The three remaining steps follow Daugman's implementation:
* the normalization step is based on the rubber-sheet model proposed by Daugman,
* the features extraction step uses Gabor filters to encode the iris texture,
* and the matching step computes the Hamming distance between two iris codes.
*
* \n\n
* @section section2 II. License
*
* Osiris is under <a href="http://opensource.org/licenses/bsd-license.php">BSD license</a>.
* By downloading, copying, installing or using the software you agree to this license.
*
* Copyright (c) 2011, Telecom SudParis
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* - Redistributions of source code must retain the above copyright 
* notice, this list of conditions and the following disclaimer.
* - Redistributions in binary form must reproduce the above copyright 
* notice, this list of conditions and the following disclaimer in the 
* documentation and/or other materials provided with the distribution.
* - The names of the copyright holders may not be used to endorse or promote
* products derived from this software without specific prior written permission.
*
* This software is provided by the regents and contributors "as is" and any
* express or implied warranties, including, but not limited to, the implied
* warranties of merchantability and fitness for a particular purpose are
* disclaimed. In no event shall the regents and contributors be liable for any
* direct, indirect, incidental, special, exemplary, or consequential damages
* (including, but not limited to, procurement of substitute goods or services;
* loss of use, data, or profits; or business interruption) however caused and
* on any theory of liability, whether in contract, strict liability, or tort
* (including negligence or otherwise) arising in any way out of the use of this
* software, even if advised of the possibility of such damage.
*
* \n\n
* @section section3 III. Overview of the system
*
* An overview of the global Osiris system is illustrated below.
* The main goal of Osiris is to allow the user to process each step individually.
* For this purpose, most of the outputs of each step can be saved,
* and/or used for the next step.
*
* <img src="Overview_Scheme.bmp" />
*/

#include "OsiManager.h"

using namespace osiris ;

int main (int argc, char * argv[])
{
    try
    {
        OsiManager osi ;        
        osi.loadConfiguration(argv[1]) ;
        osi.showConfiguration() ;        
        osi.run() ;
    }
    catch ( std::exception & e )
    {
        std::cout << e.what() << std::endl ;
    }

    return 0 ;

}

