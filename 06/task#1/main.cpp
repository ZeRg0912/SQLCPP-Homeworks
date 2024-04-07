#include <iostream>
#include <ctime>
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
	Wt::Dbo::ptr<Book> id_book;
	Wt::Dbo::ptr<Shop> id_shop;
	int count;
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
	std::string date_sale = "";
	Wt::Dbo::ptr<Stock> id_stock;
	int count;

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

std::string InputRequest() {
	system("cls");
	std::string input;
	std::cout << "Введите Издателя (id или название): ";
	std::cin >> input;
	return input;
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
		//std::cerr << "Error: " << e.what() << std::endl;
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
		//std::cout << "Publishers added successfully!" << std::endl;		
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
		//std::cout << "Shops added successfully!" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding shops!" << e.what() << std::endl;
	}
}

void AddBooks(Wt::Dbo::Session& session, int entries, std::string publisher) {
	try {
		Wt::Dbo::Transaction transaction(session);

		Wt::Dbo::collection<Wt::Dbo::ptr<Publisher>> publishers = session.find<Publisher>().where("name=?").bind(publisher);
		//Wt::Dbo::ptr<Publisher> publisher = session.find<Publisher>().where("name=?").bind(publisher);

		std::vector<std::string> values;
		for (int i = 0; i < entries; i++) {
			values.push_back(publisher + "_Book_" + std::to_string(i + 1));
		}

		for (const auto& publisher : publishers) {
			for (const auto& name : values) {
				std::unique_ptr<Book> valuePtr(new Book{ name });
				publisher.modify()->books.insert(session.add(std::move(valuePtr)));
			}
		}
		//std::cout << "Books added successfully!" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding books! " << e.what() << std::endl;
	}
}

void AddStocks(Wt::Dbo::Session& session, int count, std::string book, std::string shop) {
	try {
		Wt::Dbo::Transaction transaction(session);

		Wt::Dbo::ptr<Book> bookDb = session.find<Book>().where("title=?").bind(book);
		Wt::Dbo::ptr<Shop> shopDb = session.find<Shop>().where("name=?").bind(shop);

		std::unique_ptr<Stock> stockPtr(new Stock{ bookDb, shopDb, count });
		bookDb.modify()->stocks.insert(session.add(std::move(stockPtr)));
		//std::cout << "Stock added successfully!" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding stock! " << e.what() << std::endl;
	}
}

void AddSales(Wt::Dbo::Session& session, int price, std::string date, int stock_id, int count) {
	try {
		Wt::Dbo::Transaction transaction(session);

		Wt::Dbo::ptr<Stock> stock = session.find<Stock>().where("id=?").bind(stock_id);

		std::unique_ptr<Sale> salePtr(new Sale{ price, date, stock, count });
		stock.modify()->sales.insert(session.add(std::move(salePtr)));
		
		//std::cout << "Sales added successfully!" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding books! " << e.what() << std::endl;
	}
}

void FindAboutPublisher(Wt::Dbo::Session& session, std::string publisher) {
	try {
		system("cls");
		Wt::Dbo::Transaction transaction(session);
		Wt::Dbo::ptr<Publisher> publisherDb;
		// Проверка имя или ид
		if (publisher.find_first_not_of("0123456789") == std::string::npos) {
			publisherDb = session.find<Publisher>().where("id=?").bind(stoi(publisher));
		}
		else {
			publisherDb = session.find<Publisher>().where("name=?").bind(publisher);
		}
		Wt::Dbo::collection<Wt::Dbo::ptr<Book>> books_of_publisher = session.find<Book>().where("publisher_id=?").bind(publisherDb);
		Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stocks_of_books;
		Wt::Dbo::collection<Wt::Dbo::ptr<Shop>> shops_of_stocks;

		std::vector<std::string> shops;

		std::cout << "Find " << publisherDb->name << ":" << std::endl;
		std::cout << "Shops where selling his books:" << std::endl;
		for (const auto& book : books_of_publisher) {
			stocks_of_books = session.find<Stock>().where("book_id=?").bind(book);
			for (const auto& stock : stocks_of_books) {
				if (stock->count > 0) {
					shops_of_stocks = session.find<Shop>().where("id=?").bind(stock->id_shop);
					for (const auto& shop : shops_of_stocks) {
						if (std::find(shops.begin(), shops.end(), shop->name) == shops.end()) {
							shops.push_back(shop->name);
							std::cout << shop->name << std::endl;
						}
					}
				}
			}
		}
		
	}
	catch (const std::exception& e) {
		std::cerr << "Error find publisher! " << e.what() << std::endl;
	}
}

