/*
 * ClientAuthenticator.hpp
 *
 *  Created on: 16-01-2013
 *      Author: przemek
 */

#pragma once
#include <string>

using std::string;

class ClientAuthenticator
{
	public:
		ClientAuthenticator(const string&);
		virtual ~ClientAuthenticator() {}

		string getChallenge();
		bool verifyPassword(string receivedPassword);

	private:
		string const password;
		string currentChallenge;

		string generateRandomString();
};
