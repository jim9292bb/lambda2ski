#ifndef LAMBDA_PARSER_HPP
#define LAMBDA_PARSER_HPP
#include "lambda.hpp"
#include <memory>
#include <string>


std::shared_ptr<Lambda_expression> parse(const std::string &expression);
#endif