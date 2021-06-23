#include "Earley.h"
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

int NumberFromLetter(char letter) {
  int number = letter - 'a';
  if (letter <= 'Z' && letter >= 'A') {
    number = letter - 'A';
  }
  return number;
}

State::State(char not_terminiate, const std::vector<char> &alpha,
             const std::vector<char> &betta, int passed_way)
    : alpha_(alpha), betta_(betta), passed_way_(passed_way),
      not_terminate_(not_terminiate){};

bool State::operator==(const State &other) const {
  if (alpha_.size() != other.alpha_.size()) {
    return false;
  }
  if (betta_.size() != other.betta_.size()) {
    return false;
  }
  for (int i = 0; i < alpha_.size(); ++i) {
    if (alpha_[i] != other.alpha_[i]) {
      return false;
    }
  }
  for (int i = 0; i < betta_.size(); ++i) {
    if (betta_[i] != other.betta_[i]) {
      return false;
    }
  }
  if (not_terminate_ != other.not_terminate_) {
    return false;
  }
  if (passed_way_ != other.passed_way_) {
    return false;
  }
  return true;
}

bool IsBigLetter(char letter) { return (letter >= 'A' && letter <= 'Z'); }

Algo::Algo(const std::vector<std::pair<char, std::string>> &jumps) {
  int size = 26;
  std::vector<std::vector<char>> rule;
  for (int i = 0; i < size; ++i) {
    rules_.push_back(rule);
  }
  for (auto jump : jumps) {
    if (!IsBigLetter(jump.first)) {
      continue;
    }
    int number = NumberFromLetter(jump.first);
    std::vector<char> vec;
    rules_[number].push_back(vec);
    for (auto right_rule_part_symbol : jump.second) {
      rules_[number][rules_[number].size() - 1].push_back(
          right_rule_part_symbol);
    }
  }
}

void Algo::Predict(std::vector<std::vector<State>> &levels,
                   const State &current, int index) {
  if (current.betta_.size() != 0 && IsBigLetter(current.betta_[0])) {
    int not_terminal = NumberFromLetter(current.betta_[0]);
    for (auto rule : rules_[not_terminal]) {
      std::vector<char> passed_way_vec;
      State new_state(current.betta_[0], passed_way_vec, rule, index);
      levels[index].push_back(new_state);
    }
  }
}

void Algo::Complete(std::vector<std::vector<State>> &levels,
                    const State &current, int index) {
  if (current.betta_.size() == 0) {
    for (auto previous : levels[current.passed_way_]) {
      if (previous.betta_.size() > 0 &&
          previous.betta_[0] == current.not_terminate_) {
        std::vector<char> passed_way_vec = previous.alpha_;
        passed_way_vec.push_back(previous.betta_[0]);
        std::vector<char> index_vec;
        for (int previous_symbol = 1; previous_symbol < previous.betta_.size();
             ++previous_symbol) {
          index_vec.push_back(previous.betta_[previous_symbol]);
        }
        State new_state(previous.not_terminate_, passed_way_vec, index_vec,
                        previous.passed_way_);
        levels[index].push_back(new_state);
      }
    }
  }
}

void Algo::Scan(std::vector<std::vector<State>> &words, const State &current,
                int index, char right_symbol) {

  if (current.betta_.size() != 0 && current.betta_[0] == right_symbol) {
    std::vector<char> passed_way_vec = current.alpha_;
    passed_way_vec.push_back(current.betta_[0]);
    std::vector<char> index_vec;
    for (int current_symbol = 1; current_symbol < current.betta_.size();
         ++current_symbol) {
      index_vec.push_back(current.betta_[current_symbol]);
    }
    State new_state(current.not_terminate_, passed_way_vec, index_vec,
                    current.passed_way_);
    words[index + 1].push_back(new_state);
  }
}

bool StateCmp::operator()(const State &first, const State &second) const {
  if (first.not_terminate_ != second.not_terminate_) {
    return first.not_terminate_ < second.not_terminate_;
  }
  if (first.passed_way_ != second.passed_way_) {
    return first.passed_way_ < second.passed_way_;
  }
  if (first.alpha_.size() != second.alpha_.size()) {
    return first.alpha_.size() < second.alpha_.size();
  }
  if (first.betta_.size() != second.betta_.size()) {
    return first.betta_.size() < second.betta_.size();
  }
  for (int i = 0; i < first.alpha_.size(); ++i) {
    if (first.alpha_[i] < second.alpha_[i]) {
      return true;
    }
    if (first.alpha_[i] > second.alpha_[i]) {
      return false;
    }
  }
  for (int i = 0; i < first.betta_.size(); ++i) {
    if (first.betta_[i] < second.betta_[i]) {
      return true;
    }
    if (first.betta_[i] > second.betta_[i]) {
      return false;
    }
  }
  return false;
}

bool Algo::predict(const std::string &word) {
  for (auto symbol : word) {
    if (IsBigLetter(symbol)) {
      return false;
    }
  }
  std::vector<std::vector<State>> levels(word.size() + 1);
  char start_symbol = '@';
  std::vector<char> alpha_begin;
  std::vector<char> betta_begin = {'S'};
  State begin_state(start_symbol, alpha_begin, betta_begin, 0);
  levels[0].push_back(begin_state);
  std::set<State, StateCmp> help;
  for (int current_state_number = 0; current_state_number < levels[0].size();
       ++current_state_number) {
    State current = levels[0][current_state_number];
    help.insert(current);
    Predict(levels, current, 0);
    Complete(levels, current, 0);
  }
  for (int current_level = 0; current_level < word.size(); ++current_level) {
    levels[current_level].resize(0);
    std::set<State>::iterator it = help.begin();
    while (it != help.end()) {
      levels[current_level].push_back(*(it++));
    }
    for (int state_number = 0; state_number < levels[current_level].size();
         ++state_number) {
      State current = levels[current_level][state_number];
      Scan(levels, current, current_level, word[current_level]);
    }
    help.clear();
    for (int state_number = 0; state_number < levels[current_level + 1].size();
         ++state_number) {
      State current = levels[current_level + 1][state_number];
      help.insert(current);
      Predict(levels, current, current_level + 1);
      Complete(levels, current, current_level + 1);
    }
  }
  for (State current : levels[word.size()]) {
    if (current.not_terminate_ == start_symbol && current.betta_.size() == 0 &&
        current.alpha_.size() == 1 && current.alpha_[0] == 'S') {
      return 1;
    }
  }
  return false;
}
