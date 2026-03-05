#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class InsufficientStockException : public std::exception {
private:
    std::string message;
public:
    InsufficientStockException(const std::string& msg = "Insufficient stock available") : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidInputException : public std::exception {
private:
    std::string message;
public:
    InvalidInputException(const std::string& msg = "Invalid input provided") : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class FileOperationException : public std::exception {
private:
    std::string message;
public:
    FileOperationException(const std::string& msg = "File operation failed") : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif