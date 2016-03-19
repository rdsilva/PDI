/*
UNIVERSIDADE FEDERAL DO RIO GRANDE DO NROTE
DEPARTAMENTO DE COMPUTAÇÃO E AUTOMAÇÃO
PROCESSAMENTO DIGITAL DE IMAGENS
PROF. AGOSTINHO BRITO JR.

UNIDADE I
EXERCICIO 3.2

RODRIGO DANTAS DA SILVA - 2008020606

*/

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<algorithm>
#include<stdlib.h>
#include<time.h>

int px_1, py_1, px_2, py_2;
bool clicado = false;
cv::Mat imgOriginal;		// imagem original
cv::Mat imgGrayscale;		// imagem em escala de cinza
cv::Mat imgResult;			// imagem resultado em escala de cinza
cv::Mat listOfRoi[4];		// lista de ROIs criadas


void pickARoi() {
	int index[4] = { 0,1,2,3 };

	srand(time(0));
	std::random_shuffle(std::begin(index), std::end(index)); 

	// criando indices temporarios apenas para ficar "melhor legivel"
	int ind1 = index[0];
	int ind2 = index[1];
	int ind3 = index[2];
	int ind4 = index[3];

	// imprimindo apenas para checar a ordem
	std::cout << "\n\nOrdem : { " << ind1 << ", " << ind2 << ", " << ind3 << ", " << ind4 << " }\n\n";

	listOfRoi[ind1].copyTo(imgResult(cv::Rect(0,					0,							listOfRoi[ind1].cols,	listOfRoi[ind1].rows)));	// região 1 em formato Z
	listOfRoi[ind2].copyTo(imgResult(cv::Rect(listOfRoi[ind1].cols, 0,							listOfRoi[ind2].cols,	listOfRoi[ind2].rows)));	// região 2
	listOfRoi[ind3].copyTo(imgResult(cv::Rect(0,					listOfRoi[ind1].rows,		listOfRoi[ind3].cols,	listOfRoi[ind3].rows)));	// região 3
	listOfRoi[ind4].copyTo(imgResult(cv::Rect(listOfRoi[ind1].cols, listOfRoi[ind1].rows,		listOfRoi[ind4].cols,	listOfRoi[ind4].rows)));	// região 4

}


///////////////////////////////////////////////////////////////////////////////////////////////////
/*
Método principal, responsável por ler um arquivo de imagem fornecido pelo usuário,
configurar a exibição das imagens e setar o callback do mouse em uma imagem especifica
*/
int main() {

	std::string arquivo;							// nome do arquivo
	std::cout << "Digite o nome do arquivo : ";
	std::cin >> arquivo;							// armazenando o nome fornecido

	imgOriginal = cv::imread(arquivo);			// abrindo a imagem

	if (imgOriginal.empty()) {																	// verificando se é possível abrir a imagem
		std::cout << "error: o arquivo --> " << arquivo << " <-- não pode ser lido!\n\n";		// mensagem de erro caso contrário
		system("pause");
		return(0);																				// saindo do programa
	}

	cv::cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);		// convertendo para escala de cinza

	// declarando as janelas
	cv::namedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("imgGrayscale", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("imgResult", CV_WINDOW_AUTOSIZE);

	// calculando as ROI's e reordenando

	int col_half = imgOriginal.cols / 2;
	int row_half = imgOriginal.rows / 2;

	std::cout << "\n\nWidth : " << imgOriginal.cols << "px\n";
	std::cout << "Height : " << imgOriginal.rows << "px\n\n";

	// roi(x, y, wid, ht);
	// y ->	rows ->	linhas	-> ht
	// x ->	cols ->	colunas -> wid
	cv::Rect roi1(0,			0,			col_half,	row_half);		// roi region 1
	cv::Rect roi2(col_half,		0,			col_half,	row_half);		// roi region 2
	cv::Rect roi3(0,			row_half,	col_half,	row_half);		// roi region 3
	cv::Rect roi4(col_half,		row_half,	col_half,	row_half);		// roi region 4
	
	// adicionando os ROIs a lista
	listOfRoi[0] = cv::Mat(imgGrayscale, roi1);
	listOfRoi[1] = cv::Mat(imgGrayscale, roi2);
	listOfRoi[2] = cv::Mat(imgGrayscale, roi3);
	listOfRoi[3] = cv::Mat(imgGrayscale, roi4);

	// setando a imagem resultado
	imgResult = cv::Mat(imgGrayscale.rows, imgGrayscale.cols, imgGrayscale.type());

	//ROI2.copyTo(imgResult(cv::Rect(0, 0, ROI2.cols, ROI2.rows)));								// região 1 em formato Z
	//ROI3.copyTo(imgResult(cv::Rect(ROI2.cols, 0, ROI3.cols, ROI3.rows)));						// região 2
	//ROI1.copyTo(imgResult(cv::Rect(0, ROI2.rows, ROI1.cols, ROI1.rows)));						// região 3
	//ROI4.copyTo(imgResult(cv::Rect(ROI2.cols, ROI2.rows, ROI4.cols, ROI4.rows)));				// região 4

	pickARoi();

	// mostrando as janelas
	cv::imshow("imgOriginal", imgOriginal);
	cv::imshow("imgGrayscale", imgGrayscale);
	cv::imshow("imgResult", imgResult);


	cv::waitKey(0);

	return(0);
}

