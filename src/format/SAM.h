//
// Created by vkolobara on 1/6/18.
//

#ifndef BIOINF_SAM_H
#define BIOINF_SAM_H


#include "Format.h"
#include <map>

struct SAMRow{
    string qname;
    int flag;
    string rname;
    int pos;
    int mapq;

    string cigar;
    string alignment;

    string rnext;
    int pnext;
    int tlen;

    string seq;
    string qual;

    explicit SAMRow(string line);
    void read(string line);
    string write();
    void cigarToAlignment();

};

class SAM : public Format{
private:
    vector<SAMRow> rows;
public:

    explicit SAM(string path);

    void read(std::string path) override;

    void write(std::string path) override;

    const vector<SAMRow> &getRows() const;
};


#endif //BIOINF_SAM_H
