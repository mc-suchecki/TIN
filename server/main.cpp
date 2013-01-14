/*
 * main.cpp
 *
 *  Created on: 26-12-2012
 *      Author: przemek
 */

#include <stdio.h>
#include <stdlib.h>
#include "Server.hpp"

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


//TODO
//-ignoring errors

//- more proper sending of resultFilesNumber
//- function readCommand/writeAnswer
//- command count
//- communication algotrithm
//- log dictionaties, MAYBE logger
