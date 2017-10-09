/*******************************************************
* Open Source for Iris : OSIRIS
* Version : 4.0
* Date : 2011
* Author : Guillaume Sutra, Telecom SudParis, France
* License : BSD
********************************************************/

#include <fstream>
#include <iterator>
#include <stdexcept>
#include "OsiManager.h"
#include "OsiStringUtils.h"

using namespace std ;

namespace osiris
{


    // CONSTRUCTORS & DESTRUCTORS
    /////////////////////////////


    // Default constructor
    OsiManager::OsiManager ( )
    {
        // Associate lines of configuration file to the attributes
        mMapBool["Process segmentation"] = &mProcessSegmentation ;
        mMapBool["Process normalization"] = &mProcessNormalization ;
        mMapBool["Process encoding"] = &mProcessEncoding ;
        mMapBool["Process matching"] = &mProcessMatching ;
        mMapBool["Use the mask provided by osiris"] = &mUseMask ;
        mMapString["List of images"] = &mFilenameListOfImages ;
        mMapString["Load original images"] = &mInputDirOriginalImages ;
        mMapString["Load parameters"] = &mInputDirParameters ;
        mMapString["Load masks"] = &mInputDirMasks ;
        mMapString["Load normalized images"] = &mInputDirNormalizedImages ;
        mMapString["Load normalized masks"] = &mInputDirNormalizedMasks ;
        mMapString["Load iris codes"] = &mInputDirIrisCodes ;
        mMapString["Save segmented images"] = &mOutputDirSegmentedImages ;
        mMapString["Save contours parameters"] = &mOutputDirParameters ;
        mMapString["Save masks of iris"] = &mOutputDirMasks ;
        mMapString["Save normalized images"] = &mOutputDirNormalizedImages ;
        mMapString["Save normalized masks"] = &mOutputDirNormalizedMasks ;
        mMapString["Save iris codes"] = &mOutputDirIrisCodes ;
        mMapString["Save matching scores"] = &mOutputFileMatchingScores ;
        mMapInt["Minimum diameter for pupil"] = &mMinPupilDiameter ;
        mMapInt["Maximum diameter for pupil"] = &mMaxPupilDiameter ;
        mMapInt["Minimum diameter for iris"] = &mMinIrisDiameter ;
        mMapInt["Maximum diameter for iris"] = &mMaxIrisDiameter ;
        mMapInt["Width of normalized image"] = &mWidthOfNormalizedIris ;
        mMapInt["Height of normalized image"] = &mHeightOfNormalizedIris ;
        mMapString["Gabor filters"] = &mFilenameGaborFilters ;
        mMapString["Application points"] = &mFilenameApplicationPoints ;
        mMapString["Suffix for segmented images"] = &mSuffixSegmentedImages ;
        mMapString["Suffix for parameters"] = &mSuffixParameters ;
        mMapString["Suffix for masks of iris"] = &mSuffixMasks ;
        mMapString["Suffix for normalized images"] = &mSuffixNormalizedImages ;
        mMapString["Suffix for normalized masks"] = &mSuffixNormalizedMasks ;
        mMapString["Suffix for iris codes"] = &mSuffixIrisCodes ;

        // Initialize all parameters
        initConfiguration() ;        
    }





    // Default destructor
    OsiManager::~OsiManager ( )
    {
        // Release matrix for application points
        if ( mpApplicationPoints )
        {
            cvReleaseMat(&mpApplicationPoints) ;
        }
        
        // Release matrix for Gabor filters
        for ( int f = 0 ; f < mGaborFilters.size() ; f++ )
        {
            cvReleaseMat(&mGaborFilters[f]) ;
        }
    }








    // OPERATORS
    ////////////



