
//1.- GUARDAR MEDICOS BINARIOS
//2.- CARGAR MEDICOS BINARIOS
//3.- BUSCAR MEDICOS
//4.- MODIFICAR Y ELIMINAR MEDICOS
//5.- AGREGARLE ESPECIALIDAD

//#include "medics_tree.h"
#include "resource.h"
#include "Handlers.h"
//#include "listaLigada.h"
//#include "Specialitys_class.h"
//#include "heapSort.h"
#include "Patient_class.h"
#include "date.h"
//#include <string.h>
//#include <fstream>
#include <conio.h>
//#include <iostream>
#include <ctime>
#include <sstream>

enum toolBar {_especialidades=136, file_menu=135,btn_registro=2,ok=1};

#define OK								ok
#define REGISTRO						btn_registro
#define FILE_MENU						file_menu
#define REGISTRAR_ESPECIALIDADES		_especialidades
#define BUSCAR_ESPECIALIDADES			138
#define PACIENTES						137
#define ALTA_MEDICOS					139
#define BUSCAR_MEDICOS					140
#define BUSCAR_PACIENTE					141
#define REPORTES_MEDICOS				142
#define REPORTES_PACEINTES				143
#define BUSCAR_CITA						149
#define CHECK_FINISHED					150
#define CB_CANCEL						151
#define CONSULTA_MEDICO_MES				152
#define CONSULTA_SEMANA_ESPECIALIDAD	153
#define CITAS_REGISTROS					154

