#ifndef LMINCO_ASSEMBLER_H
#define LMINCO_ASSEMBLER_H

#include <map>
#include <set>

class Assembler {
public:
    struct Command;

    std::string* assembleFile(std::string* file, std::string* ofile);

protected:
    const static std::map<std::string, unsigned short> mnemonics;
    const static std::set<std::string> mnemonicsNodata;
};

#endif //LMINCO_ASSEMBLER_H
