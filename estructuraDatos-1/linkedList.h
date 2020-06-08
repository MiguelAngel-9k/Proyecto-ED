#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

class pacientes;
class especialidades;
template <class T> class linkedList;
template <class T> class Nodo;

class especialidades
{
public:

	especialidades(string especialidad) : _especialidad(especialidad)
	{

	}
	string getEspecialidad() { return _especialidad; }
	//friend ostream &operator<<(ostream &salida, const especialidades &especialidad)
	//{
	//	salida << "especialidad: " << especialidad._especialidad<< endl;
	//	return salida;
	//}
	especialidades *_inicio;
private:
	string _especialidad;
};

class pacientes
{
public:
	pacientes(string nombre, string enfermedad, string telefono): _nombre(nombre), _enfermedad(enfermedad), _telefono(telefono)
	{

	}
	string getNombre() { return _nombre; }
	string getEnfermedad() { return _enfermedad; }
	string getTelefono() { return _telefono; }
	//friend ostream &operator<<(ostream &insertar, const pacientes &paciente)
	//{
	//	insertar << "Nombre: " << paciente._nombre << endl;
	//	insertar << "Enfermedad: " << paciente._enfermedad << endl;
	//	insertar << "Telefono: " << paciente._telefono << endl;
	//	return insertar;
	//}
private:
	string _nombre;
	string _enfermedad;
	string _telefono;
};

template <class T>
class Nodo
{
	friend class linkedList<T>;
public:
	Nodo<T> *getAnterior() { return anterior; }
	Nodo<T> *getSiguiente() { return siguiente; }
	T getDatos();
	Nodo(T);
	Nodo();
	//friend ostream &operator<<(ostream &out, const Nodo<T>&nodo)
	//{
	//	out << nodo.datos;
	//	return out;
	//}
private:
	Nodo<T> *anterior;
	Nodo<T> *siguiente;
	T datos;
};

template <class T>
Nodo<T>::Nodo()
{
	datos = NULL;
	siguiente = NULL;
	anterior = NULL;
}

template <class T>
Nodo<T>::Nodo(T nodo)
{
	data = nodo;
	anterior = NULL;
	siguiente = NULL;
}

template <class T>
 T Nodo<T>::getDatos()
{
	 return datos;
}

 template <class T>
 class linkedList
 {
 public:
	 linkedList();
	 ~linkedList();

	 void insertarInicio(T nodo);
	 void insertarFinal(T Nodo);
	 Nodo<T> *getInicio() { return inicio; }
	 //void insertarEntre(T viejoNodo, T insertarNodo);
	 void removerNodo(T nodo);
	 void mostrarLista();
	 int largo();

 /*	 template <class TT>
	 friend ostream &operator<<(ostream &out, const linkedList<TT> &object)
	 {
		 Nodo<TT> *temp = object.inicio;
		 while (temp != NULL)
		 {
			 out << *temp->getDatos() << endl;
			 temp = temp->getSiguiente();
		 }
		 cout << endl;
		 return out;
	 }*/
	 Nodo<T> *inicio;
	 Nodo<T> *aux;
 };

 template <class T>
 linkedList<T>::linkedList() :inicio(NULL)
 {

 }

 template <class T>
 linkedList<T>::~linkedList()
 {
	 Nodo<T> *nodo = inicio;
	 Nodo<T> *tempNodo;

	 while (nodo != NULL)
	 {
		 tempNodo = nodo;
		 nodo=nodo->siguiente;
		 delete tempNodo;
	 }
 }

 template <class T>
 void linkedList<T>::insertarInicio(T nodo)
 {
	 Nodo<T> *nuevoNodo = new Nodo<T>;
	 nuevoNodo->datos = nodo;

	 if (inicio == NULL)
	 {
		 inicio = nuevoNodo;
		 inicio->anterior = NULL;
		 inicio->siguiente = NULL;
		 return;
	 }

	 nuevoNodo->siguiente = inicio;
	 inicio->anterior = nuevoNodo;
	 inicio = nuevoNodo;
	 inicio->anterior = NULL;
	 return;
 }

 template <class T>
 void linkedList<T>::insertarFinal(T nodo)
 {
	 Nodo<T> *nuevoNodo = new Nodo<T>;
	 nuevoNodo = inicio;

	 if (inicio == NULL)
	 {
		 inicio = nuevoNodo;
		 inicio->anterior = NULL;
		 inicio->siguiente = NULL;
	 }

	 Nodo<T> *tempNodo = new Nodo<T>;
	 tempNodo=inicio;

	 while (tempNodo->siguiente != NULL)
		 tempNodo = tempNodo->siguiente;

	 tempNodo->siguiente = nuevoNodo;
	 nuevoNodo->siguiente = NULL;
	 return 0;
 }

