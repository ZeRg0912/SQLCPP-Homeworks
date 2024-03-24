#include <iostream>
#include <Windows.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>

#pragma execution_character_set( "utf-8" )

bool CheckConnection(const std::string& connection_string) {
	try {
		std::unique_ptr<Wt::Dbo::backend::Postgres> connection = std::make_unique<Wt::Dbo::backend::Postgres>(connection_string);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
}

class Post;

class User {
public:
	std::string name = "";
	std::string phone = "";
	int karma = 0;
	Wt::Dbo::collection<Wt::Dbo::ptr<Post>> posts;

	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::field(a, phone, "phone");
		Wt::Dbo::field(a, karma, "karma");
		Wt::Dbo::hasMany(a, posts, Wt::Dbo::ManyToOne, "author");
	}
};

class Post{
public:
	std::string title = "";
	std::string content = "";
	Wt::Dbo::ptr<User> author;

	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, title, "title");
		Wt::Dbo::field(a, content, "content");
		Wt::Dbo::belongsTo(a, author, "author");
	}
};

int main1() {
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

		// объявление таблицы
		session.mapClass<User>("user");
		session.mapClass<Post>("post");

		Wt::Dbo::Transaction transaction(session);


		// Создание таблиц
		//session.createTables();


		// Созданик объектов в таблице
		/*std::unique_ptr<User> joe(new User);
		joe->name = "Joe";
		joe->phone = "+79876543210";
		joe->karma = 0;

		std::unique_ptr<User> steve(new User{ "Steve", "+71234567890", 50 });

		Wt::Dbo::ptr<User> joeDb = session.add(std::move(joe));

		session.add(std::move(steve));*/

		/*std::unique_ptr<User> steve(new User{ "Steve", "+7 (812) 444 55 55", 50 });
		std::unique_ptr<User> joe(new User{ "Joe", "+7 (921) 555 44 33", 33 });
		std::unique_ptr<User> marta(new User{ "Marta", "+7 (888) 999 12 12", 10 });

		session.add(std::move(steve));
		session.add(std::move(joe));
		session.add(std::move(marta));*/


		// Поиск пользователя с фильтрами
		//Wt::Dbo::ptr<User> user = session.find<User>().where("name=? AND karma=?").bind("Steve").bind(50);

		/*if (user) {
			std::cout << "User found!" << std::endl;
			std::cout << "User karma: " << user->karma << std::endl;
		} else std::cout << "User not found!" << std::endl;

		std::cout << std::endl;*/


		// Изменение данных у объекта
		/*Wt::Dbo::ptr<User> stevePtr = session.find<User>().where("name=?").bind("Steve");

		if (stevePtr) {
			stevePtr.modify()->karma = 100;
			std::cout << "User karma: " << stevePtr->karma << std::endl;
		}*/


		// Использование коллекции
		/*Wt::Dbo::collection<Wt::Dbo::ptr<User>> users = session.find<User>().where("name=?").bind("Steve");

		for (const auto& user : users) {
			std::cout << "User found!" << std::endl;
			std::cout << "User name: " << user->name << std::endl;
			std::cout << "User phone: " << user->phone << std::endl;
			std::cout << "User karma: " << user->karma << std::endl;
			std::cout << std::endl;
		}*/

		// Создание постов и связи меж таблицами
		/*Wt::Dbo::ptr<User> steve = session.find<User>().where("name=?").bind("Steve");
		if (steve) {
			std::unique_ptr<Post> p1(new Post{ "My first post", "blablabla" });
			steve.modify()->posts.insert(session.add(std::move(p1)));

			std::unique_ptr<Post> p2(new Post{ "My second post", "tututu" });
			steve.modify()->posts.insert(session.add(std::move(p2)));
		}*/

		// смена автора у поста
		Wt::Dbo::ptr<User> marta = session.find<User>().where("name=?").bind("Marta");

		Wt::Dbo::ptr<Post> p1 = session.find<Post>().where("id=1");

		if (p1) p1.modify()->author = marta;

		transaction.commit();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}