#include <memory>
#include <string>
#include "lambda.hpp"
#include <format>


// Lambda_abstraction
Lambda_abstraction::Lambda_abstraction(const char &parameter, std::shared_ptr<Lambda_expression> body) {
    this->parameter = parameter;
    this->body = body;
}

lambda_term_kind Lambda_abstraction::get_term_kind() {
    return lambda_term_kind::abstraction;
}

std::string Lambda_abstraction::to_string() {
    return std::format("λ{}. {}", this->parameter, this->body->to_string());
}

std::shared_ptr<Lambda_expression> Lambda_abstraction::clone() {
    return std::make_shared<Lambda_abstraction>(this->parameter, this->body->clone());
}


// Lambda_application

Lambda_application::Lambda_application(std::shared_ptr<Lambda_expression> first, std::shared_ptr<Lambda_expression> second) {
    this->first = first;
    this->second = second;
}

lambda_term_kind Lambda_application::get_term_kind() {
    return lambda_term_kind::application;
}

std::string Lambda_application::to_string() {
    if (this->first->get_term_kind() == lambda_term_kind::abstraction) {
    
        if (this->second->get_term_kind() == lambda_term_kind::variable) {
            return std::format("({}) {}", this->first->to_string(), this->second->to_string());
        }
        else {
            return std::format("({}) ({})", this->first->to_string(), this->second->to_string());
        }
    }
    else {
        if (this->second->get_term_kind() == lambda_term_kind::variable) {
            return std::format("{} {}", this->first->to_string(), this->second->to_string());
        }
        else {
            return std::format("{} ({})", this->first->to_string(), this->second->to_string());
        }
    }
}

std::shared_ptr<Lambda_expression> Lambda_application::clone() {
    return std::make_shared<Lambda_application>(this->first, this->second);
}

// Lambda_variable

Lambda_variable::Lambda_variable(const char &name) {
    this->name = name;
}

lambda_term_kind Lambda_variable::get_term_kind() {
    return lambda_term_kind::variable;
}

std::string Lambda_variable::to_string() {
    return std::string(1, this->name);
}

std::shared_ptr<Lambda_expression> Lambda_variable::clone() {
    return std::make_shared<Lambda_variable>(this->name);
}
