
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
//using namespace Wt::Dbo::backend;

class Publisher 
{
public:
    string	name;
    collection<ptr<Book>>	books;

    template <typename Action>
    void persist(Action& a) 
    {
        field(a, name, "name");
        hasMany(a, books, ManyToOne, "publisher");
    }
};

class Book 
{
public:
    string	title;
    ptr<Publisher>	publisher;
    collection < ptr<Stock> > stocks;

    template <typename Action>
    void persist(Action& a) 
    {
        field(a, title, "title");
        belongsTo(a, publisher, "publisher");
        hasMany(a, stocks, ManyToOne, "book");
    }
};

class Shop 
{
public:
    string	name;
    collection <Wt::Dbo::ptr<Stock> > stocks;

    template <typename Action>
    void persist(Action& a) 
    {
        field(a, name, "name");
        hasMany(a, stocks, ManyToOne, "shop");
    }
};

class Sale 
{
public:
    string	price;
    string	data_sale;
    ptr<Stock>	stock{};
    int	count = 0;

    template <typename Action>
    void persist(Action& a) 
    {
        field(a, price, "price");
        field(a, data_sale, "data sale");
        belongsTo(a, stock, "stock");
        field(a, count, "count");
    }
};

class Stock 
{
public:
    ptr<Book> book;
    ptr<Shop> shop;
    int	count=0;
    collection < ptr<Sale> > sales;

    template <typename Action>
    void persist(Action& a) 
    {
        belongsTo(a, book, "book");
        belongsTo(a, shop, "shop");
        field(a, count, "count");
        hasMany(a, sales, ManyToOne, "stock");
    }
};

int main(int argc, char** argv) 
{
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

        //Заполнение тестовыми данными...
        /*
        Transaction transaction(session); //подготовка транзакции
        unique_ptr<Publisher> publisher = make_unique<Publisher>();
        publisher->name = "Tolik";
        ptr<Publisher> publisher_tolik = session.add(move(publisher));

        unique_ptr<Shop> shop1 = make_unique<Shop>();
        shop1->name = "Магазин 1";
        ptr<Shop> shop1_added = session.add(move(shop1));

        unique_ptr<Shop> shop2 = make_unique<Shop>();
        shop2->name = "Магазин 2";
        ptr<Shop> shop2_added = session.add(move(shop2));

        unique_ptr<Book> book1 = make_unique<Book>();
        book1->title = "Книга 1";
        book1->publisher = publisher_tolik;
        ptr<Book> book1_added = session.add(move(book1));

        unique_ptr<Book> book2 = make_unique<Book>();
        book2->title = "Книга 2";
        book2->publisher = publisher_tolik;
        ptr<Book> book2_added = session.add(move(book2));

        unique_ptr<Stock> stock1 = make_unique<Stock>();
        stock1->book = book1_added;
        stock1->shop = shop1_added;
        session.add(move(stock1));

        unique_ptr<Stock> stock2 = make_unique<Stock>();
        stock2->book = book2_added;
        stock2->shop = shop2_added;
        session.add(move(stock2));
        //выполнение транзакции
        transaction.commit();
        cout << "Data added";
        */
        //выполнение запроса
//Получаем из stdin имя publisher
//пусть будет толик
        Transaction transaction(session); //подготовка транзакции
        std::set<std::string> shop_names_for_deal_publisher{};
        Wt::Dbo::ptr<Publisher> deal_publisher = session.find<Publisher>().where("name = ?").bind("Tolik");

        for (auto book : deal_publisher->books) {
            std::cout << "Book: " << book->title << "\n";
            for (auto stock : book->stocks) {
                shop_names_for_deal_publisher.insert(stock->shop->name);
            }
        }

        std::cout << "Books of the publisher '" << deal_publisher->name << "' are sold in shops: " << std::endl;

        for (auto shop_name : shop_names_for_deal_publisher) {
            std::cout << " - " << shop_name << std::endl;
        }
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}
