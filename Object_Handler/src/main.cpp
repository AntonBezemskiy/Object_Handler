#include <iostream>
#include <fstream>
#include <iomanip>

#include "structures.h"
#include "list.h"
#include "test.h"

using namespace std;

const static int Accuracy = 15;

void ProcessRead(List& list){
    std::string message = "\nEnter the path and name of the file\n(to use default file parameters enter the command \"-d\").";
    cout << message << '\n';

    std::string file_name = "Test.txt";

    std::string s;
    if(std::cin >> s){
        if(s != "-d" && s != "d"){
            file_name = s;
        }
    }else{
        return;
    }

    std::ifstream infile(file_name, ios::in | ios::binary);
    if(!infile.is_open()){
        cout << "----------Can't open file: " << file_name << "---------\n\n" ;
        return;
    }

    Object object;
    while(infile >> object.name >> object.x >> object.y >> object.type >> object.time){
        list.AddObject(object);
    }
}

void ProcessAdd(List& list){
    std::string message = "Enter the object data on one line\n";
    std::cout << message ;
    Object object;

    if(std::cin >> object.name >> object.x >> object.y >> object.type >> object.time){
        list.AddObject(object);
    }else{
        std::cout << "Wrong object data\n";
    }
}

void ProcessMake(List& list){
    std::string message = "Choose and enter the type of grouping: \n"
                          "-name : grouping by name\n"
                          "-distance : grouping by distance\n"
                          "-type : grouping by type\n"
                          "-time : grouping by time\n";
    cout << message << '\n';
    std::string type;
    if(cin >> type){
        if(type == "-name" || type == "name" || type == "n"){
            list.MakeGroups(0);
        }else if(type == "-distance" || type == "distance" || type == "d"){
            list.MakeGroups(1);
        }else if(type == "-type" || type == "type"){
            std::cout << "  Enter count element in type (or enter default value 5)" << '\n';
            int count;
            std::cin >> count;
            list.MakeGroups(2,count);
        }
        else if(type == "-time" || type == "time"){
            list.MakeGroups(3);
        }else{
            std::cout << "Wrong type of grouping\n";
        }
    }else{
        cout << "Invalid type of group" << '\n';
    }
}

void ProcessPrint(List& list){
    const auto& grops = list.GetGroups();
    cout << "-----\n\n";
    for(const auto& [name_group, list_obj]: grops){
        cout << ' ' << name_group << '\n';
        for(const auto& obj: list_obj){
            if(auto sh_obj = obj.lock()){
                cout << "   " << sh_obj->name << ' ' << std::setprecision(Accuracy) << sh_obj->x << ' ' << sh_obj->y << ' ';
                cout << sh_obj->type << ' ' << sh_obj->time << '\n';
            }else{
                throw std::invalid_argument("CompareByName: weak_ptr is expired");
            }

        }
    }
    cout << "-----\n\n";
}

void ProcessSave(List& list){
    std::string message = "\nEnter the path and name of the save file\n(to use default file parameters enter the command \"-d\").";
    cout << message << '\n';
    std::string file_name = "Test_Out.txt";

    std::string s;
    if(std::cin >> s){
        if(s != "-d" && s != "d"){
            file_name = s;
        }
    }else{
        return;
    }

    std::ofstream outfile(file_name, ios::in | ios::binary);
    if(!outfile.is_open()){
        cout << "----------Can't open file " << file_name << " ---------\n\n" ;
        return;
    }


    const auto& grops = list.GetGroups();
    outfile << "-----\n\n";
    for(const auto& [name_group, list_obj]: grops){
        outfile << ' ' << name_group << '\n';
        for(const auto& obj: list_obj){
            if(auto sh_obj = obj.lock()){
                outfile << "   " << sh_obj->name << ' ' << std::setprecision(Accuracy) <<  sh_obj->x << ' ' << sh_obj->y << ' ';
                outfile << sh_obj->type << ' ' << sh_obj->time << '\n';
            }else{
                throw std::invalid_argument("CompareByName: weak_ptr is expired");
            }

        }
    }
    outfile << "-----\n\n";
}

void RunProgram(List& list){
    std::string welcome = "\nLet's start! \n\n";
    std::string list_command = "  -read  : this command for read list object from file \n"
                          "  -add   : this command for add object to list \n"
                          "  -make  : this command for make group \n"
                          "  -print : this command to print group \n"
                          "  -save  : this command save result in the file \n\n";
    std::cout << welcome << list_command << std::endl;
    std::string command;

    while(1){
        std::cout << "Print command" << '\n';
        if(std::cin >> command){
            if(command == "-read" || command == "read" || command == "r"){
                ProcessRead(list);
                continue;
            }else if(command == "-add" || command == "add" || command == "a"){
                ProcessAdd(list);
                continue;
            }else if(command == "-make" || command == "make" || command == "m"){
                ProcessMake(list);
                continue;
            }else if(command == "-print" || command == "print" || command == "p"){
                ProcessPrint(list);
                continue;
            }else if(command == "-save" || command == "save" || command == "s"){
                ProcessSave(list);
                continue;
            }
            std::cout << "\nUnknown command, try again \n\n" <<list_command << std::endl;
        }else{
            std::cout << "\nUnknown command, try again \n\n" <<list_command << std::endl;
            continue;
        }

    }

}

int main()
{
    //test::Test();

    List list;
    RunProgram(list);
}
