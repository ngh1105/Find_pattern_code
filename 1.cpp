// 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

typedef unsigned char byte_t;
typedef vector<pair<bool, byte_t>> pattern_t;

void read_file(string file_path, vector<byte_t>& data)
{
    data.clear();

    FILE* file = nullptr;
    fopen_s(&file, file_path.c_str(), "rb");

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    data.resize(file_size);
    fread(&data[0], 1, data.size(), file);

    fclose(file);
}

// 11 22 ?? 33
// T  T  F  T

pattern_t to_pattern(string pattern)
{
    pattern_t result;

    // convert pattern string to understandable pattern

    string part;
    stringstream ss(pattern);
    while (getline(ss, part, ' '))
    {
        auto e = pattern_t::value_type(false, 0x00); // default is ignored the ?? byte

        if (part.length() == 2 && isxdigit(part[0]) && isxdigit(part[1])) // if not ?? byte then
        {
            e.first = true;
            e.second = stoi(part, nullptr, 16); // convert a hex string to a number
        }

        result.push_back(e);
    }

    return result;
}

size_t find_pattern(vector<byte_t>& data, string patternn)
{
    if (data.empty() || patternn.empty())
    {
        return -1;
    }

    size_t result = -1;

    // convert string pattern to understandable pattern

    pattern_t pattern = to_pattern(patternn);

    // search pattern in data buffer

    for (size_t i = 0; i < data.size(); ++i) // iterate over all bytes in the data buffer
    {
        size_t j = 0;

        for (j = 0; j < pattern.size(); ++j) // iterate over all bytes in the pattern
        {
            if (pattern[j].first && pattern[j].second != data[i + j]) // if any byte in pattern not matched then
            {
                break; // ignore this time and go to next byte in the data buffer and restart searching pattern from the beginning
            }
        }

        if (j == pattern.size()) // if all bytes in pattern are matched then
        {
            result = i; // return the index as found offset
            break; // and exit searching
        }
    }

    return result;
}

int main(int argc, char const* argv[])
{
    string file_path = ""; // Input file path here. For Example: string file_dir = "E:\\REs\\Tools\\Toolkit\\Mini_Cracker_Tools\\";
    

    // 1. read exe file content into a vector of bytes or called data buffer

    vector<byte_t> data;
    read_file(file_path, data);
    cout << "file size is " << data.size() << endl;

    // 2. find pattern in the data buffer

    string pattern = ""; //Input Pattern  For example:   string pattern = "8B 45 FC 8B 55 F8 E8 ?? ?? ?? ?? 0F 85 ?? ?? ?? ?? 68 40 00 04 00 68 ?? ?? ?? ?? 68 ?? ?? ?? ??";

    auto offset = find_pattern(data, pattern);



    // 3. show the found offset (expected 0x832BA)

    cout << "found at " << hex << offset << endl;

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
