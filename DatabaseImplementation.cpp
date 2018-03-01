#include "DatabaseImplementation.h"

/*In constructor the connection handler is get from the Database connection class and stored in a class instance for future use
then the encryption key value is set here*/
DatabaseImplementation::DatabaseImplementation()
{
	SubTest = Logger::getInstance(LOG4CPLUS_TEXT("Database Implementation"));
	SqlConnHandle = Connection.get_connection_handler();
}

/*In destructor the connection handler is delete to free memory*/
DatabaseImplementation::~DatabaseImplementation()
{
	SQLDisconnect(SqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SqlConnHandle);
}

SQLHANDLE DatabaseImplementation::select(SQLHANDLE SqlHandle, SQLWCHAR* Query)
{
	return (SQL_SUCCESS != SQLExecDirect(SqlHandle, Query, SQL_NTS)) ? 0 : SqlHandle;
}

/*This method is to get list of category from database and return it as vector of category object*/
vector<Category> DatabaseImplementation::get_category()
{
	LOG4CPLUS_INFO(SubTest, "Getting Category from Database");
	vector<Category> CategoryVector;
	Category CategoryObject;
	SQLHANDLE SqlHandle = NULL;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	/*check whether the query is executed successfully if not then it will return empty vector*/
	if (SqlHandle = (select(SqlHandle, GET_CATEGORY)))
	{
		SQLINTEGER Id, IsActive, PtrSqlVersion;
		SQLCHAR Name[50];
		/*while loop to fetch the data from the handler and store it into vector*/
		while (SQLFetch(SqlHandle) == SQL_SUCCESS)
		{
			SQLGetData(SqlHandle, 1, SQL_INTEGER, &Id, 0, &PtrSqlVersion);
			SQLGetData(SqlHandle, 2, SQL_CHAR, Name, 50, &PtrSqlVersion);
			SQLGetData(SqlHandle, 3, SQL_INTEGER, &IsActive, 0, &PtrSqlVersion);

			CategoryObject.set_id(Id);
			CategoryObject.set_name(Cryption.decoder((char*)Name));//method to decrypt the string from human non-understandable form to readable string
			CategoryObject.set_is_active(IsActive);
			CategoryVector.push_back(CategoryObject);
		}
	}
	else
	{
		LOG4CPLUS_ERROR(SubTest, "Cannot get Category from Database");
	}
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	return CategoryVector;
}

/*This method is to get list of difficulty from database and return it as vector of difficulty object*/
vector<Difficulty> DatabaseImplementation::get_difficulty()
{
	LOG4CPLUS_INFO(SubTest, "Getting Difficulty from Database");
	vector<Difficulty> DifficultyVector;
	Difficulty DifficultyObject;
	SQLHANDLE SqlHandle = NULL;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	/*check whether the query is executed successfully if not then it will return empty vector*/
	if (SqlHandle = (select(SqlHandle, GET_DIFFICULTY)))
	{
		SQLINTEGER Id, IsActive, PtrSqlVersion;
		SQLCHAR Name[50];
		/*while loop to fetch the data from the handler and store it into vector*/
		while (SQLFetch(SqlHandle) == SQL_SUCCESS)
		{
			SQLGetData(SqlHandle, 1, SQL_INTEGER, &Id, 0, &PtrSqlVersion);
			SQLGetData(SqlHandle, 2, SQL_CHAR, Name, 50, &PtrSqlVersion);
			SQLGetData(SqlHandle, 3, SQL_INTEGER, &IsActive, 0, &PtrSqlVersion);

			DifficultyObject.set_id(Id);
			DifficultyObject.set_name(Cryption.decoder((char*)Name));//method to decrypt the string from human non-understandable form to readable string
			DifficultyObject.set_is_active(IsActive);
			DifficultyVector.push_back(DifficultyObject);
		}
	}
	else
	{
		LOG4CPLUS_ERROR(SubTest, "Cannot get Difficulty from Database");
	}
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	return DifficultyVector;
}

/*This method is to get the currently playing game details and send the list as vector of Gamedetails object to business logic */
vector<GameDetails> DatabaseImplementation::get_playing_game_detail()
{
	LOG4CPLUS_INFO(SubTest, "Getting Game details from Database");
	vector<GameDetails> GameDetailsVector;
	SQLHANDLE SqlHandle = NULL;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	/*check whether the query is executed successfully if not then it will return empty vector*/
	if (SqlHandle = (select(SqlHandle, GET_PLAYING_GAME_DETAILS)))
	{
		GameDetailsVector = get_game_details(SqlHandle);
	}
	else
	{
		LOG4CPLUS_ERROR(SubTest, "Cannot get Game details from Database");
	}
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	return GameDetailsVector;
}

