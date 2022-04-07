#include "SPH_handle.h"

int main(int argc, char ** argv)
{  
    std::string ifile("input.json");
    std::string ofile("");
    if(argc > 1)
        ifile = argv[1];
    if(argc > 2)
        ofile = argv[2];

    SPH_handle handle(ifile, ofile);
    handle.run();

    return 0;
}