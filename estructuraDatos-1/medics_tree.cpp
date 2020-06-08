#include "medics_tree.h"


int medics::quantity = 0;

bool tree::insertNode(medics**node,medics*tempNode, SPECIALITY::Node* Specialist)
{
	medics *newNode = new medics;
	newNode=createNode(newNode,tempNode);
	if (root == NULL)
	{
		/**node = newNode*/medics::quantity++;
		root = newNode;
		root->speciality = Specialist->SpecilitysSearch(root->specialityName);
		return true;
	}
	else {
		return add(&root, newNode,NULL,Specialist);
	}
}

bool tree::add(medics**node, medics*newNode, medics*fatherNode, SPECIALITY::Node* Specialist)
{
	medics *tempNode = *node;
	if (!tempNode)
	{
		*node = newNode;
		tempNode = *node;
		tempNode->_father = fatherNode;
		tempNode->speciality = Specialist->SpecilitysSearch(tempNode->specialityName);
		medics::quantity++;
		return true;
	}

	if (newNode->_cedula > tempNode->_cedula)
		/*if (valTime(tempNode, newNode))*/
			add(&(tempNode->right), newNode, tempNode,Specialist);
		/*else return false;*/
	if (newNode->_cedula < tempNode->_cedula)
		/*if(valTime(tempNode, newNode))*/
			add(&(tempNode->left), newNode,tempNode,Specialist);
		/*else return false;*/
	if (newNode->_cedula == tempNode->_cedula)
		return false;

}

medics* tree::createNode(medics*node,medics*tempNode)
{
		node = tempNode;
		node->right = NULL;
		node->left = NULL;
		node->_father = NULL;
		return node;
}

void tree::showTree(medics*node)
{
	if (node == NULL)
		return;

	showTree(node->right);
	showTree(node->left);
}

medics* tree::searchNode(medics**node,int cedula)
{
	medics*tempNode=*node;
	if (tempNode != NULL)
	{
		if (cedula < tempNode->_cedula)
			searchNode(&tempNode->left, cedula);
		else
		{
			if (cedula > tempNode->_cedula)
				searchNode(&tempNode->right, cedula);
			else return tempNode;
		}
	}
	else return NULL;
}

medics* tree::searchByName(medics** node, char* name)
{
	medics* temp = *node;
	if (!temp)
		return NULL;

	if (!strcmp(temp->catName(), name))
		return temp;

	searchByName(&temp->left, name);
	searchByName(&temp->right, name);
}

void tree::modfifyCedule(medics* node, int cedule)
{
	if (node != NULL) {
		node->_cedula = cedule;
		return;
	}
	else return;
}

void tree::modifyName(medics*node, char *name, char* fLastName, char* mLastName)
{
	if (node != NULL)
	{
		if (!node->valNames(name)) return; else strcpy(node->_name, name);
		if (!node->valNames(fLastName)) return; else strcpy(node->_mLastName,mLastName);
		if (!node->valNames(mLastName)) return; else strcpy(node->_fLastName,fLastName);
		return;
	}
	else return;	
}

void tree::removeNode(medics*node,int data)
{
	if (node == NULL)
		return;
	else if (node->_cedula > data)
		removeNode(node->left, data);
	else if (node->_cedula < data)
		removeNode(node->right, data);
	else deleteNode(node);

}

medics*tree::_max(medics*node)
{
	if (!node)
		return NULL;
	if(node->right)
		return _max(node);
	else return node;
	
}