using namespace std;
LRESULT CALLBACK func(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK loger(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK reg(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK cambios(HWND, UINT, WPARAM, LPARAM);


NodoH *_usuarios = new NodoH;
bool cheked = false;
short lbCount = 0;
//especialidades *iniciador;
//linkedList<especialidades*> listaEspecialidades;
//pacientes *registrarPacientes;
//linkedList<pacientes*> listaPacientes;
char nombre[] = "windowName";
char *tempPath;
SPECIALITY::Specialitys *nodo;
SPECIALITY::Node *listaEspecialidades=new SPECIALITY::Node;
medics*medicos;
Patient* paciente;
nodePatient *listaPacientes = new nodePatient;
tree*arbolMedicos = new tree(&listaEspecialidades);
Dates::Node *listaCitas = new Dates::Node(listaPacientes,arbolMedicos);
daysOfWeek selected[5];
//linkear <iniSesion> *inicio=new linkear<iniSesion>;
//iniSesion Usuarios;
HINSTANCE hInst;
daysOfWeek* days(HWND *, HWND *);
void cleanLB(HWND*,HWND*);
void refilCB(HWND*);
void displayDays(HWND*, medics*);
void changeBirthDay(HWND* ,HWND* ,int,int);
void controls(HWND hWnd);
void loadIcon();
void addMenu(HWND);
void cleanCB(HWND*);
void addControls(HWND);
HWND createStatic(HWND,const char*, int , int , int , int , int);

enum _fecha { Dia, Mes, Anio } setFecha;
Reference setReferencia;
Gender setGenero;

class Fecha
{
private:
	void fecha(int cantidad,HWND *control);
public:
	char buffer[5];
	char dia[3];
	char mes[3];
	char anio[3];
	char hora[4];
	HWND cargarFecha(HWND*, HWND*, int, _fecha);
	void cargarReferencias(HWND*, HWND*, const char*, int, int);
	Reference seleccionCursor(HWND*, int);
	void setSchedule(HWND*, HWND*,int);
	void setRoom(HWND*, HWND*, int,int);
	//void cargarMes(HWND, HWND, int);
}setCb;

void Fecha::fecha(int cantidad,HWND*control)
{
	char *temp = new char[5];
	for (int i = 32; i >= 0; i--)
	{
		if (i <= 9)
		{
			_itoa(i, temp, 10);
			std::strcpy(buffer, "0");
			strcat(buffer, temp);
			SendMessage(*control, CB_ADDSTRING, 0, (LPARAM)buffer);
			continue;
		}
		_itoa(i, buffer, 10);
		SendMessage(*control, CB_ADDSTRING, 0, (LPARAM)buffer);
		//ZeroMemory(&buffer, sizeof(buffer));
	}
	delete[] temp;
}

HWND Fecha::cargarFecha(HWND *control, HWND *hWnd, int id, _fecha opcion)
{
	*control = GetDlgItem(*hWnd, id);
	switch (opcion)
	{
	case Dia:
		fecha(32, control);
		return *control;
	case Mes:
		fecha(13, control);
		return *control;
	case Anio:
		time_t t = time(NULL);
		tm* timePtr = localtime(&t);
		stringstream ss_year;
		ss_year << timePtr->tm_year + 1900;
		string Year = ss_year.str();
		int anio = stoi(Year);
		for (int i = 1992; i <= anio; i++)
		{
			_itoa(i, buffer, 10);
			SendMessage(*control, CB_ADDSTRING, 0, (LPARAM)buffer);
		}
		return *control;
	}
}

void Fecha::cargarReferencias(HWND*control, HWND*hWnd, const char *texto, int id, int idControl)
{
	*control = GetDlgItem(*hWnd, idControl);
	SendMessage(*control, id, NULL, (LPARAM)texto);
}

Reference Fecha::seleccionCursor(HWND *control, int accion)
{
	int cursel = SendMessage(*control, accion, NULL, NULL);
	switch (cursel)
	{
	case _internet: return _internet;
	case _follet: return _follet;
	case _ad: return _ad;
	case _recomendation: return _recomendation;
	}
}

void Fecha::setSchedule(HWND* control, HWND* hWnd, int id)
{
	*control = GetDlgItem(*hWnd, id);
	short int hour = 9, minutes = 0;
	char tempTimeString[8]="";
	char tempMinuteString[4]="";
	char format[8] = "00:00";
	bool adjusted = false;

	while (hour < 22)
	{
		while (minutes < 60)
		{
			_itoa(hour, tempTimeString, 10);
			_itoa(minutes, tempMinuteString, 10);
			if (hour == 9 )
			{
				char *adjust=new char[8];
				std::strcpy(adjust, &tempTimeString[0]);
				std::strcpy(tempTimeString,"0");
				strcat(tempTimeString, adjust);
				delete adjust;

			}
			if (minutes == 0)
			{
				strcat(tempMinuteString, "0");
				strcat(tempTimeString, ":");
				strcat(tempTimeString, tempMinuteString);
				SendMessage(*control, CB_INSERTSTRING, 0, (LPARAM)tempTimeString);
			}
			else
			{
				strcat(tempTimeString, ":");
				strcat(tempTimeString, tempMinuteString);
				SendMessage(*control, CB_INSERTSTRING, 0, (LPARAM)tempTimeString);
			}
			minutes+=30;
		}
		hour++;
		minutes = 0;
	}
}

void Fecha::setRoom(HWND* control, HWND* hWnd, int controlID, int quantityOfRooms)
{
	*control = GetDlgItem(*hWnd, controlID);
	char room[3] = "";

	for (short int i = 1; i < quantityOfRooms + 1; i++)
		SendMessage(*control,CB_INSERTSTRING,0,(LPARAM)_itoa(i, room, 10));
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
{

	hInst = hInstance;
	WNDCLASSEX wClase;
	ZeroMemory(&wClase, sizeof(WNDCLASSEX));
	wClase.cbClsExtra = NULL;
	wClase.cbWndExtra = NULL;
	wClase.cbSize = sizeof(WNDCLASSEX);
	wClase.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wClase.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClase.hIcon = NULL;
	wClase.hIconSm = NULL;
	wClase.hInstance = hInstance;
	wClase.lpfnWndProc = (WNDPROC)func;
	wClase.lpszClassName = "Window Class";
	wClase.lpszMenuName = NULL;
	wClase.style = CS_HREDRAW | CS_VREDRAW;

	hInst = hInstance;

	if (!RegisterClassEx(&wClase))
	{
		int nResult = GetLastError();
		MessageBox(NULL, "CREACION DE LA VENTANA FALLO", "LA VENTANA NO FUNCIONO", MB_OK | MB_ICONERROR);

	}

	principal = CreateWindowEx(NULL, "Window Class", nombre, WS_OVERLAPPEDWINDOW, 100, 50, 750, 600, NULL, NULL, hInstance, NULL);

	if (!principal)
	{
		int nResult = GetLastError();
		MessageBox(NULL, "FALLO", "ALL MOSTRAR LA VENTANA FALLO", MB_OK | MB_ICONERROR);
	}

	ShowWindow(principal, cmdShow);

	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	//listaEspecialidades->loadFile();

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	if(_usuarios!=NULL)
	delete _usuarios;
	delete listaEspecialidades;
	delete listaCitas;


	return 0;
}

LRESULT CALLBACK func(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	char *buffer = NULL;
	char *guide = NULL;
	Dates::Date *tempDate = NULL;
	int* aux = NULL;

	switch (msg)
	{
	case WM_CREATE:

		addMenu(hWnd);
		addControls(hWnd);

		SendMessage(cbCancelacion, CB_ADDSTRING, NULL, (LPARAM)"No asistío");
		SendMessage(cbCancelacion, CB_ADDSTRING, NULL, (LPARAM)"Dr. no disponible");
		SendMessage(cbCancelacion, CB_ADDSTRING, NULL, (LPARAM)"Causas Mayores");
		SendMessage(cbCancelacion, CB_ADDSTRING, NULL, (LPARAM)"Cita finalizada");



		cleanLB(&lbCitas,NULL);
		listaCitas->displayDates(&lbCitas, listaCitas);

		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, loger);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case REGISTRO:
			break;
		case OK:

			break;
		case FILE_MENU:
			MessageBeep(MB_OK);
			break;
		case REGISTRAR_ESPECIALIDADES:
			ShowWindow(principal, FALSE);
			regEspescialidades = CreateDialog(hInst, MAKEINTRESOURCE(REG_ESPECIALIDADES), principal, loger);
			ShowWindow(regEspescialidades,TRUE);
			
			MessageBeep(OK);
			break;
		case PACIENTES:
			ShowWindow(principal, FALSE);
			regPacientes = CreateDialog(hInst, MAKEINTRESOURCE(REG_PACIENTES), principal, loger);
			ShowWindow(regPacientes, TRUE);
			break;
		case REPORTES_PACEINTES:
			if (listaEspecialidades->getHead()&&listaPacientes->getHead())
			{
				Patient* arr = listaPacientes->listToArray();
				//MAX_SORT::heapSort(arr, Patient::quantity);
				//MAX_SORT::print(arr);
				delete[] arr;
			}
			else MessageBox(NULL, "No hay pacientes registrados aun", "No hay pacientes", MB_ICONERROR);
			break;
		case BUSCAR_ESPECIALIDADES:
			ShowWindow(principal, FALSE);
			buscarEspecialidades = CreateDialog(hInst, MAKEINTRESOURCE(BUSCAR_ESPECIALIDAD), principal, loger);
			ShowWindow(buscarEspecialidades, TRUE);
			MessageBeep(OK);
			break;
		case ALTA_MEDICOS:
			ShowWindow(principal, SW_HIDE);
			registrarMedicos=CreateDialog(hInst, MAKEINTRESOURCE(REGISTRAR_MEDICOS), principal, loger);
			ShowWindow(registrarMedicos,SW_SHOW);
			break;
		case BUSCAR_MEDICOS:
			ShowWindow(principal, SW_HIDE);
			buscarMedicos = CreateDialog(hInst, MAKEINTRESOURCE(MODIFICAR_MEDICOS), principal, loger);
			ShowWindow(buscarMedicos, SW_SHOW);
			break;
		case REGISTRAR_CITAS:
			ShowWindow(principal, SW_HIDE);
			agendarCitas = CreateDialog(hInst, MAKEINTRESOURCE(AGENDAR_CITA), principal, loger);
			ShowWindow(agendarCitas, SW_SHOW);
			break;
		case BUSCAR_CITA:
			buffer = new char[3];
			GetWindowText(txtBuscarCita, buffer, 4);
			cleanLB(&lbCitas, NULL);
			tempDate=listaCitas->searchDate(atoi(buffer));
			SendMessage(lbCitas, LB_ADDSTRING, (WPARAM)0, (LPARAM)"No.Cita\tFecha\t\tHora");
			_itoa(tempDate->getCode(), buffer, 10);
			strcat(buffer, "\t");
			strcat(buffer, tempDate->getDate());
			strcat(buffer, "\t");
			strcat(buffer, tempDate->getHour());
			SendMessage(lbCitas, LB_ADDSTRING, NULL, (LPARAM)buffer);
			break;
		case CONSULTA_MEDICO_MES:
			EnableWindow(principal, FALSE);
			consultaMedicoMes = CreateDialog(hInst, MAKEINTRESOURCE(CONSULTA_MEDICO_MES_VENTANA), principal, loger);
			ShowWindow(consultaMedicoMes, SW_SHOW);
			break;
		case CONSULTA_SEMANA_ESPECIALIDAD:
			EnableWindow(principal, FALSE);
			consultaSemanaEspecialidad = CreateDialog(hInst, MAKEINTRESOURCE(130), principal, loger);
			ShowWindow(consultaSemanaEspecialidad, SW_SHOW);
			break;
		case CITAS_REGISTROS:
			EnableWindow(principal, FALSE);
			citasRegistros = CreateDialog(hInst, MAKEINTRESOURCE(CONSULTA_MEDICO_MES_VENTANA), principal, loger);
			ShowWindow(citasRegistros, SW_SHOW);
			break;
		case BUSCAR_PACIENTE:
			ShowWindow(principal, SW_HIDE);
			buscarPaciente = CreateDialog(hInst, MAKEINTRESOURCE(BUSCAR_PACIENTES), principal, loger);
			ShowWindow(buscarPaciente, SW_SHOW);
			break;
		case REPORTES_MEDICOS:
			medics* arr = arbolMedicos->treeArray();
			if (arr)
			{
				quickSort(arr, 0, medics::quantity);
				sortMedicReport(arr);
				delete[] arr;
			}
			else MessageBox(NULL, "No hay medicos2 registrados", "No hay medicos", MB_ICONWARNING);
			break;

		}

		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			if (LOWORD(wParam) == CHECK_FINISHED)
			{
				buffer = new char[60];
				aux = new int[2], *(aux) = 0, *(aux + 1) = 0;
				if (!SendMessage(ckSatus, BM_GETCHECK, NULL, NULL))
				{
					SendMessage(ckSatus, BM_SETCHECK, BST_CHECKED, NULL);
					*(aux + 1) = SendMessage(lbCitas, LB_GETCURSEL, NULL, NULL);
					SendMessage(lbCitas, LB_GETTEXT, (WPARAM)*(aux + 1), (LPARAM)buffer);
					guide = new char[4];
					while (buffer[*aux] >= 48 && buffer[*aux] <= 57)
					{
						guide[*aux] = buffer[*aux];
						(*aux)++;
					}

					tempDate = listaCitas->searchDate(atoi(guide));
					delete[] guide;
					if (tempDate && tempDate->getStatus() == true) {
						tempDate->setStatus(false);

						if (MessageBox(hWnd, "La cita se a dado por terminada\n¿Desea eliminarla?", "Aviso", MB_ICONQUESTION | MB_YESNO) == IDYES) {
							listaCitas->removeNode(tempDate);
							cleanLB(&lbCitas, NULL);
							listaCitas->displayDates(&lbCitas, listaCitas);
							SendMessage(ckSatus, BM_SETCHECK, BST_UNCHECKED, NULL);
						}
						else {
							cleanLB(&lbCitas, NULL);
							listaCitas->displayDates(&lbCitas, listaCitas);
						}
					}
					else {

						if (!tempDate->getStatus()) if (MessageBox(NULL, "La cita ya esta finalizada\n¿Desea eliminarla?", "Aviso", MB_ICONQUESTION | MB_YESNO) == IDYES) {
							listaCitas->removeNode(tempDate);
							cleanLB(&lbCitas, NULL);
							listaCitas->displayDates(&lbCitas, listaCitas);
							SendMessage(ckSatus, BM_SETCHECK, BST_UNCHECKED, NULL);
						}
						else {
							MessageBox(NULL, "Debe elegir una cita de la lista", "Aviso", MB_ICONSTOP | MB_OK);
							SendMessage(ckSatus, BM_SETCHECK, BST_UNCHECKED, NULL);
						}
					}

				}
				else {
					SendMessage(ckSatus, BM_SETCHECK, BST_UNCHECKED, NULL);

				}

				delete[] buffer, delete[] aux;
			}

			SendMessage(ckSatus, BM_SETCHECK, BST_UNCHECKED, NULL);
			break;

		case LBN_DBLCLK:
				
				buffer = new char[50];
				guide = new char[4];
				aux = new int[2], *(aux + 1) = SendMessage(lbCitas, LB_GETCURSEL, NULL, NULL), *aux = 0;
				SendMessage(lbCitas, LB_GETTEXT, (WPARAM)*(aux + 1), (LPARAM)buffer);
				while (buffer[*aux] >= 48 && buffer[*aux] <= 57)
				{
					guide[*aux] = buffer[*aux];
					(*aux)++;
				}
				tempDate = listaCitas->searchDate(atoi(guide));
				if (tempDate) {
					SetWindowText(stDiaCita, tempDate->getDate());
					SetWindowText(stHora, tempDate->getHour());
					SetWindowText(stCuarto, tempDate->getRoom());
					arbolMedicos->displayFoto(pcImage, CitaImage, tempDate->getMedic(), 150, 150);
					SetWindowText(stNombreDoctor, tempDate->getDoctorName());
					SetWindowText(stEspecialidad, tempDate->getSpeciality());
					SetWindowText(stNombrePaciente, tempDate->getPatientName());
					SetWindowText(stTelefono, tempDate->getPatientPhone());
					delete[] buffer;
				}
				
				delete[] guide;				
				delete[] aux;				
			
			break;
		case CBN_SELENDOK:
			if (LOWORD(wParam) == CB_CANCEL)
			{				
				buffer = new char[MAX_PATH];
				char *guide = new char[4];
				int cursel = SendMessage(lbCitas, LB_GETCURSEL, NULL, NULL);
				SendMessage(lbCitas, LB_GETTEXT, (WPARAM)cursel, (LPARAM)buffer);
				cursel = 0;
				while (buffer[cursel] >= 48 && buffer[cursel] <= 57)
				{
					guide[cursel] = buffer[cursel];
					(cursel)++;
				}

				tempDate = listaCitas->searchDate(atoi(guide));
				delete[] guide;
				if (tempDate)
				{
					if (tempDate->getStatus())
					{
						if (MessageBox(NULL, "¿Desea cancelar la cita?", "Cancelar cita", MB_ICONQUESTION | MB_YESNO) == IDYES)
						{
							GetWindowText(cbCancelacion, buffer, 15);
							tempDate->setCancel(buffer);
							tempDate->setStatus(false);
							cleanLB(&lbCitas, NULL);
							listaCitas->displayDates(&lbCitas, listaCitas);
							MessageBox(NULL, tempDate->getCancel(), "La cita a sido cancelada", MB_OK | MB_ICONINFORMATION);
						}
						else {
							tempDate->setStatus(false);
							cleanLB(&lbCitas, NULL);
							listaCitas->displayDates(&lbCitas, listaCitas);
							MessageBox(NULL, tempDate->getCancel(), "La cita a sido cancelada", MB_OK | MB_ICONINFORMATION);
						}
					}
					else if (MessageBox(NULL, "Esta cita esta inactiva\n¿Desea eliminarla?", "Cita inactiva", MB_ICONINFORMATION | MB_YESNO) == IDYES)
					{
						cleanLB(&lbCitas, NULL);
						listaCitas->displayDates(&lbCitas, listaCitas);
					}
				}
			}
			break;

		}
		break;

	case WM_SETTEXT:


		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		if (hWnd != iniSes) DestroyWindow(hWnd);
		if (hWnd != regEspescialidades) DestroyWindow(hWnd);

		cleanLB(&lbCitas, NULL);
		listaCitas->displayDates(&lbCitas, listaCitas);

		delete arbolMedicos;
		delete listaPacientes;
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
};


BOOL CALLBACK loger(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	medics* temp = NULL;
	SPECIALITY::Specialitys* tempNode = NULL;
	char *buffer = NULL;
	char *date[7];
	
	switch (msg)
	{
	case WM_INITDIALOG:	
		controls(hWnd);
		cleanLB(&lbCitas,NULL);
		listaCitas->displayDates(&lbCitas, listaCitas);
		SendMessage(cbRegistro, CB_ADDSTRING, NULL, (LPARAM)"Por medico y mes");
		SendMessage(cbRegistro, CB_ADDSTRING, NULL, (LPARAM)"Por semana y mes");
		SendMessage(cbRegistro, CB_ADDSTRING, NULL, (LPARAM)"Por semana, mes y especialidad");
		Dates::setMonths(cbConsultaMes,NULL); Dates::setMonths(cbConsultaMesSemana,cbConsultaSemana); 		
		Dates::inicializateDisplayBy(&cbDisplayBy);
		Dates::inicializateDisplayEspecialtys(&cbSpecialitys, listaEspecialidades);
		arbolMedicos->showMedics(&cbConsultaMedico, 0, CB_INSERTSTRING, NULL);
		loadIcon();
		txtEditEdad = GetDlgItem(hWnd, TXT_EDAD_PACIENTE);
		stFechaNacimientoPaciente = GetDlgItem(hWnd, ST_FECHA_NACIMIENTO_PACIENTE);
		setCb.cargarFecha(&cbEditDiaNacimiento, &hWnd, CB_EDIT_DIA_PACIENTE,Dia);
		setCb.cargarFecha(&cbEditMesNacimiento, &hWnd, CB_EDIT_MES_PACIENTE, Mes);
		setCb.cargarFecha(&cbEditAnioNacimiento, &hWnd, CB_EDIT_ANIO_PASIENTE, Anio);
		txtModNombrePaciente = GetDlgItem(hWnd,TXT_EDIT_PACIENTE_NOMBRE);
		txtModPrimerApellidoPaciente = GetDlgItem(hWnd, TXT_EDIT_PACIENTE_APELLIDO1);
		txtModSegundoApellidoPaciente = GetDlgItem(hWnd, TXT_EDIT_PACIENTE_APELLIDO2);
		txtModTelefonoPacientes = GetDlgItem(hWnd, TXT_EDIT_TELEFONO);
		stGenerPaciente = GetDlgItem(hWnd, ST_GENERO_PACIENTE);
		cbBuscarPorTelefono = GetDlgItem(hWnd, TXT_EDIT_TELEFONO_PACIENTES);
		ckMostrarPacientes = GetDlgItem(hWnd, CK_ENABLE_DOCTORES);
		ckMostrarPorNombre = GetDlgItem(hWnd, CK_NOMBRE_PACIENTE);
		txtBuscarPorNombre = GetDlgItem(hWnd, TXT_NOMBRE);
		lbMostrarPacientes = GetDlgItem(hWnd, LB_PACIENTES);
		editUsuarioIni = GetDlgItem(hWnd, EDIT_USUARIO);
		editContraIni = GetDlgItem(hWnd, EDIT_CONTRASENIA);
		editEspecialidad = GetDlgItem(hWnd, TXT_ESPECIALIDAD);
		editDescripcion = GetDlgItem(hWnd, TXT_DESCRIPCION);
		editBuscarEspecialidad = GetDlgItem(hWnd, TXT_BUSCAR_ESPECIALIDAD);
		editGeneroMujerPaciente = GetDlgItem(hWnd, RD_GENERO_MUJER);
		editNombrePaciente = GetDlgItem(hWnd, TXT_PACIENTE_NOMBRE);
		editApellidoPaciente = GetDlgItem(hWnd, TXT_PACIENTE_APELLIDO);
		editApellidoPaciente2 = GetDlgItem(hWnd, TXT_PACIENTE_APELLIDO2);
		editTelefonoPaciente = GetDlgItem(hWnd, TXT_PACIENTE_TELEFONO);
		editMedicoPaciente = GetDlgItem(hWnd, CB_PACIENTE_MEDICO);
		lbEspecialidades = GetDlgItem(hWnd, LB_BUSCAR_ESPECIALIDADES);
		editModificarEspecialidad = GetDlgItem(hWnd, TXT_ESPECIALIDAD_ENCONTRADA);
		setCb.cargarReferencias(&editReferenciaPaciente, &hWnd, "Internet", CB_ADDSTRING, CB_PACIENTES_REFERENCIA);
		setCb.cargarReferencias(&editReferenciaPaciente, &hWnd, "Folleto", CB_ADDSTRING, CB_PACIENTES_REFERENCIA);
		setCb.cargarReferencias(&editReferenciaPaciente, &hWnd, "Anuncio", CB_ADDSTRING, CB_PACIENTES_REFERENCIA);
		setCb.cargarReferencias(&editReferenciaPaciente, &hWnd, "Recomendacion", CB_ADDSTRING, CB_PACIENTES_REFERENCIA);
		setCb.cargarFecha(&editFechaDiaPaciente, &hWnd, CB_FECHA_DIA, Dia);
		setCb.cargarFecha(&editFechaMesPaciente, &hWnd, CB_FECHA_MES, Mes);
		setCb.cargarFecha(&editFechaAnioPaciente, &hWnd, CB_FECHA_ANIO, Anio);
		editEdadPaciente = GetDlgItem(hWnd, CB_PACIENTE_EDAD);
		editCedulaMedico = GetDlgItem(hWnd, TXT_CEDULA_MEDICO);
		editNombreMedico = GetDlgItem(hWnd, TXT_NOMBRE_MEDICO);
		editApellidoP = GetDlgItem(hWnd, TXT_APELLIDOM_MEDICO);
		editApellidoM = GetDlgItem(hWnd, TXT_APELLIDOP_MEDICO);
		cbHorarioInicio = GetDlgItem(hWnd, CB_HORARIO_INICIO);
		//setCb.setSchedule(&cbHorarioInicio, &hWnd, CB_HORARIO_INICIO);
		//cbHorarioFin = GetDlgItem(hWnd, CB_HORARIO_FIN);
		//setCb.setSchedule(&cbHorarioFin, &hWnd, CB_HORARIO_FIN);
		editTelefonoMedico = GetDlgItem(hWnd, TXT_NUMERO_MEDICO);
		cbSeleccionarDias = GetDlgItem(hWnd, CB_SELECCION_DIAS);
		lbDiasSeleccionados = GetDlgItem(hWnd, LB_DIAS_SELECCIONADOS);
		pictureControl = GetDlgItem(hWnd, PC_IMAGE);
		setCb.setRoom(&roomControl, &hWnd, CB_ROOM_MEDIC, 5);
		cbBuscar = GetDlgItem(hWnd, CB_SELECT_VIEW);
		lbNombresMedicos = GetDlgItem(hWnd, LB_NOMBRES_MEDICOS);
		cbEspecialidadMedico = GetDlgItem(hWnd, CB_ESPECIALIDAD);
		txtCedula = GetDlgItem(hWnd, TXT_CEDULA);

		SendMessage(cbSeleccionarDias, CB_INSERTSTRING, 0, (LPARAM)"Lunes");
		SendMessage(cbSeleccionarDias, CB_INSERTSTRING, 0, (LPARAM)"Martes");
		SendMessage(cbSeleccionarDias, CB_INSERTSTRING, 0, (LPARAM)"Miercoles");
		SendMessage(cbSeleccionarDias, CB_INSERTSTRING, 0, (LPARAM)"Jueves");
		SendMessage(cbSeleccionarDias, CB_INSERTSTRING, 0, (LPARAM)"Viernes");

		SendMessage(cbHorarioInicio, CB_INSERTSTRING, NULL, (LPARAM)"Tarde");
		SendMessage(cbHorarioInicio, CB_INSERTSTRING, NULL, (LPARAM)"Mañana");

		SendMessage(cbBuscar, CB_ADDSTRING, NULL, (LPARAM)"Especialidad");
		SendMessage(cbBuscar, CB_ADDSTRING, NULL, (LPARAM)"Cedula");
		SendMessage(cbBuscar, CB_ADDSTRING, NULL, (LPARAM)"Nombre");

		specialityControl = GetDlgItem(hWnd, CB_ESPECIALIDAD_MEDICO);
		tempNode = listaEspecialidades->getHead();
		while (tempNode != NULL)
		{
			SendMessage(specialityControl, CB_INSERTSTRING, NULL, (LPARAM)tempNode->getSpeciality());
			SendMessage(cbEspecialidadMedico, CB_INSERTSTRING, NULL, (LPARAM)tempNode->getSpeciality());
			SendMessage(cbRegistroEspecialidad, CB_INSERTSTRING, NULL, (LPARAM)tempNode->getSpeciality());
			tempNode = tempNode->getNext();
		}

		EnableWindow(cbEspecialidadMedico, FALSE);

		if (arbolMedicos->getRoot())
		{
			arbolMedicos->showMedics(&editMedicoPaciente, 2, CB_INSERTSTRING,NULL);
		}

		if (listaPacientes->getHead())
		{
			listaPacientes->displayList(&lbMostrarPacientes,LB_INSERTSTRING);
			listaPacientes->displayList(&cbPacienteCita, CB_INSERTSTRING);
		}


		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case BTN_REGISTRARSE:
			registro = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG2), iniSes, reg);
			EnableWindow(iniSes, TRUE);
			ShowWindow(registro, SW_SHOW);
			break;
		case IDOK:
			if (hWnd != regEspescialidades && hWnd != regPacientes)
			{
				GetWindowText(editUsuarioIni, a.usuario, 80);
				GetWindowText(editContraIni, a.constrasenia, 50);
				if (!_usuarios->buscar(a.usuario, a.constrasenia, origen))
				{
					MessageBeep(MB_OK);
					EndDialog(hWnd, TRUE);
				}
				else {
					MessageBox(NULL, "Intente de nuevo", "Advertencia", MB_ICONEXCLAMATION);
				}
			}
			else if (hWnd == regEspescialidades)
			{
				buffer = new char[80];
				nodo = new SPECIALITY::Specialitys;
				GetWindowText(editEspecialidad, buffer, 80);
				nodo->setSpeciality(buffer);
				GetWindowText(editDescripcion, buffer, 80);
				nodo->setDescription(buffer);
				if (listaEspecialidades->validateData(nodo))
				{
					listaEspecialidades->addNode(nodo);
					MessageBox(NULL, "La especialidad se guardo correctamente", "Aviso", MB_ICONINFORMATION);
				}
				else MessageBox(NULL, "La especialidad no puede contener numeros\nIntente de nuevo", "Error", MB_ICONWARNING); 									
				SetWindowText(editDescripcion, "");
				SetWindowText(editEspecialidad, "");
				//if (nodo) {
				//	//GetWindowText(editDescripcion, nodo->temp_description, 80);
				//	listaEspecialidades->addNode(nodo);
				//	//iniciador = new especialidades(a.nombre, a.constrasenia);
				////	listaEspecialidades.insertarFinal(iniciador);
				//	MessageBox(NULL, "Especialidad agregada correctamente", "Aviso", MB_ICONASTERISK);
				//}else MessageBox(NULL, "La especialidad no puede contener numeros\nDebe ser escrita con mayusculas", "Error", MB_ICONWARNING);
			}
			if (hWnd == registrarMedicos)
			{
				temp = new medics;
				//char *tempCedula = new char[20];
				char* tempString = new char[20];;
				GetWindowText(editCedulaMedico, tempString, 20);
				temp->setCedule(atoi(tempString));
				//temp->temp_cedula=atoi(tempCedula);
				//delete[] tempCedula;
				GetWindowText(editNombreMedico, tempString, 20);
				temp->setName(tempString);
				GetWindowText(editApellidoP, tempString, 20);
				temp->set_mLastName(tempString);
				GetWindowText(editApellidoM, tempString, 20);
				temp->set_fLastName(tempString);
				GetWindowText(editTelefonoMedico, tempString, 13);
				temp->setPhone(tempString);
				temp->setPicture(tempPath);
				//temp->temp_day = selected;		
				GetWindowText(cbHorarioInicio, tempString, 7);
				temp->setStartAttention(tempString);
				GetWindowText(cbHorarioFin, tempString, 6);
				temp->setEndAttention(tempString);
				GetWindowText(roomControl, tempString, 2);
				temp->setRoom(tempString);
				GetWindowText(specialityControl, tempString, 20);
				temp->setSpecialityName(tempString, NULL);
				temp->setDaysOfWeek(days(&cbSeleccionarDias, &lbDiasSeleccionados));

				if (arbolMedicos->valTime(arbolMedicos->getRoot(), temp)) {					
					if (arbolMedicos->validateData(temp))
					{
						if (arbolMedicos->insertNode(&medicos, temp, listaEspecialidades))
							MessageBox(NULL, "Agregado", "Registrar Medico", MB_ICONASTERISK);
						else
						{
							MessageBox(NULL, "ERROR::NO PUEDE HABER DOS MEDICOS CON LA MISMA CEDULA::INTENTE REGISTRAR DE NUEVO", "Registrar Medico", MB_ICONASTERISK);
							delete temp;
						}
					}
					else
					{
						MessageBox(NULL, "La cedula solo puede tener 9 digitos\nEl numero de telefono es de 8 a 12 digitos\nEl nombre no pued3 contener numeros", "Informacion no valida", MB_ICONWARNING);
						delete temp;
					}
					
				}
				else {
					MessageBox(NULL, "Este horario no esta disponible", "Error", MB_ICONWARNING);
					delete temp;
				}

				cleanLB(&lbDiasSeleccionados, &cbSeleccionarDias);
			}
			else if (hWnd == regPacientes)
			{
				char *tempString = new char[MAX_PATH];

				GetWindowText(editMedicoPaciente, tempString, 80);
				Patient* tempPaciente = new Patient;
				tempPaciente->setDoctor(tempString, arbolMedicos);
				GetWindowText(editNombrePaciente, tempString, 80);
				tempPaciente->setName(tempString);
				GetWindowText(editApellidoPaciente, tempString, 80);
				tempPaciente->setflastName(tempString);
				GetWindowText(editApellidoPaciente2, tempString, 80);
				tempPaciente->setmLastName(tempString);
				GetWindowText(editTelefonoPaciente, tempString, 12);
				tempPaciente->setPhone(tempString);
				tempPaciente->setReference(setCb.seleccionCursor(&editReferenciaPaciente, CB_GETCURSEL));
				GetWindowText(editFechaDiaPaciente, tempString, 3);
				tempPaciente->setDay(tempString);
				GetWindowText(editFechaMesPaciente, tempString, 3);
				tempPaciente->setMonth(tempString);
				GetWindowText(editFechaAnioPaciente, tempString, 5);
				tempPaciente->setYear(tempString);
				GetWindowText(editEdadPaciente, tempString, 3);
				tempPaciente->setAge(tempString);
				tempPaciente->setGender(setGenero);
				//newNode->setDoctor(tempString);

				/*medics*tempNode = arbolMedicos->getRoot();
				if (tempNode)
					newNode->setDoctor(arbolMedicos->searchByName(&tempNode, tempString));*/

				if (listaPacientes->validateData(tempPaciente))
				{
					listaPacientes->addNode(tempPaciente);
					MessageBox(NULL, "Se registro con exito", "Registro", MB_ICONASTERISK|MB_OK);
				}
				else {
					delete tempPaciente;
					MessageBox(NULL, "El nombre no puede contener digitos\nEl telefono acpeta de 8 a 12 digitos", "Error", MB_ICONWARNING);
				}

				SetWindowText(editNombrePaciente,"");
				SetWindowText(editNombrePaciente, "");
				SetWindowText(editNombrePaciente,"");
				SetWindowText(editApellidoPaciente, "");
				SetWindowText(editApellidoPaciente2, "");
				 
				/*registrarPacientes = new pacientes(a.nombre, setGenero, a.constrasenia, setReferencia, setCb.dia
					, setCb.mes, setCb.anio, a.edad);
				listaPacientes.insertarFinal(registrarPacientes);*/
			}
			else if (hWnd == buscarPaciente)
			{
				buffer = new char[MAX_PATH];
				paciente = NULL;
				cleanLB(&lbMostrarPacientes, NULL);
				if (SendMessage(ckMostrarPacientes, BM_GETCHECK, NULL, NULL))
				{
					GetWindowText(cbBuscarPorTelefono, buffer, 80);
					paciente = listaPacientes->searchByPhone(buffer);
				}
				else if (SendMessage(ckMostrarPorNombre, BM_GETCHECK, NULL, NULL)) {
					GetWindowText(txtBuscarPorNombre, buffer, 80);
					paciente = listaPacientes->patientSearchByName(buffer);
				}
				if (paciente)
				{
					delete buffer;
					buffer = paciente->catName();
					SendMessage(lbMostrarPacientes, LB_INSERTSTRING, NULL, (LPARAM)buffer);
					SendMessage(lbMostrarPacientes, LB_SETSEL, (WPARAM)0, NULL);
					paciente->getName(buffer);
					SetWindowText(txtModNombrePaciente, buffer);
					paciente->getfLastName(buffer);
					SetWindowText(txtModPrimerApellidoPaciente, buffer);
					paciente->getmLastName(buffer);
					SetWindowText(txtModSegundoApellidoPaciente, buffer);
					buffer = sToA(paciente->getPhone());
					SetWindowText(txtModTelefonoPacientes, buffer);
					if (paciente->getGender() == _male)
						SetWindowText(stGenerPaciente, "HOMBRE");
					else SetWindowText(stGenerPaciente, "MUJER");

					char *BirthDay = new char[MAX_PATH];
					buffer = new char[MAX_PATH];
					paciente->getMonth(buffer);
					std::strcpy(BirthDay, buffer);
					strcat(BirthDay, "/");
					paciente->getDay(buffer);
					strcat(BirthDay, buffer); strcat(BirthDay, "/");
					paciente->getyear(buffer);
					strcat(BirthDay, buffer);
					SetWindowText(stFechaNacimientoPaciente, BirthDay);
					paciente->getAge(buffer);
					SetWindowText(txtEditEdad, buffer);
					delete[] BirthDay;
				}
				delete buffer;
			}
			else if (hWnd == agendarCitas)
			{
				buffer = new char[50];
				Dates::Date *tempDate = new Dates::Date;
				GetWindowText(cbHoraCita, buffer, 8);
				tempDate->setHour(buffer);

				GetWindowText(stCedCita, buffer, 15);
				tempDate->setDoctor(arbolMedicos, atoi(buffer));
				GetWindowText(cbPacienteCita, buffer, 50);
				if (!strcmp(buffer, "")) {
					MessageBox(NULL, "Debe escoger un paciente", "Paciente no seleccionado", MB_ICONSTOP);
					break;
				}
				tempDate->setPatient(listaPacientes, buffer);

				GetWindowText(dtpCita, buffer, 80);
				if (tempDate->setDate(buffer,listaCitas->getHead()))
				{
					listaCitas->addNode(tempDate);
					MessageBox(NULL, "La cita se agendo correctamente", "Aviso", MB_ICONASTERISK | MB_OK);
				}
				else {
					
					delete tempDate;
				}

				cleanLB(&lbCitas, NULL);
				listaCitas->displayDates(&lbCitas, listaCitas);
				cleanCB(&cbHoraCita);
				cleanLB(&lbHorarioAgenda,NULL);
				cleanLB(&lbMedics, NULL);
				arbolMedicos->displayFoto(pcAgendaImagen, NULL, NULL, 0, 0);

			}
			else if (consultaMedicoMes == hWnd)
			{
				cleanLB(&lbCitas, NULL);
				char *ced = new char[12];
				char *month = new char[10];
				GetWindowText(cbConsultaMedico, ced, 12);				
				GetWindowText(cbConsultaMes, month, 10);
				listaCitas->searchByDoctorAndMonth(atoi(ced), month, lbCitas, stConsultaNombreMedico, stConsultaEspecialidadMedico);
				MessageBeep(MB_OK);
				delete buffer;
			}
			break;
		case BTN_EDITAR_PACIENTE:

			buffer = new char[MAX_PATH];
			/*	GetWindowText(txtModTelefonoPacientes, buffer, 50);
				paciente = listaPacientes->searchByPhone(buffer);*/
			if (paciente)
			{
				GetWindowText(txtModNombrePaciente, buffer, 50);
				paciente->setName(buffer);
				GetWindowText(txtModPrimerApellidoPaciente, buffer, 50);
				paciente->setflastName(buffer);
				GetWindowText(txtModSegundoApellidoPaciente, buffer, 50);
				paciente->setmLastName(buffer);
				GetWindowText(txtModTelefonoPacientes, buffer, 50);


				GetWindowText(cbEditDiaNacimiento, buffer, 4);

				/*char *birthday[3] = {new char[2], new char[2], new char[4]};
				GetWindowText(stFechaNacimientoPaciente, buffer, 11);*/
				//*(birthday)[0] = buffer[0];
				//*(birthday)[1] = buffer[1];
				//*(birthday + 1)[0] = buffer[3];
				//*(birthday + 1)[1] = buffer[4];
				//*(birthday + 2)[0] = buffer[6];
				//*(birthday + 2)[1] = buffer[7];
				//*(birthday + 2)[2] = buffer[8];
				//*(birthday + 2)[2] = buffer[9];

				paciente->setDay(buffer);
				GetWindowText(cbEditMesNacimiento, buffer, 4);
				paciente->setMonth(buffer);
				GetWindowText(cbEditAnioNacimiento, buffer, 5);
				paciente->setYear(buffer);
				GetWindowText(txtEditEdad, buffer, 2);
				paciente->setAge(buffer);

				MessageBox(NULL, "La informacion fue actualizada con exito", "Informacion Actiualizada", MB_ICONASTERISK);
				SetWindowText(txtModNombrePaciente, "");
				SetWindowText(txtModPrimerApellidoPaciente, "");
				SetWindowText(txtModSegundoApellidoPaciente, "");
				SetWindowText(txtModTelefonoPacientes, "");
;
				cleanLB(&lbMostrarPacientes, NULL);
				listaPacientes->displayList(&lbMostrarPacientes, LB_INSERTSTRING);
			}
			else MessageBox(NULL, "No se puede actualizar la informacion", "No Actiualizada", MB_ICONERROR);


			delete[] buffer;


			break;
		case BTN_ELIMINAR_PACIENTE:
			if (MessageBox(NULL, "Se eliminaran tambien las citas agendadas\n¿Esta de acuerdo?", "Eliminar paciente", MB_OKCANCEL | MB_ICONWARNING) == IDOK)
			{
				if (paciente)
				{
					
					listaCitas->searchByPatientToRemove(paciente->catName());
					cleanLB(&lbMostrarPacientes, NULL);
					listaPacientes->removeNode(paciente);
					listaPacientes->displayList(&lbMostrarPacientes, LB_INSERTSTRING);
					MessageBox(NULL, "La informacion fue actualizada con exito", "Informacion Actiualizada", MB_ICONASTERISK);
					SetWindowText(txtModNombrePaciente, "");
					SetWindowText(txtModPrimerApellidoPaciente, "");
					SetWindowText(txtModSegundoApellidoPaciente, "");
					SetWindowText(txtModTelefonoPacientes, "");
					SetWindowText(txtEditEdad, "");
				}
			}

			break;
		case BTN_SELECT_PHOTO:
			temp = new medics;
			tempPath = new char[MAX_PATH];
			std::strcpy(tempPath, temp->loadPhoto(&pictureControl, &bitmapControl,registrarMedicos));
			delete temp;
			break;
		case BTN_BUSCAR_ESPECIALIDADES:

			//CheckDlgButton(checkTodasEspecialidades, CK_TODOS_ESPECIALIDADES, BST_UNCHECKED);
			//nodo = listaEspecialidades->getHead();
			//while (nodo != NULL)
			//{
			//	SendMessage(lbEspecialidades, LB_ADDSTRING, NULL, (LPARAM)nodo->getSpeciality());
			//	nodo = nodo->getNext();
			//}
			cleanLB(&lbEspecialidades, NULL);
			GetWindowText(editBuscarEspecialidad, a.usuario, 80);
			nodo = listaEspecialidades->SpecilitysSearch(a.usuario);
			if (a.usuario[0] != NULL && nodo != NULL)
			{
				SendMessage(lbEspecialidades, LB_ADDSTRING, NULL, (LPARAM)nodo->getSpeciality());
			}
			else {
				MessageBox(NULL, "La especialidad solicitada no esta registrada o no fue registrada previamente", "Error", MB_ICONWARNING);
			}
			MessageBeep(OK);

			break;
		case CK_TODOS_ESPECIALIDADES:
			cheked = IsDlgButtonChecked(hWnd, CK_TODOS_ESPECIALIDADES);
			if (cheked)
			{
				CheckDlgButton(hWnd, CK_TODOS_ESPECIALIDADES, BST_CHECKED);
				nodo = listaEspecialidades->getHead();
				while (nodo != NULL)
				{
					SendMessage(lbEspecialidades, LB_ADDSTRING, NULL, (LPARAM)nodo->getSpeciality());
					nodo = nodo->getNext();
				}
			}
			else {
				CheckDlgButton(hWnd, CK_TODOS_ESPECIALIDADES, BST_UNCHECKED);
				lbCount = SendMessage(lbEspecialidades, LB_GETCOUNT, 0, 0);
				while (lbCount > 0)
				{
					SendMessage(lbEspecialidades, LB_DELETESTRING, 0, 0);
					SendMessage(lbEspecialidades, LB_GETCOUNT, 0, 0);
					lbCount--;
				}

			}
			break;
		case CK_ENABLE_DOCTORES:
			if (IsDlgButtonChecked(ckMostrarPacientes, CK_ENABLE_DOCTORES) == BST_CHECKED)
				MessageBox(NULL, "CHECKED", "", NULL);
			break;
		case RD_GENERO_HOMBRE:
			setGenero = _male;
			break;
		case RD_GENERO_MUJER:
			setGenero = _female;
			break;
		case BTN_AGENDA_DOCTOR:

			for (int i = 0; i < 7; i++)
				*(date + i) = new char[MAX_PATH];

				GetWindowText(dtpCita, *(date + 5), 50);

				if (Dates::validateDate(*(date + 5), *(date), *(date + 1), *(date + 2), *(date + 3), *(date + 4)))
				{
					GetWindowText(stCedCita, *(date + 6), 20);
					Dates::schedule(&cbHoraCita, arbolMedicos, listaCitas, atoi(*(date + 6)), *(date+5));
					int index = 0;
					while (index < 7)
					{
						delete[] * (date + index);
						index++;
					}
				}
			break;
		case IDCANCEL:
			if (hWnd == regEspescialidades)
			{
				DestroyWindow(regEspescialidades);
				ShowWindow(principal, TRUE);
			}
			else if (hWnd == registrarMedicos)
			{
				DestroyWindow(registrarMedicos);
				ShowWindow(principal, TRUE);
			}
			else if (buscarMedicos == hWnd)
			{
				DestroyWindow(buscarMedicos);
				ShowWindow(principal, SW_SHOW);
			}
			else if (hWnd == buscarEspecialidades)
			{
				DestroyWindow(buscarEspecialidades);
				ShowWindow(principal, SW_SHOW);
			}
			else if (hWnd == regPacientes)
			{
				DestroyWindow(regPacientes);
				ShowWindow(principal, SW_SHOW);
			}
			else if (hWnd == consultaMedicoMes) 
			{
				EnableWindow(principal, TRUE);
				DestroyWindow(consultaMedicoMes);
			}
			else if (hWnd == consultaSemanaEspecialidad)
			{
				EnableWindow(principal, TRUE);
				DestroyWindow(consultaSemanaEspecialidad);
			}
			else if (hWnd == agendarCitas)
			{
				ShowWindow(principal, SW_SHOW);
				DestroyWindow(agendarCitas);
			}
			else if (citasRegistros == hWnd) {
				EnableWindow(principal, TRUE);
				DestroyWindow(citasRegistros);
			}
			else 
			{
				MessageBox(NULL, "El programa se va a cerrar", "Advertencia", MB_ICONERROR | MB_OK);
				DestroyWindow(principal);
				EndDialog(hWnd, TRUE);
				PostQuitMessage(0);
			}
			break;
		case BTN_BUSCAR_MEDICO:
			buffer = new char[10];
			GetWindowText(txtCedula, buffer, 10);
			if (strlen(buffer) >= 9)
			{
				editarMedicoInfo = CreateDialog(hInst, MAKEINTRESOURCE(MODIFICAR_MEDICO), buscarMedicos, reg);
				ShowWindow(editarMedicoInfo, SW_SHOW);
			}
			else MessageBox(NULL, "La cedula es incorrecta\nDebe tener 9 digitos", "Cedula incorrecta", MB_ICONERROR);
			/*medics *tempRoot = arbolMedicos->getRoot();
			tempRoot=arbolMedicos->searchNode(&tempRoot, atoi(tempString));
			cleanLB(&lbNombresMedicos, NULL);*/
			
			break;
		case BTN_SELECCIONAR_MEDICO:
			int cursel = SendMessage(lbMedics, LB_GETCURSEL, NULL, NULL);
			char *selection = new char[MAX_PATH];
			char cedule[12] = "";
			SendMessage(lbMedics, LB_GETTEXT, cursel, (LPARAM)selection);
			if (cursel > 0)
			{
				cursel = 0;
				while (selection[cursel] >= 48 && selection[cursel] <= 57)
				{
					cedule[cursel] = selection[cursel];
					cursel++;
				}
				temp = arbolMedicos->getRoot();
				temp = arbolMedicos->searchNode(&temp, atoi(cedule));
				SetWindowText(stNombreMedico, temp->catName());
				cleanLB(&lbHorarioAgenda, NULL);
				displayDays(&lbHorarioAgenda, temp);
				temp->getSpecialityName(selection);
				SetWindowText(txtEspecialidadCita, selection);
				arbolMedicos->displayFoto(pcAgendaImagen, hDoctorImagen, temp, 150, 150);
				_itoa(temp->getCed(), selection, 10);
				SetWindowText(stCedCita, selection);
				delete[] selection;
			}
			else MessageBox(NULL, "Debe elegir un medico", "Medico no seleccionado", MB_ICONWARNING);
			break;
		}

		switch (HIWORD(wParam))
		{
		case LBN_DBLCLK:
			if (hWnd == buscarEspecialidades)
			{
				lbCount = SendMessage(lbEspecialidades, LB_GETCURSEL,NULL, NULL);
				SendMessage(lbEspecialidades, LB_GETTEXT, lbCount, (LPARAM)a.nombre);
				EnableWindow(buscarEspecialidades, FALSE);
				especialidadEcontrada=CreateDialog(hInst, MAKEINTRESOURCE(MODIFICAR_ESPECIALIDADES), buscarEspecialidades, reg);
				ShowWindow(especialidadEcontrada, SW_SHOW);
				//no se puede eliminar una especialidad si hay doctores en esa especialidad
			}
			if (hWnd == registrarMedicos)
			{
				int *lb = new int[3];
				*lb = SendMessage(lbDiasSeleccionados, LB_GETCOUNT, 0, 0);
				*(lb + 2) = NULL;
				daysOfWeek counter[5];

				while (*(lb) < *(lb+2)+1)
				{
					//*(lb+1) = SendMessage(lbDiasSeleccionados, LB_GETCURSEL, NULL, NULL);
					switch (*(lb + 1))
					{
					case Friday:
						counter[*(lb+2)] = Friday;
						break;
					case Thursday:
						counter[*(lb + 2)] = Thursday;
						break;
					case Wendesday:
						counter[*(lb + 2)] = Wendesday;
						break;
					case Tuesday:
						counter[*(lb + 2)] = Tuesday;
						break;
					case Monday:
						counter[*(lb + 2)] = Monday;
						break;
					}
					*(lb + 2)=*(lb+2)+1;
				}
			}
			else if (hWnd == buscarPaciente)
			{
				char *buffer = new char[MAX_PATH];
				int cursel = SendMessage(lbMostrarPacientes, LB_GETCURSEL, NULL, NULL);
				SendMessage(lbMostrarPacientes, LB_GETTEXT, (WPARAM)cursel, (LPARAM)buffer);
				paciente = listaPacientes->patientSearchByName(buffer);

				paciente->getName(buffer);
				SetWindowText(txtModNombrePaciente, buffer);
				paciente->getfLastName(buffer);
				SetWindowText(txtModPrimerApellidoPaciente, buffer);
				paciente->getmLastName(buffer);
				SetWindowText(txtModSegundoApellidoPaciente, buffer);
				buffer = sToA(paciente->getPhone());
				SetWindowText(txtModTelefonoPacientes, buffer);
				if (paciente->getGender() == _male)
					SetWindowText(stGenerPaciente, "HOMBRE");
				else SetWindowText(stGenerPaciente, "MUJER");

				char *BirthDay = new char[MAX_PATH];
				paciente->getMonth(buffer);
				std::strcpy(BirthDay, buffer);
				strcat(BirthDay, "/");
				paciente->getDay(buffer);
				strcat(BirthDay, buffer); strcat(BirthDay, "/");
				paciente->getyear(buffer);
				strcat(BirthDay, buffer);
				SetWindowText(stFechaNacimientoPaciente, BirthDay);
				paciente->getAge(buffer);
				SetWindowText(txtEditEdad, buffer);
				//delete[] buffer;
				delete[] BirthDay;
			}
			else if (agendarCitas == hWnd && LOWORD(wParam) == LB_HORARIO_AGENDA)
			{
				int cursel = SendMessage(lbMedics, LB_GETCURSEL, NULL, NULL);
				char *selection = new char[MAX_PATH];
				char cedule[12] = "";
				SendMessage(lbMedics, LB_GETTEXT, cursel, (LPARAM)selection);
				cursel = 0;
				while (selection[cursel] >=48 && selection[cursel]<=57 )
				{
					cedule[cursel] = selection[cursel];
					cursel++;
				}
				temp = arbolMedicos->getRoot();
				temp = arbolMedicos->searchNode(&temp, atoi(cedule));
				SetWindowText(stNombreMedico, temp->catName());
				cleanLB(&lbHorarioAgenda, NULL);
				displayDays(&lbHorarioAgenda, temp);
				temp->getSpecialityName(selection);
				SetWindowText(txtEspecialidadCita, selection);
				delete[] selection;
			}
			break;
		case CBN_SELENDOK:
			if (hWnd == registrarMedicos&&LOWORD(wParam)==CB_SELECCION_DIAS)
			{
				char *tempDay = new char[10];			

				int cb = SendMessage(cbSeleccionarDias, CB_GETCURSEL, NULL, NULL);
				SendMessage(cbSeleccionarDias, CB_GETLBTEXT, cb, (LPARAM)tempDay);
				SendMessage(lbDiasSeleccionados, LB_ADDSTRING, (WPARAM)cb, (LPARAM)tempDay);
				SendMessage(cbSeleccionarDias, CB_DELETESTRING, cb,NULL);
				delete[] tempDay;
			}
			else if (hWnd == buscarMedicos && LOWORD(wParam) == CB_SELECT_VIEW)
			{
				cleanLB(&lbNombresMedicos, NULL);

				if (SendMessage(cbBuscar, CB_GETCURSEL, NULL, NULL) == 1)
					EnableWindow(cbEspecialidadMedico, TRUE);
				else if (SendMessage(cbBuscar, CB_GETCURSEL, NULL, NULL) == 0) {
					arbolMedicos->showMedics(&lbNombresMedicos, 0,LB_INSERTSTRING, NULL);
					EnableWindow(cbEspecialidadMedico, FALSE);
				}
				else if (SendMessage(cbBuscar, CB_GETCURSEL, NULL, NULL) == 2) {
					arbolMedicos->showMedics(&lbNombresMedicos, 2,LB_INSERTSTRING, NULL);
					EnableWindow(cbEspecialidadMedico, FALSE);
				}
					
			}
			else if (hWnd == buscarMedicos && LOWORD(wParam) == CB_ESPECIALIDAD)
			{
				cleanLB(&lbNombresMedicos, NULL);
				char *tempString = new char[15];
				int cursel = SendMessage(cbEspecialidadMedico, CB_GETCURSEL, NULL, NULL);
				SendMessage(cbEspecialidadMedico, CB_GETLBTEXT, cursel, (LPARAM)tempString);
				arbolMedicos->showMedics(&lbNombresMedicos, 1, LB_ADDSTRING, tempString);
			}
			else if (LOWORD(wParam) == CB_EDIT_DIA_PACIENTE)
			{
				changeBirthDay(&cbEditDiaNacimiento,&stFechaNacimientoPaciente,2,3);

			}
			else if (LOWORD(wParam) == CB_EDIT_MES_PACIENTE) changeBirthDay(&cbEditMesNacimiento, &stFechaNacimientoPaciente,2,0);
			else if (LOWORD(wParam) == CB_EDIT_ANIO_PASIENTE) {
				changeBirthDay(&cbEditAnioNacimiento, &stFechaNacimientoPaciente, 4, 6);

			}
			else if (LOWORD(wParam) == CB_ESCOGER_MEDICO)
			{
				if (SendMessage(cbDisplayBy, CB_GETCURSEL, NULL, NULL)==1)
				{
					EnableWindow(cbSpecialitys, FALSE);
					cleanLB(&lbMedics,NULL);
					//Dates::displayDoctors(lbMedics, arbolMedicos->getRoot(), tab);
					arbolMedicos->showMedics(&lbMedics, 2, LB_INSERTSTRING, NULL);
					SendMessage(lbMedics, LB_INSERTSTRING, (WPARAM)0, (LPARAM)"Cedula\t\t\tNombre");
				}
				else EnableWindow(cbSpecialitys, TRUE);

			}
			else if (LOWORD(wParam) == CB_ESPECIALIDAD_CITA) {
				char *speciality = new char[15];
				cleanLB(&lbMedics, NULL);
				int cursel = SendMessage(cbSpecialitys, CB_GETCURSEL, NULL, NULL);
				SendMessage(cbSpecialitys, CB_GETLBTEXT, cursel, (LPARAM)speciality);
				arbolMedicos->showMedics(&lbMedics, 1, NULL, speciality);
				SendMessage(lbMedics, LB_INSERTSTRING, (WPARAM)0, (LPARAM)"Cedula\t\t\tNombre");
				delete[] speciality;
			}
			else if (LOWORD(wParam) == CB_CONSULTA_MEDICO)
			{
				buffer = new char[50];
				medics* tempMedics = arbolMedicos->getRoot();
				GetWindowText(cbConsultaMedico, buffer, 10);
				tempMedics = arbolMedicos->searchNode(&tempMedics, atoi(buffer));
				SetWindowText(stConsultaNombreMedico, tempMedics->catName());
				tempMedics->getSpecialityName(buffer);
				SetWindowText(stConsultaEspecialidadMedico, buffer);
			}
			else if (LOWORD(wParam) == DTP_AGENDA_CITA)
			{
				/*SendMessage(dtpCita,)
					DTM_SETRANGE*/

			}
			else if (LOWORD(wParam) == CB_CONSULTA_SEMANA)
			{
				/*buffer = new char[10];
				char *semana = new char[3];
				int cursel = SendMessage(cbConsultaSemana, CB_GETCURSEL, NULL, NULL);
				Dates::WEEK week = Dates::First;
				SendMessage(cbConsultaSemana, CB_GETLBTEXT, (WPARAM)cursel,(LPARAM)semana);
				GetWindowText(cbConsultaMesSemana, buffer, 10);
				switch (cursel)
				{
				case Dates::First: week = Dates::First; break;
				case Dates::Second: week = Dates::Second; break;
				case Dates::Third: week = Dates::Third; break;
				case Dates::Fourth: week = Dates::Fourth; break;
				}

				listaCitas->weekly(buffer, week);
*/
			}
			else if (LOWORD(wParam) == CB_TIPO_REGISTRO)
			{				
				buffer = new char[15];				
				char *month = new char[10];
				char *semana = new char[3];
				char *speciality = new char[3];
				Dates::WEEK week = Dates::First;
				int cursel = SendMessage(cbRegistro, CB_GETCURSEL, NULL, NULL);
				switch(cursel)//citas del medico en el mes
				{
				case 0:
					cleanLB(&lbCitas, NULL);					
					GetWindowText(cbConsultaMedico, buffer, 12);
					GetWindowText(cbConsultaMes, month, 10);
					if(strlen(month)>0 && strlen(buffer)>0)
						listaCitas->searchByDoctorAndMonth(atoi(buffer), month, lbCitas, stConsultaNombreMedico, stConsultaEspecialidadMedico);
					else MessageBox(NULL, "Favor de llenar los campos", "Aviso", MB_ICONSTOP);
					MessageBeep(MB_OK);
					break;
				case 1://citas por mes y semana					
					/*cursel = SendMessage(cbConsultaSemana, CB_GETCURSEL, NULL, NULL);				
					SendMessage(cbConsultaSemana, CB_GETLBTEXT, (WPARAM)cursel, (LPARAM)semana);*/
					GetWindowText(cbConsultaMes, month, 10);
					GetWindowText(cbConsultaSemana, semana, 10);
					switch (*semana)
					{
					case '1': week = Dates::First; break;
					case '2': week = Dates::Second; break;
					case '3': week = Dates::Third; break;
					case '4': week = Dates::Fourth; break;
					}
					if (strlen(month) > 0 && strlen(semana) > 0)
						listaCitas->weekly(month, week);
					else MessageBox(NULL, "Favor de llenar los campos", "Aviso", MB_ICONSTOP);
					break;
				case 2:
					GetWindowText(cbRegistroEspecialidad, speciality, 15);
					GetWindowText(cbConsultaMes, month, 10);
					GetWindowText(cbConsultaSemana, semana, 3);

					switch (*semana)
					{
					case '1': week = Dates::First; break;
					case '2': week = Dates::Second; break;
					case '3': week = Dates::Third; break;
					case '4': week = Dates::Fourth; break;
						
					}
					if (strlen(month) > 0 && strlen(semana) > 0&&strlen(speciality)>0)
						listaCitas->speciality(speciality, month, week);
					else MessageBox(NULL, "Favor de llenar los campos", "Aviso", MB_ICONSTOP);
					break;
				}
				//SendMessage(cbRegistro, CB_GETLBTEXT, (WPARAM)cursel, (LPARAM)buffer);
				delete[] month;
				delete[] buffer;
				delete[] semana;
			}
			//else if (LOWORD(wParam) == CB_ESPECIALIDAD)
			//{
			//	int cursel = SendMessage(cbBuscarPorTelefono, CB_GETCURSEL, NULL, NULL);
			//	char *tempString = new char[MAX_PATH];
			//	SendMessage(cbBuscarPorTelefono, CB_GETLBTEXT, (WPARAM)cursel, (LPARAM)tempString);
			//	//listaPacientes->displayByDoctor(&lbMostrarPacientes, tempString);
			//}
			break;

		case BN_CLICKED:
			if (SendMessage(ckMostrarPacientes, BM_GETCHECK, NULL, NULL))
			{
				EnableWindow(cbBuscarPorTelefono, TRUE);
				SendMessage(ckMostrarPorNombre, BM_SETCHECK, BST_UNCHECKED, 0);
			}
			else if (!SendMessage(ckMostrarPacientes, BM_GETCHECK, NULL, NULL))
				EnableWindow(cbBuscarPorTelefono, FALSE);
			if (SendMessage(ckMostrarPorNombre, BM_GETCHECK, NULL, NULL))
			{
				EnableWindow(txtBuscarPorNombre, TRUE);
				SendMessage(ckMostrarPacientes, BM_SETCHECK, BST_UNCHECKED, 0);
			}
			else if (!SendMessage(txtBuscarPorNombre, BM_GETCHECK, NULL, NULL))
				EnableWindow(txtBuscarPorNombre, FALSE);

			if (LOWORD(wParam) == CK_TODOS_ESPECIALIDADES)
			{
				if (SendMessage(checkTodasEspecialidades, BM_GETCHECK, NULL, NULL))
				{
					cleanLB(&lbEspecialidades,NULL);
					nodo = listaEspecialidades->getHead();
					while (nodo != NULL)
					{
						SendMessage(lbEspecialidades, LB_ADDSTRING, NULL, (LPARAM)nodo->getSpeciality());
						nodo = nodo->getNext();
					}

				}
			}
				//cleanLB(&lbMostrarPacientes, NULL);
				/*medics* temp = arbolMedicos->getRoot();
				arbolMedicos->showMedics(&cbBuscarPorTelefono, 2, CB_INSERTSTRING, NULL);*/

			//if (LOWORD(wParam) == DTP_AGENDA_CITA)
			//{
			//	char *fecha = new char[30];

			//	GetWindowText(dtpCita, fecha, 30);
			//	//if (Dates::validateDate(fecha)) MessageBox(NULL, "correcto", "", NULL);
			//	//else MessageBox(NULL, "incorrecto", "", NULL);

			//}

			break;
		}
		break;

	case WM_NOTIFY:

		break;

	case WM_DESTROY:
	
		break;
	case WM_CLOSE:

		if(hWnd==iniSes) EndDialog(hWnd, TRUE);
		if (hWnd == regEspescialidades)
		{
			DestroyWindow(hWnd);
			ShowWindow(principal, TRUE);
		}
		if (hWnd == regPacientes)
		{
			DestroyWindow(hWnd);
			ShowWindow(principal, TRUE);
		}
		if (hWnd == registrarMedicos)
		{
			DestroyWindow(hWnd);
			ShowWindow(principal, SW_SHOW);
		}
		if (hWnd == buscarMedicos)
		{
			DestroyWindow(hWnd);
			ShowWindow(principal, SW_SHOW);
		}
		if (hWnd == buscarPaciente)
		{
			MessageBox(NULL, "La informacion no guardada se perdera", "Informacion", MB_ICONINFORMATION);
			DestroyWindow(hWnd);
			ShowWindow(principal, SW_SHOW);
		}
		break;
	default:
		
		break;
	}

	return FALSE;
};

