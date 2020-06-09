#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <bitset>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

enum { MOK = 0, ANK, ORK, ADK, JMP, JCY, JNE, JZE, MOM, BSR, ADR, MOV, ORR, ANR, CPL, LSR, LSL, ASL, CLR, SET, RET };


string dec_to_hexa(int dec_num);
string hexa_to_binary_16(string hexa_num);
string hexa_to_binary_15(string hexa_num);
string dec_to_binary_5(int dec_num);
int param_to_int(string param, int i);
string operaciones_de_registros(string decod, string Rk, string Rj, string Ri);
string operaciones_de_ctes(string decod, string K, string Ri);
string operaciones_de_PCyMem(string decod, string K, string Ri);
void switch_instruc(int i, string param, string* param_array, ofstream& file_bin, ofstream& file_lst, int line);

map<string, int> labels;


int main(int argc, char *argv[]) {
	const char* letras_intruction[21]{ "MOK","ANK","ORK","ADK","JMP","JCY","JNE","JZE","MOM","BSR",
		"ADR","MOV","ORR","ANR","CPL","LSR","LSL","ASL","CLR","SET","RET" };

	string filename = "";
	std::cout << "Escribir nombre del archivo de instrucciones (con su extension)" << endl;
	cin >> filename;

	ifstream file_lectura(filename);
	ofstream file_bin("bin_" + filename);
	ofstream file_lst("lst_" + filename);
	file_bin << "v2.0 raw" << endl;

	string input;
	vector<string> instructions;
	int line = 0;
	while (getline(file_lectura, input))
	{
		int startofopcode = 0;
		if (!input.length()) { continue; }	// si es una linea en blanco
		if (!isspace(input[0]))				// si tiene label
		{
			startofopcode = min(input.find(' '), input.find('\t')) + 1;	//permite delimitar los labels con tab o con espacio.
			while (isspace(input[startofopcode])) { startofopcode++; }	//Por si se usan varios whitespaces de delimitacion.
			string label = input.substr(0,startofopcode - 1);
			if (labels.find(label) != labels.end()) {
				cout << "Error, redefinicion de label" << endl;
			}
			labels[label] =  line;
		}
		else // si no tiene label
		{
			while (isspace(input[++startofopcode])) {}
		}
		if (startofopcode < input.length())	// Si no es una linea con todo whitespace
		{
			instructions.push_back(input.substr(startofopcode, input.length()));
			line++;
		}
	}
	int address = 0;
	for(auto& ingreso : instructions)
	{
		string instruc = ingreso.substr(0, 3);
		string param;
		string param_array[2];
		int j = 0;
		if (ingreso.size() > 3)
		{
			param = ingreso.substr(4);
			string delimiter = ",";
			size_t pos = 0;
			while ((pos = param.find(delimiter)) != std::string::npos)
			{
				param_array[j] = param.substr(0, pos);
				j++;
				param.erase(0, pos + delimiter.length());
			}
		}
		std::cout << "param ====== " << param << endl;
		std::cout << "param array 0 =========" << param_array[0] << endl;
		std::cout << "param array 1 =========" << param_array[1] << endl;

		int i = 0;
		while (strcmp(instruc.c_str(), letras_intruction[i]))
		{
			i++;
		}
		std::stringstream stream;
		stream << "0x" << std::setfill('0') << std::setw(4) << std::hex << address;

		file_lst << stream.str() << " ";
		switch_instruc(i, param, param_array, file_bin, file_lst, address);
		file_lst << " " << instructions[address] << endl;
		
		address++;
	}

	file_lectura.close();
	file_bin.close();
	file_lst.close();
	return 0;
}

