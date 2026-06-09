#ifndef USER_H
#define USER_H

#include <string>

enum class Role
{
    Employee,
    Admin
};

class User
{
protected:
    int id;
    std::string login_;
    std::string password_;
    std::string name_;
    Role role_;

public:
    User(int id, const std::string &login, const std::string &password, const std::string &name, Role role);
    virtual ~User() = default;

    virtual bool login(const std::string &login, const std::string &password);

    int getId() const { return id; }
    std::string getName() const { return name_; }
    std::string getLogin() const { return login_; }
    Role getRole() const { return role_; } // ← только один раз!

    virtual bool canManageProducts() const { return false; }
    virtual bool canFixStock() const { return false; }
};

#endif