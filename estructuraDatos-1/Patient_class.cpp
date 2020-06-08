#include "heapSort.h"


int Patient::quantity = 0;

nodePatient::nodePatient():head(NULL)
{
	Patient* tempNode = NULL;
	fstream file;
	file.open("Bin\\Pacientes.bin", ios::binary | ios::in | ios::ate);

	if (file.is_open())
	{
		int size = file.tellg();
		for (int index = 0; index < size / sizeof(Patient); index++)
		{
			if (!head)
			{
				head = new Patient;
				tempNode = new Patient;

				file.seekg(0);
				file.read(reinterpret_cast<char*>(tempNode), sizeof(Patient));
				transferInfo(head, tempNode);
				head->next = NULL;
				head->prev = NULL;

				delete reinterpret_cast<char*>(tempNode);
				continue;
			}

			tempNode = head;
			while (tempNode->next != NULL)
				tempNode = tempNode->next;

			tempNode->next = new Patient;
			Patient *auxNode = new Patient;
			file.seekg(index * sizeof(Patient));
			file.read(reinterpret_cast<char*>(auxNode), sizeof(Patient));
			tempNode->next->prev = tempNode;
			tempNode->next->next = NULL;
			tempNode = tempNode->next;
			transferInfo(tempNode, auxNode);
			
			delete reinterpret_cast<char*>(auxNode);
			Patient::quantity++;

		}
	}

	file.close();
}

nodePatient::~nodePatient()
{
	Patient*node = head;
	Patient*tempNode = NULL;
	fstream file;
	file.open("Bin\\Pacientes.bin", ios::binary | ios::trunc | ios::out);
	if (!file.is_open())
		return;

	while (node != NULL)
	{
		file.write(reinterpret_cast<char*>(node), sizeof(Patient));
		node = node->next;
	}

	file.close();
	node = head;
	while (node != tail)
	{
		tempNode = node;
		node = node->next;
		delete tempNode;
	}

}

void nodePatient::transferInfo(Patient* destination, Patient* source)
{
	strcpy(destination->_name,source->_name);
	strcpy(destination->_mLastName,source->_mLastName);
	strcpy(destination->_fLastName,source->_fLastName);
	destination->_gender = source->_gender;
	strcpy(destination->_phone, source->_phone);
	destination->_reference = source->_reference;
	destination->medic = source->medic;
	strcpy(destination->_day,source->_day);
	strcpy(destination->_month ,source->_month);
	strcpy(destination->_year ,source->_year);
	strcpy(destination->_age ,source->_age);

	return;
}

void nodePatient::addNode(Patient*node)
{
	if (head == NULL)
	{
		head = node;
		head->next = NULL;
		head->prev=NULL;
		Patient::quantity++;
		return;
	}

	Patient *tempNode=head;
	while (tempNode->next != NULL)
		tempNode = tempNode->next;

	tempNode->next = node;
	tempNode->next->prev = tempNode;
	tempNode = tempNode->next;
	tempNode->next = NULL;

	Patient::quantity++;
	return;
}

Patient* nodePatient::patientSearchByName(string name)
{
	Patient *tempNode = head;
	bool found = false;
	while(tempNode!=NULL)
	{ 
		if (tempNode->catName() == name)
		{
			found = true;
			break;
		}
		tempNode = tempNode->next;
	}

	if (found == true) return tempNode;
	else return NULL;
}

Patient* nodePatient::searchByPhone(string phone)
{
	Patient *tempNode = head;
	bool found = false;
	while (tempNode != NULL)
	{
		if (tempNode->_phone == phone)
		{
			found = true;
			break;
		}
		tempNode = tempNode->next;
	}

	if (found == true) return tempNode;
	else return NULL;
}

void nodePatient::removeNode(Patient* node)
{
	if (!head)
		return;

	if (node->next == NULL && node->prev == NULL)
	{
		head = NULL;
		Patient::quantity--;
		delete node;
		return;
	}

	if (node->next == NULL && node->prev != NULL)
	{
		node->prev->next = NULL;
		node->prev = NULL;
		Patient::quantity--;
		delete node;
		return;
	}

	if (node->next != NULL && node->prev != NULL)
	{
		node->next->prev = node->prev;
		node->prev->next = node->next;
		Patient::quantity--;
		delete node;
		return;
	}

	if (head==node)
	{
		head = node->next;
		node->next->prev = NULL;
		delete node;
		Patient::quantity--;
		return;
	}
}

