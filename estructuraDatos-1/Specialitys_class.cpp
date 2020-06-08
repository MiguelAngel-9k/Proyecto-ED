#pragma once
#include <string>
#include <fstream>
//#include "Specialitys_class.h"
#include "date.h"


int SPECIALITY::Specialitys::_id=1000;

SPECIALITY::Specialitys::Specialitys()
{
	_id++;
	_specialityId = _id ;
}


void SPECIALITY::Node::addNode(Specialitys*node)
{
	if (head == NULL)
	{
		head = node;
		std::strcpy(head->_description,node->_description);
		std::strcpy(head->_speciality, node->_speciality);
		head->next = NULL;
		head->prev = NULL;
		return;
	}

	SPECIALITY::Specialitys *tempNode;
	tempNode = head;

	while (tempNode->next != NULL)
		tempNode = tempNode->next;

	tempNode->next = node;
	tempNode->next->prev = tempNode;
	tempNode->prev = tempNode;
	tempNode = tempNode->next;

	std::strcpy(tempNode->_description, node->_description);
	std::strcpy(tempNode->_speciality, node->_speciality);
	tempNode->next = NULL;

	return;


}

SPECIALITY::Specialitys* SPECIALITY::Node::SpecilitysSearch(char*look)
{
	Specialitys *tempNode = head;
	bool find = false;

	while (tempNode != NULL)
	{
		if (!strcmp(tempNode->_speciality ,look))
		{
			find = true;
			break;
		}
		tempNode=tempNode->next;
	}

	if (find == true) return tempNode;
	else return NULL;
	

}

void SPECIALITY::Node:: removeNode(char *speciality)
{
	Specialitys *node = SpecilitysSearch(speciality);
	if(node->next==NULL&&node->prev==NULL)
	{
		head=NULL;
		delete node;
		node =NULL;
		return;
	}
	if(node->next==NULL&&node->prev!=NULL)
	{
		node->prev->next=NULL;
		node->prev=NULL;
		delete node;
		return;
							
	}
	if(node->next!=NULL&&node->prev!=NULL)
	{
		node->next->prev = node->prev;
		node->prev->next = node->next;
		delete node;
		return;
		
	}
	if(node == head)
	{
		head = node->next;
		node->next->prev=NULL;
		delete node;
		return;
	}

}

bool SPECIALITY::Node::modifyNode(Specialitys*node, Modify modifyOption,char* modify)
{
	if(head==NULL) return false;
	
	switch(modifyOption)
	{
		case _Specialitys:
			if (!notNumber(modify)) return false;
				std::strcpy(node->_speciality, modify);
			return true;
		case _Description:
			strcpy(node->_description, modify);
			return true;
	}

}

void SPECIALITY::Node::loadFile()
{
	Specialitys *tempNode=NULL;
	fstream file;
	file.open("Bin\\Especialidades.bin", ios::binary | ios::in | ios::ate);
	if (file.is_open())
	{
		int size = file.tellg();
		for (int index = 0; index < size / sizeof(Specialitys); index++)
		{

			if (head == NULL)
			{
				head = new Specialitys;
				tempNode = new Specialitys;
				file.seekg(0);
				file.read(reinterpret_cast <char*>(tempNode), sizeof(Specialitys));
				strcpy(head->_speciality, tempNode->_speciality);
				strcpy(head->_description, tempNode->_description);
				head->_specialityId = tempNode->_specialityId;
				head->_id = tempNode->_id;
				head->next = NULL;
				head->prev = NULL;

				delete reinterpret_cast<char*>(tempNode);
				continue;
			}

			tempNode = head;
			while (tempNode->next != NULL)
				tempNode = tempNode->next;


			tempNode->next = new Specialitys;
			Specialitys *auxNode = new Specialitys;
			file.seekg(index * sizeof(Specialitys));
			file.read(reinterpret_cast<char*>(auxNode), sizeof(Specialitys));
			tempNode->next->prev = tempNode;
			tempNode->next->next = NULL;
			tempNode = tempNode->next;
			strcpy(tempNode->_speciality, auxNode->_speciality);
			strcpy(tempNode->_description, auxNode->_description);
			tempNode->_specialityId = auxNode->_specialityId;
			delete reinterpret_cast<char*>(auxNode);
		}
	}

	file.close();

}

SPECIALITY::Node::~Node()
{
	Specialitys *node = head;
	Specialitys *tempNode;
	fstream file;
	file.open("Bin\\Especialidades.bin", ios::binary | ios::trunc | ios::out);
	if (file.is_open())
	{
		while (node != NULL)
		{
			file.write(reinterpret_cast<char *>(node), sizeof(Specialitys));
			node = node->next;
		}
	}
	file.close();
	node = head;
	while (node != NULL)
	{
		tempNode = node;
		node = node->next;
		delete tempNode;
	}
}

bool SPECIALITY::Node::validateData(SPECIALITY::Specialitys* newNode)
{
	if (!newNode) return false;

	if (!notNumber(newNode->_speciality)) return false;
}

bool SPECIALITY::Node::notNumber(char _character[])
{
	int index = 0;
	while (strlen(_character)>index)
	{
		if (_character[index] >= 97 && _character[index] <= 122)
			_character[index] -= 32;

		if (_character[index] < 64 || _character[index] > 91)
		{
			if(_character[index]!=32)
				return false;
		}

		index++;
	}

	return true;

}



