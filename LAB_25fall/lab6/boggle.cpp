#include <map>
#include <utility>
#include "lexicon.h"
#include <vector>

//? Preset Classes
class Player {
public:
    Player();

    ~Player();

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

    Board(const char *in_map, int n);

    ~Board();

    bool isVaildWord(const std::string *);
    bool isValidPath(const std::string *);

    void printAllWords();

private:
    //  const char *map;
    std::map<char, std::pair<int,int>> ;

    std::vector<std::string> read_words;
    void mark_read(const std::string &mkword) {read_words.push_back(mkword);}
    int size;



    Lexicon lexi;
};

//======================= Accomplishment of Board =================
Board::Board() {
    lexi = Lexicon();
    //map = nullptr;
    size = -1;
}
Board::Board(const char *in_map, int n) {
    //map = in_map;

    //TODO : init map

    size = n;
    lexi = Lexicon("EnglishWors.txt");
}

bool Board::isVaildWord(const std::string * word) {

    // > 4 characters
    if (word->empty()||word->size()<4) {
        return false;
    }

    // didn't appeared before
    for (auto & read_word : read_words) { // NOLINT(*-use-anyofallof)
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
bool Board::isValidPath(const std::string *) {

}

void Board::printAllWords() {

}


int main() {
    //* init lexicon
    //Lexicon lex("EnglishWord.txt");
    return 0;
}