BOOL CALLBACK reg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

	char *tempString = NULL;

	medics*tempNode = NULL;
	SPECIALITY::Specialitys *temp = NULL;

	switch (msg)
	{
	case WM_INITDIALOG:

		btnModName = GetDlgItem(hwnd, BTN_EDIT_NOMBRE);
		btnModFoto = GetDlgItem(hwnd, BTN_EDIT_FOTO);
		lbModDias = GetDlgItem(hwnd, LB_DIAS_EDITADOS);
		btnBuscarDias = GetDlgItem(hwnd, BTN_BUSCAR_DIAS);
		cbModDias = GetDlgItem(hwnd, CB_EDIT_DIAS);
		cbModHorario = GetDlgItem(hwnd, CB_EDIT_HORARIO);
		cbModespecialidad = GetDlgItem(hwnd, CB_EDIT_ESPECIALIDAD);
		cbModDias = GetDlgItem(hwnd, CB_EDIT_DIAS);
		txtSWCedula = GetDlgItem(hwnd, TXT_EDIT_CEDULA);
		modImagen = GetDlgItem(hwnd, EDIT_FOTO);
		modNombreMedicom = GetDlgItem(hwnd, TXT_EDIT_NOMBRE_MEDICO);
		modTelefonoMedico = GetDlgItem(hwnd, TXT_EDIT_TELEFONO);
		editModificarEspecialidad = GetDlgItem(hwnd, TXT_ESPECIALIDAD_ENCONTRADA);
		editModificarDescripcion = GetDlgItem(hwnd,TXT_DESCRIPCION_ECONTRADA);
		nodo = listaEspecialidades->SpecilitysSearch(a.nombre);
		SetWindowText(editModificarEspecialidad, nodo->getSpeciality());
		SetWindowText(editModificarDescripcion, nodo->getDescription());
		editNombreReg = GetDlgItem(hwnd, 1004);
		editUsuarioReg = GetDlgItem(hwnd, 1005);
		editContraReg = GetDlgItem(hwnd, 1006);

		tempString = new char[MAX_PATH];

			GetWindowText(txtCedula, tempString, 10);
			SetWindowText(txtSWCedula, tempString);
			tempNode = arbolMedicos->getRoot();
			tempNode = arbolMedicos->searchNode(&tempNode, atoi(tempString));
		if (tempNode){

			tempNode->getName(tempString, 0);
			SetWindowText(modNombreMedicom, tempString);
			tempNode->getPhone(tempString);
			SetWindowText(modTelefonoMedico, tempString);

			tempNode->getPhoto(tempString);
			bitmapControl = (HBITMAP)LoadImage(NULL, tempString, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
			SendMessage(modImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmapControl);

			tempNode->getSpecialityName(tempString);

			SetWindowText(cbModespecialidad, tempString);
			lbDiasActuales = GetDlgItem(hwnd, LB_DIAS_ACTUALES);
			displayDays(&lbDiasActuales, tempNode);


			SendMessage(cbModHorario, CB_INSERTSTRING, 0, (LPARAM)"Tarde");
			SendMessage(cbModHorario, CB_INSERTSTRING, 0, (LPARAM)"Mañana");

			setCb.setRoom(&cbModCuarto, &hwnd, CB_EDIT_CUARTO, 5);

			temp = listaEspecialidades->getHead();
			while (temp != NULL)
			{
				SendMessage(cbModespecialidad, CB_INSERTSTRING, NULL, (LPARAM)temp->getSpeciality());
				temp = temp->getNext();
			}
		}

		break;
	case WM_COMMAND:
		switch (LOWORD(wparam))
		{
		case IDOK:
			if (hwnd == especialidadEcontrada)
			{
				GetWindowText(editNombreReg, a.nombre, 80);
				GetWindowText(editUsuarioReg, a.usuario, 80);
				GetWindowText(editContraReg, a.constrasenia, 50);
				cleanLB(&lbEspecialidades, NULL);
				SendMessage(checkTodasEspecialidades, BM_SETCHECK, BST_CHECKED,NULL);
				//inicio->agregar(a.nombre,a.usuario,a.constrasenia);
				DestroyWindow(hwnd);
			}
			else if (hwnd == editarMedicoInfo)
			{
				tempString = new char[MAX_PATH];
				GetWindowText(txtCedula, tempString, 10);
				tempNode = arbolMedicos->getRoot();
				tempNode = arbolMedicos->searchNode(&tempNode, atoi(tempString));

				GetWindowText(modTelefonoMedico, tempString, 14);
				tempNode->setPhone(tempString);

				GetWindowText(cbModespecialidad, tempString, 20);
				if(strcmp(tempString,""))
					tempNode->setSpecialityName(tempString, listaEspecialidades);

				while (SendMessage(cbModDias, CB_GETCOUNT, NULL, NULL) != NULL)
					SendMessage(cbModDias, CB_DELETESTRING, NULL, NULL);

				MessageBox(NULL, "Informacion actualizada con exito", "Informacion actualizada", MB_ICONASTERISK);

				delete[] tempString;
			}

			break;
		case BTN_MODIFICAR:
			GetWindowText(editModificarEspecialidad, a.nombre, 80);
			if (listaEspecialidades->modifyNode(nodo, SPECIALITY::_Specialitys, a.nombre))
			{
				GetWindowText(editModificarDescripcion, a.nombre, 80);
				listaEspecialidades->modifyNode(nodo, SPECIALITY::_Description, a.nombre);
				MessageBox(NULL, "Cambios guardos exitosamente", "Aviso", MB_ICONASTERISK);
			}
			else
			{
				MessageBox(NULL, "La especialidad no puede contener numeros\nIntente de nuevo", "Error", MB_ICONWARNING);
				SetWindowText(editModificarEspecialidad, nodo->getSpeciality());
			}
			break;
		case BTN_ELIMINAR_ESPECIALIDAD:
			GetWindowText(editModificarEspecialidad, a.nombre, 80);
			tempNode = arbolMedicos->getRoot();
			if (!arbolMedicos->veryfySpeciality(tempNode, a.nombre))
			{
				if (listaCitas->searchBySpeciality(a.nombre))
					MessageBox(NULL, "La especialidad no se puede eliminar\nElimine las citas pendienes de esa espcialidad", "No se puede eliminar", MB_ICONWARNING);					
				else listaEspecialidades->removeNode(a.nombre);

			}			
			else MessageBox(NULL, "La especialidad no de puede eliminar\n elimine todas sus dependencias como\nmedicos o citas", "Error", MB_ICONSTOP);

			cleanLB(&lbEspecialidades, NULL);
			SendMessage(checkTodasEspecialidades, BM_SETCHECK, BST_UNCHECKED, NULL);
			break;
		case BTN_EDIT_NOMBRE:
			EnableWindow(editarMedicoInfo, FALSE);
			setName = CreateDialog(hInst, MAKEINTRESOURCE(CAMBIAR_NOMBRE_MEDICO), editarMedicoInfo, cambios);
			ShowWindow(setName, SW_SHOW);
			break;
		case BTN_BUSCAR_DIAS:
			if (true)
			{
				cleanCB(&cbModDias);

				char* tempHorario = new char[10];
				char* tempCuarto = new char[10];
				GetWindowText(cbModHorario, tempHorario, 10);
				GetWindowText(cbModCuarto, tempCuarto, 10);

				daysOfWeek*selected = new daysOfWeek[5];
				for (int i = 0; i < 5; i++)
					*(selected + i) = Free;

				tempNode->editDays(tempHorario, tempCuarto, arbolMedicos->getRoot(), selected);

				for (int i = 0; i < 5; i++)
				{
					if (*(selected + i) == Free)
					{
						switch (i)
						{
						case 0:
							SendMessage(cbModDias, CB_INSERTSTRING, 0, (LPARAM)"Lunes");
							break;
						case 1:
							SendMessage(cbModDias, CB_INSERTSTRING, 0, (LPARAM)"Martes");
							break;
						case 2:
							SendMessage(cbModDias, CB_INSERTSTRING, 0, (LPARAM)"Miercoles");
							break;
						case 3:
							SendMessage(cbModDias, CB_INSERTSTRING, 0, (LPARAM)"Jueves");
							break;
						case 4:
							SendMessage(cbModDias, CB_INSERTSTRING, 0, (LPARAM)"Viernes");
							break;
						}
					}
				}

				delete[] selected;
			}
			break;
		case BTN_EDIT_FOTO:

			tempString = new char[MAX_PATH];

			GetWindowText(txtCedula, tempString, 10);
			tempNode = arbolMedicos->getRoot();
			tempNode = arbolMedicos->searchNode(&tempNode, atoi(tempString));

			std::strcpy(tempString, tempNode->loadPhoto(&modImagen, &bitmapControl, editarMedicoInfo));
			bitmapControl = (HBITMAP)LoadImage(NULL, tempString, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
			SendMessage(modImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmapControl);

			break;

		case BTN_ELIMINAR_MEDICO:
			tempString = new char[MAX_PATH];
			if (MessageBox(NULL, "Se eliminaran todas las citas pendientes", "Continuar", MB_YESNO | MB_ICONINFORMATION) == IDYES)
			{
				GetWindowText(modNombreMedicom, tempString,MAX_PATH);
				listaCitas->searchByMedicToRemove(tempString);
				GetWindowText(txtCedula, tempString, 10);				
				tempNode = arbolMedicos->getRoot();
				//tempNode = arbolMedicos->searchNode(&tempNode, atoi(tempString));
				//arbolMedicos->deleteNode(tempNode);
				
				arbolMedicos->removeNode(tempNode, atoi(tempString));	
				cleanLB(&lbCitas, NULL);
				listaCitas->displayDates(&lbCitas, listaCitas);
				MessageBox(NULL, "Se elimino el medico\nLa ventana se cerrara", "Aviso", MB_ICONASTERISK);
				ShowWindow(buscarMedicos, SW_SHOW);
				DestroyWindow(hwnd);
			}
			delete tempString;

			break;
		case IDCANCEL:
			//MessageBox(NULL, "Los cambios no guardados se perderan", "Aviso", MB_ICONEXCLAMATION);
			if (hwnd == especialidadEcontrada) {
				EnableWindow(buscarEspecialidades, TRUE);
				DestroyWindow(hwnd);
			}
			else if(hwnd == editarMedicoInfo) {
				EnableWindow(buscarMedicos, TRUE);
				DestroyWindow(hwnd);
			}
			break;
		case BTN_CAMBIAR_HORARIO:
			tempString = new char[MAX_PATH];

			GetWindowText(txtCedula, tempString, 10);
			tempNode = arbolMedicos->getRoot();
			tempNode = arbolMedicos->searchNode(&tempNode, atoi(tempString));
			tempNode->setDaysOfWeek(days(NULL, &lbModDias));

			GetWindowText(cbModHorario, tempString, 7);
			tempNode->setStartAttention(tempString);
			GetWindowText(cbModCuarto, tempString, 3);
			tempNode->setRoom(tempString);

			cleanLB(&lbDiasActuales, NULL);
			displayDays(&lbDiasActuales, tempNode);

			delete[] tempString;
			break;
		}

		switch (HIWORD(wparam))
		{
		case CBN_SELENDOK:

			if (LOWORD(wparam) == CB_EDIT_DIAS)
			{
				char *daySelected = new char[10];

				int coursel = SendMessage(cbModDias, CB_GETCURSEL, NULL, NULL);
				SendMessage(cbModDias, CB_GETLBTEXT, coursel, (LPARAM)daySelected);
				SendMessage(lbModDias, LB_ADDSTRING, (WPARAM)coursel, (LPARAM)daySelected);
				SendMessage(cbModDias, CB_DELETESTRING, coursel, NULL);

				delete[] daySelected;

			}

			break;

		case LBN_DBLCLK:

			if (LOWORD(wparam) == LB_DIAS_EDITADOS)
			{
				char *dayRemoved = new char[10];

				int coursel = SendMessage(lbModDias, LB_GETCURSEL, NULL, NULL);
				SendMessage(lbModDias, LB_GETTEXT, coursel, (LPARAM)dayRemoved);
				SendMessage(cbModDias, CB_INSERTSTRING, coursel, (LPARAM)dayRemoved);
				SendMessage(lbModDias, LB_DELETESTRING, coursel, NULL);

			}

			break;
		}


		break;
	case WM_CLOSE:
		if (hwnd == especialidadEcontrada)
			DestroyWindow(hwnd);
		else { DestroyWindow(hwnd); EnableWindow(buscarMedicos, TRUE); }
		break;
	default:
		//return DefWindowProc(hwnd, msg, wparam, lparam);
		break;
	}
	return FALSE;
}

BOOL CALLBACK cambios(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	char *tempString = NULL;
	char *completeName[3] = { NULL,NULL,NULL };
	medics* tempNode = NULL;

	switch (msg)
	{
	case WM_INITDIALOG:

		tempString = new char[MAX_PATH];

		txtName = GetDlgItem(hWnd, TXT_NOMBRE);
		GetWindowText(txtCedula, tempString, 10);
		txtM_LastName = GetDlgItem(hWnd, TXT_M_APELLIDO);
		txtF_LastName = GetDlgItem(hWnd, TXT_P_APELLIDO);

		tempNode = arbolMedicos->getRoot();
		if (tempNode)
		{
			tempNode = arbolMedicos->searchNode(&tempNode, atoi(tempString));

			tempNode->getName(tempString, true);
			SetWindowText(txtName, tempString);

			tempNode->get_mLastName(tempString);
			SetWindowText(txtM_LastName, tempString);

			tempNode->get_fLastName(tempString);
			SetWindowText(txtF_LastName, tempString);
		}


		delete[] tempString;

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:

			tempString = new char[MAX_PATH];

			completeName[0] = new char[MAX_PATH];
			completeName[1] = new char[MAX_PATH];
			completeName[2] = new char[MAX_PATH];

			GetWindowText(txtCedula, tempString, 15);

			tempNode = arbolMedicos->getRoot();
			tempNode = arbolMedicos->searchNode(&tempNode, atoi(tempString));
			if (tempNode)
			{
				GetWindowText(txtName, completeName[0], 50);
				GetWindowText(txtM_LastName, completeName[2], 50);
				GetWindowText(txtF_LastName, completeName[1], 50);
				arbolMedicos->modifyName(tempNode, completeName[0], completeName[2], completeName[1]);


				tempNode->getName(tempString, 0);
				SetWindowText(modNombreMedicom, tempString);

				EnableWindow(editarMedicoInfo, TRUE);
				DestroyWindow(hWnd);

				delete[] completeName[0];
				delete[] completeName[1];
				delete[] completeName[2];

				delete[] tempString;
			}
			else MessageBox(NULL, "La informacion no se pudo actualizar\nasegurese de haber ingresado bien la cedula o la informacion", "Error", MB_ICONWARNING);

			break;
		case IDCANCEL:
			EnableWindow(editarMedicoInfo, TRUE);
			DestroyWindow(hWnd);
			break;

		}
	}
	return FALSE;
}