/*This method is to get a particular currently playing game detail and sent it as vector of gamedetails to business logic*/
vector<GameDetails> DatabaseImplementation::get_playing_game_detail(int GameId)
{
	LOG4CPLUS_INFO(SubTest, "Getting Game details with Game id from Database");
	vector<GameDetails> GameDetailsVector;
	SQLHANDLE SqlHandle = NULL;
	SQLRETURN ReturnCode;
	SQLINTEGER SqlGameId, PtrValue = SQL_NTS;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	ReturnCode = SQLPrepare(SqlHandle, GET_PLAYING_GAME_DETAILS_BY_ID, SQL_NTS);
	ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlGameId, 0, &PtrValue);
	SqlGameId = GameId;
	ReturnCode = SQLExecute(SqlHandle);
	/*check whether the query is executed successfully if not then it will return empty vector*/
	if (SQL_SUCCESS != ReturnCode)
	{
		LOG4CPLUS_ERROR(SubTest, "Cannot get Game details with game id from Database");
		SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
		return GameDetailsVector;
	}
	else
	{
		GameDetailsVector = get_game_details(SqlHandle);
	}
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	return GameDetailsVector;
}

/*This method is to return gamedetails vector for the given SqlHandler*/
vector<GameDetails> DatabaseImplementation::get_game_details(SQLHANDLE SqlHandle)
{
	LOG4CPLUS_INFO(SubTest, "Getting Game details from SQl handler");
	vector<GameDetails> GameDetailsVector;
	GameDetails GameDetailsObject;
	Words WordObject;
	SQLINTEGER GameId, SocketAddress, PtrSqlVersion;
	SQLCHAR UserName[50], Word[50];
	/*while loop to fetch the data from the handler and store it into vector*/
	while (SQLFetch(SqlHandle) == SQL_SUCCESS)
	{
		SQLGetData(SqlHandle, 1, SQL_INTEGER, &GameId, 0, &PtrSqlVersion);
		SQLGetData(SqlHandle, 2, SQL_CHAR, UserName, 50, &PtrSqlVersion);
		SQLGetData(SqlHandle, 3, SQL_INTEGER, &SocketAddress, 0, &PtrSqlVersion);
		SQLGetData(SqlHandle, 4, SQL_CHAR, Word, 50, &PtrSqlVersion);
		GameDetailsObject.set_game_id(GameId);
		GameDetailsObject.set_username(Cryption.decoder((char*)UserName));//method to decrypt the string from human non-understandable form to readable string
		GameDetailsObject.set_socket_address(SocketAddress);
		WordObject.set_word(Cryption.decoder((char*)Word));//method to decrypt the string from human non-understandable form to readable string
		GameDetailsObject.set_word_id(WordObject);
		GameDetailsVector.push_back(GameDetailsObject);
	}
	return GameDetailsVector;
}

/*This method is to give the socket address for the given game id*/
vector<int> DatabaseImplementation::get_socket_address_by_game_id(int GameId)
{
	LOG4CPLUS_INFO(SubTest, "Getting Socket address with game id from Database");
	vector<int> SocketVector;
	SQLHANDLE SqlHandle = NULL;
	SQLRETURN ReturnCode;
	SQLINTEGER SqlGameId, PtrValue = SQL_NTS;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	ReturnCode = SQLPrepare(SqlHandle, GET_SOCKET_ADDRESS_BY_GAME_ID, SQL_NTS);
	ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlGameId, 0, &PtrValue);
	SqlGameId = GameId;
	ReturnCode = SQLExecute(SqlHandle);
	/*check whether the query is executed successfully if not then it will return empty vector*/
	if (SQL_SUCCESS != ReturnCode)
	{
		LOG4CPLUS_ERROR(SubTest, "Cannot get Socket Address with game id from Database");
		SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
		return SocketVector;
	}
	else
	{
		SQLINTEGER SocketAddress, PtrSqlVersion;
		/*while loop to fetch the data from the handler and store it into vector*/
		while (SQLFetch(SqlHandle) == SQL_SUCCESS)
		{
			SQLGetData(SqlHandle, 1, SQL_INTEGER, &SocketAddress, 0, &PtrSqlVersion);
			SocketVector.push_back(SocketAddress);
		}
	}
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	return SocketVector;
}

