#include <iostream>
#include <string>
#include "Coder.h"
#include "Category.h"
#include "Words.h"
#include "Difficulty.h"
#include "DatabaseXmlParser.h"
#include "DatabaseInterface.h"
#include "DatabaseImplementation.h"

using namespace std;

void test_coder();
void test_category();
void test_difficulty();
void test_words();
void test_xml_parser_get_category_from_xml();
void test_xml_parser_get_diffiulty_from_xml();
void test_xml_parser_get_word_from_xml();
void test_get_category();
void test_get_difficulty();
void test_update_game_details();
void test_get_maximum_game_id();
void test_get_playing_game_details();
void test_insert_into_game_details();

void main()
{
	test_coder();
	test_category();
	test_difficulty();
	test_words();
	test_xml_parser_get_category_from_xml();
	test_xml_parser_get_diffiulty_from_xml();
	test_xml_parser_get_word_from_xml();
	test_get_category();
	test_get_difficulty();
	//test_insert_into_game_details();
	test_update_game_details();
	test_get_maximum_game_id();
	test_get_playing_game_details();

	cin.get();
	cin.ignore(1000, '\n');
}


void test_coder(){
	string Word = "mahesh123", Encode, Decode;
	string encodedWord = "ocjguj345";
	Coder code;
	Encode = code.encoder(Word);
	if (encodedWord.compare(Encode) == 0)
	{
		Decode = code.decoder(Encode);
		if (Word.compare(Decode) == 0)
		{
			cout << "Coder Test Passed" << endl;
		}
		else
		{
			cout << "Coder Test Failed" << endl;
		}
	}
	else
	{
		cout << "Coder Test Failed" << endl;
	}
}

void test_category(){
	int Id = 1, IsActive = 1;
	string Name = "asdfg";
	Category CategoryObject;
	CategoryObject.set_id(Id);
	CategoryObject.set_name(Name);
	CategoryObject.set_is_active(IsActive);
	if (Id == CategoryObject.get_id())
	{
		if (Name.compare(CategoryObject.get_name()) == 0)
		{
			if (IsActive == CategoryObject.get_is_active())
			{
				cout << "Category Test Passed" << endl;
			}
			else
			{
				cout << "Category Test Failed" << endl;
			}
		}
		else
		{
			cout << "Category Test Failed" << endl;
		}
	}
	else
	{
		cout << "Category Test Failed" << endl;
	}
}

void test_difficulty(){
	int Id = 1, IsActive = 1;
	string Name = "asdfg";
	Difficulty DifficultyObject;
	DifficultyObject.set_id(Id);
	DifficultyObject.set_name(Name);
	DifficultyObject.set_is_active(IsActive);
	if (Id == DifficultyObject.get_id())
	{
		if (Name.compare(DifficultyObject.get_name()) == 0)
		{
			if (IsActive == DifficultyObject.get_is_active())
			{
				cout << "Difficulty Test Passed" << endl;
			}
			else
			{
				cout << "Difficulty Test Failed" << endl;
			}
		}
		else
		{
			cout << "Difficulty Test Failed" << endl;
		}
	}
	else
	{
		cout << "Difficulty Test Failed" << endl;
	}
}

void test_words()
{
	int Id = 1, IsActive = 1, CategoryId = 1, DifficultyId = 1;
	string Word = "asdf";
	Words WordsObject;
	Category CategoryObject;
	Difficulty DifficultyObject;
	WordsObject.set_id(Id);
	CategoryObject.set_id(CategoryId);
	WordsObject.set_category_id(CategoryObject);
	DifficultyObject.set_id(DifficultyId);
	WordsObject.set_difficulty_id(DifficultyObject);
	WordsObject.set_word(Word);
	WordsObject.set_is_active(IsActive);
	if (Id == WordsObject.get_id())
	{
		if (CategoryId == WordsObject.get_category_id().get_id())
		{
			if (DifficultyId == WordsObject.get_difficulty_id().get_id())
			{
				if (Word.compare(WordsObject.get_word())==0)
				{
					if (IsActive == WordsObject.get_is_active())
					{
						cout << "Words Test Passed" << endl;
					}
					else
					{
						cout << "Words Test Failed" << endl;
					}
				}
				else
				{
					cout << "Words Test Failed" << endl;
				}
			}
			else
			{
				cout << "Words Test Failed" << endl;
			}
		}
		else
		{
			cout << "Words Test Failed" << endl;
		}
	}
	else
	{
		cout << "Words Test Failed" << endl;
	}
}

