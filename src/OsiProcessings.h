/*******************************************************
* Open Source for Iris : OSIRIS
* Version : 4.0
* Date : 2011
* Author : Guillaume Sutra, Telecom SudParis, France
* License : BSD
********************************************************/

#ifndef OSI_PROCESSINGS_H
#define OSI_PROCESSINGS_H

// pi = 3.14159
#define OSI_PI 3.14159f

// Sizes for iris and pupil
#define OSI_SMALLEST_PUPIL 11
#define OSI_SMALLEST_IRIS 99
#define OSI_MAX_RATIO_PUPIL_IRIS 0.7f
#define OSI_MIN_RATIO_PUPIL_IRIS 0.2f


#include "highgui.h"
#include "OsiCircle.h"

namespace osiris
{

    /** Image processing functions.
    * Public functions are the main steps for iris recognition :
    * segmentation, normalization, encoding, and matching.
    * Private functions are used in the public functions.
    */
    class OsiProcessings
    {

    public :

        /** Default constructor. */
        OsiProcessings ( ) ;

        /** Default destructor. */
        ~OsiProcessings ( ) ;
        


        /** Find inner and outer boundaries of iris.
        * Detect and locate the pupil. Smooth the image using the Anisotropic Smoothing.
        * Extract the radial gradients using Sobel operator.
        * Find 4 optimal contours (accurate and coarse contours for pupil and iris) using the Viterbi algorithm.
        * Build the binary mask of iris : each pixel is labelled as "iris" (1) or "not iris" (0).
        * Compute the normalization from the coarse contours detected by the Viterbi.
        * The four optional parameters allow adaptation to different database : \n
        * Increase the minimum diameters and/or decrease the maximum diameters : faster, but can miss the pupil/iris that are not in the size range \n
        * Decrease the minimum diameters and/or increase the maximum diameters : increase the probability of false alarm
        * @param [in] pSrc An eye image
        * @param [out] pMask The mask of iris. Filled by the function segment. Must be created before the function segment
        * @param [out] rPupil The circle for the pupil. Filled by the function segment
        * @param [out] rIris The circle for the iris. Filled by the function segment
        * @param [in] minIrisDiameter The minimum diameter for segmenting the iris
        * @param [in] minPupilDiameter The minimum diameter for segmenting the pupil
        * @param [in] maxIrisDiameter The maximum diameter for segmenting the iris
        * @param [in] maxPupilDiameter The maximum diameter for segmenting the pupil
        * @return void
        * @see detectPupil() , findContour() , OsiCircle::computeCircleFitting() , normalize() , OsiEye::segment()
        */
		void segment ( const IplImage * pSrc ,
							 IplImage * pMask ,
							 OsiCircle & rPupil ,
							 OsiCircle & rIris ,
							 std::vector<float> & rThetaCoarsePupil ,
							 std::vector<float> & rThetaCoarseIris ,
							 std::vector<CvPoint> & rCoarsePupilContour ,
							 std::vector<CvPoint> & rCoarseIrisContour ,
							 int minIrisDiameter = OSI_SMALLEST_IRIS,
							 int minPupilDiameter = OSI_SMALLEST_PUPIL ,
							 int maxIrisDiameter = 0 ,
							 int maxPupilDiameter = 0 ) ;



        /** Normalize iris by Daugman's rubber sheet method.
        * Use the function segment() to obtain pupil and iris contours.
        * @param pSrc The source image
        * @param pDst The normalized image. Must be created BEFORE this function.
        * @param rPupil The circle modeling the pupil
        * @param rIris The circle modeling the iris
        * @return void
        * @see normalizeFromContour() , segment() , encode() , OsiEye::normalize()
        */
        void normalize ( const IplImage * pSrc , IplImage * pDst , const OsiCircle & rPupil , const OsiCircle & rIris ) ;



		/** Normalize iris by Daugman's rubber sheet method.
        * Use the function segment() to obtain pupil and iris contours.
        * @param pSrc The source image
        * @param pDst The normalized image. Must be created BEFORE this function.
        * @param rPupil The contour modeling the pupil
        * @param rIris The contour modeling the iris
        * @return void
        * @see normalize() , segment() , encode() , OsiEye::normalize()
        */
		void normalizeFromContour ( const IplImage * pSrc ,
										  IplImage * pDst ,
									const OsiCircle & rPupil ,
									const OsiCircle & rIris ,
									const std::vector<float> rThetaCoarsePupil ,
									const std::vector<float> rThetaCoarseIris ,
									const std::vector<CvPoint> & rPupilCoarseContour ,
									const std::vector<CvPoint> & rIrisCoarseContour ) ;


		CvPoint interpolate ( const std::vector<CvPoint> coarseContour ,
											const std::vector<float> coarseTheta ,
										    const float theta ) ;


        /** Encode the iris texture into a binary image.
        * @param pSrc The normalized iris obtained by function normalize()
        * @param pDst The binary iris code. Must be created BEFORE this function.
        * @param rFilters The bank of Gabor filters used to encode the iris texture.
        * @return void
        * @see normalize() , match() , OsiEye::encode()
        */
        void encode ( const IplImage * pSrc , IplImage * pDst , const std::vector<CvMat*> & rFilters ) ;



