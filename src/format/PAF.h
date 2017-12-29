//
// Created by vkolobara on 12/29/17.
//

#ifndef BIOINF_PAF_H
#define BIOINF_PAF_H

#include "Format.h"

struct PAFRow {
    string queryName;
    int queryLength;
    int queryStart;
    int queryEnd;
    char relativeStrand;

    string targetName;
    int targetLength;
    int targetStart;
    int targetEnd;

    int residueMatches;
    int alignmentLength;
    int mappingQuality;

    string extra;

    explicit PAFRow(string line);
    void read(string line);
    string write();
};

class PAF : public Format{
private:
    vector<PAFRow> rows;
public:
    explicit PAF(string path);
    void read(string path) override;
    void write(string path) override;

    const vector<PAFRow> &getRows() const;
};


#endif //BIOINF_PAF_H
