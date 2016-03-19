/*
	UNIVERSIDADE FEDERAL DO RIO GRANDE DO NROTE
	DEPARTAMENTO DE COMPUTA��O E AUTOMA��O
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
#include<string>

int px_1,py_1, px_2, py_2;
bool clicado = false;
cv::Mat imgOriginal;		// imagem original
cv::Mat imgGrayscale;		// imagem em escala de cinza
cv::Mat imgResult;		// imagem em escala de cinza com regi�o negativa



///////////////////////////////////////////////////////////////////////////////////////////////////
/*
Este metodo reconhece as coordenadas informadas pelo click do mouse
Verifica quais s�o os pontos maximo e minimos para X e Y
Percorre a matriz partindo do ponto minimo ao maximo invertendo o valor do pixel

A invers�o do valor do pixel d�-se pelo complemento de 255, ou seja
valor_novo = 255 - valor_atual
*/
void Negativar() {

	std::cout << "\n-------- Negativando a ROI definida...\n";

	int max_X = int(fmax(px_1, px_2));
	int max_Y = int(fmax(py_1, py_2));
	int min_X = int(fmin(px_1, px_2));
	int min_Y = int(fmin(py_1, py_2));

	// criando a regi�o com negativo da imagem
	cv::cvtColor(imgOriginal, imgResult, CV_BGR2GRAY);

	for (int i = min_X; i<max_X; i++) {
		for (int j = min_Y; j<max_Y; j++) {
			imgResult.at<uchar>(j, i) = 255 - imgResult.at<uchar>(j, i);
		}
	}

	cv::imshow("imgResult", imgResult);

	std::cout << "-------- ROI Negativada...\n\n";

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
O m�todo de CallBack trata o uso de eventos do mouse sobre uma imagem
Neste caso ele � respons�vel armazenar os pontos de clique do mouse e
por chamar o metodo Negativar, respons�vel por gerar o negativo da ROI
*/

void CallBackFunc(int event, int x, int y, int flags, void* userdata){
	// Primeiro ponto, quando o bot�o esquerdo do mouse � clicado
	if (event == cv::EVENT_LBUTTONDOWN && !clicado){
		px_1 = x;
		py_1 = y;
		clicado = true;
		std::cout << "Mouse clicou na posicao 1 - (" << x << ", " << y << ")" << std::endl;
	}
	// Segundo ponto, quando o bot�o esquerdo do mouse � liberado
	if (event == cv::EVENT_LBUTTONUP && clicado){
		px_2 = x;
		py_2 = y;
		clicado = false;
		std::cout << "Mouse arrastado ate a posicao 2 - (" << x << ", " << y << ")" << std::endl;

		Negativar();
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/*
M�todo principal, respons�vel por ler um arquivo de imagem fornecido pelo usu�rio,
configurar a exibi��o das imagens e setar o callback do mouse em uma imagem especifica
*/
int main() {

	std::string arquivo;							// nome do arquivo
	std::cout << "Digite o nome do arquivo :";		
	std::cin >> arquivo;							// armazenando o nome fornecido

	imgOriginal = cv::imread(arquivo);			// abrindo a imagem

	if (imgOriginal.empty()) {																	// verificando se � poss�vel abrir a imagem
		std::cout << "error: o arquivo --> " << arquivo << " <-- n�o pode ser lido!\n\n";		// mensagem de erro caso contr�rio
		system("pause");
		return(0);																				// saindo do programa
	}

	cv::cvtColor(imgOriginal, imgGrayscale, CV_BGR2GRAY);		// convertendo para escala de cinza

	// declarando as janelas
	cv::namedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("imgGrayscale", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("imgResult", CV_WINDOW_AUTOSIZE);
	// configurando o callback na janela
	cv::setMouseCallback("imgOriginal", CallBackFunc, NULL);

	// mostrando as janelas
	cv::imshow("imgOriginal", imgOriginal);
	cv::imshow("imgGrayscale", imgGrayscale);

	std::cout << "Selecione uma regiao de interesse...\n\n";

	cv::waitKey(0);

	return(0);
}

