#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include "DatabaseImplementation.h"
using namespace std;
int main()
{
	vector<Category> CategoryVector;
	DatabaseInterface* DBInterface = new DatabaseImplementation();
	CategoryVector = DBInterface->get_category();
	for (unsigned int i = 0; i < CategoryVector.size(); i++)
	{
		cout << CategoryVector[i].get_id() << endl;
		cout << CategoryVector[i].get_name() << endl;
		cout << CategoryVector[i].get_is_active() << endl;
	}
	cin.get();
	cin.ignore(1000, '\n');
	return 0;
}

