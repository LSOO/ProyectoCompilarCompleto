#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <map>
#include <stack> // Pila
using namespace std;



/// Funcion para cargar los Simbolos Terminales
vector<string> CargarTerminales()
{
	vector<string> Termi;
	
	ifstream Archivo("TXTSintactico/Terminales.txt");
	
	
	string Ter;
	
	Archivo >> Ter;
	Termi.push_back(Ter);
	
	while(!Archivo.eof())
	{
		Archivo >> Ter;
		
		Termi.push_back(Ter);
	}
	
	Archivo.close();
	
	return Termi;
}



/// Funcion para cargar los No Terminales
vector<string> CargarNoTerminales()
{
	vector<string> NTermi;
	
	ifstream Archivo("TXTSintactico/No_Terminales.txt");
	
	
	string NTer;
	
	Archivo >> NTer;
	NTermi.push_back(NTer);
	
	while(!Archivo.eof())
	{
		Archivo >> NTer;
		
		NTermi.push_back(NTer);
	}
	
	Archivo.close();
	
	return NTermi;
}



/// Funcion que retorna un vector con la transiciones de NoTerminal de Acuerdo al Terminal que lee
vector<string> TransicionLL1(vector<string> NoTer, vector<string> Ter, string Terminal, string NoTerminal)
{
	
	int PosNT, PosT;
	
	for(int b = 0; b < int(NoTer.size()); b++)
	{
		if(NoTer[b] == NoTerminal)
		{
			PosNT = b;
			break;
		}
	}
	
	
		for(int b = 0; b < int(NoTer.size()); b++)
	{
		if(Ter[b] == Terminal)
		{
			PosT = b;
			break;
		}
	}
	
	
	vector<string> Trans;
	
	
	ifstream Archivo("TXTSintactico/Transiciones_LL1.txt");
	vector<string> te;
	/// Cargo La matriz que esta en el TXT
	for(int b = 0; b < int(NoTer.size()); b++)
	{
		
		for(int c = 0; c < int(Ter.size()); c++)
		{
			vector<string> Tem;
			
			
			/// Leo el de Valores que hay en la Transicion
			string Tam;
			Archivo >> Tam;
			
			
			/// Leo el primero Letra
			string s1;
			string s2;
			Archivo >> s1;
			Tem.push_back(s1);
			if(Tam == "2") /// Caundo hay mas de 1 valor para leer
			{
				Archivo >> s2;
				Tem.push_back(s2);		
			}
			if((b*7 + c) == (PosNT*7 + PosT))
			{
				return Tem;
			}
		}
	}
		
	
	Archivo.close();
}
	

/// Funcion que busca si cadena es un nodo Terminal o no
bool EsTerminal(string Valor, vector<string> Terminal)
{
	bool Esta = false;

	for(int b = 0; b < int(Terminal.size()); b++)
	{
		if(Terminal[b] == Valor)
		{
			Esta = true;
			break;
		}
	}
	
	return Esta;
}


void TipoError(string Valor)
{
	if (Valor == "Fecha")
	{
		cout << "Se esperaba:   Dia o Mes" << endl;
	}
	else if(Valor == "X")
	{
		cout << "Se esperaba: Guion, Slash o SeparDe" << endl;
	}
	else if(Valor == "X2" or Valor=="X3" or Valor=="X4")
	{
		cout << "Se esperaba: Mes" << endl;
	}
	else if (Valor=="X5")
	{
		cout << "Se esperaba: Guion" << endl;
	}
	else if(Valor=="X6")
	{
		cout << "Se esperaba: Slash" << endl;
	}
	else if(Valor=="X7")
	{
		cout << "Se esperaba: SeparDe" << endl;
	}
	else if(Valor=="X8")
	{
		cout << "Se esperaba: Ano" << endl;
	}
	else if (Valor=="Y")
	{
		cout << "Se esperaba: Dia" << endl;
	}
	else if(Valor=="Y2")
	{
		cout << "Se esperaba: SeparDe" << endl;
	}
}


/// Funcion para Imprimir el Estado de la Pila
void EstadoPila(stack<string> Pil)
{
	while(!Pil.empty())
	{
		cout << Pil.top() << endl;
		Pil.pop();
	}
}

