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

struct Arco {
    unsigned int origen, destino;
};

/**
*void leerHabitaciones
*Input:
* ifstream& archivo: Archivo abierto en posicion de linea 2 HABITACIONES
* Habitacion*& habitaciones: arreglo vacio de habitaciones
* int& numHabitaciones: cantidad de habitaciones, numero entero
*returns:
*Es void pero extrae del archivo .map toda la informacion de las habitaciones del juego
**/
void leerHabitaciones(ifstream& archivo, Habitacion*& habitaciones, int numHabitaciones) {

    string linea; //Lee la linea "Habitaciones"

    habitaciones = new Habitacion[numHabitaciones];

    //Leer cada habitacion
    for (int i = 0; i < numHabitaciones; i++) { // Leer la linea del id, name, desc y tipo.
        getline(archivo, linea);
        
        stringstream ss(linea); //stringstream para convertir linea 
        unsigned int id;
        string name, tipo, desc;

        ss >> id; //extrae id;
        getline(ss, name, '('); //Leemos hasta el '(', extraemos name
        name = name.substr(0, name.size()-1); // quitar espacio al final en name
        getline(ss, tipo, ')'); // Leer hasta ')', extraer tipo
        
        //Almacenamos la lista
        habitaciones[i].id = id;
        habitaciones[i].name = name;
        habitaciones[i].tipo = tipo;
        
        //extraer desc
        desc = "";
        getline(archivo, desc);
        habitaciones[i].desc = desc;
    }
}

/**
*void leerArcos
*lee arcos y los almacena en un arreglo dinamico de arcos
*
*
*
*
*
*
*
*
**/
void leerArcos(ifstream& archivo, Arco*& arcos, int numArcos){
    arcos = new Arco[numArcos];
    string linea;
    for (int i=0; i < numArcos; i++){
        getline(archivo, linea);
        stringstream ss(linea);
        string flecha;
        ss >> arcos[i].origen >> flecha >> arcos[i].destino;
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

    string* linea = new string;

    while (*linea != "HABITACIONES"){ //ciclo hasta llegar a HABITACIONES
        getline(archivo, *linea); // obtener linea
        if (*linea != "INICIO DE ARCHIVO" && *linea != "HABITACIONES"){ //verificar que cumpla con el formato
            cerr << "El archivo no posee el formato esperado" << endl; //si no cumple mensaje de error
            archivo.close();
            return 1;
        }
    }
    
    Habitacion* habitaciones = nullptr;
    int* numHabitaciones = new int;
    ; // Inicializar para guardar cantidad de habitaciones

    getline(archivo, *linea);
    *numHabitaciones = stoi(*linea);
    leerHabitaciones(archivo, habitaciones, *numHabitaciones);

    // Imprimir habitaciones
    for (int i = 0; i < *numHabitaciones; i++) {
        cout << "Habitacion " << habitaciones[i].id << ": " << habitaciones[i].name
             << " (" << habitaciones[i].tipo << ")" << endl
             << "Descripcion: " << habitaciones[i].desc << endl;
    }
    // Aca termina justo antes de linea ARCOS

    getline(archivo, *linea);
    if (*linea != "ARCOS"){
        cerr<< "Error al leer seccion ARCOS"<< endl;
        return 1;
    }
    getline(archivo, *linea);
    int* numArcos = new int;
    Arco* arcos = nullptr;

    *numArcos = stoi(*linea);
    leerArcos(archivo, arcos, *numArcos);

    for (int i= 0; i < *numArcos; i++){
        cout << arcos[i].origen << "->" << arcos[i].destino<<endl;

    }
    



    archivo.close();
    delete linea;
    delete[] habitaciones;
    delete numHabitaciones;
    delete[] arcos;
    delete numArcos;
    return 0;
}