
#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <vector>

using namespace std;
using namespace pqxx;


class ClientManager
{
public:
	void deletePhoneNumber(int clientId, const string& phone) 
	{ 
		work w(c); 
		w.exec_params("DELETE FROM phones WHERE client_id = $1 AND phone = $2;", clientId, phone); 
		w.commit(); 
		
	}


	void deleteClient(int clientId) 
	{ 
		work w(c); 
		w.exec_params("DELETE FROM clients WHERE id = $1;", clientId); 
		w.commit();
		 
	}
	void updateClient( int Ck , string fn, string ln, string email )
	{
		work w(c);
		w.exec_params("UPDATE clients SET first_name = $1, last_name = $2, email = $3 WHERE id = $4; ",  fn,ln, email, Ck);
		w.commit();
	};

	int findClient( string email )
	{
		work w(c);
		result r = w.exec_params("select id from clients where email = $1; ", email);
		if (r.size() > 1 || !r.size()) throw( runtime_error(" Not found or more then one client.\n"));
		return r[0]["id"].as<int>();
	};

	void addPhone(int Ck, string p )
	{
		work w(c);
		w.exec_params("insert into phones(client_id, phone ) values( $1 , $2); ", Ck,  p );
		w.commit();
	};

	void addClient( string fn, string ln, string email  )
	{
		work w(c);
		w.exec_params("INSERT INTO clients (first_name, last_name, email) VALUES ($1, $2, $3);",
			fn, ln, email);
		w.commit();
	};

	void createDatabaseStructure()
	{
		work w(c);
		w.exec("CREATE TABLE IF NOT EXISTS clients ("
			"id SERIAL PRIMARY KEY, "
			"first_name VARCHAR(50), "
			"last_name VARCHAR(50), "
			"email VARCHAR(100) UNIQUE NOT NULL);");

		w.exec( "CREATE TABLE IF NOT EXISTS phones ("
			"id SERIAL PRIMARY KEY, "
			"client_id INTEGER REFERENCES clients(id) ON DELETE CASCADE, "
			"phone VARCHAR(20));" );
		w.commit();




	};


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
		cout << "Connection OK\n";

		m.createDatabaseStructure();
		cout << "Created OK\n";

		// Add sample clients 
#ifdef ADDNAMES

		m.addClient("John", "Smith", "john.smith@example.com"); 
		m.addClient("Emily", "Johnson", "emily.johnson@example.com"); 
		m.addClient("Michael", "Williams", "michael.williams@example.com"); 
		m.addClient("Sarah", "Brown", "sarah.brown@example.com"); 
		m.addClient("David", "Jones", "david.jones@example.com"); 
		m.addClient("Jessica", "Garcia", "jessica.garcia@example.com"); 
		m.addClient("Chris", "Martinez", "chris.martinez@example.com"); 
		m.addClient("Amanda", "Lee", "amanda.lee@example.com"); 
		m.addClient("James", "Lopez", "james.lopez@example.com"); 
		m.addClient("Laura", "Wilson", "laura.wilson@example.com");
		cout << "Created OK\n";
#endif // ADDNAMES
		
	auto r=	m.findClient("chris.martinez@example.com");
	cout << "Find! "<< r << endl;

	//m.addPhone( r, "7777777777");

//		m.updateClient( 1 , "1111","2222","33333" );
	//m.deleteClient(1);
	//cout << "Client and their phone numbers deleted for client ID " << 1 << "\n";
	/*
	string phone = "8888888888";
	int cI = 7;
	m.deletePhoneNumber(cI, phone );
	cout << "Phone number " << phone << " deleted for client ID " << cI << "\n";
	*/


	}
	catch (sql_error e)
	{
		cout << e.what() << endl;
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
}