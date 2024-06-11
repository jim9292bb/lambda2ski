#include "lambda_parser.hpp"
#include <array>
#include <cctype>
#include <cstddef>
#include <iterator>
#include <list>
#include <locale>
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <utility>
#include <variant>
#include "lambda.hpp"

static const std::locale loc("C");
static const std::array<std::array<signed int, 6>, 5> ll1_parsing_table = {{{0, 0, 1, -1, -1, -1},
    {2, 2, -1, -1, -1, 3},
    {4, 5, -1, -1, -1, -1},
    {7, 7, 6, -1, -1, -1},
    {8, 8, -1, -1, 9, -1}}};

enum class terminal_kind { left_round_brackets = 0, id, lambda, dot, right_round_brackets, end };
enum class non_terminal_kind { expr = 0, expr_tail, term, expr2, expr2_tail };
enum class action_kind { a0, a1, a2, a3, a4, a5, a6, a7, a8, a9 };
static const std::array<const std::list<std::variant<terminal_kind, non_terminal_kind>>, 10>
    production_rules = {{{non_terminal_kind::term, non_terminal_kind::expr_tail},
        {terminal_kind::lambda, terminal_kind::id, terminal_kind::dot, non_terminal_kind::expr},
        {non_terminal_kind::term, non_terminal_kind::expr_tail},
        {terminal_kind::end},
        {terminal_kind::left_round_brackets, non_terminal_kind::expr2},
        {terminal_kind::id},
        {terminal_kind::lambda, terminal_kind::id, terminal_kind::dot, non_terminal_kind::expr2},
        {non_terminal_kind::term, non_terminal_kind::expr2_tail},
        {non_terminal_kind::term, non_terminal_kind::expr2_tail},
        {terminal_kind::right_round_brackets}}};

struct Token {
    terminal_kind kind;
    size_t index;
};

struct expr_pair {
    expr_pair(std::shared_ptr<Lambda_expression> first,
        std::optional<std::shared_ptr<expr_pair>> second) {
        this->pair = std::make_pair(first, second);
    }
    std::pair<std::shared_ptr<Lambda_expression>, std::optional<std::shared_ptr<expr_pair>>> pair;
};

struct Lambda_AST_node {
    Lambda_AST_node(
        std::variant<terminal_kind, non_terminal_kind> value,
        int rules_index) {
        this->symbol = value;
        this->rules_index = rules_index;
        this->children = {};
        this->property = {};
    }
    int rules_index;
    std::variant<terminal_kind, non_terminal_kind> symbol;
    std::optional<std::shared_ptr<expr_pair>> property;
    std::list<std::shared_ptr<Lambda_AST_node>> children;
};

std::shared_ptr<Lambda_expression> expr_merge(std::shared_ptr<Lambda_expression> expr1,
    std::optional<std::shared_ptr<expr_pair>> expr2) {
    if (!expr2.has_value()) {
        return expr1;
    } else {
        return expr_merge(std::make_shared<Lambda_application>(expr1, expr2.value()->pair.first),
            expr2.value()->pair.second);
    }
}

void ast_evaluation(std::shared_ptr<Lambda_AST_node> ast) {
    for (auto child : ast->children) {
        ast_evaluation(child);
    }
    switch (ast->rules_index) {
        case 0: {
            ast->property = std::make_shared<expr_pair>(
                expr_merge(ast->children.front()->property.value()->pair.first,
                    ast->children.back()->property),
                std::nullopt);
            break;
        }
        case 1: {
            ast->property = std::make_shared<expr_pair>(
                std::make_shared<Lambda_abstraction>(
                    (std::static_pointer_cast<Lambda_variable>(std::next(ast->children.begin())
                                                                   ->get()
                                                                   ->property.value()
                                                                   ->pair.first))
                        ->name,
                    ast->children.back()->property.value()->pair.first),
                std::nullopt);
            break;
        }
        case 2: {
            ast->property =
                std::make_shared<expr_pair>(ast->children.front()->property.value()->pair.first,
                    ast->children.back()->property);
            break;
        }
        case 3: {
            ast->property = std::nullopt;
            break;
        }
        case 4: {
            ast->property = std::make_shared<expr_pair>(
                ast->children.back()->property.value()->pair.first, std::nullopt);
            break;
        }
        case 5: {
            ast->property = std::make_shared<expr_pair>(
                ast->children.front()->property.value()->pair.first, std::nullopt);
            break;
        }
        case 6: {
            ast->property = std::make_shared<expr_pair>(
                std::make_shared<Lambda_abstraction>(
                    (std::static_pointer_cast<Lambda_variable>(
                         std::next(ast->children.begin())->get()->property.value()->pair.first))
                        ->name,
                    ast->children.back()->property.value()->pair.first),
                std::nullopt);
            break;
        }
        case 7: {
            ast->property = std::make_shared<expr_pair>(
                expr_merge(ast->children.front()->property.value()->pair.first,
                    ast->children.back()->property),
                std::nullopt);
            break;
        }
        case 8: {
            ast->property =
                std::make_shared<expr_pair>(ast->children.front()->property.value()->pair.first,
                    ast->children.back()->property);
            break;
        }
        case 9: {
            ast->property = std::nullopt;
            break;
        }
    }
}

