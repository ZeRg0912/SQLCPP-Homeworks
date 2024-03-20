#pragma once
#include <iostream>
#include <vector>
#include <pqxx/pqxx>

struct Client {
	std::string connect;
	std::string name;
	std::string last_name;
	std::string email;
	std::vector <std::string> phone_number;
};

class Manager {
private:
	std::string conn;
public:
	void CreateTable();
	void CreateClient();
	void AddPhoneNumber();
	void ChangeClient();
	void DeletePhoneNumber();
	void DeleteClient();
	void ClientInfo();
};