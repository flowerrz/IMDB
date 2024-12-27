#include <iostream>
#include <fstream>

const int bufferSize = 256;

//Functions for strings
int strCompare(const char* str1, const char* str2);
void strCopy(const char* source, char* destination);
unsigned strLength(const char* str);

#pragma region User Structures

enum UserRole {
	Admin,
	RegularUser
};

struct User {
	char* username;
	char* password;
	UserRole role;

	User() {}

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

#pragma region Film Structures

struct Film
{
	char* title;
	int year;
	char* ganre;
	char* director;
	char** actors;

	Film()
	{

	}

	Film(char* inputTitle, int inputYear, char* inputGanre, char* inputDirector, char** inputActors, int actorCount)
	{
		title = new char[strLength(inputTitle) + 1];
		ganre = new char[strLength(inputGanre) + 1];
		director = new char[strLength(inputDirector) + 1];
		strCopy(inputTitle, title);
		strCopy(inputGanre, ganre);
		strCopy(inputDirector, director);

		year = inputYear;

		actors = new char* [actorCount];
		for (int i = 0; i < actorCount; ++i)
		{
			actors[i] = new char[strLength(inputActors[i]) + 1];
			strCopy(inputActors[i], actors[i]);
		}
	}
};

#pragma endregion

//Functions for logging in
bool logInUser();
bool registerUser();
bool promptUser();
void getUserInput(char*& username, char*& password);

//Functions for the user's choices
void displayChoiceMenu();
void addFilm();
void findFilmByTitle();
void findFilmByGenre();
void showAllFilms();
void deleteFilm();

User currentUser;

int main()
{
	if (promptUser())
	{
		displayChoiceMenu();
	}
	else
	{
		std::cout << "An error ocurred. Please try again." << std::endl;
	}
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

#pragma region UserLogin

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
		if (ch == '\n' || ch == '\0') {
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

bool logInUser()
{
	bool isSuccessfullLogin = false;

	std::ifstream file("usersFile.txt");
	if (!file) {
		std::cerr << "File could not be opened!" << std::endl;
		return false;
	}

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
			isSuccessfullLogin = true;
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
	username = nullptr;
	password = nullptr;

	return isSuccessfullLogin;
}

bool registerUser()
{
	std::ofstream UsersFile("usersFile.txt", std::ios::app);

	if (!UsersFile.is_open())
	{
		std::cout << "An error occurred while opening the file" << std::endl;
		return false;
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
	username = nullptr;
	password = nullptr;

	UsersFile.close();

	return true;
}

bool promptUser()
{
	std::cout << "Would you like to\n" << "1.Log In\n" << "2.Register" << std::endl;

	bool isSuccessfull = false;

	char choice;
	do
	{
		std::cin >> choice;
		switch (choice)
		{
		case '1':
			isSuccessfull = logInUser();
			break;
		case '2':
			isSuccessfull = registerUser();
			break;
		default:
			std::cout << "Invalid choice" << std::endl;
		}
	} while (choice != '1' && choice != '2');

	return isSuccessfull;
}

#pragma endregion

#pragma region UserChoice

const int MAX_STRING_LENGTH = 50;

void getFilmInput(char*& title, int& year, char*& ganre, char*& director, char**& actors, int& actorsCount)
{
	std::cout << "Please enter title, year, ganre, director and actors of the film." << std::endl;

	char* titleArr = new char[MAX_STRING_LENGTH];
	char* ganreArr = new char[MAX_STRING_LENGTH];
	char* directorArr = new char[MAX_STRING_LENGTH];
	title = titleArr;
	ganre = ganreArr;
	director = directorArr;

	char ch;
	int i = 0;

	std::cout << "Title: " << std::endl;

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while (std::cin.get(ch) && i < MAX_STRING_LENGTH - 1) {
		if (ch == '\n' || ch == '\0') {
			break;
		}
		title[i++] = ch;
	}
	title[i] = '\0';

	std::cout << "Year: " << std::endl;
	std::cin >> year;

	std::cout << "Ganre: " << std::endl;

	i = 0;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while (std::cin.get(ch) && i < MAX_STRING_LENGTH - 1) {
		if (ch == '\n' || ch == '\0') {
			break;
		}
		ganre[i++] = ch;
	}
	ganre[i] = '\0';

	std::cout << "Director: " << std::endl;

	i = 0;
	while (std::cin.get(ch) && i < MAX_STRING_LENGTH - 1) {
		if (ch == '\n' || ch == '\0') {
			break;
		}
		director[i++] = ch;
	}
	director[i] = '\0';

	std::cout << "How many actors star in the film?" << std::endl;

	int amountOfActors;
	std::cin >> amountOfActors;
	actorsCount = amountOfActors;

	char** actorsArr = new char* [amountOfActors];
	actors = actorsArr;

	for (int i = 0; i < amountOfActors; ++i) {
		actors[i] = new char[MAX_STRING_LENGTH];
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	for (int i = 0; i < amountOfActors; i++)
	{
		char actor[MAX_STRING_LENGTH];
		int j = 0;
		std::cout << "Actor " << i + 1 << std::endl;
		while (std::cin.get(ch) && j < MAX_STRING_LENGTH - 1) {
			if (ch == '\n' || ch == '\0') {
				break;
			}
			actor[j++] = ch;
		}
		actor[j] = '\0';

		actors[i] = new char[strlen(actor) + 1];
		strCopy(actor, actors[i]);
	}
}

void displayChoiceMenu()
{
	char choice;
	while (true)
	{
		std::cout << "What would you like to do?\n"
			<< "1. Add a new film\n"
			<< "2. Search for a film by title\n"
			<< "3. Search for a film by ganre\n"
			<< "4. Show all movies\n"
			<< "5. Update film\n"
			<< "6. Delete film\n"
			<< "7. Rate film\n"
			<< "8. Sort and filter films by rating\n"
			<< "9. Exit" << std::endl;
		std::cin >> choice;
		switch (choice)
		{
		case '1':
			if (currentUser.role == RegularUser)
			{
				std::cout << "Only admins can add new films" << std::endl;
			}
			addFilm();
			break;
		case '2':
			findFilmByTitle();
			break;
		case '3':
			findFilmByGenre();
			break;
		case '4':
			showAllFilms();
			break;
		case '5':

			break;
		case '6':
			if (currentUser.role == RegularUser)
			{
				std::cout << "Only admins can delete films" << std::endl;
			}
			deleteFilm();
			break;
		case '7':

			break;
		case '8':

			break;
		case '9':
			std::cout << "You exited";
			return;
		default:
			std::cout << "Invalid choice" << std::endl;
		}
	}
}

void addFilm()
{
	std::ofstream FilmFile("filmsFile.txt", std::ios::app);

	if (!FilmFile.is_open())
	{
		std::cout << "An error occurred while opening the file" << std::endl;
		return;
	}

	char* title = nullptr;
	int year;
	char* ganre = nullptr;
	char* director = nullptr;
	char** actors;
	int actorsCount;

	getFilmInput(title, year, ganre, director, actors, actorsCount);

	Film film(title, year, ganre, director, actors, actorsCount);

	FilmFile << film.title << "\n"
		<< film.year << "\n"
		<< film.ganre << "\n"
		<< film.director << std::endl;

	for (int i = 0; i < actorsCount; i++)
	{
		FilmFile << film.actors[i] << " ";
	}

	FilmFile << "\n-\n";

	delete[] title;
	delete[] ganre;
	delete[] director;
	delete[] actors;
	title = nullptr;
	ganre = nullptr;
	director = nullptr;

	FilmFile.close();
}

void findFilmByTitle()
{
	std::cout << "------------------------" << std::endl;

	std::cout << "Title: " << std::endl;

	char title[MAX_STRING_LENGTH];
	int i = 0;
	char ch;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while (std::cin.get(ch) && i < MAX_STRING_LENGTH - 1) {
		if (ch == '\n' || ch == '\0') {
			break;
		}
		title[i++] = ch;
	}
	title[i] = '\0';

	std::ifstream FilmFile("filmsFile.txt", std::ios::app);

	char buffer[bufferSize];

	if (!FilmFile.is_open()) {
		return;
	}

	int idx = 0;
	while (FilmFile.getline(buffer, bufferSize))
	{
		if (idx > 0)
		{
			switch (idx)
			{
			case 1:
				std::cout << "Year: " << buffer << "\n";
				break;
			case 2:
				std::cout << "Ganre: " << buffer << "\n";
				break;
			case 3:
				std::cout << "Director: " << buffer << "\n";
				break;
			case 4:
				std::cout << "Actors: " << buffer << "\n";
				break;
			}
			idx++;
		}

		if (buffer[0] == '-')
		{
			idx = 0;
		}

		if (strCompare(buffer, title) == 0)
		{
			idx++;
		}
	}

	FilmFile.close();

	std::cout << "------------------------" << std::endl;
}

void findFilmByGenre()
{
	std::cout << "------------------------" << std::endl;

	std::cout << "Genre: " << std::endl;

	char genre[MAX_STRING_LENGTH];
	int i = 0;
	char ch;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while (std::cin.get(ch) && i < MAX_STRING_LENGTH - 1) {
		if (ch == '\n' || ch == '\0') {
			break;
		}
		genre[i++] = ch;
	}
	genre[i] = '\0';

	std::ifstream FilmFile("filmsFile.txt", std::ios::app);

	char buffer[bufferSize];
	char title[MAX_STRING_LENGTH];
	char year[MAX_STRING_LENGTH];

	if (!FilmFile.is_open()) {
		return;
	}

	int idx = 0;
	bool isMatch = false;
	while (FilmFile.getline(buffer, bufferSize))
	{
		if (idx == 0)
		{
			strCopy(buffer, title);
		}
		else if (idx == 1)
		{
			strCopy(buffer, year);
		}

		if (isMatch)
		{
			switch (idx)
			{
			case 3:
				std::cout << "Director: " << buffer << "\n";
				break;
			case 4:
				std::cout << "Actors: " << buffer << "\n";
				isMatch = false;
				idx = -1;
				break;
			}
		}

		if (buffer[0] == '-')
		{
			idx = -1;
		}

		if (strCompare(buffer, genre) == 0)
		{
			std::cout << "Title: " << title << "\n";
			std::cout << "Year: " << year << "\n";
			isMatch = true;
		}

		idx++;
	}

	FilmFile.close();

	std::cout << "------------------------" << std::endl;
}

void showAllFilms()
{
	std::cout << "------------------------" << std::endl;

	std::ifstream FilmFile("filmsFile.txt", std::ios::app);

	char buffer[bufferSize];
	while (FilmFile.getline(buffer, bufferSize))
	{
		std::cout << buffer << std::endl;
	}

	FilmFile.close();

	std::cout << "------------------------" << std::endl;
}

void deleteFilm()
{
	std::cout << "------------------------" << std::endl;

	std::cout << "Title: " << std::endl;

	char title[MAX_STRING_LENGTH];
	int i = 0;
	char ch;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while (std::cin.get(ch) && i < MAX_STRING_LENGTH - 1) {
		if (ch == '\n' || ch == '\0') {
			break;
		}
		title[i++] = ch;
	}
	title[i] = '\0';

	std::ifstream FilmFile("filmsFile.txt", std::ios::app);
	std::ofstream TempFile("tempFile.txt");

	if (!FilmFile.is_open()) {
		std::cout << "Error: Unable to open filmsFile.txt for reading." << std::endl;
		return;
	}

	if (!TempFile.is_open()) {
		std::cout << "Error: Unable to open tempFile.txt for writing." << std::endl;
		return;
	}

	char buffer[bufferSize];
	bool inDeleteFilm = false;
	while (FilmFile.getline(buffer, bufferSize))
	{
		if (strCompare(title, buffer) == 0)
		{
			inDeleteFilm = true;
		}

		if (!inDeleteFilm)
		{
			TempFile << buffer << "\n";
		}

		if (buffer[0] == '-')
		{
			inDeleteFilm = false;
		}
	}

	FilmFile.close();
	TempFile.close();

	if (std::remove("filmsFile.txt") != 0 || std::rename("tempFile.txt", "filmsFile.txt") != 0) {
		std::cout << "Error: Unable to update filmsFile.txt." << std::endl;
	}
	else {
		std::cout << "Film" << title << "successfully deleted." << std::endl;
	}

	std::cout << "------------------------" << std::endl;
}

#pragma endregion
