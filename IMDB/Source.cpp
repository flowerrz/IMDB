#include <iostream>
#include <fstream>

const int bufferSize = 256;

//Functions for strings
int strCompare(const char* str1, const char* str2);
void strCopy(const char* source, char* destination);
unsigned strLength(const char* str);
int parse(const char* str);

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
	char* actors;
	int rating;
	int peopleCount;

	Film()
	{

	}

	Film(char* inputTitle, int inputYear, char* inputGanre, char* inputDirector, char* inputActors, int inputRating, int peopleCount)
	{
		title = new char[strLength(inputTitle) + 1];
		ganre = new char[strLength(inputGanre) + 1];
		director = new char[strLength(inputDirector) + 1];
		actors = new char[strLength(inputActors) + 1];
		strCopy(inputTitle, title);
		strCopy(inputGanre, ganre);
		strCopy(inputDirector, director);
		strCopy(inputActors, actors);
		year = inputYear;
		rating = (inputRating / peopleCount);
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
void updateInput();
void updateFilm(char* title, char* newValue, int lineIndex);
void deleteFilm();
void rateFilm();
int countFilms();
void sort(int criteria);
void insertionSortByTitle(Film* arr, int size);
void insertionSortByRating(Film* arr, int size);
void printFilms(Film* films, int size);

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

int parse(const char* str)
{
	if (!str)
	{
		return 0;
	}

	bool isNegative = false;
	if (*str == '-')
	{
		isNegative = true;
		str++;
	}

	int result = 0;
	while (*str)
	{
		(result *= 10) += (*str - '0');
		str++;
	}

	if (isNegative)
		result *= -1;

	return result;
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

void getFilmInput(char*& title, int& year, char*& ganre, char*& director, char*& actors, int& actorsCount)
{
	std::cout << "Please enter title, year, ganre, director and actors of the film." << std::endl;

	char* titleArr = new char[MAX_STRING_LENGTH];
	char* ganreArr = new char[MAX_STRING_LENGTH];
	char* directorArr = new char[MAX_STRING_LENGTH];
	char* actorsArr = new char[MAX_STRING_LENGTH * 20];
	title = titleArr;
	ganre = ganreArr;
	director = directorArr;
	actors = actorsArr;

	char ch;
	int i = 0;

	std::cout << "Title: " << std::endl;

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while (std::cin.get(ch) && i < MAX_STRING_LENGTH - 1)
	{
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
	while (std::cin.get(ch) && i < MAX_STRING_LENGTH - 1)
	{
		if (ch == '\n' || ch == '\0') {
			break;
		}
		ganre[i++] = ch;
	}
	ganre[i] = '\0';

	std::cout << "Director: " << std::endl;

	i = 0;
	while (std::cin.get(ch) && i < MAX_STRING_LENGTH - 1)
	{
		if (ch == '\n' || ch == '\0') {
			break;
		}
		director[i++] = ch;
	}
	director[i] = '\0';

	std::cout << "Input actors seperated by a comma" << std::endl;

	i = 0;
	while (std::cin.get(ch) && i < MAX_STRING_LENGTH - 1)
	{
		if (ch == '\n' || ch == '\0') {
			break;
		}
		actors[i++] = ch;
	}
	actors[i] = '\0';
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
			<< "8. Sort films by title\n"
			<< "9. Sort films by rating\n"
			<< "10. Exit" << std::endl;
		std::cin >> choice;
		switch (choice)
		{
		case '1':
			if (currentUser.role == RegularUser)
			{
				std::cout << "Only admins can add new films" << std::endl;
			}
			else
			{
				addFilm();
			}
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
			if (currentUser.role == RegularUser)
			{
				std::cout << "Only admins can update films" << std::endl;
			}
			else
			{
				updateInput();
			}
			break;
		case '6':
			if (currentUser.role == RegularUser)
			{
				std::cout << "Only admins can delete films" << std::endl;
			}
			else
			{
				deleteFilm();
			}
			break;
		case '7':
			rateFilm();
			break;
		case '8':
			sort(1);
			break;
		case '9':
			sort(2);
			break;
		case '10':
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
	char* actors;
	int actorsCount;
	int rating = 5;
	int people = 1;

	getFilmInput(title, year, ganre, director, actors, actorsCount);

	FilmFile << title << "\n"
		<< year << "\n"
		<< ganre << "\n"
		<< director << "\n"
		<< actors << std::endl;

	FilmFile << people << "\n" << rating << "\n-\n";

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
	int idx = 1;
	int ratingIndex = 8;
	int peopleCount = 0;
	while (FilmFile.getline(buffer, bufferSize))
	{
		if (idx == ratingIndex - 1)
		{
			peopleCount = parse(buffer);
		}
		else if (idx == ratingIndex)
		{
			std::cout << ((parse(buffer)) / peopleCount) << std::endl;
			idx = 0;
		}
		else
		{
			std::cout << buffer << std::endl;
		}
		idx++;
	}

	FilmFile.close();

	std::cout << "------------------------" << std::endl;
}

void updateInput()
{
	std::cout << "Enter the title of the film you want to update: " << std::endl;

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

	std::cout << "What would you like to change?\n"
		<< "1. Rating\n"
		<< "2. Year\n"
		<< "3. Genre\n"
		<< "4. Director\n"
		<< "5. Actors" << std::endl;

	char choice;
	std::cin >> choice;

	if (choice == '5')
	{
		std::cout << "Input all actors, split by an interval." << std::endl;
	}
	else
	{
		std::cout << "Enter new value" << std::endl;
	}

	char updateString[MAX_STRING_LENGTH];
	i = 0;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while (std::cin.get(ch) && i < MAX_STRING_LENGTH - 1) {
		if (ch == '\n' || ch == '\0') {
			break;
		}
		updateString[i++] = ch;
	}
	updateString[i] = '\0';

	switch (choice)
	{
	case '1':
		if (updateString[0] < '1' || updateString[0] > 10)
		{
			std::cout << "Invalid score! Please enter numbers 1 to 10." << std::endl;
			return;
		}
		updateFilm(title, updateString, 8);
		break;
	case '2':
		updateFilm(title, updateString, 3);
		break;
	case '3':
		updateFilm(title, updateString, 4);
		break;
	case '4':
		updateFilm(title, updateString, 5);
		break;
	case '5':
		updateFilm(title, updateString, 6);
		break;
	}
}

void updateFilm(char* title, char* newValue, int lineIndex)
{
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
	int index = 0;
	bool found = false;
	while (FilmFile.getline(buffer, bufferSize))
	{
		if (strCompare(title, buffer) == 0)
		{
			found = true;
			index++;
		}

		if (found)
		{
			index++;
		}

		if (index == lineIndex)
		{
			TempFile << newValue << "\n";
		}
		else
		{
			TempFile << buffer << "\n";
		}

		if (buffer[0] == '-')
		{
			found = false;
		}
	}

	FilmFile.close();
	TempFile.close();

	if (std::remove("filmsFile.txt") != 0 || std::rename("tempFile.txt", "filmsFile.txt") != 0) {
		std::cout << "Error: Unable to update filmsFile.txt." << std::endl;
	}
	else
	{
		std::cout << "Film successfully updated." << std::endl;
	}

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
		std::cout << "Film " << title << " successfully deleted." << std::endl;
	}

	std::cout << "------------------------" << std::endl;
}

void rateFilm()
{
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

	std::cout << "Rating: " << std::endl;
	int rating;
	std::cin >> rating;

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
	int index = 0;
	bool found = false;
	const int lineIndex = 8;
	while (FilmFile.getline(buffer, bufferSize))
	{
		if (strCompare(title, buffer) == 0)
		{
			found = true;
			index++;
		}

		if (found)
		{
			index++;
		}

		if (index == lineIndex - 1)
		{
			TempFile << (parse(buffer) + 1) << "\n";
		}
		else if (index == lineIndex)
		{
			int newRating = rating + parse(buffer);
			TempFile << newRating << "\n";
		}
		else
		{
			TempFile << buffer << "\n";
		}

		if (buffer[0] == '-')
		{
			found = false;
		}
	}

	FilmFile.close();
	TempFile.close();

	if (std::remove("filmsFile.txt") != 0 || std::rename("tempFile.txt", "filmsFile.txt") != 0) {
		std::cout << "Error: Unable to update filmsFile.txt." << std::endl;
	}
	else
	{
		std::cout << "Film successfully rated." << std::endl;
	}

	std::cout << "------------------------" << std::endl;
}

int countFilms()
{
	std::ifstream FilmFile("filmsFile.txt", std::ios::app);

	char buffer[bufferSize];
	int linesCount = 0;
	while (FilmFile.getline(buffer, bufferSize))
	{
		linesCount++;
	}

	FilmFile.close();

	return ((linesCount - 1) / 8);
}

void insertionSortByTitle(Film* arr, int size)
{
	for (int i = 1; i < size; i++)
	{
		Film x = arr[i];
		int j = i;
		while (j > 0 && strCompare(x.title, arr[j - 1].title) < 0)
		{
			arr[j] = arr[j - 1];
			j--;
		}
		arr[j] = x;
	}
}

void insertionSortByRating(Film* arr, int size)
{
	for (int i = 1; i < size; i++)
	{
		Film x = arr[i];
		int j = i;
		while (j > 0 && x.rating < arr[j - 1].rating)
		{
			arr[j] = arr[j - 1];
			j--;
		}
		arr[j] = x;
	}
}

void printFilms(Film* films, int size)
{
	std::cout << "------------------------" << std::endl;

	for (int i = 0; i < size; i++)
	{
		std::cout << "Title: " << films[i].title << "\n"
			<< "Year: " << films[i].year << "\n"
			<< "Genre: " << films[i].ganre << "\n"
			<< "Director: " << films[i].director << "\n"
			<< "Actors: " << films[i].actors << "\n"
			<< "Rating: " << films[i].rating << std::endl;
		std::cout << "-\n";
	}

	std::cout << "------------------------" << std::endl;
}

void sort(int criteria)
{
	int filmsCount = countFilms();
	Film* movies = new Film[filmsCount];

	std::ifstream FilmFile("filmsFile.txt", std::ios::app);

	char buffer[bufferSize];
	int index = 1;
	int arrayIdx = 0;

	char title[MAX_STRING_LENGTH] = "";
	int year = 0;
	char genre[MAX_STRING_LENGTH] = "";
	char director[MAX_STRING_LENGTH] = "";
	char actors[MAX_STRING_LENGTH] = "";
	int rating = 0;
	int peopleCount = 0;

	FilmFile.getline(buffer, bufferSize);
	while (FilmFile.getline(buffer, bufferSize))
	{
		if (*buffer == '-')
		{
			Film film(title, year, genre, director, actors, rating, peopleCount);
			movies[arrayIdx] = film;
			arrayIdx++;
			index = 0;
		}

		switch (index)
		{
		case 1:
			strCopy(buffer, title);
			break;
		case 2:
			year = parse(buffer);
			break;
		case 3:
			strCopy(buffer, genre);
			break;
		case 4:
			strCopy(buffer, director);
			break;
		case 5:
			strCopy(buffer, actors);
			break;
		case 6:
			peopleCount = parse(buffer);
			break;
		case 7:
			rating = parse(buffer);
			break;
		}

		index++;
	}

	FilmFile.close();

	if (criteria == 1)
	{
		insertionSortByTitle(movies, filmsCount);
	}
	else
	{
		insertionSortByRating(movies, filmsCount);
	}
	printFilms(movies, filmsCount);
}

#pragma endregion