/// Funcion que hace el Analisis Sintactico de una Cadena, despues de hacer el Analisis Lexico
/// Le ingresan varios parametros, porq no los pude cargar en la matriz
void AnalisisSintactico(vector<string> Fecha, vector<string> Termi, vector<string> NoTermi, vector<string> SubCadenaF)
{
	stack<string> Pila;
	Pila.push("$");
	Pila.push("Fecha");
	bool Es = true;
	
	int c = 0;
	for(int b = 0; b < int(Fecha.size()); )
	{
		cout << "----------------------------------------------------------" << endl;
		cout <<"Apuntador en Fecha:  " <<  Fecha[b] << endl;
		cout << "Estado Actual Pila" << "\n" << endl;
		EstadoPila(Pila);
		cout << endl;
		
		if((Pila.top() == "$") or (EsTerminal(Pila.top(), Termi)==true))
		{
			if(Pila.top() == Fecha[b])
			{
				cout << "Se Sacara:   " << Pila.top() << "\n\n\n" << endl;
				Pila.pop();
				b++;
				c++;
				cout << "Estado Siguiente Pila " << endl;
				EstadoPila(Pila);
				cout << "\n\n" << endl;
			}
			else
			{
				
				cout << "Error, La Cadena no Pertenece a Ningun Formato de Fecha Establecido" << "\n\n\n" << endl;
				break;
				
			}
			
		}
		else
		{
			vector<string> Temporal;
			Temporal = TransicionLL1(NoTermi, Termi, Fecha[b], Pila.top());
			
			if(Temporal[0] == "-1")
			{
				cout << "----------------------------------------------------------" << endl;
				cout << endl;
				cout << "Error Analisis Sintactico" << endl;
				cout << endl;
				for(int d = 0; d < int(Fecha.size()); d++)
				{
					cout << SubCadenaF[d] <<" ";
				}
				cout << endl;
				for(int d = 0; d < int(Fecha.size()); d++)
				{
					cout << Fecha[d] <<" ";
				}
				cout << endl;
				cout << "En el Token " << b+1 << " de la Cadena:  (" << Fecha[b] << ", " << SubCadenaF[b] << "),   ";
				TipoError(Pila.top());
				cout << "\n\n" << endl;
				Es = false;
				break;
				
			}
			else
			{
				cout << "Con: " << Pila.top();
				if(Temporal.size() == 1)
				{
					Pila.pop();
					Pila.push(Temporal[0]);
					cout << "\tSe produce:   " << Temporal[0] << "\n\n\n\n" << endl;
					cout << "Estado Siguiente Pila " << endl;
					EstadoPila(Pila);
					cout << "\n\n" << endl;
				}
				else
				{
					string a;
					a = Pila.top();
					Pila.pop();
					Pila.push(Temporal[1]);
					Pila.push(Temporal[0]);
					
					cout << "\tSe produce:   " << Temporal[0] << "\t" << Temporal[1] << "\n\n\n\n" << endl;
					cout << "Estado Siguiente Pila " << endl;
					EstadoPila(Pila);
					cout << "\n\n" << endl;
				}
			}
		}
	}
	cout << "----------------------------------------------------------" << endl;
	
	if(Pila.top() == "$" and (c==int(Fecha.size())))
	{
		cout << "\n\nFecha Sintacticamente Correcta\n  " << endl;
		for(int d = 0; d < int(Fecha.size()); d++)
		{
			cout << SubCadenaF[d] <<" ";
		}
		cout << "\n\n" << endl;
	}
	else if(Es == true)
	{
		cout << "\nFecha Sintacticamente Incorrecta\n\n  " << endl;
		for(int d = 0; d < int(Fecha.size()); d++)
		{
			cout << SubCadenaF[d] <<" ";
		}
		cout << "\n\n" << endl;
	}
	
}




/*
int main()
{
	vector<string> NoTerminales;
	NoTerminales = CargarNoTerminales();
	
	cout << "NO TERMINALES  " << endl;
	for(int b = 0; b < int(NoTerminales.size()); b++)
	{
		cout << "No Terminal:   " << NoTerminales[b] << endl;
	}
	cout << "\n\n" << endl;




	cout << "TERMINALES  " << endl;
	vector<string> Terminales;
	Terminales = CargarTerminales();
	for(int b = 0; b < int(Terminales.size()); b++)
	{
		cout << "Terminal:   " << Terminales[b] << endl;
	}
	
	
	cout << "\n\n\nInicion Analisis Sintactico \n" << endl;
	
	
	vector<string> Fecha;;
	Fecha.push_back("Mes");
	Fecha.push_back("Dia");
	Fecha.push_back("SeparDe");
	
	Fecha.push_back("Ano");
	
	
	
	vector<string> sub;
	sub.push_back("Mayo");
	sub.push_back("10");
	sub.push_back("de");
	sub.push_back("Ago");
	

	AnalisisSintactico(Fecha, Terminales, NoTerminales, sub);
	
	
	return 0;
}*/
	
	
