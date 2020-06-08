#pragma once
#include"medics_tree.h"
#include <fstream>
//HANDLERS DE MIS VENTANAS
using namespace std;
HWND principal, iniSes, registro;
HWND regEspescialidades, regPacientes;
HWND buscarEspecialidades,lbEspecialidades, checkTodasEspecialidades,especialidadEcontrada, editModificarDescripcion;
HWND editNombreReg, editUsuarioReg, editContraReg;
HWND editEspecialidad, editDescripcion,editBuscarEspecialidad, editModificarEspecialidad;
HWND editUsuarioIni, editContraIni;
HWND editNombrePaciente,editApellidoPaciente, editApellidoPaciente2, editTelefonoPaciente, editReferenciaPaciente, editFechaDiaPaciente,
editFechaMesPaciente, editFechaAnioPaciente, editEdadPaciente, editGeneroHombrePaciente, editGeneroMujerPaciente,
editMedicoPaciente;
HBITMAP itconButton;
HWND buscarPaciente, lbMostrarPacientes, ckMostrarPacientes, cbBuscarPorTelefono, ckMostrarPorNombre,txtBuscarPorNombre;
HWND modNombreMedicom, modTelefonoMedico, txtSWCedula, modImagen, editarMedicoInfo,lbModDias, lbDiasActuales, btnModFoto, btnModDias;
HWND txtName,setName, txtM_LastName, txtF_LastName;
HWND txtModNombrePaciente,txtModPrimerApellidoPaciente, txtModSegundoApellidoPaciente, txtModTelefonoPacientes, stGenerPaciente,
cbEditDiaNacimiento,cbEditMesNacimiento, cbEditAnioNacimiento,stFechaNacimientoPaciente,txtEditEdad, btnEliminar;
static HBITMAP imagen;
HWND lbNombresMedicos,cbBuscar, txtCedula,cbEspecialidadMedico, cbModDias,cbModespecialidad,cbModCuarto,btnBuscarDias,cbModHorario, btnModName, btnDelete;
HWND registrarMedicos,editCedulaMedico, editApellidoP,editApellidoM,editTelefonoMedico, editNombreMedico, cbHorarioInicio, cbHorarioFin, lbDiasSeleccionados, cbSeleccionarDias;
HWND pictureControl; static HBITMAP bitmapControl; HWND roomControl, buscarMedicos, specialityControl ;
HMENU hMenu, hPopMedicos,hPopPacientes,hPopCitas,hPopEspecialidades, hSubCitas, hSubConsulta;
//CITA HANDLERS
HWND cbSpecialitys, cbDisplayBy, lbMedics,agendarCitas,cbPacienteCita,dtpCita,cbHoraCita;
HWND stNombreMedico, lbHorarioAgenda, txtEspecialidadCita,btnSeleccionarMedico, pcAgendaImagen,stCedCita,
stDate;
HBITMAP iconSelect, hDoctorImagen;
//CONSULATA CITAS HANDLERS
HWND lbCitas, txtBuscarCita, btnBuscar, ckSatus, stDiaCita, stHora, stCuarto,pcImage,
stNombreDoctor, stEspecialidad, stNombrePaciente, stTelefono, cbCancelacion,
consultaMedicoMes, txtConsultaMedicoMes, cbConsultaMes, cbConsultaMedico, stConsultaNombreMedico, stConsultaEspecialidadMedico
,consultaSemanaEspecialidad,cbConsultaSemana, cbConsultaMesSemana, citasRegistros, cbRegistro,cbRegistroEspecialidad;
HBITMAP CitaImage;

//////////////////////////CLASE PARA AGREGAR USUARIOS//////////////////
struct sesion {		 
	char nombre[80];  
	char usuario[10];
	char constrasenia[12];
	char edad[3];
	sesion* sig;
	sesion* ant;
}a;

class NodoH;

class usuarios
{
	char nombre[50];
	char usuario[50];
	char contrasenia[20];

public:

	usuarios *sig;
	usuarios *ant;

