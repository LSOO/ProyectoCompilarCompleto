#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <map>
using namespace std;




/// Funcion para cargar el alfabeto del Automata
vector<char> CargarAlfabeto()
{
	
	vector<char> Alfa;
	ifstream Archivo("TXTLexico/Alfabeto.txt");
	
	
	
	char Letra;
	Archivo >> Letra;
	Alfa.push_back(Letra);
	while(!Archivo.eof())
	{
		
		Archivo >> Letra;
		
		Alfa.push_back(Letra);
	}
	
	Archivo.close();
	
	return Alfa;
}




/// Funcion para Cargar los Estados del Automata
vector<int> CargarEstados()
{
	vector<int> Esta;
	ifstream Archivo("TXTLexico/Estados.txt");
	
	
	
	int Letra;
	Archivo >> Letra;
	Esta.push_back(Letra);
	while(!Archivo.eof())
	{
		
		Archivo >> Letra;
		
		Esta.push_back(Letra);
	}
	
	Archivo.close();
	
	return Esta;
}



/// Funcion para Cargar la Matriz de incidencia del Automata
int** Cargar(int CanLeAlfa, int CanEstados)
{
	
	//cout << CanLeAlfa << "  " << CanEstados << endl;
	
	ifstream Archivo("TXTLexico/Transiciones_Estados.txt");
	int **Matriz;
	
	
	Matriz = (int**)malloc(CanEstados * sizeof(int*));
	
	for(int b = 0; b < CanEstados; b++)
	{
		Matriz[b] = (int*)malloc(CanLeAlfa * sizeof(int));
	}
	
	
	for(int x = 0; x < CanEstados - 1; x++)
	{
		for(int y = 0; y < CanLeAlfa -1; y++)
		{
			int Le;
			Archivo >> Le;
			///cout << Le << "  ";
			Matriz[x][y] = Le;
		}
		///cout << endl;
	}
	
	Archivo.close();
	/// 11, 9 Matriz[Estado][PosLetraAlfabeto
	
	return Matriz;
	
}



/// Modifica la cadena entrante y le agregar los caracteres
string ModiEntrada(string Cadena)
{
	for(int b = 0; b < int(Cadena.size()); b++)
	{
		if(Cadena[b]== 32) /// Espacion en Blanco ASCII
		{
			Cadena[b] = '+';
		}
	}
	Cadena.append("|");
	
	return Cadena;
}


/// Sacar los subcadenas de las subcadenas para luego ser evaluadas en el automata
vector<string> SubCadenas(string Cadena)
{
	string tmp = ModiEntrada(Cadena);
	/// Ciclo para parti las cadenas y dejarlas sin espacio
	vector<string> tempo;
	int b = 0;
	while(b < int(tmp.size()))
	{
		if(tmp[b] == '+' or tmp[b] == '|')
		{
			
			if(int(tmp.substr(0,b).size()) > 0)
			{
				//cout << "Sub Cadena sin espacio:    " << tmp.substr(0,b) << endl;
				string tm = tmp.substr(0,b);
				tm.append("|");
				tempo.push_back(tm);
			}
			tmp.erase(0, b + 1);
			b = 0;
		}
		else
		{
			b++;
		}		
	}
	
	/// Vector final de subCadenas
	vector<string> Final;
	
	/// ciclo para sacar las subCadenas de cada SubCadena
	for(int x = 0; x < int(tempo.size()); x++)
	{
		
		int c = 0;
		string tmp2 = tempo[x];
		//cout << "Cadena Para Revisar:    " << tmp2 << endl;
		while(c < int(tmp2.size()))
		{
			if(tmp2[c] == '/' or tmp2[c] == '-' or tmp2[c] == '|')
			{
				/// Condicion cuando antes del simbolo no hay nada ij /	
				if(int(tmp2.substr(0,c).size()) == 0)
				{
					/// Condicion para cuando este un solo caracter si esta / se Agrega al vector de subCadenas
					if(tmp2[0] != '|')
					{
						//cout << "Para Agregar 1:	" << tmp2[0] << endl;
						string a = "";
						a = a + tmp2[0];
						Final.push_back(a);
					}
				}
				
				else
				{
					//cout << "Para Agregar 4:	" << tmp2.substr(0,c) << endl;
					Final.push_back(tmp2.substr(0,c));
					
					
					
					if(tmp2[c] != '|')
					{
						//cout << "Para Agregar 3:	" << tmp2[c] << endl;
						string a = "";
						a = a + tmp2[c];
						Final.push_back(a);
					}
				}
				tmp2.erase(0,c+1);
				c= 0;
			}
			else
				c++;
		}
	}
	
	return Final;
	
}



/// Funcion para transicion del estados, para el estado siguiente
int EstaSig(vector<char> Alfabeto, vector<int> Estados, int** MatrizTrans, int EstaActual, char Letra)
{
	int PosEA; /// Posicion del Estado Actual
	/// Para buscar la posicion del estado actual
	for(int b = 0; b < int(Estados.size()); b++)
	{
		if(Estados[b] == EstaActual)
		{
			PosEA = b;
			break;
		}
	}
	
	
	
	int PosLetra;
	PosLetra = -2; /// -2 para saber q esta en el alfabeto, si existe en el alfabeto se cambia
	for(int b = 0; b < int(Alfabeto.size()); b++)
	{
		if(Alfabeto[b] == Letra)
		{
			PosLetra = b;
			break;
		}
	}
	
	
	if(PosLetra == -2) /// -1 porq la letra no pertenece al lenguaje del automata
	{
		return -2;
	}
	else
	{
		return MatrizTrans[PosEA][PosLetra];
	}	
}



/// Retorna un map, con los estados finales y sus posibles terminaciones osea 1 puede ser Mes, Ano, Dia
multimap<int,string> CargarEstFinales()
{
	multimap<int,string> EFinales;
	ifstream Archivo("TXTLexico/Estados_Finales.txt");
	
	
	
	/// Carga los estados Finales con sus posibles tipos, la llave es el num de estado
	while(!Archivo.eof())
	{
		int EFinal;
		Archivo >> EFinal;
		
		int Can;
		Archivo >> Can; /// Lee la cantidad de posibilidades q tiene cada estado
		
		for(int b= 0; b < Can; b++)
		{
			string Pos;
			Archivo >> Pos;
			EFinales.insert(pair<int,string>(EFinal,Pos));  
		}
			
	}
	
	
	Archivo.close();

	return EFinales;
}




/// Funcion que imprime las posibles combinaciones de los token de acuerdo al estado final en el q queda
string PosiToken(multimap<int,string> EFinales, string Cadena, int EstadoFinal)
{
	string Valor;
	/// iterador de ese map
	multimap<int,string>::iterator it;
	
	
	/// 
	pair<multimap<int,string>::iterator,multimap<int,string>::iterator> ret;
	
	
	ret = EFinales.equal_range(EstadoFinal);
			
	it=ret.first; /// No tiene ningun Dato
	if(it == ret.second)
	{
		Valor = "Rechazada";
	}
	else
	{
		Valor = (*it).second;
	}
	
	return Valor;
}

