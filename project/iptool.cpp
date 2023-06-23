#include "../iptools/core.h"
#include <string.h>

using namespace std;

#define MAXLEN 256

int main(int argc, char **argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char infile[MAXLEN];
	char outfile[MAXLEN];
	char *pch;

	if ((fp = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	while (fgets(str, MAXLEN, fp) != NULL)
	{
		if (strncmp(str, "#", 1) == 0)
			continue;
		int NumParameters = 0;

		pch = strtok(str, " ");
		strcpy(infile, pch);

		pch = strtok(NULL, " ");
		strcpy(outfile, pch);

		pch = strtok(NULL, " ");
		if (strncmp(pch, "opencv", 6) == 0)
		{
			cv::Mat I = cv::imread(infile);
			cv::Mat I2;

			if (I.empty())
			{
				cout << "Could not open or find the image: " << infile << endl;
				return -1;
			}

			pch = strtok(NULL, " ");
			if (strncmp(pch, "gray", 4) == 0)
			{
				utility::cv_gray(I, I2);
			}
			else if (strncmp(pch, "blur_avg", 8) == 0)
			{
				pch = strtok(NULL, " ");
				utility::cv_avgblur(I, I2, atoi(pch));
			}
			else if (isdigit(*pch) > 0 && isdigit(*pch) < 4)
			{
				char *inp = strtok(NULL, " ");
				// vector store each ROI + correspoding operation's parameters
				vector<vector<string>> rois(3);
				while (inp != NULL)
				{
					for (int i = 0; i < atoi(pch); i++)
					{
						while (isdigit(*inp) && (inp != NULL))
						{
							rois[i].push_back(inp);
							inp = strtok(NULL, " ");
						}
						if (strncasecmp(inp, "sobel3", 6) == 0)
						{
							rois[i].push_back(inp);
							/* Add Intensity */
						}

						else if (strncasecmp(inp, "sobel5", 6) == 0)
						{
							rois[i].push_back(inp);
							/* Add Intensity */
						}

						else if (strncasecmp(inp, "gausobel5", 9) == 0)
						{
							rois[i].push_back(inp);
							/* gaussian + sobel5 */
							// Gaussian size
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// sigmaX
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// sigmaY
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}
						else if (strncasecmp(inp, "otsuhisto", 9) == 0)
						{
							//grey image
							I = cv::imread(infile, 0);
							rois[i].push_back(inp);
						}
						else if (strncasecmp(inp, "otsugau", 7) == 0)
						{
							cout<<"run otsugau\n";
							//grey image
							I = cv::imread(infile, 0);
							rois[i].push_back(inp);

							// Gaussian size
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// sigmaX
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// sigmaY
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}
						else if (strncasecmp(inp, "otsu", 4) == 0)
						{
							// grey image
							I = cv::imread(infile, 0);
							rois[i].push_back(inp);
							/* otsu */
						}
						else if (strncasecmp(inp, "canny", 5) == 0)
						{
							// grey image
							I = cv::imread(infile, 0);
							rois[i].push_back(inp);
							/* canny */
							// threshold1
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// threshold2
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}
						else if (strncasecmp(inp, "histostrech", 11) == 0)
						{
							// I = cv::imread(infile, 0);
							rois[i].push_back(inp);
						}
						else if (strncasecmp(inp, "histoequ", 8) == 0)
						{
							I = cv::imread(infile, 0);
							rois[i].push_back(inp);
						}
						else if(strncasecmp(inp, "houghcir", 8) == 0)
						{
							rois[i].push_back(inp);
							// Gaussian size
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// sigmaX
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// sigmaY
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// canny threshold1
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// canny threshold2
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// min Distance
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// min Radius
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// max Radius
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}
										
						else if (strncasecmp(inp, "combine", 7) == 0)
						{
							//grey image
							I = cv::imread(infile, 0);
							rois[i].push_back(inp);
							// Gaussian size
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// sigmaX
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// sigmaY
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}
						else if (strncasecmp(inp, "subtract", 8) == 0)
						{
							//grey image
							I = cv::imread(infile, 0);
							rois[i].push_back(inp);
						}
						else if (strncasecmp(inp, "QRcode", 6) == 0)
						{
							//grey image
							I = cv::imread(infile, 0);
							rois[i].push_back(inp);
						}

						inp = strtok(NULL, " ");
					}
					inp = strtok(NULL, " ");
				}
				utility::cv_Rois(I, I2, atoi(pch), rois, outfile);
			}
			else
			{
				printf("No function: %s\n", pch);
				continue;
			}

			cv::imwrite(outfile, I2);
		}
		else
		{
			src.read(infile);
			if (strncmp(pch, "add", 3) == 0)
			{
				pch = strtok(NULL, " ");
				utility::addGrey(src, tgt, atoi(pch));
			}

			else if (strncmp(pch, "binarize", 8) == 0)
			{
				pch = strtok(NULL, " ");
				utility::binarize(src, tgt, atoi(pch));
			}

			else if (strncmp(pch, "scale", 5) == 0)
			{
				pch = strtok(NULL, " ");
				utility::scale(src, tgt, atof(pch));
			}

			else if (isdigit(*pch) > 0 && isdigit(*pch) < 4)
			{
				char *inp = strtok(NULL, " ");
				// vector store each ROI + correspoding operation's parameters
				vector<vector<string>> rois(3);
				while (inp != NULL)
				{
					for (int i = 0; i < atoi(pch); i++)
					{
						while (isdigit(*inp) && (inp != NULL))
						{
							rois[i].push_back(inp);
							inp = strtok(NULL, " ");
						}
						if (strncasecmp(inp, "add", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							/* Add Intensity */
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}

						else if (strncasecmp(inp, "binarize", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							/* Thresholding */
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}

						else if (strncasecmp(inp, "scale", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							/* Image scaling */
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}

						else if (strncasecmp(inp, "aoi_bright", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							/* Area of interest */
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}

						else if (strncasecmp(inp, "color_bright", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							/* Color intensity */
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}

						else if (strncasecmp(inp, "color_vis", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							/* Color visualization */
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}

						else if (strncasecmp(inp, "combohisto", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							/* Color visualization */
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}

						else if (strncasecmp(inp, "histo", MAXLEN) == 0 || strncasecmp(inp, "histo\n", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							/* histogram stretching */
						}
						// else if (strncasecmp(inp,"histo",MAXLEN)!=0) {
						// 	cout<<inp<<endl;
						// }

						else if (strncasecmp(inp, "perchastretch", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							// RGB individual stretching
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// Imin
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// Imax
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}

						else if (strncasecmp(inp, "rgbstretch", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							// RGB combine stretching
							// Imin
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// Imax
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}

						else if (strncasecmp(inp, "colorization", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							// grey scale colorization
							// t1
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
							// t2
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}

						else if (strncasecmp(inp, "tencolor", MAXLEN) == 0 || strncasecmp(inp, "tencolor\n", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							// ten color grey scale colorization
						}

						else if (strncasecmp(inp, "binaryedge3", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							// binary edge 3x3
							// threshold
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}

						else if (strncasecmp(inp, "binaryedge5", MAXLEN) == 0)
						{
							rois[i].push_back(inp);
							// binary edge 3x3
							// threshold
							inp = strtok(NULL, " ");
							rois[i].push_back(inp);
						}
						inp = strtok(NULL, " ");
					}
					inp = strtok(NULL, " ");
				}
				utility::Rois(src, tgt, atoi(pch), rois, outfile);
			}

			else
			{
				printf("No function: %s\n", pch);
				continue;
			}
			//    cout<< outfile <<endl;
			tgt.save(outfile);
		}
	}
	fclose(fp);
	return 0;
}
