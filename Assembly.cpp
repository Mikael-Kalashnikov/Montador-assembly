#include <iostream>
#include <fstream>
using namespace std;

int main() {
	int cont = 0;
	char arquivo[20];
		cout << "Informe o nome do arquivo que cont�m o c�digo assembly: ";
		cin >> arquivo;

		/*Abre o arquivo com o nome informado*/
		ifstream fin;
		fin.open(arquivo);
			/*Se o arquivo n�o existir ou ocorrer alguma falha ao tentar abrir, apresentar� mensagem de erro*/
			if (!fin.is_open()) {
				cout << "A abertura do arquivo falhou!" << endl;
				exit(EXIT_FAILURE);
		}
}