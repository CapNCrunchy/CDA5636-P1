#include <fstream>
#include <string>
#include <vector>
using namespace std;

int dataMemory[8] = {2,4,6,8,10,12,14,16};
uint64_t registers[8] = {4,3,2,1,4,3,2,1};

vector<instruction> INM;
vector<instruction> INB;
vector<instruction> AIB;
vector<instruction> LIB;
vector<instruction> ADB;
vector<instruction> REB;

int main(){
    ofstream output;
    output.open("simulation.txt");
    


    return 0;
}

string printStep(int n){
    string step = "Step " + to_string(n) + ":\n";
    
    step += "INM:";
    for(instruction in : INM){
        step+= in.getInstructionStr();
    }step+="\n";

    step += "AIB:";
    for(instruction in : AIB){
        step+= in.getInstructionStr();
    }step+="\n";

    return step;
}

class instruction{
    public:
    string opcode;
    string DES;
    string FSO;
    string SSO;

    instruction(string op, string D, string F, string S){
        opcode = op;
        DES = D;
        FSO = F;
        SSO = S;
    }
    string getInstructionStr(){
        return "<"+opcode+","+DES+","+FSO+","+SSO+">";
    }
};