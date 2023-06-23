#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

using namespace cv;

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int value)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value)); 
		}
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int threshold)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::cv_gray(Mat &src, Mat &tgt)
{
	cvtColor(src, tgt, COLOR_BGR2GRAY);
}

/*-----------------------------------------------------------------------**/
void utility::cv_avgblur(Mat &src, Mat &tgt, int WindowSize)
{
	blur(src,tgt,Size(WindowSize,WindowSize));
}


/*-----------------------------------------------------------------------**/
// add gray scale intensity for ROI
void utility::addGreyROI(image &src, image &tgt, int xL, int yL, int xR, int yR, int value)
{
	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + value));
		}
}

/*-----------------------------------------------------------------------**/
// binarize the image for ROI
void utility::binarizeROI(image &src, image &tgt, int xL, int yL, int xR, int yR, int threshold)
{
	for (int i = yL; i <= yR; i++)
	{
		for (int j = xL; j <= xR; j++)
		{
			if (src.getPixel(i, j) < threshold)
				tgt.setPixel(i, j, MINRGB);
			else
				tgt.setPixel(i, j, MAXRGB);
		}
	}
}

/*-----------------------------------------------------------------------**/
// scale the image for ROI
void utility::scaleROI(image &src, image &tgt, int xL, int yL, int xR, int yR, float ratio)
{
	for (int i = yL; i < yR; i++)
	{
		for (int j = xL; j < xR; j++)
		{
			// make ROI black
			tgt.setPixel(i, j, 0);
			/* Map the pixel of new image back to original image */
			int i2 = xL + (int)floor((float)(i - xL) / ratio);
			int j2 = yL + (int)floor((float)(j - yL) / ratio);
			if (ratio == 2)
			{
				/* Directly copy the value */
				tgt.setPixel(i, j, checkValue(src.getPixel(i2, j2)));
			}
			if (ratio == 0.5)
			{
				/* Calculate the new image pixel coordinates */
				i2 = xL + (int)floor((float)(i - xL) * ratio);
				j2 = yL + (int)floor((float)(j - yL) * ratio);
				/* Average the values of four pixels of the original image*/
				int value = src.getPixel(i, j) + src.getPixel(i, j + 1) + src.getPixel(i + 1, j) + src.getPixel(i + 1, j + 1);
				tgt.setPixel(i2, j2, checkValue(value / 4));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
// Implement “area of interest” for ROI
void utility::AOIROI(image &src, image &tgt, int xL, int yL, int xR, int yR, int value)
{
	int xC = xL + (xR - xL) / 2;
	int yC = yL + (yR - yL) / 2;
	for (int i = yL; i <= yR; i++)
	{
		for (int j = xL; j <= xR; j++)
		{
			if (abs(i - xC) < 21)
			{
				tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + value * abs(i - xC) / 20));
			}
			else
			{
				tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
// Add color brightness modification for a ROI
void utility::cBrightROI(image &src, image &tgt, int xL, int yL, int xR, int yR, int DR, int DG, int DB)
{
	for (int i = yL; i <= yR; i++)
	{
		for (int j = xL; j <= xR; j++)
		{
			tgt.setPixel(i, j, RED, checkValue(src.getPixel(i, j, RED) + DR));
			tgt.setPixel(i, j, GREEN, checkValue(src.getPixel(i, j, GREEN) + DG));
			tgt.setPixel(i, j, BLUE, checkValue(src.getPixel(i, j, BLUE) + DB));
		}
	}
}

/*-----------------------------------------------------------------------**/
// Add color visualization for a ROI
void utility::cVisBrightROI(image &src, image &tgt, int xL, int yL, int xR, int yR, int V, int T)
{
	for (int i = yL; i <= yR; i++)
	{
		for (int j = xL; j <= xR; j++)
		{
			if (abs(V - src.getPixel(i, j)) < T)
			{
				tgt.setPixel(i, j, RED, checkValue(src.getPixel(i, j)));
				tgt.setPixel(i, j, GREEN, 0);
				tgt.setPixel(i, j, BLUE, 0);
			}
			else
			{
				tgt.setPixel(i, j, GREEN, checkValue(src.getPixel(i, j)));
				tgt.setPixel(i, j, RED, 0);
				tgt.setPixel(i, j, BLUE, 0);
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
// Check if ROI is in bounds
bool utility::isInboundsROI(image &src, int xR, int yR)
{
	if (!(src.isInbounds(yR, xR)))
	{
		std::cout << "ROI is out of bounds" << endl;
		std::cout << "x: " << src.getNumberOfColumns() << endl;
		std::cout << "y: " << src.getNumberOfRows() << endl;
		return false;
	}
	return true;
};

/*-----------------------------------------------------------------------**/
// Check if two ROIs are overlap
bool utility::overlapROi(int xL1, int yL1, int xR1, int yR1, int xL2, int yL2, int xR2, int yR2)
{
	bool XOverlap = false;
	bool YOverlap = false;

	if ((xL2 >= xL1 && xL2 <= xR1) || (xL1 >= xL2 && xL1 <= xR2))
	{
		XOverlap = true;
	}

	if ((yL2 >= yL1 && yL2 <= yR1) || (yL1 >= yL2 && yL1 <= yR2))
	{
		YOverlap = true;
	}

	if (XOverlap && YOverlap)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*-----------------------------------------------------------------------**/
// Implement ROIs with their operations
void utility::Rois(image &src, image &tgt, int roiNum, vector<vector<string>> rois, const char *outfileName)
{
	// checking rois vector
	//  for(auto i: rois)
	//  	for(auto j: i )
	//  	{
	//  		cout<<j<<"-";
	//  	}
	//  	cout<<endl;
	//  create image size
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	// render original image on target
	for (int i = 0; i < src.getNumberOfRows(); i++)
		for (int j = 0; j < src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i, j, RED, checkValue(src.getPixel(i, j, RED)));
			tgt.setPixel(i, j, GREEN, checkValue(src.getPixel(i, j, GREEN)));
			tgt.setPixel(i, j, BLUE, checkValue(src.getPixel(i, j, BLUE)));
		}

	// check if all ROIs are valid, it will discard the latest ROI that overlap
	for (int i = 0; i < roiNum; i++)
	{
		if (rois[i].empty() || !(isInboundsROI(src, stoi(rois[i][2]), stoi(rois[i][3]))))
		{
			rois[i].clear();
			continue;
		}
		for (int j = i + 1; j < roiNum; j++)
		{
			if (rois[j].empty() || !(isInboundsROI(src, stoi(rois[j][2]), stoi(rois[j][3]))))
			{
				rois[j].clear();
				continue;
			}

			if (overlapROi((stoi(rois[i][0])), (stoi(rois[i][1])), (stoi(rois[i][2])), (stoi(rois[i][3])),
						   (stoi(rois[j][0])), (stoi(rois[j][1])), (stoi(rois[j][2])), (stoi(rois[j][3]))))
			{
				rois[j].clear();
			}
		}
	}

	// Execute the operation for each ROI
	for (int i = 0; i < rois.size(); i++)
	{
		if (rois[i].size() == 0)
		{
			continue;
		}
		else
		{
			if (rois[i][4] == "add")
			{
				// 0: xL, 1: xR, 2: yL, 3: yR,  5: V
				addGreyROI(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]));
			}
			else if (rois[i][4] == "binarize")
			{
				// 5: T
				binarizeROI(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]));
			}
			else if (rois[i][4] == "scale")
			{
				// 5: ratio
				scaleROI(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stof(rois[i][5]));
			}
			else if (rois[i][4] == "aoi_bright")
			{
				// 5: value
				AOIROI(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]));
			}
			else if (rois[i][4] == "color_bright")
			{
				// 5:DR, 6:DG, 7:DB
				cBrightROI(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]), stoi(rois[i][6]), stoi(rois[i][7]));
			}
			else if (rois[i][4] == "color_vis")
			{
				// 5:V, 6:T
				cVisBrightROI(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]), stoi(rois[i][6]));
			}
			else if (rois[i][4] == "histo" || rois[i][4] == "histo\n")
			{
				// create histogram before processing
				char name[200] = "bef_";
				strcat(name, outfileName);
				histogram(src, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), NULL, name, i + 1);

				// call function to stretch histogram, no extra parameters
				histStretch(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), outfileName, i + 1);

				// create histogram after processing
				strcpy(name, "aft_");
				strcat(name, outfileName);
				histogram(tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), NULL, name, i + 1);
			}
			else if (rois[i][4] == "combohisto")
			{
				// 5: T1, 6: T2
				comboHisto(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]), stoi(rois[i][6]), outfileName, i + 1);
			}
			else if (rois[i][4] == "perchastretch")
			{
				// 5: RGB, 6: Imin, 7: Imax
				perChaStretch(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), rois[i][5][0], stoi(rois[i][6]), stoi(rois[i][7]), outfileName, i + 1);
			}
			else if (rois[i][4] == "rgbstretch")
			{
				// 5: Imin, 6: Imax
				rgbStretch(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]), stoi(rois[i][6]), outfileName, i + 1);
			}
			else if (rois[i][4] == "colorization")
			{
				// 5: t1, 6: t2
				colorization(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]), stoi(rois[i][6]));
			}
			else if (rois[i][4] == "tencolor" || rois[i][4] == "tencolor\n")
			{
				tenColor(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]));
			}
			else if (rois[i][4] == "binaryedge3")
			{
				// 5: threshold
				binaryEdge3(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]), outfileName, i + 1);
			}
			else if (rois[i][4] == "binaryedge5")
			{
				// 5: threshold
				binaryEdge5(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]), outfileName, i + 1);
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
// fucntion to make histogram for dark, medium, and bright sets
void utility::histogram2(image &src, vector<int> channel, vector<vector<int>> coordinates, const char *outfile, const int imgIndex)
{
	// create name for histogram
	image greyHistog;
	char greyHistogramName[200];
	char c = '0' + imgIndex;
	greyHistogramName[0] = c;
	greyHistogramName[1] = '\0';
	strcat(greyHistogramName, "_grey_histogram_");
	strcat(greyHistogramName, outfile);

	// histogram array
	int greyOneD[256] = {0};
	if (!channel.empty())
	{
		for (int i = 1; i < channel.size(); i++)
		{
			greyOneD[channel[i]] += 1;
		}
	}
	else
	{
		// build histogram array
		for (int i = 0; i < coordinates.size(); i++)
			greyOneD[checkValue(src.getPixel(coordinates[i][0], coordinates[i][1]))] += 1;
	}

	int greyHeight; // histogram max height
	int greyScale; // ratio for scaling down the height
	greyHeight = *max_element(greyOneD, greyOneD + 255);// calculate height
	greyScale = (int)ceil(((float)greyHeight / 256));// get ratio

	// eliminate 0 case
	if (greyScale == 0)
	{
		greyScale = 1;
	}

	// resize histogram img
	greyHistog.resize(256, 256);

	// draw histogram
	for (int i = 0; i < greyHistog.getNumberOfColumns() - 1; i++)
	{
		for (int j = 0; j < greyOneD[i] / greyScale; j++)
		{
			greyHistog.setPixel(255 - j, i, MAXRGB);
		}
	}
	greyHistog.save(greyHistogramName);
}

