#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
using namespace std;

struct Habitacion {
    // Estructura de habitacion
    unsigned int id;
    string name;
    string desc;
    string tipo;
};

void leerHabitaciones(ifstream& archivo, Habitacion*& habitaciones, int& numHabitaciones) {

    string linea; //Lee la linea "Habitaciones"
    getline(archivo, linea);
    if (linea != "HABITACIONES"){
        cout << "Error: No se encontro la seccion HABITACIONES" << endl;
        return;
    }

    habitaciones = new Habitacion[numHabitaciones];
    getline(archivo, linea);     //Leer el numero de habitaciones
    numHabitaciones = stoi(linea); // Convierte la linea a int

    //Leer cada habitacion
    for (int i = 0; i < numHabitaciones; i++) { // Leer la linea del id, name, desc y tipo.
        getline(archivo, linea);
        
        stringstream ss(linea); //stringstream para convertir linea 
        unsigned int id;
        string name, tipo;

        ss >> id; //extrae id;
        getline(ss, name, '('); //Leemos hasta el '(', extraemos name
        name = name.substr(0, name.size()-1); // quitar espacio al final en name
        getline(ss, tipo, ')'); // Leer hasta ')', extraer tipo
        
        //Almacenamos la lista
        habitaciones[i].id = id;
        habitaciones[i].name = name;
        habitaciones[i].tipo = tipo;
        
        //extraer desc
        string desc = "";
        getline(archivo, desc);
        habitaciones[i].desc = desc;
    }
}









int main() {
    cout << "Juego Aventura - EDD Tarea 2" << endl;
    cout << "Creado por: Jorge Gahona y Matias Ibañez" << endl;

    ifstream archivo("ejemplo.map");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el Archivo" << endl;
        return 1;
    }

    string linea;
    getline(archivo, linea); // Leer "INICIO DE ARCHIVO"
    if (linea != "INICIO DE ARCHIVO") {
        cerr << "Error: no se encontró INICIO DE ARCHIVO" << endl;
        archivo.close();
        return 1;
    }

    Habitacion* habitaciones = nullptr;
    int numHabitaciones = 20; // Inicializar en 20, segun el archivo.map lo indique.
    leerHabitaciones(archivo, habitaciones, numHabitaciones);

    // Imprimir habitaciones
    for (int i = 0; i < numHabitaciones; i++) {
        cout << "Habitacion " << habitaciones[i].id << ": " << habitaciones[i].name
             << " (" << habitaciones[i].tipo << ")" << endl
             << "Descripcion: " << habitaciones[i].desc << endl;
    }

    archivo.close();
    delete[] habitaciones;
    return 0;
}