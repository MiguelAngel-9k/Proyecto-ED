
#pragma once
#include"Specialitys_class.h"
#include <fstream>
#include<iostream>
#include<Windows.h>
#include<commdlg.h>

using namespace std;
class tree;

enum daysOfWeek { Free ,Friday, Thursday, Wendesday, Tuesday, Monday };
enum VALIDATION {minNumber = 48, maxNumber = 57, minWord = 65, maxWord = 90, cedSize = 9};
enum NUMBERS {phone, CED};

//class tempMedics
//{
//public:
//	char temp_name[20];
//	char temp_mLastName[20];
//	char temp_fLastName[20];
//	int temp_cedula;
//	int temp_room;
//	char temp_startAttention[6];
//	char temp_endAttention[6];
//	char temp_picture[_MAX_PATH];
//	char temp_phone[13];
//	daysOfWeek *temp_day;
//};

class medics//:public tempMedics
{
	char _name[20];
	char _mLastName[20];
	char _fLastName[20];
	int _cedula;
	char _room[2];
	char _startAttention[7];
	char _endAttention[6];
	char _picture[_MAX_PATH]="";
	char _phone[13];
	SPECIALITY::Specialitys* speciality=NULL;
	char specialityName[20];
	daysOfWeek *_day;
	medics*left;
	medics*right;
	medics*_father;

	char* cedula();	
	bool valNames(char[]);
	void deleteDays(HWND*,const char*);

public:
	friend class tree;
	static int quantity;
	int getCed() { return this->_cedula; }
	medics* getLeft() { return left; }
	medics* getRight() { return right; }
	void getName(char *name,bool mode) { mode ? strcpy(name, this->_name) : strcpy(name, catName()); }
	void get_mLastName(char *name) { strcpy(name, this->_mLastName); }
	void get_fLastName(char *name) { strcpy(name, this->_fLastName); }
	void getPhone(char* phone) { strcpy(phone, this->_phone); }
	void getPhoto(char* photo) { strcpy(photo, this->_picture); }
	void getRoom(char* room) { strcpy(room, this->_room); }
	void getStartAttention(char* buffer) { strcpy(buffer,this->_startAttention); }
	daysOfWeek* getDaysOfWeek() { return this->_day; }
	void setName(char* name) { if (valNames(_name)) strcpy(this->_name, name); else return; }
	void set_fLastName(char *lastName) { if(valNames(lastName)) strcpy(this->_fLastName, lastName); else return; }
	void set_mLastName(char *lastName) { if(valNames(lastName)) strcpy(this->_mLastName, lastName); else return; }
	void setCedule(int cedula) { char *buffer = new char[13]; _itoa(cedula, buffer, 10); if (strlen(buffer) == cedSize) this->_cedula = cedula; else return; }
	void setRoom(char* room) { strcpy(this->_room ,room);  }
	void setStartAttention(char* startAttention) { strcpy(this->_startAttention, startAttention); }
	void setEndAttention(char* endAttention) { strcpy(this -> _endAttention, endAttention); }
	void setPicture(char* photoPath) {
		photoPath != NULL ? strcpy(this->_picture, photoPath) : strcpy(this->_picture, "");
	}
	char* catName();
	void setPhone(char *phone) { if (strlen(phone) >= 8 && strlen(phone) <= 12) strcpy(this->_phone, phone); else return; }
	void setDaysOfWeek(daysOfWeek *_daysOfweek) { this->_day = _daysOfweek; }
	void setSpeciality(SPECIALITY::Specialitys* _speciality) { this->speciality = _speciality; }
	void setSpecialityName(char* name, SPECIALITY::Node *head) {
		if (!head)
		{
			strcpy(this->specialityName, name);
			return;
		}

		this->speciality = head->SpecilitysSearch(name);
		strcpy(this->specialityName, name);
		return;
	}
	char* loadPhoto(HWND* , HBITMAP*, HWND);
	daysOfWeek* editDays(char*, char*, medics*, daysOfWeek*);
	void getSpecialityName(char* speciality) {	strcpy(speciality, this->specialityName);}
	//void getCedula(char*cedula) { strcpy(cedula,this->_cedula); }
};

class tree
{
	medics*root=NULL;
	char *path = NULL;
	void replaceNode(medics*, medics*);
	medics*_min(medics*);
	medics* _max(medics*);
	void destroyNode(medics*);
	medics* createNode(medics*, medics*);
	bool add(medics**, medics*, medics*, SPECIALITY::Node*);
	void write(ofstream*,fstream*,medics*,medics*);
	void addNodeFromFile(medics*, medics*,medics*);
	void DeleteTree();
	void insertNodeFormFile(medics*,medics*, SPECIALITY::Node*);
	void showMedicsByName(HWND*, medics*,int);
	void showMedicsByCedula(HWND*, medics*,int);
	void showMedicsBySpeciality(HWND*, medics*,char*);
	void tToA(medics*,medics*,int*);
	void transferNode(medics*, medics*);
	
	

public:
	medics* getRoot() { 
		if (root != NULL)return root;
		else return NULL;
	}
	void showMedics(HWND*, int,int,char*);
	bool insertNode(medics**, medics*, SPECIALITY::Node*);
	void showTree(medics*);
	medics* searchNode(medics**, int);
	medics* searchByName(medics**, char*);
	void modfifyCedule(medics*, int);
	void modifyName(medics*, char*, char*, char*);
	void removeNode(medics*, int);
	bool valTime(medics*, medics*);
	medics* treeArray();
	tree(SPECIALITY::Node**);
	void displayFoto(HWND, HBITMAP,medics* ,int, int); 
	bool veryfySpeciality(medics*, char*);
	void deleteNode(medics*);
	bool validateData(medics*);
	~tree();
	//void showPicture(HWND *);
};


void swapNodes(medics*_destination, medics*source);
void quickSort(medics* arr, int left, int right);
void sortMedicReport(medics* arr);
void spaces(int,int, ofstream*);

