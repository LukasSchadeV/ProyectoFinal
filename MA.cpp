#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <Windows.h>
#include <Psapi.h>

using namespace std;

//Funcion encargada de medir la memoria utilizada por el codigo, propuesta por windows
// https://learn.microsoft.com/en-us/windows/win32/psapi/collecting-memory-usage-information-for-a-process
void GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc))) {
        std::cout << "Memory usage: " << pmc.PrivateUsage << " bytes" << std::endl;
    }
}
// Función para contar los números pares e impares diferentes de cero en la matriz de adyacencia
void contarParesImpares(const vector<vector<int>>& matriz, int& pares, int& impares) {
    int n = matriz.size();
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matriz[i][j] != 0 && matriz[i][j] % 2 == 0) { // Decidir si es par, y excluir el 0
                pares++;
            } else if (matriz[i][j] != 0) { // Impares // se le puede añadir (&& matriz[i][j] != 1) para verificar los distintos de 1 
                impares++;
            }
        }
    }
}

int main() {
    std::string nombreArchivo = "procesada.txt";
    int M = 1000;  // Líneas que se leen; cada una incluye las aristas nuevas

    std::ifstream archivo(nombreArchivo);
    std::string linea;
    int lineasLeidas = 0;

    // Construir el conjunto de autores únicos
    std::unordered_set<std::string> autoresUnicos;

    // Construir la lista de conexiones para cada autor
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> conexiones;

    // Procesar líneas y construir lista de adyacencia
    while (std::getline(archivo, linea) && lineasLeidas < M) {
        std::istringstream iss(linea);
        std::string autor;

        std::vector<std::string> nombresAutores;
        while (std::getline(iss, autor, '%')) {
            if (!autor.empty()) {
                autoresUnicos.insert(autor);
                nombresAutores.push_back(autor);
            }
        }

        // Crear conexiones para el conjunto de autores en la línea actual
        for (int i = 0; i < nombresAutores.size(); i++) {
            for (int j = i + 1; j < nombresAutores.size(); j++) {
                conexiones[nombresAutores[i]].emplace_back(nombresAutores[j], 1); //La función emplace_back se utiliza en C++ para agregar un elemento al final de un vector o lista de manera eficiente
                conexiones[nombresAutores[j]].emplace_back(nombresAutores[i], 1);
            }
        }

        lineasLeidas++;
    }
    archivo.close();

    // Construir la matriz de adyacencia con los pesos correspondientes
    vector<vector<int>> matrizAdyacencia(autoresUnicos.size(), vector<int>(autoresUnicos.size(), 0));

    // Asignar los pesos de las conexiones en la matriz de adyacencia
    int indice = 0;
    std::unordered_map<std::string, int> autorIndice;
    for (const std::string& autor : autoresUnicos) {
        autorIndice[autor] = indice++;
    }

    for (const auto& conexion : conexiones) {
        int indice1 = autorIndice[conexion.first];
        for (const auto& coautor : conexion.second) {
            int indice2 = autorIndice[coautor.first];
            matrizAdyacencia[indice1][indice2] += coautor.second;
        }
    }

    // Contar los números pares e impares diferentes de cero en la matriz de adyacencia
    int pares = 0;
    int impares = 0;
    contarParesImpares(matrizAdyacencia, pares, impares);

    // Imprimir los resultados
    cout << "Pares: " << pares << endl;
    cout << "Impares: " << impares << endl;

    GetMemoryUsage();
    return 0;
}
