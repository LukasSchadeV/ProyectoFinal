#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <Windows.h>
#include <Psapi.h>

using namespace std;

// Windons propone la siguiente funcion para poder indentificar cuanta memoria utiliza un codigo
void PrintMemoryInfo() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc))) {
        std::cout << "Memory usage: " << pmc.PrivateUsage << " bytes" << std::endl;
    }
}
// Declara las coneciones que almacena las publicaciones en común entre el autor 2 autores
struct Conexion {
    string coautor;
    int publicaciones;
};
// Funcion simple que busca los numeros pares e imapares de la lista
void contarParesImpares(const vector<vector<Conexion>>& listaAdyacencia, int& pares, int& impares) {
    for (const auto& autor : listaAdyacencia) {
        for (const auto& conexion : autor) {
            if (conexion.publicaciones % 2 == 0) {
                pares++;
            } else {
                impares++;
            }
        }
    }
}

int main() {
    std::ifstream archivo("procesada.txt"); // Lee el archivo
    int M = 1000;  // Número de líneas a leer; cada una incluye las aristas nuevas
    int pares = 0; //inicializa los numeros pares
    int impares = 0; // inicializa los numeros impares

    std::string linea; // Lineas a leer
    std::unordered_map<std::string, int> autorIndice;  // Mapa para mapear autor a índice único
    std::vector<vector<Conexion>> listaAdyacencia;  // Arreglo de listas de adyacencia

    int lineasLeidas = 0;  // Contador de líneas procesadas

    // Construir el grafo de co-autoría
    while (std::getline(archivo, linea) && lineasLeidas < M) {
        std::istringstream iss(linea);
        std::string autor;
        std::vector<string> autoresArista;

        // Identificar los autores de cada linea
        while (std::getline(iss, autor, '%')) { 
            if (!autor.empty()) {
                autoresArista.push_back(autor);
            }
        }
        // Evaluar cada arista
        if (autoresArista.size() > 1) {
            // Agregar la arista a la lista de adyacencia
            for (int i = 0; i < autoresArista.size(); i++) {
                if (autorIndice.count(autoresArista[i]) == 0) {
                    autorIndice[autoresArista[i]] = listaAdyacencia.size();
                    listaAdyacencia.push_back(vector<Conexion>());
                }
                int indiceAutorActual = autorIndice[autoresArista[i]];
                //Asgina cada arista
                for (int j = 0; j < autoresArista.size(); j++) {
                    if (i != j) {
                        string coautor = autoresArista[j];
                        bool existeConexion = false;

                        // Verifica la lista de adyacencia
                        for (auto& conexion : listaAdyacencia[indiceAutorActual]) {
                            if (conexion.coautor == coautor) { //Si la conexión ya existe agrea +1
                                conexion.publicaciones++;
                                existeConexion = true;
                                break;
                            }
                        }
                        if (!existeConexion) { // En caso contrario, se agrega a la lista de adyacencia
                            Conexion nuevaConexion;
                            nuevaConexion.coautor = coautor;
                            nuevaConexion.publicaciones = 1;
                            listaAdyacencia[indiceAutorActual].push_back(nuevaConexion);
                        }
                    }
                }
            }
        }
        lineasLeidas++;//Siguiente linea
    }
    archivo.close();

    contarParesImpares(listaAdyacencia, pares, impares); // Imprimir los resultados requeridos
    cout << "Par: " << pares << endl;
    cout << "Impar: " << impares << endl;

    PrintMemoryInfo(); // Memoria

    return 0;
}
