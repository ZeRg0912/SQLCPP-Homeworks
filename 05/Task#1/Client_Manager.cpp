// ClientManager.cpp
#include "Client_Manager.h"
#include <iostream>
#include <sstream>

Manager::Manager(const std::string& db_connection_string) : conn(db_connection_string) {}

void Manager::CreateTable() {
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

void Manager::AddClient(const Client& client) {
    try {
        pqxx::work txn(conn);

        // Преобразование вектора телефонов в строку
        std::string phones = "{";
        for (size_t i = 0; i < client.phones.size(); i++) {
            phones += client.phones[i];
            if (i != client.phones.size() - 1) {
                phones += ",";
            }
        }
        phones += "}";
        // Вставка данных о клиенте
        pqxx::result res = txn.exec_params(R"(
            INSERT INTO clients (first_name, last_name, email, phones)
                VALUES ($1, $2, $3, $4) RETURNING id)",
                client.first_name, client.last_name, client.email, phones);
        txn.commit();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Manager::AddPhone(const std::string& email, const std::string& phone) {
    try {
        pqxx::work txn(conn);

        // Используем array_append для добавления нового телефона в массив
        txn.exec_params(R"(
            UPDATE clients SET phones = array_append(phones, $1) 
            WHERE email = $2)",
            phone, email);
        txn.commit();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Manager::DeletePhone(const std::string& email, const std::string& phone) {
    try {
        pqxx::work txn(conn);

        // Используем array_remove для удаления телефона из массива
        txn.exec_params(R"(
            UPDATE clients SET phones = array_remove(phones, $1) 
            WHERE email = $2)",
            phone, email);
        txn.commit();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Manager::DeleteClient(const std::string& email) {
    try {
        pqxx::work txn(conn);

        // Удаляем клиента
        txn.exec_params("DELETE FROM clients WHERE email = $1",
            email);

        txn.commit();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Manager::UpdateClient(const std::string& email, const std::string& newEmail, const std::string& newFirstName, const std::string& newLastName) {
    try {
        pqxx::work txn(conn);
        txn.exec_params(R"(
            UPDATE clients SET email = $1, first_name = $2, last_name = $3 
            WHERE email = $4)",
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

std::vector<Client> Manager::FindClients(const std::string& search_query) {
    std::vector<Client> results;
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_params(R"(
            SELECT id, first_name, last_name, email, phones FROM clients
            WHERE email = $1 OR first_name = $1 OR last_name = $1)",
            search_query);
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

//void Manager::ExecuteQuery(const std::string& query, const std::vector<std::string>& params) {
//    try {
//        pqxx::work txn(conn);
//        txn.exec_params(query, params);
//        txn.commit();
//    }
//    catch (const pqxx::sql_error& e) {
//        std::cerr << "SQL error: " << e.what() << std::endl;
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//    }
//}


std::string InputConnectLine() {
    std::string input;
    Conn conn;
    std::string db_connection_string;
    system("cls");
    std::cout << "Введите данные для подключения к БД:" << std::endl;
    std::cout << "Введите dbname: ";
    std::cin >> input;
    db_connection_string += "dbname=" + input;
    std::cout << "Введите user: ";
    std::cin >> input;
    db_connection_string += " user=" + input;
    std::cout << "Введите password: ";
    std::cin >> input;
    db_connection_string += " password=" + input;
    std::cout << "Введите host, можно указать просто local: ";
    std::cin >> input;
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    if (input == "local") input = "127.0.0.1";
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

void ShowFoundClients(std::vector<Client> found_clients) {
    for (const auto& client : found_clients) {
        std::cout << "Found client: " << client.first_name << " " << client.last_name << " (" << client.email << ")" << std::endl;
        std::string phone_str;
        std::string delete_chars = "{}\"'";
        std::cout << "Phone: ";
        for (const auto& phone : client.phones) {
            /*if (client.phones.back() == phone) std::cout << phone << std::endl;
            else std::cout << phone << ", ";*/
            if (client.phones.back() == phone) phone_str += phone;
            else {
                phone_str += phone;
                phone_str += ", ";
            }
        }
        for (char c : delete_chars) {
            phone_str.erase(std::remove(phone_str.begin(), phone_str.end(), c), phone_str.end());
        }
        std::cout << phone_str << std::endl;
    }
}