        /** Match two iris codes.
        * @param image1 First binary iris code, obtained by function encode()
        * @param image2 Second binary iris code, obtained by function encode()
        * @param mask Mask of matching. Same size as image1 and image2.
        * @return The macthing score between 0 (completely similar) and 1 (completely different)
        * @see encode() , OsiEye::match()
        */
        float match ( const IplImage * image1 , const IplImage * image2 , const IplImage * mask ) ;



    private :



        /** Add borders on left and right of unwrapped image.        
        * @param pImage The original image
        * @param width The border width
        * @return The new image
        * @see match() , encode()
        */
        IplImage * addBorders ( const IplImage * pImage , int width ) ;



        /** Convert polar coordinates to cartesian coordinates.
        * @param rCenter The reference center in cartesian coordinates
        * @param rRadius The radius coordinate
        * @param rTheta The angle coordinate in radians
        * @return The cartesian coordinates of the point
        */
        CvPoint convertPolarToCartesian ( const CvPoint & rCenter , int rRadius , float rTheta ) ;



        /** Morphological reconstruction.
        * @param pMarker [in] A binary image. The "on" pixels are to be reconstructed
        * @param pMask [in] A binary image. The "off" pixels will not be reconstructed
        * @param pDst [out] The destination image. Must be created BEFORE this function.
        * @return void
        * @see fillWhiteHoles()
        */
        void reconstructMarkerByMask ( const IplImage * pMarker , const IplImage * pMask , IplImage * pDst ) ;



        /** Fill white holes such as spotlights (specular reflections).
        * @param pSrc [in] The source image
        * @param pDst [out] The destination image
        * @return void
        * @see reconstructMarkerByMask()
        */
        void fillWhiteHoles ( const IplImage * pSrc , IplImage * pDst ) ;



        /** Detect a pupil inside an image.
        * The optional arguments limit the search domain.
        * @param pSrc [in] The source image
        * @param rPupil [out] The detected pupil
        * @param minPupilDiameter [in] The minimum diameter for detecting the pupil
        * @param maxPupilDiameter [in] The maximum diameter for detecting the pupil
        * @return void
        * @see segment() , fillWhiteHoles()
        */
        void detectPupil ( const IplImage * pSrc , OsiCircle & rPupil , int minPupilDiameter = OSI_SMALLEST_PUPIL , int maxPupilDiameter = 0 ) ;



        /** Show an image (rescale if needed).
        * @param pImage An image
        * @param delay Milliseconds to wait
        * @param rWindowName The window name
        * @return void
        */
        void showImage ( const IplImage * pImage , int delay = 0 , const std::string & rWindowName = "Show image" ) ;



        /** Unwrap a ring into a rectangular band.
        * @param pSrc The source image
        * @param rCenter The ring center
        * @param minRadius The minimum radius
        * @param maxRadius The maximum radius
        * @param rTheta A vector of angles in radians
        * @return The unwrapped ring
        * @see findContour() , runViterbi() , convertPolarToCartesian()
        */
        IplImage * unwrapRing ( const IplImage * pSrc , const CvPoint & rCenter , int minRadius , int maxRadius , const std::vector<float> & rTheta ) ;



        /** Smooth image using anisotropic smoothing.
        * @param pSrc [in] The source image
        * @param pDst [out] The destination image
        * @param iterations [in] The number of iterations
        * @param lambda [in] The smooth constraint
        * @return void
        * @see segment()
        */
        void processAnisotropicSmoothing ( const IplImage * pSrc , IplImage * pDst , int iterations = 100 , float lambda = 1 ) ;



        /** Compute vertical gradients using Sobel operator.
        * @param pSrc [in] The source image
        * @param pDst [out] The destination image. Must be IPL_DEPTH_32F
        * @return void
        * @see findContour()
        */
        void computeVerticalGradients ( const IplImage * pSrc , IplImage * pDst ) ;



        /** Run Viterbi algorithm on gradient or probability image and find the optimal path.
        * This function is used by findContour function. \n
        * Source image is an \b unwrapped image. This function works in polar representation.
        * @param pSrc An unwrapped image
        * @param rOptimalPath The series of radii
        * @return void
        * @see unwrapRing()
        * @see convertPolarToCartesian()
        * @see findContour()
        */
        void runViterbi ( const IplImage * pSrc , std::vector<int> & rOptimalPath ) ;



        /** Find a contour using anisotropic smoothing + viterbi algorithm.
        * @param pSrc The source image
        * @param rCenter The center around which the contour turns
        * @param rTheta A vector of angles in radians
        * @param minRadius The minimum radius for the contour
        * @param maxRadius The maximum radius for the contour
        * @param pMask An optional mask to forbid some pixels during the search of the contour
        * @return The contour
        * @see runViterbi() , unwrapRing()
        */
        std::vector<CvPoint> findContour ( const IplImage * pSrc , const CvPoint & rCenter , const std::vector<float> & rTheta , int minRadius , int maxRadius , const IplImage * pMask = 0 ) ;



        /** Draw a contour (vector of CvPoint) on an image.
        * @param pImage The image on which contour is drawn
        * @param rContour The contour to draw
        * @param rColor The color of contour
        * @param thickness The contour thickness
        * @return void
        */
        void drawContour ( IplImage * pImage , const std::vector<CvPoint> & rContour , const CvScalar & rColor = cvScalar(255) , int thickness = 1 ) ;
    

    } ; // end of class

} // end of namespace

#endif

