#include <iostream>
#include <Windows.h>
#include "Client_Manager.h"

#pragma execution_character_set("utf-8")

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

	std::cout << "Hello World!123" << std::endl;

    std::string dbname = "ClientManager";
    std::string user = "postgres";
    std::string password = "Zerg0912";
    std::string host = "127.0.0.1";
    int port = 5432;

    std::string db_connection_string = 
        "dbname=" + dbname + 
        " user=" + user + 
        " password=" + password + 
        " host=" + host + 
        " port=" + std::to_string(port);
    ClientManager manager(db_connection_string);
    manager.createTables();

    // Добавляем клиентов
    Client client1 = { 1, "John", "Doe", "john.doe@example.com", {"1111111", "2222222"} };
    //manager.addClient(client1);

    Client client2 = { 2, "Alice", "Smith", "alice@example.com", {"111222333"} };
    //manager.addClient(client2);

    // Добавляем телефон для существующего клиента
    manager.addPhone("john.doe@example.com", "555555555");

    // Изменяем данные о клиенте
    manager.updateClient("alice@example.com", "new_alice@example.com", "Alice", "Doe");

    // Удаляем телефон у клиента
    manager.deletePhone("john.doe@example.com", "555555555");

    // Удаляем клиента
    manager.deleteClient("new_alice@example.com");

    // Находим клиентов по данным
    std::vector<Client> foundClients = manager.findClients("john");
    for (const auto& client : foundClients) {
        std::cout << "Found client: " << client.first_name << " " << client.last_name << " (" << client.email << ")" << std::endl;
        for (const auto& phone : client.phones) {
            std::cout << "Phone: " << phone << std::endl;
        }
    }

    return 0;
};