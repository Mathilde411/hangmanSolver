//
// Created by mathilde on 29/07/24.
//

#ifndef UNTITLED_SOLVER_H
#define UNTITLED_SOLVER_H


#include <memory>
#include <vector>
enum Letters : uint64_t {
    NONE = 0,
    A = 1<<0,
    B = 1<<1,
    C = 1<<2,
    D = 1<<3,
    E = 1<<4,
    F = 1<<5,
    G = 1<<6,
    H = 1<<7,
    I = 1<<8,
    J = 1<<9,
    K = 1<<10,
    L = 1<<11,
    M = 1<<12,
    N = 1<<13,
    O = 1<<14,
    P = 1<<15,
    Q = 1<<16,
    R = 1<<17,
    S = 1<<18,
    T = 1<<19,
    U = 1<<20,
    V = 1<<21,
    W = 1<<22,
    X = 1<<23,
    Y = 1<<24,
    Z = 1<<25,
    DASH = 1<<26,
    ALL = (1<<27) - 1
};

const std::array<Letters, 27> ALPHABET = {
        Letters::A, Letters::B, Letters::C, Letters::D, Letters::E, Letters::F, Letters::G, Letters::H,
        Letters::I, Letters::J, Letters::K, Letters::L, Letters::M, Letters::N, Letters::O, Letters::P,
        Letters::Q, Letters::R, Letters::S, Letters::T, Letters::U, Letters::V, Letters::W, Letters::X,
        Letters::Y, Letters::Z, Letters::DASH
};



class Solver {
private:
    unsigned int _wordSize;
    double _totalProba{};
    uint64_t _guessedLetters;
    std::unique_ptr<uint64_t[]> _currentGuess;
    std::vector<std::pair<std::unique_ptr<uint64_t[]>, double>> _wordList;

    static uint64_t createCharRepresentation(char c);
    static char createCharString(uint64_t c);

    std::unique_ptr<uint64_t[]> createWordRepresentation(std::string& word) const;
    std::string createWordString(uint64_t* word) const;

    bool correspondsToGuess(uint64_t* word);
    void filterWordList();

    [[nodiscard]] std::pair<uint64_t, double> computeLetterCollision(const std::pair<std::unique_ptr<uint64_t[]>, double>& word, Letters letter) const;
    double computeGuessEntropy(Letters letter);

public:
    Solver(unsigned int wordSize, std::vector<std::string>& wordList);

    std::vector<std::pair<char, double>> guessEntropies();
    std::vector<std::pair<std::string, double>> wordsProbabilities();
    std::string currentGuess();
    double entropy();
    void guess(char letter, std::vector<unsigned long>& places);
    void printInfo();

};


#endif //UNTITLED_SOLVER_H
