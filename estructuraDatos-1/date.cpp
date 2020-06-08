#include "date.h"

Dates::Date::Date():next(NULL), prev(NULL) {

}

Dates::Node::Node(nodePatient* _patient, tree* _medic){

	ifstream myFile;
	medics *tempMedic = NULL;
	myFile.open("Bin\\Citas.bin", ios::in | ios::binary | ios::ate);

	Date* tempNode = NULL;
	if (myFile.is_open())
	{
		int size = myFile.tellg();

		for (int index = 0; index < size / sizeof(Date); index++)
		{
			if (!head)
			{
				head = new Date;
				tempNode = new Date;
				myFile.seekg(0);
				myFile.read(reinterpret_cast<char*>(tempNode), sizeof(Date));
				head->medicCedule = tempNode->medicCedule;
				tempMedic = _medic->getRoot();
				head->medic =_medic->searchNode(&tempMedic, head->medicCedule);
				strcpy(head->patientName, tempNode->patientName);
				head->patient=_patient->patientSearchByName(head->patientName);
				head->dateCode = tempNode->dateCode;
				strcpy(head->day, tempNode->day);
				strcpy(head->month, tempNode->month);
				strcpy(head->year, tempNode->year);
				strcpy(head->dd, tempNode->dd);
				strcpy(head->mm, tempNode->mm);
				strcpy(head->date, tempNode->date);
				strcpy(head->hour, tempNode->hour);
				head->week = tempNode->week;
				head->dayId = tempNode->dayId;
				head->status = tempNode->status;
				head->dateCode = tempNode->dateCode;
				head->next = NULL;
				head->prev = NULL;
				Date::datesQuantity++;
				delete reinterpret_cast<char*>(tempNode);
				continue;
			}

			tail = head;
			while (tail->next != NULL)
				tail = tail->next;

			tail->next = new Date;
			tempNode = new Date;
			myFile.seekg(index * sizeof(Date));
			myFile.read(reinterpret_cast<char*>(tempNode), sizeof(Date));
			tail->next->prev = tail;
			tail = tail->next;
			tail->medicCedule = tempNode->medicCedule;
			tempMedic=_medic->getRoot();
			tail->medic=_medic->searchNode(&tempMedic, tail->medicCedule);

			strcpy(tail->patientName, tempNode->patientName);
			tail->patient = _patient->patientSearchByName(tail->patientName);
			tail->dateCode = tempNode->dateCode;
			strcpy(tail->day, tempNode->day);
			strcpy(tail->month, tempNode->month);
			strcpy(tail->year, tempNode->year);
			strcpy(tail->dd, tempNode->dd);
			strcpy(tail->mm, tempNode->mm);
			strcpy(tail->date, tempNode->date);
			strcpy(tail->hour, tempNode->hour);
			tail->week = tempNode->week;
			tail->dayId = tempNode->dayId;
			tail->status = tempNode->status;
			tail->dateCode = tempNode->dateCode;

			tail->next = NULL;
			Date::datesQuantity++;
			delete reinterpret_cast<char*>(tempNode);
		}
	}

}

Dates::Node::~Node(){

	Date *tempNode = head;
	ofstream myFile;
	myFile.open("Bin\\Citas.bin", ios::binary | ios::out | ios::trunc);

	if (myFile.is_open()&&tempNode!=NULL)
	{

		while (tempNode != NULL)
		{
			myFile.write(reinterpret_cast<char*>(tempNode), sizeof(Date));
			tempNode = tempNode->next;			
		}

		myFile.close();
		tempNode = this->head;
		while (tempNode != this->tail)
		{	
			this->head = tempNode->next;			
			delete tempNode;
			tempNode = head;
		}

		delete tail;
	}

}

void Dates::Node::addNode(Dates::Date* newNode)
{
	Date *tempNode = NULL;

	if (!head)
	{
		head = newNode;
		head->next = NULL;
		head->prev = NULL;
		head->status = true;
		head->medicCedule = head->medic->getCed();
		Date::datesQuantity++;
		return;
	}

	tempNode = head;
	while (tempNode->next != NULL)
		tempNode = tempNode->next;

	tempNode->next = newNode;
	tempNode->next->next = NULL;
	tempNode->next->prev = tempNode;
	tempNode = tempNode->next;
	tempNode->status = true;
	tempNode->medicCedule = tempNode->medic->getCed();
	tail = tempNode;
	Date::datesQuantity++;

	return;
}

