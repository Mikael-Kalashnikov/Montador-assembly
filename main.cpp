#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "matrix.cpp"

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
	/*Se o arquivo nao existir ou ocorrer alguma falha ao tentar abrir, apresentara mensagem de erro*/
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
			//cout << aux;
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
		int value = 0;
		fin >> aux;
		
		regex regInst("[0-9$:]");

		if (!regex_search(aux, regInst))
			if(isR(aux)){
				for(int i = 0; i < 16; i++)
					if(aux == r[i].type){
						value = value | r[i].op;

						for(int j = 0; j < 3; j++){
							fin >> aux; // $19,							
							aux.erase(2,3);
							int regis;
							for(regis = 0; i< 32; i++)
								if(aux == registradores[regis])
									break;
							
							cout << aux << endl;
							value = value << 5;
							value = value | regis;
						}

						value = value << 5;
						//value = value | r[i].shemt;

						value = value << 6;
						value = value | r[i].funct;
					}
			}
			else if (isI(aux))
				for(int k = 0; k < 11; k++){
					if(aux == i[k].type){
						value = value | i[k].op;

						for(int j = 0; j < 2; j++){
							fin >> aux; // $19,
							value = value << 5;
							// value = value | std::stoi(aux); 
						}

						fin >> aux;
						value = value << 16;
						// value = value | std::stoi(aux);
					}
				}
			else if(isJ(aux))
				for(int i = 0; i < 2; i++){
					if(aux == j[i].type){
						value = j[i].op;
						value = value << 26;

						fin >> aux;
						// value = value | std::stoi(aux);
					}
				}
			else 
				exit(EXIT_FAILURE);

		cout << value;
	}
}