void tree::deleteNode(medics*node)
{
	if (node->left&&node->right)
	{		
		medics*tempNode = _min(node->right);
		/*tempNode->left->_father = node->right;
		tempNode->right->left = node->left;*/
		strcpy(node->_name, tempNode->_name);
		strcpy(node->_fLastName, tempNode->_fLastName);
		strcpy(node->_mLastName, tempNode->_mLastName);
		node->_cedula = tempNode->_cedula;
		strcpy(node->_room, tempNode->_room);
		strcpy(node->_startAttention, tempNode->_startAttention);
		strcpy(node->_picture, tempNode->_picture);
		strcpy(node->_phone, tempNode->_phone);
		node->speciality = tempNode->speciality;
		strcpy(node->specialityName, tempNode->specialityName);
		node->_day = tempNode->_day;
		
		deleteNode(tempNode);
	}
	else if (node->left)
	{
		if (node->_father)
		{
			if (node->_father->left == node)
			{
				node->_father->left = node->left;
				node->left->_father = node->_father;
				node->_father = NULL;
				node->left = NULL;
				node->right = NULL;
				medics::quantity--;
				delete node;
				return;
			}
			else if (node->_father->right == node)
			{
				node->_father->right = node->left;
				node->left->_father = node->_father;
				node->_father = NULL;
				node->left = NULL;
				node->right = NULL;
				medics::quantity--;
				delete node;
				return;
			}
		}
		this->root = node->left;
		node->left->_father = NULL;
		node->left = NULL;
		medics::quantity--;
		delete node;
	}
	else if (node->right)
	{
		if (node->_father)
		{
			if (node->_father->left == node)
			{
				node->_father->left = node->right;
				node->right->_father = node->_father;
				node->_father = NULL;
				node->left = NULL;
				node->right = NULL;
				medics::quantity--;
				delete node;
				return;
			}
			else if (node->_father->right == node)
			{
				node->_father->right = node->left;
				node->left->_father = node->_father;
				node->_father = NULL;
				node->left = NULL;
				node->right = NULL;
				medics::quantity--;
				delete node;
				return;
			}
		}

		this->root = node->right;
		node->right->_father = NULL;
		medics::quantity--;
		delete node;
	}
	else {
		if (node->_father) {
			medics *tempNode = node->_father;
			if (tempNode->right == node)
			{
				medics::quantity--;
				delete node;
				tempNode->right = NULL;
				return;
			}
			if (tempNode->left == node)
			{
				medics::quantity--;
				delete node;
				tempNode->left = NULL;
				return;
			}
		}
		else {
			medics::quantity--;
			delete node;
			this->root = NULL;
			return;
		}
	}
}

medics*tree::_min(medics* node)
{
	if (node == NULL)
		return NULL;
	if (node->left)
		return _min(node->left);
	else return node;
}

void tree::replaceNode(medics*node, medics*newNode)
{
	if (node->_father)
	{
		if (node->_cedula == node->_father->left->_cedula)
			node->_father->left = newNode;
		else if (node->_cedula == node->_father->right->_cedula)
			node->_father->right = newNode;
	}

	if (newNode)
	{
		newNode->_father = node->_father;
	}
}

void tree::destroyNode(medics*node)
{
	node->left = NULL;
	node->right = NULL;

	delete node;
}

char* medics::loadPhoto(HWND* pictureControl, HBITMAP *bitmapControl, HWND owner)
{
	OPENFILENAME file;
	ZeroMemory(&file, sizeof(file));
	file.hwndOwner = owner;
	file.lStructSize=sizeof(file);
	file.lpstrFile = this->_picture;
	//file.hwndOwner = owner;
	file.nMaxFile = MAX_PATH;
	file.lpstrDefExt = ".bmp";
	file.lpstrFilter = "ALL FILES (*.bmp)";
	file.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY |OFN_EXPLORER | OFN_NOCHANGEDIR;
	if (GetOpenFileName(&file) != TRUE)
		return this->_picture;

	MessageBox(NULL,this->_picture , "NOTIFICACION", MB_ICONINFORMATION);
	*bitmapControl = (HBITMAP)LoadImage(NULL, this->_picture, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
	SendMessage(*pictureControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)*bitmapControl);

	return this->_picture;
}

