#include <database.h>

// Before connecting, run a shell command to create the database, also use the config.json file for loading
pqxx::connection* Database::ConnectToDatabase(){
    try{
        pqxx::connection* conn = new pqxx::connection("dbname=DB_NAME hostaddr=127.0.0.1 port=5432");
        if(!conn->is_open()){
            printf("[E] Error connecting to the database...\n");
            delete conn;
            return nullptr;
        }
        return conn;
    }
    catch(const std::exception& e){
        printf("[E] %s\n",e.what());
        return nullptr;
    }
    
    return nullptr;
}
/*
    ### Prepares statements for PostgreSQL
*/
short Database::PrepareStatements(pqxx::connection* dbConn){
    try{
        // Insert customer
        dbConn->prepare("insert_user", 
            "INSERT INTO users(id, username, email, password, role, \"firstName\", \"lastName\", phone, \"registrationDate\", birthday, gender, subscriptions, \"coachData\", auth) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14)");
        dbConn->prepare("insert_gym", 
            "INSERT INTO gyms(id, name, address,\"dateCreated\") "
            "VALUES ($1, $2, $3, $4)");
        dbConn->prepare("insert_subscription", 
            "INSERT INTO subscriptions(id, name, \"dateCreated\", length, uses, price) "
            "VALUES ($1, $2, $3, $4, $5, $6)");
        dbConn->prepare("insert_subscription_payment", 
            "INSERT INTO \"subscriptionPayments\"(id, \"clientId\", \"subscriptionId\",\"gymId\", method, date, discount) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7)");
        dbConn->prepare("insert_client_subscription", 
            "INSERT INTO \"clientSubscriptions\"(id, \"clientId\", \"subscriptionId\",\"gymId\",\"paymentId\",\"startDate\",\"endDate\", \"usedTimes\", active, \"presenceLog\") "
            "VALUES ($1, $2, $3, $4, $5, $6, $7,$8,$9,$10)");
        dbConn->prepare("insert_article", 
            "INSERT INTO \"articles\"(id, type, name,price,image) "
            "VALUES ($1, $2, $3, $4, $5)");
        dbConn->prepare("insert_gym_article", 
            "INSERT INTO \"gymArticles\"(id, \"gymId\", amount,imports) "
            "VALUES ($1, $2, $3,$4)");
        dbConn->prepare("insert_article_payment", 
            "INSERT INTO \"articlePayments\"(id,\"clientId\",\"articleId\", \"gymId\", amount,method,date) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7)");
    }
    catch(const std::exception& e){
        printf("[E] Error:\n%s\n",e.what());
    }
    
    
    return 0;
}