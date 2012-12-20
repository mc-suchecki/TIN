#include <stdio.h>
#include <stdlib.h>
#include "Server.h"

using namespace std;

int main(int argc, char *argv[])
{
     if (argc < 2)
     {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     Server server = Server(atoi(argv[1]));
     return 0;
}