	usuarios() {}
	usuarios(char *_nombre, char*_usuario, char*_contrsenia);
	char *getUsuario() { return usuario; }
	char *getContrasenia() { return contrasenia; }

	friend class NodoH;

}*origen, *ind;

usuarios::usuarios(char *_nombre, char*_usuario, char*_contrasenia)
{
	strcpy(nombre, _nombre);
	strcpy(usuario, _usuario);
	strcpy(contrasenia, _contrasenia);
}

class NodoH
{
	usuarios *datos;
	fstream us;

public:
	NodoH();
	~NodoH();
	bool buscar(char*, char*, usuarios*);
};

NodoH::NodoH()
{
	usuarios *tempNodo = nullptr;
	tempNodo = origen;

	us.open("usuarios.txt", ios::in | ios::ate | ios::app);
	if (us.is_open())
	{
		int size = us.tellg();
		while (size > -1) {
			if (origen == NULL)
			{
				us.seekg(0);
				us >> a.usuario;
				us >> a.constrasenia;
				size = us.tellg();
				us.seekg(size + 1);
				us.getline(a.nombre, 100);
				origen = new usuarios(a.nombre, a.usuario, a.constrasenia);
				origen->sig = NULL;
				origen->ant = NULL;
			}
			else {

				tempNodo = origen;

				while (tempNodo->sig != NULL)
					tempNodo = tempNodo->sig;

				us >> a.usuario;
				us >> a.constrasenia;
				size = us.tellg();
				us.seekg(size + 1);
				us.getline(a.nombre, 100);
				tempNodo->sig = new usuarios(a.nombre, a.usuario, a.constrasenia);
				tempNodo->sig->ant = tempNodo;
				tempNodo = tempNodo->sig;
				tempNodo->sig = NULL;
			}
			size = us.tellg();
			us.seekg(size);
			ind = origen;
		}
		us.close();
	}
	else {
		MessageBox(NULL, "No se pudo cargar el archivo", "Advertencia", MB_ICONERROR);
	}
};

NodoH::~NodoH()
{
	ind = origen;
	if (origen != nullptr)
	{
		while (ind != NULL)
		{
			origen = origen->sig;
			ind->ant = NULL;
			delete ind;
			ind = origen;
		}
	}
}

bool NodoH::buscar(char* bNombre, char* bContrasenia, usuarios* nodo ) {
	if (bNombre[0] != NULL)
	{

		if (!strcmp(bNombre, nodo->getUsuario()))
		{
			if (!strcmp(bContrasenia, nodo->getContrasenia()))
			{
				ind = nodo;
				return false;
			}
			else return true;
		}
		else {
			//ind = ind->sig;
			nodo = nodo->sig;
			if (nodo != NULL) return buscar(bNombre, bContrasenia, nodo);
			else return true;
		}
	}return true;
}

void openFile()
{
	OPENFILENAME file;
	ZeroMemory(&file, sizeof(file));

}

