#include "StringCalculator.h"
#include <sstream>
#include <algorithm>
#include <numeric>

int StringCalculator::add(const std::string& numbers) {
    
    if (numbers.empty()) return 0;

    ExtractNumbersAndDelimiters(numbers);
    
    parseNumbers();
    validateNumbers();
    parsedNumbers = filterLargeNumbers();

    return sumOfNumbers();
}

void StringCalculator::ExtractNumbersAndDelimiters(const std::string& input) {
    
    if (input.substr(0, 2) == "//") { //checks if there is a custom delimiter
        setCustomDelimiter(input);
        extractNumberString(input);
    } else {
        numberString = input;
    }
}

void StringCalculator::setCustomDelimiter(const std::string& input) {
    size_t newlinePos = input.find("\n");
    delimiter = input.substr(2, newlinePos - 2);
}

void StringCalculator::extractNumberString(const std::string& input) {
    numberString = input.substr(input.find("\n") + 1);
}

int StringCalculator::convertToInt(const std::string& number) {
    return std::stoi(number);
}


void StringCalculator::parseNumbers() {
    parsedNumbers.clear();
    size_t startPos = 0;
    size_t pos = 0;

    // Parse each token from the numberString
    while ((pos = findNextDelimiter(startPos)) != std::string::npos) {
        parseSingleNumber(startPos, pos);
        startPos = pos + 1;
    }

    // Handle the last part of the string
    if (startPos < numberString.length()) {
        parseSingleNumber(startPos, numberString.length());
    }
}

size_t StringCalculator::findNextDelimiter(size_t startPos) const {
    return numberString.find_first_of(delimiter, startPos);
}

void StringCalculator::parseSingleNumber(size_t startPos, size_t pos) {
    std::string token = numberString.substr(startPos, pos - startPos);
    if (!token.empty()) {
        parsedNumbers.push_back(convertToInt(token));
    }
}


void StringCalculator::validateNumbers() {
    checkForNegatives();
}

void StringCalculator::checkForNegatives() {
    std::vector<int> negatives = gatherNegatives();

    if (!negatives.empty()) {
        throw std::runtime_error(buildNegativeErrorMessage(negatives));
    }
}

std::vector<int> StringCalculator::gatherNegatives() const {
    std::vector<int> negatives;
    for (int num : parsedNumbers) {
        if (num < 0) {
            negatives.push_back(num);
        }
    }
    return negatives;
}

std::string StringCalculator::buildNegativeErrorMessage(const std::vector<int>& negatives) const {
    std::string errorMsg = "negatives not allowed: ";
    for (int neg : negatives) {
        errorMsg += std::to_string(neg) + " ";
    }
    return errorMsg;
}

std::vector<int> StringCalculator::filterLargeNumbers() {
    std::vector<int> filtered;
    std::copy_if(parsedNumbers.begin(), parsedNumbers.end(), std::back_inserter(filtered), [](int n){ return n <= 1000; });
    return filtered;
}

int StringCalculator::sumOfNumbers() {
    return std::accumulate(parsedNumbers.begin(), parsedNumbers.end(), 0);
}
