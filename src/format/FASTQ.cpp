//
// Created by vkolobara on 12/29/17.
//

#include <fstream>
#include <sstream>
#include <utility>
#include "FASTQ.h"

FASTQ::FASTQ(std::string path) {
    read(std::move(path));
}

void FASTQ::read(string path) {
    ifstream file(path);

    string line;

    while(getline(file, line)) {
        if (line.empty()) break;
        vector<string> lines;
        lines.push_back(line);

        getline(file,line);
        lines.push_back(line);

        // + sign
        getline(file,line);
        lines.push_back(line);

        getline(file,line);
        lines.push_back(line);

        readings.emplace_back(lines);
    }

    file.close();
}

void FASTQ::write(string path) {
    ofstream file(path);

    for (auto reading : readings) {
        file << reading.write() << endl;
    }

    file.close();
}

const vector<Reading> &FASTQ::getReadings() const {
    return readings;
}


void Reading::read(vector<string> lines) {
    if (lines.size() != 4) {
        //TODO: ERROR
    }
    name = lines[0].substr(1);
    sequence = lines[1];
    quality = lines[3];

}

Reading::Reading(vector<string> line) {
    read(std::move(line));
}

string Reading::write() {
    ostringstream oss;
    oss << name << "\n" << sequence << "\n+\n" << quality;
    return oss.str();
}
