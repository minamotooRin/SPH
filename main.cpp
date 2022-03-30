#include "SPH.h"

int main()
{   
    SPH_handle handle( 1, 1, 1, 1000, "water");
    handle.set_export_file("data.bin");
    handle.run(1000);
    return 0;
}