#include <iostream>
#include <Windows.h>
#include "Client_Manager.h"



int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    std::string dbname = "ClientManager";
    std::string user = "postgres";
    std::string password = "Zerg0987";
    std::string host = "127.0.0.1";
    int port = 5432;

    std::string db_connection_string = 
        "dbname=" + dbname + 
        " user=" + user + 
        " password=" + password + 
        " host=" + host + 
        " port=" + std::to_string(port);
    Manager manager(db_connection_string);
    manager.CreateTable();

    // Добавляем клиентов
    Client client1 = { 1, "John", "Doe", "john.doe@example.com", {"+7 (921) 421-33-44", "+7 (921) 635-98-81"} };
    Client client2 = { 2, "Alice", "Smith", "alice@example.com", {"+7 (752) 451-63-82"} };
    Client client3 = { 3, "Bob", "Big", "BigBob@example.com", {"+7 (362) 123-45-67"} };
    Client client4 = { 4, "Alex", "Crown", "A_C_@example.com", };

    /*manager.AddClient(client1);
    manager.AddClient(client2);
    manager.AddClient(client3);
    manager.AddClient(client4);*/

    // Добавляем телефон для существующего клиента
    manager.AddPhone("A_C_@example.com", "+7 (111) 111-11-11");
    manager.AddPhone("A_C_@example.com", "+7 (222) 222-22-22");

    // Изменяем данные о клиенте
    //manager.UpdateClient("alice@example.com", "new_alice@example.com", "Alice", "Doe");

    // Удаляем телефон у клиента
    manager.DeletePhone("A_C_@example.com", "+7 (111) 111-11-11");
    manager.DeletePhone("A_C_@example.com", "111");

    // Удаляем клиента
    //manager.DeleteClient("john.doe@example.com");

    // Находим клиентов по данным
    std::vector<Client> foundClients = manager.FindClients("Alex");
    for (const auto& client : foundClients) {
        std::cout << "Found client: " << client.first_name << " " << client.last_name << " (" << client.email << ")" << std::endl;
        std::string phone_str;
        std::string delete_chars = "{}'";
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

    return 0;
};