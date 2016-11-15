#include "binaryfileformat.h"

#include <iostream>
#include <fstream>
#include <utility>
#include <tuple>
#include <iomanip>
#include <regex>

using namespace std;

void writeBinary(const FileContent & content, const std::string & filename) {
    ofstream outputFile(filename, iostream::trunc | iostream::binary);
    std::streamsize ss = outputFile.precision();

    // write out magic number
    unsigned short introToken = 0x95CC;
    outputFile.write((char*)&introToken, sizeof(unsigned short));

    // write placeholder for raw data section start
    long rawBodyStart = 0;
    outputFile.write((char*)&rawBodyStart, sizeof(long));

    // write out the header
    for (auto keyValue: content.header()) {
        // format:
        // 2 bits for type
        // 30 bits for number of characters in key
        unsigned int typeInfo = keyValue.first.length();
        Value& v = get<1>(keyValue);

        switch(v.type()) {
            case ValueType::BOOL: {
                typeInfo |= 0 << 31;
                typeInfo |= 0 << 30;
                bool t = keyValue.second.asBool();

                outputFile.write((char *) &typeInfo, sizeof(unsigned int));
                outputFile.write(keyValue.first.c_str(), keyValue.first.size());
                outputFile.write((char *) &t, sizeof(bool));
                }
                break;

            case ValueType::INTEGER: {
                typeInfo |= 0 << 31;
                typeInfo |= 1 << 30;
                int t = keyValue.second.asInt();

                outputFile.write((char *) &typeInfo, sizeof(unsigned int));
                outputFile.write(keyValue.first.c_str(), keyValue.first.size());
                outputFile.write((char *) &t, sizeof(int));
                }
                break;

            case ValueType::FLOAT: {
                typeInfo |= 1 << 31;
                typeInfo |= 0 << 30;
                float t = keyValue.second.asFloat();

                outputFile.write((char *) &typeInfo, sizeof(unsigned int));
                outputFile.write(keyValue.first.c_str(), keyValue.first.size());
                outputFile.write((char *) &t, sizeof(float));
                }
                break;

            case ValueType::STRING: {
                typeInfo |= 1 << 31;
                typeInfo |= 1 << 30;
                string t = keyValue.second.asString();
                unsigned int sz = t.size();

                outputFile.write((char *) &typeInfo, sizeof(unsigned int));
                outputFile.write(keyValue.first.c_str(), keyValue.first.size());
                outputFile.write((char*)&sz, sizeof(unsigned int));
                outputFile.write(t.c_str(), t.size());
                }
                break;
        }
    }

    long beginOfRawBody = outputFile.tellp();
    outputFile.seekp(2);
    outputFile.write((char*)&beginOfRawBody, sizeof(long));
    outputFile.seekp(beginOfRawBody);

    // write out the body
    std::copy(content.body().begin(), content.body().end(), std::ostreambuf_iterator<char>(outputFile));
}

FileContent readBinary(const std::string & filename, bool skipHeader) {
    ifstream inputFile(filename, iostream::binary);
    unsigned short introToken;
    long rawBodyStart;
    int typeIdAndKeyLength;
    FileContent::Header headi;

    // read magic number at the beginning
    inputFile.read((char*)&introToken, sizeof(unsigned short));
    if (introToken != 0x95CC) return FileContent{};

    // find out where the raw body starts
    inputFile.read((char*)&rawBodyStart, sizeof(long));

    while(inputFile.tellg() < rawBodyStart) {
        inputFile.read((char *) &typeIdAndKeyLength, sizeof(unsigned int));

        short typeId = ((typeIdAndKeyLength >> 31) & 1)*2 + ((typeIdAndKeyLength >> 30) & 1);

        // unset the typeId bits
        typeIdAndKeyLength &= ~(1 << 30);
        typeIdAndKeyLength &= ~(1 << 31);

        string key(typeIdAndKeyLength, ' ');
        inputFile.read(&key[0], typeIdAndKeyLength);

        switch(typeId) {
            case 0: {// bool
                bool c;
                inputFile.read((char *) &c, sizeof(bool));
                headi.push_back(make_pair(key, c));
                break;
            }
            case 1: {// int
                int i;
                inputFile.read((char *) &i, sizeof(int));
                headi.push_back(make_pair(key, i));
                break;
            }
            case 2: {// float
                float f;
                inputFile.read((char *) &f, sizeof(float));
                headi.push_back(make_pair(key, f));
                break;
            }
            case 3: {// string
                unsigned int sz;
                inputFile.read((char *) &sz, sizeof(unsigned int));
                string s(sz, ' ');
                inputFile.read(&s[0], sz);
                headi.push_back(make_pair(key, s));
                break;
            }

            default: break;
        }
    }

    // now read binary part
    inputFile.seekg(0, inputFile.end);
    auto bodySize = inputFile.tellg() - rawBodyStart;
    FileContent::Body bodi(bodySize);
    inputFile.seekg(rawBodyStart);
    // read directly into vector
    inputFile.read(&bodi[0], bodySize);

    return FileContent{headi, bodi};
}
