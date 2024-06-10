#ifndef SKI_HPP
#define SKI_HPP

#include <memory>


enum class ski_term_kind {s, k, i, application};


class SKI_expression {
public:
    virtual std::string to_string() = 0;
    virtual std::shared_ptr<SKI_expression> clone() = 0;
    virtual ski_term_kind get_term_kind() = 0;
};

class SKI_S: public SKI_expression {
public:
    SKI_S() = default;
    ~SKI_S() = default;
    std::string to_string() override;
    std::shared_ptr<SKI_expression> clone() override;
    ski_term_kind get_term_kind() override;
};

class SKI_K: public SKI_expression {
public:
    SKI_K() = default;
    ~SKI_K() = default;
    std::string to_string() override;
    std::shared_ptr<SKI_expression> clone() override;
    ski_term_kind get_term_kind() override;
};

class SKI_I: public SKI_expression {
public:
    SKI_I() = default;
    ~SKI_I() = default;
    std::string to_string() override;
    std::shared_ptr<SKI_expression> clone() override;
    ski_term_kind get_term_kind() override;
};

class SKI_application: public SKI_expression {
public:
    std::shared_ptr<SKI_expression> first;
    std::shared_ptr<SKI_expression> second;

    SKI_application(std::shared_ptr<SKI_expression> first, std::shared_ptr<SKI_expression> second);
    ~SKI_application() = default;
    std::string to_string() override;
    std::shared_ptr<SKI_expression> clone() override;
    ski_term_kind get_term_kind() override;
};

#endif