    // Initialize all configuration parameters
    void OsiManager::initConfiguration ( )
    {
        // Options of processing
        mProcessSegmentation = false ;
        mProcessNormalization = false ;
        mProcessEncoding = false ;
        mProcessMatching = false ;
        mUseMask = true ;

        // Inputs
        mListOfImages.clear() ;
        mFilenameListOfImages = "" ;
        mInputDirOriginalImages = "" ;
        mInputDirMasks = "" ;
        mInputDirParameters = "" ;
        mInputDirNormalizedImages = "" ;
        mInputDirNormalizedMasks = "" ;
        mInputDirIrisCodes = "" ;

        // Outputs
        mOutputDirSegmentedImages = "" ;
        mOutputDirParameters = "" ;
        mOutputDirMasks = "" ;
        mOutputDirNormalizedImages = "" ;
        mOutputDirNormalizedMasks = "" ;
        mOutputDirIrisCodes = "" ;
        mOutputFileMatchingScores = "" ;

        // Parameters
        mMinPupilDiameter = 21 ;
        mMaxPupilDiameter = 91 ;
        mMinIrisDiameter = 99 ;
        mMaxIrisDiameter = 399 ;
        mWidthOfNormalizedIris = 512 ;
        mHeightOfNormalizedIris = 64 ;
        mFilenameGaborFilters = "./filters.txt" ;
        mFilenameApplicationPoints = "./points.txt" ;
        mGaborFilters.clear() ;
        mpApplicationPoints = 0 ;

        // Suffix for filenames
        mSuffixSegmentedImages = "_segm.bmp" ;
        mSuffixParameters = "_para.txt" ;
        mSuffixMasks = "_mask.bmp" ;
        mSuffixNormalizedImages = "_imno.bmp" ;
        mSuffixNormalizedMasks = "_mano.bmp" ;
        mSuffixIrisCodes = "_code.bmp" ;
    }









    // Load the configuration from a textfile (ini)
    void OsiManager::loadConfiguration ( const string & rFilename )
    {
        // Open the file
        ifstream file(rFilename.c_str(),ifstream::in) ;
        
        if ( ! file.good() )
            throw runtime_error("Cannot read configuration file " + rFilename ) ;

        // Some string functions
        OsiStringUtils osu ;

        // Loop on lines
        while ( file.good() && ! file.eof() )
        {
            // Get the new line
            string line ;
            getline(file,line) ;

            // Filter out comments
            if ( ! line.empty() )
            {
                int pos = line.find('#') ;
                if ( pos != string::npos )
                    line = line.substr(0,pos) ;
            }

            // Split line into key and value
            if ( ! line.empty() )
            {
                int pos = line.find("=") ;

                if ( pos != string::npos )
                {
                    // Trim key and value
                    string key = osu.trim(line.substr(0,pos)) ;
                    string value = osu.trim(line.substr(pos+1)) ;

                    if ( ! key.empty() && ! value.empty() )
                    {
                        // Option is type bool
                        if ( mMapBool.find(key) != mMapBool.end() )
                            *mMapBool[key] = osu.fromString<bool>(value) ;

                        // Option is type int
                        else if ( mMapInt.find(key) != mMapInt.end() )
                            *mMapInt[key] = osu.fromString<int>(value) ;

                        // Option is type string
                        else if ( mMapString.find(key) != mMapString.end() )
                            *mMapString[key] = osu.convertSlashes(value) ;

                        // Option is not stored in any mMap
                        else
                            cout << "Unknown option in configuration file : " << line << endl ;
                    }
                }
            }
        }

        
        // Load the list containing all images
        loadListOfImages() ;

        // Load the datas for Gabor filters
        if ( mProcessEncoding && mFilenameGaborFilters != "" )
        {
            loadGaborFilters() ;
        }

        // Load the application points
        if ( mProcessMatching && mFilenameApplicationPoints != "" )
        {
            loadApplicationPoints() ;
        }
    }



