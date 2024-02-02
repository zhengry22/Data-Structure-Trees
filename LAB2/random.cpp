#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <iterator>
#define HASH_MISS 1000
using namespace std;

int HM = 0;

// Function to read poj.txt and store data in a vector
vector<string> readPojFile(const string& filename) {
    vector<string> data;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            data.push_back(line);
        }
        file.close();
    }
    return data;
}

// Function to generate a random string (for case 1)
string generateRandomString() {
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()-=_+[]{}|;:,.<>?";
    const int stringLength = 8;
    string result;

    for (int i = 0; i < stringLength; ++i) {
        result += charset[rand() % (sizeof(charset) - 1)];
    }

    return result;
}

// Function to generate random data
string generateRandomData0(const vector<string>& pojData, unordered_set<string>& generatedStrings) {
    string result;

    // Select a random line from poj.txt
    int randomLine = rand() % pojData.size();
    vector<string> tokens;
    istringstream iss(pojData[randomLine]);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));

    if (tokens.size() >= 3) {
        string generatedString = tokens[1] + " " + tokens[2];
        generatedStrings.insert(generatedString);
        result = "0 " + generatedString;
    }
    return result;
}

string generateRandomData1(unordered_set<string>& generatedStrings) {
    string result;
    // Determine whether to use an existing string or generate a new one
    if (rand() % HM != 0 && !generatedStrings.empty()) {
        auto it = generatedStrings.begin();
        advance(it, rand() % generatedStrings.size());

        // Split the string and take the second part
        istringstream iss(*it);
        string secondPart;
        iss >> secondPart;  // Skip the first part
        result = "1 " + secondPart;
    } else {
         result = "1 " + generateRandomString();
    }
        

    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <argument>" << std::endl;
        return 1; // Exit with an error code
    }
    freopen("test.txt","w",stdout);
    srand(time(0));  // Seed for random number generation

    // Read poj.txt
    vector<string> pojData = readPojFile("poj.txt");

    // Set to store generated strings
    unordered_set<string> generatedStrings;

    // Generate and print random data
    int zero_Lines = stoi(argv[1]);  // Read from cmd
    int one_Lines = stoi(argv[2]); // Read from cmd
    HM = stoi(argv[3]);
    
    for (int i = 0; i < zero_Lines; ++i) {
        string randomData = generateRandomData0(pojData, generatedStrings);
        cout << randomData << endl;
    }

    for (int i = 0; i < one_Lines; ++i) {
        string randomData = generateRandomData1(generatedStrings);
        cout << randomData << endl;
    }

    // Ensure the last line is '2'
    cout << "2" << endl;

    return 0;
}