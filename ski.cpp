#include "ski.hpp"
#include <format>
#include <memory>
#include <string>


// SKI_S

std::string SKI_S::to_string() {
    return std::string(1, 'S');
}

ski_term_kind SKI_S::get_term_kind() {
    return ski_term_kind::s;
}

std::shared_ptr<SKI_expression> SKI_S::clone() {
    return std::make_shared<SKI_S>();
}

// SKI_K

std::string SKI_K::to_string() {
    return std::string(1, 'K');
}

ski_term_kind SKI_K::get_term_kind() {
    return ski_term_kind::k;
}

std::shared_ptr<SKI_expression> SKI_K::clone() {
    return std::make_shared<SKI_K>();
}

// SKI_I

std::string SKI_I::to_string() {
    return std::string(1, 'I');
}

ski_term_kind SKI_I::get_term_kind() {
    return ski_term_kind::i;
}

std::shared_ptr<SKI_expression> SKI_I::clone() {
    return std::make_shared<SKI_I>();
}

// SKI_application

SKI_application::SKI_application(std::shared_ptr<SKI_expression> first, std::shared_ptr<SKI_expression> second) {
    this->first = first;
    this->second = second;
}

std::string SKI_application::to_string() {
    if (this->second->get_term_kind() == ski_term_kind::application) {
        return std::format("{} ({})", this->first->to_string(), this->second->to_string());
    }
    else {
        return std::format("{} {}", this->first->to_string(), this->second->to_string());
    }
}

ski_term_kind SKI_application::get_term_kind() {
    return ski_term_kind::application;
}

std::shared_ptr<SKI_expression> SKI_application::clone() {
    return std::make_shared<SKI_application>(first->clone(), second->clone());
}