/*This method is to give the maximum game id to the business logic for creating a new unique game id for new game*/
int DatabaseImplementation::get_maximum_game_id()
{
	LOG4CPLUS_INFO(SubTest, "Getting Maximum game id from Database");
	SQLHANDLE SqlHandle = NULL;
	SQLINTEGER GameId = 0, PtrSqlVersion;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	/*check whether the query is executed successfully if not then it will return -1 to indicate no execution failed*/
	if (SqlHandle = (select(SqlHandle, GET_MAXIMUM_GAME_ID)))
	{
		/*while loop to fetch the data from the handler*/
		while (SQLFetch(SqlHandle) == SQL_SUCCESS)
		{
			SQLGetData(SqlHandle, 1, SQL_INTEGER, &GameId, 0, &PtrSqlVersion);
		}

		SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	}
	else
	{
		LOG4CPLUS_ERROR(SubTest, "Cannot get Maximum game id from Database");
	}
	return (GameId > 0) ? (GameId == 1024) ? 0 : GameId : 0;
}

/*This method id to update the game result by using the game id*/
string DatabaseImplementation::update_game_result(int GameId, char* Result)
{
	LOG4CPLUS_INFO(SubTest, "Updating Game result to Database");
	SQLHANDLE SqlHandle = NULL;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	SQLRETURN ReturnCode;
	char SqlResult[50];
	SQLINTEGER SqlGameId, PtrValue = SQL_NTS;
	ReturnCode = SQLPrepare(SqlHandle, UPDATE_GAME_RESULT, SQL_NTS);
	ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(Result), 0, &SqlResult, 0, &PtrValue);
	ReturnCode = SQLBindParameter(SqlHandle, 2, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlGameId, 0, &PtrValue);
	strcpy_s((char*)SqlResult, _countof(SqlResult), Result);
	SqlGameId = GameId;
	ReturnCode = SQLExecute(SqlHandle);
	/*check whether the query is executed successfully if not then it will return a string*/
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	if (SQL_SUCCESS != ReturnCode)
	{
		LOG4CPLUS_ERROR(SubTest, "Error Quering SQL Server");
		return "Error Quering SQL Server";
	}
	else
	{
		LOG4CPLUS_INFO(SubTest, "Updated Successfully");
		return "Updated Successfully";
	}
}

string DatabaseImplementation::update_game_result(int GameId, int SocketAddress, char* Result)
{
	LOG4CPLUS_INFO(SubTest, "Updating Game result for disconnected client to Database");
	SQLHANDLE SqlHandle = NULL;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	SQLRETURN ReturnCode;
	char SqlResult[50];
	SQLINTEGER SqlGameId, SqlSocketAddress, PtrValue = SQL_NTS;
	ReturnCode = SQLPrepare(SqlHandle, UPDATE_GAME_RESULT_EXIT, SQL_NTS);
	ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(Result), 0, &SqlResult, 0, &PtrValue);
	ReturnCode = SQLBindParameter(SqlHandle, 2, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlGameId, 0, &PtrValue);
	ReturnCode = SQLBindParameter(SqlHandle, 3, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlSocketAddress, 0, &PtrValue);
	strcpy_s((char*)SqlResult, _countof(SqlResult), Result);
	SqlGameId = GameId;
	SqlSocketAddress = SocketAddress;
	ReturnCode = SQLExecute(SqlHandle);
	/*check whether the query is executed successfully if not then it will return a string*/
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	if (SQL_SUCCESS != ReturnCode)
	{
		LOG4CPLUS_ERROR(SubTest, "Error Quering SQL Server");
		return "Error Quering SQL Server";
	}
	else
	{
		LOG4CPLUS_INFO(SubTest, "Updated Successfully");
		return "Updated Successfully";
	}
}

