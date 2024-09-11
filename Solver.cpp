//
// Created by mathilde on 29/07/24.
//

#include <string>
#include <algorithm>
#include <map>
#include <cmath>
#include <numeric>
#include <iostream>
#include "Solver.h"


Solver::Solver(unsigned int wordSize, std::vector<std::string> &wordList) :
        _wordSize(wordSize),
        _currentGuess(new uint64_t[_wordSize]),
        _wordList(),
        _guessedLetters(Letters::NONE) {

    for (uint64_t *letter = _currentGuess.get(); letter < _currentGuess.get() + wordSize; letter++)
        *letter = Letters::ALL;

    for (std::string word: wordList) {
        if (word.length() == _wordSize) {
            _wordList.emplace_back(this->createWordRepresentation(word), 1.);
            _totalProba += 1.;
        }
    }
}

uint64_t Solver::createCharRepresentation(char c) {
    switch (c) {
        case 'a':
            return Letters::A;
        case 'b':
            return Letters::B;
        case 'c':
            return Letters::C;
        case 'd':
            return Letters::D;
        case 'e':
            return Letters::E;
        case 'f':
            return Letters::F;
        case 'g':
            return Letters::G;
        case 'h':
            return Letters::H;
        case 'i':
            return Letters::I;
        case 'j':
            return Letters::J;
        case 'k':
            return Letters::K;
        case 'l':
            return Letters::L;
        case 'm':
            return Letters::M;
        case 'n':
            return Letters::N;
        case 'o':
            return Letters::O;
        case 'p':
            return Letters::P;
        case 'q':
            return Letters::Q;
        case 'r':
            return Letters::R;
        case 's':
            return Letters::S;
        case 't':
            return Letters::T;
        case 'u':
            return Letters::U;
        case 'v':
            return Letters::V;
        case 'w':
            return Letters::W;
        case 'x':
            return Letters::X;
        case 'y':
            return Letters::Y;
        case 'z':
            return Letters::Z;
        case '-':
            return Letters::DASH;
        case '_':
            return Letters::ALL;
        default:
            return Letters::NONE;
    }
}

char Solver::createCharString(uint64_t c) {
    switch (c) {
        case Letters::A:
            return 'a';
        case Letters::B:
            return 'b';
        case Letters::C:
            return 'c';
        case Letters::D:
            return 'd';
        case Letters::E:
            return 'e';
        case Letters::F:
            return 'f';
        case Letters::G:
            return 'g';
        case Letters::H:
            return 'h';
        case Letters::I:
            return 'i';
        case Letters::J:
            return 'j';
        case Letters::K:
            return 'k';
        case Letters::L:
            return 'l';
        case Letters::M:
            return 'm';
        case Letters::N:
            return 'n';
        case Letters::O:
            return 'o';
        case Letters::P:
            return 'p';
        case Letters::Q:
            return 'q';
        case Letters::R:
            return 'r';
        case Letters::S:
            return 's';
        case Letters::T:
            return 't';
        case Letters::U:
            return 'u';
        case Letters::V:
            return 'v';
        case Letters::W:
            return 'w';
        case Letters::X:
            return 'x';
        case Letters::Y:
            return 'y';
        case Letters::Z:
            return 'z';
        case Letters::DASH:
            return '-';
        case Letters::NONE:
            return '?';
        default:
            return '_';
    }
}

std::unique_ptr<uint64_t[]> Solver::createWordRepresentation(std::string &word) const {
    std::unique_ptr<uint64_t[]> res(new uint64_t[_wordSize]);

    for (unsigned int i = 0; i < _wordSize; ++i) {
        res[i] = createCharRepresentation(word[i]);
    }

    return res;
}

std::string Solver::createWordString(uint64_t *word) const {
    std::string res;

    for (unsigned int i = 0; i < _wordSize; ++i) {
        res += createCharString(word[i]);
    }

    return res;
}

bool Solver::correspondsToGuess(uint64_t *word) {
    for (size_t i = 0; i < _wordSize; ++i)
        if ((word[i] & _currentGuess[i]) == 0)
            return false;
    return true;
}

