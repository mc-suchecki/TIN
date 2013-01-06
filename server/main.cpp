<<<<<<< HEAD
/*
 * main.cpp
 *
 *  Created on: 26-12-2012
 *      Author: przemek
 */

#include <stdio.h>
=======
#include <iostream>
>>>>>>> upstream/master
#include <stdlib.h>
#include "Server.hpp"

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


//TODO
//- binary messages
//- logger?