bool Dates::Node::removeNode(Date *node)
{
	Date *tempNode = NULL;
	if (!node) return false;

	if (node->next == NULL && node->prev == NULL){
		delete node;
		this->head = NULL;
		this->tail = NULL;
		Date::datesQuantity--;
		return true;
	}
	else if (node->next!=NULL,node->prev==NULL) {
		this->head = node->next;
		node->next->prev = NULL;
		delete node;
		Date::datesQuantity--;
		return true;
	}
	else if (node->next != NULL && node->prev != NULL) {
		node->next->prev = node->prev;
		node->prev->next = node->next;
		delete node;
		Date::datesQuantity--;
		return true;
	}
	else if (node->next == NULL && node->prev != NULL) {
		tail = node->prev;
		tail->next = NULL;
		node->prev = NULL;
		delete node;
		Date::datesQuantity--;
		return true;
	}

	return false;
}

bool Dates::Date::setDate(char* date, Date* head)
{
	char*buffer=NULL;
	bool validDay = false;

	if (validateDate(date, this->day, this->month, this->year, this->dd, this->mm))
	{
		if (atoi(dd) <= 4)
		{
			this->week = First;
		}
		else if (atoi(dd) <= 15)
		{
			this->week = Second;
		}
		else if (atoi(dd) <= 20)
		{
			this->week = Third;
		}
		else if (atoi(dd) <= 31)
		{
			this->week = Fourth;
		}

		this->status = false;

		strcpy(this->date, date);

		if (!strcmp(this->day, "lunes"))
		{
			this->dayId = Monday;
		}
		else if (!strcmp(this->day, "martes"))
		{
			this->dayId = Tuesday;
		}
		else if (!strcmp(this->day, "miércoles"))
		{
			this->dayId = Wendesday;
		}
		else if (!strcmp(this->day, "jueves"))
		{
			this->dayId = Thursday;
		}
		else if (!strcmp(this->day, "viernes")) {
			this->dayId = Friday;
		}

		daysOfWeek *dayDate = this->medic->getDaysOfWeek();
		int index = 0;
		if (this->dayId != Free)
		{
			while ((*(dayDate + index)) != this->dayId)
			{
				if (index >= 4)
				{
					MessageBox(NULL, "No esta dentro del horario de trabajo", "Aviso", MB_ICONEXCLAMATION | MB_OK);
					return false;
				}
				index++;
			}
		}
		else {
			MessageBox(NULL, "No esta dentro del horario de trabajo", "Aviso", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}

		
		//if (this->dayId == Free)
		//{
		//	//buffer = this->medic->catName();
		//	//strcat(buffer, " no esta disponible es dia");
		//	MessageBox(NULL, "El doctor no esta disponoble ese dia","Error",NULL);
		//	return false;
		//}
		Date*tempNode = head;
		while (tempNode != NULL)
		{
			this->dateCode = _LOW_LIMIT + rand() % (_RAND_RANGE(1000, 100));
			if (dateCode == tempNode->dateCode)
				continue;
			tempNode = tempNode->next;
		}

		return true;
	}
	MessageBox(NULL, "La fecha de la cita no es valida\nasegurese de que sea en la fehca avutal\no menor a 3 meses ", "Aviso", MB_ICONERROR | MB_OK);
	return false;
}

void Dates::Date::setDoctor(tree *node, int ced)
{
	medics* tempNode = node->getRoot();
	this->medic = node->searchNode(&tempNode, ced);
	this->medicCedule = this->medic->getCed();
}

void Dates::Date::setPatient(nodePatient *node,char* name)
{
	this->patient=node->patientSearchByName(name);
	strcpy(this->patientName, patient->catName());

}

void Dates::inicializateDisplayBy(HWND* _cbcontrol_1)
{
	SendMessage(*_cbcontrol_1, CB_INSERTSTRING, NULL, (LPARAM)"Todos");
	SendMessage(*_cbcontrol_1, CB_INSERTSTRING, NULL, (LPARAM)"Especialidad");
}

void Dates::inicializateDisplayEspecialtys(HWND*control,SPECIALITY::Node* _specialitysList)
{
	SPECIALITY::Specialitys* tempNode = _specialitysList->getHead();
	while (tempNode != NULL)
	{
		SendMessage(*control, CB_INSERTSTRING, NULL, (LPARAM)tempNode->getSpeciality());
		tempNode = tempNode->getNext();
	}
}

void Dates::displayDoctors(HWND lbcontrol, medics* node,int tab[])
{
	if (!node)
		return;

	
	catRow(node,lbcontrol,tab);
	displayDoctors(lbcontrol, node->getLeft(), tab);
	displayDoctors(lbcontrol, node->getRight(), tab);

}

void Dates::catRow(medics* node, HWND lbcontrol,int tab[])
{
	char *buffer = new char[10];
	char *row = NULL;
	row = node->catName();
	std::strcat(row, "\t");
	_itoa(node->getCed(), buffer, 10);
	std::strcat(row, buffer);

	delete[] buffer;

	SendMessage(lbcontrol, LB_SETCURSEL, (WPARAM)0, 0);
	SendMessage(lbcontrol, LB_SETTABSTOPS, (WPARAM)2, (LPARAM)tab);

	SendMessage(lbcontrol, LB_ADDSTRING, (WPARAM)0, (LPARAM)row);
	delete[] row;
}

bool Dates::validateDate(char *date, char* _day, char* _month, char* _year, char*_dd,char *_mm)
{
	bool flag = false;
	time_t actualTime = time(NULL);
	struct tm *timeInfo = NULL;
	timeInfo = localtime(&actualTime);
	char actualDate[12];
	strftime(actualDate, 11, "%d/%m/%Y", timeInfo);

	char day[3]="";
	char month[3] = "";
	char year[5] = "";

	format(date,_month,_day,_year,_dd,_mm);

	char *acom = new char[3];

	for (int i = 0; i <= 62; i++)
	{
		if (!strcmp(date, actualDate)) return true;


		day[0] = actualDate[0];
		day[1] = actualDate[1];
		int _dia = atoi(day) + 1;
		if (_dia <= 9)
		{
			_itoa(_dia, acom, 10);
			strcpy(day, "0");
			strcat(day, acom);
			actualDate[0] = day[0];
			actualDate[1] = day[1];
		}
		else
		{
			_itoa(_dia, day, 10);
			actualDate[0] = day[0];
			actualDate[1] = day[1];
		}
		if (atoi(day) > 31)
		{
			month[0] = actualDate[3];
			month[1] = actualDate[4];
			int _mes = atoi(month) + 1;
			if (_mes <= 9)
			{
				_itoa(_mes, acom, 10);
				strcpy(month, "0");
				strcat(month, acom);
				actualDate[3] = month[0];
				actualDate[4] = month[1];
				strcpy(day, "01");
				actualDate[0] = day[0];
				actualDate[1] = day[1];
			}
			else {
				_itoa(_mes, month, 10);
				actualDate[3] = month[0];
				actualDate[4] = month[1];
			}


			if (atoi(month) > 12)
			{
				year[0] = actualDate[6];
				year[1] = actualDate[7];
				year[2] = actualDate[8];
				year[3] = actualDate[9];
				int _anio = atoi(year) + 1;
				_itoa(_anio, year, 10);
				actualDate[6] = year[0];
				actualDate[7] = year[1];
				actualDate[8] = year[2];
				actualDate[9] = year[3];

				strcpy(month, "01");
				actualDate[3] = month[0];
				actualDate[4] = month[0];
			}

		}
	}


	return false;

}

Dates::Date* Dates::Node::searchDate(int code)
{
	Date *tempDate = this->head;
	while (tempDate != NULL)
	{
		if (tempDate->getCode() == code) return tempDate;
		tempDate = tempDate->next;
	}

	return NULL;

}

void Dates::format(char *date, char *_month, char *_day, char *_year, char *_dd, char* _mm)
{
	char day[10] = "";
	char month[10] = "";
	char year[5] = "";
	char dd[3] = "";
	char mm[3] = "";

	int index = 0, counter = 0;

	while (*(date+index)!=44)
	{
		day[counter] = date[index];
		index++;
		counter++;
	}

	strcpy(_day, day);

	dd[0] = *(date+(index+2));
	dd[1] = *(date+(index+3));

	if (atoi(dd) <= 9)
	{
		index += 7;
		dd[1] = dd[0], dd[0] = 48;
	}
	else index += 8;
	//strcmp(dd, "1 ") ? index += 8 : index += 7, dd[1]=dd[0], dd[0]=48;

	counter = 0;

	while (*(date + index) != 32)
	{
		month[counter] = *(date + index);
		index++ ,counter ++;
	}

	index += 4;
	counter = 0;

	while (*(date + index) != NULL)
	{
		year[counter] = *(date + index);
		counter++, index++;
	}



	strcpy(_day, day);
	strcpy(_month, month);
	strcpy(_year,year);
	strcpy(_dd, dd);
	strcpy(_mm, monthValue(month));
	
	strcpy(mm, _mm);
	strcpy(date, dd);
	strcat(date, "/");
	strcat(date, mm);
	strcat(date,"/");
	strcat(date, year);

}

char* Dates::monthValue(char* _month)
{
	int index = 0;
	char mm[4] = "";
	char *temp = new char[3];

	char*months[14] = {

		NULL,
	  new char[12],
	  new char[12],
	  new char[12],
	  new char[12],
	  new char[12],
	  new char[12],
	  new char[12],
	  new char[12],
	  new char[12],
	  new char[12],
	  new char[12],
	  new char[12]
	};

	strcpy(*(months + 1), "enero");
	strcpy(*(months + 2), "febrero");
	strcpy(*(months + 3), "marzo");
	strcpy(*(months + 4), "abril");
	strcpy(*(months + 5), "mayo");
	strcpy(*(months + 6), "junio");
	strcpy(*(months + 7), "julio");
	strcpy(*(months + 8), "agosto");
	strcpy(*(months + 9), "septiembre");
	strcpy(*(months + 10), "octubre");
	strcpy(*(months + 11), "noviembre");
	strcpy(*(months + 12), "diciembre");
	index = 1;

	while (strcmp(_month, *(months + index)) != 0)
		index++;

	_itoa(index, mm, 10);

	if (index <= 9)
	{
		strcpy(temp, mm);
		strcpy(mm, "0");
		strcat(mm, temp);
	}

	index = 1;
	while (*(months + index) != NULL)
	{
		delete[] * (months + index);
		index++;
	}

	delete[] temp;


	return mm;
}

void Dates::schedule(HWND* cbControl, tree *node,Dates::Node* dateList,int ced, char* dateScheduled)
{
	char *agenda[32];
	char * buffer=new char[8];
	bool equal = false;
	//node->medic->getStartAttention(buffer);
	medics*temp = node->getRoot();
	temp = node->searchNode(&temp, ced);
	temp->getStartAttention(buffer);

	!strcmp(buffer, "Mañana") ? displayTime(0, agenda) : displayTime(1, agenda);

	delete[] buffer;

	Date *tempNode = new Date;
	tempNode = dateList->getHead();

	short int index = 0;

	while (tempNode != NULL)
	{
		if (!strcmp(tempNode->getDate(), dateScheduled))
		{
			if (tempNode->getMedicCed() == ced)
			{

				while(index <= 32)
				{     					
					if (*(agenda + index) == NULL) {
						index++;
						continue;
					}
					else if (!strcmp(tempNode->getHour(), (*(agenda + index)))) break;					

					index++;
				}

				delete[] * (agenda + index);
				*(agenda + index) = NULL;
				index++;
			}
		}
		tempNode = tempNode->getNext();
		index = 0;
	}

	index = 32;

	while (index >= 0)
	{
		if(*(agenda + index)) SendMessage(*cbControl, CB_INSERTSTRING, NULL, (LPARAM)*(agenda + index));
		index--;
	}

}

void  Dates::displayTime( bool turn, char *agenda[])
{
	short int hour = 9, minutes = 0;
	char tempTimeString[8] = ""; 
	char tempMinuteString[4] = "";
	char format[8] = "00:00";
	bool adjusted = false;
	short int index = 0;

	short int end = 0;

	if (!turn)
	{
		end = 14;
		hour = 6;
	}
	else {
		end = 21;
		hour = 14;
	}

	while (hour < end)
	{
		while (minutes <= 60)
		{
			_itoa(hour, tempTimeString, 10);
			_itoa(minutes, tempMinuteString, 10);
			if (hour <= 9)
			{
				char *adjust = new char[8];
				strcpy(adjust, &tempTimeString[0]);
				strcpy(tempTimeString, "0");
				strcat(tempTimeString, adjust);
				delete adjust;

			}
			if (minutes == 0)
			{
				strcat(tempMinuteString, "0");
				strcat(tempTimeString, ":");
				strcat(tempTimeString, tempMinuteString);
			}
			else
			{
				strcat(tempTimeString, ":");
				strcat(tempTimeString, tempMinuteString);
			}
			*(agenda + index) = new char[6];
			strcpy(*(agenda + index), tempTimeString);
			index++;
			minutes += 20;
		}
		hour++;
		minutes = 0;
	}
}

void Dates::Node::displayDates(HWND *hControl, Dates::Node* node)
{
	char *buffer = new char[MAX_PATH];
	Date *tempNode = this->head;
	SendMessage(*hControl, LB_ADDSTRING,(WPARAM) 0, (LPARAM)"No.Cita\tFecha\t\tHora\tSatus");
	while (tempNode != NULL)
	{
		_itoa(tempNode->dateCode, buffer, 10);
		strcat(buffer, "\t");
		strcat(buffer, tempNode->date);
		strcat(buffer, "\t");
		strcat(buffer, tempNode->hour);
		strcat(buffer, "\t");
		if (tempNode->status)
			strcat(buffer, "ACTIVA");
		else strcat(buffer, "Finalizada");
		SendMessage(*hControl, LB_ADDSTRING, NULL, (LPARAM)buffer);
		tempNode = tempNode->getNext();
	}
	delete[] buffer;
	return;
}

char* Dates::getIdDate(char* data) {

	char guide[4] = "";
	int aux = 0;
	while (data[aux] >= 48 && data[aux] <= 57)
	{
		guide[aux] = data[aux];
		aux++;
	}
	return guide;

}

int Dates::Node::binarySearch(Date* arrayList,int data, int size)
{
	short int first = 0, last = size, n=0;

	short int med = (last + first) / 2;
	while (first <= last&&n<=size)
	{
		if (arrayList[med].medic->getCed() == data)
		{	
			return med;
		}
		if (arrayList[med].medic->getCed() > data)
		{
			last = med;
			med = (last + first) / 2;
		}
		if (arrayList[med].medic->getCed() < data)
		{
			first = med;
			med = (last + first) / 2;
		}
		n++;
	}
	MessageBox(NULL, "El medico no tiene citas de momento", "Estado", MB_ICONSTOP);
	return 0;
}

void Dates::Node::displayByMonth(HWND*hControl, int ced, char* month)
{
	char *buffer = new char[MAX_PATH];
	Date *tempNode = this->head;
	int last, first, med;
	SendMessage(*hControl, LB_ADDSTRING, (WPARAM)0, (LPARAM)"No.Cita\tFecha\t\tHora\tSatus");
	while (tempNode != NULL)
	{
		if (tempNode->medicCedule == ced && strcmp(tempNode->month, month) == NULL)
		{
			_itoa(tempNode->dateCode, buffer, 10);
			strcat(buffer, "\t");
			strcat(buffer, tempNode->date);
			strcat(buffer, "\t");
			strcat(buffer, tempNode->hour);
			strcat(buffer, "\t");
			if (tempNode->status)
				strcat(buffer, "Finalizada");
			else strcat(buffer, "Activa");
			SendMessage(*hControl, LB_ADDSTRING, NULL, (LPARAM)buffer);				

		}
		tempNode = tempNode->getNext();
	}
}

void Dates::setMonths(HWND control, HWND cbControl)
{
	SendMessage(control, CB_INSERTSTRING, NULL, (LPARAM)"diciembre");
	SendMessage(control, CB_INSERTSTRING, NULL, (LPARAM)"noviembre");
	SendMessage(control, CB_INSERTSTRING, NULL, (LPARAM)"octubre");
	SendMessage(control, CB_INSERTSTRING, NULL, (LPARAM)"septiembre");
	SendMessage(control, CB_INSERTSTRING, NULL, (LPARAM)"agosto");
	SendMessage(control, CB_INSERTSTRING, NULL, (LPARAM)"julio");
	SendMessage(control, CB_INSERTSTRING, NULL, (LPARAM)"junio");
	SendMessage(control, CB_INSERTSTRING, NULL, (LPARAM)"mayo");
	SendMessage(control, CB_INSERTSTRING, NULL, (LPARAM)"abril");
	SendMessage(control, CB_INSERTSTRING, NULL, (LPARAM)"marzo");
	SendMessage(control, CB_INSERTSTRING, NULL, (LPARAM)"febrero");
	SendMessage(control, CB_INSERTSTRING, NULL, (LPARAM)"enero");

	if (cbControl)
	{
		int week = 1;
		char buffer[3] = "";
		for (week; week < 5; week++)
		{
			_itoa(week, buffer, 10);
			SendMessage(cbControl, CB_ADDSTRING, NULL, (LPARAM)buffer);
		}
	}

}

int Dates::Date::datesQuantity = 0;

Dates::Date* Dates::Node::arrayList()
{

	Date* tempNode = this->head;
	Date* arr = new Date[Date::datesQuantity];
	int index = 0;
	char *buffer = NULL;

	while (tempNode != NULL)
	{

		swapNode((arr + index), tempNode);

		tempNode = tempNode->next;
		index++;
	}

	heapSort(arr, Date::datesQuantity);
	return arr;

}

void Dates::Node::heapSort(Date* arreglo, int n)
{
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arreglo, n, i);

	for (int i = n - 1; i >= 0; i--)
	{
		Date* temporal = new Date;

		//temporal->_cedula = (arreglo + i)->_cedula;
		//std::strcpy(temporal->_name, (arreglo + i)->_name);
		//std::strcpy(temporal->_fLastName, (arreglo + i)->_fLastName);
		//std::strcpy(temporal->_mLastName, (arreglo + i)->_mLastName);
		swapNode(temporal, (arreglo + i));
		swapNode((arreglo + i), arreglo);
		swapNode((arreglo), temporal);
		/*(arreglo + i)->_cedula = (arreglo)->_cedula;
		std::strcpy((arreglo + i)->_name, (arreglo)->_name);
		std::strcpy((arreglo + i)->_fLastName, (arreglo)->_fLastName);
		std::strcpy((arreglo + i)->_mLastName, (arreglo)->_mLastName);*/

		//(arreglo)->_cedula = temporal->_cedula;
		//std::strcpy((arreglo)->_name, temporal->_name);
		//std::strcpy((arreglo)->_fLastName, temporal->_fLastName);
		//std::strcpy((arreglo)->_mLastName, temporal->_mLastName);

		delete temporal;

		heapify(arreglo, i, 0);

	}
}

