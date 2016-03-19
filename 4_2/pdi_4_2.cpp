#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat image2bw(Mat image) {
	int width = image.size().width;
	int height = image.size().height;

	for (int i = 0; i<height; i++) {
		for (int j = 0; j<width; j++) {
			if (image.at<uchar>(i, j) >= 128) {
				image.at<uchar>(i, j) == 255;
			}
			else {
				image.at<uchar>(i, j) == 0;
			}
		}
	}

	return image;
}

int main(int argc, char** argv) {
	Mat original, image, imagebw, result, mask;
	int width, height;
	int nobjects;

	CvPoint p;

	string arquivo;							// nome do arquivo
	cout << "Digite o nome do arquivo : ";
	cin >> arquivo;							// armazenando o nome fornecido

	original = cv::imread(arquivo);			// abrindo a imagem
	
	if (original.empty()) {																		// verificando se é possível abrir a imagem
		cout << "error: o arquivo --> " << arquivo << " <-- não pode ser lido!\n\n";		// mensagem de erro caso contrário
		system("pause");
		return(0);																				// saindo do programa
	}

	cvtColor(original, image, CV_BGR2GRAY);

	adaptiveThreshold(image, imagebw, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 105, 1);

	//dilate(imagebw, result, 0, Point(-1, -1), 2, 1, 1);
	int erosion_size = 10;
	Mat element = getStructuringElement(cv::MORPH_CROSS,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));

	erode(imagebw, result, element);


	//imagebw = image2bw(image);

	width = result.size().width;
	height = result.size().height;

	p.x = 0;
	p.y = 0;

	// busca objetos com buracos presentes
	nobjects = 0;
	for (int i = 0; i<height; i++) {
		for (int j = 0; j<width; j++) {
			if (result.at<uchar>(i, j) == 255) {
				// achou um objeto
				nobjects++;
				p.x = j;
				p.y = i;
				floodFill(result, p, nobjects);
			}
		}
	}

	cout << "\n\nTotal de bolhas encontradas : " << nobjects << "\n\n";

	namedWindow("Original", CV_WINDOW_AUTOSIZE);
	namedWindow("Escala de Cinza", CV_WINDOW_AUTOSIZE);
	namedWindow("B&W", CV_WINDOW_AUTOSIZE);
	namedWindow("Resultado", CV_WINDOW_AUTOSIZE);

	imshow("Original", original);
	imshow("Escala de Cinza", image);
	imshow("B&W", imagebw);
	imshow("Resultado", result);
	waitKey();
	return 0;
}
