//g++ main.cpp -lgmock -lgtest -lgtest_main -pthread


#include "gtest/gtest.h"
#include </usr/src/googletest/googlemock/include/gmock/gmock.h>
#include <string>



class DBConnection {
public:
    virtual ~DBConnection() = default;
    virtual int executeQuery(const std::string& query) = 0;
};


class ClassThatUsesDB {
public:
    ClassThatUsesDB(DBConnection* dbConnection) : dbConnection(dbConnection) {}

    int performDatabaseOperation(const std::string& query) {
        return dbConnection->executeQuery(query);
    }

private:
    DBConnection* dbConnection;
};


class MockDBConnection : public DBConnection {
public:
    MOCK_METHOD(int, executeQuery, (const std::string& query), (override));
};

class ClassThatUsesDBTest : public testing::Test {
protected:
    MockDBConnection mockDBConnection;
    ClassThatUsesDB classUnderTest{&mockDBConnection};
};


TEST_F(ClassThatUsesDBTest, PerformDatabaseOperationTest) {
    const std::string query = "SELECT * FROM users";
  
    EXPECT_CALL(mockDBConnection, executeQuery(query))
        .WillOnce(testing::Return(42));

    int result = classUnderTest.performDatabaseOperation(query);
    ASSERT_EQ(result, 42);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
