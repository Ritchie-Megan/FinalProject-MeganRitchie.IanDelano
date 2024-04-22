#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iterator>
using namespace std;

map<string, vector<string>> myMap;

void LoadFileToMap(ifstream &inFile) {
    if (inFile.is_open()) {

        string header1, header2;
        getline(inFile, header1, ',');
        getline(inFile, header2, ',');
        //cout << "Header1: " << header1 << endl;
        //cout << "Header2: " << header2 << endl;


        int newJobs = 0;
        string currentJob;
        //while (newJobs < 20) {
        while (newJobs < 100000) {
            string description;
            getline(inFile, description, ',');
            if (description.find('\"') == 0) {
                //cout << "[NEW JOB]" << endl;
                currentJob = description.substr(1, description.size());
                newJobs++;
                vector<string> tempVec;
                myMap[currentJob] = tempVec;
                description = description.substr(1, description.size());
                //cout << description << endl;
            }
            else if (description.find('\"') != -1 && description.find('\"') != 0) {
                //cout << "[END OF JOB POSTING FOUND, WEBSITE RIGHT AFTER]" << endl;
                int test = description.find("\"");
                description = description.substr(0,test);
                description = description.substr(1, description.size());
                myMap[currentJob].push_back(description);
                //cout << description << endl;
            }
            else {
                description = description.substr(1, description.size());
                myMap[currentJob].push_back(description);
                //cout << description << endl;
            }
        }
        cout << "Number of Jobs in this set: " << newJobs << endl;
        cout << "Number of Jobs in Map: " << myMap.size() << endl;
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

vector<string> getJobDescription(string &jobName) {
    if (myMap.find(jobName) != myMap.end()) {
        return myMap[jobName];
    }
    else {
        return {};
    }
}

int main() {
    ifstream inFile("src/job_skills.csv");
    LoadFileToMap(inFile);
    cout << "Please type in a job name to see its description(please capitalize the first word only): " << endl;
    string jobName;
    cin >> jobName;
    cout << "Job requirements/benefits for " << jobName << ":" << endl;
    vector<string> jobRequirementsBenefits = getJobDescription(jobName);
    if (jobRequirementsBenefits.size() != 0) {
        for (int i = 0; i < jobRequirementsBenefits.size(); i++) {
            cout << jobRequirementsBenefits[i] << ", ";
        }
        cout << jobRequirementsBenefits[jobRequirementsBenefits.size() - 1];
        cout << endl;
    }
    else {
        cout << "not found" << endl;
    }
    return 0;
}