    // Show the configuration of Osiris in prompt command
    void OsiManager::showConfiguration ( )
    {
        cout << "=============" << endl ;
        cout << "Configuration" << endl ;
        cout << "=============" << endl ;
        
        cout << endl ;

        cout << "- Process : " ;
        if ( mProcessSegmentation )
        {
            cout << "| segmentation |" ;
        }
        if ( mProcessNormalization )
        {
            cout << "| normalization |" ;
        }
        if ( mProcessEncoding )
        {
            cout << "| encoding |" ;
        }
        if ( mProcessMatching )
        {
            cout << "| matching |" ;
        }
        if ( ! mUseMask )
        {
            cout << " do not use osiris masks" ;
        }
        cout << endl ;

        cout << "- List of images " << mFilenameListOfImages << " contains " << mListOfImages.size() << " images" << endl ;
        
        cout << endl ;

        if ( mInputDirOriginalImages != "" )
        {
            cout << "- Original images will be loaded from : " << mInputDirOriginalImages << endl ;
        }
        if ( mInputDirMasks != "" )
        {
            cout << "- Masks will be loaded from : " << mInputDirMasks << endl ;
        }
        if ( mInputDirParameters != "" )
        {
            cout << "- Parameters will be loaded from : " << mInputDirParameters << endl ;
        }
        if ( mInputDirNormalizedImages != "" )
        {
            cout << "- Normalized images will be loaded from : " << mInputDirNormalizedImages << endl ;
        }
        if ( mInputDirNormalizedMasks != "" )
        {
            cout << "- Normalized masks will be loaded from : " << mInputDirNormalizedMasks << endl ;
        }
        if ( mInputDirIrisCodes != "" )
        {
            cout << "- Iris codes will be loaded from : " << mInputDirIrisCodes << endl ;
        }

        cout << endl ;

        if ( mProcessSegmentation && mOutputDirSegmentedImages != "" )
        {
            cout << "- Segmented images will be saved as : " << mOutputDirSegmentedImages  << "XXX" << mSuffixSegmentedImages << endl ;
        }
        if ( mProcessSegmentation && mOutputDirParameters != "" )
        {
            cout << "- Parameters will be saved as : " << mOutputDirParameters << "XXX" << mSuffixParameters << endl ;
        }
        if ( mProcessSegmentation && mOutputDirMasks != "" )
        {
            cout << "- Masks will be saved as : " << mOutputDirMasks << "XXX" << mSuffixMasks << endl ;
        }
        if ( mProcessNormalization && mOutputDirNormalizedImages != "" )
        {
            cout << "- Normalized images will be saved as : " << mOutputDirNormalizedImages << "XXX" << mSuffixNormalizedImages << endl ;
        }
        if ( mProcessNormalization && mOutputDirNormalizedMasks != "" )
        {
            cout << "- Normalized masks will be saved as : " << mOutputDirNormalizedMasks << "XXX" << mSuffixNormalizedMasks << endl ;
        }
        if ( mProcessEncoding && mOutputDirIrisCodes != "" )
        {
            cout << "- Iris codes will be saved as : " << mOutputDirIrisCodes << "XXX" << mSuffixIrisCodes << endl ;
        }
        if ( mProcessMatching && mOutputFileMatchingScores != "" )
        {
            cout << "- Matching scores will be saved in : " << mOutputFileMatchingScores << endl ;
        }

        cout << endl ;

        if ( mProcessSegmentation )
        {
            cout << "- Pupil diameter ranges from " << mMinPupilDiameter << " to " << mMaxPupilDiameter << endl ;
            cout << "- Iris diameter ranges from " << mMinIrisDiameter << " to " << mMaxIrisDiameter << endl ;
        }

        if ( mProcessNormalization || mProcessMatching || mProcessEncoding )
        {
            cout << "- Size of normalized iris is " << mWidthOfNormalizedIris << " x " << mHeightOfNormalizedIris << endl ;
        }
        
        cout << endl ;

        if ( mProcessEncoding && mGaborFilters.size() )
        {
            cout << "- " << mGaborFilters.size() << " Gabor filters : " ;
            for ( int f = 0 ; f < mGaborFilters.size() ; f++ )
                cout << mGaborFilters[f]->rows << "x" << mGaborFilters[f]->cols << " " ;
            cout << endl ;
        }

        if ( mProcessMatching && mpApplicationPoints )
        {
            double max_val ;
            cvMinMaxLoc(mpApplicationPoints,0,&max_val) ;
            cout << "- " << cvSum(mpApplicationPoints).val[0]/max_val << " application points" << endl ;
        }

    } // end of function



    // Load the Gabor filters (matrix coefficients) from a textfile
    void OsiManager::loadGaborFilters ( )
    {
        // Open text file containing the filters
        ifstream file(mFilenameGaborFilters.c_str(),ios::in) ;
        if ( ! file )
        {
            throw runtime_error("Cannot load Gabor filters in file " + mFilenameGaborFilters) ;
        }
        
        // Get the number of filters
        int n_filters ;
        file >> n_filters ;
        mGaborFilters.resize(n_filters) ;

        // Size of filter
        int rows , cols ;

        // Loop on each filter
        for ( int f = 0 ; f < n_filters ; f++ )
        {    
            // Get the size of the filter
            file >> rows ;
            file >> cols ;

            // Temporary filter. Will be destroyed at the end of loop
            mGaborFilters[f] = cvCreateMat(rows,cols,CV_32FC1) ;            
            
            // Set the value at coordinates r,c
            for ( int r = 0 ; r < rows ; r++ )
            {
                for ( int c = 0 ; c < cols ; c++ )
                {
                    file >> mGaborFilters[f]->data.fl[r*cols+c] ;
                }
            }

        } // Loop on each filter
        
        // Close the file
        file.close() ;

    } // end of function





