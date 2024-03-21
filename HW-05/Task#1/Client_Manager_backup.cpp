#include "Client_Manager.h"

Manager::Manager(
	const std::string& dbname,
	const std::string& user,
	const std::string& password,
	const std::string& host,
	const std::string& port
) : conn(
	"dbname=" + dbname +
	" user=" + user +
	" password=" + password + 
	" host=" + host + 
	" port=" + port
) {}

void Manager::CreateTable() {
	try {
		ExecuteUpdate(
			"CREATE TABLE IF NOT EXISTS clients (    \
                id SERIAL PRIMARY KEY,               \
                first_name TEXT,                     \
                last_name TEXT,                      \
                email TEXT                           \
            )",
			{}
		);
		ExecuteUpdate(
			"CREATE TABLE IF NOT EXISTS phones (     \
                id SERIAL PRIMARY KEY,               \
                client_id INT REFERENCES clients(id),\
                phone_number TEXT                    \
            )",
			{}
		);
	}
	catch (const std::exception& e) {
		std::cerr << "Error creating table: " << e.what() << std::endl;
	}
}

void Manager::AddClient(
	const std::string& first_name,
	const std::string& last_name,
	const std::string& email) 
{
	try {
		ExecuteUpdate(
			"INSERT INTO clients					\
				(first_name, last_name, email)		\
				VALUES ($1, $2, $3)",
			{ first_name, last_name, email }
		);
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding client: " << e.what() << std::endl;
	}
}

void Manager::AddPhone(
	const std::string& first_name,
	const std::string& last_name,
	const std::string& phone) 
{
	try {
		pqxx::result result = ExecuteQuery(
			"SELECT id FROM clients							\
				WHERE first_name = $1 AND last_name = $2",
			{ first_name, last_name }
		);
		int client_id = result[0][0].as<int>();
		ExecuteUpdate(
			"INSERT INTO phones					\
				(client_id, phone_number)		\
				VALUES ($1, $2)",
			{ std::to_string(client_id), phone }
		);
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding phone: " << e.what() << std::endl;
	}
}

void Manager::UpdateClient(
	const std::string& first_name,
	const std::string& last_name,
	const std::string& email,

	const std::string& new_first_name,
	const std::string& new_last_name,
	const std::string& new_email) 
{
	try {
		ExecuteUpdate(
			"UPDATE clients							\
				SET		first_name = $4,			\
						last_name = $5,				\
						email = $6,					\
				WHERE	first_name = $1	AND			\
						last_name = $2	AND			\
						email = $3",
			{ first_name, last_name, email, new_first_name, new_last_name, new_email }
		);
	}
	catch (const std::exception& e) {
		std::cerr << "Error updating client: " << e.what() << std::endl;
	}
}

void Manager::DeletePhone(
	const std::string& first_name,
	const std::string& last_name,
	const std::string& phone) 
{
	try {
		pqxx::result result = ExecuteQuery(
			"SELECT id FROM clients							\
				WHERE first_name = $1 AND last_name = $2",
			{ first_name, last_name }
		);
		int client_id = result[0][0].as<int>();
		ExecuteUpdate(
			"DELETE FROM phones					\
				WHERE	client_id = $1 AND		\
						phone_number = $2",
			{ std::to_string(client_id), phone }
		);
	}
	catch (const std::exception& e) {
		std::cerr << "Error deleting phone: " << e.what() << std::endl;
	}
}

void Manager::DeleteClient(
	const std::string& first_name,
	const std::string& last_name,
	const std::string& email)
{
	try {
		ExecuteUpdate(
			"DELETE FROM clients				\
				WHERE	first_name = $1 AND		\
						last_name = $2	AND		\
						email = $3",
			{ first_name, last_name, email }
		);
	}
	catch (const std::exception& e) {
		std::cerr << "Error deleting client: " << e.what() << std::endl;
	}
}

std::vector<Client> Manager::FindClient(const std::string& search_term) {
	std::vector<Client> results;
	try {
		pqxx::result result = ExecuteQuery(
			"SELECT * FROM clients				\
				WHERE	first_name = $1 OR		\
						last_name = $2	OR		\
						emal = $3",
			{ search_term, search_term, search_term }
		);
		for (const auto& row : result) {
			Client client;
			client.id = row["id"].as<int>();
			client.first_name = row["first_name"].as<std::string>();
			client.last_name = row["last_name"].as<std::string>();
			client.email = row["email"].as<std::string>();
			results.push_back(client);
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error finding client: " << e.what() << std::endl;
	}
	return results;
}

pqxx::result Manager::ExecuteQuery(
	const std::string& query,
	const std::vector <std::string>& parameters)
{
	pqxx::nontransaction txn(conn);
	pqxx::result result(txn.exec_params(query, parameters));
	return result;
}

void Manager::ExecuteUpdate(
	const std::string& query,
	const std::vector<std::string>& parameters)
{
	pqxx::work txn(conn);
	txn.exec_params(query, parameters);
	txn.commit();
}