/*-----------------------------------------------------------------------**/
// fucntion to make histogram grey and RGB images
void utility::histogram(image &src, int xL, int yL, int xR, int yR, vector<vector<int>> *coordinates, const char *outfile, const int imgIndex)
{
	if (strstr(outfile, ".pgm") != NULL)
	{ /* PGM Gray-level Image */
		image greyHistog;
		//create histogram name
		char greyHistogramName[200];
		char c = '0' + imgIndex;
		greyHistogramName[0] = c;
		greyHistogramName[1] = '\0';
		strcat(greyHistogramName, "_grey_histogram_");
		strcat(greyHistogramName, outfile);
		cout << greyHistogramName << endl;
		
		// create histogram array
		int greyOneD[256] = {0};

		// build histogram array
		for (int i = yL; i <= yR; i++)
			for (int j = xL; j <= xR; j++)
			{
				greyOneD[checkValue(src.getPixel(i, j))] += 1;
			}
		int greyHeight;// histogram max height
		int greyScale;// ratio for scaling down the height
		greyHeight = *max_element(greyOneD, greyOneD + 255);// calculate height
		greyScale = (int)ceil(((float)greyHeight / 256));// get ratio
		
		// eliminate 0 case
		if (greyScale == 0)
		{
			greyScale = 1;
		}

		// resize histogram img
		greyHistog.resize(256, 256);

		// draw histogram
		for (int i = 0; i < greyHistog.getNumberOfColumns() - 1; i++)
		{
			for (int j = 0; j < greyOneD[i] / greyScale; j++)
			{
				greyHistog.setPixel(255 - j, i, MAXRGB);
			}
		}
		greyHistog.save(greyHistogramName);
	}

	// repeat the same process for RGB image
	else if (strstr(outfile, ".ppm") != NULL)
	{ /* PPM Color Image */

		image rHistog, bHistog, gHistog;
		// make histograms' names
		char rHistogramName[200];
		char bHistogramName[200];
		char gHistogramName[200];
		char c = '0' + imgIndex;

		rHistogramName[0] = c;
		bHistogramName[0] = c;
		gHistogramName[0] = c;
		rHistogramName[1] = '\0';
		bHistogramName[1] = '\0';
		gHistogramName[1] = '\0';

		strcat(rHistogramName, "_red_histogram_");
		strcat(rHistogramName, outfile);
		strcat(bHistogramName, "_blue_histogram_");
		strcat(bHistogramName, outfile);
		strcat(gHistogramName, "_green_histogram_");
		strcat(gHistogramName, outfile);
		

		cout <<"histogram name: " << rHistogramName << endl;
		cout <<"histogram name: " << bHistogramName << endl;
		cout <<"histogram name: " << gHistogramName << endl;
		
		// make histograms
		int redOneD[256] = {0};
		int blueOneD[256] = {0};
		int greenOneD[256] = {0};

		// store number of pixelss
		for (int i = yL; i <= yR; i++)
			for (int j = xL; j <= xR; j++)
			{
				redOneD[checkValue(src.getPixel(i, j, RED))] += 1;
				blueOneD[checkValue(src.getPixel(i, j, BLUE))] += 1;
				greenOneD[checkValue(src.getPixel(i, j, GREEN))] += 1;
			}

		// maximum height of each rgb
		int redHeight, blueHeight, greenHeight;
		int redScale, blueScale, greenScale;
		redHeight = *max_element(redOneD, redOneD + 255);
		redScale = (int)ceil(((float)redHeight / 256));
		if (redScale == 0)
		{
			redScale = 1;
		}
		blueHeight = *max_element(blueOneD, blueOneD + 255);
		blueScale = (int)ceil(((float)blueHeight / 256));
		if (blueScale == 0)
		{
			blueScale = 1;
		}
		greenHeight = *max_element(greenOneD, greenOneD + 255);
		greenScale = (int)ceil(((float)greenHeight / 256));
		if (greenScale == 0)
		{
			greenScale = 1;
		}

		rHistog.resize(256, 256);
		bHistog.resize(256, 256);
		gHistog.resize(256, 256);

		// draw histograms
		// exclude col[255]
		for (int i = 0; i < rHistog.getNumberOfColumns() - 1; i++)
		{
			if (redOneD[i] / redScale > 256)
			{
				cout << "rows " << redOneD[i] / redScale << endl;
				cout << "at col " << i << endl;

				cout << "max height" << redHeight << endl;
				cout << "scale" << redScale << "-heigt: " << redOneD[i] << endl;
				break;
			}

			for (int j = 0; j < redOneD[i] / redScale; j++)
			{
				rHistog.setPixel(255 - j, i, RED, MAXRGB);
				// bHistog.setPixel(256 - j, i, BLUE, MINRGB);
				// bHistog.setPixel(256 - j, i, GREEN, MINRGB);
			}
		}
		cout << "debug" << endl;

		for (int i = 0; i < bHistog.getNumberOfColumns() - 1; i++)
		{
			for (int j = 0; j < blueOneD[i] / blueScale; j++)
			{
				bHistog.setPixel(255 - j, i, BLUE, MAXRGB);
				// bHistog.setPixel(256 - j, i, RED, MINRGB);
				// bHistog.setPixel(256 - j, i, GREEN, MINRGB);
			}
		}

		for (int i = 0; i < gHistog.getNumberOfColumns() - 1; i++)
		{
			for (int j = 0; j < greenOneD[i] / greenScale; j++)
			{
				gHistog.setPixel(255 - j, i, GREEN, MAXRGB);
				// bHistog.setPixel(256 - j, i, RED, MINRGB);
				// bHistog.setPixel(256 - j, i, BLUE, MINRGB);
			}
		}

		rHistog.save(rHistogramName);
		bHistog.save(bHistogramName);
		gHistog.save(gHistogramName);
	}
}

