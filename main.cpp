#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "matrix.cpp"

using namespace std;

unsigned valueInstructionR(ifstream&, string);

bool isR(string str) {
	for (int i = 0; i < 16; i++) {
		if (str == r[i].type)
			return true;
	}
	return false;
}

// Colocar na matriz
// Ler linhas e colocar na matriz
// Contar a linha do label

int main()
{
	unsigned labelCount = 0;
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
				valueInstructionR(fin, aux);
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

unsigned valueInstructionR(std::ifstream& fin, string aux) {
	unsigned value = 0;
	for (int i = 0; i < 16; i++)
		if (aux == r[i].type && (aux != r[0].type && aux != r[1].type)) {
			int regs[3];

			value = value | r[i].op;
			value = value << 6;

			for (int j = 0; j < 3; j++) {
				fin >> aux; // $19,							
				aux.erase(3, 3);
				int regis;
				for (regis = 0; regis < 32; regis++)
					if (aux == registradores[regis])
						break;
				regs[j] = regis;
			}

			value = value | regs[1];
			value = value << 5;

			value = value | regs[2];
			value = value << 5;

			value = value | regs[0];
			value = value << 5;

			value = value << 6;
			value = value | r[i].funct;
		}
		else if (aux == r[0].type || aux == r[1].type) {
			int regs[3];
			int func;

			aux == r[0].type ? func = 0 : func = 2;

			value = value | r[i].op;
			value = value << 6;

			for (int j = 0; j < 2; j++) {
				fin >> aux; // $19,							
				aux.erase(3, 3);
				int regis;
				for (regis = 0; regis < 32; regis++)
					if (aux == registradores[regis])
						break;
				regs[j] = regis;
			}

			fin >> aux;
			regs[2] = stoi(aux);
			value = value << 5; // rs

			value = value | regs[1]; // rt
			value = value << 5;

			value = value | regs[0]; // rd
			value = value << 5;

			value = value | regs[2]; // shamt
			
			value = value << 6;
			value = value | func;
		}
	return value;
}