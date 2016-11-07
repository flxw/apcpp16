#include "textfileformat.h"

#include <iostream>
#include <fstream>
#include <utility>
#include <tuple>
#include <regex>
#include <iomanip>

using namespace std;


void writeText(const FileContent & content, const std::string & filename) {
    ofstream outputFile(filename, iostream::trunc);
    std::streamsize ss = outputFile.precision();

    // write out the header
    for (auto keyValue: content.header()) {
        outputFile << get<0>(keyValue) << "=";

        Value& v = get<1>(keyValue);

        switch(v.type()) {
            case ValueType::BOOL:
                outputFile << ((v.asBool()) ? "true" : "false" ) << endl;
                break;
            case ValueType::INTEGER:
                outputFile << setprecision(ss) << v.asInt() << endl;
                break;
            case ValueType::FLOAT:
                outputFile << fixed << setprecision(10) << v.asFloat() << endl;
                break;
            case ValueType::STRING:
                outputFile << "\"" << v.asString() << "\"" << endl;
                break;
        }
    }

    // insert the specified newline
    outputFile << endl;

    // write out the body
    std::copy(content.body().begin(), content.body().end(), std::ostreambuf_iterator<char>(outputFile));
}

FileContent readText(const std::string & filename, bool skipHeader) {
    fstream inputFile(filename);
    FileContent::Header headi;

    regex booleanRegex{"^(true|false)$"};
    regex integerRegex{"^[+-]{0,1}[0-9]+$"};
    regex floatRegex{"^(([0-9]+(\\.[0-9]{0,}){0,1}e[+-][0-9]+)|([+-]{0,1}[0-9]+\\.[0-9]+))$"};

    // read header first
    if (skipHeader) {
        for (string line; getline(inputFile, line);) {
            if (line.compare("") == 0) break;
        }
    } else {
        for (string line; getline(inputFile, line);) {
            if (line.compare("") == 0) break;

            // find seperator
            auto index = line.find('=');
            // make type distinction
            auto readValueString = line.substr(index+1);

            if (regex_match(readValueString, booleanRegex)) {
                headi.push_back(std::make_pair(
                        line.substr(0, index),
                        (readValueString.compare("true") == 0 ? true : false)
                ));
            } else if (regex_match(readValueString, integerRegex)) {
                headi.push_back(std::make_pair(
                        line.substr(0, index),
                        stoi(readValueString)
                ));
            } else if (regex_match(readValueString, floatRegex)) {
                headi.push_back(std::make_pair(
                        line.substr(0, index),
                        stof(readValueString)
                ));
            } else {
                headi.push_back(std::make_pair(
                        line.substr(0, index),
                        readValueString.substr(1, readValueString.size() - 2)
                ));
            }
        }
    }

    // now read the body
    // determine body size
    auto bodyBegin = inputFile.tellg();
    inputFile.seekg(0, inputFile.end);
    auto bodySize = inputFile.tellg() - bodyBegin;
    FileContent::Body bodi(bodySize);
    inputFile.seekg(bodyBegin);
    // read directly into vector
    inputFile.read(&bodi[0], bodySize);

    return FileContent{headi, bodi};
}
