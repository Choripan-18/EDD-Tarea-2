#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <limits>
#include <cctype> // Para toupper
#include <algorithm> // Para transform
using namespace std;



//Clases y Structs
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

        bool estaVivo() const;

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
/*
* int calcularAltura
*Calcula altura del arbol ternario
* input: puntero al nodo actual
* return : latura del subarbol desde el nodo dado
*/
int calcularAltura(NodoArbol* nodo) {
    if (!nodo) return 0;
    int h1 = calcularAltura(nodo->hijo1);
    int h2 = calcularAltura(nodo->hijo2);
    int h3 = calcularAltura(nodo->hijo3);
    return 1 + max({h1, h2, h3});
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

void seleccionarMejora(Mejora* mejoras, int numMejoras, Jugador& jugador){

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, numMejoras - 1);

    int mejora1 = dist(gen);
    int mejora2;
    do {
        mejora2 = dist(gen);
    } while (mejora2 == mejora1); // Asegurarse de que las mejoras sean diferentes

    cout << "\nFelicidades, puedes elegir una mejora!\n";
    cout << "A: +" << mejoras[mejora1].valor << " " << mejoras[mejora1].tipo << endl;
    cout << "B: +" << mejoras[mejora2].valor << " " << mejoras[mejora2].tipo << endl;
    cout << "Selecciona una mejora (A o B): ";
    char eleccion;
    cin >> eleccion;
    eleccion = toupper(eleccion);
    while (eleccion != 'A' && eleccion != 'B') { // Validar entrada
        cout << "Opcion invalida. Elige A o B: ";
        cin >> eleccion;
        eleccion = toupper(eleccion);
    }

    Mejora seleccionada = (eleccion == 'A') ? mejoras[mejora1] : mejoras[mejora2];  //condicion? si : no
    // Aplicar la mejora al jugador
    if (seleccionada.tipo == "Vida") {
        jugador.setVida(jugador.getVida() + seleccionada.valor);
        cout << "¡Tu vida ha aumentado a " << jugador.getVida() << "!\n";
    }
    else if (seleccionada.tipo == "Ataque") {
        jugador.setAtaque(jugador.getAtaque() + seleccionada.valor);
        cout << "¡Tu ataque ha aumentado a " << jugador.getAtaque() << "!\n";
    } 
    else if (seleccionada.tipo == "Precision") {
        jugador.setPrecision(jugador.getPrecision() + seleccionada.valor);
        cout << "¡Tu precision ha aumentado a " << jugador.getPrecision() << "!\n";
    } 
    else if (seleccionada.tipo == "Recuperacion") {
        jugador.setRecuperacion(jugador.getRecuperacion() + seleccionada.valor);
        cout << "¡Tu recuperacion ha aumentado a " << jugador.getRecuperacion() << "!\n";
    }
    else {
        cout << "Mejora desconocida: " << seleccionada.tipo << endl;
    }
    cout << "\n";
};

void elegirCamino(NodoArbol*& nodoActual) {
    cout << "Caminos disponibles:\n";
    int opcionMenu = 1;


    if (nodoActual->hijo1) {
        cout << opcionMenu << ". " << nodoActual->hijo1->name << endl;
        opcionMenu++;
    }

    if (nodoActual->hijo2) {
        cout << opcionMenu << ". " << nodoActual->hijo2->name << endl;
        opcionMenu++;

    }
    if (nodoActual->hijo3) {
        cout << opcionMenu << ". " << nodoActual->hijo3->name << endl;

    }
    int opcion;
    cout << "\nTu eleccion: ";
    cin >> opcion;     //ingresar eleccion

    //validar camino y mover al jugador a la habitacion elegida
    while (cin.fail() || opcion < 1 || opcion > opcionMenu) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Opcion invalida. Elige entre 1 y " << opcionMenu << ": ";
        cin >> opcion;
    }
    
    if (opcion == 1) {
        nodoActual = nodoActual->hijo1;
    }
    else if (opcion == 2) {
        nodoActual = nodoActual->hijo2;
    }
    else if (opcion == 3) {
        nodoActual = nodoActual->hijo3;
    }
    else {
        cout << "Opcion invalida. Intenta de nuevo." << endl;
        elegirCamino(nodoActual);
    }
}

/*
* void generarEnemigos
* Genera los enemigos de manera aleatoria para luego colocarlos en Cola
* input: Arreglo de Enemigo, cantidad de enemigos, y la cola vacia para el combate
* return: Void pero modifica la cola con los enemigos para el combate
*/

