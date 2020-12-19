#include "Earley.h"
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

State::State(char not_terminiate, const std::vector<char> &alpha,
             const std::vector<char> &betta, int i)
    : alpha_(alpha), betta_(betta), i_(i), not_terminate_(not_terminiate){};

bool State::operator==(const State &other) const {
  if (alpha_.size() != other.alpha_.size())
    return false;
  if (betta_.size() != other.betta_.size())
    return false;
  for (int i = 0; i < alpha_.size(); ++i)
    if (alpha_[i] != other.alpha_[i])
      return false;
  for (int i = 0; i < betta_.size(); ++i)
    if (betta_[i] != other.betta_[i])
      return false;
  if (not_terminate_ != other.not_terminate_)
    return false;
  if (i_ != other.i_)
    return false;
  return true;
}

bool IsBigLetter(char c) { return (c >= 'A' && c <= 'Z'); }

Algo::Algo(const std::vector<std::pair<char, std::string>> &jumps) {
  int size = 26;
  std::vector<std::vector<char>> rule;
  for (int i = 0; i < size; ++i)
    rules_.push_back(rule);

  for (int i = 0; i < jumps.size(); ++i) {
    if (!IsBigLetter(jumps[i].first))
      continue;
    int number = jumps[i].first - 'A';
    std::vector<char> vec;
    rules_[number].push_back(vec);
    for (int j = 0; j < jumps[i].second.size(); ++j)
      rules_[number][rules_[number].size() - 1].push_back(jumps[i].second[j]);
  }
}

void Algo::Predict(std::vector<std::vector<State>> &levels, State &current,
                   int j_ind) {
  if (current.betta_.size() != 0 && IsBigLetter(current.betta_[0])) {
    int not_terminal = current.betta_[0] - 'A';
    for (int i = 0; i < rules_[not_terminal].size(); ++i) {
      std::vector<char> i_vec;
      State new_state(current.betta_[0], i_vec, rules_[not_terminal][i], j_ind);
      levels[j_ind].push_back(new_state);
    }
  }
}

void Algo::Complete(std::vector<std::vector<State>> &levels, State &current,
                    int j_ind) {
  if (current.betta_.size() == 0)
    for (int i = 0; i < levels[current.i_].size(); ++i) {
      State previous = levels[current.i_][i];
      if (previous.betta_.size() > 0 &&
          previous.betta_[0] == current.not_terminate_) {
        std::vector<char> i_vec = previous.alpha_;
        i_vec.push_back(previous.betta_[0]);
        std::vector<char> j_vec;
        for (int j = 1; j < previous.betta_.size(); ++j)
          j_vec.push_back(previous.betta_[j]);
        State new_state(previous.not_terminate_, i_vec, j_vec, previous.i_);
        levels[j_ind].push_back(new_state);
      }
    }
}

void Algo::Scan(std::vector<std::vector<State>> &words, State &current,
                int j_ind, char right_symbol) {

  if (current.betta_.size() != 0 && current.betta_[0] == right_symbol) {
    std::vector<char> i_vec = current.alpha_;
    i_vec.push_back(current.betta_[0]);
    std::vector<char> j_vec;
    for (int j = 1; j < current.betta_.size(); ++j)
      j_vec.push_back(current.betta_[j]);
    State new_state(current.not_terminate_, i_vec, j_vec, current.i_);
    words[j_ind + 1].push_back(new_state);
  }
}

bool StateCmp::operator()(const State &first, const State &second) const {
  if (first.not_terminate_ != second.not_terminate_)
    return first.not_terminate_ < second.not_terminate_;
  if (first.i_ != second.i_)
    return first.i_ < second.i_;
  if (first.alpha_.size() != second.alpha_.size())
    return first.alpha_.size() < second.alpha_.size();
  if (first.betta_.size() != second.betta_.size())
    return first.betta_.size() < second.betta_.size();
  for (int i = 0; i < first.alpha_.size(); ++i) {
    if (first.alpha_[i] < second.alpha_[i])
      return true;
    if (first.alpha_[i] > second.alpha_[i])
      return false;
  }
  for (int i = 0; i < first.betta_.size(); ++i) {
    if (first.betta_[i] < second.betta_[i])
      return true;
    if (first.betta_[i] > second.betta_[i])
      return false;
  }
  return false;
}

bool Algo::predict(const std::string &word) {
  for (int i = 0; i < word.size(); ++i)
    if (IsBigLetter(word[i]))
      return false;
  std::vector<std::vector<State>> levels(word.size() + 1);
  char start_symbol = '@';
  std::vector<char> alpha_begin;
  std::vector<char> betta_begin = {'S'};
  State begin_state(start_symbol, alpha_begin, betta_begin, 0);
  levels[0].push_back(begin_state);
  std::set<State, StateCmp> help;
  for (int i = 0; i < levels[0].size(); ++i) {
    State current = levels[0][i];
    help.insert(current);
    Predict(levels, current, 0);
    Complete(levels, current, 0);
  }
  for (int i = 0; i < word.size(); ++i) {
    levels[i].resize(0);
    std::set<State>::iterator it = help.begin();
    while (it != help.end())
      levels[i].push_back(*(it++));

    for (int j = 0; j < levels[i].size(); ++j) {
      State current = levels[i][j];
      Scan(levels, current, i, word[i]);
    }
    help.clear();
    for (int j = 0; j < levels[i + 1].size(); ++j) {
      State current = levels[i + 1][j];
      help.insert(current);
      Predict(levels, current, i + 1);
      Complete(levels, current, i + 1);
    }
  }
  for (int i = 0; i < levels[word.size()].size(); ++i) {
    State current = levels[word.size()][i];
    if (current.not_terminate_ == start_symbol && current.betta_.size() == 0 &&
        current.alpha_.size() == 1 && current.alpha_[0] == 'S')
      return 1;
  }
  return false;
}
