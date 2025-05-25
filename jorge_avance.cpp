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

struct NodoArbol{
    unsigned int id;
    string name;
    string desc;
    string tipo;
    NodoArbol* hijo1;
    NodoArbol* hijo2;
    NodoArbol* hijo3;
    NodoArbol(): hijo1(nullptr),hijo2(nullptr), hijo3(nullptr){} //Constructor
    ~NodoArbol() { //Destructor
        delete hijo1;
        delete hijo2;
        delete hijo3;
    }
};
struct Enemigo {
    string nombre;
    int vida, ataque;
    float precision, probabilidad;
};

struct Opcion {
    string accion;
    string desc;
    string consecuencia;
};


struct Evento {
    string nombre;
    string desc;
    float probabilidad;
    Opcion opciones[2];
};


struct NodoCola{
    Enemigo enemigo;
    NodoCola* sig;
    NodoCola(Enemigo e) : enemigo(e), sig(nullptr){}
    ~NodoCola() {delete sig;}
};

struct Mejora {
    string tipo;
    float valor;
};

class Cola {

    private: 
        NodoCola* frente;
        NodoCola* final;
    public:
        Cola() : frente(nullptr), final(nullptr){}
        ~Cola() {delete frente; }
    
        void enqueue(Enemigo e) {
            NodoCola* nuevo = new NodoCola(e);
            if (!frente) {
                frente = final = nuevo;
            }
            else {
                final->sig = nuevo;
                final = nuevo;
            }

        }
    bool estaVacia(){ return frente == nullptr;}