void generarEnemigos(Enemigo* enemigos, int numEnemigos, Cola& combate){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0,1);
    int numEnemigosCombate = 1 + (dis(gen) < 0.5 ? 1 : 0) + (dis(gen) < 0.3 ? 1 : 0);
    for (int i = 0; i < numEnemigosCombate; i++){
        float sumaProb = 0;
        float r = dis(gen);
        for (int j = 0; j < numEnemigos; j++) {
            sumaProb += enemigos[j].probabilidad;
            if (r < sumaProb) {
                combate.enqueue(enemigos[j]);
                break;
            }
        }
    }

}
// verificar si esta vivo
bool Jugador :: estaVivo() const {
    return vida > 0;
}
/*
* bool combatir
* toda la logica del combate;
* input: arreglo de tipo Jugador, Cola con los enemigos
* return: retorn True si sigue vive o False si se muere
*/
bool combatir (Jugador& jugador, Cola& combate){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0,1);
    while (!combate.estaVacia() && jugador.estaVivo()){
        Enemigo enemigo = combate.dequeue();

        //ataque jugador
        if (dis(gen) <= jugador.getPrecision()){
        cout << "Golpeas a " << enemigo.nombre << " y le inflinges " << jugador.getAtaque() << " de daño!!" << endl;
        enemigo.vida -= jugador.getAtaque();
        }
        else {
            cout << "Fallaste el ataque!" << endl;
        }
        
        if (enemigo.vida > 0) {
            if (dis(gen) <= enemigo.precision) {
                cout << enemigo.nombre << " te golpea por " << enemigo.ataque << " de daño!" << endl;
                jugador.setVida(jugador.getVida() - enemigo.ataque);
            }
            else {
                cout << enemigo.nombre << " falla el ataque! " << endl;
            }
            combate.enqueue(enemigo); // reenfila si sobrevive

        }
        else {
            cout << " has derrotado a " << enemigo.nombre << "!!" << endl;
        }
        cout << "Tu vida: " << jugador.getVida() << "\n" << endl;
    }
    return jugador.estaVivo();
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

string toUpper(const string& str) {
    string upperStr = str;
    transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper );
    return upperStr;

} 

void aplicarConsecuencia(Jugador& jugador, const string& consecuencia) {
    if (consecuencia == "Ninguna consecuencia. ") return;
    stringstream ss(consecuencia);
    float valor;
    string atributo;
    ss >> valor >> atributo;
    atributo = toUpper(atributo);
    if (atributo == "VIDA") {
        jugador.setVida(jugador.getVida() + static_cast<int>(valor));
    } else if (atributo == "ATAQUE") {
        jugador.setAtaque(jugador.getAtaque() + static_cast<int>(valor));
    } else if (atributo == "PRECISION") {
        jugador.setPrecision(jugador.getPrecision() + valor);
    } else if (atributo == "RECUPERACION") {
        jugador.setRecuperacion(jugador.getRecuperacion() + static_cast<int>(valor));
    } else {
        cout << "Consecuencia no reconocida: " << consecuencia << endl;
    }
}

void iniciarJuego(Jugador& jugador, NodoArbol* raiz, Evento* eventos, int numEventos, Enemigo* enemigos, int numEnemigos, Mejora* mejoras, int numMejoras){
    NodoArbol* nodoActual = raiz;
    bool juegoActivo = true;

    while (juegoActivo && jugador.getVida() > 0) {
        cout << nodoActual -> desc << endl;
        if (nodoActual -> tipo == "INICIO" || nodoActual->tipo == "EVENTO" || nodoActual-> tipo == "COMBATE") {
            if (nodoActual -> tipo == "EVENTO") {
                Evento& eventoSeleccionado = elegirEvento(eventos, numEventos);
                cout << "Llegaste a un evento!!!\n" << eventoSeleccionado.nombre << endl;
                cout << eventoSeleccionado.desc << endl;
                for (int i = 0; i < 2; i++) {
                    cout << eventoSeleccionado.opciones[i].accion << endl;
                }
                cout << "\nSelecciona una opcion: ";
                char letra;
                cin >> letra;
                while (letra != 'A' && letra != 'B') {
                    cout << "\nOpcion invalida. Elige A o B";
                    cin >> letra;
                    letra = toupper(letra);
                }
                int eleccion = letra - 'A';
                cout << eventoSeleccionado.opciones[eleccion].desc << endl;
                aplicarConsecuencia(jugador, eventoSeleccionado.opciones[eleccion].consecuencia);
                cout << "Consecuencia aplicada: " << eventoSeleccionado.opciones[eleccion].consecuencia << endl;
            } else if (nodoActual -> tipo == "COMBATE"){
                Cola combate;
                generarEnemigos(enemigos, numEnemigos, combate);
                if (!combatir(jugador, combate)){
                    cout << "Has muerto..." << endl;
                    juegoActivo = false;
                    break;
                }
                seleccionarMejora(mejoras, numMejoras, jugador);
            }
            jugador.regenerarVida();
            elegirCamino(nodoActual);

        }else if (nodoActual -> tipo == "FIN") {
            cout << "\n--- Fin del juego ---\n" << nodoActual -> desc << endl;
            juegoActivo = false;
        }

    }
    cout << "\nFIN DEL JUEGO" << endl;
}


