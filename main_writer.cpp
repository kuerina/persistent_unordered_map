#include "persistent.h"
#include <iostream>
#include <fstream>
int main(int argc,char *argv[]){
    long sizeTest = 10000L;
    if (argc>1){
        sizeTest=atol(argv[1]);
    }
    unordered_map<int, char []> myPersistentMap (persistent,"testManagedFile","Good_Map",sizeTest, 20);
    bool per;
    int x;
    std::string value;
    char payload[1024];
    std::vector<char> paying;
    for (int x=0; x < 1023 ; x++){
        payload[x]='a';
        paying.push_back('a');
    }
    payload[1023]='\0';
    for(int i = 0; i > -1; ++i){
        per =  myPersistentMap.insert(i,payload);
        if (!per){
            std::cout << std::to_string(i) << std::endl;
            x=i;
            break;
        }
    }
    std::ofstream myfile;
    myfile.open (".last_key");
    myfile << x ;
    myfile.close();
    return 0;
}
