#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <opencv2/opencv.hpp>
// #include "opencv2/core/core.hpp"
// #include "opencv2/highgui/highgui.hpp"
// #include "opencv2/imgproc/imgproc.hpp"
#include <sstream>
#include <math.h>

#include <algorithm>
#include <strings.h>
#include <string.h>
#include <cstring>

#include <opencv2/core/fast_math.hpp>

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &src, image &tgt, int value);
		static void binarize(image &src, image &tgt, int threshold);
		static void scale(image &src, image &tgt, float ratio);
		static void cv_gray(cv::Mat &src, cv::Mat &tgt);
		static void cv_avgblur(cv::Mat &src, cv::Mat &tgt, int WindowSize);
		
		// hw1
		static bool isInboundsROI(image &src, int xR, int yR);
		static bool overlapROi(int xL1, int yL1, int xR1, int yR1, int xL2, int yL2, int xR2, int yR2);
		static void Rois(image &src, image &tgt, int roiNum, vector< vector <string> > rois, const char *outfileName);
		
		static void addGreyROI(image &src, image &tgt, int xL, int yL, int xR, int yR, int value);
		static void binarizeROI(image &src, image &tgt, int xL, int yL, int xR, int yR, int threshold);
		static void scaleROI(image &src, image &tgt, int xL, int yL, int xR, int yR, float ratio);
		static void AOIROI(image &src, image &tgt, int xL, int yL, int xR, int yR, int value);
		static void cBrightROI(image &src, image &tgt, int xL, int yL, int xR, int yR, int DR, int DG, int DB);
		static void cVisBrightROI(image &src, image &tgt, int xL, int yL, int xR, int yR, int V, int T);
		
		//hw2
		static void histogram2(image &src, vector<int>  channel, vector< vector <int> > coordinates, const char *outfile, const int imgIndex);
		static void histogram(image &src, int xL, int yL, int xR, int yR, vector< vector <int> > *coordinates, const char *outfile, const int imgIndex);
		static void histStretch(image &src, image &tgt, int xL, int yL, int xR, int yR, const char *histoName, const int roiIndex);
		static void comboHisto(image &src, image &tgt, int xL, int yL, int xR, int yR, int threshold1, int threshold2, const char *outfile, const int imgIndex);
		static void perChaStretch(image &src, image &tgt, int xL, int yL, int xR, int yR, char RGB, int Imin, int Imax, const char *histoName, const int roiIndex);
		static void rgbStretch(image &src, image &tgt, int xL, int yL, int xR, int yR, int Imin, int Imax, const char *histoName, const int roiIndex);
		static void colorization(image &src, image &tgt, int xL, int yL, int xR, int yR, int t1, int t2);
		static void tenColor(image &src, image &tgt, int xL, int yL, int xR, int yR);

		//hw3
		static void binaryEdge3(image &src, image &tgt, int xL, int yL, int xR, int yR, int threshold, const char *outfile, const int imgIndex);
		static void binaryEdge5(image &src, image &tgt, int xL, int yL, int xR, int yR, int threshold, const char *outfile, const int imgIndex);

		static bool cv_isInboundsROI(cv::Mat &src, int xR, int yR);
		static void cv_Rois(cv::Mat &src, cv::Mat &tgt, int roiNum, vector< vector <string> > rois, const char *outfileName);
		static void cv_gausobel(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, int kernelSize, int gauSize, double signmaX, double sigmaY);
		static void cv_sobel(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, int kernelSize);
		static void cv_otsu(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR);
		static void cv_canny(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, double threshold1, double threshold2);
		//hw4
		static void cv_histogram(cv::Mat &src, int xL, int yL, int xR, int yR, const char *outfile, const int imgIndex);
		static void cv_histostretch(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, const char *outfile, const int imgIndex);
		static void cv_histoequ(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, const char *outfile, const int imgIndex);
		static void cv_houghcir(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, int gauSize, double signmaX, double sigmaY, double threshold1, double threshold2,
		int minDist, int minR, int maxR);
		static void cv_otsuhisto(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, const char *outfile, const int imgIndex);
		static void cv_otsugau(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, int gauSize, double signmaX, double sigmaY);
		static void cv_combine(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, int gauSize, double signmaX, double sigmaY, const char *outfile, const int imgIndex);
		static void cv_subtract(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, const char *outfile);
		static void cv_qrcode(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR);
};

#endif

