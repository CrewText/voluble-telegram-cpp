#ifndef DATABASE_EXCEPTIONS_H
#define DATABASE_EXCEPTIONS_H
#include <exception>
#include <sstream>
#include <string>

class UsersTableNotFound : public std::exception
{
    const char *what() const throw()
    {
        return "Users Table Not Found exception";
    }
};

class IntegrityCheckFailed : public std::exception
{
    const char *_data;

public:
    IntegrityCheckFailed()
    {
        _data = nullptr;
    };
    IntegrityCheckFailed(const char *data)
    {
        _data = data;
    }
    const char *what() const throw()
    {
        std::stringstream message;
        message << "Integrity Check Failed exception";
        if (_data != nullptr)
        {
            message << "; data from check: " << _data;
        }

        return message.str().c_str();
    }
};

class UserNotFoundInDB : public std::exception
{
    unsigned int _user_id;
    std::string _phone_number;

public:
    UserNotFoundInDB(const unsigned int user_id)
    {
        _user_id = user_id;
    }
    UserNotFoundInDB(const std::string phone_number)
    {
        _phone_number = phone_number;
    }
    const char *what() const throw()
    {
        std::stringstream message;
        if (_user_id)
        {
            message << "User " << _user_id << " not found in database";
        }
        else if (_phone_number.length())
        {
            message << "User with phone number " << _phone_number << " not found in database";
        }
        return message.str().c_str();
    }
};

#endif //DATABASE_EXCEPTIONS_H