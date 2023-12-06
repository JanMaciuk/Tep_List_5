#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Tree.h"
using namespace std;

const string command_exit = "exit";
const string command_quit = "quit";
const string command_enterTree = "enter";
const string command_listVars = "vars";
const string command_printTree = "print";
const string command_calculate = "comp";
const string command_addTree = "join";
const string command_typeInt = "1";
const string command_typeDouble = "2";
const string command_typeString = "3";
const string defaultVarName = "x";
const char char_space = ' ';
const string chooseType = "Choose type:\n1. int\n2. double\n3. string\nor any other input to end the program";

const string notification_wrongCommand = "Wrong command, try again";
const string notification_exit = "Terminating the program";
const string notification_awaitingCommand = "Awaiting command to execute";
const string notification_ommitingInvalidChar = " is not valid in a variable name, ommiting";
const string notification_emptyVariableName = "Variable name cannot be empty, using default name: ";
const string notification_varNameOnlyNumbers = "Variable name cannot contain only numbers, appending default name: ";
const string notification_noTree = "No tree found, use enter command to create a new tree";
const string notification_printingTree = "Printing tree: ";
const string notification_printingExpression = "Constructed a tree using the expression: ";
const string notification_printingVars = "Printing variables: ";
const string notification_ommitingLeftovers = "Ommiting invalid values at the end of expression which couldnt be attached:";
const string notification_missingValue = " operator should be followed by a value, adding default: ";
const string notification_invalidVariablesNumber = "Wrong amount of variable values found";
const string notification_invalidVariableValue = " is not a valid value for a variable, a positive number required ";
const string notification_zeroNotAllowed = "entered numbers must be positive, zero is not supported, replacing with default value: ";
const string notification_overflow = "Integer overflow occured, use smaller values and consider result invalid, replaced by default: ";
const string notification_divisionByZero = "Division by zero occured during calculation, check expression and consider result invalid";


template <typename T> class interface
{
public:
	static vector<string> getUserInput(); // Get user input from console and split it into vector of strings
	static inline void print(const string& message) { cout << message << endl; }; // Print to console
	static inline void print(const double& message) { cout << message << endl; }; // Print to console
	static inline void printSpace(const string& message) { cout << message << " "; }; // Print to console
	static void print(const vector<string>& message); // Print vector of strings to console
	static vector<string> splitBySpace(const string& input); // Split string into a vector
	static bool handleCommand(const vector<string>& command, CTree<T>** tree); // Call methods based on user command, false if exit
	static bool treeIsInitialized(CTree<T>** tree); // Check if tree is initialized
	static void run();
};