#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <pqxx/pqxx>

#pragma execution_character_set("utf-8")

struct Conn {
    std::string dbname;
    std::string user;
    std::string password;
    std::string host;
    int port;
};

struct Client {
    int id;
    std::string first_name;
    std::string last_name;
    std::string email;
    std::vector<std::string> phones;
};

class Manager {
public:
    Manager(const std::string& db_connection_string);

    void CreateTable();
    void AddClient(const Client& client);
    void AddPhone(const std::string& email, const std::string& phone);
    void DeletePhone(const std::string& email, const std::string& phone);
    void DeleteClient(const std::string& email);
    void UpdateClient(const std::string& email, const std::string& newEmail, const std::string& newFirstName, const std::string& newLastName);
    std::vector<Client> FindClients(const std::string& search_query);

private:
    pqxx::connection conn;

    void ExecuteQuery(const std::string& query, const std::vector<std::string>& params = {});
};