void test_xml_parser_get_category_from_xml()
{
	xml_document<> Document;
	int count = 0;
	string category[4] = { "Animals","Countries","Movies","Plants" };
	xml_node<> *Node;
	string Buffer="<Hangman><Categories><Category><Name>Animals</Name><IsActive>1</IsActive></Category><Category><Name>Countries</Name><IsActive>1</IsActive></Category><Category><Name>Movies</Name><IsActive>1</IsActive></Category><Category><Name>Plants</Name><IsActive>1</IsActive></Category></Categories></Hangman>";
	Document.parse<0>(&Buffer[0]);
	Node = Document.first_node();
	DatabaseXmlParser Xml;
	vector<Category> CategoryVector;
	CategoryVector = Xml.get_category_from_xml(Node);
	if (CategoryVector.size() == 4)
	{
		for (unsigned int i = 0; i < CategoryVector.size(); i++)
		{
			if (CategoryVector[i].get_name().compare(category[i])==0)
			{
				count++;
			}
		}
		if (count == CategoryVector.size())
		{
			cout << "Xml Category Test Passed" << endl;
		}
		else
		{
			cout << "Xml Category Test Failed" << endl;
		}
	}
	else
	{
		cout << "Xml Category Test Failed" << endl;
	}
}

void test_xml_parser_get_diffiulty_from_xml()
{
	xml_document<> Document;
	xml_node<> *Node;
	int count = 0;
	string difficulty[4] = { "Easy", "Medium", "Hard"};
	string Buffer = "<Hangman> <Difficulties><Difficulty><Name>Easy</Name><IsActive>1</IsActive></Difficulty><Difficulty><Name>Medium</Name><IsActive>1</IsActive></Difficulty><Difficulty><Name>Hard</Name><IsActive>1</IsActive></Difficulty></Difficulties></Hangman>";
	Document.parse<0>(&Buffer[0]);
	Node = Document.first_node();
	DatabaseXmlParser Xml;
	vector<Difficulty> DifficultyVector;
	DifficultyVector = Xml.get_difficulty_from_xml(Node);
	if (DifficultyVector.size() == 3)
	{
		for (unsigned int i = 0; i < DifficultyVector.size(); i++)
		{
			if (DifficultyVector[i].get_name().compare(difficulty[i]) == 0)
			{
				count++;
			}
		}
		if (count == DifficultyVector.size())
		{
			cout << "Xml Difficulty Test Passed" << endl;
		}
		else
		{
			cout << "Xml Difficulty Test Failed" << endl;
		}
	}
	else
	{
		cout << "Xml Difficulty Test Failed" << endl;
	}
}

void test_xml_parser_get_word_from_xml()
{
	xml_document<> Document;
	xml_node<> *Node;
	int count = 0;
	string word[6] = { "Lion", "Tiger", "Snake", "Zebra", "Yak", "Wolf" };
	string Buffer = "<Hangman><Words><Word><CategoryId>1</CategoryId><DifficultyId>1</DifficultyId><Name>Lion</Name><IsActive>1</IsActive></Word><Word><CategoryId>1</CategoryId><DifficultyId>1</DifficultyId><Name>Tiger</Name><IsActive>1</IsActive></Word><Word><CategoryId>1</CategoryId><DifficultyId>1</DifficultyId><Name>Snake</Name><IsActive>1</IsActive></Word><Word><CategoryId>1</CategoryId><DifficultyId>1</DifficultyId><Name>Zebra</Name><IsActive>1</IsActive></Word><Word><CategoryId>1</CategoryId><DifficultyId>1</DifficultyId><Name>Yak</Name><IsActive>1</IsActive></Word><Word><CategoryId>1</CategoryId><DifficultyId>1</DifficultyId><Name>Wolf</Name><IsActive>1</IsActive></Word></Words></Hangman>";
	Document.parse<0>(&Buffer[0]);
	Node = Document.first_node();
	DatabaseXmlParser Xml;
	vector<Words> WordsVector;
	WordsVector = Xml.get_words_from_xml(Node);
	if (WordsVector.size() == 6)
	{
		for (unsigned int i = 0; i < WordsVector.size(); i++)
		{
			if (WordsVector[i].get_word().compare(word[i]) == 0)
			{
				count++;
			}
		}
		if (count == WordsVector.size())
		{
			cout << "Xml Words Test Passed" << endl;
		}
		else
		{
			cout << "Xml Words Test Failed" << endl;
		}
	}
	else
	{
		cout << "Xml Words Test Failed" << endl;
	}
}

