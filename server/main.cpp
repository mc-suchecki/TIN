/*
 * main.cpp
 *
 *  Created on: 26-12-2012
 *      Author: przemek
 */

#include <stdio.h>
#include <stdlib.h>
#include "Server.hpp"

int main(int argc, char *argv[])
{
     if (argc < 3)
     {
         std::cerr << "ERROR, invalid arguments! (proper call: server [port] [password]))" << std::endl;
         exit(1);
     }

     Server server = Server(atoi(argv[1]), argv[2]);
     return 0;
}


//TODO
//-ignoring errors

//- more proper sending of resultFilesNumber
//- function readCommand/writeAnswer
//- command count
//- communication algotrithm
//- log dictionaties, MAYBE logger
