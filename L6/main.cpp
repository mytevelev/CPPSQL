
#include <iostream>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Windows.h> //для SetConsoleOutputCP

#pragma execution_character_set ("utf-8") //строковые литералы в UTF-8

class Publisher;
class Stock;
class Book;

using namespace std;
using namespace Wt::Dbo;

class Publisher {
public:
    std::string	name;
    collection<ptr<Book>>	books;

    template <typename Action>
    void persist(Action& a) {
        field(a, name, "name");
        hasMany(a, books, ManyToOne, "publisher");
    }
};

class Book {
public:
    std::string	title;
    ptr<Publisher>	publisher;
    collection < ptr<Stock> > stocks;

    template <typename Action>
    void persist(Action& a) {
        field(a, title, "title");
        belongsTo(a, publisher, "publisher");
        hasMany(a, stocks, ManyToOne, "book");
    }
};

class Shop {
public:
    string	name;
    collection <Wt::Dbo::ptr<Stock> > stocks;

    template <typename Action>
    void persist(Action& a) {
        field(a, name, "name");
        hasMany(a, stocks, Wt::Dbo::ManyToOne, "shop");
    }
};

class Sale {
public:
    string	price;
    string	data_sale;
    ptr<Stock>	stock{};
    int	count = 0;

    template <typename Action>
    void persist(Action& a) {
        field(a, price, "price");
        field(a, data_sale, "data sale");
        belongsTo(a, stock, "stock");
        field(a, count, "count");
    }
};

class Stock {
public:
    ptr<Book> book;
    ptr<Shop> shop;
    int	count=0;
    collection < ptr<Sale> > sales;

    template <typename Action>
    void persist(Action& a) {
        belongsTo(a, book, "book");
        belongsTo(a, shop, "shop");
        field(a, count, "count");
        hasMany(a, sales, ManyToOne, "stock");
    }
};

int main(int argc, char** argv) {
    SetConsoleOutputCP(CP_UTF8); //вывод на консоль в UTF-8

    try {
        //коннектимся к базе
        string connectionString = //введите ваши данные для подключения
            "host=localhost "
            "port=5432 "
            "user=postgres "
            "password= "
            "client_encoding=UTF8 ";

        unique_ptr<backend::Postgres> connection{ new backend::Postgres(connectionString) };
        Session session;
        session.setConnection(move(connection));


        session.mapClass<Book>("book");
        session.mapClass<Publisher>("Publisher");
        session.mapClass<Shop>("Shop");
        session.mapClass<Stock>("Stock");
        session.mapClass<Sale>("Sale");

#if 0
        //Создание таблиц, если их ещё не было
        session.createTables();
        cout << "Create tables ok!\n";
        
        //Заполнение тестовыми данными...
#else 
        //Чтение таблиц, если уже были
        session.rereadAll();
        cout << "Read tables ok!\n";
#endif
	    //выполнение запроса...
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}
