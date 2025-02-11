/**
*
* Solution to course project # 1
* Introduction to programming course 
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2024/2025
* 
* @author Ralitsa Taneva
* @idnumber 3MI0600266
* @compiler VC
* 
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const char* MENU_FILE = "menu.txt";
const char* ORDERS_FILE = "orders.txt";
const char* DATE_FILE = "date.txt";
const char* STORAGE_FILE = "stock.txt";

constexpr int MAX_STOCK_CAPACITY = 50;
constexpr int MAX_MENU_ITEMS = 100;
constexpr int MAX_INGREDIENTS = 10;
constexpr int MAX_NAME_LEN = 50;
constexpr int MAX_LINE_LENGTH = 256;
constexpr int DATE_FORMAT_LEN = 10;
constexpr int INVALID_INDEX = -1;

const int MAX_VALID_YR = 9999;
const int MIN_VALID_YR = 2025;


struct Product {
	char name[MAX_NAME_LEN + 1];
	double quantity; // in kilograms
};

struct MenuItem {
	char name[MAX_NAME_LEN + 1];
	double price;
	Product ingredients[MAX_INGREDIENTS];
	int ingredientsCount;
};

void myStrcat(char* dest, const char* src) {
	while (*dest) {
		dest++;
	}
	while (*src) {
		*dest++ = *src++;
	}
	*dest = '\0';
}

char* myStrtok(char* str, const char* delimiters) {
	static char* nextToken = nullptr;
	if (str != nullptr) {
		nextToken = str;
	}
	if (nextToken == nullptr) {
		return nullptr;
	}

	char* start = nextToken;
	while (*nextToken != '\0') {
		for (int i = 0; delimiters[i] != '\0'; i++) {
			if (*nextToken == delimiters[i]) {
				*nextToken = '\0';
				nextToken++;
				return start;
			}
		}
		nextToken++;
	}

	nextToken = nullptr;
	return start;
}

void myStrcpy(char* destination, const char* source) {
	int i = 0;
	while (source[i] != '\0') {
		destination[i] = source[i];
		i++;
	}
	destination[i] = '\0';
}

int myStrcmp(const char* str1, const char* str2) {
	int i = 0;
	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i]) {
			return str1[i] - str2[i];
		}
		i++;
	}
	return str1[i] - str2[i];
}

char* mystrtok(char* s, char d) {

	static char* input = nullptr;

	if (s != nullptr)
		input = s;

	if (input == nullptr)
		return nullptr;

	while (*input == d)
		input++;

	if (*input == '\0')
		return nullptr;

	char* result = input;
	while (*input != '\0' && *input != d)
		input++;

	if (*input == d) {
		*input = '\0';
		input++;
	}

	if (*result == '\0') {
		return nullptr;
	}

	return result;
}


int myStrlen(const char* arr) {
	int length = 0;

	while (arr[length] != '\0') {
		length++;
	}

	return length;
}

//Menu format is:
//DishName,Price:Ingredient1,Quantity1;Ingredient2,Quantity2;...
//ShopskaSalad,10.5:Cucumbers,0.1;Tomatoes,0.2;

void getProductName(char* productName)
{
	cout << "Enter the product name: ";
	cin.ignore();
	cin.getline(productName, MAX_NAME_LEN + 1);
}

void getProductQuantity(double& productQuantity)
{
	cout << "Enter the product quantity: ";
	cin >> productQuantity;
	cin.ignore();
}

bool isInvalidMenu(const MenuItem menu[])
{
	return menu == nullptr;
}

void parseDishInfo(char* dishInfo, MenuItem& menuItem) {
	if (dishInfo == nullptr) {
		std::cout << "Error: Missing dish information.\n";
		return;
	}

	char* dishName = myStrtok(dishInfo, ",");
	char* priceStr = myStrtok(dishInfo + myStrlen(dishName) + 1, "\0");

	if (dishName == nullptr || priceStr == nullptr) {
		std::cout << "Error: Missing dish name or price.\n";
		return;
	}

	myStrcpy(menuItem.name, dishName);
	menuItem.price = atof(priceStr);
}

void parseIngredient(char* ingredientStr, Product& ingredient) {
	if (ingredientStr == nullptr) {
		std::cout << "Error: Invalid ingredient format.\n";
		return;
	}

	char* ingredientName = myStrtok(ingredientStr, ",");
	char* quantityStr = myStrtok(ingredientStr + myStrlen(ingredientName) + 1, "\n");

	if (ingredientName == nullptr || quantityStr == nullptr) {
		std::cout << "Error: Missing ingredient name or quantity.\n";
		return;
	}

	myStrcpy(ingredient.name, ingredientName);
	ingredient.quantity = atof(quantityStr);
}

void parseIngredients(char* ingredientsStr, MenuItem& menuItem) {

	menuItem.ingredientsCount = 0;

	char* ingredient = mystrtok(ingredientsStr, ';');

	while (ingredient != nullptr) {

		if (menuItem.ingredientsCount >= MAX_INGREDIENTS) {
			std::cout << "Error: Too many ingredients in menu item: " << menuItem.name << "\n";
			break;
		}
		parseIngredient(ingredient, menuItem.ingredients[menuItem.ingredientsCount]);
		menuItem.ingredientsCount++;

		ingredient = mystrtok(nullptr, ';');
	}
}

void parseMenuLine(char* line, MenuItem& menuItem) {
	if (line == nullptr || line[0] == '\0') {
		std::cerr << "Error: Empty menu line.\n";
		return;
	}

	char* dishInfo = myStrtok(line, ":");
	char* ingredientsStr = myStrtok(line + (myStrlen(dishInfo) + 1), "\n");

	if (dishInfo == nullptr || ingredientsStr == nullptr) {
		std::cerr << "Error: Missing dish info or ingredients.\n";
		return;
	}

	parseDishInfo(dishInfo, menuItem);
	parseIngredients(ingredientsStr, menuItem);
}

// max menu len
void loadMenuFromFile(MenuItem menu[], int& menuSize) {

	ifstream menuFile(MENU_FILE);
	menuSize = 0;

	if (menuFile.is_open()) {
		char line[MAX_LINE_LENGTH + 1];
		while (menuFile.getline(line, MAX_LINE_LENGTH + 1)) {
			parseMenuLine(line, menu[menuSize]);
			menuSize++;
		}
		menuFile.close();
	}
	else {
		std::cout << "Error: Could not open menu file.\n";
	}
}

void saveMenuToFile(const MenuItem menu[], int& menuSize) {
	if (isInvalidMenu(menu))
	{
		cout << "Error: Menu cannot be nullptr.\n";
		return;
	}

	std::ofstream menuFileStream(MENU_FILE, std::ios::out);
	if (!menuFileStream.is_open()) {
		std::cerr << "Error: Unable to open menu file for writing.\n";
		return;
	}

	for (int i = 0; i < menuSize; i++) {
		menuFileStream << menu[i].name << "," << menu[i].price << ":";
		for (int j = 0; j < menu[i].ingredientsCount; j++) {
			menuFileStream << menu[i].ingredients[j].name << "," << menu[i].ingredients[j].quantity;
			menuFileStream << ";";
		}
		menuFileStream << "\n";
	}
	menuFileStream.close();
}

void addMenuItem(MenuItem menu[], int& menuSize, const MenuItem& newItem) {
	if (isInvalidMenu(menu))
	{
		cout << "Error: Menu cannot be nullptr.\n";
		return;
	}

	if (menuSize >= MAX_MENU_ITEMS) {
		std::cout << "Error: Menu is full. Cannot add more items.\n";
		return;
	}
	menu[menuSize] = newItem;
	menuSize++;
	saveMenuToFile(menu, menuSize);
	loadMenuFromFile(menu, menuSize);
}

void getMenuItemInput(MenuItem& newItem) {
	std::cout << "Enter the name of the dish: ";
	std::cin.ignore();
	std::cin.getline(newItem.name, MAX_NAME_LEN + 1);

	std::cout << "Enter the price of the dish: ";
	std::cin >> newItem.price;

	std::cout << "Enter the number of ingredients (max " << MAX_INGREDIENTS << "): ";
	std::cin >> newItem.ingredientsCount;

	if (newItem.ingredientsCount > MAX_INGREDIENTS) {
		std::cerr << "Error: Too many ingredients. Setting to max allowed.\n";
		newItem.ingredientsCount = MAX_INGREDIENTS;
	}

	for (int i = 0; i < newItem.ingredientsCount; i++) {
		std::cout << "Enter the name of ingredient " << i + 1 << ": ";
		std::cin.ignore();
		std::cin.getline(newItem.ingredients[i].name, MAX_NAME_LEN + 1);

		std::cout << "Enter the quantity of " << newItem.ingredients[i].name << ": ";
		std::cin >> newItem.ingredients[i].quantity;
	}
}

void switchMenuItems(MenuItem& item1, MenuItem& item2)
{
	MenuItem temp = item1;
	item1 = item2;
	item2 = temp;

}

int findDishIndex(const char* dishName, const MenuItem menu[], int menuSize) {
	if (dishName == nullptr || menu == nullptr || menuSize <= 0) {
		return -1;
	}

	for (int i = 0; i < menuSize; i++) {

		if (myStrcmp(menu[i].name, dishName) == 0) {
			return i;
		}
	}
	return -1;
}

void removeMenuItem(MenuItem menu[], int& menuSize)
{
	if (isInvalidMenu(menu))
	{
		cout << "Error: Menu cannot be nullptr.\n";
		return;
	}

	if (menuSize == 0) {
		std::cout << "The menu is empty. Nothing to remove.\n";
		return;
	}

	cout << "Which item from the menu you want to remove?" << "\n";
	char itemName[MAX_NAME_LEN + 1];
	getProductName(itemName);

	int itemIndex = findDishIndex(itemName, menu, menuSize);
	if (itemIndex == INVALID_INDEX)
	{
		cerr << "Error: There is no such item in the menu!" << "\n";
		return;
	}

	switchMenuItems(menu[itemIndex], menu[menuSize - 1]);
	menuSize--;
	saveMenuToFile(menu, menuSize);
	loadMenuFromFile(menu, menuSize);
	cout << "Successfully removed " << itemName << " from the menu!" << "\n\n";
}

void displayMenu(const MenuItem menu[], int& menuSize) {
	if (isInvalidMenu(menu))
	{
		cout << "Error: Menu cannot be nullptr.\n";
		return;
	}

	std::cout << "\n--- --- --- MENU --- --- ---\n";

	for (int i = 0; i < menuSize; i++) {
		std::cout << menu[i].name << " - $" << menu[i].price << "\n";
	}
}

void loadStorageFromFile(Product stock[], int& stockSize) {
	ifstream stockFile(STORAGE_FILE);
	stockSize = 0;

	if (stockFile.is_open()) {
		char line[MAX_LINE_LENGTH + 1];
		while (stockFile.getline(line, MAX_LINE_LENGTH + 1)) {
			char* name = myStrtok(line, ",");
			char* quantityStr = myStrtok(line + myStrlen(name) + 1, ",");

			myStrcpy(stock[stockSize].name, name);
			stock[stockSize].quantity = atof(quantityStr);
			stockSize++;
		}
		stockFile.close();
	}
	else {
		cout << "Could not load stock file.\n";
	}
}

void saveStorageToFile(const Product storage[], int stockSize) {
	ofstream stockFile(STORAGE_FILE);
	if (stockFile.is_open()) {
		for (int i = 0; i < stockSize; i++) {
			stockFile << storage[i].name << "," << storage[i].quantity << "\n";
		}
		stockFile.close();
	}
	else {
		std::cout << "Could not save stock file.\n";
	}
}

int findProductInStorage(const char* ingredientName,const Product storage[], int stockSize) {
	for (int i = 0; i < stockSize; i++) {
		if (myStrcmp(ingredientName, storage[i].name) == 0) {
			return i;
		}
	}
	return INVALID_INDEX;
}

void switchProducts(Product& item1, Product& item2) {
	Product temp = item1;
	item1 = item2;
	item2 = temp;
}

void removeProductFromStorage(Product storage[], int& stockSize)
{
	std::cout << "Which product do you want to remove from the storage?" << "\n";
	char productName[MAX_NAME_LEN + 1];
	getProductName(productName);

	int productIndex = findProductInStorage(productName, storage, stockSize);
	if (productIndex == INVALID_INDEX)
	{
		std::cout << "There is no such product in the storage!" << "\n";
		return;
	}

	switchProducts(storage[productIndex], storage[stockSize - 1]);
	stockSize--;
	saveStorageToFile(storage, stockSize);
	std::cout << "Successfully removed " << productName << " from the storage!" << "\n" << "\n";
}

void addNonExistingProductToStorage(Product storage[], int& stockSize, const char* productName, double productQuantity)
{
	myStrcpy(storage[stockSize].name, productName);
	storage[stockSize].quantity = productQuantity;
	stockSize++;

	cout << "Successfully added " << productQuantity << " kg of " << productName << " (new product)" << ".\n";
}

void addExistingProductToStorage(Product storage[], int productIndex, const char* productName, double productQuantity)
{
	storage[productIndex].quantity += productQuantity;

	cout << "\n" << "Succesfully added " << productQuantity << " kg of " << productName << ".\n";
}


void addProductToStorage(Product stock[], int& stockSize)
{
	char productName[MAX_NAME_LEN + 1];
	double productQuantity;

	cout << "Which stock do you want to add to the storage and how much (in kg)?" << "\n";
	getProductName(productName);
	getProductQuantity(productQuantity);

	int productIndex = findProductInStorage(productName, stock, stockSize);

	if (stockSize == MAX_STOCK_CAPACITY && productIndex == INVALID_INDEX)
	{
		cout << "Storage is full. Cannot add more products!" << "\n";
		return;
	}

	if (productIndex == INVALID_INDEX)
	{
		addNonExistingProductToStorage(stock, stockSize, productName, productQuantity);
	}
	else
	{
		addExistingProductToStorage(stock, productIndex, productName, productQuantity);
	}

	saveStorageToFile(stock, stockSize);
	cout << "\n";
}

void displayStorage(Product stock[], int stockSize)
{
	cout << " --- --- --- STOCK --- --- ---" << "\n";
	if (stockSize == 0)
	{
		cout << "Error: There is no products in the storage" << "\n";
	}

	for (int i = 0; i < stockSize; i++) {
		cout << stock[i].name << ": " << stock[i].quantity << "kg" << "\n";
	}
	cout << "\n";
}

// check if there is enough stock for all ingredients in the dish
bool checkProductAvailabilityForMenuItem(const MenuItem& dish, Product stock[], int stockSize) {
	for (int i = 0; i < dish.ingredientsCount; i++) {

		int stockIndex = findProductInStorage(dish.ingredients[i].name, stock, stockSize);
		if (stockIndex == INVALID_INDEX || stock[stockIndex].quantity < dish.ingredients[i].quantity) {
			return false; // ingredient not found or insufficient stock
		}
	}
	return true;
}

void deductStock(const MenuItem& dish, Product stock[], int stockSize) {
	for (int i = 0; i < dish.ingredientsCount; i++) {
		int stockIndex = findProductInStorage(dish.ingredients[i].name, stock, stockSize);
		if (stockIndex != INVALID_INDEX) {
			stock[stockIndex].quantity -= dish.ingredients[i].quantity;
		}
	}
	saveStorageToFile(stock, stockSize);
}

void appendOrderToFile(const char* name, const char* date) {
	std::ofstream ordersFile("orders.txt", std::ios::app);
	if (!ordersFile.is_open()) {
		std::cerr << "Error: Unable to open orders file for writing.\n";
		return;
	}

	ordersFile << name << "," << date << "\n";
	ordersFile.close();
}

void placeOrder(MenuItem menu[], int menuSize, Product stock[], int stockSize, const char* date) {
	if (menu == nullptr || stock == nullptr || date == nullptr)
	{
		cout << "Invalid arguments.\n";
		return;
	}

	char dishName[MAX_NAME_LEN + 1];
	cout << "Enter the name of the dish: " << endl;
	cin.ignore();
	cin.getline(dishName, MAX_NAME_LEN + 1);

	int dishIndex = findDishIndex(dishName, menu, menuSize);
	if (dishIndex == INVALID_INDEX) {
		cout << "Error: Dish not found.\n";
		return;
	}

	MenuItem& dish = menu[dishIndex];

	if (checkProductAvailabilityForMenuItem(dish, stock, stockSize)) {

		appendOrderToFile(dishName, date);
		deductStock(dish, stock, stockSize);

		cout << "Order placed successfully for " << dishName << ".\n";
		cout << "Total price: $" << dish.price << "\n";
	}
	else {
		cerr << "Error: Order cannot be placed due to insufficient stock.\n";
	}
}

void returnStock(const MenuItem& dish, Product stock[], int stockSize) {
	for (int i = 0; i < dish.ingredientsCount; i++) {
		int stockIndex = findProductInStorage(dish.ingredients[i].name, stock, stockSize);
		if (stockIndex != INVALID_INDEX) {
			stock[stockIndex].quantity += dish.ingredients[i].quantity;
		}
	}
	saveStorageToFile(stock, stockSize);
}

bool cancelLastOrderByNameAndDate(const char* name, const char* date, std::vector<char*>& orders) {
	for (int i = orders.size() - 1; i >= 0; --i) {
		const char* order = orders[i];

		if (strstr(order, name) != nullptr && strstr(order, date) != nullptr) {
			delete[] orders[i];

			for (int j = i; j < orders.size() - 1; ++j) {
				orders[j] = orders[j + 1];
			}

			orders.pop_back();
			return true;
		}
	}
	return false;
}

int readOrdersFromFile(const char* filename, std::vector<char*>& orders) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error: Could not open file for reading.\n";
		return -1;
	}

	char line[MAX_LINE_LENGTH + 1];
	while (file.getline(line, MAX_LINE_LENGTH + 1)) {
		char* order = new char[myStrlen(line) + 1];
		myStrcpy(order, line);
		orders.push_back(order);
	}

	file.close();
	return 0;
}

void writeOrdersToFile(const char* filename, const std::vector<char*>& orders) {
	std::ofstream file(filename, std::ios::out);
	if (!file.is_open()) {
		std::cerr << "Error: Could not open file for writing.\n";
		return;
	}

	for (const char* order : orders) {
		file << order << "\n";
	}
	file.close();
}

void getOrderToBeCancelledName(char* dishName, int maxLength) {
	std::cout << "Which order do you want to cancel?\nEnter dish name: ";
	std::cin.ignore();
	std::cin.getline(dishName, maxLength);
}


void cancelLastOrderByName(const char* name, const char* date, MenuItem menu[], int menuSize, Product stock[], int stockSize) {
	std::vector<char*> orders;

	if (readOrdersFromFile(ORDERS_FILE, orders) != 0) {
		return; 
	}

	if (cancelLastOrderByNameAndDate(name, date, orders)) {
		int menuIndex = findDishIndex(name, menu, menuSize);
		if (menuIndex != -1) {
			returnStock(menu[menuIndex], stock, stockSize);
		}

		std::cout << "Order with name '" << name << "' has been canceled.\n";
	}
	else {
		std::cout << "No order with name '" << name << "' was found for " << date << " .\n";
	}

	writeOrdersToFile(ORDERS_FILE, orders);

	for (char* order : orders) {
		delete[] order;
	}
}

void displayAllOrders() {
	std::ifstream inFile(ORDERS_FILE);

	if (!inFile.is_open()) {
		std::cerr << "Error: Unable to open orders file for reading.\n";
		return;
	}

	char line[MAX_LINE_LENGTH + 1];
	int orderCount = 0;

	std::cout << "\n--- --- - ALL ORDERS - --- ---\n\n";

	while (inFile.getline(line, MAX_LINE_LENGTH)) {
		orderCount++;
		std::cout << line << std::endl;
	}

	if (orderCount == 0) {
		std::cout << "No orders found.\n";
	}

	cout << endl;
	inFile.close();
}

void extractDistinctOrderNames(const std::vector<char*>& orders, std::vector<char*>& names, std::vector<int>& counts) {
	for (const char* order : orders) {
		char temp[MAX_LINE_LENGTH + 1];
		strncpy(temp, order, MAX_LINE_LENGTH);
		char* name = mystrtok(temp, ',');
		if (!name) continue;

		bool found = false;
		for (size_t i = 0; i < names.size(); ++i) {
			if (strcmp(names[i], name) == 0) {
				counts[i]++;
				found = true;
				break;
			}
		}

		if (!found) {
			char* newName = new char[strlen(name) + 1];
			myStrcpy(newName, name);
			names.push_back(newName);
			counts.push_back(1);
		}
	}
}

void sortOrdersLexicographically(std::vector<char*>& names, std::vector<int>& counts) {
	for (size_t i = 0; i < names.size() - 1; ++i) {
		for (size_t j = i + 1; j < names.size(); ++j) {
			if (strcmp(names[i], names[j]) > 0) {
				std::swap(names[i], names[j]);
				std::swap(counts[i], counts[j]);
			}
		}
	}
}

void displaySortedOrders(const std::vector<char*>& names, const std::vector<int>& counts) {
	std::cout << "\nSorted orders with counts:\n";
	for (size_t i = 0; i < names.size(); ++i) {
		std::cout << names[i] << ": " << counts[i] << "\n";
	}
}

void free(std::vector<char*>& data) {
	for (char* ptr : data) {
		delete[] ptr;
	}
	data.clear();
}

void displaySortedOrdersWithCounts() {
	std::vector<char*> orders;
	std::vector<char*> names;
	std::vector<int> counts;

	if (readOrdersFromFile(ORDERS_FILE, orders) == -1)
	{
		return;
	}

	extractDistinctOrderNames(orders, names, counts);

	sortOrdersLexicographically(names, counts);

	displaySortedOrders(names, counts);

	free(orders);
	free(names);
}

bool isOrderOnDate(const char* order, const char* date) {
	return strstr(order, date) != nullptr;
}

const char* extractDishName(const char* order) {
	char temp[MAX_LINE_LENGTH + 1];
	strncpy(temp, order, MAX_LINE_LENGTH);
	char* dishName = strtok(temp, ",");
	return dishName;
}

double getDishPrice(const char* dishName,const MenuItem menu[], int menuSize) {
	if (isInvalidMenu(menu))
	{
		cout << "Error: Menu cannot be nullptr.\n";
		return -1;
	}

	if (dishName == nullptr)
	{
		cout << "Error: Dish name cannot be nullptr\n";
	}

	int menuIndex = findDishIndex(dishName, menu, menuSize);
	if (menuIndex != INVALID_INDEX) {
		return menu[menuIndex].price;
	}
	else {
		std::cerr << "Error: Dish '" << dishName << "' not found in the menu.\n";
		return 0.0;
	}
}

double processOrdersForDate(const char* date, const MenuItem menu[], int menuSize, const std::vector<char*>& orders, bool& hasOrdersOnDate)
{
	if (isInvalidMenu(menu))
	{
		cout << "Error: Menu cannot be nullptr.\n";
		return 0.0;
	}

	if (date == nullptr)
	{
		cout << "Error: Date cannot be nullptr\n";
	}

	double totalTurnover = 0.0;
	for (const char* order : orders) {
		if (isOrderOnDate(order, date)) {
			hasOrdersOnDate = true;
			const char* dishName = extractDishName(order);

			if (dishName) {
				totalTurnover += getDishPrice(dishName, menu, menuSize);
			}
		}
	}
	return totalTurnover;
}

void printDailyTurnover(const char* date,const MenuItem menu[], int menuSize) {
	if (isInvalidMenu(menu))
	{
		cout << "Error: Menu cannot be nullptr.\n";
		return;
	}

	if (date == nullptr)
	{
		cout << "Error: Date cannot be nullptr\n";
	}

	std::vector<char*> orders;
	if (readOrdersFromFile(ORDERS_FILE, orders) != 0) {
		std::cerr << "Error: Could not read orders file.\n";
		return;
	}

	bool hasOrders = false;
	double totalTurnover = processOrdersForDate(date, menu, menuSize, orders, hasOrders);

	for (char* order : orders) {
		delete[] order;
	}

	if (!hasOrders) {
		std::cout << "No orders were placed on " << date << ".\n";
		return;
	}
	else
	{
		cout << "The turnover for " << date << " is " << totalTurnover << "$." << endl;
	}
}

bool isLeap(int year)
{
	return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

bool isValidDate(int d, int m, int y)
{
	if (y > MAX_VALID_YR || y < MIN_VALID_YR)
	{
		return false;
	}

	if (m < 1 || m > 12)
	{
		return false;
	}

	if (d < 1 || d > 31)
	{
		return false;
	}

	if (m == 2)
	{
		if (isLeap(y))
		{
			return (d <= 29);
		}
		else
		{
			return (d <= 28);
		}
	}

	if (m == 4 || m == 6 || m == 9 || m == 11)
	{
		return (d <= 30);
	}
	return true;
}

bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

int myAtoi(const char* str) {
	if (str == nullptr) {
		return 0;
	}

	int result = 0;
	int sign = 1;
	int i = 0;

	while (str[i] == ' ') {
		i++;
	}

	if (str[i] == '-' || str[i] == '+') {
		sign = (str[i] == '-') ? -1 : 1;
		i++;
	}

	while (str[i] != '\0' && isDigit(str[i])) {
		result = result * 10 + (str[i] - '0');
		i++;
	}

	return result * sign;
}

bool isValidDateFormat(const char* date) {
	if (date == nullptr)
	{
		cout << "Error: Date cannot be nullptr\n";
		return false;
	}

	if (strlen(date) != DATE_FORMAT_LEN) {
		return false;
	}

	if (date[2] != '.' || date[5] != '.') {
		return false;
	}

	int day = myAtoi(date);
	int month = myAtoi(date + 3);
	int year = myAtoi(date + 6);

	return isValidDate(day, month, year);
}

bool getStartDate(char* startDate) {
	if (startDate == nullptr)
	{
		cout << "Error: Date cannot be nullptr\n";
		return false;
	}

	std::cout << "Enter the start date (format DD.MM.YYYY): ";
	cin.ignore();
	std::cin.getline(startDate, DATE_FORMAT_LEN + 1);

	if (isValidDateFormat(startDate)) {
		return true;
	}
	else {
		std::cout << "Invalid date format. Exiting function.\n";
		return false;
	}
}

void loadCurrentDateFromFile(char currentDate[]) {
	ifstream dateFile(DATE_FILE);

	if (dateFile.is_open()) {
		char tempDate[DATE_FORMAT_LEN + 1];
		while (dateFile >> tempDate) {
			myStrcpy(currentDate, tempDate);
		}
		dateFile.close();
	}
	else {
		myStrcpy(currentDate, "01.01.2025");
		ofstream dateFileOut(DATE_FILE);
		dateFileOut << currentDate;
		dateFileOut.close();
	}
}

bool parseDate(const char* date, int& day, int& month, int& year) {
	if (date == nullptr)
	{
		cout << "Error: Date cannot be nullptr\n";
		return false;
	}

	if (date == nullptr || strlen(date) != 10 || date[2] != '.' || date[5] != '.') {
		return false;
	}

	day = (date[0] - '0') * 10 + (date[1] - '0');
	month = (date[3] - '0') * 10 + (date[4] - '0');
	year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

	if (day < 1 || day > 31 || month < 1 || month > 12 || year < MIN_VALID_YR || year > MAX_VALID_YR) {
		return false;
	}

	return true;
}

void incrementDate(char currentDate[]) {
	if (currentDate == nullptr)
	{
		cout << "Error: Date cannot be nullptr\n";
		return;
	}

	int day, month, year;
	parseDate(currentDate, day, month, year);

	day++;
	if ((day > 31 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)) ||
		(day > 30 && (month == 4 || month == 6 || month == 9 || month == 11)) ||
		(day > 28 && month == 2 && year % 4 != 0) ||
		(day > 29 && month == 2 && year % 4 == 0)) {
		day = 1;
		month++;
	}
	if (month > 12) {
		month = 1;
		year++;
	}

	sprintf(currentDate, "%02d.%02d.%04d", day, month, year);
}

int compareDates(const char* date1, const char* date2) {
	int d1, m1, y1, d2, m2, y2;

	parseDate(date1, d1, m1, y1);
	parseDate(date2, d2, m2, y2);

	if (y1 != y2) {
		return y1 - y2;
	}

	if (m1 != m2) {
		return m1 - m2;
	}

	return d1 - d2;
}

void printDailyTurnoversFromStartDate(const char* startDate, const char* currentDate, MenuItem menu[], int menuSize) {
	if (startDate == nullptr || currentDate == nullptr)
	{
		cout << "Error: Date cannot be nullptr\n";
		return;
	}

	if (compareDates(startDate, currentDate) > 0) {
		std::cout << "Error: Start date cannot be later than the current date.\n";
		return;
	}

	std::cout << "Daily Turnovers from " << startDate << " to " << currentDate << ":\n\n";

	char tempDate[DATE_FORMAT_LEN + 1];
	myStrcpy(tempDate, startDate);  

	while (strcmp(tempDate, currentDate) <= 0) {
		printDailyTurnover(tempDate, menu, menuSize);
		incrementDate(tempDate);
	}
}

void saveDateToFile(const char* date)
{
	if (date == nullptr)
	{
		cout << "Error: Date cannot be nullptr\n";
		return;
	}

	ofstream dateFile(DATE_FILE);

	if (!dateFile.is_open()) {
		cout << "Error: Cannot open date file for writing!\n";
	}

	dateFile << date << endl;
	dateFile.close();
}

void updateDateInFile(char date[])
{
	if (date == nullptr)
	{
		cout << "Error: Date cannot be nullptr\n";
		return;
	}
	incrementDate(date);
	saveDateToFile(date);
}

void displayCommonOptions() {
	cout << "\n--- CHOOSE OPTION ---\n";
	cout << "0. Exit\n";
	cout << "1. View menu\n";
	cout << "2. Place order\n";
	cout << "3. Cancel order\n";
	cout << "4. View orders\n";
	cout << "5. View sorted orders\n";
	cout << "6. View daily total\n";
}

void displayAdditionalOptions() {
	cout << "7. View stock availability\n";
	cout << "8. Remove product from storage\n";
	cout << "9. Add product to storage\n";
	cout << "10. View daily report\n";
	cout << "11. View report from a given date\n";
	cout << "12. Add new menu item\n";
	cout << "13. Remove menu item\n";
}

void createFileIfNotExists(const char* fileName) {

	ifstream inFile(fileName);
	if (inFile) {
		cout << "File '" << fileName << "' already exists.\n";
		inFile.close();
	}
	else {

		ofstream outFile(fileName);
		if (outFile) {
			cout << "File '" << fileName << "' has been created.\n";
			outFile.close();
		}
		else {
			cout << "Error: Could not create file '" << fileName << "'.\n";
		}
	}
	cout << endl;
}

// NOTE: this function is exceptionally more than 40 lines so as not to separate the logic in 2 smaller ones
// (one for common functionalities and one for Menager ones)
void handleOption(int option, MenuItem menu[], int& menuSize, Product stock[], int& stockSize, char* date)
{
	switch (option)
	{
	case 1:
		displayMenu(menu, menuSize);
		break;
	case 2:
		placeOrder(menu, menuSize, stock, stockSize, date);
		break;
	case 3:
		char menuItemName[MAX_NAME_LEN + 1];
		getOrderToBeCancelledName(menuItemName, MAX_NAME_LEN);
		cancelLastOrderByName(menuItemName, date, menu, menuSize, stock, stockSize);
		break;
	case 4:
		displayAllOrders();
		break;
	case 5:
		displaySortedOrdersWithCounts();
		break;
	case 6:
		printDailyTurnover(date, menu, menuSize);
		break;
	case 7:
		displayStorage(stock, stockSize);
		break;
	case 8:
		removeProductFromStorage(stock, stockSize);
		break;
	case 9:
		addProductToStorage(stock, stockSize);
		break;
	case 10:
		printDailyTurnover(date, menu, menuSize);
		updateDateInFile(date);
		break;
	case 11:
		char startDate[DATE_FORMAT_LEN + 1];
		if (getStartDate(startDate))
		{
			printDailyTurnoversFromStartDate(startDate, date, menu, menuSize);
		}
		break;
	case 12:
		MenuItem newItem;
		getMenuItemInput(newItem);
		addMenuItem(menu, menuSize, newItem);
		break;
	case 13:
		removeMenuItem(menu, menuSize);
		break;
	default:
		cout << "Error: Invalid option. Please try again.\n";
		break;
	}
}

void handleUserType(int role, MenuItem menu[], int& menuSize, Product stock[], int& stockSize, char* date) {
	int option = 0;

	while (true)
	{
		displayCommonOptions();

		if (role == 2)
		{
			displayAdditionalOptions();
		}

		cout << "\n--- --- --- --- ---\n";
		cin >> option;

		if (option == 0)
		{
			cout << "Exiting program..." << endl << "Goodbye!\n";
			updateDateInFile(date);
			return;
		}
		else if (role == 1 && !(1 <= option && option <= 6))
		{
			cout << "Error: Invalid option! Try again!\n";
		}
		else if (role == 2 && !(1 <= option && option <= 13))
		{
			cout << "Error: Invalid option! Try again!\n";
		}
		else
		{
			handleOption(option, menu, menuSize, stock, stockSize, date);
		}
	}
}



int main()
{
	createFileIfNotExists(MENU_FILE);
	createFileIfNotExists(STORAGE_FILE);
	createFileIfNotExists(ORDERS_FILE);

	MenuItem menu[MAX_MENU_ITEMS];
	int menuSize = 0;

	Product stock[MAX_STOCK_CAPACITY];
	int stockSize = 0;

	char date[11];

	loadMenuFromFile(menu, menuSize);
	loadStorageFromFile(stock, stockSize);
	loadCurrentDateFromFile(date);

	int role;

	std::cout << "Enter your role (1 for Waiter, 2 for Manager): ";
	std::cin >> role;

	if (role == 1 || role == 2) {
		handleUserType(role, menu, menuSize, stock, stockSize, date);
	}
	else {
		std::cout << "Invalid role. Exiting program.\n";
	}

	return 0;
}