/*class iniSesion {

	char nombre[80];
	char usuario[10];
	char contrasenia[10];
public:

	iniSesion *sig;
	iniSesion *ant;


	bool buscar(char* bNombre, char* bContrasenia) {
		if (bNombre[0] != NULL)
		{

			if (!strcmp(bNombre, ind->usuario))
			{
				if (!strcmp(bContrasenia, ind->contrasenia))
				{
					return false;
				}
				else {
					if (ind != NULL) return buscar(bNombre, bContrasenia);
					else return true;
				}
			}
			else {
				ind = ind->sig;
				if (ind != NULL) return buscar(bNombre, bContrasenia);
				else return true;
			}
		}return true;
	}

	void sacar(char *Pnombre, char *Pcontrasenia, char *Pusuario)
	{
		strcpy(Pnombre, nombre);
		strcpy(Pusuario, usuario);
		strcpy(Pcontrasenia, contrasenia);
	}

	void meter(char pNombre[], char pUsuario[], char pContrasenia[])
	{
		strcpy_s(nombre, pNombre);
		strcpy_s(usuario, pUsuario);
		strcpy_s(contrasenia, pContrasenia);
	}

	void colocar(iniSesion*temp)
	{
		if(org==NULL)
			org = temp;
		//else {
		//	while (ind != NULL)
		//		ind = ind->sig;

		//	ind = temp;
		//}
	}

	void inicio(iniSesion*inicio)
	{
		inicio = org;
	}



} ;

//template <class list>
//class linkear
//{
//	list *ind;
//	list *origen;
//	fstream us;
//	//friend class inSes;
//public:
//
//	char aNombre[30];
//	char aUsuario[30];
//	char aContrasenia[30];
//	
//	linkear() {
//	/*	list datos;
//		us.open("usuarios.txt", ios::in | ios::ate | ios::app);
//		if (us.is_open())
//		{
//			int size = us.tellg();
//			while (size > -1) {
//				if (origen == NULL)
//				{
//					us.seekg(0);
//					origen = new list;
//					us >> aUsuario;
//					us >> aContrasenia;
//					size = us.tellg();
//					us.seekg(size + 1);
//					us.getline(aNombre, 100);
//					origen->sig = NULL;
//					origen->ant = NULL;
//				}
//				else {
//
//					while (ind->sig != NULL)
//						ind = ind->sig;
//
//
//					ind->sig = new list;
//					ind->sig->ant = ind;
//					ind = ind->sig;
//
//					us >> ind->usuario;
//					us >> ind->constrasenia;
//					size = us.tellg();
//					us.seekg(size + 1);
//					us.getline(ind->nombre, 100);
//					ind->sig = NULL;
//				}
//				size = us.tellg();
//				us.seekg(size);
//				ind = origen;
//			}
//			us.close();
//		}*/
//		/*else {
//			MessageBox(NULL, "No se pudo cargar el archivo", "Advertencia", MB_ICONERROR);
//		}*/
//
//	}
//
//	~linkear()
//	{
//		ind = origen;
//		us.open("usuarios.txt", ios::out | ios::trunc | ios::ate);
//		list guardar;
//		if (us.fail()) MessageBox(NULL, "No se pudo leer el fichero, revise si existe en: structuraDatos-1>estructuraDatos-1", "ERROR", MB_ICONERROR);
//		else {
//			while (ind != NULL)
//			{
//				guardar.sacar(aNombre, aContrasenia, aUsuario);
//				us << aUsuario << " ";
//				us << aContrasenia << " ";
//				us << aNombre << endl;
//				origen = ind->sig;
//				delete ind;
//				ind = origen;
//			}
//			us.close();
//		}
//	}
//
//	void agregar(char Pnombre[], char Pusuario[], char Pcontrasenia[])
//	{
//		origen->inicio(origen);
//		ind = origen;
//		if (origen == NULL)
//		{
//			origen = new list;
//			/*strcpy_s(nombre, pNombre);
//			strcpy_s(usuario, pUsuario);
//			strcpy_s(contrasenia, pContrasenia);*/
//			origen->meter(Pnombre, Pusuario, Pcontrasenia);
//			origen->sig = NULL;
//			origen->ant = NULL;
//			origen->colocar(origen);
//
//		}
//		else {
//			while (ind->sig != NULL)
//				ind = ind->sig;
//
//			ind->sig = new list;
//			ind->sig->ant = ind;
//			ind = ind->sig;
//			ind->meter(Pnombre, Pusuario, Pcontrasenia);
//			/*strcpy_s(nombre, pNombre);
//			strcpy_s(usuario, pUsuario);
//			strcpy_s(contrasenia, pContrasenia);*/
//			//ind->colocar(ind);
//			ind->sig = NULL;
//
//		}
//	}
//
//	void IncioLista()
//	{
//		ind = origen;
//	}
//
//}/**org, *aux*/;*/

//tareas mañana, hacer clases para todas las listas, menos los doctores, junto con las ventanas y validaciones correspondientes