/*-----------------------------------------------------------------------**/
// stretching histogram to range o - 255
void utility::histStretch(image &src, image &tgt, int xL, int yL, int xR, int yR, const char *histoName, const int roiIndex)
{

	int inMin, inMax;
	// find min and max intensity
	inMin = checkValue(src.getPixel(yL, xL, RED));
	inMax = checkValue(src.getPixel(yL, xL, RED));
	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			if (inMin > checkValue(src.getPixel(i, j, RED)))
				inMin = checkValue(src.getPixel(i, j, RED));
			if (inMax < checkValue(src.getPixel(i, j, RED)))
				inMax = checkValue(src.getPixel(i, j, RED));
		}
	// cout << "min bef " << inMin << endl;
	inMin = 1.05 * inMin;
	// cout << "min aft " << inMin << endl;
	// cout << "max bef " << inMax << endl;
	inMax = 0.95 * inMax;
	// cout << "max aft " << inMax << endl;

	// stretching equation
	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			tgt.setPixel(i, j, checkValue((src.getPixel(i, j) - inMin) * (255 / (inMax - inMin))));
		}

	// check max min pixels
	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			if (inMin > checkValue(tgt.getPixel(i, j, RED)))
				inMin = checkValue(tgt.getPixel(i, j, RED));
			if (inMax < checkValue(tgt.getPixel(i, j, RED)))
				inMax = checkValue(tgt.getPixel(i, j, RED));
		}
	// cout << "min check " << inMin << endl;
	// cout << "max check " << inMax << endl;

}

/*-----------------------------------------------------------------------**/
// stretch gray scale image using 2 thresholds
void utility::comboHisto(image &src, image &tgt, int xL, int yL, int xR, int yR, int threshold1, int threshold2, const char *outfile, const int imgIndex)
{
	// vector for coordinate of pixels: [[y,x] , [...] , ...]
	vector<vector<int>> dark;
	vector<vector<int>> medium;
	vector<vector<int>> bright;

	// categorizing
	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			if (src.getPixel(i, j) < threshold1)
				dark.push_back({i, j});
			else if (src.getPixel(i, j) > threshold2)
				bright.push_back({i, j});
			else
				medium.push_back({i, j});
		}
	
	// call func to make histogram
	char name[200] = "bef_dark_";
	strcat(name, outfile);
	histogram2(src, {}, dark, name, imgIndex);

	strcpy(name, "bef_medium_");
	strcat(name, outfile);
	histogram2(src, {}, medium, name, imgIndex);

	strcpy(name, "bef_bright_");
	strcat(name, outfile);
	histogram2(src, {}, bright, name, imgIndex);

	// dark max and min
	int inMin, inMax;
	if (!dark.empty())
	{
		inMin = checkValue(src.getPixel(dark[0][0], dark[0][1]));
		inMax = checkValue(src.getPixel(dark[0][0], dark[0][1]));
	}

	for (int i = 1; i < dark.size(); i++)
	{
		if (inMin > checkValue(src.getPixel(dark[i][0], dark[i][1])))
			inMin = checkValue(src.getPixel(dark[i][0], dark[i][1]));
		if (inMax < checkValue(src.getPixel(dark[i][0], dark[i][1])))
			inMax = checkValue(src.getPixel(dark[i][0], dark[i][1]));
	}
	inMin = 1.05 * inMin;
	inMax = 0.95 * inMax;
	// dark stretch
	for (int i = 1; i < dark.size(); i++)
		tgt.setPixel(dark[i][0], dark[i][1], checkValue((src.getPixel(dark[i][0], dark[i][1]) - inMin) * ((threshold2 - threshold1) / (inMax - inMin)) + threshold1));

	// medium max min
	if (!medium.empty())
	{
		inMin = checkValue(src.getPixel(medium[0][0], medium[0][1]));
		inMax = checkValue(src.getPixel(medium[0][0], medium[0][1]));
	}
	for (int i = 1; i < medium.size(); i++)
	{
		if (inMin > checkValue(src.getPixel(medium[i][0], medium[i][1])))
			inMin = checkValue(src.getPixel(medium[i][0], medium[i][1]));
		if (inMax < checkValue(src.getPixel(medium[i][0], medium[i][1])))
			inMax = checkValue(src.getPixel(medium[i][0], medium[i][1]));
	}
	inMin = 1.05 * inMin;
	inMax = 0.95 * inMax;
	// medium stretch
	for (int i = 1; i < medium.size(); i++)
		tgt.setPixel(medium[i][0], medium[i][1], checkValue((src.getPixel(medium[i][0], medium[i][1]) - inMin) * ((threshold2 - threshold1) / (inMax - inMin)) + threshold1));

	// bright max min
	if (!bright.empty())
	{
		inMin = checkValue(src.getPixel(bright[0][0], bright[0][1]));
		inMax = checkValue(src.getPixel(bright[0][0], bright[0][1]));
	}

	for (int i = 1; i < bright.size(); i++)
	{
		if (inMin > checkValue(src.getPixel(bright[i][0], bright[i][1])))
			inMin = checkValue(src.getPixel(bright[i][0], bright[i][1]));
		if (inMax < checkValue(src.getPixel(bright[i][0], bright[i][1])))
			inMax = checkValue(src.getPixel(bright[i][0], bright[i][1]));
	}
	inMin = 1.05 * inMin;
	inMax = 0.95 * inMax;
	// bright stretch
	for (int i = 1; i < bright.size(); i++)
		tgt.setPixel(bright[i][0], bright[i][1], checkValue((src.getPixel(bright[i][0], bright[i][1]) - inMin) * ((threshold2 - threshold1) / (inMax - inMin)) + threshold1));

	// make histogram after stretching
	strcpy(name, "aft_dark_");
	strcat(name, outfile);
	histogram2(tgt, {}, dark, name, imgIndex);

	strcpy(name, "aft_medium_");
	strcat(name, outfile);
	histogram2(tgt, {}, medium, name, imgIndex);

	strcpy(name, "aft_bright_");
	strcat(name, outfile);
	histogram2(tgt, {}, bright, name, imgIndex);

	// cout << "debug" << endl;
}

