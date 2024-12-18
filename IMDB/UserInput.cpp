#include <iostream>
#include <fstream>

#include "UserInputHeader.h"

const int MAX_USERNAME_SIZE = 25;
const int MAX_PASSWORD_SIZE = 15;

enum UserRole {
	Admin,
	User
};

struct User {
	char* username;
	char* password;
	UserRole role;
};

void promptUser()
{
	std::cout << "Would you like to\n" << "1.Log In\n" << "2.Register" << std::endl;

	int choice;
	do
	{
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			logInUser();
			break;
		case 2:
			registerUser();
			break;
		default:
			std::cout << "Invalid choice";
		}
	} while (choice != 1 && choice != 2);
}

void logInUser()
{
}

void registerUser()
{
	std::ofstream UsersFile("usersFile.txt");

	std::cout << "Please enter username and password";
	char* username[MAX_USERNAME_SIZE];
	char* password[MAX_PASSWORD_SIZE];

	//TODO: Get user input; Create a user object

	UsersFile.close();
}