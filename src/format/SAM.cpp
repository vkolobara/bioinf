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
        ret.push_back(complement(sequence[i]));
    }

    return ret;
}

string inverse(string str) {
    string ret;

    for (int i=str.size()-1; i>=0; i--) {
        ret.push_back(str[i]);
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
        if (row.flag & (1<<2)) {
            continue;
        }
        row.cigarToAlignment();
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

    if ((flag & (1<<2)) == 0) {
        this->cigarToAlignment();
    }

   /* if ((flag & (1<<4)) != 0){
        seq = inverseComplement(seq);
        //alignment = inverseComplement(alignment);
        qual = inverse(qual);
    }*/

}

string SAMRow::write() {
    ostringstream oss;
    oss << qname << "\t" << flag << "\t" << rname << "\t" << pos << "\t"
        << mapq << "\t"
        << cigar << "\t" << rnext << "\t" << pnext << "\t" << tlen << "\t"
        << seq << "\t" << qual;

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
                alignment += seq.substr(pos, n);
                pos+=n;
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
                //insertions[pos] = seq.substr(pos, n);
                //alignment += seq.substr(pos, n);
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