/*-----------------------------------------------------------------------**/
// stretch individual RGB channel based on thresholds
void utility::perChaStretch(image &src, image &tgt, int xL, int yL, int xR, int yR, char RGB, int Imin, int Imax, const char *histoName, const int roiIndex)
{
	// create histogram
	char name[200] = "bef_";
	strcat(name, histoName);
	histogram(src, xL, yL, xR, yR, NULL, name, roiIndex);

	// cout << "find" << endl;
	// find max, min intensity
	int inMin, inMax;

	switch (RGB)
	{
	case 'R':
		// find intensity max intensity min
		inMin = checkValue(src.getPixel(yL, xL, RED));
		inMax = checkValue(src.getPixel(yL, xL, RED));
		for (int i = yL; i <= yR; i++)
			for (int j = xL; j <= xR; j++)
			{
				if (inMin > checkValue(src.getPixel(i, j, RED)))
					inMin = checkValue(src.getPixel(i, j, RED));
				if (inMax < checkValue(src.getPixel(i, j, RED)))
					inMax = checkValue(src.getPixel(i, j, RED));
			}
		inMin = 1.05 * inMin;
		inMax = 0.95 * inMax;

		// stretch the channel
		for (int i = yL; i <= yR; i++)
			for (int j = xL; j <= xR; j++)
			{
				tgt.setPixel(i, j, RED, checkValue((src.getPixel(i, j, RED) - inMin) * ((Imax - Imin) / (inMax - inMin)) + Imin));
			}
		break;
	case 'B':
		inMin = checkValue(src.getPixel(yL, xL, BLUE));
		inMax = checkValue(src.getPixel(yL, xL, BLUE));
		for (int i = yL; i <= yR; i++)
			for (int j = xL; j <= xR; j++)
			{
				if (inMin > checkValue(src.getPixel(i, j, BLUE)))
					inMin = checkValue(src.getPixel(i, j, BLUE));
				if (inMax < checkValue(src.getPixel(i, j, BLUE)))
					inMax = checkValue(src.getPixel(i, j, BLUE));
			}
		inMin = 1.05 * inMin;
		inMax = 0.95 * inMax;
		
		// stretch the channel
		for (int i = yL; i <= yR; i++)
			for (int j = xL; j <= xR; j++)
			{
				tgt.setPixel(i, j, BLUE, checkValue((src.getPixel(i, j, BLUE) - inMin) * ((Imax - Imin) / (inMax - inMin)) + Imin));
			}
		break;
	case 'G':
		inMin = checkValue(src.getPixel(yL, xL, GREEN));
		inMax = checkValue(src.getPixel(yL, xL, GREEN));
		for (int i = yL; i <= yR; i++)
			for (int j = xL; j <= xR; j++)
			{
				if (inMin > checkValue(src.getPixel(i, j, GREEN)))
					inMin = checkValue(src.getPixel(i, j, GREEN));
				if (inMax < checkValue(src.getPixel(i, j, GREEN)))
					inMax = checkValue(src.getPixel(i, j, GREEN));
			}
		inMin = 1.05 * inMin;
		inMax = 0.95 * inMax;
		
		// stretch the channel
		for (int i = yL; i <= yR; i++)
			for (int j = xL; j <= xR; j++)
			{
				tgt.setPixel(i, j, GREEN, checkValue((src.getPixel(i, j, GREEN) - inMin) * ((Imax - Imin) / (inMax - inMin)) + Imin));
			}
		break;
	}

	// create after histogram
	strcpy(name, "aft_");
	strcat(name, histoName);
	histogram(tgt, xL, yL, xR, yR, NULL, name, roiIndex);
}

/*-----------------------------------------------------------------------**/
// stretch ROI on all RGB using thresholds
void utility::rgbStretch(image &src, image &tgt, int xL, int yL, int xR, int yR, int Imin, int Imax, const char *histoName, const int roiIndex)
{
	// create histograms
	char name[200] = "bef_";
	strcat(name, histoName);
	histogram(src, xL, yL, xR, yR, NULL, name, roiIndex);

	// cout << "find" << endl;

	// find intensity max and min
	int inMin, inMax;
	inMin = checkValue(src.getPixel(yL, xL, RED));
	inMax = checkValue(src.getPixel(yL, xL, RED));
	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			if (inMin > checkValue(src.getPixel(i, j, RED)))
				inMin = checkValue(src.getPixel(i, j, RED));
			if (inMax < checkValue(src.getPixel(i, j, RED)))
				inMax = checkValue(src.getPixel(i, j, RED));
		}
	inMin = 1.05 * inMin;
	inMax = 0.95 * inMax;

	// red stretching
	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			// if (src.getPixel(i, j) < inMin)
			// 	tgt.setPixel(i, j, RED, MINRGB);
			// else if (src.getPixel(i, j) > inMax)
			// 	tgt.setPixel(i, j, RED, MAXRGB);
			// else
				tgt.setPixel(i, j, RED, checkValue((src.getPixel(i, j, RED) - inMin) * ((Imax - Imin) / (inMax - inMin)) + Imin));
		}

	// blue stretching
	inMin = checkValue(src.getPixel(yL, xL, BLUE));
	inMax = checkValue(src.getPixel(yL, xL, BLUE));
	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			if (inMin > checkValue(src.getPixel(i, j, BLUE)))
				inMin = checkValue(src.getPixel(i, j, BLUE));
			if (inMax < checkValue(src.getPixel(i, j, BLUE)))
				inMax = checkValue(src.getPixel(i, j, BLUE));
		}
	inMin = 1.05 * inMin;
	inMax = 0.95 * inMax;

	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			// if (src.getPixel(i, j, BLUE) < inMin)
			// 	tgt.setPixel(i, j, BLUE, MINRGB);
			// else if (src.getPixel(i, j, BLUE) > inMax)
			// 	tgt.setPixel(i, j, BLUE, MAXRGB);
			// else
				tgt.setPixel(i, j, BLUE, checkValue((src.getPixel(i, j, BLUE) - inMin) * ((Imax - Imin) / (inMax - inMin)) + Imin));
		}

	// green stretching
	inMin = checkValue(src.getPixel(yL, xL, GREEN));
	inMax = checkValue(src.getPixel(yL, xL, GREEN));
	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			if (inMin > checkValue(src.getPixel(i, j, GREEN)))
				inMin = checkValue(src.getPixel(i, j, GREEN));
			if (inMax < checkValue(src.getPixel(i, j, GREEN)))
				inMax = checkValue(src.getPixel(i, j, GREEN));
		}
	inMin = 1.05 * inMin;
	inMax = 0.95 * inMax;

	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			// if (src.getPixel(i, j, GREEN) < inMin)
			// 	tgt.setPixel(i, j, GREEN, MINRGB);
			// else if (src.getPixel(i, j, GREEN) > inMax)
			// 	tgt.setPixel(i, j, GREEN, MAXRGB);
			// else
				tgt.setPixel(i, j, GREEN, checkValue((src.getPixel(i, j, GREEN) - inMin) * ((Imax - Imin) / (inMax - inMin)) + Imin));
		}

	strcpy(name, "aft_");
	strcat(name, histoName);
	histogram(tgt, xL, yL, xR, yR, NULL, name, roiIndex);
}