void tree::displayFoto(HWND pControl, HBITMAP _image, medics*node,int _x, int _y)
{
	if (node)
	{
		char _path[MAX_PATH] = "";
		node->getPhoto(_path);

		_image = (HBITMAP)LoadImage(NULL, _path, IMAGE_BITMAP, _x, _y, LR_LOADFROMFILE);
		SendMessage(pControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)_image);
		return;
	}

	_image = (HBITMAP)LoadImage(NULL, "", IMAGE_BITMAP, _x, _y, LR_LOADFROMFILE);
	SendMessage(pControl, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)_image);
}

bool tree::valTime(medics*tempNode,medics* node)
{
	if (!tempNode) return true;

		for (short int i = 0; i < 5; i++)
		{
			if (*(node->_day + i) != Free)
				if (*(node->_day + i) == *(tempNode->_day + i))
					if (!strcmp(node->_startAttention, tempNode->_startAttention))
						if (!strcmp(node->_room, tempNode->_room))
							return false;
		}

	if (valTime(tempNode->left, node))
	{
		if (!valTime(tempNode->right, node)) {
			return false;
		}
		else return true;
	} else return false;
}

tree::~tree()
{
	medics* tempNode = NULL;
	fstream myFile;
	ofstream daysTest;
	myFile.open("Bin\\Medicos.bin", ios::binary | ios::trunc | ios::out);
	daysTest.open("Bin\\dias.bin", ios::binary|ios::trunc | ios::out);
	if (daysTest.is_open()&&myFile.is_open()) write(&daysTest,&myFile, root, tempNode);

	myFile.close();
	daysTest.close();


}

void tree::write(ofstream *daysFile,fstream *myFile, medics*node, medics* tempNode)
{
	if (node==NULL)
		return;

	myFile->write(reinterpret_cast<char*>(node), sizeof(medics));

	for (int i = 0; i < 5; i++)
		daysFile->write(reinterpret_cast<char*>((node->_day + i)), sizeof(daysOfWeek));

	write(daysFile,myFile, node->left, tempNode);
	write(daysFile,myFile, node->right, tempNode);

	medics* tempFather = node->_father;
	delete node;

	return;
}

void tree::DeleteTree()
{
	medics *tempNode = _min(root);
}


tree::tree(SPECIALITY::Node** loadSpecialitys)
{
	medics* temp = NULL;
	daysOfWeek *tempDays;
	int	*size = new int[4];
	*(size + 2) = NULL;
	medics* node = NULL;
	fstream myFile;
	ifstream daysFile;
	myFile.open("Bin\\Medicos.bin", ios::binary | ios::ate | ios::in);
	daysFile.open("Bin\\dias.bin", ios::binary | ios::ate | ios::in);

	*loadSpecialitys = new SPECIALITY::Node;
	SPECIALITY::Node *tempList = *loadSpecialitys;
	tempList->loadFile();

	if (myFile.is_open()&&daysFile.is_open())
	{
		*size = myFile.tellg();
		myFile.seekg(0);
		daysFile.seekg(0);
		for (int i = 0; i < *size / sizeof(medics); i++)
		{
			tempDays = new daysOfWeek[5];
			medics *tempNode = new medics;
			myFile.read(reinterpret_cast<char*>(tempNode), sizeof(medics));
			for (int i = 0; i < 5; i++)
			{
				daysFile.read(reinterpret_cast<char*>(size+2), sizeof(int));
				switch (*(size+2))
				{
				case 0:
					*(tempDays + i) = Free;
					break;
				case 5:
					*(tempDays + i) = Monday;
					break;
				case 4:
					*(tempDays + i) = Tuesday;
					break;
				case 3:
					*(tempDays + i) = Wendesday;
					break;
				case 2:
					*(tempDays + i) = Thursday;
					break;
				case 1:
					*(tempDays + i) = Friday;
					break;
				}
			}
			tempNode->_day = tempDays;
			//tempNode->speciality=loadSpecialitys->SpecilitysSearch(tempNode->specialityName);
			insertNodeFormFile(node,tempNode,tempList);
			//addNodeFromFile(node, tempNode);
			tempNode = NULL;
			delete reinterpret_cast<char*>(tempNode);
			medics::quantity = i;
		}

		
	}

	myFile.close();
}

