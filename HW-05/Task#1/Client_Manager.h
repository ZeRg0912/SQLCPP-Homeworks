#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <pqxx/pqxx>

struct Client {
    int id;
    std::string first_name;
    std::string last_name;
    std::string email;
    std::vector<std::string> phones;
};

class ClientManager {
public:
    ClientManager(const std::string& db_connection_string);

    void createTables();
    void addClient(const Client& client);
    void addPhone(const std::string& email, const std::string& phone);
    void deletePhone(const std::string& email, const std::string& phone);
    void deleteClient(const std::string& email);
    void updateClient(const std::string& email, const std::string& newEmail, const std::string& newFirstName, const std::string& newLastName);
    std::vector<Client> findClients(const std::string& search_query);

private:
    pqxx::connection conn;

    void executeQuery(const std::string& query, const std::vector<std::string>& params = {});
};
