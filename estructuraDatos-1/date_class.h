#pragma once
#include "Patient_class.h"
#include "medics_tree"
using namespace std;

class dateList;

class Date
{

friend dateList;

public:
	Patient *patient;
	medics *doctor;

	char hour[8];
	char month[8];
	daysOfWeek day;

	char room[3];

	char doctorName[50];
	char patientName[50];
	char phone[12];
};

class dateList
{
	Date *head;
	Date *tail;

public:
	
	void addNode(Date* node);
	Date* searchNode();
	void removeNode(Date* node);
};