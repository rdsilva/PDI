#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

int width, height;
int nbins = 64;
int histw = nbins, histh = nbins / 2;
float range[] = { 0, 256 };
const float *histrange = { range };
bool uniform = true;
bool acummulate = false;

Mat histograma(Mat image, vector<Mat> planes, Mat histImgR, Mat histImgG, Mat histImgB, Mat histR, Mat histG, Mat histB) {
	// calculando o histograma da imagem original
	calcHist(&planes[0], 1, 0, Mat(), histR, 1,
		&nbins, &histrange,
		uniform, acummulate);
	calcHist(&planes[1], 1, 0, Mat(), histG, 1,
		&nbins, &histrange,
		uniform, acummulate);
	calcHist(&planes[2], 1, 0, Mat(), histB, 1,
		&nbins, &histrange,
		uniform, acummulate);

	// normalizando o histograma
	normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
	normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
	normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());

	histImgR.setTo(Scalar(0));
	histImgG.setTo(Scalar(0));
	histImgB.setTo(Scalar(0));

	// linearizando o historgama
	for (int i = 0; i<nbins; i++) {
		line(histImgR, Point(i, histh),
			Point(i, cvRound(histR.at<float>(i))),
			Scalar(0, 0, 255), 1, 8, 0);
		line(histImgG, Point(i, histh),
			Point(i, cvRound(histG.at<float>(i))),
			Scalar(0, 255, 0), 1, 8, 0);
		line(histImgB, Point(i, histh),
			Point(i, cvRound(histB.at<float>(i))),
			Scalar(255, 0, 0), 1, 8, 0);
	}

	// apresentando o histograma
	histImgR.copyTo(image(Rect(0, 0, nbins, histh)));
	histImgG.copyTo(image(Rect(0, histh, nbins, histh)));
	histImgB.copyTo(image(Rect(0, 2 * histh, nbins, histh)));

	return image;
}

int main(int argc, char** argv) {
	Mat imageOriginal, imageResult;
	Mat histR, histG, histB;
	Mat histREq, histGEq, histBEq;
	VideoCapture cap;
	vector<Mat> planes;
	vector<Mat> planesEq;
	
	cap.open(1);

	if (!cap.isOpened()) {
		cout << "cameras indisponiveis\n";
		system("pause");
		return -1;
	}

	width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	cout << "largura = " << width << endl;
	cout << "altura  = " << height << endl;

	Mat histImgR(histh, histw, CV_8UC3, Scalar(0, 0, 0));
	Mat histImgG(histh, histw, CV_8UC3, Scalar(0, 0, 0));
	Mat histImgB(histh, histw, CV_8UC3, Scalar(0, 0, 0));

	Mat histImgREq(histh, histw, CV_8UC3, Scalar(0, 0, 0));
	Mat histImgGEq(histh, histw, CV_8UC3, Scalar(0, 0, 0));
	Mat histImgBEq(histh, histw, CV_8UC3, Scalar(0, 0, 0));

	while (1) {
		// capturando a imagem
		cap >> imageOriginal;
		
		// separando os canais da imagem original
		split(imageOriginal, planes);


		// EQUALIZANDO A IMAGEM
		// ---------------------------------------------------------------------------

		// copiando e convertendo a imagem original para YCrCb
		cvtColor(imageOriginal, imageResult, CV_BGR2YCrCb);

		// separando os canais da imagem convertida
		split(imageResult, planesEq);

		// equalizando o canal Y (intensidade) da imagem convertida
		equalizeHist(planesEq[0], planesEq[0]);
		
		// agrupando os canais da imagem convertida
		merge(planesEq, imageResult);

		// convertendo a imagem manipulada para o dominio de cores original
		cvtColor(imageResult, imageResult, CV_YCrCb2BGR);

		// ---------------------------------------------------------------------------

		// CALCULAR E APRESENTAR HISTOGRAMAS
		// ---------------------------------------------------------------------------

		imageOriginal = histograma(imageOriginal, planes, histImgR, histImgG, histImgB, histR, histG, histB);
		imageResult = histograma(imageResult, planesEq, histImgREq, histImgGEq, histImgBEq, histREq, histGEq, histBEq);

		// ---------------------------------------------------------------------------

		// apresentando as imagens
		imshow("Original", imageOriginal);
		imshow("Resultado", imageResult);
		if (waitKey(30) >= 0) break;
	}
	return 0;
}
