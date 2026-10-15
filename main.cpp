#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <stdexcept>
#include "Model.h"

int main(int argc, char* argv[]) {
    try {
        double alpha = 0.0;
        int arg_index = 1;

        // Check for optional -a flag
        if (argc > 1 && std::string(argv[1]) == "-a") {
            if (argc < 6) {
                throw std::invalid_argument("Option -a must be followed by a non-negative number and three arguments.");
            }
            try {
                alpha = std::stod(argv[2]);
            } catch (...) {
                throw std::invalid_argument("Option -a is not followed by a valid number.");
            }
            if (alpha < 0.0) {
                throw std::invalid_argument("Pseudocount alpha cannot be negative.");
            }
            arg_index = 3; // Shift argument parsing window past "-a" and its value
        }

        // We expect exactly 3 arguments after optional -a (total argc should be 4 or 6)
        if (argc - arg_index != 3) {
            throw std::invalid_argument("Wrong number of arguments. Usage: slm [-a alpha] k training_file output_length");
        }

        // Parse k, training file name, and output length
        int k = 0;
        int output_length = 0;
        try {
            k = std::stoi(argv[arg_index]);
            output_length = std::stoi(argv[arg_index + 2]);
        } catch (...) {
            throw std::invalid_argument("k and output length must be valid integers.");
        }

        if (k <= 0) {
            throw std::invalid_argument("k must be a positive integer.");
        }
        if (output_length <= 0) {
            throw std::invalid_argument("Output length must be a positive integer.");
        }
        if (output_length < k) {
            throw std::invalid_argument("Output length cannot be smaller than k.");
        }

        std::string filename = argv[arg_index + 1];

        // Open training file
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("slm: cannot open file '" + filename + "'");
        }

        // Read entire file into string
        std::ostringstream ss;
        ss << file.rdbuf();
        std::string training_text = ss.str();
        file.close();

        // Train model
        Model model(k, alpha);
        model.train(training_text);

        // Generate text using random device and generator
        std::random_device rd;
        std::mt19937 rng(rd());
        std::string generated_text = model.generate(output_length, rng);

        // Output result
        std::cout << generated_text << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}