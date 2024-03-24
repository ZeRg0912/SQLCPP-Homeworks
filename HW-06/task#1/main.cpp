#include <iostream>
#include <Windows.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>

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

class User {
public:
	std::string name = "";
	std::string phone = "";
	int karma = 0;

	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::field(a, phone, "phone");
		Wt::Dbo::field(a, karma, "karma");
	}
};

int main() {
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	system("chcp 1251");
	system("cls");

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
		session.mapClass<User>("user");
		session.createTables();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}