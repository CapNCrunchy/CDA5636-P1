#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cstdint>
#include <iostream>
using namespace std;

class instruction{
    public:
    string opcode;
    string DES;
    string FSO;
    string SSO;
    int64_t value;
    int64_t F_Value;
    int64_t S_Value;

    instruction(string op, string D, string F, string S){
        opcode = op;
        DES = D;
        FSO = F;
        SSO = S;
        value = -1;
    }
    string getInstructionStr(){
        return "<"+opcode+","+DES+","+FSO+","+SSO+">";
    }
    string getInstructionDecoded(){
        return "<"+opcode+","+DES+","+ to_string(F_Value) +","+ to_string(S_Value) +">";
    }
    string getRegisterValue(){
        return "<" + DES + "," + to_string(value)+">";
    }
};

int dataMemory[8] = {2,4,6,8,10,12,14,16};
uint64_t registers[8] = {4,3,2,1,4,3,2,1};

queue<instruction> INM;
vector<instruction> INB;
vector<instruction> AIB;
vector<instruction> LIB;
vector<instruction> ADB;
queue<instruction> REB;


string printStep(int n){
    string step = "STEP " + to_string(n) + ":\n";
    
    step += "INM:";
    for(int i=0;i<INM.size();i++){
        step+= INM.front().getInstructionStr() +",";
        instruction temp = INM.front();
        INM.push(temp);
        INM.pop();
    }if(INM.size()!=0) step.pop_back();
    step+="\n";

    step += "INB:";
    for(instruction in : INB){
        step+= in.getInstructionDecoded() +",";
    }if(INB.size()!=0) step.pop_back();
    step+="\n";

    step += "AIB:";
    for(instruction in : AIB){
        step+= in.getInstructionDecoded() +",";
    }if(AIB.size()!=0) step.pop_back();
    step+="\n";

    step += "LIB:";
    for(instruction in : LIB){
        step+= in.getInstructionDecoded() +",";
    }if(LIB.size()!=0) step.pop_back();
    step+="\n";

    step += "ADB:";
    for(instruction in : ADB){
        step+= in.getRegisterValue() +",";
    }if(ADB.size()!=0) step.pop_back();
    step+="\n";

    step += "REB:";
    for(int i= 0;i<REB.size();i++){
        step+= REB.front().getRegisterValue() +",";
        instruction temp = REB.front();
        REB.push(temp);
        REB.pop();
    }if(REB.size()!=0) step.pop_back();
    step+="\n";

    step += "RGF:";
    for(int i= 0;i< 8 ;i++){
        step+= "<R"+ to_string(i) + ","+to_string(registers[i])+">" +",";      
    }step.pop_back();
    step+="\n";
    
    step += "DAM:";
    for(int i= 0;i< 8 ;i++){
        step+= "<"+ to_string(i) + ","+to_string(dataMemory[i])+">" +",";   
    }step.pop_back();

    return step;
}

uint64_t ALU_Perform(instruction in){
    int DES = in.DES[1] -'0';
    uint64_t FSO = in.F_Value;
    uint64_t SSO = in.S_Value;
    if(in.opcode == "ADD"){
        return FSO + SSO;
    }
    if(in.opcode == "SUB"){
        return FSO - SSO;
    }
    if(in.opcode == "AND"){
        return FSO & SSO;
    }
    if(in.opcode == "OR"){
        return FSO | SSO;
    }
    return registers[DES];
}





int main(){

    //fill instructions
    INM.push(instruction("ADD","R1","R2","R3"));
    INM.push(instruction("LD","R4","R2","R3"));
    INM.push(instruction("AND","R5","R2","R3"));
    INM.push(instruction("LD","R6","R2","R2"));
    INM.push(instruction("OR","R1","R3","R2"));

    ofstream output;
    output.open("simulation.txt");
    int step = 0;
    output << printStep(step);
    
    while(INM.size() != 0 || INB.size() !=0 || AIB.size() !=0 || LIB.size() !=0 || ADB.size() !=0 || REB.size() !=0){
        step++;
        //Write
        if(REB.size() != 0){
            instruction temp = REB.front();
            REB.pop();
            int pos = temp.DES[1] -'0';
            registers[pos] = temp.value;
        }
        //LOAD
        if(ADB.size() != 0){
            int val = ADB[0].value;
            ADB[0].value = dataMemory[val];
            REB.push(ADB[0]);
            ADB.pop_back();
        }
        //ALU
        if(AIB.size() != 0){
            AIB[0].value = ALU_Perform(AIB[0]);
            REB.push(AIB[0]);
            AIB.pop_back();
        }
        //ADDR
        if(LIB.size() !=0){
            LIB[0].value = LIB[0].F_Value + LIB[0].S_Value;
            ADB.push_back(LIB[0]);
            LIB.pop_back();
        }
        //ISSUE2
        if(INB.size() != 0 && INB[0].opcode == "LD"){
            LIB.push_back(INB[0]);
            INB.pop_back();
        }
        //ISSUE1
        else if(INB.size() != 0){
            AIB.push_back(INB[0]);
            INB.pop_back();
        }
        //DECODE/READ
        if(INM.size() != 0){
            instruction temp = INM.front();
            INM.pop();
            int FSO = temp.FSO[1] - '0';
            int SSO = temp.SSO[1] - '0';
            temp.F_Value = registers[FSO];
            temp.S_Value = registers[SSO];
            INB.push_back(temp);
        }
        output << "\n\n";
        output << printStep(step);
    }
    output.close();
    return 0;
}




