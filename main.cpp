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

//Map
map<string, vector<string>> myMap;
unordered_map<string, vector<string>> myHashMap;

//Functions
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

void LoadFileToHash(ifstream &inFile) {
    if (inFile.is_open()) {
        string header1, header2;
        getline(inFile, header1, ',');
        getline(inFile, header2, ',');

        int newJobs = 0;
        string currentJob;

        while (newJobs < 100000) {
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
                myHashMap[currentJob] = tempVec;
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
                vector<string> currentListOfDescription = getJobDescriptionFromHash(currentJob);
                auto iter = find(currentListOfDescription.begin(), currentListOfDescription.end(), description);
                if (iter == currentListOfDescription.end()) {
                    myHashMap[currentJob].push_back(description);
                    //cout << description << endl;
                }
            }
                //else we just put in the word, eliminating the space in front of it, checking to make sure it isn't already there
            else {
                description = description.substr(1, description.size());

                //check for duplicates
                vector<string> currentListOfDescription = getJobDescriptionFromHash(currentJob);
                auto iter = find(currentListOfDescription.begin(), currentListOfDescription.end(), description);
                if (iter == currentListOfDescription.end()) {
                    myHashMap[currentJob].push_back(description);
                    //cout << description << endl;
                }
            }
        }
    }
    else {
        cout << "Error: Couldn't open file." << endl;
    }
}