/*This method is used to get the Updated result from gamedetails with the game id*/
/*This method is for testing purpose only*/
vector<GameDetails> DatabaseImplementation::get_updated_result(int GameId)
{
	LOG4CPLUS_INFO(SubTest, "Getting Game results from Database(Testing purpose)");
	SQLHANDLE SqlHandle = NULL;
	vector<GameDetails> GameDetailsVector;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	SQLRETURN ReturnCode;
	SQLINTEGER SqlGameId, PtrValue = SQL_NTS;
	ReturnCode = SQLPrepare(SqlHandle, GET_UPDATED_RESULT_BY_GAME_ID, SQL_NTS);
	ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlGameId, 0, &PtrValue);
	SqlGameId = GameId;
	ReturnCode = SQLExecute(SqlHandle);
	if (SQL_SUCCESS != ReturnCode)
	{
		LOG4CPLUS_ERROR(SubTest, "Error Quering SQL Server");
		return GameDetailsVector;
	}
	else
	{
		GameDetails GameDetailsObject;
		SQLINTEGER Id, PtrSqlVersion;
		SQLCHAR Result[50];
		/*while loop to fetch the data from the handler and store it into vector*/
		while (SQLFetch(SqlHandle) == SQL_SUCCESS)
		{
			SQLGetData(SqlHandle, 1, SQL_INTEGER, &Id, 0, &PtrSqlVersion);
			SQLGetData(SqlHandle, 2, SQL_CHAR, Result, 50, &PtrSqlVersion);
			GameDetailsObject.set_game_id(Id);
			GameDetailsObject.set_result((char*)Result);
			GameDetailsVector.push_back(GameDetailsObject);
		}
	}
	return GameDetailsVector;
}

/*This method is used to get the Updated result from gamedetails with the game id*/
/*This method is for testing purpose only*/
vector<GameDetails> DatabaseImplementation::get_updated_result(int GameId, int SocketAddress)
{
	LOG4CPLUS_INFO(SubTest, "Getting Game results from Database(Testing purpose)");
	SQLHANDLE SqlHandle = NULL;
	vector<GameDetails> GameDetailsVector;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	SQLRETURN ReturnCode;
	SQLINTEGER SqlGameId, SqlSocketAddress, PtrValue = SQL_NTS;
	ReturnCode = SQLPrepare(SqlHandle, GET_UPDATED_RESULT_BY_SOCKET_ADDRESS, SQL_NTS);
	ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlGameId, 0, &PtrValue);
	ReturnCode = SQLBindParameter(SqlHandle, 2, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlSocketAddress, 0, &PtrValue);
	SqlGameId = GameId;
	SqlSocketAddress = SocketAddress;
	ReturnCode = SQLExecute(SqlHandle);
	if (SQL_SUCCESS != ReturnCode)
	{
		LOG4CPLUS_ERROR(SubTest, "Error Quering SQL Server");
		return GameDetailsVector;
	}
	else
	{
		GameDetails GameDetailsObject;
		SQLINTEGER Id, PtrSqlVersion;
		SQLCHAR Result[50];
		/*while loop to fetch the data from the handler and store it into vector*/
		while (SQLFetch(SqlHandle) == SQL_SUCCESS)
		{
			SQLGetData(SqlHandle, 1, SQL_INTEGER, &Id, 0, &PtrSqlVersion);
			SQLGetData(SqlHandle, 2, SQL_CHAR, Result, 50, &PtrSqlVersion);
			GameDetailsObject.set_game_id(Id);
			GameDetailsObject.set_result((char*)Result);
			GameDetailsVector.push_back(GameDetailsObject);
		}
	}
	return GameDetailsVector;
}

/*This method will give a random word from database for a new game based on category name and difficulty name choosen by the user*/
string DatabaseImplementation::get_word(char* CategoryName, char* DifficultyName)
{
	LOG4CPLUS_INFO(SubTest, "Getting Random Word from Database");
	SQLHANDLE SqlHandle = NULL;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	SQLRETURN ReturnCode;
	char SqlCategory[50], SqlDifficulty[50];
	SQLINTEGER PtrValue = SQL_NTS, PtrSqlVersion;
	ReturnCode = SQLPrepare(SqlHandle, GET_WORD, SQL_NTS);
	strcpy_s((char*)SqlCategory, _countof(SqlCategory), Cryption.encoder(CategoryName).c_str()); //method to encrypt the string to human non-understandable form
	strcpy_s((char*)SqlDifficulty, _countof(SqlDifficulty), Cryption.encoder(DifficultyName).c_str());//method to encrypt the string to human non-understandable form
	ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(CategoryName), 0, &SqlCategory, 0, &PtrValue);
	ReturnCode = SQLBindParameter(SqlHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(DifficultyName), 0, &SqlDifficulty, 0, &PtrValue);
	/*check whether the query is executed successfully if not then it will return string*/
	if (SQL_SUCCESS != SQLExecute(SqlHandle))
	{
		LOG4CPLUS_ERROR(SubTest, "Error Quering SQL Server");
		SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
		return "Error querying SQL Server";
	}
	else
	{
		char Word[50];
		/*while loop to fetch the data from the handler*/
		while (SQLFetch(SqlHandle) == SQL_SUCCESS)
		{
			SQLGetData(SqlHandle, 1, SQL_CHAR, Word, 50, &PtrSqlVersion);
		}
		SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
		return Cryption.decoder(Word);//method to decrypt the string from human non-understandable form to readable string;
	}
}

