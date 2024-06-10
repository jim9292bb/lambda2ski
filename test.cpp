#include <fstream>
#include <memory>
#include <sstream>
#include "lambda.hpp"
#include "lambda2ski.hpp"
#include "lambda_parser.hpp"


int main() {
    std::string input_file_name = "input.lc";
    std::string output_file_name = "output.lc";
    std::ifstream input_file(input_file_name);
    std::string s = "";
    if (input_file.is_open()) {
        std::stringstream buffer;
        buffer << input_file.rdbuf();
        s = buffer.str();
    }

    std::shared_ptr<Lambda_expression> expr = parse(s);
    std::ofstream output_file(output_file_name);
    auto ski_str = lambda2ski(expr)->to_string();
    output_file.write(ski_str.c_str(), ski_str.length());
    input_file.close();
    output_file.close();
    return 0;
}