#include "textfileformat.h"

#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include "Value.h"

using namespace std;


void writeText(const FileContent & content, const std::string & filename) {
    ofstream outputFile(filename, ofstream::trunc);
    Value v&;

    // write out the header
    for (auto keyValue: content.header()) {
        outputFile << get<0>(keyValue); << "=";

        v = get<1>(keyValue);
        switch(v.type()) {
            case ValueType::BOOL:
                outputFile << ((v.asBool()) ? "true" : "false" ) << endl;
                break;
            case ValueType::INTEGER:
                outputFile << v.asInt() << endl;
                break;
            case ValueType::FLOAT:
                outputFile << v.asFloat() << endl;
                break;
            case ValueType::STRING:
                outputFile << v.asString() << endl;
                break;
        }
    }

    // write out the body
    std::copy(content.body().begin(), content.body.end(), std::ostreambuf_iterator<char>(outputFile));
}

FileContent readText(const std::string & filename, bool skipHeader) {
    fstream inputFile(filename);
    FileContent::Header headi;
    std::pair<std::string, Value> keyVal;

    // read header first
    if (skipHeader) {
        for (string line; getline(filename, line);) {
            if (line.compare("") == 0) break;
        }
    } else {
        for (string line; getline(filename, string);) {
            if (line.compare("") == 0) break;

            auto index = line.find('=');
            
            if (index != std::string::npos) {
                headi.push_back(std::make_pair(
                    line.substr(0,index),
                    Value(line.substr(index+1))
                ));
            }
        }
    }

    // now read the body
    // determine body size
    auto bodyBegin = inputFile.tellg();
    inputFile.seekg(bodyBegin, inputFile.end);
    auto bodySize = inputFile.tellg() - bodyBegin;
    FileContent::Body bodi(bodySize);
    inputFile.seekg(bodyBegin);
    // read directly into string
    inputFile.read(&bodi[0], bodySize);

    return FileContent{headi, bodi};
}