void tree::insertNodeFormFile( medics* node, medics*tempNode, SPECIALITY::Node *specialist)
{
	medics* newNode = new medics;
	 newNode = createNode(newNode, tempNode);
	if (root == NULL)
	{
		root = newNode;
		root->right = NULL;
		root->left = NULL;
		root->_father = NULL;
		root->speciality=specialist->SpecilitysSearch(root->specialityName);

		return;
	}

	add(&root,tempNode,NULL,specialist);
	return;
}

void tree::addNodeFromFile(medics*node, medics*tempNode, medics* father)
{
	if (node == NULL)
	{
		node = tempNode;
		node->_father = father;

		return;
	}

	if (node->_cedula > tempNode->_cedula)
		addNodeFromFile(node->left,tempNode,node);
	if (node->_cedula < tempNode->_cedula)
		addNodeFromFile(node->right, tempNode,node);

	return;
}

void tree::showMedicsByName(HWND* control,medics* node, int msg)
{
	if (node == NULL) return;


		char *completeName=node->catName();
		char temp[50]="";
		_itoa(node->_cedula, temp, 10);
		strcat(temp, "\t\t");
		strcat(temp, completeName);
		SendMessage(*control, msg, NULL, (LPARAM)temp);
		delete[] completeName;
		showMedicsByName(control, node->left,msg);
		showMedicsByName(control, node->right,msg);
}

void tree::showMedicsByCedula(HWND* control, medics*node,int msg)
{
	char *buffer = new char[MAX_PATH];

	if (node == NULL) return;
	_itoa(node->_cedula, buffer, 10);

	SendMessage(*control,msg,NULL,(LPARAM)buffer);
	delete[] buffer;

	showMedicsByCedula(control, node->left,msg);
	showMedicsByCedula(control, node->right,msg);


}

void tree::showMedicsBySpeciality(HWND* control, medics* node, char* speciality)
{
	if (node == NULL) return;



	if (!strcmp(node->specialityName, speciality)) {

		char *buffer = node->catName();
		char temp[50] = "";
		_itoa(node->_cedula, temp, 10);

		strcat(temp, "\t\t");
		strcat(temp, buffer);

		SendMessage(*control, LB_ADDSTRING, NULL, (LPARAM)temp);
		delete[] buffer;
	}

		showMedicsBySpeciality(control, node->left, speciality);
		showMedicsBySpeciality(control, node->right, speciality);

}

bool tree::veryfySpeciality(medics* node, char* speciality)
{
	if (node == NULL) return false;



	if (!strcmp(node->specialityName, speciality)) return true;

	veryfySpeciality(node->left, speciality);
	veryfySpeciality(node->right, speciality);

}

void tree::showMedics(HWND* control, int mode,int msg, char*speciality)
{
	if (!this->root)
	{
		MessageBox(NULL, "No hay medicos registrados", "No se pueden mostar", MB_ICONWARNING);
		return;
	}
	switch (mode)
	{
	case 0:
		showMedicsByCedula(control, root, msg);
		break;
	case 1:
		showMedicsBySpeciality(control, root, speciality);
		break;
	case 2:
		showMedicsByName(control, root,msg);
		break;
	default:
		break;
	}
}

char* medics::catName()
{
	char *buffer=new char [MAX_PATH];

	strcpy(buffer, this->_name);
	strcat(buffer, " ");
	strcat(buffer, this->_fLastName);
	strcat(buffer, " ");
	strcat(buffer, this->_mLastName);

	return buffer;
}

char* medics::cedula()
{
	char *buffer = new char [MAX_PATH];

	_itoa(this->_cedula, buffer, 10);

	return buffer;

}

