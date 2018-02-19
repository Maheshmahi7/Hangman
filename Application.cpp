#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include "DatabaseImplementation.h"
using namespace std;
int main()
{
	vector<Category> CategoryVector;
	vector<GameDetails> GamedetailsVector;
	DatabaseInterface* DBInterface = new DatabaseImplementation();
	CategoryVector = DBInterface->get_category();
	for (unsigned int i = 0; i < CategoryVector.size(); i++)
	{
		cout << CategoryVector[i].get_id() << endl;
		cout << CategoryVector[i].get_name() << endl;
		cout << CategoryVector[i].get_is_active() << endl;
	}
	GamedetailsVector=DBInterface->get_playing_game_detail(7);
	for (unsigned int i = 0; i < CategoryVector.size(); i++)
	{
		cout << GamedetailsVector[i].get_id() << endl;
		cout << GamedetailsVector[i].get_username() << endl;
		cout << GamedetailsVector[i].get_socket_address() << endl;
		cout << GamedetailsVector[i].get_word_id().get_word() << endl;
	}
	cin.get();
	cin.ignore(1000, '\n');
	return 0;
}

