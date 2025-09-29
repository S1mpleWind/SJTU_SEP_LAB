# Boggle Game

## 1. What I've got

- `Lexicon` : Help to search words in a dictionary
  -   Lexicon(const std::string &filename); 
  -   bool add(const std::string &word);
  -   bool contains(const std::string &word) const;
  - bool containsPrefix(const std::string &prefix) const;

- `EnglishWords.txt` : A dictionary of English words

---

## 2. TODO list
- [x] Frame of the program
- [x] Player Class
- [ ] Board Class
  - [x] Valid Word
  - [ ] valid Path
  - [ ] map < char , std :: vector < std::pair<int,int>>
---

## 3. Bugs
### 1). Test one
1. In the Base Case of `validPathHelper` , I didn't check if the character exist in the board
2. I didn't check if startpos equals to the possible next position
3. change the logic of return in the loop to ensure all it can search all the branches.