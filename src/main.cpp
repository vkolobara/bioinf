//
// Created by vkolobara on 12/26/17.
//

#include <iostream>
#include "format/Format.h"
#include "format/PAF.h"

int main(int argc, char *argv[]) {
    PAF paf("../data/lambda_mapping.paf");

    for (auto row : paf.getRows()) {
        cout << row.write() << endl;
    }

}