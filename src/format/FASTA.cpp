//
// Created by vkolobara on 12/29/17.
//

#include <fstream>
#include "FASTA.h"

void FASTA::read(std::string path) {
    ifstream file(path);

    string line;

    getline(file, line);

    name = line.substr(1);


    while(getline(file,line))
    {
        sequence += line;
    }

    file.close();
}


void FASTA::write(std::string path) {
    ofstream file(path);

    file << ">" << name << endl;
    file << sequence << "\n";

    file.close();
}

FASTA::FASTA(std::string path) {
    read(path);
}

const string &FASTA::getName() const {
    return name;
}

const string &FASTA::getSequence() const {
    return sequence;
}


