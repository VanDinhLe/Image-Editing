This software is architectured as follows. This software can work on grad server.

iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory. It should contains
	the input original image in pgm format, the parameter text file (in .txt), and the output image in pgm
	format.



*** INSTALATION ***

On Linux

Enter the project directory in terminal and run make

As a result you should get iptool in project/bin directory.

Enter the bin directory and run with this command: "./iptool parameters.txt"
where parameters.txt is the parameter text file

*** FUNCTIONS (without OpenCV) ***

I. There are 3 functions for whole image

1. Add intensity: add
Increase the intensity for a gray-level image.

2. Binarization: binarize
Binarize the pixels with the threshold.

3. Scaling: Scale
Reduce or expand the heigh and width with two scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.

II. there are 9 functions for different ROIs (up to 3 ROIs)

1. Add, binarize, and scale work similarly like mentioned before but now they
apply to 3 separate ROIs

2. Area of interest: aoi_bright
Highlight the center area of the selected ROIs

3. Adding color brightness: color_bright
Make the ROIs brighter in 3 rgb values, red, green, and blue

4.Color visualization: aoi_vis
Turn the pixels in ROIs into red or green with the same intensity based on
the threshold

5. Grey scale image histogram stretching: histo
Stretch the the grey image histogram into range of 0 to 255

6. Combine histogram stretching with thresholds: combohisto
Stretch the grey image histogram into range of user desired

7. Individual color channel stretching: perchastretch
Stretch the histo gram of a desired color channel to a specific range

8. RGB Combine stretching: rgbstretch
Apply histogram  stretching  to  three channels simultaneously 
into a specific range on histogram

9. Colorize grey scale image: colorization
Colorization of gray level images into 3 color channels using 2 threshold values.

10. Dividing gray scale into 10 different color: tencolor
Dividing the intensity range of the image into ten part and colors them differently

*** FUNCTIONS (with OpenCV) ***

I. There are 2 functions for whole image

1. Gray Scale: gray
Change whole image to gray-level.
parameter: no

2. Average Blur: blur_avg
Uniform smoothing.
parameter: window_size

II. these are functions for different ROIs (up to 3 ROIs)

1. Edge detection with Sobel: sobel3 or sobel5
Applying sobel of kernel size 3x3 or 5x5
parameter: no

2. Gaussian smoothing combined with Sobel operation: gausobel5
Applying Gaussian smoothing then use Sobel function of 5x5
parameter: Gaussian kernel size, Sigma X, Sigma Y

3. Otsu addaptive threshold: otsu
Applying Otsu addaptive threshold
parameter: no

4. Canny Edge detection: canny
Apply Canny Edge detection with thresholds
parameter: lower threshold, upper threshold

5. Histogram stretching: histostrech
Stretch histogram on an image

6. histogram equalizatio: histoequ
perform histogram equalization on an image

7. hough circle detection: houghcir
detect circles in an image

8. Gaussian and Otsu: otsugau
perform Gaussian smoothing + otsu

9. Otsu + histogram equalization: otsuhisto
perform histogram equalization + otsu

10. combine operation: combine
perform histogram equalization + Gaussian + otsu

11. subtract image: subtract
subtract two images, the source image will subtract the output
and the result will be stored in the output

12. QR code read: QRcode
detect and decode QR code, it will print out the decoded string
in the terminal in the form : 'Decoded Data : "decoded tring"'

*** PARAMETERS FILE (without OpenCV) ***

There are four parameters that operate on the whole image:
1. the input file name;
2. the output file name;
3. the name of the filter. Use "add", "binarize", and "scale" for your filters;
4. the value for adding intensity, threshold value for binarize filter, or the scaling factor for scale filter.

For implemented ROIs there are more additional parameters:
1. the input file name;
2. the output file name;
3. the number os ROIs (up to 3)
4. for each ROI the parameters will contain:
	1/ x coordinate of top left corner
	2/ y coordinate of top left corner
	3/ x coordinate of bototm right corner
	4/ y coordinate of bototm right corner
	5/ the function's name
	6/ the last parameters will vary depended on the function:
		- for add, binarize, scale, aoi_bright, there will be one value for
		  adding intensity, threshold value for binarize filter, or the scaling factor for scale filter,
		  and value for highlighting the area of interest
		- for color_bright, 3 mores rgb values are needed in the order of red, green and blue
		- for color_vis, 2 mores parameters are needed for the value and the threshold
		- for histo - grey scale image histogram stretching no additional parameter needed
		- for combohisto, 2 more parameters needed for threshold t1, t2
		- for perchastretch - Individual rbg stretching, needs a first letter of R or G or B, and two thresholds
		- for rgbstretch, needs two additional thresholds
		- for colorization, needs two thresholds
		- for tencolor, no needs additional parameter

*** PARAMETERS FILE (with OpenCV) ***

There are four parameters that operate on the whole image:
1. the input file name
2. the output file name
3. opencv
4. the name of the function with OpenCV

For implemented ROIs there are more additional parameters:
1. the input file name;
2. the output file name;
4. opencv
5. the number os ROIs (up to 3)
6. for each ROI the parameters will contain:
	1/ x coordinate of top left corner
	2/ y coordinate of top left corner
	3/ x coordinate of bototm right corner
	4/ y coordinate of bototm right corner
	5/ the function's name
	6/ the last parameters will vary depended on the function:
		-gausobel5 needs Gaussian size, SigmaX, SigmaY
		-Canny needs threshold1, threshold2
		-histocir needs Gaussian size, SigmaX, SigmaY, threshold1, threshold2,
		 minimum distance between centers, minimum radius, maximum radius
		- otsugau needs Gaussian size, SigmaX, SigmaY
		- combine needs Gaussian size, SigmaX, SigmaY



*** HISTOGRAM OUTPUT NAME ***

The name of output histogram follows this rule:

{ROI number from 1 to 3 based on order of function calls}_{red, green, blue or grey}_{before or after processing}_{output file's name}. extendsion

example:
1_blue_histogram_aft_catImage.ppm

*** Run the program: ./iptool parameters.txt
