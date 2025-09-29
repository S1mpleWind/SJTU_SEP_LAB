#include <iostream>
#include <map>
#include <utility>
#include "lexicon.h"
#include <vector>

//? Preset Classes
class Player {
public:
    Player() { score = 0; };

    ~Player() {
    };

    int getScore() const { return score; }
    void addScore() { score++; }

private:
    int score;
};


/**
 * Board class is designed to store characters ,
 * check if valid word , return all the word
 */
class Board {
public:
    Board();

    Board(const std::string *in_str, int n);

    ~Board() {
    };

    bool isVaildWord(std::string *);

    bool isValidPath(std::string *);

    void printBoard() const;

    void printAllWords() const;

private:
    // * member variables

    // the value is an array to store the same char
    std::map<const char, std::vector<std::pair<int, int> > > char_map;
    std::vector<std::string> read_words;
    int size;
    Lexicon lexi;

    // * private functions
    bool isNeighbor(const std::pair<int, int> &pos1, const std::pair<int, int> &pos2) const;

    void init_map(const std::string *in_str);

    void mark_read(const std::string &mkword) { read_words.push_back(mkword); }

    // need another parameter to make sure the path is contious
    bool isValidPathHelper(std::string *in_, std::pair<int, int> startPos);
};

//======================= Accomplishment of Board =================
Board::Board() {
    lexi = Lexicon();
    //map = nullptr;
    size = -1;
}

Board::Board(const std::string *str, int n) {
    //map = in_map;

    size = n;
    //TODO : init map
    init_map(str);

    lexi = Lexicon("EnglishWords.txt");
}


void Board::init_map(const std::string *str) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // if alreaday exists
            if (char_map.count(str[i][j])) {
                char_map[str[i][j]].emplace_back(std::make_pair(i, j));
            }

            // Didin't appeared before
            else {
                std::vector<std::pair<int, int> > vec;
                vec.emplace_back(i, j);
                char_map.insert(std::make_pair(str[i][j], vec));
            }
        }
    }
}

bool Board::isNeighbor(const std::pair<int, int> &pos1, const std::pair<int, int> &pos2) const {
    const int x1 = pos1.first;
    const int y1 = pos1.second;
    const int x2 = pos2.first;
    const int y2 = pos2.second;

    if ((x1 - x2) % size <= 1 && (y1 - y2) % size <= 1) {
        return true;
    }

    return false;
}


bool Board::isVaildWord(std::string *word) {
    // > 4 characters
    if (word->empty() || word->size() < 4) {
        return false;
    }

    // didn't appeared before
    for (auto &read_word: read_words) { // NOLINT(*-use-anyofallof)
        if (read_word == *word) {
            return false;
        }
    }

    if (isValidPath(word) && lexi.contains(*word)) {
        mark_read(*word);
        return true;
    }
    return false;
}


//* Most important functions here
// check if the sequence is legal in the map
//? use Recursion

//TODO : how to ensure the path is contious ?
//! the startPos is the position of the char previously checked
bool Board::isValidPathHelper(std::string *in_, std::pair<int, int> startPos) {
    //base case:
    if (in_->size() == 1) {
        if (!char_map.count(in_->at(0))) {
            return false;
        }
        for (auto it = char_map[in_->at(0)].begin(); it != char_map[in_->at(1)].end(); ++it) {
            if (isNeighbor(startPos, *it)) {
                return true;
            }
        }
        return false;
    } else {
        if (char_map.count(in_->at(0)) == 0 ) {
            return false;
        }
        for (auto it = char_map[in_->at(0)].begin(); it != char_map[in_->at(1)].end(); ++it) {
            if (isNeighbor(startPos, *it)) {
                std::string temp = in_->substr(1, in_->size() - 1);
                //TODO : how to solve the issue of temporary variable?
                //TODO : currently use a temp
                return isValidPathHelper(&temp, *it);
            }
        }
        return false;
    }
    //return false;
}


bool Board::isValidPath(std::string *check) {
    char first_char = check->at(0);
    if (!char_map.count(first_char)) {
        return false;
    } else {
        for (auto &it: char_map[first_char]) {
            std::string temp = check->substr(1, check->size() - 1);
            if (isValidPathHelper(&temp, it)) {
                return true;
            }
        }
        return false;
    }
}

void Board::printAllWords() const {
    std::cout << "All the words you have found: " << std::endl;
}

void Board::printBoard() const {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (auto &it: char_map) {
                for (auto &pos: it.second) {
                    if (pos.first == i && pos.second == j) {
                        std::cout << it.first << " ";
                    }
                }
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    //* 1. data input & creating instance
    int board_size;

    std::cout << "Please input the board size (n): ";
    std::cin >> board_size;

    std::cout << "Please input the board character row by row: " << std::endl;
    auto inputChar = new std::string[board_size];
    for (int i = 0; i < board_size; i++) {
        std::string row_str;
        std::cin >> row_str;
        if (row_str.size() != board_size) {
            std::cout << "Wrong input format! Please input again!" << std::endl;
            i--;
        } else {
            inputChar[i] = row_str;
        }
    }

    Board board(inputChar, board_size);
    board.printBoard();

    Player p1;
    Player p2;

    int current_player = 1;

    //TODO: TEST
    while (true) {
        std::string input_word;
        std::cin >> input_word;
        if (input_word == "???") {
            break;
        }
        if (board.isVaildWord(&input_word)) {
            std::cout << "Valid Word!" << std::endl;
        }
        else {
            std::cout << "Invalid Word!" << std::endl;
        }
    }

    //* 2. game circle
    /*
    while (true) {
        board.printBoard();
        std::cout << "Player " << current_player << std::endl;
        std::cout << "Current Score: " << (current_player == 1 ? p1.getScore() : p2.getScore()) << std::endl;
        //std::cout <<
        std::string input_word;
        std::cin >> input_word;
    }
    */
    //* 3. ending

    delete [] inputChar;
    return 0;
}
