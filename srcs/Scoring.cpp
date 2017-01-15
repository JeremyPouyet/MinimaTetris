#include "Scoring.hh"

using namespace std;
using namespace std::chrono;

Scoring::Scoring() {
  std::ifstream		scoringFile(_scoringFilePath, ifstream::out | ifstream::app);
  unsigned int		score;
  std::string		date;

  if (scoringFile.is_open() == false)
    throw "Can't open scoring file";
  while (scoringFile >> score >> date)
    _scores.push_back({score, date});
  scoringFile.close();
}

void			Scoring::addScore(unsigned int score) {
  std::ofstream		scoringFile;
  std::time_t		now_c = system_clock::to_time_t(system_clock::now());
  std::basic_ostringstream<char> ss;
  ss << std::put_time(std::localtime(&now_c), "%b-%d-%Y");
  std::string		today = ss.str();

  _scores.push_back({score, today});
  scoringFile.open(_scoringFilePath, ofstream::out | ofstream::app);
  if (scoringFile.is_open() == false)
    throw "Can't open scoring file";
  scoringFile << score << ' '<< today << '\n';
  scoringFile.close();
}

std::vector<Score>	Scoring::getBestScores(unsigned int n) {
  std::sort(_scores.begin(), _scores.end());
  if (n > _scores.size())
    n = _scores.size();
  return {_scores.begin(), _scores.begin() + n};
}
