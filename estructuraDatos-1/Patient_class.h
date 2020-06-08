
//#include <string>
#pragma once
#include "medics_tree.h"
#include "heapSort.h"
using namespace std;
enum Gender {_male, _female};
enum Reference { _internet, _follet, _ad, _recomendation };

char* sToA(string str);


//class Person
//{
//	string name;
//	string firstLastName;
//	string secondLastName;
//
//public:
//	string getName() { return name; }
//	string getFirstLastName() { return firstLastName; }
//	string getaSecondLastName() { return secondLastName; }
//
//	void setPerson(string _completeName) {
//
//		int type = 0;
//		int strLength = 0;
//		int endStr = 0;
//		int length = _completeName.length();
//		for (int i = 0; i <= length; i++)
//		{
//
//			if (_completeName[i] == 32 && type == 0)
//			{
//				name = _completeName.substr(0, endStr);
//				strLength = name.length();
//				endStr = 0;
//				type = 1;
//			}
//			else if (_completeName[i] == 32 && type == 1)
//			{
//				firstLastName = _completeName.substr(strLength+1 , endStr-1 );
//				strLength = firstLastName.length();
//				endStr = 0;
//				type = 2;
//			}
//			else if (type == 2)
//			{
//				length -= name.length() + firstLastName.length() + 2;
//				secondLastName = _completeName.substr(i,length );
//			}
//			endStr++;
//		}
//
//	}
//};

class Patient
{
	char _name[20];
	char _mLastName[20];
	char _fLastName[20];
	Gender _gender;
	char _phone[12];
	Reference _reference;
	char _day[3];
	char _month[3];
	char _year[5];
	char _age[3];
	Patient *next;
	Patient *prev;

	bool isNotNumber(char[]);

	friend class nodePatient;
public:

	/*bool validateData();*/

	medics* medic;
	void getName(char* name) {strcpy(name, _name); }
	void getmLastName(char* lastName) { strcpy(lastName, _mLastName);}
	void getfLastName(char* lastName) { strcpy(lastName,_fLastName); }
	Gender getGender() { return _gender; }
	char* getPhone() { return this->_phone; }
	Reference getReference() { return _reference; }
	void getDay(char* day) { strcpy(day,_day); }
	void getMonth(char* month) { strcpy(month, _month); }
	void getyear(char *year) { strcpy(year,_year); }
	void getAge(char* age) { strcpy(age ,_age); }

	void setName(char* _name) { if (isNotNumber(_name)) strcpy(this->_name, _name); else return; }
	void setmLastName(char *lastName) { if (isNotNumber(lastName)) strcpy(this->_mLastName,lastName); else return; }
	void setflastName(char *lastName) { if (isNotNumber(lastName)) strcpy(this->_fLastName,lastName); else return; }
	void setGender(Gender gender) { this->_gender = gender; }
	void setPhone(char* phone) { if (strlen(phone) >= 8 || strlen(phone) <= 12) strcpy(this->_phone, phone); else return; }
	void setReference(Reference reference) { this->_reference = reference; }
	void setDay(char *day) { strcpy(this->_day,day); }
	void setMonth(char *month) { strcpy(this->_month,month); }
	void setYear(char *year) { strcpy(this->_year,year); }
	void setAge(char *age) { strcpy(this->_age,age); }
	void setDoctor(char* doctorName,tree* node) {
		
		if (node->getRoot())
		{
			this->medic = new medics;
			medics*medico = node->treeArray();
			int index = 0;
			while (index <= medics::quantity)
			{
				if (!strcmp((medico + index)->catName(), doctorName))
				{
					this->medic->setCedule((medico + index)->getCed());
					(medico + index)->getName(doctorName, 1);
					this->medic->setName(doctorName);
					(medico + index)->get_mLastName(doctorName);
					this->medic->set_mLastName(doctorName);
					(medico + index)->get_fLastName(doctorName);
					this->medic->set_fLastName(doctorName);

					break;
				}

				index++;
			}

			delete[] medico;
		}

	}

	char* catName();
	static int quantity;

	Patient* getNext() { return next; }
	Patient*getPrev() { return prev; }
};

class nodePatient
{
	Patient *head = NULL;
	Patient *tail = NULL;

	void transferInfo(Patient*, Patient*);
	void heapify(Patient*, int, int);
	bool print(Patient* arr);

public:
	nodePatient();
	Patient* getHead() { return this->head; }
	void addNode(Patient*);
	Patient* patientSearchByName(string);
	Patient* searchByPhone(string);
	void removeNode(Patient*);
	void modifyBirthDay(Patient*, char*, char*, char*);
	void modifyName(Patient*, char*, char*, char*);
	void modifyGender(Patient*,Gender);
	void modifyAge(Patient*, char*);
	void displayList( HWND*,int);
	Patient* listToArray();
	void heapSort(Patient* arr, int n);
	bool validateData(Patient*);
	//void displayByDoctor(HWND*, string);
	~nodePatient();
};

