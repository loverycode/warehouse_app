#include <gtest/gtest.h>
#include "core/User.h"

class UserTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        user = new User(1, "john", "pass123", "John Doe", Role::Employee);
    }
    void TearDown() override { delete user; }
    User *user;
};

TEST_F(UserTest, ConstructorAndGetters)
{
    EXPECT_EQ(user->getId(), 1);
    EXPECT_EQ(user->getLogin(), "john");
    EXPECT_EQ(user->getName(), "John Doe");
}

TEST_F(UserTest, LoginSuccess)
{
    EXPECT_TRUE(user->login("john", "pass123"));
}

TEST_F(UserTest, LoginWrongPassword)
{
    EXPECT_FALSE(user->login("john", "wrong"));
}

TEST_F(UserTest, LoginEmptyCredentials)
{
    EXPECT_FALSE(user->login("", ""));
    EXPECT_FALSE(user->login("john", ""));
    EXPECT_FALSE(user->login("", "pass123"));
}

TEST_F(UserTest, CaseSensitiveLogin)
{
    EXPECT_FALSE(user->login("JOHN", "pass123"));
    EXPECT_FALSE(user->login("john", "PASS123"));
}