/*This method is used to get category name for a given word*/
string DatabaseImplementation::get_category_name_by_word(char* Word)
{
	LOG4CPLUS_INFO(SubTest, "Getting Category name from Database");
	SQLHANDLE SqlHandle = NULL;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	SQLRETURN ReturnCode;
	int WordId = 0;
	char SqlWord[50],SqlCategoryName[50];
	SQLINTEGER PtrValue = SQL_NTS, PtrSqlVersion;

	ReturnCode = SQLPrepare(SqlHandle, GET_CATEGORY_NAME_WITH_WORD, SQL_NTS);
	ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(Word), 0, &SqlWord, 0, &PtrValue);
	strcpy_s((char*)SqlWord, _countof(SqlWord), Cryption.encoder(Word).c_str());//method to encrypt the string to human non-understandable form
	/*check whether the query is executed successfully if not then it will return string*/
	if (SQL_SUCCESS != SQLExecute(SqlHandle))
	{
		LOG4CPLUS_ERROR(SubTest, "Error Quering SQL Server");
		SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
		return "Error querying SQL Server";
	}
	else
	{
		/*while loop to fetch the data from the handler*/
		while (SQLFetch(SqlHandle) == SQL_SUCCESS)
		{
			SQLGetData(SqlHandle, 1, SQL_CHAR, &SqlCategoryName, 50, &PtrSqlVersion);
		}
	}
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	return Cryption.decoder(SqlCategoryName);
}


/*This method is used to insert game details into databse for maintaining game records*/
string DatabaseImplementation::insert_into_game_details(int GameId, char* UserName, int SocketAddress, char* Word)
{
	LOG4CPLUS_INFO(SubTest, "Inserting Game details into Database");
	SQLHANDLE SqlHandle = NULL;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	SQLRETURN ReturnCode;
	int WordId = 0;
	char SqlUserName[50], SqlWord[50];
	SQLINTEGER SqlGameId, SqlWordId, SqlSocketAddress, PtrValue = SQL_NTS, PtrSqlVersion;

	ReturnCode = SQLPrepare(SqlHandle, SELECT_WORD_ID, SQL_NTS);
	ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(Word), 0, &SqlWord, 0, &PtrValue);
	strcpy_s((char*)SqlWord, _countof(SqlWord), Cryption.encoder(Word).c_str());//method to encrypt the string to human non-understandable form
	/*check whether the query is executed successfully if not then it will return string*/
	if (SQL_SUCCESS != SQLExecute(SqlHandle))
	{
		LOG4CPLUS_ERROR(SubTest, "Error Quering SQL Server");
		SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
		return "Error querying SQL Server";
	}
	else
	{
		/*while loop to fetch the data from the handler*/
		while (SQLFetch(SqlHandle) == SQL_SUCCESS)
		{
			SQLGetData(SqlHandle, 1, SQL_INTEGER, &WordId, 50, &PtrSqlVersion);
		}
	}
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	ReturnCode = SQLPrepare(SqlHandle, INSERT_INTO_GAME_DETAILS, SQL_NTS);
	ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlGameId, 0, &PtrValue);
	SqlGameId = GameId;
	ReturnCode = SQLBindParameter(SqlHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, strlen(UserName), 0, &SqlUserName, 0, &PtrValue);
	strcpy_s((char*)SqlUserName, _countof(SqlUserName), Cryption.encoder(UserName).c_str());//method to encrypt the string to human non-understandable form
	ReturnCode = SQLBindParameter(SqlHandle, 3, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlWordId, 0, &PtrValue);
	SqlWordId = WordId;
	ReturnCode = SQLBindParameter(SqlHandle, 4, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlSocketAddress, 0, &PtrValue);
	SqlSocketAddress = SocketAddress;
	ReturnCode = SQLExecute(SqlHandle);
	/*check whether the query is executed successfully if not then it will return string*/
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	if (SQL_SUCCESS != ReturnCode)
	{
		LOG4CPLUS_ERROR(SubTest, "Error Quering SQL Server");
		return "Error Quering SQL Server";
	}
	else
	{
		LOG4CPLUS_INFO(SubTest, "Inserted Successfully");
		return "Inserted Successfully";
	}
}

