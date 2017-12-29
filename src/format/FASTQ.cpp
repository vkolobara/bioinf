//
// Created by vkolobara on 12/29/17.
//

#include <fstream>
#include "FASTQ.h"

const vector<string> &FASTQ::getNames() const {
    return names;
}

const vector<string> &FASTQ::getSequences() const {
    return sequences;
}

const vector<string> &FASTQ::getQualities() const {
    return qualities;
}

FASTQ::FASTQ(std::string path) {
    read(path);
}

void FASTQ::read(string path) {
    ifstream file(path);

    string line;

    while(getline(file, line)) {
        if (line.empty()) break;
        string name = line.substr(1);
        names.push_back(name);

        getline(file,line);
        sequences.push_back(line);

        // + sign
        getline(file,line);

        getline(file,line);
        qualities.push_back(line);
    }

    file.close();
}

void FASTQ::write(string path) {
    ofstream file(path);

    auto size = names.size();

    for (int i=0; i<size; i++) {
        file << "@" << names[i] << endl;
        file << sequences[i] << endl;
        file << "+" << endl;
        file << qualities[i] << endl;
    }

    file.close();
}
