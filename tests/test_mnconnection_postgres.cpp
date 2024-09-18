#include "../mnlibrary/mnconnection_postgres.h"

void TEST(MnConnectionPostgresTest, ConnectTest) {
    // Test the connect method
    mnconnection_postgres conn("test_db", "localhost", 5432, "test_user", "test_password");
    bool connected = conn.connect();
    EXPECT_TRUE(connected);
}

// Add more tests as needed