void nodePatient::modifyBirthDay(Patient*node, char* day, char* month, char* year)
{
	strcpy(node->_day,day);
	strcpy(node->_month,month);
	strcpy(node->_year,year);
	return;
}

void nodePatient::modifyName(Patient*node, char* name, char* mLastName, char* fLastName)
{
	if (name != NULL) strcpy(node->_name,name);
	if (mLastName != NULL) strcpy(node->_mLastName,mLastName);
	if (fLastName != NULL) strcpy(node->_fLastName,fLastName);

	return;
}

void nodePatient::modifyGender(Patient*node, Gender gender)
{
	node->_gender = gender;
	return;
}

void nodePatient::modifyAge(Patient*node, char* age)
{
	strcpy(node->_age,age);
	return;
}

void nodePatient::displayList(HWND* control,int msg)
{
	Patient* tempNode = this->head;
	char *tmp = NULL;

	while (tempNode != NULL)
	{
		tmp=tempNode->catName();
		SendMessage(*control, msg, NULL, (LPARAM)tmp);
		tempNode = tempNode->next;
		delete tmp;
		tmp = NULL;
	}
}

char* Patient::catName()
{
	char *buffer = new char[MAX_PATH];
	strcpy(buffer, this->_name);
	strcat(buffer, " ");
	strcat(buffer, this->_fLastName);
	strcat(buffer, " ");
	strcat(buffer, this->_mLastName);

	return buffer;
}

//void nodePatient::displayByDoctor(HWND* control,string doctor)
//{
//	Patient* tempNode = this->head;
//	char *tmp = NULL;
//	while (tempNode != NULL)
//	{
//		if (doctor == tempNode->medic)
//		{
//			tmp = sToA(tempNode->medic);
//			SendMessage(*control, LB_INSERTSTRING, NULL, (LPARAM)tmp);
//			delete tmp;
//			tmp = NULL;
//		}
//	}
//}

char* sToA(string str)
{
	int lenght = str.length();
	char *tmp = new char[str.length()+1];
	strcpy(tmp, str.c_str());

	return tmp;
}

Patient*nodePatient::listToArray()
{
	Patient* tempNode = head;
	Patient* arr = new Patient[Patient::quantity+1];
	int index = 0;
	char *buffer = NULL;

	while (tempNode != NULL)
	{
		strcpy((arr + index)->_name,tempNode->_name);
		strcpy((arr + index)->_fLastName, tempNode->_fLastName);
		strcpy((arr + index)->_mLastName ,tempNode->_mLastName);
		strcpy((arr + index)->_age,tempNode->_age);
		tempNode = tempNode->next;
		index++;
	}

	heapSort(arr, Patient::quantity+1);
	print(arr);

	return arr;
}

void nodePatient::heapSort(Patient* arreglo, int n)
{
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arreglo, n, i);

	for (int i = n - 1; i >= 0; i--)
	{
		Patient* temporal = new Patient;

		transferInfo(temporal, (arreglo + i));
		transferInfo((arreglo+i), (arreglo));
		transferInfo((arreglo), temporal);
		/*strcpy(temporal->_name, (arreglo + i)->_name);
		strcpy(temporal->_fLastName, (arreglo + i)->_fLastName);
		strcpy(temporal->_mLastName, (arreglo + i)->_mLastName);
		strcpy(temporal->_age, (arreglo + i)->_age);

		strcpy((arreglo + i)->_name, (arreglo)->_name);
		strcpy((arreglo + i)->_fLastName, (arreglo)->_fLastName);
		strcpy((arreglo + i)->_mLastName, (arreglo)->_mLastName);
		strcpy((arreglo + i)->_age, (arreglo)->_age);

		strcpy((arreglo)->_name, temporal->_name);
		strcpy((arreglo)->_fLastName, temporal->_fLastName);
		strcpy((arreglo)->_mLastName, temporal->_mLastName);
		strcpy((arreglo)->_age, temporal->_age);*/

		delete temporal;

		heapify(arreglo, i, 0);

	}
}

