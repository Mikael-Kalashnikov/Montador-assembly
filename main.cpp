#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

struct R {
	string type;
	short funct;
	short op{};
};

struct I
{
	string type;
	short op;
};

struct J {
	string type;
	short op;
};

struct Label {
	string label;
	int line;
};

unsigned valueInstructionR(ifstream&, string, int&);
unsigned valueInstructionI(ifstream&, string, vector<Label>, int&);
unsigned valueInstructionJ(ifstream&, string, vector<Label>, int&);

bool isR(string);
bool isI(string);
bool isJ(string);

R typeR[18] = { {"sll", 0}, {"srl", 2}, {"jr", 8}, {"mfhi", 16}, {"mflo", 18}, {"mult", 24}, {"multu", 25},{ "div", 26},{ "divu", 27}, {"add", 32}, {"addu", 33}, {"sub", 34}, {"subu", 35}, {"and", 36},{ "or", 37},{ "slt", 43}, {"sltu", 43}, {"mul", 2, 28} };
I typeI[11] = { {"beq", 4}, {"bne", 5}, {"addi", 8}, {"addiu", 9}, {"slti", 10}, {"sltiu", 11}, {"andi", 12}, {"ori", 13}, {"lui", 15}, {"lw", 35}, {"sw", 43} };
J typeJ[2] = { {"j", 2}, {"jal", 3} };

string registradores[32] = { "$0", "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23", "$24", "$25", "$26", "$27", "$28", "$29", "$30","$31" };

const int initialAddress = 0x00400000;


// Colocar na matriz
// Ler linhas e colocar na matriz
// Contar a linha do label

int main()
{
	vector<Label> l;
	cout << "Informe o nome do arquivo ou o caminho ate ele: ";
	string arquivo;
	cin >> arquivo;
	std::ifstream inFile(arquivo);

	/*Abre o arquivo com o nome informado*/
	ifstream fin;
	fin.open(arquivo);
	/*Se o arquivo nao existir ou ocorrer alguma falha ao tentar abrir, apresentara mensagem de erro*/
	if (!fin.is_open())
	{
		cout << "A abertura do arquivo falhou!" << endl;
		exit(EXIT_FAILURE);
	}
	int line = 0;
	while (!fin.eof())
	{
		string aux;
		fin >> aux;

		regex reg(":");
		if (regex_search(aux, reg)){
			aux.pop_back();
			l.push_back({ aux, (line * 4) + initialAddress });
		}
		char checker;
		do {
			checker = fin.get();
		}
		while (checker != '\n' && !fin.eof()); // vai rodar at� pegar a quebra de linha
		line++;
	}

	fin.close();
	
	ofstream fout;
	char res;
	cout << "Deseja salvar o arquivo em qual formato?" << endl;
	do {
		cout << "[1] - Binario\n[2] - Hexadecimal\n:_\b";
		cin >> res;
	} while (res != '1' && res != '2');

	res == '1' ? fout.open("stdout.bin", ios_base::out | ios_base::binary) : fout.open("stdout.hex");

	fin.open(arquivo);
	int runTimeLine = 0;
 	while(!fin.eof()){
		string aux;
		fin >> aux;
		regex regInst("[0-9$:]");

		if (!regex_search(aux, regInst))
			if (isR(aux)) {
				unsigned instruction = valueInstructionR(fin, aux, runTimeLine);
				res == '1' ? fout.write((char *) &instruction, sizeof(unsigned)) : fout << hex << instruction << endl;
			}
			else if (isI(aux)) {
				unsigned instruction = valueInstructionI(fin, aux, l, runTimeLine);
				res == '1' ? fout.write((char *) &instruction, sizeof(unsigned)) : fout << hex << instruction << endl;
			}
			else if (isJ(aux)) {
				unsigned instruction = valueInstructionJ(fin, aux, l, runTimeLine);
				res == '1' ? fout.write((char *) &instruction, sizeof(unsigned)) : fout << hex << instruction << endl;
			}
			else 
				exit(EXIT_FAILURE);
	}

	cout << "\nArquivo salvo com o nome 'stdout.bin|.hex'\n";

	fin.close();
	fout.close();
}

bool isR(string str) {
	for (int i = 0; i < 18; i++) {
		if (str == typeR[i].type)
			return true;
	}
	return false;
}

bool isI(string str) {
	for (int k = 0; k < 11; k++) {
		if (str == typeI[k].type)
			return true;
	}
	return false;
}

bool isJ(string str) {
	for (int i = 0; i < 2; i++) {
		if (str == typeJ[i].type)
			return true;
	}
	return false;
}

bool isLabel(string aux) {
	regex reg("[:]");
	if (regex_search(aux, reg))
		return true;
	return false;
}