daysOfWeek* days(HWND *cbControl, HWND *lbControl)
{
	char *tempString = new char[10];
	int index = NULL;



	daysOfWeek* tempDay = new daysOfWeek[5];

		for (int i = 0; i < 5; i++)
			*(tempDay + i) = Free;

	while (index<5)
	{
		SendMessage(*lbControl, LB_GETTEXT, 0, (LPARAM)tempString);
		if (!strcmp(tempString, "Lunes"))
			*(tempDay) = Monday;
		if (!strcmp(tempString, "Martes"))
			*(tempDay + 1) = Tuesday;
		if (!strcmp(tempString, "Miercoles"))
			*(tempDay + 2) = Wendesday;
		if (!strcmp(tempString, "Jueves"))
			*(tempDay + 3) = Thursday;
		if (!strcmp(tempString, "Viernes"))
			*(tempDay + 4) = Friday;
		//if (!strcmp(tempString, ""))
		//	*(tempDay + index) = Free;
		SendMessage(*lbControl,LB_DELETESTRING,NULL,NULL);
		index++;
		std::strcpy(tempString,"");
	}	

	delete[] tempString;

	return tempDay;
}

void cleanLB(HWND* lbControl,HWND* cbControl)
{
	while (SendMessage(*lbControl, LB_GETCOUNT, NULL, NULL) != NULL)
		SendMessage(*lbControl, LB_DELETESTRING, NULL, NULL);

	if(cbControl!=NULL) refilCB(cbControl);
	return;
}

