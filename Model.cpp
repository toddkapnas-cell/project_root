#include "Model.h"
#include <stdexcept>
#include <vector>
#include <algorithm>

Model::Model(int k_val, double alpha_val) : k(k_val), alpha(alpha_val) {
    if (k <= 0) {
        throw std::invalid_argument("k must be a positive integer.");
    }
    if (alpha < 0.0) {
        throw std::invalid_argument("Alpha must be non-negative.");
    }
}

void Model::train(const std::string& text) {
    // Check if training text is too short to contain any k-gram followed by a character
    if (text.length() <= static_cast<size_t>(k)) {
        throw std::runtime_error("Training text is too short to contain any k-gram followed by a character.");
    }

    // Populate alphabet and counts
    for (size_t i = 0; i < text.length(); ++i) {
        alphabet.insert(text[i]);
        
        if (i + k < text.length()) {
            std::string w = text.substr(i, k);
            char c = text[i + k];
            kgram_counts[w]++;
            transition_counts[w][c]++;
        }
    }

    if (kgram_counts.empty()) {
        throw std::runtime_error("Training text did not yield any valid k-grams.");
    }
}

double Model::get_probability(const std::string& w, char c) const {
    double n_wc = 0.0;
    auto w_it = transition_counts.find(w);
    if (w_it != transition_counts.end()) {
        auto c_it = w_it->second.find(c);
        if (c_it != w_it->second.end()) {
            n_wc = c_it->second;
        }
    }

    double n_w = 0.0;
    auto kgram_it = kgram_counts.find(w);
    if (kgram_it != kgram_counts.end()) {
        n_w = kgram_it->second;
    }

    double sigma_size = static_cast<double>(alphabet.size());
    return (n_wc + alpha) / (n_w + alpha * sigma_size);
}

std::string Model::generate(int length, std::mt19937& rng) const {
    if (length < k) {
        throw std::invalid_argument("Requested output length cannot be smaller than k.");
    }

    // Step 1: Choose a starting k-gram according to k-gram frequencies
    std::vector<std::string> kgrams;
    std::vector<double> weights;
    for (const auto& pair : kgram_counts) {
        kgrams.push_back(pair.first);
        weights.push_back(pair.second);
    }

    std::discrete_distribution<size_t> kgram_dist(weights.begin(), weights.end());
    std::string current_w = kgrams[kgram_dist(rng)];

    std::string result = current_w;

    // Step 3-6: Generate subsequent characters
    while (static_cast<int>(result.length()) < length) {
        // If alpha == 0 and w has no transitions, stop early
        if (alpha == 0.0 && transition_counts.find(current_w) == transition_counts.end()) {
            break;
        }

        // Build probability distribution for the next character over the alphabet
        std::vector<char> alpha_chars(alphabet.begin(), alphabet.end());
        std::vector<double> char_weights;
        for (char c : alpha_chars) {
            char_weights.push_back(get_probability(current_w, c));
        }

        std::discrete_distribution<size_t> char_dist(char_weights.begin(), char_weights.end());
        char next_c = alpha_chars[char_dist(rng)];

        result += next_c;

        // Update current w: remove first character, append sampled character
        current_w = current_w.substr(1) + next_c;
    }

    return result;
}