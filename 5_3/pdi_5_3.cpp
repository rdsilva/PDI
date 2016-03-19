#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	Mat image, imageHSV;
	int width, height;
	int count = 0;
	double last_value = 0.0;
	double threshold = 1.6;
	VideoCapture cap;

	cap.open(0);

	if (!cap.isOpened()) {
		cout << "cameras indisponiveis";
		return -1;
	}

	width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	cout << "largura = " << width << endl;
	cout << "altura  = " << height << endl;

	while (1) {
		cap >> image;
		
		cvtColor(image, imageHSV, COLOR_BGR2HSV);

		Mat hsv_half_down = imageHSV(Range(imageHSV.rows / 2, imageHSV.rows - 1), Range(0, imageHSV.cols - 1));

		/// Using 50 bins for hue and 60 for saturation
		int h_bins = 50; int s_bins = 60;
		int histSize[] = { h_bins, s_bins };

		// hue varies from 0 to 179, saturation from 0 to 255
		float h_ranges[] = { 0, 180 };
		float s_ranges[] = { 0, 256 };

		const float* ranges[] = { h_ranges, s_ranges };

		// Use the o-th and 1-st channels
		int channels[] = { 0, 1 };
		
		/// Histograms
		MatND hist_base;    
		MatND hist_half_down;

		/// Calculate the histograms for the HSV images
		calcHist(&imageHSV, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false);
		normalize(hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat());

		calcHist(&hsv_half_down, 1, channels, Mat(), hist_half_down, 2, histSize, ranges, true, false);
		normalize(hist_half_down, hist_half_down, 0, 1, NORM_MINMAX, -1, Mat());

		/// Apply the histogram comparison methods
		
		int compare_method = 4;
		double base_half = compareHist(hist_base, hist_half_down, compare_method);

		double test = abs(base_half - last_value);
		
		if (test > threshold) {
			printf("%3d --> MOVIMENTO \n", count);
			last_value = base_half;
			count++;
		}

		imshow("image", image);
		if (waitKey(30) >= 0) break;
	}
	return 0;
}
