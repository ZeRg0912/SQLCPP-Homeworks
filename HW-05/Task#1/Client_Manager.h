#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <pqxx/pqxx>

struct Client {
	int id;
	std::string first_name;
	std::string last_name;
	std::string email;
	std::vector <std::string> phone_number;
};

class Manager {
private:
	std::string conn_string;
	pqxx::connection conn;

	pqxx::result ExecuteQuery(
		const std::string& query, 
		const std::vector <std::string>& parameters
	);

	void ExecuteUpdate(
		const std::string& query,
		const std::vector<std::string>& parameters
	);

public:
	Manager(
		const std::string& dbname,
		const std::string& user,
		const std::string& password,
		const std::string& host,
		const std::string& port
	);

	void CreateTable();

	void AddClient(
		const std::string& first_name,
		const std::string& last_name,
		const std::string& email
	);

	void AddPhone(
		const std::string& first_name,
		const std::string& last_name,
		const std::string& phone
	);

	void UpdateClient(
		const std::string& first_name,
		const std::string& last_name,
		const std::string& email,

		const std::string& new_first_name,
		const std::string& new_last_name,
		const std::string& new_email
	);

	void DeletePhone(
		const std::string& first_name,
		const std::string& last_name,
		const std::string& phone
	);

	void DeleteClient(
		const std::string& first_name,
		const std::string& last_name,
		const std::string& email
	);

	std::vector<Client> FindClient(const std::string& search_term);
};