void nodePatient::heapify(Patient* arreglo, int n, int i)
{
	int largo = i;
	int izq = 2 * i + 1;
	int der = 2 * i + 2;

	if (izq < n && strcmp((arreglo+izq)->_fLastName,(arreglo+largo)->_fLastName)>NULL)
		largo = izq;

	if (der < n && strcmp((arreglo + der)->_fLastName, (arreglo + largo)->_fLastName)>NULL)
		largo = der;

	if (largo != i)
	{
		Patient* temporal = new Patient;

		transferInfo(temporal, (arreglo + i));
		transferInfo((arreglo + i), (arreglo + largo));
		transferInfo((arreglo + largo), temporal);

/*		strcpy(temporal->_name, (arreglo + i)->_name);
		strcpy(temporal->_fLastName, (arreglo + i)->_fLastName);
		strcpy(temporal->_mLastName, (arreglo + i)->_mLastName);
		strcpy(temporal->_age, (arreglo + i)->_age)*/;

		/*strcpy((arreglo + i)->_name, (arreglo + largo)->_name);
		strcpy((arreglo + i)->_fLastName, (arreglo + largo)->_fLastName);
		strcpy((arreglo + i)->_mLastName, (arreglo + largo)->_mLastName);
		strcpy((arreglo + i)->_age, (arreglo + largo)->_age);

		strcpy((arreglo + largo)->_name, temporal->_name);
		strcpy((arreglo + largo)->_fLastName, temporal->_fLastName);
		strcpy((arreglo + largo)->_mLastName, temporal->_mLastName);
		strcpy((arreglo + largo)->_age, temporal->_age);*/

		delete temporal;

		heapify(arreglo, n, largo);
		//swap((arreglo+i),(arreglo+largo));
	}
}

bool nodePatient::print(Patient* arr)
{
	ofstream report;
	char *buffer = NULL;
	char *age = new char[3];
	report.open("Reporte pacientes.txt", ios::out | ios::trunc);

	if (!report.is_open())
		return false;
	report << "Nombre\t\t\t\tEdad" << endl;
	for (int i = 0; i <= Patient::quantity; i++)
	{
		buffer = (arr + i)->catName();
		strcat(buffer, "\t\t\t");
		strcat(buffer, (arr + i)->_age);
		//strcat(buffer, "\t");
		//strcat(buffer, (arr + i)->_phone);
		//strcat(buffer, "\t");
		//strcat(buffer, (arr + i)->_day);
		//strcat(buffer, "/");
		//strcat(buffer, (arr + i)->_month);
		//strcat(buffer, "/");
		//strcat(buffer, (arr + i)->_year);
		report << buffer << endl;
		delete buffer;
	}

	report.close();
	return true;
}

//bool Patient::valNames(char name[]) {
//
//	bool succed = false;
//	for (short int i = 0; name[i] != NULL; i++)
//	{
//		if (name[i] < 65 || name[i] > 90)
//		{
//			if(name[i]!=32)
//			return false;
//
//		}
//	}
//
//	return true;
//}

bool Patient::isNotNumber(char _character[])
{
	int index = 0;
	while (strlen(_character) > index)
	{
		if (_character[index] >= 97 && _character[index] <= 122)
			_character[index] -= 32;

		if (_character[index] < 64 || _character[index] > 91)
		{
			if (_character[index] != 32)
				return false;
		}

		index++;
	}

	return true;
}


//bool Patient::validateData()
//{
//	if (!valNames(this->_name))
//		return false;
//	if (!valNames(this->_fLastName))
//		return false;
//	if (!valNames(this->_mLastName))
//		return false;
//	if (!numbers(this->_phone))
//		return false;
//
//	return true;
//}

bool nodePatient::validateData(Patient* node)
{
	if (!node->isNotNumber(node->_name)) return false;
	if (!node->isNotNumber(node->_fLastName)) return false;
	if (!node->isNotNumber(node->_mLastName)) return false;

	if (strlen(node->_phone) < 8 || strlen(node->_phone) > 12) return false;

	return true;
}