void Solver::filterWordList() {
    _wordList.erase(
            std::remove_if(_wordList.begin(), _wordList.end(),
                           [this](std::pair<std::unique_ptr<uint64_t[]>, double> &word) {
                               return !this->correspondsToGuess(word.first.get());
                           }),
            _wordList.end());

    _totalProba = std::accumulate(_wordList.begin(), _wordList.end(), 0.0,
                                  [](double acc, std::pair<std::unique_ptr<uint64_t[]>, double> &w) {
                                      return acc + w.second;
                                  });
}

std::pair<uint64_t, double> Solver::computeLetterCollision(const std::pair<std::unique_ptr<uint64_t[]>, double>& word, Letters letter) const {
    uint64_t unit = 1;
    uint64_t res = 0;
    for (int i = 0; i < _wordSize; ++i) {
        if ((letter & word.first[i]) != 0)
            res += unit;
        unit = unit << 1;
    }
    return {res, word.second};
}

double Solver::computeGuessEntropy(Letters letter) {
    std::map<uint64_t, double> stat;
    for (std::pair<std::unique_ptr<uint64_t[]>, double> &word: _wordList) {
        std::pair<uint64_t, double> coll = computeLetterCollision(word, letter);
        stat[coll.first] = (stat.contains(coll.first) ? stat[coll.first] : 0.) + coll.second;
    }

    double entropy = 0;
    for (auto &s: stat) {
        double p = s.second / _totalProba;
        entropy -= p * log2(p);
    }

    return entropy;
}

std::vector<std::pair<char, double>> Solver::guessEntropies() {
    std::vector<std::pair<char, double>> res;
    res.reserve(ALPHABET.size());

    for (Letters l: ALPHABET) {
        if((l & _guessedLetters) == 0)
            res.emplace_back(createCharString(l), computeGuessEntropy(l));
    }

    std::sort(res.begin(), res.end(),
              [](std::pair<char, double> el1, std::pair<char, double> el2) { return el1.second > el2.second; });

    return res;

}

std::vector<std::pair<std::string, double>> Solver::wordsProbabilities() {
    std::vector<std::pair<std::string, double>> res;
    res.reserve(_wordList.size());

    for (auto &word: _wordList) {
        res.emplace_back(createWordString(word.first.get()), word.second/_totalProba);
    }

    std::sort(res.begin(), res.end(),
              [](const std::pair<std::string, double>& el1, const std::pair<std::string, double>& el2) { return el1.second < el2.second; });

    return res;
}

std::string Solver::currentGuess() {
    return createWordString(_currentGuess.get());
}

void Solver::guess(char letter, std::vector<unsigned long> &places) {
    auto l = createCharRepresentation(letter);

    _guessedLetters |= l;

    unsigned long acc = 0;
    for(unsigned long i : places) {
        acc |= (1 << i);
    }

    for(size_t i = 0; i < _wordSize; ++i) {
        _currentGuess[i] &= ((acc & 1) == 0 ? ~l : l);
        acc = acc >> 1;
    }

    filterWordList();

}

double Solver::entropy() {
    double entropy = 0;
    for (auto &s: _wordList) {
        double p = s.second / _totalProba;
        entropy -= p * log2(p);
    }
    return entropy;
}

void Solver::printInfo() {

    std::cout << "Word: " << currentGuess() << std::endl;
    std::cout << "Entropy: " << entropy() << " bits" << std::endl;
    std::cout << "Possible words:" << std::endl;
    auto words = wordsProbabilities();
    int c = 0;
    for(const auto& word : words) {
        std::cout << " - " << word.first << ": " << (word.second * 100) << "%" << std::endl;
        if(++c >= 5) break;
    }
    c = 0;
    std::cout << "Possible guesses:" << std::endl;
    auto guesses = guessEntropies();
    for(const auto& guess : guesses) {
        std::cout << " - " << guess.first << ": " << guess.second << " bits" << std::endl;
        if(++c >= 5) break;
    }
}
