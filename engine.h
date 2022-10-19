#ifndef LMINCO_ENGINE_H
#define LMINCO_ENGINE_H

#include <queue>

class Engine {
public:
    ~Engine();

    void load(std::string *file);

    void loadStdin();

    void run();

protected:
    unsigned char *memory = (unsigned char *) calloc(100, 2);
    unsigned char programCounter = 0;
    unsigned short accumulator = 0;
    bool accumulatorFlag = false;
    std::queue<unsigned short> inputs = std::queue<unsigned short>();
    std::queue<unsigned short> outputs = std::queue<unsigned short>();

    bool execute();

    void op_add(unsigned char address);

    void op_sub(unsigned char address);

    void op_sto(unsigned char address);

    void op_lda(unsigned char address);

    void op_br(unsigned char address);

    void op_brz(unsigned char address);

    void op_brp(unsigned char address);

    void op_in();

    void op_out();

    void op_hlt();
};

#endif //LMINCO_ENGINE_H