void Dates::Node::heapify(Date* arreglo, int n, int i)
{
	int largo = i;
	int izq = 2 * i + 1;
	int der = 2 * i + 2;

	if (izq < n && (arreglo + izq)->medic->getCed() >(arreglo + largo)->medic->getCed())
		largo = izq;

	if (der < n && (arreglo + der)->medic->getCed() >(arreglo + largo)->medic->getCed())
		largo = der;

	if (largo != i)
	{
		Date* temporal = new Date;

		//temporal->_cedula = (arreglo + i)->_cedula;
		//strcpy(temporal->_name, (arreglo + i)->_name);
		//strcpy(temporal->_fLastName, (arreglo + i)->_fLastName);
		//strcpy(temporal->_mLastName, (arreglo + i)->_mLastName);
		//strcpy(temporal->_room, (arreglo + i)->_room);
		swapNode(temporal, (arreglo + i));
		swapNode((arreglo + i), (arreglo + largo));
		swapNode((arreglo + largo), temporal);

		/*(arreglo + i)->_cedula = (arreglo + largo)->_cedula;
		strcpy((arreglo + i)->_name, (arreglo + largo)->_name);
		strcpy((arreglo + i)->_fLastName, (arreglo + largo)->_fLastName);
		strcpy((arreglo + i)->_mLastName, (arreglo + largo)->_mLastName);
		strcpy((arreglo + i)->_room, (arreglo + largo)->_room);*/

		/*(arreglo + largo)->_cedula = temporal->_cedula;
		strcpy((arreglo + largo)->_name, temporal->_name);
		strcpy((arreglo + largo)->_fLastName, temporal->_fLastName);
		strcpy((arreglo + largo)->_mLastName, temporal->_mLastName);
		strcpy((arreglo + largo)->_room, temporal->_room);*/

		delete temporal;

		heapify(arreglo, n, largo);
		//swap((arreglo+i),(arreglo+largo));
	}
}