void test_get_category()
{
	DatabaseInterface* DBInterface = new DatabaseImplementation();
	int count = 0;
	string category[4] = { "Animals", "Countries", "Movies", "Plants" };
	vector<Category> CategoryVector;
	CategoryVector = DBInterface->get_category();
	if (CategoryVector.size() == 4)
	{
		for (unsigned int i = 0; i < CategoryVector.size(); i++)
		{
			if (CategoryVector[i].get_name().compare(category[i]) == 0)
			{
				count++;
			}
		}
		if (count == CategoryVector.size())
		{
			cout << "Category from DB Test Passed" << endl;
		}
		else
		{
			cout << "Category from DB Test Failed" << endl;
		}
	}
	else
	{
		cout << "Category from DB Test Failed" << endl;
	}
}

void test_get_difficulty()
{
	DatabaseInterface* DBInterface = new DatabaseImplementation();
	vector<Difficulty> DifficultyVector;
	int count = 0;
	string difficulty[4] = { "Easy", "Medium", "Hard" };
	DifficultyVector = DBInterface->get_difficulty();
	if (DifficultyVector.size() == 3)
	{
		for (unsigned int i = 0; i < DifficultyVector.size(); i++)
		{
			if (DifficultyVector[i].get_name().compare(difficulty[i]) == 0)
			{
				count++;
			}
		}
		if (count == DifficultyVector.size())
		{
			cout << "Difficulty from DB Test Passed" << endl;
		}
		else
		{
			cout << "Difficulty from DB Test Failed" << endl;
		}
	}
	else
	{
		cout << "Difficulty from DB Test Failed" << endl;
	}
}

void test_update_game_details()
{
	int GameId = 2;
	string Result = "WIN";
	DatabaseInterface* DBInterface = new DatabaseImplementation();
	string Status = DBInterface->update_game_result(GameId, (char*)Result.c_str());
	if (Status.compare("Updated Successfully"))
	{
			cout << "Test Update Game Details Passed" << endl;
	}
	else
	{
		cout << "Test Update Game Details Failed" << endl;
	}
}

void test_get_maximum_game_id()
{
	int GameId;
	DatabaseInterface* DBInterface = new DatabaseImplementation();
	GameId = DBInterface->get_maximum_game_id();
	if (GameId >= 0)
	{
		cout << "Test Get Maximum Game Id Passed" << endl;
	}
	else
	{
		cout << "Test Get Maximum Game Id Failed" << endl;
	}
}

void test_get_playing_game_details()
{
	int GameId = 3;
	vector<GameDetails> GameDetailsVector;
	DatabaseInterface* DBInterface = new DatabaseImplementation();
	GameDetailsVector = DBInterface->get_playing_game_detail();
	if (GameDetailsVector.size() >= 0)
	{
		cout << "Test Get Game Details Passed" << endl;
	}
	else
	{
		cout << "Test Get Game Details Failed" << endl;
	}
	GameDetailsVector = DBInterface->get_playing_game_detail(GameId);
	if (GameDetailsVector.size() >= 0)
	{
		cout << "Test Get Game Details By Id Passed" << endl;
	}
	else
	{
		cout << "Test Get Game Details By Id Failed" << endl;
	}
}

void test_insert_into_game_details()
{
	int GameId = 3, SocketAddress = 4125, count = 0;
	string UserName = "mani",Word="Elephant";
	DatabaseInterface* DBInterface = new DatabaseImplementation();
	string Status = DBInterface->insert_into_game_details(GameId, (char*)UserName.c_str(), SocketAddress, (char*)Word.c_str());
	vector<GameDetails> gamedetails = DBInterface->get_playing_game_detail(GameId);
	for (unsigned int i = 0; i < gamedetails.size(); i++)
	{
		if (gamedetails[i].get_username().compare(UserName)==0)
		{
			if (gamedetails[i].get_socket_address() == SocketAddress)
			{
				if (gamedetails[i].get_word_id().get_word().compare(Word)==0)
				{
					count++;
				}
			}
		}
	}

	if (count == 1)
	{
		cout << "Test Insert Game Details Passed" << endl;
	}
	else
	{
		cout << "Test Insert Game Details Failed" << endl;
	}
}





