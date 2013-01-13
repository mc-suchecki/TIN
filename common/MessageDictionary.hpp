/*
 * MessageDictionary.hpp
 *
 *  Created on: 13-01-2013
 *      Author: przemek
 */
#pragma once

#include <string>

class MessageDictionary
{
	public:
		static const std::string passwordCorrect;
		static const std::string passwordIncorrect;
		static const std::string sendResultFilesNumber;
		static const std::string sendResultFiles;
		static const std::string closeConnection;
};

//DO JACKA
//-oddzielenie wysyłąnia liczby plików i plików, bo zaraz za liczbą do socketa trafi kupę szitu, a wiadomości zapewnią synchronizację
//-metoda close_internal() - dodać na końcu komendę 'closeConnection' ; potwierdzenie poprawnego odbioru i eleganckie rozwiązanie