void LoadFileToMapOpposite(ifstream &inFile) {
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
            //turn all words lowercase: https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
            transform(description.begin(), description.end(), description.begin(), ::tolower);

            //If there is " at the begining, we know that it is the first in the tuple, meaning it is the Job title
            if (description.find('\"') == 0) {
                //cout << "[NEW JOB]" << endl;
                currentJob = description.substr(1, description.size());
                newJobs++;
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
                //if it is there we need to check if it is in the vector, if it isn't put it in
                if (myMap.find(description) != myMap.end()) {
                    vector<string> temp = myMap[description];
                    bool isThere = false;
                    for (string job: temp) {
                        if (job == currentJob) {
                            isThere = true;
                        }
                    }
                    if (!isThere) {
                        myMap[description].push_back(currentJob);
                    }

                }
                    //we just need to create a new one with the current job
                else {
                    myMap[description].push_back(currentJob);
                }

            }
                //else we just put in the word, eliminating the space in front of it, checking to make sure it isn't already there
            else {
                description = description.substr(1, description.size());
                //check for duplicates
                //if it is there we need to check if it is in the vector, if it isn't put it in
                if (myMap.find(description) != myMap.end()) {
                    vector<string> temp = myMap[description];
                    bool isThere = false;
                    for (string job: temp) {
                        if (job == currentJob) {
                            isThere = true;
                        }
                    }
                    if (!isThere) {
                        myMap[description].push_back(currentJob);
                    }

                }
                    //we just need to create a new one with the current job
                else {
                    myMap[description].push_back(currentJob);
                }
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
        //cout << "Would you like to search jobs or search skills(j/s)?" << endl;
        answer = "j";
        if (answer == "j") {
            cout << "Retrieving Job Listings..." << endl;
            auto start = high_resolution_clock::now();
            //find the file and open
            ifstream inFile("src/job_skills.csv");
            //load into the map
            LoadFileToMap(inFile);

            //stop clock and report time
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout << "Time to put into map: " << duration.count() << " microseconds" << endl;
            //reset cin so we can use it again
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            bool going = true;
            while (going) {
                //ask for name of job
                cout << "Please type in a job name to see its description(q to quit): " << endl;
                string jobName;
                getline(cin, jobName);
                transform(jobName.begin(), jobName.end(), jobName.begin(), ::tolower);

                if (jobName != "q") {
                    //get corresponding vector and return, printing out the vector if it has items
                    cout << "Job requirements/benefits for " << jobName << ":" << endl;
                    vector<string> jobRequirementsBenefits = getJobDescription(jobName);
                    if (!jobRequirementsBenefits.empty()) {
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
                    going = false;
                }
            }
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Loading Jobs into Map..." << endl;
            ifstream inFile("src/job_skills.csv");
            //load into the map
            LoadFileToMapOpposite(inFile);


            //get user skills
            cout << "Please enter you skills(separate by commas):" << endl;
            string userSkills;
            getline(cin, userSkills);

            //get vector of skills
            vector<string> vectorOfSkills;
            auto found = userSkills.find(',');
            while (found != -1) {
                auto findSpace = userSkills.find(' ');
                if (findSpace == 0) {
                    vectorOfSkills.push_back(userSkills.substr(1,found-1));
                    //cout << "adding to the vector: " << userSkills.substr(1, found-1) << endl;
                }
                else {
                    vectorOfSkills.push_back(userSkills.substr(0,found));
                    //cout << "adding to the vector: " << userSkills.substr(0, found) << endl;
                }
                userSkills = userSkills.substr(found+1, userSkills.size());
                found = userSkills.find(',');
            }


            if (myMap.find("housekeeping") != myMap.end()) {
                cout << "it's in there" << endl;
            }
            /*
            vector<string> listOfPossibleJobs;
            for (string skill: vectorOfSkills){
                if (myMap.find(skill) != myMap.end()) {
                    vector<string> tempJobs = myMap[skill];
                    for (string job: tempJobs) {
                        auto iter = find(listOfPossibleJobs.begin(), listOfPossibleJobs.end(), job);
                        if (iter == listOfPossibleJobs.end()) {
                            listOfPossibleJobs.push_back(job);
                        }
                    }
                }
            }
            cout << "\nPossible Job choices: " << endl;
            for (int i = 0; i < listOfPossibleJobs.size()-1; i++) {
                cout << listOfPossibleJobs[i];
            }
            cout << listOfPossibleJobs[listOfPossibleJobs.size()];
            /*
             * print out vector of skills
            vectorOfSkills.push_back(userSkills);
            cout << "adding to the vector: " << userSkills << endl;

            cout << "In the vector: ";
            for (string s: vectorOfSkills) {
                cout << s << " ";
            }
             */

            /*
             * now that the skills are loaded, go through all the map and find jobs with those skillsets
             * the next round only go through the jobs on the list, unless there aren't any in the list, then go through the map again
            map<string, vector<string>>::iterator iter;
            vector<string> listOfPossibleJobs;
            for (auto i = myMap.begin(); i != myMap.end(); i++) {
                cout << "currently looking: " << i->first << endl;
                for (string skill: i->second) {
                    if (skill == vectorOfSkills[0]) {
                        listOfPossibleJobs.push_back(i->first);
                        break;
                    }
                }
            }
            if (!listOfPossibleJobs.empty()) {
                for (string job: listOfPossibleJobs) {
                    cout << job << ", " << endl;
                }
            }
            else {
                cout << "no list of jobs" << endl;
            }
            */
        }

    }
    else if (answer == "h") {
        cout << "Retrieving Job Listings using Hashmaps..." << endl;
        auto start = high_resolution_clock::now();

        ifstream inFile("src/job_skills.csv");
        LoadFileToHash(inFile); // Loading to unordered_map instead

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time to put into hashmap: " << duration.count() << " microseconds" << endl;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        bool going = true;
        while (going) {
            cout << "Please type in a job name to see its description: " << endl;
            string jobName;
            getline(cin, jobName);
            transform(jobName.begin(), jobName.end(), jobName.begin(), ::tolower); // Added transform here

            if (jobName != "q") {
                cout << "Job requirements/benefits for " << jobName << ":" << endl;
                vector<string> jobRequirementsBenefits = getJobDescriptionFromHash(
                        jobName); // Using getJobDescriptionFromHash
                if (!jobRequirementsBenefits.empty()) {
                    for (int i = 0; i < jobRequirementsBenefits.size() - 1; i++)
                        cout << jobRequirementsBenefits[i] << ", ";
                    cout << jobRequirementsBenefits.back() << endl;
                } else {
                    cout << "Not found" << endl;
                }
            }
            else {
                going = false;
            }
        }
    }
    else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}
