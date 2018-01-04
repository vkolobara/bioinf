//
// Created by vkolobara on 12/29/17.
//

#ifndef BIOINF_FASTA_H
#define BIOINF_FASTA_H

#include "Format.h"

class FASTA : public Format {
private:
    std::string name;
public:
    std::string sequence;
    explicit FASTA(std::string path);
    void read(std::string path) override;
    void write(std::string path) override;

    const std::string &getName() const;

    const std::string &getSequence() const;
};


#endif //BIOINF_FASTA_H
