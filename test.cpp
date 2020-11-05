#include <fstream>
#include <iostream>
#include <filesystem>
#include <dirent.h>
using namespace std;
 
int main()
{
    DIR *dir = opendir("./buffer");
    struct dirent *ent = readdir(dir);

    while(ent){
        string d_name = ent->d_name;
        if(d_name!="." && d_name!=".."){
            cout << d_name << endl;
        }

        ent = readdir(dir);
    }

    closedir (dir);
 
    return 0;
}