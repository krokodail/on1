#include <gtest/gtest.h>
#include </usr/src/googletest/googlemock/include/gmock/gmock.h>
#include <string>


class DBConnection {
public:
    virtual ~DBConnection() = default;
    virtual bool open(const std::string& connectionString) = 0;
    virtual bool close() = 0;
    virtual int execQuery(const std::string& query) = 0;
};


class MockDBConnection : public DBConnection {
public:
    MOCK_METHOD(bool, open, (const std::string& connectionString), (override));
    MOCK_METHOD(bool, close, (), (override));
    MOCK_METHOD(int, execQuery, (const std::string& query), (override));
};


class ClassThatUsesDB {
public:
    ClassThatUsesDB(DBConnection* dbConnection) : dbConnection(dbConnection) {}

    bool openConnection(const std::string& connectionString) {
        return dbConnection->open(connectionString);
    }

    int useConnection(const std::string& query) {
        return dbConnection->execQuery(query);
    }

    bool closeConnection() {
        return dbConnection->close();
    }

private:
    DBConnection* dbConnection;
};

class ClassThatUsesDBTest : public testing::Test {
protected:
    MockDBConnection mockDBConnection;
    ClassThatUsesDB classUnderTest{&mockDBConnection};
};

TEST_F(ClassThatUsesDBTest, OpenConnectionTest) {
    const std::string connectionString = "connection_string";
    EXPECT_CALL(mockDBConnection, open(connectionString))
        .WillOnce(testing::Return(true));

    bool result = classUnderTest.openConnection(connectionString);
    ASSERT_TRUE(result);
}

TEST_F(ClassThatUsesDBTest, UseConnectionTest) {
    const std::string query = "SELECT * FROM users";
    EXPECT_CALL(mockDBConnection, execQuery(query))
        .WillOnce(testing::Return(42));

    int result = classUnderTest.useConnection(query);
    ASSERT_EQ(result, 42);
}

TEST_F(ClassThatUsesDBTest, CloseConnectionTest) {
    EXPECT_CALL(mockDBConnection, close())
        .WillOnce(testing::Return(true));

    bool result = classUnderTest.closeConnection();
    ASSERT_TRUE(result);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

