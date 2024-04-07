#include <iostream>
#include <Windows.h>
#include "Client_Manager.h"



int main() {
    //setlocale(LC_ALL, "Russian");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8); 
    //system("chcp 1251");
    //system("cls");

    // Задаем вручную через консоль параметры подключения
    /*try {
    Manager manager(InputConnectLine());
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }*/

    // Задаем заранее параметры подключения
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

    try {
        Manager manager(db_connection_string);
        manager.CreateTable();

        // Добавляем клиентов
        Client client1 = { 1, "John", "Doe", "john.doe@example.com", {"+7 (921) 421-33-44", "+7 (921) 635-98-81"} };
        Client client2 = { 2, "Alice", "Smith", "alice@example.com", {"+7 (752) 451-63-82"} };
        Client client3 = { 3, "Bob", "Big", "BigBob@example.com", {"+7 (362) 123-45-67"} };
        Client client4 = { 4, "John", "Crown", "J_C_@example.com", };

        manager.AddClient(client1);
        manager.AddClient(client2);
        manager.AddClient(client3);
        manager.AddClient(client4);

        // Добавляем телефон для существующего клиента
        manager.AddPhone("J_C_@example.com", "+7 (111) 111-11-11");
        manager.AddPhone("J_C_@example.com", "+7 (222) 222-22-22");

        // Изменяем данные о клиенте
        manager.UpdateClient("BigBob@example.com", "new_BigBob@example.com", "Dob", "Big");

        // Удаляем телефон у клиента
        manager.DeletePhone("J_C_@example.com", "+7 (111) 111-11-11");

        // Удаляем клиента
        manager.DeleteClient("alice@example.com");

        // Находим клиентов по данным
        std::vector<Client> found_clients = manager.FindClients("John");
        std::cout << "Find clients like \"John\": " << std::endl;
        ShowFoundClients(found_clients);

    }
    catch (const pqxx::broken_connection& e) {
        system("chcp 1251");
        system("cls");
        std::cerr << "Connection Error: " << e.what() << std::endl;
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
};