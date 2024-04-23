#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <iterator>
#include <cctype>
#include <algorithm> // Added for transform

using namespace std;
using namespace chrono;

// Maps
map<string, vector<string>> myMap;
unordered_map<string, vector<string>> myHashMap; // Added unordered_map

// Custom hash function using polynomial rolling hash
size_t customHash(const string &s) {
    const int p = 31; // Prime number for hashing
    const int m = 1e9 + 9; // Large prime number for modulo
    size_t hashValue = 0;
    size_t pPow = 1;
    for (char c : s) {
        hashValue = (hashValue + (c - 'a' + 1) * pPow) % m;
        pPow = (pPow * p) % m;
    }
    return hashValue;
}

void LoadFileToMap(ifstream &inFile) {
    if (inFile.is_open()) {
        string header1, header2;
        getline(inFile, header1, ',');
        getline(inFile, header2, ',');

        int newJobs = 0;
        string currentJob;

        while (newJobs < 100000 && getline(inFile, currentJob, ',')) {
            transform(currentJob.begin(), currentJob.end(), currentJob.begin(), ::tolower);

            if (currentJob.front() == '\"')
                currentJob = currentJob.substr(1); // remove opening quote

            auto &jobDescriptions = myMap[currentJob]; // Using myMap for now

            string description;
            getline(inFile, description, ',');
            transform(description.begin(), description.end(), description.begin(), ::tolower);

            if (description.back() == '\"')
                description.pop_back(); // remove closing quote

            auto iter = find(jobDescriptions.begin(), jobDescriptions.end(), description);
            if (iter == jobDescriptions.end())
                jobDescriptions.push_back(description);

            // Same operations for unordered_map with custom hash
            auto &hashJobDescriptions = myHashMap[currentJob];
            auto hashIter = find(hashJobDescriptions.begin(), hashJobDescriptions.end(), description);
            if (hashIter == hashJobDescriptions.end())
                hashJobDescriptions.push_back(description);

            ++newJobs;
        }
    } else {
        cout << "Error: Couldn't open file." << endl;
    }
}

vector<string> getJobDescription(const string &jobName) {
    if (myMap.find(jobName) != myMap.end())
        return myMap[jobName];
    else
        return {};
}

vector<string> getJobDescriptionFromHash(const string &jobName) {
    if (myHashMap.find(jobName) != myHashMap.end())
        return myHashMap[jobName];
    else
        return {};
}

int main() {
    cout << "Would you like to try Maps or Hashes to find your job(m/h)?" << endl;
    string answer;
    cin >> answer;

    if (answer == "m") {
        cout << "Retrieving Job Listings using Maps..." << endl;
        auto start = high_resolution_clock::now();

        ifstream inFile("C://Users//Ian//Downloads//job_skills.csv//job_skills.csv");
        LoadFileToMap(inFile);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time to put into map: " << duration.count() << " microseconds" << endl;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Please type in a job name to see its description: " << endl;
        string jobName;
        getline(cin, jobName);
        transform(jobName.begin(), jobName.end(), jobName.begin(), ::tolower); // Added transform here

        cout << "Job requirements/benefits for " << jobName << ":" << endl;
        vector<string> jobRequirementsBenefits = getJobDescription(jobName);
        if (!jobRequirementsBenefits.empty()) {
            for (size_t i = 0; i < jobRequirementsBenefits.size() - 1; ++i)
                cout << jobRequirementsBenefits[i] << ", ";
            cout << jobRequirementsBenefits.back() << endl;
        } else {
            cout << "Not found" << endl;
        }
    }
    else if (answer == "h") {
        cout << "Retrieving Job Listings using Hashmaps..." << endl;
        auto start = high_resolution_clock::now();

        ifstream inFile("C://Users//Ian//Downloads//job_skills.csv//job_skills.csv");
        LoadFileToMap(inFile); // Loading to unordered_map instead

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time to put into hashmap: " << duration.count() << " microseconds" << endl;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Please type in a job name to see its description: " << endl;
        string jobName;
        getline(cin, jobName);
        transform(jobName.begin(), jobName.end(), jobName.begin(), ::tolower); // Added transform here

        cout << "Job requirements/benefits for " << jobName << ":" << endl;
        vector<string> jobRequirementsBenefits = getJobDescriptionFromHash(jobName); // Using getJobDescriptionFromHash
        if (!jobRequirementsBenefits.empty()) {
            for (int i = 0; i < jobRequirementsBenefits.size() - 1; i++)
                cout << jobRequirementsBenefits[i] << ", ";
            cout << jobRequirementsBenefits.back() << endl;
        } else {
            cout << "Not found" << endl;
        }
    }
    else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}
