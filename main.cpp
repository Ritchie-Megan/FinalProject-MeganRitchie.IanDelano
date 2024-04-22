#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <iterator>
#include <cctype>
#include<bits/stdc++.h>
using namespace std;
using namespace chrono;


//tuples from: https://www.kaggle.com/datasets/asaniczka/1-3m-linkedin-jobs-and-skills-2024?resource=download

map<string, vector<string>> myMap;

void LoadFileToMap(ifstream &inFile) {
    if (inFile.is_open()) {
        //Gets rid of the header lines
        string header1, header2;
        getline(inFile, header1, ',');
        getline(inFile, header2, ',');
        //cout << "Header1: " << header1 << endl;
        //cout << "Header2: " << header2 << endl;


        //Counts up newJobs to 100,000 in the file to make sure it gets all the tuples
        int newJobs = 0;
        string currentJob;
        //while (newJobs < 20) {
        while (newJobs < 100000) {
            //get the word
            string description;
            getline(inFile, description, ',');
            //turn all words lowercase
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
                myMap[currentJob].push_back(description);
                //cout << description << endl;
            }
                //else we just put in the word, eliminating the space in front of it, checking to make sure it isn't already there
            else {
                //todo: make it check if there are duplicates
                description = description.substr(1, description.size());
                myMap[currentJob].push_back(description);
                //cout << description << endl;
            }
        }
        //Here you can see how many are in the set, and see how many duplicate job links there were.
        //cout << "Number of Jobs in this set: " << newJobs << endl;
        //cout << "Number of Jobs in Map: " << myMap.size() << endl;
        /*
         * Printing out Job listings:
        map<string, vector<string>>::iterator iter;
        for (iter = myMap.begin(); iter != myMap.end(); iter++) {
            cout << iter->first << ": ";
            for (string s: iter->second) {
                cout << s << ",";
            }
            cout << endl;
        }
         */
    }
    else {
        cout << "file not open";
    }
}

//returns the vectors that correspond with the name in the Map
vector<string> getJobDescription(string &jobName) {
    if (myMap.find(jobName) != myMap.end()) {
        return myMap[jobName];
    }
    else {
        return {};
    }
}

int main() {
    cout << "Would you like to try Maps or Hashes to find your job(m/h)?" << endl;
    string answer;
    cin >> answer;
    if (answer == "m") {
        cout << "Retrieving Job Listings..." << endl;
        auto start = high_resolution_clock::now();
        //find the file and open
        ifstream inFile("src/job_skills.csv");
        //load into the map
        LoadFileToMap(inFile);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time to put into map: " << duration.count() << " seconds" << endl;
        //reset cin so we can use it again
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        //ask for name of job
        cout << "Please type in a job name to see its description(please capitalize!): " << endl;
        //todo: make all words in map lowercase
        string jobName;
        getline(std::cin, jobName);
        transform(jobName.begin(), jobName.end(), jobName.begin(), ::tolower);
        //get corresponding vector and return, printing out the vector if it has items
        cout << "Job requirements/benefits for " << jobName << ":" << endl;
        vector<string> jobRequirementsBenefits = getJobDescription(jobName);
        if (jobRequirementsBenefits.size() != 0) {
            for (int i = 0; i < jobRequirementsBenefits.size(); i++) {
                cout << jobRequirementsBenefits[i] << ", ";
            }
            cout << jobRequirementsBenefits[jobRequirementsBenefits.size() - 1];
            cout << endl;
        } else {
            cout << "not found" << endl;
        }

    }
    else {

    }

    return 0;
}