/*-----------------------------------------------------------------------**/
// colorize gray scale image using thresholds
void utility::colorization(image &src, image &tgt, int xL, int yL, int xR, int yR, int t1, int t2)
{
	// cout << t1 << "-" << t2 << endl;

	// loop and colorize the image
	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			if (src.getPixel(i, j) < t1)
			{
				tgt.setPixel(i, j, RED, checkValue(src.getPixel(i, j)));
				tgt.setPixel(i, j, BLUE, MINRGB);
				tgt.setPixel(i, j, GREEN, MINRGB);
			}
			else if (src.getPixel(i, j) > t2)
			{
				tgt.setPixel(i, j, BLUE, checkValue(src.getPixel(i, j)));
				tgt.setPixel(i, j, RED, MINRGB);
				tgt.setPixel(i, j, GREEN, MINRGB);
			}
			else
			{
				tgt.setPixel(i, j, GREEN, checkValue(src.getPixel(i, j)));
				tgt.setPixel(i, j, RED, MINRGB);
				tgt.setPixel(i, j, BLUE, MINRGB);
			}
		}
}
/*-----------------------------------------------------------------------**/
// extra credit
void utility::tenColor(image &src, image &tgt, int xL, int yL, int xR, int yR)
{
	int inMin, inMax;
	inMin = checkValue(src.getPixel(yL, xL, RED));
	inMax = checkValue(src.getPixel(yL, xL, RED));
	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			if (inMin > checkValue(src.getPixel(i, j, RED)))
				inMin = checkValue(src.getPixel(i, j, RED));
			if (inMax < checkValue(src.getPixel(i, j, RED)))
				inMax = checkValue(src.getPixel(i, j, RED));
		}
	int range;
	range = floor((float)(inMax - inMin) / 10);
	for (int i = yL; i <= yR; i++)
		for (int j = xL; j <= xR; j++)
		{
			if (src.getPixel(i,j) >=  inMin+range*0 && src.getPixel(i,j) < inMin + range * 1)
			{
				tgt.setPixel(i,j,RED,MAXRGB);
			}
			else if(src.getPixel(i,j) >=  inMin+range*1 && src.getPixel(i,j) < inMin + range * 2)
			{
				tgt.setPixel(i,j,BLUE,MAXRGB);
			}
			else if(src.getPixel(i,j) >=  inMin+range*2 && src.getPixel(i,j) < inMin + range * 3)
			{
				tgt.setPixel(i,j,GREEN,MAXRGB);
			}
			else if(src.getPixel(i,j) >=  inMin+range*3 && src.getPixel(i,j) < inMin + range * 4)
			{
				tgt.setPixel(i,j,RED,MAXRGB);
				tgt.setPixel(i,j,BLUE,MAXRGB);
			}
			else if(src.getPixel(i,j) >=  inMin+range*4 && src.getPixel(i,j) < inMin + range * 5)
			{
				tgt.setPixel(i,j,RED,MAXRGB);
				tgt.setPixel(i,j,GREEN,MAXRGB);
			}
			else if(src.getPixel(i,j) >=  inMin+range*4 && src.getPixel(i,j) < inMin + range * 5)
			{
				tgt.setPixel(i,j,BLUE,MAXRGB);
				tgt.setPixel(i,j,GREEN,MAXRGB);
			}
			else if(src.getPixel(i,j) >=  inMin+range*5 && src.getPixel(i,j) < inMin + range * 6)
			{
				tgt.setPixel(i,j,RED,MAXRGB);
				tgt.setPixel(i,j,BLUE,MAXRGB);
				tgt.setPixel(i,j,GREEN,MAXRGB);
			}
			else if(src.getPixel(i,j) >=  inMin+range*6 && src.getPixel(i,j) < inMin + range * 7)
			{
				tgt.setPixel(i,j,RED,100);
				tgt.setPixel(i,j,BLUE,100);
				tgt.setPixel(i,j,GREEN,100);
			}
			else if(src.getPixel(i,j) >=  inMin+range*7 && src.getPixel(i,j) < inMin + range * 8)
			{
				tgt.setPixel(i,j,RED,200);
				tgt.setPixel(i,j,BLUE,100);
				tgt.setPixel(i,j,GREEN,200);
			}
			else if(src.getPixel(i,j) >=  inMin+range*8 && src.getPixel(i,j) < inMin + range * 9)
			{
				tgt.setPixel(i,j,RED,10);
				tgt.setPixel(i,j,BLUE,150);
				tgt.setPixel(i,j,GREEN,200);
			}
			else if(src.getPixel(i,j) >=  inMin+range*9 && src.getPixel(i,j) <= inMin + range * 10)
			{
				tgt.setPixel(i,j,RED,MINRGB);
				tgt.setPixel(i,j,BLUE,MINRGB);
				tgt.setPixel(i,j,GREEN,MINRGB);
			}
		}
}

/*-----------------------------------------------------------------------**/
void utility::binaryEdge3(image &src, image &tgt, int xL, int yL, int xR, int yR, int threshold, const char *outfile, const int imgIndex)
{
	char gradientName[200];
	char c = '0' + imgIndex;
	gradientName[0] = c;
	gradientName[1] = '\0';
	strcat(gradientName, "_gradient_");
	strcat(gradientName, outfile);
	
	char stretchName[200];
	stretchName[0] = c;
	stretchName[1] = '\0';
	strcat(stretchName, "_gradient_stretch_");
	strcat(stretchName, outfile);

	// create image to store sobel 3x3
	// int sobelDX[src.getNumberOfRows() * src.getNumberOfColumns()];
	// int sobelDy[src.getNumberOfRows() * src.getNumberOfColumns()];

	int sobelDX;
	int sobelDy;
	image gradient;
	image stretch;
	image padding;

	// sobelDX.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	// sobelDy.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	gradient.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	stretch.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	padding.resize(src.getNumberOfRows() + 2, src.getNumberOfColumns() + 2);

	for (int i = 0; i < src.getNumberOfRows(); i++)
		for(int j = 0; j < src.getNumberOfColumns(); j++)
		{
			padding.setPixel(i + 1, j + 1, checkValue(src.getPixel(i,j)));
		}
	


	// sobel 3x3 kernel dx, dy
	int kernel3Dx[9] ={-1, 0, 1, -2, 0, 2, -1, 0, 1};
	int kernel3Dy[9] ={1, 2, 1, 0, 0, 0, -1, -2, -1};

	for (int i = yL; i <= yR; i++)
		for(int j = xL; j <= xR; j++)
		{
			sobelDX = padding.getPixel(i-1, j-1) * kernel3Dx[0] + padding.getPixel(i-1,j) *kernel3Dx[1] + padding.getPixel(i-1,j+1)*kernel3Dx[2] +
				padding.getPixel(i, j-1) *kernel3Dx[3] + padding.getPixel(i, j)*kernel3Dx[4]+ padding.getPixel(i, j+1)*kernel3Dx[5] +
				padding.getPixel(i+1, j-1)*kernel3Dx[6] + padding.getPixel(i+1,j)*kernel3Dx[7] + padding.getPixel(i+1,j+1)*kernel3Dx[8];
			
			sobelDy = padding.getPixel(i-1, j-1) * kernel3Dy[0] + padding.getPixel(i-1,j) *kernel3Dy[1] + padding.getPixel(i-1,j+1)*kernel3Dy[2] +
				padding.getPixel(i, j-1) *kernel3Dy[3] + padding.getPixel(i, j)*kernel3Dy[4]+ padding.getPixel(i, j+1)*kernel3Dy[5] +
				padding.getPixel(i+1, j-1)*kernel3Dy[6] + padding.getPixel(i+1,j)*kernel3Dy[7] + padding.getPixel(i+1,j+1)*kernel3Dy[8];
			// sobelDy.setPixel(i, j, checkValue(src.getPixel(i, j) * kernel3Dy[(i%3)*3 + (j%3)] ));
			tgt.setPixel(i, j, checkValue(sqrt( pow(sobelDX,2) + pow(sobelDy,2) )));
		}
	
		char name[200] = "bef_";
	strcat(name, outfile);
	histogram(tgt, xL, yL, xR, yR, NULL, name, imgIndex);

	histStretch(tgt, tgt, xL, yL, xR, yR, outfile, imgIndex);

	strcpy(name, "aft_");
	strcat(name, outfile);
	histogram(tgt, xL, yL, xR, yR, NULL, name, imgIndex);


	for (int i = yL; i <= yR; i++)
		for(int j = xL; j <= xR; j++)
			if(tgt.getPixel(i, j) < threshold)
			{
				tgt.setPixel(i, j, MINRGB);
			}
			else{
				tgt.setPixel(i, j, MAXRGB);
			}
}

