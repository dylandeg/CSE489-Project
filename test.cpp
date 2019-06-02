#include <unistd.h>
#include <string.h>
#include <cstring>
#include <iostream>

using namespace std;

int main(){
    // cout << "What do you say?(cin)" << endl;
    // cout << "You: ";
    
    // char * input;
    // cin >> input;

    // cin.clear();
    // cin.ignore();
    // cout << input << endl;

    // cout << "What do you say?(cin.getline())" << endl;
    // cout << "You: ";
    
    // char input2[512];
    // cin.getline(input2, sizeof(input2), '\n');
    // cin.clear();
    // cin.ignore();
    // cout << input2 << endl;

    while(true){
        cout << "What do you say?(getline())" << endl;
        cout << "You: ";
        
        string ins;
        getline(cin, ins);
        char input3[512];
        strcpy(input3, ins.c_str());

        cout << input3 << endl;
        if(input3 == "q"){
            break;
        }
    }
    
}