#pragma once
#include <string>
using namespace std;

namespace SPECIALITY
{
	class Node;

	enum Modify { _Specialitys, _Description };

	class tempSpeciality
	{
	public:
		char temp_speciality[80];
		char temp_description[80];
	};

	class Specialitys :public tempSpeciality
	{
		char _speciality[80];
		char _description[80];
		static int _id;
		int _specialityId;
		Specialitys *next;
		Specialitys *prev;

	public:

		Specialitys();
		Specialitys*getNext() { return next; }
		char* getSpeciality() { return _speciality; }
		char* getDescription() { return _description; }
		int getSpecialityId() { return _specialityId; }
		void setSpeciality(char* specialityName) { strcpy(this->_speciality, specialityName); }
		void setDescription(char* specialityDescription) { strcpy(this->_description, specialityDescription); }

		friend class Node;

	};

	class Node
	{
		Specialitys *head;
		Specialitys *tail;
		char*path = NULL;

		bool notNumber(char[]);

	public:
		Node() :head(NULL) {}
		~Node();
		Specialitys* getHead() { return head; }
		void loadFile();
		void addNode(Specialitys*);
		void removeNode(char*);
		bool modifyNode(Specialitys*, Modify, char*);
		bool validateData(SPECIALITY::Specialitys*);
		Specialitys* SpecilitysSearch(char*);

	};
}
