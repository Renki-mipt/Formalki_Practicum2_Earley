#include "Earley.h"
#include <gtest/gtest.h>

TEST(IsBigLetter, BigAndSmall) {
  char big_A = 'A', small_a = 'a';
  char big_S = 'S', small_s = 's';
  char big_Z = 'Z', small_z = 'z';
  EXPECT_TRUE(IsBigLetter(big_A));
  EXPECT_TRUE(IsBigLetter(big_S));
  EXPECT_TRUE(IsBigLetter(big_Z));
  EXPECT_TRUE(!IsBigLetter(small_a));
  EXPECT_TRUE(!IsBigLetter(small_s));
  EXPECT_TRUE(!IsBigLetter(small_z));
}

TEST(Algo, Scan) {
  std::vector<std::pair<char, std::string>> jumps;
  Algo Earley(jumps);
  std::vector<std::vector<State>> levels(2);
  char not_terminiate = 'T';
  std::vector<char> alpha = {'d'};
  std::vector<char> betta = {'a'};
  int i = 1;
  State current(not_terminiate, alpha, betta, i);
  int j_ind = 0;
  char right_symbol = 'a';
  char wrong_symbol = 'c';
  Earley.Scan(levels, current, j_ind, right_symbol);
  Earley.Scan(levels, current, j_ind, wrong_symbol);
  EXPECT_EQ(levels[1].size(), 1);
  if (levels[1].size() == 1) {
    EXPECT_EQ(levels[1][0].alpha_.size(), 2);
    EXPECT_EQ(levels[1][0].betta_.size(), 0);
    if (levels[1][0].alpha_.size() == 2) {
      EXPECT_EQ(levels[1][0].alpha_[0], 'd');
      EXPECT_EQ(levels[1][0].alpha_[1], 'a');
    }
  }
}

TEST(Algo, Predict) {
  std::vector<std::pair<char, std::string>> jumps;
  jumps.push_back(std::make_pair('S', "bTa"));
  jumps.push_back(std::make_pair('T', ""));
  Algo Earley(jumps);
  std::vector<std::vector<State>> levels(2);
  char not_terminiate = 'T';
  std::vector<char> alpha = {'a'};
  std::vector<char> betta = {'S'};
  int i = 0;
  State current(not_terminiate, alpha, betta, i);
  int j_ind = 1;
  Earley.Predict(levels, current, j_ind);
  EXPECT_EQ(levels[1].size(), 1);
  if (levels[1].size() == 1) {
    EXPECT_EQ(levels[1][0].alpha_.size(), 0);
    EXPECT_EQ(levels[1][0].betta_.size(), 3);
    if (levels[1][0].alpha_.size() == 3) {
      EXPECT_EQ(levels[1][0].alpha_[0], 'b');
      EXPECT_EQ(levels[1][0].alpha_[1], 'T');
      EXPECT_EQ(levels[1][0].alpha_[2], 'a');
    }
    EXPECT_EQ(levels[1][0].i_, 1);
  }
}

TEST(Algo, Complete) {
  std::vector<std::pair<char, std::string>> jumps;
  Algo Earley(jumps);
  std::vector<std::vector<State>> levels(2);
  char not_terminiate1 = 'A';
  std::vector<char> alpha1 = {'a'};
  std::vector<char> betta1 = {'B', 'b'};
  int i1 = 0;
  State prev(not_terminiate1, alpha1, betta1, i1);
  char not_terminiate2 = 'B';
  std::vector<char> alpha2 = {'c'};
  std::vector<char> betta2;
  int i2 = 0;
  State current(not_terminiate2, alpha2, betta2, i2);
  levels[0].push_back(prev);
  int j_ind = 1;
  Earley.Complete(levels, current, j_ind);
  EXPECT_EQ(levels[1].size(), 1);
  if (levels[1].size() == 1) {
    EXPECT_EQ(levels[1][0].not_terminate_, 'A');
    EXPECT_EQ(levels[1][0].alpha_.size(), 2);
    EXPECT_EQ(levels[1][0].betta_.size(), 1);
    if (levels[1][0].alpha_.size() == 2) {
      EXPECT_EQ(levels[1][0].alpha_[0], 'a');
      EXPECT_EQ(levels[1][0].alpha_[1], 'B');
    }
    if (levels[1][0].betta_.size() == 1) {
      EXPECT_EQ(levels[1][0].betta_[0], 'b');
    }
    EXPECT_EQ(levels[1][0].i_, 0);
  }
}

TEST(Algo, predict) {
  std::vector<std::pair<char, std::string>> jumps;
  jumps.push_back(std::make_pair('S', ""));
  jumps.push_back(std::make_pair('S', "aSbS"));
  jumps.push_back(std::make_pair('S', "bSaS"));
  Algo Earley(jumps);
  std::string word1 = "abbaba";
  std::string word2 = "abbaaba";
  EXPECT_EQ(Earley.predict(word1), 1);
  EXPECT_EQ(Earley.predict(word2), 0);
}

TEST(StateCmp, operator_round_bracket) {
  std::set<State, StateCmp> example;
  char not_terminiate1 = 'A';
  std::vector<char> alpha1 = {'a'};
  std::vector<char> betta1 = {'B', 'b'};
  int i1 = 0;
  State first(not_terminiate1, alpha1, betta1, i1);
  char not_terminiate2 = 'B';
  std::vector<char> alpha2 = {'c'};
  std::vector<char> betta2;
  int i2 = 0;
  State second(not_terminiate2, alpha2, betta2, i2);
  example.insert(second);
  example.insert(first);
  example.insert(first);
  EXPECT_EQ(example.size(), 2);
  if (example.size() == 2) {
    std::set<State>::iterator it = example.begin();
    EXPECT_EQ(*it, first);
    EXPECT_EQ(*(++it), second);
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}