    // Load the application points (build a binary matrix) from a textfile
    void OsiManager::loadApplicationPoints ( )
    {
        // Open text file containing the filters
        ifstream file(mFilenameApplicationPoints.c_str(),ios::in) ;
        if ( ! file )
        {
            throw runtime_error("Cannot load the application points in " + mFilenameApplicationPoints) ;
        }
        
        // Get the number of points
        int n_points = 0 ;
        file >> n_points ;

        // Allocate memory for the matrix containing the points
        mpApplicationPoints = cvCreateMat(mHeightOfNormalizedIris,mWidthOfNormalizedIris,CV_8UC1) ;

        // Initialize all pixels to "off"
        cvSet(mpApplicationPoints,cvScalar(0)) ;        

        // Local variables
        int i , j ;

        // Loop on each point
        for ( int p = 0 ; p < n_points ; p++ )
        {    
            // Get the coordinates
            file >> i ; file >> j ;
            
            // Set pixel to "on"
            if ( i < 0 || i > mpApplicationPoints->rows-1 || j < 0 || j > mpApplicationPoints->cols-1 )
            {
                cout << "Point (" << i << "," << j << ") " ;
                cout << "exceeds size of normalized image : " ;
                cout << mpApplicationPoints->rows << "x" << mpApplicationPoints->cols ;
                cout << " while loading application points" << endl ;
            }
            else
            {
                mpApplicationPoints->data.ptr[(i)*mpApplicationPoints->cols+j] = 255 ;
            }
        }

        // Close the file
        file.close() ;

    } // end of function







    // Load the application points from a textfile
    void OsiManager::loadListOfImages ( )
    {
        // Open the file
        ifstream file(mFilenameListOfImages.c_str(),ios::in) ;

        // If file is not opened
        if ( ! file )
        {
            throw runtime_error("Cannot load the list of images in " + mFilenameListOfImages) ;
        }

        // Fill in the list
        copy(istream_iterator<string>(file),istream_iterator<string>(),back_inserter(mListOfImages)) ;

        // Close the file
        file.close() ;

    } // end of function






    // Load, segment, normalize, encode, and save according to user configuration
    void OsiManager::processOneEye ( const string & rFileName , OsiEye & rEye )
    {
        //cout << "Process " << rFileName << endl ;

        // Strings handle
        OsiStringUtils osu ;

        // Get eye name
        string short_name = osu.extractFileName(rFileName) ;

        // Load original image only if segmentation or normalization is requested
        if ( mProcessSegmentation || mProcessNormalization )
        {
            if ( mInputDirOriginalImages != "" )
            {
                rEye.loadOriginalImage(mInputDirOriginalImages+rFileName) ;                
            }
            else
            {
                throw runtime_error("Cannot segmente/normalize without loading original image") ;
            }
        }




        /////////////////////////////////////////////////////////////////
        // SEGMENTATION : process, load
        /////////////////////////////////////////////////////////////////

        // Segmentation step
        if ( mProcessSegmentation )
        {
            rEye.segment(mMinIrisDiameter,mMinPupilDiameter,mMaxIrisDiameter,mMaxPupilDiameter) ;

            // Save segmented image
            if ( mOutputDirSegmentedImages != "" )
            {
                rEye.saveSegmentedImage(mOutputDirSegmentedImages+short_name+mSuffixSegmentedImages) ;
            }

            // If user don't want to use the mask provided by Osiris
            if ( ! mUseMask )
            {
                rEye.initMask() ;
            }
        }

        // Load parameters
        if ( mInputDirParameters != "" )
        {
            rEye.loadParameters(mInputDirParameters+short_name+mSuffixParameters) ;
        }

        // Load mask
        if ( mInputDirMasks != "" )
        {
            rEye.loadMask(mInputDirMasks+short_name+mSuffixMasks) ;
        }





        /////////////////////////////////////////////////////////////////
        // NORMALIZATION : process, load
        /////////////////////////////////////////////////////////////////

        // Normalization step
        if ( mProcessNormalization )
        {
            rEye.normalize(mWidthOfNormalizedIris,mHeightOfNormalizedIris) ;
        }

        // Load normalized image
        if ( mInputDirNormalizedImages != "" )
        {
            rEye.loadNormalizedImage(mInputDirNormalizedImages+short_name+mSuffixNormalizedImages) ;
        }

        // Load normalized mask
        if ( mInputDirNormalizedMasks != "" )
        {
            rEye.loadNormalizedMask(mInputDirNormalizedMasks+short_name+mSuffixNormalizedMasks) ;
        }        



        /////////////////////////////////////////////////////////////////
        // ENCODING : process, load
        /////////////////////////////////////////////////////////////////

        // Encoding step
        if ( mProcessEncoding )
        {
            rEye.encode(mGaborFilters) ;
        }

        // Load iris code
        if ( mInputDirIrisCodes != "" )
        {
            rEye.loadIrisCode(mInputDirIrisCodes+short_name+mSuffixIrisCodes) ;
        }



        /////////////////////////////////////////////////////////////////
        // SAVE
        /////////////////////////////////////////////////////////////////

        // Save parameters
        if ( mOutputDirParameters != "" )
        {
            if ( ! mProcessSegmentation && ( mInputDirParameters == "" ) )
            {
                cout << "Cannot save parameters because they are neither computed nor loaded" << endl ;
            }
            else
            {
                rEye.saveParameters(mOutputDirParameters+short_name+mSuffixParameters) ;
            }
        }

        // Save mask
        if ( mOutputDirMasks != "" )
        {
            if ( ! mProcessSegmentation && ( mInputDirMasks == "" ) )
            {
                cout << "Cannot save masks because they are neither computed nor loaded" << endl ;
            }
            else
            {
                rEye.saveMask(mOutputDirMasks+short_name+mSuffixMasks) ;
            }
        }

        // Save normalized image
        if ( mOutputDirNormalizedImages != "" )
        {
            if ( ! mProcessNormalization && ( mInputDirNormalizedImages == "" ) )
            {
                cout << "Cannot save normalized images because they are neither computed nor loaded" << endl ;
            }
            else
            {
                rEye.saveNormalizedImage(mOutputDirNormalizedImages+short_name+mSuffixNormalizedImages) ;
            }
            
        }

        // Save normalized mask
        if ( mOutputDirNormalizedMasks != "" )
        {
            if ( ! mProcessNormalization && ( mInputDirNormalizedMasks == "" ) )
            {
                cout << "Cannot save normalized masks because they are neither computed nor loaded" << endl ;
            }
            else
            {
                rEye.saveNormalizedMask(mOutputDirNormalizedMasks+short_name+mSuffixNormalizedMasks) ;
            }            
        }

        // Save iris code
        if ( mOutputDirIrisCodes != "" )
        {
            if ( ! mProcessEncoding && ( mInputDirIrisCodes == "" ) )
            {
                cout << "Cannot save iris codes because they are neither computed nor loaded" << endl ;
            }
            else
            {
                rEye.saveIrisCode(mOutputDirIrisCodes+short_name+mSuffixIrisCodes) ;                
            }    
        }

    } // end of function










