#include <iostream>
#include <fstream>

int strCompare(const char* str1, const char* str2);
void strCopy(const char* source, char* destination);
unsigned strLength(const char* str);

#pragma region UserStructures

enum UserRole {
	Admin,
	RegularUser
};

struct User {
	char* username;
	char* password;
	UserRole role;

	User()
	{
	}

	User(char* user, char* pass, UserRole r)
	{
		username = new char[strLength(user) + 1];
		password = new char[strLength(pass) + 1];
		strCopy(user, username);
		strCopy(pass, password);
		role = r;
	}
};

#pragma endregion

void logInUser();
void registerUser();
void promptUser();
void getUserInput(char*& username, char*& password);

User currentUser;

int main()
{
	promptUser();



	return 0;
}

#pragma region String Methods

int strCompare(const char* str1, const char* str2)
{
	if (!str1 || !str2)
		return -1;

	while (*str1 && *str2)
	{
		if (*str1 < *str2)
		{
			return -1;
		}
		else if (*str2 < *str1)
		{
			return 1;
		}

		str1++;
		str2++;
	}

	if (!*str1 && !*str2)
	{
		return 0;
	}
	else
	{
		return (*str1 ? 1 : -1);
	}
}

void strCopy(const char* source, char* destination)
{
	if (!*source)
		return;

	char* tempPtr = destination;

	while (*source) {
		*tempPtr = *source;
		source++;
		tempPtr++;
	}

	*tempPtr = '\0';
}

unsigned strLength(const char* str)
{
	if (!str)
		return 0;

	unsigned len = 0;
	while (*str)
	{
		len++;
		str++;
	}

	return len;
}

#pragma endregion

#pragma region UserInput

const int MAX_USERNAME_SIZE = 25;
const int MAX_PASSWORD_SIZE = 15;

void getUserInput(char*& username, char*& password)
{
	std::cout << "Please enter username and password" << std::endl;
	std::cout << "Username: " << std::endl;

	char* usernameArr = new char[MAX_USERNAME_SIZE];
	char* passwordArr = new char[MAX_PASSWORD_SIZE];
	username = usernameArr;
	password = passwordArr;

	char ch;
	int i = 0;

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while (std::cin.get(ch) && i < MAX_USERNAME_SIZE - 1) {
		if (ch == ' ' || ch == '\n' || ch == '\0') {
			break;
		}
		username[i++] = ch;
	}
	username[i] = '\0';

	std::cout << "Password: " << std::endl;

	i = 0;
	while (std::cin.get(ch) && i < MAX_PASSWORD_SIZE - 1) {
		if (ch == ' ' || ch == '\n' || ch == '\0') {
			break;
		}
		password[i++] = ch;
	}
	password[i] = '\0';
}

void logInUser()
{
	std::ifstream file("usersFile.txt");
	if (!file) {
		std::cerr << "File could not be opened!" << std::endl;
		return;
	}

	const int bufferSize = 256;
	char buffer[bufferSize];

	char* username = nullptr;
	char* password = nullptr;
	getUserInput(username, password);

	while (file.getline(buffer, bufferSize))
	{
		char* tempUsername = username;
		char* tempPassword = password;
		char* currentLine = buffer;
		bool sameUsername = true;
		bool samePassword = true;
		while (*currentLine != ' ')
		{
			if (*tempUsername != *currentLine)
			{
				sameUsername = false;
			}
			currentLine++;
			tempUsername++;
		}
		currentLine++;

		while (*currentLine != ' ')
		{
			if (*tempPassword != *currentLine)
			{
				samePassword = false;
			}
			currentLine++;
			tempPassword++;
		}
		currentLine++;

		if (sameUsername && samePassword)
		{
			char currentRole = *currentLine;

			UserRole role;
			if (currentRole == '0')
			{
				role = Admin;
			}
			else
			{
				role = RegularUser;
			}

			User user(username, password, role);
			currentUser = user;
			break;
		}
	}

	file.close();

	delete[] username;
	delete[] password;
}

void registerUser()
{
	std::ofstream UsersFile("usersFile.txt", std::ios::app);

	if (!UsersFile.is_open())
	{
		std::cout << "An error occurred while opening the file" << std::endl;
		return;
	}

	char* username = nullptr;
	char* password = nullptr;
	getUserInput(username, password);

	UserRole role = RegularUser;

	User user(username, password, role);
	currentUser = user;

	UsersFile << user.username << " " << user.password << " " << role << "\n";

	delete[] username;
	delete[] password;

	UsersFile.close();
}

void promptUser()
{
	std::cout << "Would you like to\n" << "1.Log In\n" << "2.Register" << std::endl;

	char choice;
	do
	{
		std::cin >> choice;
		switch (choice)
		{
		case '1':
			logInUser();
			break;
		case '2':
			registerUser();
			break;
		default:
			std::cout << "Invalid choice" << std::endl;
		}
	} while (choice != '1' && choice != '2');
}

#pragma endregion