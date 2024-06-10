#include "lambda2ski.hpp"
#include <memory>
#include "lambda.hpp"
#include "ski.hpp"

std::shared_ptr<Lambda_SKI_expression> lambda2lambda_ski_expr(
    std::shared_ptr<Lambda_expression> expr) {
    switch (expr->get_term_kind()) {
        case lambda_term_kind::abstraction: {
            std::shared_ptr<Lambda_abstraction> abstraction =
                std::static_pointer_cast<Lambda_abstraction>(expr);
            return std::make_shared<Lambda_SKI_expression_abstraction>(
                abstraction->parameter, lambda2lambda_ski_expr(abstraction->body));
        }

        case lambda_term_kind::application: {
            std::shared_ptr<Lambda_application> application =
                std::static_pointer_cast<Lambda_application>(expr);
            return std::make_shared<Lambda_SKI_expression_application>(
                lambda2lambda_ski_expr(application->first),
                lambda2lambda_ski_expr(application->second));
        }

        case lambda_term_kind::variable: {
            std::shared_ptr<Lambda_variable> variable =
                std::static_pointer_cast<Lambda_variable>(expr);
            return std::make_shared<Lambda_SKI_expression_variable>(variable->name);
        }
    }
}

std::shared_ptr<Lambda_SKI_expression> __lambda2ski(
    const std::shared_ptr<Lambda_SKI_expression> expr) {
    switch (expr->get_kind()) {
        case lambda_ski_expression_kind::abstraction: {
            std::shared_ptr<Lambda_SKI_expression_abstraction> abstraction =
                std::static_pointer_cast<Lambda_SKI_expression_abstraction>(expr);
            std::shared_ptr<Lambda_SKI_expression> body = __lambda2ski(abstraction->body);

            if (body->get_kind() == lambda_ski_expression_kind::application) {
                std::shared_ptr<Lambda_SKI_expression_application> application =
                    std::static_pointer_cast<Lambda_SKI_expression_application>(body);
                return std::make_shared<Lambda_SKI_expression_application>(
                    std::make_shared<Lambda_SKI_expression_application>(
                        std::make_shared<Lambda_SKI_expression_S>(),
                        __lambda2ski(std::make_shared<Lambda_SKI_expression_abstraction>(
                            abstraction->parameter, application->first))),
                    __lambda2ski(std::make_shared<Lambda_SKI_expression_abstraction>(
                        abstraction->parameter, application->second)));
            } else {
                if (body->contains_var(abstraction->parameter)) {
                    return std::make_shared<Lambda_SKI_expression_I>();
                } else {
                    return std::make_shared<Lambda_SKI_expression_application>(
                        std::make_shared<Lambda_SKI_expression_K>(), body);
                }
            }
        }
        case lambda_ski_expression_kind::application: {
            std::shared_ptr<Lambda_SKI_expression_application> application =
                std::static_pointer_cast<Lambda_SKI_expression_application>(expr);
            return std::make_shared<Lambda_SKI_expression_application>(
                __lambda2ski(application->first), __lambda2ski(application->second));
        }
        default: {
            return expr;
        }
    }
}

std::shared_ptr<SKI_expression> lambda_ski_expr2ski(
    const std::shared_ptr<Lambda_SKI_expression> expr) {
    switch (expr->get_kind()) {
        case lambda_ski_expression_kind::s: {
            return std::make_shared<SKI_S>();
        }
        case lambda_ski_expression_kind::k: {
            return std::make_shared<SKI_K>();
        }
        case lambda_ski_expression_kind::i: {
            return std::make_shared<SKI_I>();
        }
        case lambda_ski_expression_kind::application: {
            std::shared_ptr<Lambda_SKI_expression_application> application =
                std::static_pointer_cast<Lambda_SKI_expression_application>(expr);
            return std::make_shared<SKI_application>(
                lambda_ski_expr2ski(application->first), lambda_ski_expr2ski(application->second));
        }
        case lambda_ski_expression_kind::variable: {
            throw "error";
        }
        case lambda_ski_expression_kind::abstraction: {
            throw "error";
        }
    }
}

std::shared_ptr<SKI_expression> lambda2ski(const std::shared_ptr<Lambda_expression> expr) {
    return lambda_ski_expr2ski(__lambda2ski(lambda2lambda_ski_expr(expr)));
}

// Lambda_SKI_expression_abstraction

Lambda_SKI_expression_abstraction::Lambda_SKI_expression_abstraction(char const& parameter,
    std::shared_ptr<Lambda_SKI_expression> body) {
    this->parameter = parameter;
    this->body = body;
}

lambda_ski_expression_kind Lambda_SKI_expression_abstraction::get_kind() {
    return lambda_ski_expression_kind::abstraction;
}

bool Lambda_SKI_expression_abstraction::contains_var(char const& var) {
    if (var == parameter) {
        return false;
    } else {
        return body->contains_var(var);
    }
}

// Lambda_SKI_expression_application

Lambda_SKI_expression_application::Lambda_SKI_expression_application(
    std::shared_ptr<Lambda_SKI_expression> first,
    std::shared_ptr<Lambda_SKI_expression> second) {
    this->first = first;
    this->second = second;
}

lambda_ski_expression_kind Lambda_SKI_expression_application::get_kind() {
    return lambda_ski_expression_kind::application;
}

bool Lambda_SKI_expression_application::contains_var(char const& var) {
    return this->first->contains_var(var) or this->second->contains_var(var);
}

// Lambda_SKI_expression_variable

Lambda_SKI_expression_variable::Lambda_SKI_expression_variable(char const& name) {
    this->name = name;
}

lambda_ski_expression_kind Lambda_SKI_expression_variable::get_kind() {
    return lambda_ski_expression_kind::variable;
}

bool Lambda_SKI_expression_variable::contains_var(char const& var) {
    return this->name == var;
}

// Lambda_SKI_expression_S

lambda_ski_expression_kind Lambda_SKI_expression_S::get_kind() {
    return lambda_ski_expression_kind::s;
}

bool Lambda_SKI_expression_S::contains_var(char const& var) {
    return false;
}

// Lambda_SKI_expression_K

lambda_ski_expression_kind Lambda_SKI_expression_K::get_kind() {
    return lambda_ski_expression_kind::k;
}

bool Lambda_SKI_expression_K::contains_var(char const& var) {
    return false;
}

// Lambda_SKI_expression_I

lambda_ski_expression_kind Lambda_SKI_expression_I::get_kind() {
    return lambda_ski_expression_kind::i;
}

bool Lambda_SKI_expression_I::contains_var(char const& var) {
    return false;
}