int main() {
	srand(static_cast<int>(time(0)));
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

#if 1
	try {
		//std::unique_ptr<Wt::Dbo::backend::Postgres> connection = std::make_unique<Wt::Dbo::backend::Postgres>(InputConnectLine());
		std::unique_ptr<Wt::Dbo::backend::Postgres> connection = std::make_unique<Wt::Dbo::backend::Postgres>(connection_string);
		Wt::Dbo::Session session;
		session.setConnection(std::move(connection)); 
		Wt::Dbo::Transaction transaction(session);

		int num_publishers = 3;
		int num_shops = 5;
		int num_books = 3;
		int num_stocks = num_publishers * num_books;

		std::string publisher = "Publisher_";
		std::string book = "Book_";
		std::string shop = "Shop_";

		// Объявление таблицы
		AddTables(session);

		// Добавляем контент
		// Добавляем publishers
		try {
			AddPublishers(session, num_publishers);
			std::cout << "Publishers added successfully!" << std::endl;
		} 
		catch(const std::exception& e) {
			std::cerr << "Error adding publishers!" << std::endl;
		}

		// Добавляем books
		try {
			for (int i = 1; i <= num_publishers; i++) {
				AddBooks(session, num_books, (publisher + std::to_string(i)));
			}
			std::cout << "Books added successfully!" << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << "Error adding books! " << e.what() << std::endl;
		}
		

		// Добавляем shops
		try {
			AddShops(session, num_shops);
			std::cout << "Shops added successfully!" << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << "Error adding shops!" << std::endl;
		}

		// Добавляем stocks
		try {
			for (int i = 1; i <= num_publishers; i++) {
				for (int j = 1; j <= num_books; j++) {
					AddStocks(session, (0 + rand() % (100 - 0)), (publisher + std::to_string(i) + "_" + book + std::to_string(j)), (shop + std::to_string(1 + rand() % (num_shops - 1))));
				}
			}
			std::cout << "Stock added successfully!" << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << "Error adding stock! " << e.what() << std::endl;
		}

		// Добаляем sales
		try {
			for (int i = 1; i <= num_stocks; i++) {
				for (int j = 0; j <= (0 + rand() % (5 - 0)); j++) {
					int year = 2000 + rand() % (2024 - 2000);
					int month = 1 + rand() % (12 - 01);
					std::string formatted_month = (month < 10) ? "0" + std::to_string(month) : std::to_string(month);
					int day = 1 + rand() % (28 - 01);
					std::string formatted_day = (day < 10) ? "0" + std::to_string(day) : std::to_string(day);
					std::string date = std::to_string(year) + "-" + formatted_month + "-" + formatted_day;
					AddSales(session, (50 + rand() % (500 - 50)), date, i, (10 + rand() % (35 - 10)));
				}
			}
			std::cout << "Sales added successfully!" << std::endl;
		} catch(const std::exception& e) {
			std::cerr << "Error adding books! " << e.what() << std::endl;
		}		

		// Отправляем в БД
		transaction.commit();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
#endif

	try {
		std::unique_ptr<Wt::Dbo::backend::Postgres> connection = std::make_unique<Wt::Dbo::backend::Postgres>(connection_string);
		Wt::Dbo::Session session;
		session.setConnection(std::move(connection));
		Wt::Dbo::Transaction transaction(session);

		AddTables(session);
		FindAboutPublisher(session, InputRequest());
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}