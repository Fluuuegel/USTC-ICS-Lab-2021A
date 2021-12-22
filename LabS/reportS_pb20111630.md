# ICS LabS 实验报告

> 张艺耀 PB2011630

## 最终文件目录如下

​	深度为2的树型目录 `tree -L 2` 

```bash
.
├── CMakeCache.txt
├── CMakeFiles
│   ├── 3.22.1
│   ├── CMakeDirectoryInformation.cmake
│   ├── CMakeError.log
│   ├── CMakeOutput.log
│   ├── CMakeTmp
│   ├── Makefile.cmake
│   ├── Makefile2
│   ├── TargetDirectories.txt
│   ├── cmake.check_cache
│   ├── lc3simulator.dir
│   └── progress.marks
├── CMakeLists.txt
├── Makefile
├── cmake_install.cmake
├── include
│   ├── _memory.h
│   ├── common.h
│   ├── register.h
│   └── simulator.h
├── input.txt
├── inputs
│   └── input.txt
├── lc3simulator
├── output.txt
└── src
    ├── main.cpp
    ├── memory.cpp
    ├── register.cpp
    └── simulator.cpp
```



## 前置条件

+ 安装Cmake

+ 安装boost

`port install boost`

boost相关头文件默认存在于`/opt/local/include/`目录下

故需要在c_cpp_properties.json中includepath：

```json
//c_cpp_properities.json
{
    "configurations": [
        {
            "name": "Mac",
            "includePath": [
                "${workspaceFolder}/**",
                "/opt/local/include/"
            ],
            "defines": [],
            "macFrameworkPath": [
                "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks"
            ],
            "compilerPath": "/usr/bin/clang",
            "cStandard": "c17",
            "cppStandard": "c++11",
            "intelliSenseMode": "macos-clang-arm64"
        }
    ],
    "version": 4
}
```

## 代码分析与阅读

### common.h

+ 头文件的包含，全局变量的声明。

```c++
//common.h
#pragma once

#include <iostream>
#include <fstream>
#include <cstdio>
#include <array>
#include <vector>
#include <cmath>
#include <climits>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>

// Boost library
#include <boost/program_options.hpp>

// Application global variables
extern bool gIsSingleStepMode;	//	单步模式
extern bool gIsDetailedMode;	//  细节模式
extern std::string gInputFileName;
extern std::string gRegisterStatusFileName;
extern std::string gOutputFileName;
extern int gBeginningAddress;
```

### _memory.h

类memory_tp的对象为长为65535的int16_t类型数组，其构造函数memory_tp将数组清零。

该类的方法为从文件中读取内存和得到某地址的内存。

由于助教给的原文件名为memory.h 与某库文件冲突，故改名。

```c++
//_memory.h
#include "common.h"

namespace virtual_machine_nsp {
const int kInstructionLength = 16;

inline int16_t TranslateInstruction(std::string &line) {
    // TODO: translate hex mode
    short num = 0; if(line.size() != 16) return 0;
    for (int i = 0; i < kInstructionLength; i++)
        num = (num << 1) | (line[i] & 1);
    return num;
}

const int kVirtualMachineMemorySize = 0xFFFF;

class memory_tp {
    private:
    int16_t memory[kVirtualMachineMemorySize];

    public:
    memory_tp() {
        memset(memory, 0, sizeof(int16_t) * kVirtualMachineMemorySize);
    }
    // Managements
    void ReadMemoryFromFile(std::string filename, int beginning_address=0x3000);
    int16_t GetContent(int address) const;
    int16_t& operator[](int address);
};

}; // virtual machine nsp
```

### register.h

+ 声明virtual_machine_nsp名字空间下的寄存器数和寄存器枚举类型，数出运算符重载。

```c++
//register.h
#include "common.h"
namespace virtual_machine_nsp {
    const int kRegisterNumber = 10;
    enum RegisterName {
        R_R0 = 0,
        R_R1,
        R_R2,
        R_R3,
        R_R4,
        R_R5,
        R_R6,
        R_R7,
        R_PC, // 8
        R_COND // 9
    };
    
    typedef std::array<int16_t, kRegisterNumber> register_tp;
    std::ostream& operator<<(std::ostream& os, const register_tp& reg);
} // virtual machine namespace
```

