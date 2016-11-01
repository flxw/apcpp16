#include <iostream>
#include <string>
#include <regex>
#include <list>
#include <tuple>
#include <sstream>

#include <fstream>

using namespace std;

tuple<string,string> splitEmailAddress(const string& stringToBeSplit) {
  stringstream splitstream;
  string seq, segment1, segment2;
  splitstream.str(stringToBeSplit);

  for (int i=0; getline(splitstream, seq, '@') && i<2; ++i) {
    switch(i) {
      case 0: segment1 = seq; break;
      case 1: segment2 = seq; break;
    }
  }

  return make_tuple(segment1, segment2);
}

bool sortValidByDomainAndName(const string& first, const string& second) {
  tuple<string,string> firstSplitString, secondSplitString;

  firstSplitString  = splitEmailAddress(first);
  secondSplitString = splitEmailAddress(second);

  short firstStringMatchesFirstRule = get<0>(firstSplitString).compare(get<0>(secondSplitString));
  bool  firstStringMatchesSecondRule = get<1>(firstSplitString).compare(get<1>(secondSplitString)) < 0;

  if      (firstStringMatchesFirstRule == 0) return firstStringMatchesSecondRule;
  else if (firstStringMatchesFirstRule < 0)  return true;
  else return false; 
}

void printList(const list<string>& s, ostream &os) {
	list<string>::const_iterator i;

	for( i = s.begin(); i != s.end(); ++i) os << *i << endl;
}

int main(int argc, char * argv[]) {
  regex emailRegex {"^[a-zA-Z0-9][-a-zA-Z0-9_]*(\\.[_a-zA-Z0-9-]+)*@[a-zA-Z0-9][a-zA-Z0-9-]+(\\.[a-zA-Z0-9-]+)*(\\.[a-zA-Z]{2,4})$", regex::ECMAScript};
  list<string> validEmails, invalidEmails;

  // read commandline arguments and set flags
  bool shouldPrintOnlyValid = false;
  bool shouldPrintOnlyInvalid = false;

  // operating modes are exclusive
  for (int i=1; i < argc; ++i) {
    if (string("--print-valid").compare(argv[i]) == 0) {
      shouldPrintOnlyValid   = true;
      shouldPrintOnlyInvalid = false;
    } else if (string("--print-invalid").compare(argv[i]) == 0) {
      shouldPrintOnlyValid   = false;
      shouldPrintOnlyInvalid = true;
    }
  }

  // read in emails until end of line
  for(string line; getline(cin, line); ) {
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

    if(regex_match(line, emailRegex)) {
      validEmails.push_back(line);
    } else {
      invalidEmails.push_back(line);
    }
  }

  // now print everything
  if (shouldPrintOnlyValid) {
    validEmails.sort(sortValidByDomainAndName);
    printList(validEmails, cout);
  } else if (shouldPrintOnlyInvalid) {
    printList(invalidEmails, cout);
  } else {
    printList(validEmails, cout);
    printList(invalidEmails, cerr);
  }

  return 0;
}