void refilCB(HWND*control)
{

	int count = SendMessage(*control, CB_GETCOUNT, NULL, NULL);

	while (SendMessage(*control, CB_GETCOUNT, NULL, NULL) != NULL)
		SendMessage(*control, CB_DELETESTRING, NULL, NULL);	

	SendMessage(*control, CB_INSERTSTRING, 0, (LPARAM)"Lunes");
	SendMessage(*control, CB_INSERTSTRING, 0, (LPARAM)"Martes");
	SendMessage(*control, CB_INSERTSTRING, 0, (LPARAM)"Miercoles");
	SendMessage(*control, CB_INSERTSTRING, 0, (LPARAM)"Jueves");
	SendMessage(*control, CB_INSERTSTRING, 0, (LPARAM)"Viernes");

	return;
}

void displayDays(HWND* lbControl, medics* node)
{
	SendMessage(*lbControl, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Dias\tHorario\tCuarto");

	int tab[4] = { 50,80,130,160 };

	char *tempString = new char[MAX_PATH];

	for (int i = 0; i < 5; i++)
	{
		char *buffer = new char[512];

		switch (*(node->getDaysOfWeek() + i))
		{
		case Monday:
			std::strcpy(tempString, "Lunes\t");
			break;
		case Tuesday:
			std::strcpy(tempString, "Martes\t");
			break;
		case Wendesday:
			std::strcpy(tempString, "Miercoles\t");
			break;
		case Thursday:
			std::strcpy(tempString, "Jueves\t");
			break;
		case Friday:
			std::strcpy(tempString, "Viernes\t");
			break;
		}

		if (*(node->getDaysOfWeek() + i) != Free)
		{
			node->getStartAttention(buffer);
			strcat(tempString, buffer);
			node->getRoom(buffer);
			strcat(tempString, "\t");
			strcat(tempString, buffer);
			sprintf(buffer, tempString, i, i * 213);

			SendMessage(*lbControl, LB_SETCURSEL, (WPARAM)0, 0);
			SendMessage(*lbControl, LB_SETTABSTOPS, (WPARAM)4, (LPARAM)tab);

			SendMessage(*lbControl, LB_ADDSTRING, (WPARAM)0, (LPARAM)buffer);
		}

		delete[] buffer;
	}

	delete[] tempString;
}

void changeBirthDay(HWND* control, HWND *text, int quantity, int _begin)
{
	char *buffer = new char[12];
	char *fecha = new char[12];
	for (int i = 0; i < 11; i++)
		*(fecha + i) = NULL;

	int cursel = SendMessage(*control, CB_GETCURSEL, NULL, NULL);
	SendMessage(*control, CB_GETLBTEXT, (WPARAM)cursel, (LPARAM)buffer);

	GetWindowText(stFechaNacimientoPaciente, fecha, 12);
	int fechaPos = _begin, bufferPos = 0;
	int i = 0;
	while (i < quantity)
	{
		fecha[fechaPos] = buffer[bufferPos];
		fechaPos++;
		bufferPos++;
		i++;
	}
	//strcpy(fecha[0], buffer[0]);
	SetWindowText(*text, fecha);
	delete[] fecha;
}

void controls(HWND hWnd)
{
	//especialidades
	checkTodasEspecialidades = GetDlgItem(hWnd, CK_TODOS_ESPECIALIDADES);
	//agendarCita
	cbSpecialitys = GetDlgItem(hWnd, CB_ESPECIALIDAD_CITA);
	cbDisplayBy = GetDlgItem(hWnd, CB_ESCOGER_MEDICO);
	lbMedics = GetDlgItem(hWnd, LB_MEDICOS);
	stNombreMedico = GetDlgItem(hWnd, ST_NOMBRE_MEDICO);
	lbHorarioAgenda = GetDlgItem(hWnd, LB_HORARIO_AGENDA);
	txtEspecialidadCita = GetDlgItem(hWnd, TXT_SPECIALIDAD_CITA);
	btnSeleccionarMedico = GetDlgItem(hWnd, BTN_SELECCIONAR_MEDICO);
	pcAgendaImagen = GetDlgItem(hWnd, PC_AGENDA_IMAGEN);
	cbPacienteCita = GetDlgItem(hWnd, CB_PACIENTE_CITA);
	dtpCita = GetDlgItem(hWnd, DTP_AGENDA_CITA);
	stCedCita = GetDlgItem(hWnd, ST_CEDULA_CITA);
	cbHoraCita = GetDlgItem(hWnd, CB_HORA_CITA);
	btnEliminar = GetDlgItem(hWnd, BTN_ELIMINAR_PACIENTE);
	//CONSULTA DE CITAS POR MEDICO
	txtConsultaMedicoMes = GetDlgItem(hWnd, TXT_CEDULA_MEDICO);
	cbConsultaMes = GetDlgItem(hWnd, CB_CONSULTA_MES);
	cbConsultaMedico = GetDlgItem(hWnd, CB_CONSULTA_MEDICO);
	stConsultaNombreMedico = GetDlgItem(hWnd,ST_CONSULTA_NOMBRE_MEDICO);
	stConsultaEspecialidadMedico = GetDlgItem(hWnd, ST_CONSULTA_MEDICO_ESPECIALIDAD);
	cbConsultaMesSemana = GetDlgItem(hWnd, CB_MES_CONSULTA);
	//cbConsultaSemana = GetDlgItem(hWnd, CB_CONSULTA_SEMANA);
	cbConsultaSemana = GetDlgItem(hWnd, CB_REGISTRO_SEMANA);
	cbRegistro = GetDlgItem(hWnd, CB_TIPO_REGISTRO);
	cbRegistroEspecialidad = GetDlgItem(hWnd, CB_REGISTRO_ESPECIALIDAD);
	//stDate = GetDlgItem(hWnd, ST_DATE);
}

void loadIcon()
{
	itconButton = (HBITMAP)LoadImage(NULL, "icons\\Eliminate.bmp", IMAGE_BITMAP, 100, 50, LR_LOADFROMFILE);
	SendMessage(btnEliminar, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)itconButton);
	iconSelect = (HBITMAP)LoadImage(NULL, "icons\\correct.bmp", IMAGE_BITMAP, 100, 50,LR_LOADFROMFILE);
	SendMessage(btnSeleccionarMedico, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)iconSelect);
}

