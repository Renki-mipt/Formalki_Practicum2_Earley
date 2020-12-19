#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

struct State {
  char not_terminate_;
  std::vector<char> alpha_;
  std::vector<char> betta_;
  int i_;
  State() {}
  State(char not_terminiate, const std::vector<char> &alpha,
        const std::vector<char> &betta, int i);
  bool operator==(const State& other) const;
};

// std::ostream &operator<<(std::ostream &os, State &st);
bool IsBigLetter(char c);

class Algo {
public:
  Algo() {}
  Algo(const std::vector<std::pair<char, std::string>> &jumps);
  void Predict(std::vector<std::vector<State>> &levels, State &current,
               int j_ind);
  void Complete(std::vector<std::vector<State>> &levels, State &current,
                int j_ind);
  void Scan(std::vector<std::vector<State>> &levels, State &current, int j_ind,
            char right_symbol);
  bool predict(const std::string &word);

private:
  std::vector<std::vector<std::vector<char>>> rules_;
};

struct StateCmp {
  bool operator()(const State &first, const State &second) const;
};
