#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <fstream>

using namespace std;

enum {MOK=0,ANK,ORK,ADK,JMP,JCY,JNE,JZE,MOM,BSR,ADR,MOV,ORR,ANR,CPL,LSR,LSL,ASL,CLR,SET,RET};


string dec_to_hexa(int dec_num);
string hexa_to_binary_16(string hexa_num);
string hexa_to_binary_15(string hexa_num);
string dec_to_binary_5(int dec_num);
int param_to_int(string param, int i);
string operaciones_de_registros(string decod, string Rk, string Rj, string Ri);
string operaciones_de_ctes(string decod, string K, string Ri);
string operaciones_de_PCyMem(string decod, string K, string Ri);
void switch_instruc(int i, string param, string* param_array, ofstream& file);

int main() {
    const char* letras_intruction[21]{ "MOK","ANK","ORK","ADK","JMP","JCY","JNE","JZE","MOM","BSR",
                                       "ADR","MOV","ORR","ANR","CPL","LSR","LSL","ASL","CLR","SET","RET" };

    ifstream file_lectura("INSTRUCCIONES.txt");
    ofstream file_escritura("codigo.txt");

    string ingreso;
    while (getline(file_lectura, ingreso)) 
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
        cout << "param ====== " << param << endl;
        cout << "param array 0 =========" << param_array[0] << endl;
        cout << "param array 1 =========" << param_array[1] << endl;

        int i = 0;
        while (strcmp(instruc.c_str(), letras_intruction[i]))
        {
            i++;
        }
        switch_instruc(i, param, param_array, file_escritura);
    }

    file_lectura.close();
    file_escritura.close();
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
    cout << "instruccion en BINARIO ================  " << decod + bin_Rk + bin_Rj + bin_Ri << endl;
    unsigned long decimal = std::bitset<24>(decod + bin_Rk + bin_Rj + bin_Ri).to_ulong();
    cout << "instruccion en HEXA ================  " << dec_to_hexa(decimal) << endl;
    return dec_to_hexa(decimal);

}

string operaciones_de_ctes(string decod, string K, string Ri)
{
    int int_Ri = param_to_int(Ri, 1);
    string bin_Ri = dec_to_binary_5(int_Ri);

    K = K.substr(2);
    string bin_K = hexa_to_binary_16(K);

    cout << "instruccion en BINARIO =========" << decod + bin_K + bin_Ri << endl;
    unsigned long decimal = std::bitset<24>(decod + bin_K + bin_Ri).to_ulong();
    cout << "instruccion en HEXA ================  " << dec_to_hexa(decimal) << endl;
    return dec_to_hexa(decimal);
}

string operaciones_de_PCyMem(string decod, string K, string Ri)
{
    int int_Ri = param_to_int(Ri, 1);
    string bin_Ri = dec_to_binary_5(int_Ri);

    K = K.substr(2);
    string bin_K = hexa_to_binary_15(K);

    cout << "instruccion en BINARIO =========" << decod + bin_K + bin_Ri << endl;
    unsigned long decimal = std::bitset<24>(decod + bin_K + bin_Ri).to_ulong();
    cout << "instruccion en HEXA ================  " << dec_to_hexa(decimal) << endl;
    return dec_to_hexa(decimal);
}




void switch_instruc(int i, string param, string* param_array, ofstream& file)
{
    switch (i)
    {
    case MOK:
    {
        file << operaciones_de_ctes("100", param, param_array[0]) << "\n";
        break;
    }

    case ANK:
    {
        file <<  operaciones_de_ctes("101", param, param_array[0]) << "\n";
        break;
    }

    case ORK:
    {
        file <<  operaciones_de_ctes("110", param, param_array[0]) << "\n";
        break;
    }

    case ADK:
    {
        file << operaciones_de_ctes("111", param, param_array[0]) << "\n";
        break;
    }

    case JMP:
    {
        file << operaciones_de_PCyMem("0100", param, "00000") << "\n";
        break;
    }

    case JCY:
    {
        file << operaciones_de_PCyMem("0101", param, "00000") << "\n";
        break;
    }

    case JNE:
    {
        file << operaciones_de_PCyMem("0110", param, param_array[0]) << "\n";
        break;
    }

    case JZE:
    {
        file << operaciones_de_PCyMem("0111", param, param_array[0]) << "\n";
        break;
    }

    case MOM:
    {
        string primera_letra = param_array[0].substr(0, 1);
        if (primera_letra == "0")
        {
            file << operaciones_de_PCyMem("0011", param_array[0], param) << "\n";
        }
        else
        {
            file << operaciones_de_PCyMem("0010", param, param_array[0]) << "\n";
        }
        break;
    }

    case BSR:
    {
        file << operaciones_de_PCyMem("0001", param, "00000") << "\n";
        break;
    }

    case ADR:
    {
        file << operaciones_de_registros("000010000", param, param_array[1], param_array[0]) << "\n";
        break;
    }

    case MOV:
    {
        file << operaciones_de_registros("000010100", "00000", param, param_array[0]) << "\n";
        break;
    }

    case ORR:
    {
        file << operaciones_de_registros("000011000", "00000", param, param_array[0]) << "\n";
        break;
    }

    case ANR:
    {
        file << operaciones_de_registros("000011100", "00000", param, param_array[0]) << "\n";
        break;
    }

    case CPL:
    {
        file << operaciones_de_registros("000001000", "00000", "00000", param) << "\n";
        break;
    }

    case LSR:
    {
        file << operaciones_de_registros("000001010", "00000", "00000", param) << "\n";
        break;
    }

    case LSL:
    {
        file << operaciones_de_registros("000001100", "00000", "00000", param) << "\n";
        break;
    }

    case ASL:
    {
        file << operaciones_de_registros("000001110", "00000", "00000", param) << "\n";
        break;
    }

    case CLR:
    {
        cout << "020000" << endl;
        file << "020000" << "\n";
        break;
    }

    case SET:
    {
        cout << "030000" << endl;
        file << "030000" << "\n";
        break;
    }

    case RET:
    {
        cout << "010000" << endl;
        file << "010000" << "\n";
        break;

    }
    };
}


