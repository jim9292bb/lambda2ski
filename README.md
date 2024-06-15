# lambda2ski

A tool to convert Lambda calculus to SKI combinators calculus.

## Prerequisites

- [Boost C++ Libraries](https://www.boost.org/) (Boost.Program_options)
- g++ compiler

## Compilation

```bash
g++ -std=c++20 lambda.cpp test.cpp ski.cpp lambda2ski.cpp lambda_parser.cpp -lboost_program_options -o test
```

## Usage

```bash
./test --input-file input.txt --output-file output.txt
```

### Options

- `--input-file, -i`: Input file with Lambda calculus (required)
- `--output-file, -o`: Output file for SKI combinators calculus (required)
- `--help, -h`: Show help message

## Example

```bash
./test --input-file input.txt --output-file output.txt
```

## Parser Grammar

For detailed information about the grammar and parsing rules used in this project, see the [lambda_parser.md](lambda_parser.md) file.