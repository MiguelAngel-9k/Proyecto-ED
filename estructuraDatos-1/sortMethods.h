#pragma once
#include"medics_tree.h"
#include<fstream>


const int pivotDiv = 2;

void swapNodes(medics*_destination, medics*source);

void quickSort(medics* arr, int left, int right)
{
	int i = left, j = right;
	int p = arr[(left + right) / pivotDiv].getCed();

	while (i <= j)
	{
		while (arr[i].getCed() < p) i++;
		while (arr[j].getCed() > p) j--;
		if (i <= j)
		{
			swapNodes((arr+i), (arr+j));
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
	medics*temp = NULL;
	char *tempString = new char[MAX_PATH];
	//PASAR VALORES A UN NODO TEMPORAL
	_destination->getName(tempString, 1);
	temp->setName(tempString);
	_destination->get_fLastName(tempString);
	temp->set_fLastName(tempString);
	_destination->get_mLastName(tempString);
	temp->set_mLastName(tempString);
	temp->setCedule(_destination->getCed());

	//PASAR LOS VALORES DE LA FUENTE AL DESTINO
	_source->getName(tempString, 1);
	_destination->setName(tempString);
	_source->get_fLastName(tempString);
	_destination->set_fLastName(tempString);
	_source->get_mLastName(tempString);
	_destination->set_mLastName(tempString);
	_destination->setCedule(_source->getCed());

	//PASAR LOS VALORES DE MI TEMPORAL A MI NODO FUENTE
	temp->getName(tempString, 1);
	_source->setName(tempString);
	temp->get_fLastName(tempString);
	_source->set_fLastName(tempString);
	temp->get_mLastName(tempString);
	_source->set_mLastName(tempString);
	_source->setCedule(temp->getCed());

	return;
}


void sortMedicReport(medics* arr)
{
	ofstream report;
	int index = 0;
	report.open("Reporte Medico.txt", ios::out | ios::trunc);
	if (report.is_open())
	{
		while (index <= sizeof(arr) / 4)
		report << (arr + index)->getCed() << "	" << (arr + index)->catName() << endl;
		index++;
	}
}