/*-----------------------------------------------------------------------**/
void utility::binaryEdge5(image &src, image &tgt, int xL, int yL, int xR, int yR, int threshold, const char *outfile, const int imgIndex)
{
	char gradientName[200];
	char c = '0' + imgIndex;
	gradientName[0] = c;
	gradientName[1] = '\0';
	strcat(gradientName, "_gradient_");
	strcat(gradientName, outfile);
	
	char stretchName[200];
	stretchName[0] = c;
	stretchName[1] = '\0';
	strcat(stretchName, "_gradient_stretch_");
	strcat(stretchName, outfile);


	// create image to store sobel 5x5
	// int sobelDX[src.getNumberOfRows() * src.getNumberOfColumns()];
	// int sobelDy[src.getNumberOfRows() * src.getNumberOfColumns()];

	int sobelDX;
	int sobelDy;
	image gradient;
	image stretch;
	image padding;

	// sobelDX.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	// sobelDy.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	gradient.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	stretch.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	padding.resize(src.getNumberOfRows() + 4, src.getNumberOfColumns() + 4);

	for (int i = 0; i < src.getNumberOfRows(); i++)
		for(int j = 0; j < src.getNumberOfColumns(); j++)
		{
			padding.setPixel(i + 2, j + 2, checkValue(src.getPixel(i,j)));
		}
	


	// sobel 5x5 kernel dx, dy
	int kernel5Dx[25] ={-5, -4, 0, 4, 5,
						-8, -10, 0, 10, 8,
						-10, -20, 0, 20, 10,
						-8, -10, 0, 10, 8,
						-5, -4, 0, 4, 5};
	int kernel5Dy[25] ={-5, -8, -10, -8, -5,
						-4, -10, -20, -10, -4,
						0, 0, 0, 0, 0,
						4, 10, 20, 10, 4,
						5, 8, 10, 8, 5};

	for (int i = yL; i <= yR; i++)
		for(int j = xL; j <= xR; j++)
		{
			sobelDX = padding.getPixel(i-2, j-2)*kernel5Dx[0] + padding.getPixel(i-2, j-1)*kernel5Dx[1] + padding.getPixel(i-2,j)*kernel5Dx[2] + padding.getPixel(i-2,j+1)*kernel5Dx[3] + padding.getPixel(i-2,j+2)*kernel5Dx[4]+
			padding.getPixel(i-1,j-2)*kernel5Dx[5] + padding.getPixel(i-1, j-1) * kernel5Dx[6] + padding.getPixel(i-1,j) *kernel5Dx[7] + padding.getPixel(i-1,j+1)*kernel5Dx[8] + padding.getPixel(i-1,j+2)*kernel5Dx[9]+
			padding.getPixel(i,j-2)*kernel5Dx[10] + padding.getPixel(i, j-1) *kernel5Dx[11] + padding.getPixel(i, j)*kernel5Dx[12]+ padding.getPixel(i, j+1)*kernel5Dx[13] + padding.getPixel(i,j+2)*kernel5Dx[14]+
			padding.getPixel(i+1,j-2)*kernel5Dx[15] + padding.getPixel(i+1, j-1)*kernel5Dx[16] + padding.getPixel(i+1,j)*kernel5Dx[17] + padding.getPixel(i+1,j+1)*kernel5Dx[18] + padding.getPixel(i+1,j+2)*kernel5Dx[19]+
			padding.getPixel(i+2,j-2)*kernel5Dx[20] + padding.getPixel(i+2,j-1)*kernel5Dx[21] + padding.getPixel(i+2,j)*kernel5Dx[22] + padding.getPixel(i+2,j+1)*kernel5Dx[23] + padding.getPixel(i+2,j+2)*kernel5Dx[24];
			
			sobelDy = padding.getPixel(i-2, j-2)*kernel5Dy[0] + padding.getPixel(i-2, j-1)*kernel5Dy[1] + padding.getPixel(i-2,j)*kernel5Dy[2] + padding.getPixel(i-2,j+1)*kernel5Dy[3] + padding.getPixel(i-2,j+2)*kernel5Dy[4]+
			padding.getPixel(i-1,j-2)*kernel5Dy[5] + padding.getPixel(i-1, j-1) * kernel5Dy[6] + padding.getPixel(i-1,j) *kernel5Dy[7] + padding.getPixel(i-1,j+1)*kernel5Dy[8] + padding.getPixel(i-1,j+2)*kernel5Dy[9]+
			padding.getPixel(i,j-2)*kernel5Dy[10] + padding.getPixel(i, j-1) *kernel5Dy[11] + padding.getPixel(i, j)*kernel5Dy[12]+ padding.getPixel(i, j+1)*kernel5Dy[13] + padding.getPixel(i,j+2)*kernel5Dy[14]+
			padding.getPixel(i+1,j-2)*kernel5Dy[15] + padding.getPixel(i+1, j-1)*kernel5Dy[16] + padding.getPixel(i+1,j)*kernel5Dy[17] + padding.getPixel(i+1,j+1)*kernel5Dy[18] + padding.getPixel(i+1,j+2)*kernel5Dy[19]+
			padding.getPixel(i+2,j-2)*kernel5Dy[20] + padding.getPixel(i+2,j-1)*kernel5Dy[21] + padding.getPixel(i+2,j)*kernel5Dy[22] + padding.getPixel(i+2,j+1)*kernel5Dy[23] + padding.getPixel(i+2,j+2)*kernel5Dx[24];
			// sobelDy.setPixel(i, j, checkValue(src.getPixel(i, j) * kernel3Dy[(i%3)*3 + (j%3)] ));
			tgt.setPixel(i-2, j-2, checkValue(sqrt( pow(sobelDX,2) + pow(sobelDy,2) )));
		}
	
	char name[200] = "bef_";
	strcat(name, outfile);
	histogram(tgt, xL, yL, xR, yR, NULL, name, imgIndex);

	histStretch(tgt, tgt, xL, yL, xR, yR, outfile, imgIndex);

	strcpy(name, "aft_");
	strcat(name, outfile);
	histogram(tgt, xL, yL, xR, yR, NULL, name, imgIndex);
	
	for (int i = yL; i <= yR; i++)
		for(int j = xL; j <= xR; j++)
			if(tgt.getPixel(i, j) < threshold)
			{
				tgt.setPixel(i, j, MINRGB);
			}
			else{
				tgt.setPixel(i, j, MAXRGB);
			}
}