template<class T>
void linkedList<T>::removerNodo(T nodo)
{
	Nodo<T> tempNodo;
	Nodo<T> tempNodo2;

	if (inicio == NULL)
	{
		cout << "la lista esta vacia";
		return;
	}
	else if (inicio->datos == nodo)
	{
		tempNodo = inicio;
		if (tempNodo->siguiente!=NULL&&tempNodo->anterior==NULL)
		{
			inicio = inicio->siguiente;
			inicio->anterior = NULL;
			delete tempNodo;
			tempNodo = NULL;
		}
		else
		{
			delete inicio;
			inicio = NULL;
			tempNodo = NULL;
			return;
		}

		tempNodo2 = inicio;
		while (tempNodo2->siguiente->siguiente!= NULL)
		{
			
			if (tempNodo->siguiente->datos == nodo)
			{
				tempNodo = tempNodo2->siguiente;
				tempNodo->siguiente = tempNodo->siguiente->siguinte;
				tempNodo->siguiente->anterior = tempNodo2;
				delete tempNodo2;
				return;
			}
			tempNodo = tempNodo2->siguiente;
		}
		if (tempNodo2->siguiente->datos == nodo)
		{
			tempNodo = tempNodo2->siguiente;
			delete tempNodo;
			tempNodo = NULL;
			tempNodo2->siguiente = NULL;
			return;
		}

		cout << "Elemento no encontrado";
	}
}

template <class T>
int linkedList<T>::largo()
{
	if (inicio == NULL)
		return 0;

	Nodo<T> *tempNodo=inicio;
	int contador = 0;

	while (tempNodo != NULL)
	{
		contador++;
		tempNodo = tempNodo->siguiente;
	}
	return contador;
}

template <class T>
void linkedList<T>::mostrarLista()
{
	Nodo<T> *tempNodo = inicio;
	if (tempNodo->siguiente == NULL)
	{
		cout << tempNodo->datos << endl;
		return;
	}
	else {

		while (tempNodo != NULL)
		{
			cout << tempNodo->datos << endl;
			tempNodo = tempNodo->siguiente;
		}
	}
	cout << "lista imprimida" << endl;
	return;
}

int main()
{
	string especialidad;
	string nombre;
	string enfermedad;
	string telefono;
	pacientes *regPaciente;
	especialidades *regEspecialidades;
	linkedList<pacientes*> _pacientes;
	linkedList<especialidades*> lista;
	Nodo<especialidades> *mostrar;


	short op = 0;
	do {
		cout << "\t\t-------MENU DE INICIO----------" << endl;
		cout << "1.- registrar especialidad" << endl;
		cout << "2.- registrar paciente" << endl;
		cin >> op;
		switch (op)
		{
		case 1:
			cout << "ingrese la especialidad: ";
			cin >> especialidad;
			regEspecialidades = new especialidades(especialidad);
			lista.insertarInicio(regEspecialidades);

			cout << "quiere ver la lista de especialidades?  ";
			cin >> op;
			if (op == 1)
			{
				lista.aux = lista.inicio;
				while (lista.aux != NULL)
				{
					cout<<lista.aux->getDatos()->getEspecialidad()<<endl;
					lista.aux = lista.aux->getSiguiente();
				}
			}
			break;

		case 2:
			cout << "ingrese nombre del paciente: ";
			cin >> nombre;
			cout << "ingrese la enfermedad: ";
			cin >> enfermedad;
			cout << "ingrese el telefono: ";
			cin >> telefono;
			regPaciente = new pacientes(nombre, enfermedad, telefono);
			_pacientes.insertarInicio(regPaciente);

			cout << "quiere ver la lista de pacientes?  ";
			cin >> op;
			if (op == 1)
			{
				_pacientes.aux = _pacientes.inicio;
				while (_pacientes.aux != NULL)
				{
					cout << _pacientes.aux->getDatos()->getNombre() << endl;
					cout << _pacientes.aux->getDatos()->getEnfermedad() << endl;
					cout << _pacientes.aux->getDatos()->getTelefono() << endl;
					_pacientes.aux = _pacientes.aux->getSiguiente();
				}
			}
			break;
		}
	
	} while (op < 3);

	return 0;
}