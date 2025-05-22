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

void leerHabitaciones(ifstream& archivo, Habitacion habitaciones[], int& numHabitaciones) {
    string linea; //Lee la linea "Habitaciones"
    getline(archivo, linea);
    if (linea != "HABITACIONES"){
        cout << "Error: No se encontro la seccion HABITACIONES" << endl;
        return;
    }

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



struct Enemigo {
    string nombre;
    int vida, ataque;
    float precision, probabilidad;
};

void leerEnemigos(ifstream& archivo, Enemigo enemigos[], int& numEnemigos) {
    string linea; 
    getline(archivo, linea);    //Lee la linea "ENEMIGOS"
    if (linea != "ENEMIGOS"){
        cout << "Error: No se encontro la seccion ENEMIGOS" << endl;
        return;
    }

    getline(archivo, linea);     //Leer el numero de enemigos
    numEnemigos = stoi(linea);   //Convierte la linea a int

    //Leer cada enemigo
    for (int i = 0; i < numEnemigos; i++) { // Ciclo que se repite para cada enemigo
        getline(archivo, linea);  //Datos del enemigo
        stringstream ss(linea); //Stringstream para convertir linea 
        Enemigo enemigo; //Crear un objeto enemigo
        getline(ss, enemigo.nombre, '|'); //Guarda el nombre dentro de la casilla del objeto
        enemigo.nombre = enemigo.nombre.substr(0, enemigo.nombre.size()-1); //Quitar espacio al final en nombre
        ss.ignore(6); //Ignorar " Vida "
        ss >> enemigo.vida; //Guardar vida
        ss.ignore(10); //Ignorar " | Ataque "
        ss >> enemigo.ataque; //Guardar ataque
        ss.ignore(13); //Ignorar " | Precision "
        ss >> enemigo.precision; //Guardar precision
        ss.ignore(15); //Ignorar " | Probabilidad "
        ss >> enemigo.probabilidad; //Guardar probabilidad

        enemigos[i] = enemigo; //Guardar enemigo en el arreglo enemigos

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

    Habitacion habitaciones[100];
    int numHabitaciones = 0; // Inicializar en 0
    leerHabitaciones(archivo, habitaciones, numHabitaciones);

    // Imprimir habitaciones
    for (int i = 0; i < numHabitaciones; i++) {
        cout << "Habitacion " << habitaciones[i].id << ": " << habitaciones[i].name
             << " (" << habitaciones[i].tipo << ")" << endl
             << "Descripcion: " << habitaciones[i].desc << endl;
    }

    Enemigo enemigos[100];
    int numEnemigos = 0; // Inicializar en 0
    leerEnemigos(archivo, enemigos, numEnemigos);

    // Imprimir enemigos
    for (int i = 0; i < numEnemigos; i++) {
        cout << "Enemigo " << i + 1 << ": " << enemigos[i].nombre
             << " | Vida: " << enemigos[i].vida
             << " | Ataque: " << enemigos[i].ataque
             << " | Precision: " << enemigos[i].precision
             << " | Probabilidad: " << enemigos[i].probabilidad << endl;
    }


    archivo.close();
    return 0;
}