#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include "processor.h"
#include "linux_parser.h"

//tset
#include <iostream>
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<string> prevCpuSate = LinuxParser::CpuUtilization();
    std::this_thread::sleep_for (std::chrono::milliseconds(3000));
    std::vector<string> cpuState = LinuxParser::CpuUtilization();

    //Prev Time
    float prevuser = stof(prevCpuSate[LinuxParser::CPUStates::kUser_]);
    float prevnice = stof(prevCpuSate[LinuxParser::CPUStates::kNice_]);
    float prevsystem = stof(prevCpuSate[LinuxParser::CPUStates::kSystem_]);
    float previdle = stof(prevCpuSate[LinuxParser::CPUStates::kIdle_]);
    float previowait = stof(prevCpuSate[LinuxParser::CPUStates::kIOwait_]);
    float previrq = stof(prevCpuSate[LinuxParser::CPUStates::kIRQ_]);
    float prevsoftIrq = stof(prevCpuSate[LinuxParser::CPUStates::kSoftIRQ_]);
    float prevsteal = stof(prevCpuSate[LinuxParser::CPUStates::kSteal_]);

    //Current Time
    float user = stof(cpuState[LinuxParser::CPUStates::kUser_]);
    float nice = stof(cpuState[LinuxParser::CPUStates::kNice_]);
    float system = stof(cpuState[LinuxParser::CPUStates::kSystem_]);
    float idle = stof(cpuState[LinuxParser::CPUStates::kIdle_]);
    float iowait = stof(cpuState[LinuxParser::CPUStates::kIOwait_]);
    float irq = stof(cpuState[LinuxParser::CPUStates::kIRQ_]);
    float softIrq = stof(cpuState[LinuxParser::CPUStates::kSoftIRQ_]);
    float steal = stof(cpuState[LinuxParser::CPUStates::kSteal_]);

    float PrevIdle = previdle + previowait;
    float Idle = idle + iowait;

    float PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftIrq + prevsteal;
    float NoneIdle = user + nice + system + irq + softIrq + steal;

    float PrevTotal = previdle + PrevNonIdle;
    float Total = Idle + NoneIdle;

    float totald = Total - PrevTotal;
    float idled = Idle - PrevIdle;

    return (totald - idled)/totald;
    //return (Total-idle) / Total;
    }