/*This is private class method for inserting into category table while starting the server
the input is read from the xml file*/
void DatabaseImplementation::insert_into_category(vector<Category> CategoryVector)
{
	LOG4CPLUS_INFO(SubTest, "Inserting Category details into Database");
	SQLHANDLE SqlHandle = NULL;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	SQLRETURN ReturnCode;
	char SqlName[50];
	SQLINTEGER SqlIsActive, PtrValue = SQL_NTS;
	int Size = CategoryVector.size();
	for (int i = 0; i < Size; i++)
	{
		string Name = CategoryVector[i].get_name();
		int IsActive = CategoryVector[i].get_is_active();
		ReturnCode = SQLPrepare(SqlHandle, (SQLWCHAR*)INSERT_INTO_CATEGORY, SQL_NTS);
		ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, Name.size(), 0, &SqlName, 0, &PtrValue);
		strcpy_s((char*)SqlName, _countof(SqlName), Cryption.encoder(Name).c_str());//method to encrypt the string to human non-understandable form
		ReturnCode = SQLBindParameter(SqlHandle, 2, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlIsActive, 0, &PtrValue);
		SqlIsActive = IsActive;
		ReturnCode = SQLExecute(SqlHandle);
	}
	if (SQL_SUCCESS != ReturnCode)
	{
		LOG4CPLUS_ERROR(SubTest, "Error Quering SQL Server");
	}
	else
	{
		LOG4CPLUS_INFO(SubTest, "Inserted Successfully");
	}
	/*check whether the query is executed successfully if not then it will return string*/
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
}

/*This is private class method for inserting into difficulty table while starting the server
the input is read from the xml file*/
void DatabaseImplementation::insert_into_difficulty(vector<Difficulty> DifficultyVector)
{
	LOG4CPLUS_INFO(SubTest, "Inserting Difficulty details into Database");
	SQLHANDLE SqlHandle = NULL;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	SQLRETURN ReturnCode;
	char SqlName[50];
	SQLINTEGER  SqlIsActive, PtrValue = SQL_NTS;
	int Size = DifficultyVector.size();
	for (int i = 0; i < Size; i++)
	{
		string Name = DifficultyVector[i].get_name();
		int IsActive = DifficultyVector[i].get_is_active();
		ReturnCode = SQLPrepare(SqlHandle, (SQLWCHAR*)INSERT_INTO_DIFFICULTY, SQL_NTS);
		ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, Name.size(), 0, &SqlName, 0, &PtrValue);
		strcpy_s((char*)SqlName, _countof(SqlName), Cryption.encoder(Name).c_str());//method to encrypt the string to human non-understandable form
		ReturnCode = SQLBindParameter(SqlHandle, 2, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlIsActive, 0, &PtrValue);
		SqlIsActive = IsActive;
		ReturnCode = SQLExecute(SqlHandle);
	}
	if (SQL_SUCCESS != ReturnCode)
	{
		LOG4CPLUS_ERROR(SubTest, "Error Quering SQL Server");
	}
	else
	{
		LOG4CPLUS_INFO(SubTest, "Inserted Successfully");
	}
	/*check whether the query is executed successfully if not then it will return string*/
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
}