### simulator.h

+ 声明virtual_machine_nsp名字空间下的操作数枚举类型。声明virtual_machine_tp类，其成员为寄存器Array类型数组和内存（memory）。方法为各个操作数对应的操作和构造函数，执行函数等等。

```c++
//simulator.h
#pragma once

#include "common.h"
#include "register.h"
#include "_memory.h"

namespace virtual_machine_nsp {

enum kOpcodeList {
    O_ADD = 0b0001,
    O_AND = 0b0101,
    O_BR  = 0b0000,
    O_JMP = 0b1100,
    O_JSR = 0b0100,
    O_LD  = 0b0010,
    O_LDI = 0b1010,
    O_LDR = 0b0110,
    O_LEA = 0b1110,
    O_NOT = 0b1001,
    O_RTI = 0b1000,
    O_ST  = 0b0011,
    O_STI = 0b1011,
    O_STR = 0b0111,
    O_TRAP = 0b1111
};

enum kTrapRoutineList {
};

class virtual_machine_tp {
    public:
    register_tp reg;
    memory_tp mem;
    
    // Instructions
    void VM_ADD(int16_t inst);
    void VM_AND(int16_t inst);
    void VM_BR(int16_t inst);
    void VM_JMP(int16_t inst);
    void VM_JSR(int16_t inst);
    void VM_LD(int16_t inst);
    void VM_LDI(int16_t inst);
    void VM_LDR(int16_t inst);
    void VM_LEA(int16_t inst);
    void VM_NOT(int16_t inst);
    void VM_RTI(int16_t inst);
    void VM_ST(int16_t inst);
    void VM_STI(int16_t inst);
    void VM_STR(int16_t inst);
    void VM_TRAP(int16_t inst);

    // Managements
    virtual_machine_tp() {} //无参数构造函数
    virtual_machine_tp(const int16_t address, const std::string &memfile, const std::string &regfile);
    void UpdateCondRegister(int reg);
    void SetReg(const register_tp &new_reg);
    int16_t NextStep();
};

}; // virtual machine namespace
```



### main.cpp

+ 头文件的包含，全局变量的定义，默认初始地址的定义。

```c++
#include "simulator.h"

using namespace virtual_machine_nsp;
namespace po = boost::program_options;

bool gIsSingleStepMode = false;
bool gIsDetailedMode = false;
std::string gInputFileName = "input.txt";
std::string gRegisterStatusFileName = "register.txt";
std::string gOutputFileName = "";
int gBeginningAddress = 0x3000;
```

+ Linux Bash GNU下的转义字符\e

格式控制： `\e[<格式代码>m`

| 代码十进制 |                             作用                             |
| :--------: | :----------------------------------------------------------: |
|     0      | 清除所有格式（常用在格式控制末尾，以免对后序字符串造成影响） |
|     1      |                      加粗 与格式`2`冲突                      |
|     2      |                    字体变暗 与格式`1`冲突                    |
|     3      |                             斜体                             |
|     4      |                            下划线                            |
|     5      |                呼吸闪烁（但有的机器上没效果）                |
|     6      |                             同上                             |
|     7      |            反显（背景色当前景色，前景色当背景色）            |
|     8      |          隐形（字符仍然存在，可以选中，只是看不到）          |
|     9      |                            删除线                            |

EX:

```c++
#include<stdio.h>
int main()
{
        int i;
        printf("输出格式设置：0-9\n");
        for(i=0;i<=9;++i){
                printf("%d: \e[%dmHello,World!\e[0m\n",i,i);
        }
        return 0;
}
```

颜色控制： `\e[<颜色代码>m` *对于同一颜色，背景色代码=前景色代码 + 10*

