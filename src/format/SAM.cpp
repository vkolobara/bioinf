//
// Created by vkolobara on 1/6/18.
//

#include <sstream>
#include <fstream>
#include <iostream>
#include "SAM.h"


char complement(char seq) {
    switch (seq) {
        case 'A': return 'T';
        case 'T': return 'A';
        case 'C': return 'G';
        case 'G': return 'C';
        default: return seq;
    }
}

string inverseComplement(string sequence) {
    string ret;

    for (int i=sequence.size()-1; i>=0; i--) {
        auto c = sequence[i];
        ret.push_back(complement(c));
    }

    return ret;
}

void SAM::read(std::string path) {
    ifstream file(path);

    string line;

    while(getline(file, line)) {
        if (line.empty()) break;
        if (line[0] == '@') continue;
        SAMRow row(line);
        if (row.flag == 4) {
            continue;
        }
        row.cigarToAlignment();
        row.cigar.clear();
        row.seq.clear();
        row.qual.clear();
        row.rname.clear();
        row.rnext.clear();
        rows.push_back(row);
    }

    file.close();
}

void SAM::write(std::string path) {
    ofstream file(path);

    for (auto row : rows) {
        file << row.write() << endl;
    }

    file.close();
}

SAM::SAM(string path) {
    read(std::move(path));
}

const vector<SAMRow> &SAM::getRows() const {
    return rows;
}

SAMRow::SAMRow(string line) {
    read(std::move(line));
}

void SAMRow::read(string line) {
    istringstream iss(line);

    iss >> qname >> flag >> rname >> pos
        >> mapq
        >> cigar >> rnext >> pnext >> tlen
        >> seq >> qual;

    if (flag == 16) {
        seq = inverseComplement(seq);
    }

    if (flag != 4) {
        this->cigarToAlignment();
        cigar.clear();
        seq.clear();
    }
}

string SAMRow::write() {
    ostringstream oss;
    oss << qname << "\t" << flag << "\t" << rname << "\t" << pos << "\t"
        << mapq << "\t"
        << cigar << "\t" << rnext << "\t" << pnext << "\t" << tlen << "\t"
        << inverseComplement(seq) << "\t" << qual;

    return oss.str();}

void SAMRow::cigarToAlignment() {
    for (int i=0, pos=0; i<cigar.size();) {

        string num;
        while(i < cigar.size() && isdigit(cigar[i++])) {
            num.push_back(cigar[i-1]);
        }
        i--;
        char op = cigar[i++];

        auto n = stol(num);

        switch(op){
            case 'M':{
                while (n>0) {
                    alignment += seq[pos++];
                    n--;
                }
                break;
            }
            case 'D': {
                while(n>0){
                    alignment+='-';
                    n--;
                }
            break;
            }
            case 'I':{
                insertions[pos] = seq.substr(pos, n);
                pos+=n;
                break;
            }
            case 'S':{
                pos+=n;
                break;
            }
            default: break;
        }
    }
}

