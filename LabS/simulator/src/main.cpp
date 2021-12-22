/*
 * @Author       : Chivier Humber
 * @Date         : 2021-09-14 21:41:49
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-11-24 05:33:45
 * @Description  : file content
 */
#include "simulator.h"

using namespace virtual_machine_nsp;
namespace po = boost::program_options;

bool gIsSingleStepMode = false;
bool gIsDetailedMode = false;
std::string gInputFileName = "input.txt";
std::string gRegisterStatusFileName = "register.txt";
std::string gOutputFileName = "";
int gBeginningAddress = 0x3000;

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
}