| 前景色代码 |     颜色      | 背景色代码 |
| :--------: | :-----------: | :--------: |
|     30     |     黑色      |     40     |
|     31     |     红色      |     41     |
|     32     |     绿色      |     42     |
|     33     |     黄色      |     43     |
|     34     |     蓝色      |     44     |
|     35     |     紫色      |     45     |
|     36     |     青色      |     46     |
|     37     | 白色 *亮灰色* |     47     |

EX: 

```c++
#include<stdio.h>
int main()
{
    int i;
    printf("前景色设置：30-37\n");
    for(i=30;i<=37;++i){
        printf("%d: \e[%dmHello World!\e[0m\n",i,i);	//\e[0m不可或缺
    }
    printf("背景色设置：40-47\n");
    for(i=40;i<=47;++i){
        printf("%d: \e[%dmHello World!\e[0m\n",i,i);
    }
    return 0;
}
```

+ boost::program_options 

  *program options是一系列`pair<name,value>`组成的选项列表,它允许程序通过命令行或配置文件来读取这些参数选项。*

```c++
int main(int argc, char **argv) {
    po::options_description desc{"\e[1mLC3 SIMULATOR\e[0m\n\n\e[1mOptions\e[0m"};   //  选项描述器
    desc.add_options()      //为选项描述器增加选项 参数为key，value类型，该选项的描述
        ("help,h", "Help screen")                                                                  //
        ("file,f", po::value<std::string>()->default_value("input.txt"), "Input file")             //
        ("register,r", po::value<std::string>()->default_value("register.txt"), "Register Status") //
        ("single,s", "Single Step Mode")                                                           //
        ("begin,b", po::value<int>()->default_value(0x3000), "Begin address (0x3000)")
        ("output,o", po::value<std::string>()->default_value(""), "Output file")
        ("detail,d", "Detailed Mode");

    po::variables_map vm;   //  选项存储器
    store(parse_command_line(argc, argv, desc), vm);    //parse_command_line()对输入的选项做解析 store()将解析后的结果存入选项存储器
    notify(vm); //  更新外部变量

    if (vm.count("help")) {
        //options_description对象支持流输出， 会自动打印所有的选项信息
        std::cout << desc << std::endl;
        return 0;
    }
    if (vm.count("file")) {
        //variables_map(选项存储器)是std::map的派生类,可以像关联容器一样使用,
        //通过operator[]来取出其中的元素.但其内部的元素类型value_type是boost::any,
        //用来存储不确定类型的参数值,必须通过模板成员函数as<type>()做类型转换后,
        //才能获取其具体值.
        gInputFileName = vm["file"].as<std::string>();
    }
    if (vm.count("register")) {
        gRegisterStatusFileName = vm["register"].as<std::string>();
    }
    if (vm.count("single")) {
        gIsSingleStepMode = true;
    }
    if (vm.count("begin")) {
        gBeginningAddress = vm["begin"].as<int>();
    }
    if (vm.count("output")) {
        gOutputFileName = vm["output"].as<std::string>();
    }
    if (vm.count("detail")) {
        gIsDetailedMode = true;
    }
  ...
}
```

+ 执行simulator部分：

```c++
	virtual_machine_tp virtual_machine(gBeginningAddress, gInputFileName, gRegisterStatusFileName);
    int halt_flag = true;
    int time_flag = 0;
    while(halt_flag) {
        // Single step
        // TO BE DONE
        if (virtual_machine.NextStep() == 0)
            halt_flag = 0;
        if (gIsDetailedMode)
            std::cout << virtual_machine.reg << std::endl;
        ++time_flag;
    }

    std::cout << virtual_machine.reg << std::endl;
    std::cout << "cycle = " << time_flag << std::endl;
    return 0;
```



### memory.cpp

+ 对memory.h中函数的定义

```c++
//memory.cpp
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
```

### register.cpp

+ 打印寄存器值

