#include "StringCalculator.h"
#include <sstream>
#include <algorithm>
#include <numeric>
#include <regex>

// Adds numbers from the input string
int StringCalculator::add(const std::string& numbers) {
    if (numbers.empty()) 
        return 0;
    std::string delimiter = ",|\n";
    std::string input = numbers;
    
    // Handle custom delimiters
    if (numbers.substr(0, 2) == "//") {
        delimiter = getCustomDelimiter(numbers);
        input = getNumberString(numbers);
    }
    std::vector<int> nums = parseNumbers(input, delimiter);
    validateNumbers(nums);
    nums = filterLargeNumbers(nums);
    return sumNumbers(nums);
}

// Get custom delimiter from input
std::string StringCalculator::getCustomDelimiter(const std::string& input) {
    std::regex customDelimRegex("//(\\[.*?\\])+\n");
    std::smatch match;
    if (std::regex_search(input, match, customDelimRegex)) {
        std::string customDelimiters = match.str();
        customDelimiters.erase(0, 2);  // Remove the "//"
        customDelimiters.pop_back();   // Remove the "\n"
        std::regex escapeRegex(R"([\[|\]])");
        return std::regex_replace(customDelimiters, escapeRegex, "");
    }
    return input.substr(2, input.find("\n") - 2);
}

// Extract the part of the string with numbers, excluding the delimiter line
std::string StringCalculator::getNumberString(const std::string& input) {
    return input.substr(input.find("\n") + 1);
}

// Converts string to an integer
int StringCalculator::toInt(const std::string& number) {
    return std::stoi(number);
}

// Parses input string into vector of integers
std::vector<int> StringCalculator::parseNumbers(const std::string& numbers, const std::string& delimiter) {
    std::vector<int> result;
    std::regex delimRegex(delimiter);
    std::sregex_token_iterator iter(numbers.begin(), numbers.end(), delimRegex, -1);
    std::sregex_token_iterator end;

    while (iter != end) {
        std::string token = *iter++;
        if (!token.empty()) {
            result.push_back(toInt(token));
        }
    }
    return result;
}

// Validates numbers and checks for negatives
void StringCalculator::validateNumbers(const std::vector<int>& numbers) {
    std::vector<int> negatives = findNegativeNumbers(numbers);
    throwIfNegatives(negatives);
}

// Finds all negative numbers in the list
std::vector<int> StringCalculator::findNegativeNumbers(const std::vector<int>& numbers) {
    std::vector<int> negatives;
    for (int num : numbers) {
        if (num < 0) {
            negatives.push_back(num);
        }
    }
    return negatives;
}

// Throws an exception if there are negative numbers
void StringCalculator::throwIfNegatives(const std::vector<int>& negatives) {
    if (!negatives.empty()) {
        std::string errorMsg = "negatives not allowed: ";
        for (int neg : negatives) {
            errorMsg += std::to_string(neg) + " ";
        }
        throw std::runtime_error(errorMsg);
    }
}

// Sums up numbers
int StringCalculator::sumNumbers(const std::vector<int>& numbers) {
    return std::accumulate(numbers.begin(), numbers.end(), 0);
}

// Filters out numbers greater than 1000
std::vector<int> StringCalculator::filterLargeNumbers(const std::vector<int>& numbers) {
    std::vector<int> filtered;
    std::copy_if(numbers.begin(), numbers.end(), std::back_inserter(filtered), [](int n){ return n <= 1000; });
    return filtered;
}