/*This is private class method for inserting into words table while starting the server
the input is read from the xml file*/
void DatabaseImplementation::insert_into_words(vector<Words> WordVector)
{
	LOG4CPLUS_INFO(SubTest, "Inserting Word details into Database");
	SQLHANDLE SqlHandle = NULL;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	SQLRETURN ReturnCode;
	char SqlWord[50];
	SQLINTEGER SqlIsActive, SqlCategoryId, SqlDifficultyId, PtrValue = SQL_NTS;
	int Size = WordVector.size();
	for (int i = 0; i < Size; i++)
	{
		int CategoryId = WordVector[i].get_category_id().get_id();
		int DifficultyId = WordVector[i].get_difficulty_id().get_id();
		string Word = WordVector[i].get_word();
		int IsActive = WordVector[i].get_is_active();
		ReturnCode = SQLPrepare(SqlHandle, (SQLWCHAR*)INSERT_INTO_WORDS, SQL_NTS);
		ReturnCode = SQLBindParameter(SqlHandle, 1, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlCategoryId, 0, &PtrValue);
		SqlCategoryId = CategoryId;
		ReturnCode = SQLBindParameter(SqlHandle, 2, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlDifficultyId, 0, &PtrValue);
		SqlDifficultyId = DifficultyId;
		ReturnCode = SQLBindParameter(SqlHandle, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, Word.size(), 0, &SqlWord, 0, &PtrValue);
		strcpy_s((char*)SqlWord, _countof(SqlWord), Cryption.encoder(Word).c_str());//method to encrypt the string to human non-understandable form
		ReturnCode = SQLBindParameter(SqlHandle, 4, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &SqlIsActive, 0, &PtrValue);
		SqlIsActive = IsActive;
		ReturnCode = SQLExecute(SqlHandle);
	}
	if (SQL_SUCCESS != ReturnCode)
	{
		LOG4CPLUS_ERROR(SubTest, "Error Quering SQL Server");
	}
	else
	{
		LOG4CPLUS_INFO(SubTest, "Inserted Successfully");
	}
	/*check whether the query is executed successfully if not then it will return string*/
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
}
/*This method to execute procedure call*/
SQLRETURN DatabaseImplementation::procedure_call(SQLWCHAR* Query)
{
	SQLHANDLE SqlHandle = NULL;
	SQLRETURN ReturnCode;
	SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
	ReturnCode = SQLExecDirect(SqlHandle, Query, SQL_NTS);
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	return ReturnCode;
}

/*This is the starting method which will load data to database from xml file
Before loading the data it will delete the already available data along with the table
and create a new table and insert the new data*/
void DatabaseImplementation::load_data()
{
	ifstream File(XML_FILE);
	if (File)
	{
		LOG4CPLUS_INFO(SubTest, "Loading the DataBase with the Value from the Data.xml file");
		DatabaseXmlParser Xml;
		xml_document<> Document;
		xml_node<> *Node;
		stringstream Buffer;
		Buffer << File.rdbuf();
		File.close();
		string Content(Buffer.str());
		Document.parse<0>(&Content[0]);
		Node = Document.first_node();
		Xml.set_node(Node);
		SQLRETURN ReturnCode;
		ReturnCode = procedure_call(CHECK_TABLE_PROCEDURE);
		if ((SQL_SUCCESS != ReturnCode) && (ReturnCode != SQL_SUCCESS_WITH_INFO))
		{
			LOG4CPLUS_ERROR(SubTest, "Error Quering Check Table Procedure");
			return;
		}
		else
		{
			ReturnCode = procedure_call(CREATE_CATEGORY_PROCEDURE);
			if ((SQL_SUCCESS != ReturnCode) && (ReturnCode != SQL_SUCCESS_WITH_INFO))
			{
				LOG4CPLUS_ERROR(SubTest, "Error Quering Create Category Procedure");
				return;
			}
			else
			{
				vector<Category> CatergoryVector;
				CatergoryVector = Xml.get_category_from_xml(Xml.get_node());
				insert_into_category(CatergoryVector);
			}
			ReturnCode = procedure_call(CREATE_DIFFICULTY_PROCEDURE);
			if ((SQL_SUCCESS != ReturnCode) && (ReturnCode != SQL_SUCCESS_WITH_INFO))
			{
				LOG4CPLUS_ERROR(SubTest, "Error Quering Create DIfficulty Procedure");
				return;
			}
			else
			{
				vector<Difficulty> DifficultyVector;
				DifficultyVector = Xml.get_difficulty_from_xml(Xml.get_node());
				insert_into_difficulty(DifficultyVector);
			}
			ReturnCode = procedure_call(CREATE_WORDS_PROCEDURE);
			if ((SQL_SUCCESS != ReturnCode) && (ReturnCode != SQL_SUCCESS_WITH_INFO))
			{
				LOG4CPLUS_ERROR(SubTest, "Error Quering Create Word Procedure");
				return;
			}
			else
			{
				vector<Words> WordVector;
				WordVector = Xml.get_words_from_xml(Xml.get_node());
				insert_into_words(WordVector);
			}
		}
	}
	else
	{
		cout << "Data.Xml file is missing so the program starts with old data" << endl;
		LOG4CPLUS_WARN(SubTest, "Data.Xml file is missing so the program starts with old data");
	}
}