```c++
//register.cpp
#include "register.h"

namespace virtual_machine_nsp {
    std::ostream& operator<<(std::ostream& os, const register_tp& reg) {
        os << "\e[1mR0\e[0m = " << std::hex << reg[R_R0] << ", ";
        os << "\e[1mR1\e[0m = " << std::hex << reg[R_R1] << ", ";
        os << "\e[1mR2\e[0m = " << std::hex << reg[R_R2] << ", ";
        os << "\e[1mR3\e[0m = " << std::hex << reg[R_R3] << std::endl;
        os << "\e[1mR4\e[0m = " << std::hex << reg[R_R4] << ", ";
        os << "\e[1mR5\e[0m = " << std::hex << reg[R_R5] << ", ";
        os << "\e[1mR6\e[0m = " << std::hex << reg[R_R6] << ", ";
        os << "\e[1mR7\e[0m = " << std::hex << reg[R_R7] << std::endl;
        os << "\e[1mCOND[NZP]\e[0m = " << std::bitset<3>(reg[R_COND]) << std::endl;
        os << "\e[1mPC\e[0m = " << std::hex << reg[R_PC] << std::endl;
        return os;
    }
} // virtual machine namespace
```

### simulator.cpp

+ 较为关键的一个文件 包含了位扩展函数、条件码更新函数、操作函数、主函数（读取文件，设置初始寄存器和地址值）、设置寄存器函数、执行函数的定义。

