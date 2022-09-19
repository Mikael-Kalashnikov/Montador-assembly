#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

// Colocar na matriz
// Ler linhas e colocar na matriz
// Contar a linha do label

int main()
{
	int labelCount = 0;
	string arquivo = "test.asm";
	std::ifstream inFile(arquivo);
	//	std::count(std::istreambuf_iterator<char>(inFile),
	//           std::istreambuf_iterator<char>(), '\n');
	// cout << "Informe o nome do arquivo que contem o codigo assembly: ";
	// cin >> arquivo;

	/*Abre o arquivo com o nome informado*/
	ifstream fin;
	fin.open(arquivo);
	/*Se o arquivo n�o existir ou ocorrer alguma falha ao tentar abrir, apresentar� mensagem de erro*/
	if (!fin.is_open())
	{
		cout << "A abertura do arquivo falhou!" << endl;
		exit(EXIT_FAILURE);
	}
	while (!fin.eof())
	{
		string aux;
		fin >> aux;

		regex reg(":");
		if (regex_search(aux, reg)){
			cout << aux;
			// Contar linha do label
			// Retirar ultimo caracter ':' e salvar numa matriz 
			labelCount++;
		}

		// Pegar a linha do label
	}
	fin.close();
	fin.open(arquivo);
	while(!fin.eof()){
		string aux;
		fin >> aux;
		
		regex reg(":");
		if (!regex_search(aux, reg))
			cout << aux;
	}
}