string dec_to_hexa(int dec_num)
{
	int r;
	string hexdec_num = "";
	char hex[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	while (dec_num > 0)
	{
		r = dec_num % 16;
		hexdec_num = hex[r] + hexdec_num;
		dec_num = dec_num / 16;
	}
	return hexdec_num;
}

string hexa_to_binary_16(string hexa_num)
{
	stringstream ss;
	ss << hex << hexa_num;
	unsigned n;
	ss >> n;
	bitset<16> b(n);
	return b.to_string();
}

string hexa_to_binary_15(string hexa_num)
{
	stringstream ss;
	ss << hex << hexa_num;
	unsigned n;
	ss >> n;
	bitset<15> b(n);
	return b.to_string();
}


string dec_to_binary_5(int dec_num)
{
	std::string binary = std::bitset<5>(dec_num).to_string(); //to binary
	return binary;
}

int param_to_int(string param, int i)
{
	param = param.substr(i);
	stringstream geek(param);
	int x = 0;
	geek >> x;
	return x;
}

string operaciones_de_registros(string decod, string Rk, string Rj, string Ri)
{
	int int_Ri = param_to_int(Ri, 1);
	int int_Rj = param_to_int(Rj, 1);
	int int_Rk = param_to_int(Rk, 1);
	string bin_Ri = dec_to_binary_5(int_Ri);
	string bin_Rj = dec_to_binary_5(int_Rj);
	string bin_Rk = dec_to_binary_5(int_Rk);
	std::cout << "instruccion en BINARIO ================  " << decod + bin_Rk + bin_Rj + bin_Ri << endl;
	unsigned long decimal = std::bitset<24>(decod + bin_Rk + bin_Rj + bin_Ri).to_ulong();
	std::cout << "instruccion en HEXA ================  " << dec_to_hexa(decimal) << endl;
	return dec_to_hexa(decimal);

}

string operaciones_de_ctes(string decod, string K, string Ri)
{
	int int_Ri = param_to_int(Ri, 1);
	string bin_Ri = dec_to_binary_5(int_Ri);

	K = K.substr(2);
	string bin_K = hexa_to_binary_16(K);

	std::cout << "instruccion en BINARIO =========" << decod + bin_K + bin_Ri << endl;
	unsigned long decimal = std::bitset<24>(decod + bin_K + bin_Ri).to_ulong();
	std::cout << "instruccion en HEXA ================  " << dec_to_hexa(decimal) << endl;
	return dec_to_hexa(decimal);
}

string operaciones_de_PCyMem(string decod, string K, string Ri)
{
	int int_Ri = param_to_int(Ri, 1);
	string bin_Ri = dec_to_binary_5(int_Ri);

	if (isalpha(K[0]))
	{
		std::stringstream stream;
		stream << std::hex << labels[K];
		K = stream.str();
	}
	else
	{
		K = K.substr(2);
	}

	string bin_K = hexa_to_binary_15(K);

	std::cout << "instruccion en BINARIO =========" << decod + bin_K + bin_Ri << endl;
	unsigned long decimal = std::bitset<24>(decod + bin_K + bin_Ri).to_ulong();
	std::cout << "instruccion en HEXA ================  " << dec_to_hexa(decimal) << endl;
	return dec_to_hexa(decimal);
}




void switch_instruc(int i, string param, string* param_array, ofstream& file_bin, ofstream& file_lst, int address)
{
	switch (i)
	{
	case MOK:
	{
		file_bin << operaciones_de_ctes("100", param, param_array[0]) << "\n";
		file_lst << operaciones_de_ctes("100", param, param_array[0]);
		break;
	}

	case ANK:
	{
		file_bin << operaciones_de_ctes("101", param, param_array[0]) << "\n";
		file_lst << operaciones_de_ctes("101", param, param_array[0]);
		break;
	}

	case ORK:
	{
		file_bin << operaciones_de_ctes("110", param, param_array[0]) << "\n";
		file_lst << operaciones_de_ctes("110", param, param_array[0]);
		break;
	}

	case ADK:
	{
		file_bin << operaciones_de_ctes("111", param, param_array[0]) << "\n";
		file_lst << operaciones_de_ctes("111", param, param_array[0]);
		break;
	}

	case JMP:
	{
		file_bin << operaciones_de_PCyMem("0100", param, "00000") << "\n";
		file_lst << operaciones_de_PCyMem("0100", param, "00000");
		break;
	}

	case JCY:
	{
		file_bin << operaciones_de_PCyMem("0101", param, "00000") << "\n";
		file_lst << operaciones_de_PCyMem("0101", param, "00000");
		break;
	}

	case JNE:
	{
		file_bin << operaciones_de_PCyMem("0110", param, param_array[0]) << "\n";
		file_lst << operaciones_de_PCyMem("0110", param, param_array[0]);
		break;
	}

	case JZE:
	{
		file_bin << operaciones_de_PCyMem("0111", param, param_array[0]) << "\n";
		file_lst << operaciones_de_PCyMem("0111", param, param_array[0]);
		break;
	}

	case MOM:
	{
		string primera_letra = param_array[0].substr(0, 1);
		if (primera_letra == "0")
		{
			file_bin << operaciones_de_PCyMem("0011", param_array[0], param) << "\n";
			file_lst << operaciones_de_PCyMem("0011", param_array[0], param);
		}
		else
		{
			file_bin << operaciones_de_PCyMem("0010", param, param_array[0]) << "\n";
			file_lst << operaciones_de_PCyMem("0010", param, param_array[0]);
		}
		break;
	}

	case BSR:
	{
		if (isalpha(param[0]))
		{
			//calcular el offset
			std::stringstream stream;
			stream << std::hex << labels[param] - address;
			param = "0x" + stream.str();	
		}

		file_bin << operaciones_de_PCyMem("0001", param, "00000") << "\n";
		file_lst << operaciones_de_PCyMem("0001", param, "00000");
		break;
	}

	case ADR:
	{
		file_bin << operaciones_de_registros("000010000", param, param_array[1], param_array[0]) << "\n";
		file_lst << operaciones_de_registros("000010000", param, param_array[1], param_array[0]);
		break;
	}

	case MOV:
	{
		file_bin << operaciones_de_registros("000010100", "00000", param, param_array[0]) << "\n";
		file_lst << operaciones_de_registros("000010100", "00000", param, param_array[0]);
		break;
	}

	case ORR:
	{
		file_bin << operaciones_de_registros("000011000", "00000", param, param_array[0]) << "\n";
		file_lst << operaciones_de_registros("000011000", "00000", param, param_array[0]);
		break;
	}

	case ANR:
	{
		file_bin << operaciones_de_registros("000011100", "00000", param, param_array[0]) << "\n";
		file_lst << operaciones_de_registros("000011100", "00000", param, param_array[0]);
		break;
	}

	case CPL:
	{
		file_bin << operaciones_de_registros("000001000", "00000", "00000", param) << "\n";
		file_lst << operaciones_de_registros("000001000", "00000", "00000", param);
		break;
	}

	case LSR:
	{
		file_bin << operaciones_de_registros("000001010", "00000", "00000", param) << "\n";
		file_lst << operaciones_de_registros("000001010", "00000", "00000", param);
		break;
	}

	case LSL:
	{
		file_bin << operaciones_de_registros("000001100", "00000", "00000", param) << "\n";
		file_lst << operaciones_de_registros("000001100", "00000", "00000", param);
		break;
	}

	case ASL:
	{
		file_bin << operaciones_de_registros("000001110", "00000", "00000", param) << "\n";
		file_lst << operaciones_de_registros("000001110", "00000", "00000", param);
		break;
	}

	case CLR:
	{
		std::cout << "020000" << endl;
		file_bin << "020000" << "\n";
		file_lst << "020000";
		break;
	}

	case SET:
	{
		std::cout << "030000" << endl;
		file_bin << "030000" << "\n";
		file_lst << "030000";
		break;
	}

	case RET:
	{
		std::cout << "010000" << endl;
		file_bin << "010000" << "\n";
		file_lst << "010000";
		break;

	}
	};
}