```c++
//simulator.cpp
#include "simulator.h"

namespace virtual_machine_nsp {
template <typename T, unsigned B>
inline T SignExtend(const T x) {
    // Extend the number
    // TO BE DONE
    short i = 1, j = 0xffff;
    if((i << (B - 1)) & x)    //最高位为1
        return x | (j << (B - 1));
    return x;
}

void virtual_machine_tp::UpdateCondRegister(int regname) {
    // Update the condition register
    // TO BE DONE
    if(reg[regname] == 0) reg[R_COND] = 2;
    else if(reg[regname] < 0) reg[R_COND] = 4;
    else reg[R_COND] = 1;
}

void virtual_machine_tp::VM_ADD(int16_t inst) {
    int flag = inst & 0b100000;
    int dr = (inst >> 9) & 0x7; //0b111
    int sr1 = (inst >> 6) & 0x7;    //0b111
    if (flag) { //  立即数模式
        // add inst number
        int16_t imm = SignExtend<int16_t, 5>(inst & 0b11111);
        reg[dr] = reg[sr1] + imm;
    } else {// 寄存器模式
        // add register
        int sr2 = inst & 0x7;
        reg[dr] = reg[sr1] + reg[sr2];
    }
    // Update condition register
    UpdateCondRegister(dr);
}

void virtual_machine_tp::VM_AND(int16_t inst) {
    // TO BE DONE
    int flag = inst & 0b100000;
    int dr = (inst >> 9) & 0x7; //0b111
    int sr1 = (inst >> 6) & 0x7;    //0b111
    if (flag) { //  立即数模式
        // add inst number
        int16_t imm = SignExtend<int16_t, 5>(inst & 0b11111);
        reg[dr] = reg[sr1] & imm;
    } else {// 寄存器模式
        // add register
        int sr2 = inst & 0x7;
        reg[dr] = reg[sr1] & reg[sr2];
    }
    // Update condition register
    UpdateCondRegister(dr);
}

void virtual_machine_tp::VM_BR(int16_t inst) {
    int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);   //  inst前9位
    int16_t cond_flag = (inst >> 9) & 0x7;
    if (gIsDetailedMode) {
        std::cout << reg[R_PC] << std::endl;
        std::cout << pc_offset << std::endl;
    }
    if (cond_flag & reg[R_COND]) {
        reg[R_PC] += pc_offset;
    }
}

void virtual_machine_tp::VM_JMP(int16_t inst) {
    // TO BE DONE
    int16_t baser = (inst >> 6) & 0x7;
    reg[R_PC] = baser;
}

void virtual_machine_tp::VM_JSR(int16_t inst) {
    // TO BE DONE
    reg[R_R7] = reg[R_PC];
    int16_t pc_offset = SignExtend<int16_t, 11>(inst & 0x7FF);
    reg[R_PC] += pc_offset;
}

void virtual_machine_tp::VM_LD(int16_t inst) {
    int16_t dr = (inst >> 9) & 0x7;
    int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
    reg[dr] = mem[reg[R_PC] + pc_offset];
    UpdateCondRegister(dr);
}

void virtual_machine_tp::VM_LDI(int16_t inst) {
    // TO BE DONE
    int16_t dr = (inst >> 9) & 0x7;
    int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
    reg[dr] = mem[mem[reg[R_PC] + pc_offset]];
    UpdateCondRegister(dr);
}

void virtual_machine_tp::VM_LDR(int16_t inst) {
    // TO BE DONE
    int16_t dr = (inst >> 9) & 0x7;
    int16_t baser = (inst >> 6) & 0x7;
    int16_t offset = SignExtend<int16_t, 6>(inst &0x3F);
    reg[dr] = mem[reg[baser] + offset];
    UpdateCondRegister(dr);
}

void virtual_machine_tp::VM_LEA(int16_t inst) {
    // TO BE DONE
    int16_t dr = (inst >> 9) & 0x7;
    int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
    reg[dr] = reg[R_PC] + pc_offset;
}

void virtual_machine_tp::VM_NOT(int16_t inst) {
    // TO BE DONE
    int16_t dr = (inst >> 9) & 0x7;
    int16_t sr = (inst >> 6) & 0x7;
    reg[dr] = ~ reg[sr];
}

void virtual_machine_tp::VM_RTI(int16_t inst) {
    ; // PASS
}

void virtual_machine_tp::VM_ST(int16_t inst) {
    // TO BE DONE
    int16_t sr = (inst >> 9) & 0x7;
    int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
    mem[reg[R_PC] + pc_offset] = reg[sr];
}

void virtual_machine_tp::VM_STI(int16_t inst) {
    // TO BE DONE
    int16_t dr = (inst >> 9) & 0x7;
    int16_t pc_offset = SignExtend<int16_t, 9>(inst & 0x1FF);
    mem[mem[reg[R_PC] + pc_offset]] = reg[dr];
}

void virtual_machine_tp::VM_STR(int16_t inst) {
    // TO BE DONE
    int16_t dr = (inst >> 9) & 0x7;
    int16_t baser = (inst >> 6) & 0x7;
    int16_t offset = SignExtend<int16_t, 6>(inst &0x3F);
    mem[reg[baser] + offset] = reg[dr];
}

void virtual_machine_tp::VM_TRAP(int16_t inst) {
    int trapnum = inst & 0xFF;
    // if (trapnum == 0x25)
    //     exit(0);
    // TODO: build trap program
    if(trapnum == 0x25) exit(0);
}

virtual_machine_tp::virtual_machine_tp(const int16_t address, const std::string &memfile, const std::string &regfile) {
    // Read memory
    if (memfile != ""){
        mem.ReadMemoryFromFile(memfile);
    }
    
    // Read registers
    std::ifstream input_file;
    input_file.open(regfile);
    if (input_file.is_open()) {
        int line_count = std::count(std::istreambuf_iterator<char>(input_file), std::istreambuf_iterator<char>(), '\n');    //返回[first，last)范围内等于val的元素数
        input_file.close();
        input_file.open(regfile);
        if (line_count >= 8) {
            for (int index = R_R0; index <= R_R7; ++index) {
                input_file >> reg[index];
            }
        } else {
            for (int index = R_R0; index <= R_R7; ++index) {
                reg[index] = 0;
            }
        }
        input_file.close();
    } else {
        for (int index = R_R0; index <= R_R7; ++index) {
            reg[index] = 0;
        }
    }

    // Set address
    reg[R_PC] = address;
    reg[R_COND] = 0;
}

void virtual_machine_tp::SetReg(const register_tp &new_reg) {
    reg = new_reg;
}

int16_t virtual_machine_tp::NextStep() {
    int16_t current_pc = reg[R_PC];
    reg[R_PC]++;
    int16_t current_instruct = mem[current_pc];
    int opcode = (current_instruct >> 12) & 15; //0b1111
    
    switch (opcode) {
        case O_ADD:
            if (gIsDetailedMode) {
                std::cout << "ADD" << std::endl;
            }
            VM_ADD(current_instruct);
            break;
        case O_AND:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "AND" << std::endl;
            }
            VM_AND(current_instruct);
            break;
        case O_BR:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "BR" << std::endl;
            }
            VM_BR(current_instruct);
            break;
        case O_JMP:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "JMP" << std::endl;
            }
            VM_JMP(current_instruct);
            break;
        case O_JSR:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "JSR" << std::endl;
            }
            VM_JSR(current_instruct);
            break;
        case O_LD:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "LD" << std::endl;
            }
            VM_LD(current_instruct);
            break;
        case O_LDI:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "LDI" << std::endl;
            }
            VM_LDI(current_instruct);
            break;
        case O_LDR:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "LDR" << std::endl;
            }
            VM_LDR(current_instruct);
            break;
        case O_LEA:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "LEA" << std::endl;
            }
            VM_LEA(current_instruct);
            break;
        case O_NOT:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "NOT" << std::endl;
            }
            VM_NOT(current_instruct);
            break;
        case O_RTI:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "RTI" << std::endl;
            }
            VM_RTI(current_instruct);
            break;
        case O_ST:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "ST" << std::endl;
            }
            VM_ST(current_instruct);
            break;
        case O_STI:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "STI" << std::endl;
            }
            VM_STI(current_instruct);
            break;
        case O_STR:
        // TO BE DONE
            if (gIsDetailedMode) {
                std::cout << "STR" << std::endl;
            }
            VM_STR(current_instruct);
            break;
        case O_TRAP:
        if (gIsDetailedMode) {
            std::cout << "TRAP" << std::endl;
        }
        if ((current_instruct & 0xFF) == 0x25) {
            reg[R_PC] = 0;
        }
        VM_TRAP(current_instruct);
        break;
        default:
        VM_RTI(current_instruct);
        break;       
    }

    if (current_instruct == 0) {
        // END
        // TODO: add more detailed judge information
        std::cout << std::endl << "The program ends." << std::endl;
        return 0;
    }
    return reg[R_PC];
}

} // namespace virtual_machine_nsp
```

