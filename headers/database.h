#include <pqxx/pqxx>

namespace Database{
    pqxx::connection* ConnectToDatabase();
    short PrepareStatements(pqxx::connection* dbConn);
}