void addMenu(HWND hWnd)
{
	hMenu = CreateMenu();
	hPopEspecialidades = CreateMenu();
	hPopMedicos = CreateMenu();
	hPopPacientes = CreateMenu();
	hPopCitas = CreateMenu();
	hSubCitas = CreateMenu();
	hSubConsulta = CreateMenu();

	//TOOL BAR
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hPopEspecialidades, "ESPECIALIDADES");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hPopMedicos, "MEDICOS");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hPopPacientes, "PACIENTES");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hPopCitas, "CITAS");

	//POP MENUS
	AppendMenu(hPopMedicos, MF_STRING, ALTA_MEDICOS, "REGISTRAR");
	AppendMenu(hPopMedicos, MF_STRING, BUSCAR_MEDICOS, "BUSCAR");
	AppendMenu(hPopMedicos, MF_STRING, REPORTES_MEDICOS, "REPORTES");
	AppendMenu(hPopPacientes, MF_STRING, REPORTES_PACEINTES, "REPORTES");
	AppendMenu(hPopPacientes, MF_STRING, BUSCAR_PACIENTE, "BUSCAR");
	AppendMenu(hPopPacientes, MF_STRING, PACIENTES, "REGISTRAR");
	AppendMenu(hPopEspecialidades, MF_STRING, REGISTRAR_ESPECIALIDADES, "REGISTRAR");
	AppendMenu(hPopEspecialidades, MF_STRING, BUSCAR_ESPECIALIDADES, "BUSCAR");
	AppendMenu(hPopCitas, MF_STRING, REGISTRAR_CITAS, "REGISTRAR");
	AppendMenu(hPopCitas, MF_POPUP, (UINT_PTR)hSubCitas, "CONSULTA");

	// SUB CITAS
	AppendMenu(hSubCitas, MF_STRING, CITAS_REGISTROS, "REGISTROS");
	AppendMenu(hSubCitas, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hSubCitas, MF_POPUP, (UINT_PTR)hSubConsulta, "CONSULTA");

	//SUB CONSULTA
	AppendMenu(hSubConsulta, MF_STRING,CONSULTA_MEDICO_MES , "CONSULTA POR MEDICO Y MES");
	AppendMenu(hSubConsulta, MF_STRING, CONSULTA_SEMANA_ESPECIALIDAD, "ORDENAR POR ESPECIALIDAD Y SEMANA");


	SetMenu(hWnd, hMenu);
}