## 程序运行

初次运行应 install symlinks to '/usr/local/bin'

```bash
 sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install
```

在CMakeList.txt所在目录下在终端执行：

```bash
cmake .
make
```

这样就生成了makefile文件。

在终端运行：

<img src="/Users/fluegelcat/Desktop/Snapshot/截屏2021-12-22 下午4.38.39.png" alt="截屏2021-12-22 下午4.38.39" style="zoom:50%;" />

汇编码：

```assembly
.ORIG x3000
	LD R0, N1
	LD R1, N2
LOOP	ADD R7, R7, R1
	ADD R0, R0, #-1
	BRnp LOOP
	ST R7, N3
	TRAP x25
N1 .FILL #8
N2 .FILL #9
N3 .FILL #0
```

该程序计算先将N1，N2中的值分别存入R0，R1，再计算R0 * R1存入R7。

使用assembler转成机器码存入input.txt内文件如下：

```
0010000000000110
0010001000000110
0001111111000001
0001000000111111
0000101111111101
0011111000000011
1111000000100101
0000000000001000
0000000000001001
0000000000000000
```

<img src="/Users/fluegelcat/Desktop/Snapshot/截屏2021-12-22 下午4.45.24.png" alt="截屏2021-12-22 下午4.45.24" style="zoom:50%;" />

最终结果如下：

<img src="/Users/fluegelcat/Desktop/Snapshot/截屏2021-12-22 下午4.44.47.png" alt="截屏2021-12-22 下午4.44.47" style="zoom:50%;" />

R7中存入x0048 PC为x3006 结果正确。
