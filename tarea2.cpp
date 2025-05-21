#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
using namespace std;

int main() {
    cout << "Juego Aventura - EDD Tarea 2" << endl;
    cout << "Creado por: Jorge Gahona y Matias Ibañez " << endl;

    ifstream archivo("ejemplo.map");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el Archivo" << endl;
        return 1;
    }
    cout << "Archivo abierto correctamente." << endl;
    archivo.close();
    return 0;

}