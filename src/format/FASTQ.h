//
// Created by vkolobara on 12/29/17.
//

#ifndef BIOINF_FASTQ_H
#define BIOINF_FASTQ_H

#include "Format.h"

struct Reading {
    string name;
    string sequence;
    string quality;

    Reading(const string &name, const string &sequence, const string &quality);

    explicit Reading(vector<string> line);
    void read(vector<string> line);
    string write();
};

class FASTQ : public Format {
private:
    vector<Reading> readings;
public:
    explicit FASTQ(std::string path);
    void read(string path) override;
    void write(string path) override;
    const vector<Reading> &getReadings() const;

    void setReadings(const vector<Reading> &readings);

};


#endif //BIOINF_FASTQ_H
