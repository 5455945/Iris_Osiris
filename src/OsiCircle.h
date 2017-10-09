/*******************************************************
* Open Source for Iris : OSIRIS
* Version : 4.0
* Date : 2011
* Author : Guillaume Sutra, Telecom SudParis, France
* License : BSD
********************************************************/

#ifndef OSI_CIRCLE_H
#define OSI_CIRCLE_H

#include <iostream>
#include "highgui.h"


namespace osiris
{
    /** Circle handler.
    * Used by the Daugman's rubber sheet method
    * @see OsiProcessings::normalize()
    */
    class OsiCircle
    {

    public :

        /** Default constructor. */
        OsiCircle ( ) ;

        /** Default destructor. */
        ~OsiCircle ( ) ;

        /** Overloaded constructor.
        * @param rCenter Initialization of the center
        * @param rRadius Initialization of the radius
        */
        OsiCircle ( const CvPoint & rCenter , int rRadius ) ;



        /** Compute circle fitting by least-squares method.
        * This function is called by OsiProcessings::segment() \n
        * Reference : http://www.dtcenter.org/met/users/docs/write_ups/circle_fit.pdf
        * @param rPoints A contour in cartesian coordinates
        * @return void
        * @see segment()
        */
        void computeCircleFitting ( const std::vector<CvPoint> & rPoints ) ;



        /** Draw a circle on an image.
        * @param pImage The image on which circle is to be drawn
        * @param rColor Color of the circle
        * @param thickness Circle thickness. Set to -1 to draw the disk inside the circle
        * @return void
        */
        void drawCircle ( IplImage * pImage , const CvScalar & rColor = cvScalar(255) , int thickness = 1 ) ;



        /** Get the circle center.
        * @return The circle center
        */
        CvPoint getCenter ( ) const ;



        /** Get the circle radius.
        * @return The circle radius
        */
        int getRadius ( ) const ;



        /** Set the circle center.
        * @param rCenter The circle center
        * @return void
        */
        void setCenter ( const CvPoint & rCenter ) ;



        /** Set the circle radius.
        * @param rRadius The circle radius
        * @return void
        */
        void setRadius ( int rRadius ) ;



        /** Set the circle center and radius.
        * @param rCenter The circle center
        * @param rRadius The circle radius
        * @return void
        */
        void setCircle ( const CvPoint & rCenter , int rRadius ) ;



        /** Set the circle center and radius.
        * @param rCenterX The x-coordinate of circle center
        * @param rCenterY The y-coordinate of circle center
        * @param rRadius The circle radius
        * @return void
        */
        void setCircle ( int rCenterX , int rCenterY , int rRadius ) ;



    private :



        /** The circle center. */
        CvPoint mCenter ;

        /** The circle radius. */
        int mRadius ;


    } ; // end of class

} // end of namespace

#endif