daysOfWeek* medics::editDays(char* horario, char* room, medics* node, daysOfWeek*dias)
{
	if (node == NULL) return dias;

	if (!strcmp(node->_room, room) && !strcmp(node->_startAttention, horario))
	{
		for (int i = 0; i < 5; i++)
		{
			switch (*(node->_day + i))
			{
			case Monday:
				*(dias) = Monday;
				break;
			case Tuesday:
				*(dias + 1) = Tuesday;
				break;
			case Wendesday:
				*(dias + 2) = Wendesday;
			case Thursday:
				*(dias + 3) = Thursday;
				break;
			case Friday:
				*(dias + 4) = Friday;
				break;
			}
		}
	}

	editDays(horario, room, node->left,dias);
	editDays(horario, room, node->right, dias);

	return dias;
}

void medics::deleteDays(HWND* control, const char* day)
{
	char *tempDay = new char[10];

	for (int i = 0; i <= SendMessage(*control, LB_GETCOUNT, NULL, NULL); i++)
	{
		SendMessage(*control, LB_GETTEXT, i, (LPARAM)tempDay);

		if (!strcmp(tempDay, day))
			SendMessage(*control, LB_DELETESTRING, i, NULL);
	}

	delete[] tempDay;
}

medics* tree::treeArray()
{
	if (medics::quantity == 0) return NULL;
	medics* arr = new medics[medics::quantity + 1];
	int index=0;
	tToA(root, arr,&index);

	return arr;
}

void tree::tToA(medics* node, medics*arr, int* index)
{
	if (!node) return;

	transferNode(arr+(*index), node);
	(*index)++;
	tToA(node->left, arr, index);
	tToA(node->right, arr, index);

	return;

}

void tree::transferNode(medics* _destination, medics* _source)
{
	strcpy(_destination->_name, _source->_name);
	strcpy(_destination->_mLastName, _source->_mLastName);
	strcpy(_destination->_fLastName, _source->_fLastName);
	strcpy(_destination->_mLastName, _source->_mLastName);
	_destination->_cedula = _source->_cedula;
	strcpy(_destination->specialityName, _source->specialityName);
	strcpy(_destination->_phone, _source->_phone);
	strcpy(_destination->_startAttention, _source->_startAttention);
	strcpy(_destination->_room, _source->_room);
	/*strcpy(_destination->_room, _source->_room);
	strcpy(_destination->_startAttention, _source->_startAttention);
	strcpy(_destination->_picture, _source->_picture);
	strcpy(_destination->_phone, _source->_phone);
	_destination->speciality = _source->speciality;
	strcpy(_destination->specialityName, _source->specialityName);
	_destination->_day = _source->_day;*/

	return;
}
/*si es menor se va hacua la izq y si es mayor hacua la derecha*/

void quickSort(medics* arr, int left, int right)
{
	int i = left, j = right;
	int p = arr[(left + right) / 2].getCed();

	while (i <= j)
	{
		while (arr[i].getCed() < p) i++;
		while (arr[j].getCed() > p) j--;
		if (i <= j)
		{
			swapNodes((arr + i), (arr + j));
			//temp = arr[i];
			//arr[i] = arr[j];
			//arr[j] = temp;
			i++, j--;
		}
	}

	if (left < j)
		quickSort(arr, left, j);
	if (i < right)
		quickSort(arr, i, right);
}

