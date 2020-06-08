#pragma once
#include "medics_tree.h"
#include "Patient_class.h"
#include "asignID.h"
#include <ctime>
#include<sstream>
#include <cstdlib>

#define _REAL_TIME 999
#define _LOW_LIMIT 100
#define _HIGH_LIMIT 1000;
#define _RAND_RANGE(_LOW,_HIGH) (_LOW+1-_HIGH)

namespace Dates {

	enum WEEK {
		First = 1,
		Second,
		Third,
		Fourth
	};

	class Node;

	class Date {

		medics* medic;
		Patient* patient;
		char day[10];
		char month[10];
		char year[4];
		char mm[2];
		char dd[2];
		char date[12];

		char cancel[20];

		char hour[8]="";
		WEEK week;

		daysOfWeek dayId;

		char patientName[50];
		int medicCedule;

		bool status;

		int dateCode;
		static int datesQuantity;

		Date* next;
		Date* prev;

		friend class Node;
	public:
		Date();
		Date*getNext() { return next; }
		Date*getPrev() { return prev; }
		char* getDate() { return date; }
		int getMedicCed() { return medic->getCed(); }
		medics* getMedic() { return this->medic; }
		char* getHour() { return hour; }
		bool setDate(char*,Date*);
		void setDoctor(tree*,int);
		void setPatient(nodePatient*,char*);
		void setHour(char* _hour) { strcpy(this->hour, _hour); }
		int getCode() { return this->dateCode; }
		void setStatus(bool _status) { this->status = _status; }
		char* getRoom() { char *buffer = new char[5]; this->medic->getRoom(buffer); return buffer; }
		char* getDoctorName() { return this->medic->catName(); }
		char* getSpeciality() { char* buffer = new char[15]; this->medic->getSpecialityName(buffer); return buffer; }
		char* getPatientName() { return this->patientName; }
		char* getPatientPhone() { return this->patient->getPhone(); }
		bool getStatus() { return this->status; }
		void setCancel(char* _cancel) { strcpy(this->cancel, _cancel); }
		char* getCancel() { return this->cancel; }
	};


	class Node {

		Date* head;
		Date* tail;

		Date* arrayList();
		void heapSort(Date*,int);
		void heapify(Date*, int, int);
		void swapNode(Date*, Date*);
		int binarySearch(Date*, int, int);
		friend class Date;

	public:


		Node(nodePatient* patient, tree* medic);
		void addNode(Dates::Date*);
		Date* getHead() { return head; }
		Date* searchDate(int);
		bool removeNode(Date*);
		Date* searchBySpeciality(char*);
		void displayDates(HWND *hControl, Dates::Node*);
		void displayByMonth(HWND*, int, char*);
		void searchByDoctorAndMonth(int,char*,HWND,HWND,HWND);
		void weekly(char*, WEEK);
		void speciality(char*, char*, WEEK);
		void searchByPatientToRemove(char*);
		void searchByMedicToRemove(char *_medicName);
		Node() {};
		~Node();

	};

	void inicializateDisplayBy(HWND*);
	void inicializateDisplayEspecialtys(HWND*,SPECIALITY::Node*);
	void displayDoctors(HWND ,medics*,int []);
	void catRow(medics*,HWND,int []);
	bool validateDate(char*, char*, char*, char*, char*,char*);
	void format(char*,char*, char*, char*, char*,char*);
	char* monthValue(char*);
	void displayTime( bool, char*[]);
	void schedule(HWND*, tree*,Dates::Node*,int,char*);
	char *getIdDate(char*);
	void setMonths(HWND,HWND); 
	
}