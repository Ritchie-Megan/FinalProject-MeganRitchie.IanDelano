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

// Functions
void LoadFileToHash(ifstream &inFile);
vector<string> getJobDescription(string &jobName);
vector<string> getJobDescriptionFromHash(const string &jobName);

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

void LoadFileToHash(ifstream &inFile) {
    if (inFile.is_open()) {
        string header1, header2;
        getline(inFile, header1, ',');
        getline(inFile, header2, ',');

        int newJobs = 0;
        string currentJob;

        while (newJobs < 100000 && getline(inFile, currentJob, ',')) {
            //get the word
            string description;
            getline(inFile, description, ',');
            //turn all words lowercase: https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
            transform(description.begin(), description.end(), description.begin(), ::tolower);

            //If there is " at the begining, we know that it is the first in the tuple, meaning it is the Job title
            if (description.find('\"') == 0) {
                //cout << "[NEW JOB]" << endl;
                currentJob = description.substr(1, description.size());
                newJobs++;
                vector<string> tempVec;
                myMap[currentJob] = tempVec;
                description = description.substr(1, description.size());
                //cout << description << endl;
            }
                // if there is a " but it isn't in the first position that means it is the last descpiptor of the job
            else if (description.find('\"') != -1 && description.find('\"') != 0) {
                //cout << "[END OF JOB POSTING FOUND, WEBSITE RIGHT AFTER]" << endl;
                int test = description.find("\"");
                description = description.substr(0,test);
                description = description.substr(1, description.size());
                //check for duplicates
                vector<string> currentListOfDescription = getJobDescription(currentJob);
                auto iter = find(currentListOfDescription.begin(), currentListOfDescription.end(), description);
                if (iter == currentListOfDescription.end()) {
                    myMap[currentJob].push_back(description);
                    //cout << description << endl;
                }
            }
                //else we just put in the word, eliminating the space in front of it, checking to make sure it isn't already there
            else {
                description = description.substr(1, description.size());

                //check for duplicates
                vector<string> currentListOfDescription = getJobDescription(currentJob);
                auto iter = find(currentListOfDescription.begin(), currentListOfDescription.end(), description);
                if (iter == currentListOfDescription.end()) {
                    myMap[currentJob].push_back(description);
                    //cout << description << endl;
                }
            }
        }
    }
    else {
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
        LoadFileToHash(inFile);

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
        LoadFileToHash(inFile); // Loading to unordered_map instead

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