/*-----------------------------------------------------------------------**/
// OpenCV image bounds checking
bool utility::cv_isInboundsROI(cv::Mat &src, int xR, int yR)
{
	if ((yR < 0) || (xR < 0) || (xR >= src.cols) || (yR >= src.rows))
	{	
		std::cout << "ROI is out of bounds" << endl;
		std::cout << "x: " << src.cols << endl;
		std::cout << "y: " << src.rows << endl;
		return false;
	}
	else 
		return true;
}

/*-----------------------------------------------------------------------**/
// OpenCV ROIs checking
void utility::cv_Rois(cv::Mat &src, cv::Mat &tgt, int roiNum, vector< vector <string> > rois, const char *outfileName)
{
	tgt = src.clone();

	for (int i = 0; i < roiNum; i++)
	{
		if (rois[i].empty() || !(cv_isInboundsROI(src, stoi(rois[i][2]), stoi(rois[i][3]))))
		{
			rois[i].clear();
			continue;
		}
		for (int j = i + 1; j < roiNum; j++)
		{
			if (rois[j].empty() || !(cv_isInboundsROI(src, stoi(rois[j][2]), stoi(rois[j][3]))))
			{
				rois[j].clear();
				continue;
			}

			if (overlapROi((stoi(rois[i][0])), (stoi(rois[i][1])), (stoi(rois[i][2])), (stoi(rois[i][3])),
						   (stoi(rois[j][0])), (stoi(rois[j][1])), (stoi(rois[j][2])), (stoi(rois[j][3]))))
			{
				rois[j].clear();
			}
		}
	}
	cout<<"run\n";
	for (int i = 0; i < rois.size(); i++)
	{
		if (rois[i].size() == 0)
		{
			continue;
		}
		else
		{
			if (rois[i][4] == "sobel3" || rois[i][4] == "sobel3\n")
			{
							cout<<"run"<<endl;
				
				// 0: xL, 1: xR, 2: yL, 3: yR,  kernelSize
				cv_sobel(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), 3);
			}
			else if (rois[i][4] == "sobel5" || rois[i][4] == "sobel5\n")
			{
							cout<<"run"<<endl;

				// 0: xL, 1: xR, 2: yL, 3: yR,  kernelSize
				cv_sobel(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), 5);
			}
			else if (rois[i][4] == "gausobel5")
			{
				// 0: xL, 1: xR, 2: yL, 3: yR,  kernelSize, 5: gauSize, 6: sigmaX, 7:sigmaY
				cv_gausobel(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), 5, stoi(rois[i][5]), stod(rois[i][6]), stod(rois[i][7]));
			}
			else if (rois[i][4] == "otsu" || rois[i][4] == "otsu\n")
			{
				// 0: xL, 1: xR, 2: yL, 3: yR
				cv_otsu(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]));
			}
			else if (rois[i][4] == "canny")
			{
				// 0: xL, 1: xR, 2: yL, 3: yR, 5: threshold1, 6: threshold2
				cv_canny(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stod(rois[i][5]), stod(rois[i][6]));
			}
			else if (rois[i][4] == "histostrech" || rois[i][4] == "histostrech\n")
			{
				// 0: xL, 1: xR, 2: yL, 3: yR
				cout<<"run\n";
				cv_histostretch(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), outfileName, i+1);
			}
			else if (rois[i][4] == "histoequ" || rois[i][4] == "histoequ\n")
			{
				// 0: xL, 1: xR, 2: yL, 3: yR
				cout<<"run\n";
				cv_histoequ(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), outfileName, i+1);
			}
			else if (rois[i][4] == "houghcir")
			{
				// 0: xL, 1: xR, 2: yL, 3: yR, 5: gauSize, 6: sigmaX, 7:sigmaY, 8: threshold1, 9: threshold2, 10: minDist, 11: minR; 12: maxR
				cout<<"run houghcir\n";
				cv_houghcir(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]), stod(rois[i][6]), stod(rois[i][7]), stod(rois[i][8]), stod(rois[i][9]), stoi(rois[i][10]), stoi(rois[i][11]), stoi(rois[i][12]));
			}
			else if (rois[i][4] == "otsuhisto" || rois[i][4] == "otsuhisto\n")
			{
				// 0: xL, 1: xR, 2: yL, 3: yR
				cout<<"run otsuhisto\n";
				cv_otsuhisto(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), outfileName, i+1);
			}
			else if (rois[i][4] == "otsugau")
			{
				// 0: xL, 1: xR, 2: yL, 3: yR, 5: gauSize, 6: sigmaX, 7:sigmaY
				cout<<"run otsugau\n";
				cv_otsugau(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]), stod(rois[i][6]), stod(rois[i][7]));
			}
			else if (rois[i][4] == "combine")
			{
				// 0: xL, 1: xR, 2: yL, 3: yR, 5: gauSize, 6: sigmaX, 7:sigmaY
				cout<<"run combine\n";
				cv_combine(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), stoi(rois[i][5]), stod(rois[i][6]), stod(rois[i][7]), outfileName, i+1);
			}
			else if (rois[i][4] == "subtract" || rois[i][4] == "subtract\n")
			{
				// 0: xL, 1: xR, 2: yL, 3: yR
				cout<<"run subtract\n";
				cv_subtract(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]), outfileName);
			}
			else if (rois[i][4] == "QRcode" || rois[i][4] == "QRcode\n")
			{
				cout<<"run QRcode\n";
				cv_qrcode(src, tgt, stoi(rois[i][0]), stoi(rois[i][1]), stoi(rois[i][2]), stoi(rois[i][3]));
			}
		}
	}

}


/*-----------------------------------------------------------------------**/
//Gaussian + Sobel5
void utility::cv_gausobel(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, int kernelSize, int gauSize, double signmaX, double sigmaY)
{
	GaussianBlur(src(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), Size(gauSize, gauSize), sigmaY, sigmaY);
	Sobel(tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), -1, 1, 1, kernelSize);
}

/*-----------------------------------------------------------------------**/
// Sobel of 3x3 or 5x5
void utility::cv_sobel(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, int kernelSize)
{
	Sobel(src(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), -1, 1, 1, kernelSize);
}

/*-----------------------------------------------------------------------**/
// Otsu adaptive threshold
void utility::cv_otsu(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR)
{
	threshold(src(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), 0, 255,THRESH_OTSU);
}

/*-----------------------------------------------------------------------**/
// Canny edge detection
void utility::cv_canny(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, double threshold1, double threshold2)
{
	Canny(src(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), threshold1, threshold2);
}

