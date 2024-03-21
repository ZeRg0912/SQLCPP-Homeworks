// ClientManager.cpp
#include "Client_Manager.h"
#include <iostream>
#include <sstream>

ClientManager::ClientManager(const std::string& db_connection_string) : conn(db_connection_string) {}

void ClientManager::createTables() {
    try {
        pqxx::work txn(conn);
        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS clients (
                id SERIAL PRIMARY KEY,
                first_name TEXT,
                last_name TEXT,
                email TEXT UNIQUE,
                phones TEXT[]
            )
        )");
        txn.commit();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ClientManager::addClient(const Client& client) {
    try {
        pqxx::work txn(conn);

        // Преобразование вектора телефонов в строку
        std::string phones_str = "{";
        for (size_t i = 0; i < client.phones.size(); ++i) {
            phones_str += "'" + client.phones[i] + "'";
            if (i != client.phones.size() - 1) {
                phones_str += ",";
            }
        }
        phones_str += "}";

        // Вставка данных о клиенте
        pqxx::result res = txn.exec_params("INSERT INTO clients (first_name, last_name, email, phones) VALUES ($1, $2, $3, $4) RETURNING id",
            client.first_name, client.last_name, client.email, phones_str);

        txn.commit();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ClientManager::addPhone(const std::string& email, const std::string& phone) {
    try {
        pqxx::work txn(conn);

        // Используем array_append для добавления нового телефона в массив
        txn.exec_params("UPDATE clients SET phones = array_append(phones, $1) WHERE email = $2", phone, email);

        txn.commit();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ClientManager::deletePhone(const std::string& email, const std::string& phone) {
    try {
        pqxx::work txn(conn);

        // Используем array_remove для удаления телефона из массива
        txn.exec_params("UPDATE clients SET phones = array_remove(phones, $1) WHERE email = $2", phone, email);

        txn.commit();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ClientManager::deleteClient(const std::string& email) {
    try {
        pqxx::work txn(conn);

        // Удаляем клиента
        txn.exec_params("DELETE FROM clients WHERE email = $1", email);

        txn.commit();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void ClientManager::updateClient(const std::string& email, const std::string& newEmail, const std::string& newFirstName, const std::string& newLastName) {
    try {
        pqxx::work txn(conn);
        txn.exec_params("UPDATE clients SET email = $1, first_name = $2, last_name = $3 WHERE email = $4",
            newEmail, newFirstName, newLastName, email);
        txn.commit();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

std::vector<Client> ClientManager::findClients(const std::string& search_query) {
    std::vector<Client> results;
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_params(R"(
            SELECT id, first_name, last_name, email, phones
            FROM clients
            WHERE email = $1 OR first_name = $1 OR last_name = $1
        )", search_query);
        txn.commit();

        for (const auto& row : res) {
            Client client;
            client.id = row["id"].as<int>();
            client.first_name = row["first_name"].as<std::string>();
            client.last_name = row["last_name"].as<std::string>();
            client.email = row["email"].as<std::string>();
            // Разделение строки телефонов по разделителю и добавление в вектор
            std::stringstream ss(row["phones"].as<std::string>());
            std::string phone;
            while (std::getline(ss, phone, ',')) {
                client.phones.push_back(phone);
            }
            results.push_back(client);
        }
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return results;
}

void ClientManager::executeQuery(const std::string& query, const std::vector<std::string>& params) {
    try {
        pqxx::work txn(conn);
        txn.exec_params(query, params);
        txn.commit();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