unsigned valueInstructionR(std::ifstream& fin, string aux, int& runTimeLine) {
	unsigned value = 0;
	if (aux == typeR[0].type || aux == typeR[1].type) {
		int regs[3]{};

		aux == typeR[0].type ? value = value | typeR[0].op : value = value | typeR[1].op;
		value = value << 6;

		for (int j = 0; j < 2; j++) {
			fin >> aux; // $t0,							
			aux.pop_back(); // $t0
			int regis;
			for (regis = 0; regis < 32; regis++)
				if (aux == registradores[regis])
					break;
			regs[j] = regis; // Salva o valor dos registradores
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
		aux == typeR[0].type ? value = value | typeR[0].funct : value = value | typeR[1].funct;

		fin.get() == '\n' ? runTimeLine++ : runTimeLine = runTimeLine; // Incremento de linha em tempo de execu��o

		return value;
	}

	for (int i = 2; i < 18; i++) {
		if (aux == typeR[i].type) {
			int regs[3]{};

			value = value | typeR[i].op;
			value = value << 6;

			if (typeR[i].type == "mult" || typeR[i].type == "multu" || typeR[i].type == "lui" || typeR[i].type == "div"){
				for (int j = 0; j < 2; j++) {
					fin >> aux; // $19,		
					regex reg("[,]");
					if (regex_search(aux, reg))
						aux.pop_back();			
					int regis;
					for (regis = 0; regis < 32; regis++)
						if (aux == registradores[regis])
							break;
					regs[j] = regis;
				}
				value = value | regs[1];
				value = value << 5;

				value = value | regs[0];
				value = value << 5;

				value = value << 6;
				value = value | typeR[i].funct;

			} else if (typeR[i].type == "mfhi" || typeR[i].type == "mflo"){
				int regis;
					for (regis = 0; regis < 32; regis++)
						if (aux == registradores[regis])
							break;
					regs[0] = regis;

				value = value | regs[0];
				value = value << 5;

				value = value << 6;
				value = value | typeR[i].funct;
			} else {
				for (int j = 0; j < 3; j++) {
					fin >> aux; // $19,		
					regex reg("[,]");
					if (regex_search(aux, reg))
						aux.pop_back();			
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
				value = value | typeR[i].funct;
			}

			fin.get() == '\n' ? runTimeLine++ : runTimeLine = runTimeLine; // Incremento de linha em tempo de execu��o

			break;
		}
	}
	return value;
}

unsigned valueInstructionI(ifstream& fin, string aux, vector<Label> l, int& runTimeLine) {
	unsigned regs[2]{};
	int value = 0;

	if (aux == typeI[0].type || aux == typeI[1].type) {
		aux == typeI[0].type ? value = value | typeI[0].op : value = value | typeI[1].op;
		value = value << 5;
		// Pegar a linha atual da execu��o
		for (int j = 0; j < 2; j++) {
			fin >> aux; // $19,							
			aux.pop_back();
			int regis;
			for (regis = 0; regis < 32; regis++)
				if (aux == registradores[regis])
					break;
			regs[j] = regis;
		}
		value = value | regs[0];
		value = value << 5;

		value = value | regs[1];
		value = value << 16;

		fin >> aux;
		for (int j = 0; j < l.size(); j++) {
			if (aux == l[j].label) {
				int p1 = initialAddress - (l[j].line + (runTimeLine + 1) * 4);
				p1 = 65535 + p1 / 4;
				value = value | p1 + 1; // Pega o numero de instrucoes acima, -12 por exemplo, 3 instrucoes acima e faz um pseudo complemento de 2 para 16 bits
			}
		}

		fin.get() == '\n' ? runTimeLine++ : runTimeLine = runTimeLine; // Incremento de linha em tempo de execu��o
		return value;
	}

	for (int j = 2; j < 11; j++) {
		if (aux == typeI[j].type) {
			value = value | typeI[j].op;
			value = value << 5;
			if (typeI[j].type == "lw" || typeI[j].type == "sw"){
				fin >> aux;
				aux.pop_back();
				int regis;
				for (regis = 0; regis < 32; regis++)
					if (aux == registradores[regis])
						break;
				regs[0] = regis;

				int stack;
				fin >> aux;
				stack = stoi(aux);

				fin >> aux;
				aux.erase(0, 1);
				aux.pop_back();
				for (regis = 0; regis < 32; regis++)
					if (aux == registradores[regis])
						break;
				regs[1] = regis;

				value = value | regs[1];
				value = value << 5;

				value = value | regs[0];
                value = value << 5;

				value = value | initialAddress + stack;
                value = value << 16;

				} else if (typeI[j].type == "lui") {
					fin >> aux;
					aux.pop_back();
					int regis;
					for (regis = 0; regis < 32; regis++)
						if (aux == registradores[regis])
							break;
					regs[0] = regis;

					int stack;
					fin >> aux;
					stack = stoi(aux);

					value = value << 5;

					value = value | regs[0];
					value = value << 5;
				} else {
				for (int k = 0; k < 2; k++) {
					fin >> aux;
					aux.pop_back();

					int regis;
					for (regis = 0; regis < 32; regis++)
						if (aux == registradores[regis])
							break;
					regs[k] = regis;
				}

					value = value | regs[1];
					value = value << 5;

					value = value | regs[0];
					value = value << 16;

					if (typeI[j].type != "mult" || typeI[j].type != "multu" || typeI[j].type != "lui" || typeI[j].type != "div"){
						fin >> aux;
						value = value | stoi(aux);
					}
			}
			fin.get() == '\n' ? runTimeLine++ : runTimeLine = runTimeLine; // Incremento de linha em tempo de execucao

			break;
		}
	}
	return value;
}

unsigned valueInstructionJ(ifstream& fin, string aux, vector<Label> l, int& runTimeLine) {
	int value = 0;
	if (aux == typeJ[0].type) {
		value = value | typeJ[0].op;
		value = value << 26;

		fin >> aux;
		for (int i = 0; i < l.size(); i++) {
			if (aux == l[i].label)
				value = value | (l[i].line/4);
		}

		fin.get() == '\n' ? runTimeLine++ : runTimeLine = runTimeLine; // Incremento de linha em tempo de execu��o
	}
	return value;
}