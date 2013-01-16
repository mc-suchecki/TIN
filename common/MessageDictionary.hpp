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
		static const std::string sendChallenge;
		static const std::string sendFile;
		static const std::string closeConnection;

		static const std::string passwordCorrect;
		static const std::string passwordIncorrect;
		static const std::string endOfFile;
};
