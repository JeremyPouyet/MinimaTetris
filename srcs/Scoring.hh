#pragma once

#include <fstream>
#include <chrono>
#include <iomanip> // std::put_time
#include <string>
#include <utility>
#include <algorithm>
#include <sstream>

using Score = std::pair<unsigned int, std::string>;

class Scoring {
public:
  Scoring();

  void	addScore(unsigned int score);
  std::vector<Score>	getBestScores(unsigned int n);

private:
  const std::string _scoringFilePath = "./scores.txt";
  std::vector<Score> _scores;
};