void cleanCB(HWND*cbControl)
{
	while (SendMessage(*cbControl, CB_GETCOUNT, NULL, NULL) != NULL)
		SendMessage(*cbControl, CB_DELETESTRING, NULL, NULL);
}

void addControls(HWND hWnd)
{
	lbCitas = CreateWindowExW(WS_EX_CLIENTEDGE
		, L"LISTBOX", NULL
		, WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_USETABSTOPS | LBS_MULTICOLUMN | LBS_NOTIFY
		, 40, 35, 350, 150
		, hWnd, NULL, hInst, NULL);

	createStatic(hWnd, "Buscar por guia", 400, 15, 50, 20, SS_LEFT);
	createStatic(hWnd, "Citas", 40, 15, 50, 20, SS_LEFT);
	stDiaCita = createStatic(hWnd, "", 100, 200, 100, 50, SS_LEFT);
	createStatic(hWnd, "Fecha", 40, 200, 150, 50, SS_LEFT);
	createStatic(hWnd, "Hora", 40, 225, 100, 50, SS_LEFT);
	stHora = createStatic(hWnd, "", 100, 225, 100, 50, SS_LEFT);
	createStatic(hWnd, "Cuarto", 40, 250, 100, 300, SS_LEFT);
	stCuarto = createStatic(hWnd, "", 100, 250, 10, 50,SS_LEFT);
	createStatic(hWnd, "Doctor", 200, 200, 100, 50, SS_LEFT);
	stNombreDoctor = createStatic(hWnd, "", 200,225, 150, 50, SS_LEFT);
	createStatic(hWnd, "Especialidad", 200, 275, 100, 50, SS_LEFT);
	stEspecialidad = createStatic(hWnd, "", 200, 300, 150, 50, SS_LEFT);
	createStatic(hWnd, "Nombre paciente", 40, 350, 200, 50, SS_LEFT);
	stNombrePaciente = createStatic(hWnd, "", 40, 375, 200, 50, SS_LEFT);
	createStatic(hWnd, "Telefono", 40, 425, 100, 50, SS_LEFT);
	stTelefono = createStatic(hWnd, "", 40, 450, 200, 50, SS_LEFT);
	createStatic(hWnd, "Motivos de cancelacion", 400, 375, 150, 50, SS_LEFT);
	txtBuscarCita = CreateWindowEx(NULL, "EDIT", ""
		, WS_VISIBLE | SS_LEFT | WS_CHILD | WS_BORDER | ES_NUMBER
		, 400, 35, 100, 20	
		, hWnd, NULL, hInst, NULL);

	btnBuscar = CreateWindowEx(NULL, "BUTTON", "Buscar"
		, WS_VISIBLE | SS_CENTER | WS_CHILD
		, 400, 70, 100, 20
		, hWnd, (HMENU)BUSCAR_CITA, hInst, NULL);

	ckSatus = CreateWindowEx(NULL, "BUTTON", "Terminada"
		, BS_CHECKBOX | WS_VISIBLE | WS_CHILD
		, 400, 350, 100, 40
		, hWnd, (HMENU)CHECK_FINISHED, hInst, NULL);
	
	pcImage = CreateWindowEx(NULL, "STATIC", ""
		, WS_VISIBLE | WS_CHILD | SS_BITMAP | WS_BORDER
		, 400, 200, 150, 150
		, hWnd, NULL, hInst, NULL);

	cbCancelacion = CreateWindowEx(NULL, "COMBOBOX"
		, NULL, CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | WS_OVERLAPPED
		, 400,425, 150, 500
		, hWnd, (HMENU)CB_CANCEL, GetModuleHandle(NULL), NULL);


	return;
}

HWND createStatic(HWND hWnd,const char* _text, int _xPos, int _yPos, int _xSize, int _ySize, int SS_STYLE)
{
	return CreateWindowEx(NULL, "STATIC", _text
		, WS_CHILD | WS_VISIBLE | SS_STYLE
		, _xPos, _yPos, _xSize, _ySize
		, hWnd, NULL, hInst, NULL);
}
