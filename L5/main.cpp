/*
void createDatabaseStructure();
		void addClient(const std::string& firstName, const std::string& lastName, const std::string& email);
		void addPhone(int clientId, const std::string& phone);


*/


#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>

using namespace std;
using namespace pqxx;


class ClientManager
{
public:
	ClientManager( string connection ):c(connection) /// creating connection in constructor
	{
		

	};
	ClientManager() { throw runtime_error( " Can't create.\n" ); };

private:
	connection c; 
	

};


int main()
{
	try {
		ClientManager m(
			"host=localhost " //обязательно пробел
			"port=5432 " //стандартный порт psql
			"dbname=postgres " //своё имя базы
			"user=postgres " //своё имя пользователя
			"password=");    //свой пароль
		std::cout << "Connection OK\n";

		
		
	}
	catch (pqxx::sql_error e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
}