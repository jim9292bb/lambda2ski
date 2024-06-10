#ifndef LAMBDA2SKI_HPP
#define LAMBDA2SKI_HPP

#include "lambda.hpp"
#include "ski.hpp"
#include <memory>


enum class lambda_ski_expression_kind {abstraction, application, variable, s, k, i};

class Lambda_SKI_expression {
public:
    virtual lambda_ski_expression_kind get_kind() = 0;
    virtual bool contains_var(const char &var) = 0;
};

class Lambda_SKI_expression_abstraction : public Lambda_SKI_expression {
public:
    char parameter;
    std::shared_ptr<Lambda_SKI_expression> body;

    Lambda_SKI_expression_abstraction(const char &parameter, std::shared_ptr<Lambda_SKI_expression> body);
    ~Lambda_SKI_expression_abstraction() = default;
    lambda_ski_expression_kind get_kind();
    bool contains_var(const char &var);
};

class Lambda_SKI_expression_application : public Lambda_SKI_expression {
public:
    std::shared_ptr<Lambda_SKI_expression> first;
    std::shared_ptr<Lambda_SKI_expression> second;

    Lambda_SKI_expression_application(std::shared_ptr<Lambda_SKI_expression> first, std::shared_ptr<Lambda_SKI_expression> second);
    ~Lambda_SKI_expression_application() = default;
    lambda_ski_expression_kind get_kind();
    bool contains_var(const char &var);
};

class Lambda_SKI_expression_variable : public Lambda_SKI_expression {
public:
    char name;

    Lambda_SKI_expression_variable(const char &name);
    ~Lambda_SKI_expression_variable() = default;
    lambda_ski_expression_kind get_kind();
    bool contains_var(const char &var);
};

class Lambda_SKI_expression_S : public Lambda_SKI_expression {
public:
    Lambda_SKI_expression_S() = default;
    ~Lambda_SKI_expression_S() = default;
    lambda_ski_expression_kind get_kind();
    bool contains_var(const char &var);
};

class Lambda_SKI_expression_K : public Lambda_SKI_expression {
public:
    Lambda_SKI_expression_K() = default;
    ~Lambda_SKI_expression_K() = default;
    lambda_ski_expression_kind get_kind();
    bool contains_var(const char &var);
};

class Lambda_SKI_expression_I : public Lambda_SKI_expression {
public:
    Lambda_SKI_expression_I() = default;
    ~Lambda_SKI_expression_I() = default;
    lambda_ski_expression_kind get_kind();
    bool contains_var(const char &var);
};

std::shared_ptr<SKI_expression> lambda2ski(std::shared_ptr<Lambda_expression> expr);

#endif