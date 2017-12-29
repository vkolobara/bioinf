//
// Created by vkolobara on 12/29/17.
//

#include <sstream>
#include <utility>
#include <fstream>
#include "PAF.h"

PAFRow::PAFRow(string line) {
    read(std::move(line));
}

void PAFRow::read(string line) {
    istringstream iss(line);

    iss >> queryName >> queryLength >> queryStart >> queryEnd
        >> relativeStrand
        >> targetName >> targetLength >> targetStart >> targetEnd
        >> residueMatches >> alignmentLength >> mappingQuality
        >> extra;
}

string PAFRow::write() {
    ostringstream oss;
    oss << queryName << "\t" << queryLength << "\t" << queryStart << "\t" << queryEnd << "\t"
        << relativeStrand << "\t"
        << targetName << "\t" << targetLength << "\t" << targetStart << "\t" << targetEnd << "\t"
        << residueMatches << "\t" << alignmentLength << "\t" << mappingQuality << "\t"
        << extra;

    return oss.str();
}

void PAF::read(string path) {
    ifstream file(path);

    string line;

    while(getline(file, line)) {
        if (line.empty()) break;
        rows.emplace_back(line);
    }

    file.close();
}

PAF::PAF(string path) {
    read(std::move(path));
}

void PAF::write(string path) {
    ofstream file(path);

    for (auto row : rows) {
        file << row.write() << endl;
    }

    file.close();
}

const vector<PAFRow> &PAF::getRows() const {
    return rows;
}
