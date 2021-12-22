/*
 * @Author       : Chivier Humber
 * @Date         : 2021-09-15 21:15:24
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-11-23 16:08:51
 * @Description  : file content
 */
#include "common.h"
#include "_memory.h"

namespace virtual_machine_nsp {
    void memory_tp::ReadMemoryFromFile(std::string filename, int beginning_address) {
        // Read from the file
        // TO BE DONE
        std::ifstream input_file;
        std::string s;
        int16_t tmp = 0;
        input_file.open(filename);
        int addr = beginning_address;
        while(getline(input_file, s)){
            for(int i = 0; i < 16; i++)
                tmp += (s[i] - '0') << (15 - i);
            memory[addr] = tmp;
            tmp = 0;
            addr++;
        }
        input_file.close();
    }

    int16_t memory_tp::GetContent(int address) const {
        // get the content
        // TO BE DONE
        return memory[address];
    }

    int16_t& memory_tp::operator[](int address) {
        // get the content
        // TO BE DONE
        return memory[address];
    }    
}; // virtual machine namespace
