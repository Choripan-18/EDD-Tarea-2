#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
using namespace std;

struct Habitacion { //Estructura para almacenar los datos de cada habitacion
    int id;
    string nombre;
    string tipo;
    string descripcion;
};




int main() {
    cout << "Juego Aventura - EDD Tarea 2" << endl;
    cout << "Creado por: Jorge Gahona y Matias Ibañez " << endl;

    ifstream archivo("ejemplo.map");
    if (!archivo.is_open()) {
        cout << "Error al abrir el Archivo." << endl;
        return 1;
    }

    cout << "Archivo abierto correctamente." << endl;

    //Buscar habitaciones
    string linea;
    while (getline(archivo, linea)) {
        if (linea.find("HABITACIONES") != string::npos) break;
    }

    int CantidadDeHabitaciones = 0;
    archivo >> CantidadDeHabitaciones;
    archivo.ignore();                   // Ignorar el salto de línea después del número de habitaciones

    Habitacion* habitaciones = new Habitacion[CantidadDeHabitaciones]; //Crea un arreglo de habitaciones

    for(int i = 0; i < CantidadDeHabitaciones; i++) {       //Ciclo que se repite la cantidad de habitaciones que tenga el archivo
        
        int id;                                             //Declaracion de vcariables para guardar los datos de la habitacion
        string nombre, tipo, descripcion;
        getline(archivo, linea);                          //Lee la linea de la habitacion
        stringstream ss(linea);                             //Crear un stringstream para leer la linea
        ss >> id;
        getline(ss, nombre, '(');                           //Lee hasta antes del tipo y lo guarda en nombre
        nombre = nombre.substr(0, nombre.size() - 1);       //Elimina el espacio al final
        getline(ss, tipo, ')');                             //Lee hasta el ) y lo guarda en tipo
        getline(archivo, descripcion);                      //Lee la siguiente linea y lo guarda en descripcion

        //Guardar los datos de la habitacion en el arreglo
        habitaciones[i].id = id;
        habitaciones[i].nombre = nombre;
        habitaciones[i].tipo = tipo;
        habitaciones[i].descripcion = descripcion;

    } //Habitaciones guardadas.

    // * PRUEBA * mostrar habitaciones para ver que se guardaron

    cout << "\nHabitaciones encontradas:\n";
    for (int i = 0; i < CantidadDeHabitaciones; i++) {
        cout << habitaciones[i].id << " - " << habitaciones[i].nombre << " (" << habitaciones[i].tipo << ")\n";
        cout << "  " << habitaciones[i].descripcion << endl;
    }


    archivo.close();
    return 0;

}