std::shared_ptr<Lambda_expression> ast2lambda(std::shared_ptr<Lambda_AST_node> ast) {
    ast_evaluation(ast);
    return ast->property.value()->pair.first;
}

std::shared_ptr<Lambda_expression> parse(const std::string& expression) {
    std::list<Token> token_list{};
    size_t index = 0;
    for (auto iter = expression.cbegin(); iter != expression.cend(); iter++) {
        switch (*iter) {
            case '\\': {
                token_list.push_back(Token{.kind = terminal_kind::lambda, .index = 0});
                break;
            }
            case '.': {
                token_list.push_back(Token{.kind = terminal_kind::dot, .index = 0});
                break;
            }
            case '(': {
                token_list.push_back(Token{.kind = terminal_kind::left_round_brackets, .index = 0});
                break;
            }
            case ')': {
                token_list.push_back(
                    Token{.kind = terminal_kind::right_round_brackets, .index = 0});
                break;
            }
            default: {
                if (std::islower(*iter, loc)) {
                    token_list.push_back(Token{.kind = terminal_kind::id, .index = index});
                }
                break;
            }
        }
        index++;
    }
    token_list.push_back(Token{.kind = terminal_kind::end, .index = 0});
    std::shared_ptr<Lambda_AST_node> ast_root =
        std::make_shared<Lambda_AST_node>(non_terminal_kind::expr, 0);
    std::stack<std::shared_ptr<Lambda_AST_node>> ast_node_stack{
        {std::make_shared<Lambda_AST_node>(terminal_kind::end, -1), ast_root}};

    auto token = token_list.cbegin();
    while (!ast_node_stack.empty() and token != token_list.cend()) {
        auto current_node = ast_node_stack.top();
        if (std::holds_alternative<non_terminal_kind>(current_node->symbol)) {
            int rules_index = ll1_parsing_table[static_cast<int>(
                std::get<non_terminal_kind>(current_node->symbol))][static_cast<int>(token->kind)];
            if (rules_index != -1) {
                ast_node_stack.pop();
                current_node->rules_index = rules_index;
                for (auto& symbol : production_rules[rules_index]) {
                    if (std::holds_alternative<non_terminal_kind>(symbol)) {
                        auto node = std::make_shared<Lambda_AST_node>(
                            std::get<non_terminal_kind>(symbol), -1);
                        current_node->children.push_back(node);

                    } else {
                        auto node =
                            std::make_shared<Lambda_AST_node>(std::get<terminal_kind>(symbol), -1);
                        current_node->children.push_back(node);
                    }
                }
                for (auto iter = current_node->children.crbegin();
                     iter != current_node->children.crend();
                     iter++) {
                    ast_node_stack.push(*iter);
                }
            } else {
                throw "non_terminal syntax error";
            }
        } else if (std::holds_alternative<terminal_kind>(current_node->symbol)) {
            if (token->kind == std::get<terminal_kind>(current_node->symbol)) {
                if (token->kind == terminal_kind::id) {
                    current_node->property = std::make_shared<expr_pair>(
                        std::make_shared<Lambda_variable>(expression.at(token->index)),
                        std::nullopt);
                }
                ast_node_stack.pop();
                token++;
            } else {
                throw "terminal syntax error";
            }
        }
    }

    return ast2lambda(ast_root);
}