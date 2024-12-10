#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <tuple>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <sstream>

using WORD = std::string;
using WORD_LIST = std::set<WORD>;
using WORD_FREQ_LIST = std::map<char, int>;
using FREQ_WORD_LIST = std::multimap<int, WORD>;
using COUNTER = std::tuple<int, int, int>;
using LONGEST = std::priority_queue<WORD>;

struct DICTION {
    COUNTER stats;
    WORD_LIST words;
    WORD_FREQ_LIST letterFreq;
    FREQ_WORD_LIST wordFreq;
    LONGEST longWord;
};

std::string convertToLowerCase(const std::string &str) {
    std::string result = str;
    for (char &ch : result) {
        ch = std::tolower(ch);
    }
    return result;
}

WORD getNextWord(std::istream &input) {
    WORD currentWord;
    char ch;
    bool inWord = false;

    while (input.get(ch)) {
        if (std::isalpha(ch)) {
            currentWord.push_back(ch);
            inWord = true;
        } else if (inWord) {
            return convertToLowerCase(currentWord);
        }
    }
    return convertToLowerCase(currentWord);
}

void displayLetterFrequency(const WORD_FREQ_LIST &letterFreq) {
    std::cout << "/--------------\\\n|  Letter Freq |\n\\--------------/\n";
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        if (letterFreq.count(ch)) {
            std::cout << ch << " |";
            for (int i = 0; i < std::min(letterFreq.at(ch), 10); ++i) {
                std::cout << '*';
            }
            if (letterFreq.at(ch) > 10) {
                std::cout << " (" << letterFreq.at(ch) << ")";
            }
            std::cout << "\n";
        }
    }
}

void displayWordFrequency(const FREQ_WORD_LIST &wordFreq) {
    std::cout << "/------------\\\n| Dictionary |\n\\------------/\n";
    std::cout << "Word  Frequency\n-------------------------\n";
    for (const auto &[freq, word] : wordFreq) {
        std::cout << std::left << std::setw(10) << word << freq << " |";
        for (int i = 0; i < freq; ++i) {
            std::cout << '*';
        }
        std::cout << "\n";
    }
}

void displayWordHistogram(const FREQ_WORD_LIST &wordFreq) {
    size_t maxWordLen = 0;
    for (const auto &[freq, word] : wordFreq) {
        maxWordLen = std::max(maxWordLen, word.length());
    }

    std::string dashes(maxWordLen + 10, '-');
    std::cout << "/------------\\\n| Histogram  |\n\\------------/\n";
    std::cout << dashes << "\n";

    for (const auto &[freq, word] : wordFreq) {
        std::cout << std::right << std::setw(maxWordLen) << word << " ";
        for (int i = 0; i < freq; ++i) {
            std::cout << '*';
        }
        std::cout << "\n";
    }
    std::cout << dashes << "\n";
}

int main() {
    const std::string filename = "word.txt";

    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Error: Unable to open file " << filename << "\n";
        return 1;
    }

    DICTION d;
    auto &[numChars, numWords, numLines] = d.stats;
    numChars = numWords = numLines = 0;

    std::map<WORD, int> tempWordFreq;

    std::string line;
    while (std::getline(inputFile, line)) {
        ++numLines;

        std::istringstream lineStream(line);
        WORD word;
        while (!(word = getNextWord(lineStream)).empty()) {
            d.words.insert(word);
            tempWordFreq[word]++;
            d.longWord.push(word);
            ++numWords;
        }
    }

    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);

    char ch;
    while (inputFile.get(ch)) {
        ++numChars;
        if (std::isalpha(ch)) {
            ++d.letterFreq[std::tolower(ch)];
        }
    }

    for (const auto &[word, freq] : tempWordFreq) {
        d.wordFreq.insert({freq, word});
    }

    std::cout << "Words in dictionary – ";
    for (const auto &w : d.words) {
        std::cout << w << ",";
    }
    std::cout << "\n";

    std::cout << "Number of Letters   : " << numChars << "\n";
    std::cout << "Number of Words     : " << numWords << "\n";
    std::cout << "Number of Lines     : " << numLines << "\n";

    displayLetterFrequency(d.letterFreq);
    displayWordFrequency(d.wordFreq);
    displayWordHistogram(d.wordFreq);

    return 0;
}
