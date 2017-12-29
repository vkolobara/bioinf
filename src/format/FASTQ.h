//
// Created by vkolobara on 12/29/17.
//

#ifndef BIOINF_FASTQ_H
#define BIOINF_FASTQ_H

#include "Format.h"

class FASTQ : public Format {
private:
    vector<string> names;
    vector<string> sequences;
    vector<string> qualities;
public:
    explicit FASTQ(std::string path);
    void read(string path) override;
    void write(string path) override;

    const vector<string> &getNames() const;

    const vector<string> &getSequences() const;

    const vector<string> &getQualities() const;
};



#endif //BIOINF_FASTQ_H