    // Run osiris
    void OsiManager::run ( )
    {
        cout << endl ;
        cout << "================" << endl ;
        cout << "Start processing" << endl ;
        cout << "================" << endl ;
        cout << endl ;

        // If matching is requested, create a file
        ofstream result_matching ;
        if ( mProcessMatching && mOutputFileMatchingScores != "" )
        {
            try 
            {
                result_matching.open(mOutputFileMatchingScores.c_str(),ios::out) ;
            }
            catch ( exception & e )
            {
                cout << e.what() << endl ;
                throw runtime_error("Cannot create the file for matching scores : " + mOutputFileMatchingScores) ;
            }
        }

        for ( int i = 0 ; i < mListOfImages.size() ; i++ )
        {
            // Message on prompt command to know the progress
            cout << i+1 << " / " << mListOfImages.size() << endl ;

            try
            {
                // Process the eye
                OsiEye eye ;
                processOneEye(mListOfImages[i],eye) ;            

                // Process a second eye if matching is requested
                if ( mProcessMatching && (i<mListOfImages.size()-1) )
                {
                    i++ ;
                    cout << i+1 << " / " << mListOfImages.size() << endl ;
                    OsiEye eye2 ;
                    processOneEye(mListOfImages[i],eye2) ;

                    // Match the two iris codes
                    float score = eye.match(eye2,mpApplicationPoints) ;

                    // Save in file
                    if ( result_matching )
                    {                    
                        try
                        {
                            result_matching << mListOfImages[i-1] << " " ;
                            result_matching << mListOfImages[i] << " " ;
                            result_matching << score << endl ;
                        }
                        catch ( exception & e )
                        {
                            cout << e.what() << endl ;
                            throw runtime_error("Error while saving result of matching in " + mOutputFileMatchingScores) ;
                        }
                    }
                }
            }

            catch ( exception & e )
            {
                cout << e.what() << endl ;                
            }

        } // end for images

        // If matching is requested, close the file
        if ( result_matching )
        {
            result_matching.close() ;
        }

        cout << endl ;
        cout << "==============" << endl ;
        cout << "End processing" << endl ;
        cout << "==============" << endl ;
        cout << endl ;

    } // end of function

} // end of namespace


