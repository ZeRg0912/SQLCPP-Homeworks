#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>

#undef max

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
		Wt::Dbo::hasMany(a, books, Wt::Dbo::ManyToOne, "publisher");
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
		Wt::Dbo::belongsTo(a, id_publisher, "publisher");
		Wt::Dbo::hasMany(a, stocks, Wt::Dbo::ManyToOne, "book");
	}
};

class Shop {
public:
	std::string name = "";
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stocks;

	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, stocks, Wt::Dbo::ManyToOne, "shop");
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
		Wt::Dbo::belongsTo(a, id_book, "book");
		Wt::Dbo::belongsTo(a, id_shop, "shop");
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::hasMany(a, sales, Wt::Dbo::ManyToOne, "stock");
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
		Wt::Dbo::belongsTo(a, id_stock, "stock");
		Wt::Dbo::field(a, count, "count");
	}
};

std::string InputConnectLine() {
	std::string input;
	std::string db_connection_string;
	system("cls");
	std::cout << "Введите данные для подключения к БД:" << std::endl;
	std::cout << "Введите dbname: ";
	std::cin >> input;
	db_connection_string += "dbname=" + input;
	std::cout << "Введите user: ";
	std::cin >> input;
	transform(input.begin(), input.end(), input.begin(), ::tolower);
	if (input == "admin") input = "postgres";
	db_connection_string += " user=" + input;
	std::cout << "Введите password: ";
	std::cin >> input;
	db_connection_string += " password=" + input;
	std::cout << "Введите host, можно указать просто local: ";
	std::cin >> input;
	transform(input.begin(), input.end(), input.begin(), ::tolower);
	if (input == "local") input = "localhost";
	db_connection_string += " host=" + input;
	input = " ";
	while (!(input.find_first_not_of("0123456789") == std::string::npos)) {
		std::cout << "Введите port, можно укзаать просто base: ";
		std::cin >> input;
		transform(input.begin(), input.end(), input.begin(), ::tolower);
		if (input == "base") {
			input = "5432";
			break;
		}
		if (!(input.find_first_not_of("0123456789") == std::string::npos)) {
			std::cout << "Неверный ввод, повторите!" << std::endl;
			std::cout << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else break;
	}
	db_connection_string += " port=" + input;
	return db_connection_string;
}

int InputEntries() {
	system("cls");
	int input;
	// Проверка на правильный ввод
	while (true) {
		std::cout << "Введите кол-во для добавления контента: ";
		std::cin >> input;
		if (std::cin.fail()) {
			std::cout << "Неверный ввод, повторите!" << std::endl;
			std::cout << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else {
			return input;
		}
	}
}

void AddTables(Wt::Dbo::Session& session) {
	session.mapClass<Publisher>("Publisher");
	session.mapClass<Book>("Book");
	session.mapClass<Shop>("Shop");
	session.mapClass<Stock>("Stock");
	session.mapClass<Sale>("Sale");
	// Создание таблиц, если их еще нет
	try {
		session.createTables();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

void AddPublishers(Wt::Dbo::Session& session, int entries) {
	try {
		Wt::Dbo::Transaction transaction(session);

		std::vector<std::string> values;
		for (int i = 0; i < entries; i++) {
			values.push_back("Publisher_" + std::to_string(i + 1));
		}

		for (const auto& name : values) {
			std::unique_ptr<Publisher> valuePtr(new Publisher{ name });
			Wt::Dbo::ptr<Publisher> value = session.add(std::move(valuePtr));
		}
		std::cout << "Publishers added successfully!" << std::endl;		
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding publishers!" << std::endl;
	}
}

void AddShops(Wt::Dbo::Session& session, int entries) {
	try {
		Wt::Dbo::Transaction transaction(session);

		std::vector<std::string> values;
		for (int i = 0; i < entries; i++) {
			values.push_back("Shop_" + std::to_string(i + 1));
		}

		for (const auto& name : values) {
			std::unique_ptr<Shop> valuePtr(new Shop{ name });
			Wt::Dbo::ptr<Shop> value = session.add(std::move(valuePtr));
		}
		std::cout << "Shops added successfully!" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding shops!" << std::endl;
	}
}

void AddBooksToPublisher(Wt::Dbo::Session& session, int entries, std::string name) {
	try {
		Wt::Dbo::Transaction transaction(session);

		Wt::Dbo::collection<Wt::Dbo::ptr<Publisher>> publishers = session.find<Publisher>().where("name=?").bind(name);
		//Wt::Dbo::ptr<Publisher> publisher = session.find<Publisher>().where("name=?").bind(name);

		std::vector<std::string> values;
		for (int i = 0; i < entries; i++) {
			values.push_back(name + "_Book_" + std::to_string(i + 1));
		}

		for (const auto& publisher : publishers) {
			for (const auto& name : values) {
				std::unique_ptr<Book> valuePtr(new Book{ name });
				publisher.modify()->books.insert(session.add(std::move(valuePtr)));
			}
		}
		std::cout << "Books added successfully!" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding books! " << e.what() << std::endl;
	}
}

int main() {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	/*system("chcp 1251");
	system("cls");*/

	// Строка подключения к БД
	std::string connection_string =
		"host=localhost "
		"port=5432 "
		"dbname=BookShop "
		"user=postgres "
		"password=Zerg0987";

	try {
		//std::unique_ptr<Wt::Dbo::backend::Postgres> connection = std::make_unique<Wt::Dbo::backend::Postgres>(InputConnectLine());
		std::unique_ptr<Wt::Dbo::backend::Postgres> connection = std::make_unique<Wt::Dbo::backend::Postgres>(connection_string);
		Wt::Dbo::Session session;
		session.setConnection(std::move(connection)); 
		Wt::Dbo::Transaction transaction(session);

		// Объявление таблицы
		AddTables(session);

		// Добавляем контент
		AddPublishers(session, 3);

		AddBooksToPublisher(session, 3, "Publisher_1");
		AddBooksToPublisher(session, 2, "Publisher_2");
		AddBooksToPublisher(session, 1, "Publisher_3");



		// Отправляем в БД
		transaction.commit();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}