void limpiarRecursos(Habitacion*& habitaciones, Arco*& arcos, Enemigo*& enemigos, Evento*& eventos, Mejora*& mejoras, NodoArbol*& raiz, string*& linea, int*& numHabitaciones, int*& numArcos, int*& numEnemigos, ifstream& archivo) {
    delete[] habitaciones;
    delete[] arcos;
    delete[] enemigos;
    delete[] eventos;
    delete[] mejoras;
    delete raiz;
    delete linea;
    delete numHabitaciones;
    delete numArcos;
    delete numEnemigos;
    archivo.close();
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
    int *numHabitaciones = new int, *numArcos = new int, *numEnemigos = new int;
    Habitacion* habitaciones = nullptr;
    Arco* arcos = nullptr;
    Enemigo* enemigos = nullptr;
    Evento* eventos = nullptr;
    Mejora* mejoras = nullptr;
    NodoArbol* raiz = nullptr;
    int numEventos = 0, numMejoras = 0;

    while (*linea != "HABITACIONES"){ //ciclo hasta llegar a HABITACIONES
        getline(archivo, *linea); // obtener linea
        if (*linea != "INICIO DE ARCHIVO" && *linea != "HABITACIONES"){ //verificar que cumpla con el formato
            cerr << "El archivo no posee el formato esperado" << endl; //si no cumple mensaje de error
            archivo.close();
            return 1;
        }
    }
    
    ; // Inicializar para guardar cantidad de habitaciones
    getline(archivo, *linea);
    *numHabitaciones = stoi(*linea);
    leerHabitaciones(archivo, habitaciones, *numHabitaciones);

    //leer Arcos
    getline(archivo, *linea);
    if (*linea != "ARCOS"){
        cerr<< "Error al leer seccion ARCOS"<< endl;
        limpiarRecursos(habitaciones, arcos, enemigos, eventos, mejoras, raiz, linea, numHabitaciones, numArcos, numEnemigos, archivo);
        return 1;
    }
    getline(archivo, *linea);
    *numArcos = stoi(*linea);
    leerArcos(archivo, arcos, *numArcos);


    //Arbol ternario
    crearArbol(habitaciones, arcos, raiz, *numHabitaciones, *numArcos);
    int altura = calcularAltura(raiz); //verificar que el arbol cumple con la altura
    if (altura < 5){
        cerr << "Error: El arbol debe tener una altura minima de 5" << endl;
        limpiarRecursos(habitaciones, arcos, enemigos, eventos, mejoras, raiz, linea, numHabitaciones, numArcos, numEnemigos, archivo );
    
    }

    //lectura enemigos
    getline(archivo, *linea);
    if (*linea != "ENEMIGOS"){
        cerr<< "Error al leer seccion ENEMIGOS"<< endl;
        limpiarRecursos(habitaciones, arcos, enemigos, eventos, mejoras, raiz, linea, numHabitaciones, numArcos, numEnemigos, archivo);
        return 1;
    }
    getline(archivo, *linea);
    *numEnemigos = stoi(*linea);
    leerEnemigos(archivo, enemigos, *numEnemigos);

    //Lectura de eventos
    getline(archivo, *linea); // Leer "EVENTOS"
    if(*linea != "EVENTOS"){
        cerr << "Error al leer seccion EVENTOS" << endl;
        limpiarRecursos(habitaciones, arcos, enemigos, eventos, mejoras, raiz, linea, numHabitaciones, numArcos, numEnemigos, archivo);
        return 1;
    }
    getline(archivo, *linea);
    numEventos = stoi(*linea);
    leerEventos(archivo, eventos, numEventos);

    //Lectura de eventos
    getline(archivo, *linea);
    if (*linea != "MEJORAS DE COMBATE") {
        cerr << "Error al leer mejoras de combate" << endl;
        limpiarRecursos(habitaciones, arcos, enemigos, eventos, mejoras, raiz, linea, numHabitaciones, numArcos, numEnemigos, archivo);
        return 1;
    }
    leerMejoras(archivo, mejoras, numMejoras);


//EMPIEZA EL JUEGO **********
    Jugador jugador;
    jugador.setVida(30);
    jugador.setAtaque(5);
    jugador.setPrecision(0.8);
    jugador.setRecuperacion(2);

    iniciarJuego(jugador, raiz, eventos, numEventos, enemigos, *numEnemigos, mejoras, numMejoras);
    limpiarRecursos(habitaciones, arcos, enemigos, eventos, mejoras, raiz, linea, numHabitaciones, numArcos, numEnemigos, archivo);
    return 0;

}