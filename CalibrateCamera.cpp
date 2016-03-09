


#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS
#endif

using namespace cv;
using namespace std;



template <typename T>
string NumberToString ( T Number )
{
	stringstream ss;
	ss << Number;
	return ss.str();
}

int main()
{
int numBoards = 20;
int numCornersHor = 8;
int numCornersVer = 5;

string commonFileName = "D:\\CS9645-Assign-2-Data-Set\\IMG_11"; 
string fileType = ".JPG";
int fileNum = 7; 
string fileNumStr;
Size ImgSize(816, 612); 


int numSquares = numCornersHor * numCornersVer;
Size board_sz = Size(numCornersHor, numCornersVer);

vector<vector<Point3f>> object_points;
vector<vector<Point2f>> image_points;

vector<Point2f> corners;
vector<Point3f> obj;

int successes=0;

Mat image;
Mat gray_image;

fileNumStr = NumberToString(fileNum);
if (fileNum <10)
	fileNumStr = "0"+ NumberToString(fileNum);

image = imread(commonFileName+fileNumStr+fileType,1);
resize(image,image, ImgSize);
fileNum++;


for(int j=0;j<numSquares;j++)
    obj.push_back(Point3f(j/numCornersHor, j%numCornersHor, 0.0f));

while(successes<numBoards)
{
    cvtColor(image, gray_image, CV_BGR2GRAY);

    bool found = findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		
	if(found)
    {
        cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
        drawChessboardCorners(gray_image, board_sz, corners, found);

		image_points.push_back(corners);
        object_points.push_back(obj);

        successes++;

        if(successes>=numBoards)
            break;
	}

	//imshow("win1 "+ fileNumStr, image);
    //imshow("win2 " + fileNumStr, gray_image);

	fileNumStr = NumberToString(fileNum);
	if (fileNum <10)
		fileNumStr = "0"+ NumberToString(fileNum); 
	image = imread(commonFileName+fileNumStr+fileType,1);
	resize(image,image, ImgSize);
	fileNum++;

    int key = waitKey(1);

    if(key==27)
        return 0;
	
}

Mat intrinsic = Mat(3, 3, CV_32FC1);
Mat distCoeffs;
vector<Mat> rvecs;
vector<Mat> tvecs;

intrinsic.ptr<float>(0)[0] = 1;
intrinsic.ptr<float>(1)[1] = 1;

//vector<vector<Point3f> > object_points(1);
//object_points.resize(image_points.size(),object_points[0]);

calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);

Mat imageUndistorted;
//while(1)
//{
//    //capture >> image;
//    undistort(image, imageUndistorted, intrinsic, distCoeffs);
//
//    imshow("win1", image);
//    imshow("win2", imageUndistorted);
//
//    waitKey(1);
//}

//capture.release();

return 0;
}