void Dates::Node::swapNode(Date*_destination, Date*_source) {

	strcpy(_destination->day, _source->day);
	strcpy(_destination->month, _source->month);
	strcpy(_destination->year, _source->year);
	strcpy(_destination->mm, _source->mm);
	strcpy(_destination->dd, _source->dd);
	strcpy(_destination->date, _source->date);
	strcpy(_destination->hour, _source->hour);
	strcpy(_destination->patientName, _source->patientName);
	_destination->medic = _source->medic;
	_destination->medicCedule = _source->medicCedule;
	_destination->dateCode = _source->dateCode;
	_destination->week = _source->week;
	return;
}

void Dates::Node::searchByDoctorAndMonth(int ced, char* month,HWND hControl, HWND name, HWND speciality)
{
	/*ofstream Registry;
	Registry.open("RegistrosConsultas\\Registro por medico y mes.txt", ios::out | ios::trunc);
	char *buffer = new char[MAX_PATH];
	Date*list = arrayList();
	int pos = binarySearch(list, ced, Date::datesQuantity);
	SetWindowText(name, list[pos].medic->catName());
	SetWindowText(speciality, list[pos].getSpeciality());
	int index = 0;

	if (!Registry.is_open()) return;
	SendMessage(hControl, LB_ADDSTRING, (WPARAM)0, (LPARAM)"No.Cita\tFecha\t\tHora\tSatus");
	while (index < Date::datesQuantity)
	{
		if (list[index].medic->getCed() == list[pos].medic->getCed() && strcmp(list[index].month, month) == NULL)
		{
			_itoa(list[index].dateCode, buffer, 10);
			strcat(buffer, "\t");
			strcat(buffer, list[index].date);
			strcat(buffer, "\t");
			strcat(buffer, list[index].hour);
			strcat(buffer, "\t");
			if (list[index].status)
				strcat(buffer, "ACTIVA");
			else strcat(buffer, "Finalizada");
			SendMessage(hControl, LB_ADDSTRING, NULL, (LPARAM)buffer);

			
			GetCurrentDirectory(MAX_PATH, buffer);
			if (MessageBox(NULL, buffer, "Se guardo en: ", MB_OK | MB_ICONHAND) == IDOK)
			{
				system("Registro por medico y mes.txt");
			}

		}
		index++;
	}*/

	ofstream Registry;
	Registry.open("RegistrosConsultas\\Registro por medico y mes.txt");
	Date* pendingList = new Date[Date::datesQuantity];
	for (int i = 0; i < Date::datesQuantity; i++)
		pendingList[i].medic = NULL;
	int index = 0;
	Date* tempNode = this->head;
	if (!Registry.is_open()) return;
	while (tempNode != NULL)
	{
		if (tempNode->status)
		{

			swapNode((pendingList + index), tempNode);
			index++;
		}
		tempNode = tempNode->next;
	}
	int pos = binarySearch(pendingList, ced, Date::datesQuantity);
	index = 0;
	int room = 1;
	char *buffer = new char[4];

	while (index < Date::datesQuantity && pendingList[index].medic != NULL)
	{

		if (pendingList[index].medic->getCed() == pendingList[pos].medic->getCed() && strcmp(pendingList[index].month, month) == NULL)
		{
			Registry << "-----------------------------------------------------------------------" << endl;
			Registry << "Nombre del medico: " << pendingList[index].medic->catName() << "\t Cedula: " << pendingList[index].medicCedule << "\nEspecialidad: " << pendingList[index].getSpeciality() << endl;
			Registry << "Fecha de cita: " << pendingList[index].date << "\tHora: " << pendingList[index].hour << "\tCuarto: " << pendingList[index].getRoom() << "\tRegistro: " << pendingList[index].dateCode << endl;
			Registry << "Nombre del paciente: " << pendingList[index].patientName << endl;
		}
	index++;
	}
	index = 0;
	MessageBox(NULL, "El registro se guardo correctamente", "Aviso", MB_ICONINFORMATION);
	Registry.close();

	delete[] buffer;
	delete[] pendingList;
}

