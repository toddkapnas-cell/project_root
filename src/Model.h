#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <map>
#include <set>
#include <random>

class Model {
private:
    int k;
    double alpha;
    std::set<char> alphabet;
    std::map<std::string, int> kgram_counts;                  // n(w)
    std::map<std::string, std::map<char, int>> transition_counts; // n(w, c)

    // Helper to calculate P(c|w) with pseudocounts
    double get_probability(const std::string& w, char c) const;

public:
    // Constructor accepting k-gram size and pseudocount alpha
    Model(int k, double alpha);

    // Train the model using the input text string
    void train(const std::string& text);

    // Generate new text of a given length using a random number generator
    std::string generate(int length, std::mt19937& rng) const;
};

#endif // MODEL_H