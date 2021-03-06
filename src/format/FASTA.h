//
// Created by vkolobara on 12/29/17.
//

#ifndef BIOINF_FASTA_H
#define BIOINF_FASTA_H

#include "Format.h"

class FASTA : public Format {
private:
    std::string name;
    std::string sequence;

public:
    explicit FASTA(std::string path);
    void read(std::string path) override;
    void write(std::string path) override;

    const std::string &getName() const;

    const std::string &getSequence() const;

    void setName(const string &name);

    void setSequence(const string &sequence);
};


#endif //BIOINF_FASTA_H
