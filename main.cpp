#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <random>
#include <algorithm>
#include "Solver.h"

std::vector<std::string> words() {
    std::ifstream file;
    std::vector<std::string> res;
    file.open("/home/mathilde/CLionProjects/untitled/dict.csv");
    if(file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            res.push_back(line.substr(0, line.find(';')));
        }

    }
    file.close();

    return res;
}

void game(std::vector<std::string> w) {
    int letters;
    std::cout << "Nombre de lettres: ";
    std::cin >> letters;
    Solver s(letters, w);

    while(s.entropy() > 0) {
        std::cout << std::endl;
        s.printInfo();
        std::cout << std::endl;
        char c;
        std::cout << "Lettre choisie: ";
        std::cin >> c;
        c = std::tolower(c);

        unsigned int numberOfPos;
        std::cout << "Nombre de hits: ";
        std::cin >> numberOfPos;

        std::vector<unsigned long> positions;
        for(int _ = 0; _ < numberOfPos; ++_) {
            unsigned long pos;
            std::cout << "Position " << (_ + 1) <<": ";
            std::cin >> pos;

            if(pos < 1)
                --_;
            else
                positions.push_back(pos - 1);
        }
        s.guess(c, positions);
    }

    auto possiblilities = s.wordsProbabilities();

    if(possiblilities.empty())
        std::cout << "Mot non trouvé" << std::endl;
    else
        std::cout << "Mot trouvé: " << possiblilities[0].first << std::endl;
}

void testWord(std::vector<std::string> ws) {
    std::string w;
    std::cout << "Word: ";
    std::cin >> w;
    std::cout  << std::endl;

    Solver s(w.size(), ws);

    std::multimap<char, unsigned long> letters;
    unsigned long i = 0;
    for(char c : w) {
        letters.insert({c,i});
        ++i;
    }

    unsigned long tries = 0;
    unsigned long wrongs = 0;
    while(s.entropy() > 0) {
        char c = s.guessEntropies()[0].first;
        auto range = letters.equal_range(c);
        std::vector<unsigned long> pos;
        for (auto k = range.first; k != range.second; ++k)
            pos.push_back(k->second);
        s.guess(c, pos);
        if(pos.empty())
            ++wrongs;
        std::cout << "Guess: " << c << " " << (pos.empty() ? "ERROR" : "OK") << " " << s.currentGuess() << " " << std::endl;
        ++tries;
    }

    std::cout  << std::endl;
    std::cout << "Tries: " << tries << std::endl;
    std::cout << "Wrongs: " << wrongs << std::endl;
    auto p = s.wordsProbabilities();
    std::cout << "Found: " << (s.wordsProbabilities().empty() ? "NO" : p[0].first) << std::endl;
}

void test(std::vector<std::string>& ws) {
    std::ofstream file;
    file.open("output.csv");

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(ws.begin(), ws.end(), g);

    for(const std::string& w : ws) {
        Solver s(w.size(), ws);

        std::multimap<char, unsigned long> letters;
        unsigned long i = 0;
        for(char c : w) {
            letters.insert({c,i});
            ++i;
        }

        unsigned long tries = 0;
        unsigned long wrongs = 0;
        while(s.entropy() > 0) {
            char c = s.guessEntropies()[0].first;
            auto range = letters.equal_range(c);
            std::vector<unsigned long> pos;
            for (auto k = range.first; k != range.second; ++k)
                pos.push_back(k->second);
            s.guess(c, pos);
            if(pos.empty())
                ++wrongs;
            ++tries;
        }

        std::cout << w << ";" << tries << ";" << wrongs << std::endl;

    }
}


int main() {
    auto w = words();

    game(w);

    return 0;
}
