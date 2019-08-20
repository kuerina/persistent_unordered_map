#include "persistent.h"
#include <iostream>
#include <fstream>
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
int main(int argc,char *argv[]){
    std::ifstream infile { ".last_key" };
    std::string file_contents { std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
    int const indexes = atoi(file_contents.c_str());
    std::vector<int> foo(indexes);
    std::iota(foo.begin(), foo.end(), 1);
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (foo.begin(), foo.end(), std::default_random_engine(seed));
    std::string value;
    long sizeTest = 10000L;
    long sucs =0L;
    if (argc>1){
        sizeTest=atol(argv[1]);
    }
    unordered_map<int,std::string> myPersistentMap (persistent,"testManagedFile","Good_Map",sizeTest, 20);
    for (int& x: foo){
        std::cout << sucs << " out of " << indexes << x<< std::endl;
        value = myPersistentMap.get( x);

        sucs +=1;
    }
    std::cout << sucs << " out of " << indexes << std::endl;
    return 0;

}
