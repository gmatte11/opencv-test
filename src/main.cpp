#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

const char *window_name = "Viewer";

enum Keys
{
    kQ = 1048689,

    kLeft = 1113937,
    kUp = 1113938,
    kRight = 1113939,
    kDown = 1113940
};

void apply(Mat & dst, const Mat & src, double angle, double warp)
{
    Point2f srcTri[3];
    Point2f dstTri[3];

    Mat rot_mat( 2, 3, CV_32FC1 );
    Mat warp_mat( 2, 3, CV_32FC1 );
    Mat warp_dst;

    /// Set the dst image the same type and size as src
    warp_dst = Mat::zeros( src.rows, src.cols, src.type() );

    /// Set your 3 points to calculate the  Affine Transform
    srcTri[0] = Point2f(0, 0);
    srcTri[1] = Point2f(src.cols - 1, 0);
    srcTri[2] = Point2f(0, src.rows - 1);

    dstTri[0] = Point2f(warp, 0);
    dstTri[1] = Point2f(src.cols - 1, 0);
    dstTri[2] = Point2f(0, src.rows * warp);

    /// Get the Affine Transform
    warp_mat = getAffineTransform( srcTri, dstTri );

    /// Apply the Affine Transform just found to the src image
    warpAffine( src, warp_dst, warp_mat, warp_dst.size() );

    /** Rotating the image after Warp */

    /// Compute a rotation matrix with respect to the center of the image
    Point center = Point( warp_dst.cols/2, warp_dst.rows/2 );
    double scale = 1.0;

    /// Get the rotation matrix with the specifications above
    rot_mat = getRotationMatrix2D( center, angle, scale );

    /// Rotate the warped image
    warpAffine( warp_dst, dst, rot_mat, warp_dst.size() );
}

/** @function main */
 int main( int argc, char** argv )
 {
    Mat src, dst;

    src = imread(argv[1], 1);
    dst = src.clone();

    /// Show what you got
    namedWindow(window_name, CV_WINDOW_AUTOSIZE);
    imshow(window_name, dst);

    int key = 0;
    double angle = 0.0;
    double warp = 1.0;

    while ((key = waitKey(0)) != kQ)
    {
        if (key == kLeft) angle -= 5.0;
        if (key == kRight) angle += 5.0;
        if (key == kDown) warp -= 0.1;
        if (key == kUp) warp += 0.1;
        if (key == -1) break;

        warp = std::max(0.1, warp);

        apply(dst, src, angle, warp);

        imshow(window_name, dst);
    }

    return 0;
  }