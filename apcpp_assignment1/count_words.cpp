#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <regex>
#include <map>

using namespace std;

int main(int argc, char * argv[]) {
  // read number of occurrences to print and exit if bad parameter
  int numberOfOccurences = 32;

  if (argc == 2) {
    try {
      numberOfOccurences = stoi(argv[1]);
    } catch (...) {
      cerr << "Wrong type of argument! Only numbers are allowed! Exiting..." << endl;
      return 1;
    }
  } else if (argc > 2) {
    cerr << "Wrong number of arguments! Only a single one is allowed! Exiting..." << endl;
    return 1;
  }

  // open file
  fstream fs("./hgg.txt");
  regex wordRegex("\\w+");
  smatch smatches;
  map<string,int> wordCountMap;
  multimap<int,string> wordSortMap;

  for (string line; getline(fs, line); ) {
    // convert everything to lower
    for (unsigned int j=0; j < line.length(); ++j) line[j] = tolower(line[j]);
    string::const_iterator lineIt = line.cbegin();
    // count words and put into set map
    while (regex_search (lineIt, line.cend(), smatches, wordRegex)) {
      string matchedWord(smatches[0].first, smatches[0].second);
      lineIt = smatches[0].second;

      try {
        wordCountMap[matchedWord]++;
      } catch (...) {
        wordCountMap.insert(pair<string,int>(matchedWord,1));
      }
    }
  }

  // sort map
  for (map<string,int>::const_iterator it = wordCountMap.begin(); it != wordCountMap.end(); ++it) {
    wordSortMap.insert(pair<int,string>(it->second, it->first));
  }

  // print number of occurences accordingly
  multimap<int,string>::const_iterator wsIt = wordSortMap.end();
  wsIt--; // skip spaces
  for (int i = 0;  i < numberOfOccurences && wsIt != wordSortMap.begin(); wsIt--, i++) {
    cout << right << setw(5) << wsIt->first << " " << left << wsIt->second << endl;
  }

  return 0;
}
