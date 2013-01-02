#include <iostream>
#include <stdlib.h>
#include "Server.h"

using namespace std;

int main(int argc, char *argv[])
{
     if (argc < 2)
     {
         std::cerr << "ERROR, no port provided" << std::endl;
         exit(1);
     }

     Server server = Server(atoi(argv[1]));
     return 0;
}
