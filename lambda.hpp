#ifndef LAMBDA_HPP
#define LAMBDA_HPP

#include <memory>
#include <string>

enum class lambda_term_kind {abstraction, application, variable};

class Lambda_expression {
public:
    virtual std::shared_ptr<Lambda_expression> clone() = 0;
    virtual lambda_term_kind get_term_kind() = 0;
    virtual std::string to_string() = 0;
    // virtual void reduce() = 0;
    // virtual void replace(const char &var, std::shared_ptr<Lambda_expression> replacement) = 0;
    // virtual bool contains_var(const char &varvar) = 0;
};

class Lambda_abstraction : public Lambda_expression {
public:
    char parameter;
    std::shared_ptr<Lambda_expression> body;

    Lambda_abstraction(const char &parameter, std::shared_ptr<Lambda_expression> body);
    ~Lambda_abstraction() = default;
    std::shared_ptr<Lambda_expression> clone() override;
    lambda_term_kind get_term_kind() override;
    std::string to_string() override;
    // void reduce() override;
    // void replace(const char &var, std::shared_ptr<Lambda_expression> replacement) override;
    // bool contains_var(const char &var) override;
};

class Lambda_application : public Lambda_expression {
public:
    std::shared_ptr<Lambda_expression> first;
    std::shared_ptr<Lambda_expression> second;

    Lambda_application(std::shared_ptr<Lambda_expression> first, std::shared_ptr<Lambda_expression> second);
    ~Lambda_application() = default;
    std::shared_ptr<Lambda_expression> clone() override;
    lambda_term_kind get_term_kind() override;
    std::string to_string() override;
    // void reduce() override;
    // void replace(const char &var, std::shared_ptr<Lambda_expression> replacement) override;
    // bool contains_var(const char &var) override;
};

class Lambda_variable : public Lambda_expression {
public:
    char name;

    Lambda_variable(const char &name);
    ~Lambda_variable() = default;
    std::shared_ptr<Lambda_expression> clone() override;
    lambda_term_kind get_term_kind() override;
    std::string to_string() override;
    // void reduce() override;
    // void replace(const char &var, std::shared_ptr<Lambda_expression> replacement) override;
    // bool contains_var(const char &var) override;
};

#endif