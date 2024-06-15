#include <boost/program_options/errors.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include "lambda.hpp"
#include "lambda2ski.hpp"
#include "lambda_parser.hpp"
#include <boost/program_options.hpp>
#include <string>

namespace program_options = boost::program_options;


int main(int argc, char* argv[]) {
    std::string input_file_path;
    std::string output_file_path;
    program_options::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("input-file,i", program_options::value<std::string>(&input_file_path)->required(), "input file")
    ("output-file,o", program_options::value<std::string>(&output_file_path)->required(), "output file");

    program_options::variables_map options;

    try {
        program_options::store(program_options::parse_command_line(argc, argv, desc), options);

        if (options.count("help")) {
            std::cout << desc << std::endl;
            return 0;
        }

        options.notify();
    } catch (program_options::error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << desc << std::endl;
        return 1;
    }


    std::ifstream input_file(input_file_path);
    std::ofstream output_file(output_file_path);
    std::string lambda_code = "";
    if (input_file.is_open()) {
        std::stringstream buffer;
        buffer << input_file.rdbuf();
        lambda_code = buffer.str();
    }

    std::shared_ptr<Lambda_expression> lambda_expr = parse(lambda_code);
    auto ski_str = lambda2ski(lambda_expr)->to_string();
    output_file.write(ski_str.c_str(), ski_str.length());
    input_file.close();
    output_file.close();
    return 0;
}
