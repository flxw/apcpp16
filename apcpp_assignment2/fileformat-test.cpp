#include <fstream>
#include <iostream>
#include <chrono>
#include <functional>
#include <algorithm>
#include <random>

#include "FileContent.h"
#include "textfileformat.h"
#include "binaryfileformat.h"
#include "fileformat-testdata.h"


using namespace std;
using namespace chrono;


namespace {


using Clock = high_resolution_clock;


vector<char> generateRawData(size_t size) {
    default_random_engine rng{ random_device{}() };
    vector<char> data(size);
    generate_n(data.begin(), data.size(), rng);
    return data;
}

FileContent::Header getHeader() {
    FileContent::Header header;
    createHeader(header);
    return header;
}

const FileContent g_content{ getHeader(), generateRawData(20971520) }; // 20 MB


}  // anonymous namespace


auto measure(const string & name, const function<void()> & f) {
    const auto begin = Clock::now();
    f();
    const auto end = Clock::now();
    cout << name << ": " << duration_cast<milliseconds>(end - begin).count() << " ms" << endl;
};



int main(int argc, char * argv[]) {
    //
    // Test Text
    //
    {
        // write test data
        measure("Text write", [] {
            writeText(g_content, "data.txt");
        });

        // read back data
        FileContent contentText;
        measure("Text read", [&contentText] {
            contentText = readText("data.txt");
        });

        // verify data
        cout << "Text correct: " << (contentText == g_content ? "yes" : "no") << endl;
    }


    //
    // Test Binary
    //
    {
        // write test data
        measure("Binary write", [] {
            writeBinary(g_content, "data.bin");
        });

        // read back data
        FileContent contentBinary;
        measure("Binary read", [&contentBinary] {
            contentBinary = readBinary("data.bin");
        });

        // verify data
        cout << "Binary correct: " << (contentBinary == g_content ? "yes" : "no") << endl;
    }


    return 0;
}