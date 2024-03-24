#include <iostream>
#include <Windows.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>

#pragma execution_character_set( "utf-8" )

class Publisher;
class Book;
class Shop;
class Stock;
class Sale;

class Publisher {
public:
	std::string name = "";
	Wt::Dbo::collection<Wt::Dbo::ptr<Book>> books;

	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, books, Wt::Dbo::ManyToOne, "id_publisher");
	}
};

class Book {
public:
	std::string title = "";
	Wt::Dbo::ptr<Publisher> id_publisher;
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stocks;

	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, title, "title");
		Wt::Dbo::belongsTo(a, id_publisher, "id_publisher");
		Wt::Dbo::hasMany(a, stocks, Wt::Dbo::ManyToOne, "id_book");
	}
};

class Shop {
public:
	std::string name = "";
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stocks;

	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, stocks, Wt::Dbo::ManyToOne, "id_shop");
	}
};

class Stock {
public:
	int count;
	Wt::Dbo::ptr<Book> id_book;
	Wt::Dbo::ptr<Shop> id_shop;
	Wt::Dbo::collection<Wt::Dbo::ptr<Sale>> sales;

	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::belongsTo(a, id_book, "id_book");
		Wt::Dbo::belongsTo(a, id_shop, "id_shop");
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::hasMany(a, sales, Wt::Dbo::ManyToOne, "id_stock");
	}
};

class Sale {
public:
	int price;
	int count;
	std::string date_sale = "";
	Wt::Dbo::ptr<Stock> id_stock;

	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, price, "price");
		Wt::Dbo::field(a, date_sale, "date_sale");
		Wt::Dbo::belongsTo(a, id_stock, "id_stock");
		Wt::Dbo::field(a, count, "count");
	}
};

int main() {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	/*system("chcp 1251");
	system("cls");*/

	std::string connection_string =
		"host=localhost "
		"port=5432 "
		"dbname=BookShop "
		"user=postgres "
		"password=Zerg0912";

	try {
		std::unique_ptr<Wt::Dbo::backend::Postgres> connection = std::make_unique<Wt::Dbo::backend::Postgres>(connection_string);
		Wt::Dbo::Session session;
		session.setConnection(std::move(connection)); 
		Wt::Dbo::Transaction transaction(session);

		// объ€вление таблицы
		session.mapClass<Publisher>("Publisher");
		session.mapClass<Book>("Book");
		session.mapClass<Shop>("Shop");
		session.mapClass<Stock>("Stock");
		session.mapClass<Sale>("Sale");

		session.createTables();

		transaction.commit();

	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	

	return 0;
}