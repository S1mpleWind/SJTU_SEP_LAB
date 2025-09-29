/**
 * @file boggle.cpp
 * @brief Accomplish a boggle game . However I used map instead of array ,
 *        which turns out not a good idea
 * @author J
 * @date 2025.9.29
 * @version 1.0
 */

#include <iostream>
#include <map>
#include <utility>
#include "lexicon.h"
#include <vector>
#include <algorithm>
#include <set>

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

    void printAllWords();

    void resetRead() { read_words.clear(); }

private:
    // * member variables
    std::map<const char, std::vector<std::pair<int, int> > > char_map;
    std::vector<std::string> read_words;
    std::set<std::string> all_words;
    int size;
    Lexicon lexi;
    std::vector<std::vector<bool>> visited;

    // * private functions
    bool isNeighbor(const std::pair<int, int> &pos1, const std::pair<int, int> &pos2) const;

    void init_map(const std::string *in_str);

    void mark_read(const std::string &mkword) { read_words.push_back(mkword); }

    // need another parameter to make sure the path is contious
    bool isValidPathHelper(std::string *in_, std::pair<int, int> startPos);

    void printAllWordsHelper(std::string *curr , std::pair<int,int> currPos) ;
};

//======================= Accomplishment of Board =================
Board::Board() {
    lexi = Lexicon();
    size = -1;
    // visited初始化为空
}

Board::Board(const std::string *str, int n) {
    size = n;
    //TODO : init map
    init_map(str);

    lexi = Lexicon("EnglishWords.txt");
    visited = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false)); // 初始化visited
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

    if ((std::abs(x1 - x2) == 1 || y1 == y2) &&
        (std::abs(y1 - y2) == 1  || y1 == y2) &&
        !(x1 == x2 && y1 == y2)) {
        return true;
    }
    return false;
}


bool Board::isVaildWord(std::string *word) {
    // > 4 characters
    if (word->empty() || word->size() < 4) {
        std::cout << "too short word" << std::endl;
        return false;
    }

    // didn't appeared before
    for (auto &read_word: read_words) {
        if (read_word == *word) {
            std::cout << "Word already found!" << std::endl;
            return false;
        }
    }

    if (!isValidPath(word)) {
        std::cout << "Word not in map" << std::endl;
        return false;
    }
    if (!lexi.contains(*word)) {
        std::cout << "Word not in dictionary" << std::endl;
        return false;
    }
    mark_read(*word);
    return true;
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
        for (auto it = char_map[in_->at(0)].begin(); it != char_map[in_->at(0)].end(); ++it) {
            if (isNeighbor(startPos, *it)) {
                return true;
            }
        }
        return false;
    } else {
        if (char_map.count(in_->at(0)) == 0) {
            return false;
        }
        for (auto it = char_map[in_->at(0)].begin(); it != char_map[in_->at(0)].end(); ++it) {
            if (*it != startPos && isNeighbor(startPos, *it)) {
                std::string temp = in_->substr(1, in_->size() - 1);
                //TODO : how to solve the issue of temporary variable?
                //TODO : currently use a temp
                if (isValidPathHelper(&temp, *it)) return true;
            }
        }
        return false;
    }
    //return false;
}


bool Board::isValidPath(std::string *check) {
    char first_char = check->at(0);
    if (!char_map.count(first_char)) {
        //std::cout << "Not in the map"<<std::endl;
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

void Board::printAllWordsHelper(std::string * curr , std::pair<int,int> currPos)  {
    int x = currPos.first, y = currPos.second;
    if (visited[x][y]) return; // if already visited
    visited[x][y] = true; // mark the current as visted

    //* pruning
    if (!lexi.containsPrefix(*curr)) {
        visited[x][y] = false;
        return;
    }

    //* valid out
    if (curr->size() >= 4 && lexi.contains(*curr)) {
        all_words.insert(*curr);
    }

    //* traverse all the neighbors
    static const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    static const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int d = 0; d < 8; ++d) {
        int nx = x + dx[d];
        int ny = y + dy[d];

        //check the boundaries
        if (nx < 0 || ny < 0 || nx >= size || ny >= size) continue;

        if (visited[nx][ny]) continue; // jump the visited

        char nextChar = 0;

        //? search backward for coordinate
        //? I should have used arrays ...
        for (const auto &it : char_map) {
            for (const auto &pos : it.second) {
                if (pos.first == nx && pos.second == ny) {
                    nextChar = it.first;
                    break;
                }
            }
            if (nextChar) break;
        }

        // * recursion
        if (nextChar) {
            std::string next = *curr + nextChar;
            printAllWordsHelper(&next, {nx, ny});
        }
    }

    visited[x][y] = false; //* backtrack
}

void Board::printAllWords()  {
    std::cout << "All possible words : " ;
    all_words.clear();
    for (auto it = char_map.begin(); it != char_map.end(); ++it) {
        for (auto itr = it->second.begin(); itr != it->second.end(); ++itr) {
            std::string curr(1, it->first);
            std::fill(visited.begin(), visited.end(), std::vector<bool>(size, false)); // 每次新起点清空visited
            printAllWordsHelper(&curr, *itr);
        }
    }
    for (auto it : all_words) {
        std::cout << it <<" ";
    }
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
    //board.printBoard();

    Player p1;
    Player p2;

    int current_player = 1;

    // //TODO: TEST
    // while (true) {
    //     std::string input_word;
    //     std::cin >> input_word;
    //     std::transform(input_word.begin(), input_word.end(), input_word.begin(), ::toupper);
    //     if (input_word == "???") {
    //         break;
    //     }
    //     if (board.isVaildWord(&input_word)) {
    //         std::cout << "Valid Word!" << std::endl;
    //     }
    //     else {
    //         std::cout << "Invalid Word!" << std::endl;
    //     }
    // }

    //* 2. game circle

    while (true) {
        board.printBoard();
        std::cout << "Player " << current_player << std::endl;
        std::cout << "Current Score: " << (current_player == 1 ? p1.getScore() : p2.getScore()) << std::endl;

        std::string input_word;
        std::cin >> input_word;
        // transform into upper characters
        std::transform(input_word.begin(), input_word.end(), input_word.begin(), ::toupper);
        if (input_word == "???") {
            if (current_player == 1) {
                current_player++;
                board.resetRead();
                continue;
            }
            break;
        }
        if (board.isVaildWord(&input_word)) {
            std::cout << "Correct!" << std::endl;
            current_player == 1 ? p1.addScore() : p2.addScore();
        } else {
            std::cout << "Invalid Word!" << std::endl;
        }
    }

    //* 3. ending
    std::cout << "player 1 : " << p1.getScore() << std::endl;
    std::cout << "player 2 : " << p2.getScore() << std::endl;

    if (p1.getScore() == p2.getScore()) {
        std::cout << "A Tie" << std::endl;
    } else if (p1.getScore() > p2.getScore()) {
        std::cout << "Player 1 wins!" << std::endl;
    } else {
        std::cout << "Player 2 wins!" << std::endl;
    }

    board.printAllWords();

    delete [] inputChar;
    return 0;
}
