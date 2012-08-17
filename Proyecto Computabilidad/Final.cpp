#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <map>

/// Se Incluye las Funciones del Analisador Lexico
#include "Lexico.cpp"
#include "Analisis_Sintactico.cpp"
using namespace std;



int main()
{
	
	/*
	 * Parte Para hacer el Analisis Lexico
	 * 
	 * */
	vector<int> Estados;
	vector<char> Alfabeto;
	multimap<int,string> EstadosFinales;
	
	/// Se Cargador los Estados en un Vector
	Estados = CargarEstados();
	
	
	
	/// Se Carga el Alfabeto del Automata en un Vector
	Alfabeto = CargarAlfabeto();
	
	
	
	/// Se Cargan lo  Estados de Aceptacion del Automata
	EstadosFinales = CargarEstFinales();
	
	
	
	/// Se carga la Matriz de Transiciones del Automata, Matriz de Incidencia Estados vs Alfabeto
	int **Transiciones;
	Transiciones = Cargar(int(Alfabeto.size()), int(Estados.size()));  
	
	
	
	
	/// Parte donde se pide que ingrese la Cadena a Analizar
	char Cadena[500];
	cout << "Ingrese Cadena:   ";
	cin.getline(Cadena, 500);
	vector<string> Prueba = SubCadenas(Cadena);
	
	
	/// Parte donde se inicia el Analisis Lexico
	cout << "\n\n" << endl;
	cout << "Analisis Lexico Cadena:    " << Cadena << "\n" << endl;

	
	/// Variable que indica si Paso el Analisis Lexico
	bool PasLexico = true;
	
	/// Variable donde se Guarda los Tokens de la Cadena
	vector<string> Tokens;
	
	/// Recorrido de las subCadenas a la cuales le va a hacer analisis lexico
	for(int b = 0; b < int(Prueba.size()); b++)
	{
		
		/// Condicion para saber si continua con el Analisis Lexico
		if(PasLexico == true)
		{
			int EstadoActual = 0;
			bool Impri = true;
			for(int c = 0; c < int(Prueba[b].size()); c++)
			{
				EstadoActual = EstaSig(Alfabeto, Estados, Transiciones, EstadoActual, Prueba[b][c]);
				if(EstadoActual == -1)
				{
					cout << "No Pertenece:   " << Prueba[b] << endl;
					Impri = false;
					cout << "\n\n" << endl;
					PasLexico = false;
					break;
				}
				else
				{
					if(EstadoActual == -2)
					{
						cout << "La Cadena tiene una Cadena que no pertenece al Lenguaje:    " << endl;
						Impri = false;
						cout << "\n\n" << endl;
						PasLexico = false;
						break;
					}
				}
			}
			if(Impri == true)
			{
				string a;
				a = PosiToken(EstadosFinales, Prueba[b], EstadoActual);
				if(a != "Rechazada")
				{
					cout << "(" <<a << ",  " << Prueba[b] << ")" << endl;
					Tokens.push_back(a);
				}
				else
				{
					cout << "Cadena Rechazada:   " << Prueba[b] << endl;
					break;
					PasLexico = false;
				}
			}
		}
		else
		{
			break;
		}
	}

	/*
	 * Fin Parte Analisis Lexico
	 * */
	 
	 
	 if(PasLexico == false)
	 {
		 cout << "\n\nFallo el Analisis Lexico, hay Cadenas que no Pertenecen al Lenguaje" << endl;
		 cout << "Por lo tanto no se puede hacer el Analsis Lexico \n\n\n\n" << endl;
	 }
	 else
	 {
		 cout << "\n\n\n\nInicio de Analis Sintactico \n" << endl;
		 
		vector<string> NoTerminales;
		NoTerminales = CargarNoTerminales();
		cout << "\n\n" << endl;



		vector<string> Terminales;
		Terminales = CargarTerminales();


		AnalisisSintactico(Tokens, Terminales, NoTerminales, Prueba);
	 }


	
	return 0;
}