void Dates::Node::weekly(char* month, WEEK week)
{
	ofstream Registry;
	Registry.open("RegistrosConsultas\\Registro Semana Mes.txt");
	Date* pendingList = new Date[Date::datesQuantity];
	for (int i = 0; i < Date::datesQuantity; i++)
		pendingList[i].medic = NULL;
	int index = 0;	
	Date* tempNode = this->head;
	if (!Registry.is_open()) return;
	while (tempNode != NULL)
	{
		if (tempNode->status)
		{

			swapNode((pendingList + index), tempNode);
			index++;
		}
		tempNode = tempNode->next;
	}

	index = 0;
	int room = 1;
	char *buffer = new char[4];
	while (room != 5)	
	{
		while (index < Date::datesQuantity && pendingList[index].medic!=NULL ) 
		{
			_itoa(room, buffer, 10);
			if (!strcmp(pendingList[index].getRoom(),buffer))
			{
				if (pendingList[index].week == week && strcmp(pendingList[index].month, month) == NULL)
				{
					Registry << "-----------------------------------------------------------------------" << endl;
					Registry << "Nombre del medico: " << pendingList[index].medic->catName() << "\t Cedula: " << pendingList[index].medicCedule << "\nEspecialidad: " << pendingList[index].getSpeciality() << endl;
					Registry << "Fecha de cita: " << pendingList[index].date << "\tHora: " << pendingList[index].hour << "\tCuarto: " << pendingList[index].getRoom() << "\tRegistro: " << pendingList[index].dateCode << endl;
					Registry << "Nombre del paciente: " << pendingList[index].patientName << endl;
				}
			}
			index++;
		}
		index = 0;
		room++;
	}
	MessageBox(NULL, "El registro se guardo correctamente", "Aviso", MB_ICONINFORMATION);
	Registry.close();
	delete[] pendingList;
	return;
}

