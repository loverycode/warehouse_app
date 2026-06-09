#include "core/User.h"

User::User(int id, const std::string &login, const std::string &password, const std::string &name, Role role)
    : id(id), login_(login), password_(password), name_(name), role_(role) {}

bool User::login(const std::string &login, const std::string &password)
{
    return (login_ == login && password_ == password);
}