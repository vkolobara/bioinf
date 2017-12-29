//
// Created by vkolobara on 12/29/17.
//

#ifndef BIOINF_FORMAT_H
#define BIOINF_FORMAT_H

#include <vector>
#include "string"

using namespace std;

class Format {

public:
    virtual void read(std::string path) = 0;
    virtual void write(std::string path) = 0;
};

#endif //BIOINF_FORMAT_H
