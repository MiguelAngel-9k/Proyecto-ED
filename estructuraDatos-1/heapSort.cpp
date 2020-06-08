#include"heapSort.h"

//void MAX_SORT::heapify(Patient* arr, int n, int i)
//{
//	int largo = i;
//	int izq = 2 * i + 1;
//	int der = 2 * i + 2;
//	char *buffer1 = new char[20];
//	(arr + izq)->getfLastName(buffer1);
//	char *buffer2 = new char[20];
//	(arr + der)->getfLastName(buffer2);
//
//
//	if (izq < n && strcmp(buffer1,buffer2)>NULL)
//		largo = izq;
//
//	if (der < n && strcmp(buffer2,buffer1)>NULL)
//		largo = der;
//
//	delete[] buffer1; delete[] buffer2;
//
//	if (largo != i)
//	{
//		/*Patient* temporal = new Patient;
//		char *buffer = new char[MAX_PATH];
//
//		(arr + i)->getName(buffer);
//		temporal->setName(buffer);
//		(arr + i)->getfLastName(buffer);
//		temporal->setName(buffer);
//		(arr + i)->getmLastName(buffer);
//		temporal->setmLastName(buffer);
//		(arr + i)->getAge(buffer);
//		temporal->setAge(buffer);
//
//		(arr + largo)->getName(buffer);
//		(arr + i)->setName(buffer);
//		(arr + largo)->getfLastName(buffer);
//		(arr + i)->setflastName(buffer);
//		(arr + largo)->getmLastName(buffer);
//		(arr + i)->setmLastName(buffer);
//		(arr + largo)->getAge(buffer);
//		
//		temporal->getName(buffer);
//		(arr + largo)->setName(buffer);
//		temporal->getfLastName(buffer);
//		(arr + largo)->setflastName(buffer);
//		temporal->getmLastName(buffer);
//		(arr + largo)->setmLastName(buffer);
//		temporal->getAge(buffer);
//		(arr + largo)->setAge(buffer);
//
//		delete temporal;
//		delete[] buffer;*/
//		swap(arr, i, largo);
//
//		heapify(arr, n, largo);
//		//swap((arreglo+i),(arreglo+largo));
//	}
//}
//
//void MAX_SORT::heapSort(Patient* arr, int n)
//{
//	for (int i = n / 2 - 1; i >= 0; i--)
//		heapify(arr, n, i);
//
//	for (int i = n - 1; i >= 0; i--)
//	{
//		swap(arr, i, 0);
//		/*Patient* temporal = new Patient;
//		char *buffer = NULL;
//
//		(arr+i)
//		buffer = sToA((arr + i)->getName());
//		temporal->setName(buffer);
//		delete buffer;
//		buffer = sToA((arr + i)->getmLastName());
//		temporal->setmLastName(buffer);
//		delete buffer;
//		buffer = sToA((arr + i)->getfLastName());
//		temporal->setflastName(buffer);
//		delete buffer;
//		buffer = sToA((arr + i)->getAge());
//		temporal->setAge(buffer);
//		delete buffer;
//
//		buffer = sToA((arr)->getName());
//		(arr + i)->setName(buffer);
//		delete buffer;
//		buffer = sToA((arr)->getmLastName());
//		buffer = sToA((arr)->getAge());
//		(arr + i)->setAge(buffer);
//		delete buffer;
//
//		buffer = sToA((temporal->getName()));
//		(arr)->setName(buffer);
//		delete buffer;
//		buffer = sToA(temporal->getAge());
//		(arr)->setAge(buffer);
//		delete buffer;
//
//		delete temporal;*/
//
//		heapify(arr, i, 0);
//
//	}
//}
//
//bool MAX_SORT::print(Patient* arr)
//{
//	ofstream report;
//	char *buffer = NULL;
//	char *age = new char[3];
//	report.open("Reporte pacientes.txt", ios::out|ios::trunc);
//
//	if (!report.is_open())
//		return false;
//
//	for (int i = 0; i <= Patient::quantity; i++)
//	{
//		buffer=(arr + i)->catName();
//		(arr + i)->getAge(age);
//		report <<buffer << "\t" << age << endl;
//		delete buffer;
//	}
//
//	report.close();
//	return true;
//}
//
//void MAX_SORT::swap(Patient* arr, int posOne, int posTwo)
//{
//	Patient* temporal = new Patient;
//	char *buffer = new char[MAX_PATH];
//
//	(arr + posOne)->getName(buffer);
//	temporal->setName(buffer);
//	(arr + posOne)->getfLastName(buffer);
//	temporal->setflastName(buffer);
//	(arr + posOne)->getmLastName(buffer);
//	temporal->setmLastName(buffer);
//	(arr + posOne)->getAge(buffer);
//	temporal->setAge(buffer);
//
//	(arr + posTwo)->getName(buffer);
//	(arr + posOne)->setName(buffer);
//	(arr + posTwo)->getfLastName(buffer);
//	(arr + posOne)->setflastName(buffer);
//	(arr + posTwo)->getmLastName(buffer);
//	(arr + posOne)->setmLastName(buffer);
//	(arr + posTwo)->getAge(buffer);
//	(arr + posOne)->setAge(buffer);
//
//	temporal->getName(buffer);
//	(arr + posTwo)->setName(buffer);
//	temporal->getfLastName(buffer);
//	(arr + posTwo)->setflastName(buffer);
//	temporal->getmLastName(buffer);
//	(arr + posTwo)->setmLastName(buffer);
//	temporal->getAge(buffer);
//	(arr + posTwo)->setAge(buffer);
//
//	delete temporal;
//	delete[] buffer;
//}