void swapNodes(medics*_destination, medics*_source)
{
	medics*temp = new medics;
	char *tempString = new char[MAX_PATH];
	//PASAR VALORES A UN NODO TEMPORAL
	_destination->getName(tempString, 1);
	temp->setName(tempString);
	_destination->get_fLastName(tempString);
	temp->set_fLastName(tempString);
	_destination->get_mLastName(tempString);
	temp->set_mLastName(tempString);
	temp->setCedule(_destination->getCed());
	_destination->getSpecialityName(tempString);
	temp->setSpecialityName(tempString,NULL);
	_destination->getPhone(tempString);
	temp->setPhone(tempString);
	_destination->getStartAttention(tempString);
	temp->setStartAttention(tempString);
	_destination->getRoom(tempString);
	temp->setRoom(tempString);

	//PASAR LOS VALORES DE LA FUENTE AL DESTINO
	_source->getName(tempString, 1);
	_destination->setName(tempString);
	_source->get_fLastName(tempString);
	_destination->set_fLastName(tempString);
	_source->get_mLastName(tempString);
	_destination->set_mLastName(tempString);
	_destination->setCedule(_source->getCed());
	_source->getSpecialityName(tempString);
	_destination->setSpecialityName(tempString, NULL);
	_source->getPhone(tempString);
	_destination->setPhone(tempString);
	_source->getStartAttention(tempString);
	_destination->setStartAttention(tempString);
	_source->getRoom(tempString);
	_destination->setRoom(tempString);

	//PASAR LOS VALORES DE MI TEMPORAL A MI NODO FUENTE
	temp->getName(tempString, 1);
	_source->setName(tempString);
	temp->get_fLastName(tempString);
	_source->set_fLastName(tempString);
	temp->get_mLastName(tempString);
	_source->set_mLastName(tempString);
	_source->setCedule(temp->getCed());
	temp->getSpecialityName(tempString);
	_source->setSpecialityName(tempString, NULL);
	temp->getPhone(tempString);
	_source->setPhone(tempString);
	temp->getStartAttention(tempString);
	_source->setStartAttention(tempString);
	temp->getRoom(tempString);
	_source->setRoom(tempString);

	return;
}


void sortMedicReport(medics* arr)
{
	ofstream report;
	int index = 0;
	string temp;
	char *buffer = new char [16];
	report.open("Reporte Medico.txt", ios::out | ios::trunc);
	if (report.is_open())
	{
		while (index <= medics::quantity)
		{
			_itoa((arr + index)->getCed(), buffer, 10);
			temp = buffer;
			report << temp;/* << "	" << (arr + index)->catName() << endl*/;
			spaces(temp.length(), 29, &report);
			(arr + index)->getSpecialityName(buffer);
			temp = buffer;
			report << temp;
			spaces(temp.length(), 27, &report);
			temp = (arr + index)->catName();
			report << temp;
			spaces(temp.length(), 31, &report);
			(arr + index)->getPhone(buffer);
			temp = buffer;
			report << temp;
			spaces(temp.length(), 21, &report);
			(arr + index)->getStartAttention(buffer);
			temp = buffer;
			if (temp == "Mañana")
				report << "6:00AM a 1:00PM";
			else if (temp == "Tarde")
				report << "1:00PM a 7:00PM";
			spaces(15, 19, &report);
			(arr + index)->getRoom(buffer);
			temp = buffer;
			report <<temp<< endl;

			index++;
		}
		delete[] buffer;
	}
}

void spaces(int charSize,int numberOfTotalSpaces,ofstream* outPut)
{
	int counterOfSpaces = numberOfTotalSpaces-charSize;
	int counter = 0;
	while (counterOfSpaces >= counter)
	{
		*outPut << "-";
		counter++;
	}
}

bool medics::valNames(char _character[]) {

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

bool numbers(char number[], NUMBERS valNumber)
{
	bool succed = false;

	if (valNumber == phone)
	{
		if (strlen(number) >= 8 && strlen(number) <= 12)
			succed = true;
		else return false;
	}
	else if (valNumber == CED)
	{
		if (strlen(number) == cedSize)
			succed = true;
		else return false;

	}

	return true;
}

bool tree::validateData(medics* node)
{
	if (!node->valNames(node->_name))
		return false;
	if (!node->valNames(node->_fLastName))
		return false;
	if (!node->valNames(node->_mLastName))
		return false;

	char buffer[10] = "";
	_itoa(node->_cedula, buffer, 10);
	if (strlen(buffer)!=9) return false;

	if (strlen(node->_phone) < 8 || strlen(node->_phone) > 12) return false;

	return true;
}