void Dates::Node::speciality(char* speciality, char* month, WEEK week)
{
	ofstream Registry;
	char *buffer = NULL;
	Registry.open("RegistrosConsultas\\Registro por especialidad.txt", ios::trunc | ios::out);
	if (!Registry.is_open()) return;
	Date *tempNode = this->head;
	Date *pendingList=new Date[Date::datesQuantity];
	for (int i = 0; i < Date::datesQuantity; i++)
		pendingList[i].medic = NULL;

	int index = 0;
	while (tempNode != NULL)
	{
		if (tempNode->status)
		{

			swapNode((pendingList + index), tempNode);
			index++;
		}
		tempNode = tempNode->next;
	}

	index = 0;
	while (index < Date::datesQuantity&&pendingList[index].medic != NULL)
	{
		if (!strcmp(pendingList[index].month, month) && pendingList[index].week == week)
		{
			buffer = pendingList[index].getSpeciality();
			if (!strcmp(buffer, speciality))
			{
				Registry << "-----------------------------------------------------------------------" << endl;
				Registry << "Nombre del medico: " << pendingList[index].medic->catName() << "\t Cedula: " << pendingList[index].medicCedule << "\nEspecialidad: " << pendingList[index].getSpeciality() << endl;
				Registry << "Fecha de cita: " << pendingList[index].date << "\tHora: " << pendingList[index].hour << "\tCuarto: " << pendingList[index].getRoom() << "\tRegistro: " << pendingList[index].dateCode << endl;
				Registry << "Nombre del paciente: " << pendingList[index].patientName << endl;
			}
		}
		index++;
	}

	MessageBox(NULL, "El registro se guardo correctamente", "Aviso", MB_ICONINFORMATION);
	Registry.close();
	delete[] pendingList;
}

Dates::Date*::Dates::Node::searchBySpeciality(char* speciality)
{
	Date *tempNode = this->head;

	while (tempNode != NULL)
	{
		if (!strcmp(tempNode->getSpeciality(), speciality))
			return tempNode;
		tempNode = tempNode->next;
	}

	return NULL;
}

void Dates::Node::searchByPatientToRemove(char *_patientName)
{
	Date *tempNode = this->head;

	while (tempNode != NULL)
	{
		if (!strcmp(tempNode->patientName, _patientName))
		{			
			removeNode(tempNode);
			tempNode = this->head;
			continue;
		}
		tempNode = tempNode->next;
	}

	return;
}

void Dates::Node::searchByMedicToRemove(char *_medicName)
{
	Date *tempNode = this->head;

	while (tempNode != NULL)
	{
		if (!strcmp(tempNode->medic->catName(), _medicName))
		{
			removeNode(tempNode);
			tempNode = this->head;
			continue;
		}
		tempNode = tempNode->next;
	}

	return;
}