/*-----------------------------------------------------------------------**/
// create histogram
void utility::cv_histogram(cv::Mat &src, int xL, int yL, int xR, int yR, const char *outfile, const int imgIndex)
{
	vector<cv::Mat> bgr_planes;
    cv::split( src(cv::Rect(xL, yL, xR-xL, yR-yL)), bgr_planes );
    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;

	if(bgr_planes.size() > 1)
	{
		cv::Mat b_hist, g_hist, r_hist;
		cv::calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
		cv::calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
		cv::calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );

		cv::normalize(b_hist, b_hist, 0, 256, cv::NORM_MINMAX, -1, cv::Mat() );
		cv::normalize(g_hist, g_hist, 0, 256, cv::NORM_MINMAX, -1, cv::Mat() );
		cv::normalize(r_hist, r_hist, 0, 256, cv::NORM_MINMAX, -1, cv::Mat() );
		
		cv::Mat red(256, 256, CV_8UC3, cv::Scalar( 0,0,0));
		cv::Mat blue = red.clone();
		cv::Mat green = red.clone();
		for(int i = 0; i < histSize; i++)
		{
				// cout<< r_hist.at<float>(i)<<endl;

			for(int j = 0; j < cvRound(r_hist.at<float>(i)); j++)
			{
				// if(j > 255)
				red.at<Vec3b>(255 - j>0?255-j:255, i)[2] = 255;
			}
			for(int j = 0; j < cvRound(b_hist.at<float>(i)); j++)
			{
				blue.at<Vec3b>(255 - j>0?255-j:255, i)[0] = 255;
			}
			for(int j = 0; j < cvRound(g_hist.at<float>(i)); j++)
			{
				// if(j > 255)
				green.at<Vec3b>(255 - j>0?255-j:255, i)[1] = 255;
			}
		}

		char rHistogramName[200];
		char bHistogramName[200];
		char gHistogramName[200];
		char c = '0' + imgIndex;

		rHistogramName[0] = c;
		bHistogramName[0] = c;
		gHistogramName[0] = c;
		rHistogramName[1] = '\0';
		bHistogramName[1] = '\0';
		gHistogramName[1] = '\0';

		strcat(rHistogramName, "_red_histogram_");
		strcat(rHistogramName, outfile);
		strcat(bHistogramName, "_blue_histogram_");
		strcat(bHistogramName, outfile);
		strcat(gHistogramName, "_green_histogram_");
		strcat(gHistogramName, outfile);


		cv::imwrite(rHistogramName, red);
		cv::imwrite(bHistogramName, blue);
		cv::imwrite(gHistogramName, green);
	}
	else
	{
		cv::Mat r_hist;
		cv::calcHist( &bgr_planes[0], 1, 0, cv::Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );

		cv::normalize(r_hist, r_hist, 0, 256, cv::NORM_MINMAX, -1, cv::Mat() );
		
		cv::Mat red(256, 256, CV_8UC3, cv::Scalar( 0,0,0));
		for(int i = 0; i < histSize; i++)
		{
				// cout<< r_hist.at<float>(i)<<endl;

			for(int j = 0; j < cvRound(r_hist.at<float>(i)); j++)
			{
				// if(j > 255)
				red.at<Vec3b>(255 - j>0?255-j:255, i)[2] = 255;
			}
		}

		char rHistogramName[200];
		char c = '0' + imgIndex;

		rHistogramName[0] = c;
		
		rHistogramName[1] = '\0';

		strcat(rHistogramName, "_red_histogram_");
		strcat(rHistogramName, outfile);

		cv::imwrite(rHistogramName, red);
	}
}


/*-----------------------------------------------------------------------**/
// Histrogram stretching
void utility::cv_histostretch(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, const char *outfile, const int imgIndex)
{
	char name[200] = "bef_";
	strcat(name, outfile);
	cv_histogram(src, xL, yL, xR, yR, name, imgIndex);

	// stretch
	cv::normalize(src(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), 0, 255, cv::NORM_MINMAX, -1, cv::Mat());

	strcpy(name, "aft_");
	strcat(name, outfile);
	cv_histogram(tgt, xL, yL, xR, yR, name, imgIndex);
}

/*-----------------------------------------------------------------------**/
// histogram equalization
void utility::cv_histoequ(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, const char *outfile, const int imgIndex)
{
	char name[200] = "bef_";
	strcat(name, outfile);
	cv_histogram(src, xL, yL, xR, yR, name, imgIndex);

	cv::equalizeHist(src(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)));
	
	strcpy(name, "aft_");
	strcat(name, outfile);
	cv_histogram(tgt, xL, yL, xR, yR, name, imgIndex);
}

/*-----------------------------------------------------------------------**/
// Hough transform circle detection
void utility::cv_houghcir(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, int gauSize, double signmaX, double sigmaY, double threshold1, double threshold2,
	int minDist, int minR, int maxR)
{
 	Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray(cv::Rect(xL, yL, xR-xL, yR-yL)), gray(cv::Rect(xL, yL, xR-xL, yR-yL)), Size(gauSize, gauSize), sigmaY, sigmaY);

	Canny(gray(cv::Rect(xL, yL, xR-xL, yR-yL)), gray(cv::Rect(xL, yL, xR-xL, yR-yL)), threshold1, threshold2);
    
	vector<Vec3f> circles;
	HoughCircles(gray(cv::Rect(xL, yL, xR-xL, yR-yL)), circles, HOUGH_GRADIENT, 1,
                minDist,  // change this value to detect circles with different distances to each other
                100, 30, minR, maxR // change the last two parameters
            // (min_radius & max_radius) to detect larger circles
    );
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        circle( tgt, center, 1, Scalar(0,100,100), 3, LINE_AA);
        // circle outline
        int radius = c[2];
        circle( tgt, center, radius, Scalar(255,0,255), 3, LINE_AA);
    }
}

/*-----------------------------------------------------------------------**/
// histogram equalization + otsu
void utility::cv_otsuhisto(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, const char *outfile, const int imgIndex)
{
	cv_histoequ(src, tgt, xL, yL, xR, yR, outfile, imgIndex);
	threshold(tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), 0, 255,THRESH_OTSU);
}

/*-----------------------------------------------------------------------**/
// Gaussian blur + otsu
void utility::cv_otsugau(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, int gauSize, double signmaX, double sigmaY)
{
	GaussianBlur(src(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), Size(gauSize, gauSize), sigmaY, sigmaY);
	threshold(tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), 0, 255,THRESH_OTSU);
}

/*-----------------------------------------------------------------------**/
// combine operation of Gaussian+histogram qualization+otsu
void utility::cv_combine(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, int gauSize, double signmaX, double sigmaY, const char *outfile, const int imgIndex)
{
	GaussianBlur(src(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), Size(gauSize, gauSize), sigmaY, sigmaY);
	cv_histoequ(tgt, tgt, xL, yL, xR, yR, outfile, imgIndex);
	threshold(tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)), 0, 255,THRESH_OTSU);
}
/*-----------------------------------------------------------------------**/
// subtract 2 image
void utility::cv_subtract(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR, const char *outfile)
{
	Mat dest;
	tgt = cv::imread(outfile, 0);
	cv::subtract(src(cv::Rect(xL, yL, xR-xL, yR-yL)), tgt(cv::Rect(xL, yL, xR-xL, yR-yL)),dest);
	tgt = dest.clone();
	// cvtColor(tgt, tgt, COLOR_GRAY2RGB);
	std::cout<<"type: "<<tgt.type()<<std::endl;
	// for(int i =0; i < tgt.rows; i++)
	// {
	// 	for(int j=0; j < tgt.cols; j++)
	// 	{
	// 		tgt.at<Vec3b>(j, i)[2] = 255;
	// 	}
	// }

}

/*-----------------------------------------------------------------------**/
// Qr code decodeing and detecting
void utility::cv_qrcode(cv::Mat &src, cv::Mat &tgt, int xL, int yL, int xR, int yR)
{
	Mat bbox, temp;
	QRCodeDetector qr;
	std::string data = qr.detectAndDecode(src(cv::Rect(xL, yL, xR-xL, yR-yL)), bbox, temp);
	if(data.length()>0)
	{
		cout << "Decoded Data : " << data << endl;
		tgt = temp.clone();
	}
	else
	{
		cout << "Failed " << endl;
		
	}
}

