#include "Earley.h"

int main() {
  std::ifstream fin("C:\\test.txt");
  int input_size;
  std::string right_part_of_rule, word;
  char left_part_of_rule;
  std::vector<std::pair<char, std::string>> rules;
  std::cin >> input_size;
  try {
    for (int i = 0; i < input_size; ++i) {
      std::cin >> left_part_of_rule >> right_part_of_rule;
      if (left_part_of_rule < 'A' || left_part_of_rule > 'Z') {
        throw std::runtime_error("Uncorrect input");
      }
      if (right_part_of_rule == "EPS") {
        right_part_of_rule = "";
      }
      for (char symbol : right_part_of_rule) {
        if ((symbol >= 'A' && symbol <= 'Z') ||
            (symbol >= 'a' && symbol <= 'z')) {
          continue;
        }
        throw std::runtime_error("Uncorrect input");
      }
      rules.push_back(std::make_pair(left_part_of_rule, right_part_of_rule));
    }
  } catch (std::exception &right_part_of_rule) {
    std::cout << right_part_of_rule.what();
  }
  std::cin >> word;
  Algo Erlie(rules);
  std::cout << Erlie.predict(word);
  return 0;
}
