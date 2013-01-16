/*
 * ClientAuthenticator.cpp
 *
 *  Created on: 16-01-2013
 *      Author: przemek
 */

#include "ClientAuthenticator.hpp"
#include "../common/MD5.hpp"
#include <cstdlib>
#include <ctime>

using std::string;

ClientAuthenticator::ClientAuthenticator(const string & passwd) : password(passwd) {}

string ClientAuthenticator::getChallenge()
{
	currentChallenge = generateRandomString();
	return currentChallenge;
}

bool ClientAuthenticator::verifyPassword(string receivedEncryptedPassword)
{
	string encryptedPassword = md5(password+currentChallenge);
	if( encryptedPassword.compare(receivedEncryptedPassword) == 0 )
		return true;
	return false;
}

string ClientAuthenticator::generateRandomString()
{
    srand(time(0));
	int length = rand() % 50 + 20;
    string result;
    result.resize(length);

    static string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    for (int i = 0; i < length; i++)
        result[i] = charset[rand() % charset.length()];

    return result;
}

