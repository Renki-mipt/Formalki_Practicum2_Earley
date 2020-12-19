#include "Earley.h"

int main() {
  std::ifstream fin("C:\\test.txt");
  int n;
  std::string s, word;
  char c;
  std::vector<std::pair<char, std::string>> rules;
  std::cin >> n;
  try {
    for (int i = 0; i < n; ++i) {
      std::cin >> c >> s;
      if (c < 'A' || c > 'Z')
        throw std::runtime_error("Uncorrect input");
      if (s == "EPS")
        s = "";
      for (int j = 0; j < s.size(); ++j) {
        if ((s[j] >= 'A' && s[j] <= 'Z') || (s[j] >= 'a' && s[j] <= 'z'))
          continue;
        throw std::runtime_error("Uncorrect input");
      }
      rules.push_back(std::make_pair(c, s));
    }
  } catch (std::exception &s) {
    std::cout << s.what();
  }
  std::cin >> word;
  Algo Erlie(rules);
  std::cout << Erlie.predict(word);
  return 0;
}