    Enemigo dequeue(){
        if (estaVacia()) throw runtime_error("Cola vacia");
        NodoCola* temp = frente;
        Enemigo e = temp -> enemigo;
        frente = frente->sig;
        if(!frente) final = nullptr;
        temp->sig = nullptr;
        delete temp;
        return e;
    }
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
void leerHabitaciones(ifstream& archivo, Habitacion*& habitaciones, int& numHabitaciones) {

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
* Input: Archivo .map a leer, Arreglo dinamico de arcos, Cantidad de arcos extraida del .map
* Return:
* ES VOID PERO Extrae del .map los Arcos que relacionan las habitaciones del juego.
**/
void leerArcos(ifstream& archivo, Arco*& arcos, int& numArcos){
    arcos = new Arco[numArcos];
    string linea;
    for (int i=0; i < numArcos; i++){
        getline(archivo, linea);
        stringstream ss(linea);
        string flecha;
        ss >> arcos[i].origen >> flecha >> arcos[i].destino;
    }

}

/*
* void crearArbol
* Construye el arbol ternario con las habitaciones y arcos.
* Input:
* Habitacion* habitaciones: arreglo de habitaciones
* Arco* arcos: arreglo de arcos.
* NodoArbol*& raiz: puntero a la raiz del arbol
*Returns:
* es void pero construye el arbol ternario.
*/
void crearArbol(Habitacion* habitaciones, Arco* arcos, NodoArbol*& raiz, int& numHabitaciones, int& numArcos){
    NodoArbol** nodos = new NodoArbol*[numHabitaciones];
    for (int i = 0; i < numHabitaciones; i++){
        nodos[i] = new NodoArbol;
        nodos[i] -> id = habitaciones[i].id;
        nodos[i] -> name = habitaciones[i].name;
        nodos[i] -> desc = habitaciones[i].desc;
        nodos[i] -> tipo = habitaciones[i].tipo;
    }
    for (int i = 0; i < numArcos; i++){
        int origen = arcos[i].origen;
        int destino = arcos[i].destino;
        if (!nodos[origen]->hijo1) nodos[origen]->hijo1 = nodos[destino];
        else if (!nodos[origen]->hijo2) nodos[origen]->hijo2 = nodos[destino];
        else nodos[origen]->hijo3 = nodos[destino];
    }
    raiz = nodos[0];
    delete[] nodos;
    

}
/* USAR EN CASO DE COMPROBAR FUNCIONAMIENTO DEL ARBOL
void imprimirArbol(NodoArbol* nodo, int nivel = 0) {
    if (!nodo) return;
    for (int i = 0; i < nivel; i++) cout << "  ";
    cout << "Nodo ID: " << nodo->id << ", Nombre: " << nodo->name 
         << ", Tipo: " << nodo->tipo << ", Desc: " << nodo->desc << endl;
    imprimirArbol(nodo->hijo1, nivel + 1);
    imprimirArbol(nodo->hijo2, nivel + 1);
    imprimirArbol(nodo->hijo3, nivel + 1);
}
    */

/* void leerEnemigos
 * Lee la seccion de enemigos y los almacena en un arreglo de Enemigos
 * Input: 
 * el archivo .map
 * Un arreglo dinamico de enemigos pasado por referencia
 * La cantidad de enemigos pasado por referencia.
 * Archivo.map
 * Return:
 * Es void pero modifica el arreglo de Enemigo con todos los enemigos.
*/

void leerEnemigos(ifstream& archivo, Enemigo*& enemigos, int& numEnemigos) {
    string linea; 
    enemigos = new Enemigo[numEnemigos];
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
        ss.ignore(16); //Ignorar " | Probabilidad "
        ss >> enemigo.probabilidad; //Guardar probabilidad
        enemigos[i] = enemigo; //Guardar enemigo en el arreglo enemigos

    }
}

/*
* void leerEventos
* Extrae los datos de el apartado EVENTOS y los almacena en un arreglo de Evento.
* Input:
*  El archivo.map, el arreglo dinamico de Evento, la cantidad de eventos
* Return:
* Void, pero modifica el arreglo dinamico de Evento con los eventos que hayan.
*/
void leerEventos(ifstream& archivo, Evento*& eventos, int numEventos) {
    eventos = new Evento[numEventos];
    string linea;
    for (int i = 0; i < numEventos; i++){
        getline(archivo, linea); //&
        if (linea != "&") cerr << "Error al leer eventos" << endl;
        getline(archivo, eventos[i].nombre);
        getline(archivo, linea);
        eventos[i].probabilidad = stof(linea.substr(linea.find(" ") +1 ));
        getline(archivo, eventos[i].desc);
        for (int j = 0; j<2; j++){
            getline(archivo, linea);
            eventos[i].opciones[j].accion = linea;
            getline(archivo, eventos[i].opciones[j].desc);
            getline(archivo, eventos[i].opciones[j].consecuencia);
        }
    }
}
/*
* void leerMejoras
* Extrae de el archivo .map las mejoras.
* Input: el archivo.map, Arreglo de Mejora, cantidad de mejoras por referencia)
* Return: void, modifica el arreglo de Mejora incluyendo las indicadas en el archivo.map.
*/

void leerMejoras(ifstream& archivo, Mejora*& mejoras, int& numMejoras) {
    string linea;
    Mejora* temp = new Mejora[10];
    numMejoras = 0;
    while (getline(archivo, linea) && linea != "FIN DE ARCHIVO"){
        stringstream ss(linea);
        float valor;
        string tipo;
        ss >> valor >> tipo;
        temp[numMejoras].valor = valor;
        temp[numMejoras].tipo = tipo;
        numMejoras++;
    }
    mejoras = new Mejora[numMejoras];
    for (int i = 0; i < numMejoras; i++){
        mejoras[i] = temp[i];
    }
    delete[] temp;
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

    NodoArbol* raiz = nullptr;
    crearArbol(habitaciones, arcos, raiz, *numHabitaciones, *numArcos);

    getline(archivo, *linea);
    if (*linea != "ENEMIGOS"){
        cerr<< "Error al leer seccion ENEMIGOS"<< endl;
        delete[] habitaciones;
        delete[] arcos;
        archivo.close();
        return 1;
    }
    getline(archivo, *linea);
    int* numEnemigos = new int;

    *numEnemigos = stoi(*linea);
    Enemigo* enemigos = nullptr;

    leerEnemigos(archivo, enemigos, *numEnemigos);

       // Imprimir enemigos
    for (int i = 0; i < *numEnemigos; i++) {
        cout << "Enemigo " << i + 1 << ": " << enemigos[i].nombre
             << " | Vida: " << enemigos[i].vida
             << " | Ataque: " << enemigos[i].ataque
             << " | Precision: " << enemigos[i].precision
             << " | Probabilidad: " << enemigos[i].probabilidad << endl;
    }
    //Aqui nos quedamos justo en linea EVENTOS
    getline(archivo, *linea); // Leer "EVENTOS"
    if(*linea != "EVENTOS"){
        cerr << "Error al leer seccion EVENTOS" << endl;
        delete[] habitaciones;
        delete[] enemigos;
        delete[] arcos;
        archivo.close();
        return 1;
    }

    getline(archivo, *linea);
    int numEventos;
    numEventos = stoi(*linea);
    Evento* eventos = nullptr;
    leerEventos(archivo, eventos, numEventos);
    for (int i = 0; i < numEventos; i++) {
    cout << "Evento " << eventos[i].nombre << ": Probabilidad " << eventos[i].probabilidad << endl;
    for (int j = 0; j < 2; j++) {
        cout << "  " << eventos[i].opciones[j].accion << " - " << eventos[i].opciones[j].desc
             << " (" << eventos[i].opciones[j].consecuencia << ")" << endl;
    }
}
    getline(archivo, *linea);
    if (*linea != "MEJORAS DE COMBATE") {
        cerr << "Error al leer mejoras de combate" << endl;
        delete[] habitaciones;
        delete[] enemigos;
        delete[] eventos;
        archivo.close();
        return 1;
    }

    Mejora* mejoras = nullptr;
    int numMejoras = 0;
    leerMejoras(archivo, mejoras, numMejoras);
    for (int i =0; i < numMejoras; i++) {
        cout << "Mejora: +" << mejoras[i].valor << " " << mejoras[i].tipo << endl;
    }
    

    archivo.close();
    delete[] eventos;
    delete[] mejoras;
    delete linea;
    delete numEnemigos;
    delete[] habitaciones;
    delete numHabitaciones;
    delete[] arcos;
    delete numArcos;
    delete raiz;
    return 0;
}