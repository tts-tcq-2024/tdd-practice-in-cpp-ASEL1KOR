#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <regex>

class StringCalculator {
public:
    int Add(const std::string& numbers) {
        if (numbers.empty()) {
            return 0;
        }

        std::string delimiter = extractDelimiter(numbers);
        std::string numPart = extractNumbersPart(numbers);

        std::vector<int> parsedNumbers = splitAndParseNumbers(numPart, delimiter);
        handleNegatives(parsedNumbers);
        
        return sumNumbers(parsedNumbers);
    }

private:
    // Function to extract custom delimiter or default
    std::string extractDelimiter(const std::string& numbers) {
        if (numbers.substr(0, 2) == "//") {
            std::string delimiterSpec = numbers.substr(2, numbers.find('\n') - 2);
            if (delimiterSpec[0] == '[' && delimiterSpec.back() == ']') {
                return std::regex_replace(delimiterSpec.substr(1, delimiterSpec.size() - 2), std::regex("\\*"), "\\*");
            }
            return std::regex_replace(delimiterSpec, std::regex("\\*"), "\\*");
        }
        return ",|\n";  // Default delimiter
    }

    // Function to extract the part of the string containing the numbers
    std::string extractNumbersPart(const std::string& numbers) {
        if (numbers.substr(0, 2) == "//") {
            return numbers.substr(numbers.find("\n") + 1);
        }
        return numbers;
    }

    // Function to split the string and parse numbers
    std::vector<int> splitAndParseNumbers(const std::string& numbers, const std::string& delimiter) {
        std::regex delimiterRegex(delimiter);
        std::sregex_token_iterator iter(numbers.begin(), numbers.end(), delimiterRegex, -1);
        std::sregex_token_iterator end;

        std::vector<int> result;
        for (; iter != end; ++iter) {
            if (!iter->str().empty()) {
                result.push_back(std::stoi(iter->str()));
            }
        }
        return result;
    }

    // Function to handle negative numbers
    void handleNegatives(const std::vector<int>& numbers) {
        std::vector<int> negatives;
        for (int number : numbers) {
            if (number < 0) {
                negatives.push_back(number);
            }
        }
        if (!negatives.empty()) {
            throw std::invalid_argument("Negatives not allowed: " + join(negatives, ", "));
        }
    }

    // Function to sum numbers, ignoring those greater than 1000
    int sumNumbers(const std::vector<int>& numbers) {
        int sum = 0;
        for (int number : numbers) {
            if (number <= 1000) {
                sum += number;
            }
        }
        return sum;
    }

    // Helper function to join a vector of numbers into a string
    std::string join(const std::vector<int>& numbers, const std::string& delimiter) {
        std::ostringstream result;
        for (size_t i = 0; i < numbers.size(); ++i) {
            if (i != 0) {
                result << delimiter;
            }
            result << numbers[i];
        }
        return result.str();
    }
};

int main() {
    StringCalculator calculator;
    try {
        std::cout << calculator.Add("//;\n1;2") << std::endl;   // Output: 3
        std::cout << calculator.Add("1\n2,3") << std::endl;     // Output: 6
        std::cout << calculator.Add("1001,2") << std::endl;     // Output: 2
        std::cout << calculator.Add("2,1000") << std::endl;     // Output: 1002
        std::cout << calculator.Add("//***\n12***3***1001") << std::endl;  // Output: 15
        // Negative number test:
        // std::cout << calculator.Add("1,-2,3") << std::endl;  // Throws exception: "Negatives not allowed: -2"
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

