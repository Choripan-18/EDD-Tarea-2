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


struct NodoCola{
    Enemigo enemigo;
    NodoCola* sig;
    NodoCola(Enemigo e) : enemigo(e), sig(nullptr){}
    ~NodoCola() {delete sig;}
};


struct OpcionEvento {
    string opcion;
    string descripcion;
    string consecuencia;
};


struct Evento {
    string nombre;
    float probabilidad;
    string descripcion;
    OpcionEvento opciones[2]; 
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


class Jugador{
    private:
        int vida;
        int ataque;
        float precision;
        int recuperacion;
    public:
        // Getters
        int getVida() const { return vida; }
        int getAtaque() const { return ataque; }
        float getPrecision() const { return precision; }
        int getRecuperacion() const { return recuperacion; }

        // Setters
        void setVida(int v) { vida = v; }
        void setAtaque(int a) { ataque = a; }
        void setPrecision(float p) { precision = p; }
        void setRecuperacion(int r) { recuperacion = r; }

        void regenerarVida() {
            vida += recuperacion;
            cout << "\nHas recuperado vida. Vida actual: " << vida << endl;
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


void leerEventos(ifstream& archivo, Evento*& eventos, int& numEventos) {
    string linea;
    while(linea != "EVENTOS") { // Ciclo hasta llegar a la seccion de eventos
        getline(archivo, linea);
    }
    getline(archivo, linea); // Leer la cantidad de eventos
    numEventos = stoi(linea);
    eventos = new Evento[numEventos];

    for (int i = 0; i < numEventos; i++) {
        getline(archivo, linea); // &
        getline(archivo, eventos[i].nombre); // Nombre del evento
        getline(archivo, linea); // Probabilidad
        eventos[i].probabilidad = stof(linea.substr(13)); // Saltar "Probabilidad "
        getline(archivo, eventos[i].descripcion); // Descripcion

        for (int j = 0; j < 2; j++) { // Leer dos opciones
            getline(archivo, eventos[i].opciones[j].opcion);
            getline(archivo, eventos[i].opciones[j].descripcion);
            getline(archivo, eventos[i].opciones[j].consecuencia);
        }

    }
}


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


void elegirCamino(NodoArbol*& nodoActual){
    cout << "Caminos disponibles:\n";
    int opcion = 1;     //mostrar opciones
    if (nodoActual->hijo1) {
        cout << opcion << ". " << nodoActual->hijo1->name << endl;
        opcion++;
    }
    if (nodoActual->hijo2) {
        cout << opcion << ". " << nodoActual->hijo2->name << endl;
        opcion++;
    }
    if (nodoActual->hijo3) {
        cout << opcion << ". " << nodoActual->hijo3->name << endl;
    }
    cout << "\nTu eleccion: ";
    cin >> opcion;      //ingresar eleccion

    //validar camino y mover al jugador a la habitacion elegida
    if (opcion == 1 && nodoActual->hijo1) {
        nodoActual = nodoActual->hijo1;
    }
    else if (opcion == 2 && nodoActual->hijo2) {
        nodoActual = nodoActual->hijo2;
    }
    else if (opcion == 3 && nodoActual->hijo3) {
        nodoActual = nodoActual->hijo3;
    }
    else {
        cout << "Opcion invalida. Intenta de nuevo." << endl;
        elegirCamino(nodoActual);
    }
}


Evento& elegirEvento(Evento* eventos, int numEventos){

    random_device rd; 
    mt19937 gen(rd()); // Generador de numeros aleatorios
    uniform_real_distribution<float> dist(0.0, 1.0); // Distribucion uniforme entre 0 y 1
    float prob = dist(gen); // Generar un flotante aleatorio entre 0 y 1
    float acumulado = 0.0f;
    for (int i = 0; i < numEventos; i++) {
        acumulado += eventos[i].probabilidad;
        if (prob <= acumulado) {
            return eventos[i]; // Retorna el evento seleccionado
        }
    }
    return eventos[0]; //Si ningun evento es seleccionado retorna por defecto el primero

}


Enemigo* generarEnemigos(Enemigo* enemigos, int numEnemigos) {
    random_device rd;
    mt19937 gen(rd()); 
    uniform_real_distribution<float> dist(0.0, 1.0); 
    uniform_int_distribution<int> distNum(1,3);

    int numEnemigosGenerados = distNum(gen); 
    Enemigo* enemigosGenerados = new Enemigo[numEnemigosGenerados];

    for (int i = 0; i < numEnemigosGenerados; i++) {
        float seleccionador = dist(gen);
        float acumulado = 0.0f;

        for (int j = 0; j < numEnemigos; j++) {
            acumulado += enemigos[j].probabilidad;
            if (seleccionador <= acumulado) {
                enemigosGenerados[i] = enemigos[j]; 
                break;
            }
        }
    }
    return enemigosGenerados;
};


int main() {
    cout << "Juego Aventura - EDD Tarea 2" << endl;
    cout << "Creado por: Jorge Gahona y Matias Ibañez\n" << endl;


    //CONSEGUIR DATOS **********
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

    /* Imprimir habitaciones (prueba)
    for (int i = 0; i < *numHabitaciones; i++) {
        cout << "Habitacion " << habitaciones[i].id << ": " << habitaciones[i].name
             << " (" << habitaciones[i].tipo << ")" << endl
             << "Descripcion: " << habitaciones[i].desc << endl;
    }*/
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

    /* Imprimir arcos (prueba)
    for (int i= 0; i < *numArcos; i++){
        cout << arcos[i].origen << "->" << arcos[i].destino<<endl;
    }*/

    NodoArbol* raiz = nullptr;
    crearArbol(habitaciones, arcos, raiz, *numHabitaciones, *numArcos);


    while (*linea != "ENEMIGOS") { // Ciclo hasta llegar a la seccion de enemigos
        getline(archivo, *linea);
    }
    getline(archivo, *linea);
    int* numEnemigos = new int;
    *numEnemigos = stoi(*linea);
    Enemigo* enemigos = nullptr;

    leerEnemigos(archivo, enemigos, *numEnemigos);

    /* Imprimir enemigos (prueba)
    for (int i = 0; i < *numEnemigos; i++) {
        cout << "Enemigo " << i + 1 << ": " << enemigos[i].nombre
             << " | Vida: " << enemigos[i].vida
             << " | Ataque: " << enemigos[i].ataque
             << " | Precision: " << enemigos[i].precision
             << " | Probabilidad: " << enemigos[i].probabilidad << endl;
    }*/


    //Aqui nos quedamos justo en linea EVENTOS

    Evento* eventos = nullptr;
    int numEventos = 0;
    leerEventos(archivo, eventos, numEventos);

    /*Probar eventos
    for (int i = 0; i < numEventos; i++) {
    cout << "Evento: " << eventos[i].nombre << " | Probabilidad: " << eventos[i].probabilidad << endl;
    for (int j = 0; j < 2; j++) {
        cout << "  Opcion " << eventos[i].opciones[j].opcion << ": " << eventos[i].opciones[j].descripcion << endl;
        cout << "    Consecuencia: " << eventos[i].opciones[j].consecuencia << endl;
        }
    }*/

    getline(archivo, *linea);
    Mejora* mejoras = nullptr;
    int numMejoras = 0;
    leerMejoras(archivo, mejoras, numMejoras); //Conseguir mejoras
    /*Probar mejoras
    for (int i =0; i < numMejoras; i++) {   
        cout << "Mejora: +" << mejoras[i].valor << " " << mejoras[i].tipo << endl;
    }*/


    //EMPIEZA EL JUEGO **********
    Jugador jugador;
    jugador.setVida(30);
    jugador.setAtaque(5);
    jugador.setPrecision(0.8);
    jugador.setRecuperacion(2);

    NodoArbol* nodoActual = raiz; // Inicio del juego en la raiz del arbol
    bool juegoActivo = true;

    while (juegoActivo && jugador.getVida() > 0) {

        if (nodoActual->tipo == "INICIO") {
            cout << nodoActual->desc << endl;
            elegirCamino(nodoActual);
        }
        else if (nodoActual->tipo == "FIN") {
            cout << nodoActual->desc << endl;
            juegoActivo = false; // Terminar el juego
        }
        else if (nodoActual->tipo == "EVENTO") {
            cout << nodoActual->desc << endl; 
            Evento& eventoSeleccionado = elegirEvento(eventos, numEventos); // Elegir un evento aleatorio
            cout << "¡Evento encontrado!\n" << eventoSeleccionado.nombre << endl;
            cout << eventoSeleccionado.descripcion << endl;
            for (int i = 0; i < 2; i++) {
                cout << eventoSeleccionado.opciones[i].opcion << endl;
            }
            cout << "\nSelecciona una opcion: ";
            int eleccion;
            char letra;
            cin >> letra; //Entrada del usuario
            while (letra != 'A' && letra != 'B') { // Validar entrada
                cout << "\nOpcion invalida. Elige A o B: ";
                cin >> letra;
            }
            eleccion = letra - 'A'; // Convertir letra a indice (A=0, B=1, utiliza valores ASCII)
            cout << eventoSeleccionado.opciones[eleccion].descripcion << "\n" << eventoSeleccionado.opciones[eleccion].consecuencia << "\n" << endl;
            jugador.regenerarVida(); // Regenerar vida después de un evento
            elegirCamino(nodoActual); // Elegir un camino después del evento            
        }
        else if (nodoActual->tipo == "COMBATE") {
            cout << nodoActual->desc << endl;
            jugador.regenerarVida(); // Regenerar vida después de un combate
            elegirCamino(nodoActual); // Elegir un camino después del combate
        }
    }
    
    cout << "\n\nFIN DEL JUEGO" << endl;


    archivo.close();
    delete linea;
    delete numEnemigos;
    delete[] habitaciones;
    delete numHabitaciones;
    delete[] arcos;
    delete numArcos;
    delete raiz;
    delete[] eventos;
    return 0;
}