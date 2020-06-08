#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

class pacientes;
class especialidades;

template <class T> class linkedList;
template <class T> class Nodo;

class citas
{
public:
	citas(string fecha, string hora) : _fecha(fecha), _hora(hora) {}
	string getFecha() { return _fecha; }
	string getHora() { return _hora; }
private:
	string _fecha;
	string _hora;
};

class especialidades
{
	friend class linkedList<especialidades>;

public:
	especialidades(string especialidad, string descripcion) : _especialidad(especialidad), _descripcion(descripcion)
	{
		identificador=_id++;
	}

	void buscarEspecialidad(especialidades*nodo);
	string getEspecialidad() { return _especialidad; }
private:
	string _especialidad;
	string _descripcion;
	static int _id;
	int identificador;
};

void especialidades::buscarEspecialidad(especialidades*nodo)
{
	{
		while (nodo != NULL)
		{
			
		}
	}
}

int especialidades::_id = 1000;

class pacientes
{
public:
	pacientes(string nombre, Genero genero, string telefono, Referencia referencia, string dia,string mes,
		string anio, string edad) 
	: _nombre(nombre), _genero(genero), _telefono(telefono), _referencia(referencia), 
		_dia(dia),_mes(mes),_anio(anio), _edad(edad)
	{

	}
	string getNombre() { return _nombre; }
	string getTelefono() { return _telefono; }
	Genero getGenero() { return _genero; }
	Referencia getReferencia() { return _referencia; }
	string getFechaDia() { return _dia; }
	string getFechaMes() { return _mes; }
	string getFehcaAnio() { return _anio; }
	string getEdad() { return _edad; }
	//friend ostream &operator<<(ostream &insertar, const pacientes &paciente)
	//{
	//	insertar << "Nombre: " << paciente._nombre << endl;
	//	insertar << "Enfermedad: " << paciente._enfermedad << endl;
	//	insertar << "Telefono: " << paciente._telefono << endl;
	//	return insertar;
	//}
private:
	string _nombre;
	Genero _genero;
	string _telefono;
	Referencia _referencia;
	string _dia;
	string _mes;
	string _anio;
	string _edad;
	//primerMedico que atendio queda pendeiente
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

	friend class especialidades;

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
		nodo = nodo->siguiente;
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
	nuevoNodo->datos = nodo;

	if (inicio == NULL)
	{
		inicio = nuevoNodo;
		return;
	}

	Nodo<T> *tempNodo = new Nodo<T>;
	tempNodo = inicio;

	while (tempNodo->siguiente != NULL)
		tempNodo = tempNodo->siguiente;

	tempNodo->siguiente = nuevoNodo;
	nuevoNodo->anterior = tempNodo;
	return;
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
		if (tempNodo->siguiente != NULL && tempNodo->anterior == NULL)
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
		while (tempNodo2->siguiente->siguiente != NULL)
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

	Nodo<T> *tempNodo = inicio;
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
