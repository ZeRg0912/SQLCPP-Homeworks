cmake_minimum_required(VERSION 3.20)

set(PostgreSQL_ADDITIONAL_VERSIONS "16")

add_subdirectory("..\\libpqxx-7.9.0" pqxx-build)

set(SOURCES
	main.cpp
	Client_Manager.cpp
	Client_Manager.h
)

add_executable(ClientManager ${SOURCES})

target_compile_features(ClientManager PRIVATE cxx_std_17)

target_link_libraries(ClientManager pqxx)