#include "Interface.h"
using namespace std;


template <typename T> void interface<T>::run()
{
	interface<T>::print(chooseType);
	string consoleInput;
	std::getline(cin, consoleInput);
	bool running = true;
	if (consoleInput == command_typeInt) {
		CTree<int>* tree = NULL;
		while (running)
		{
			vector<string> command = interface<int>::getUserInput();
			running = interface<int>::handleCommand(command, &tree);

		}
	}
	if (consoleInput == command_typeDouble)
	{
		CTree<double>* tree = NULL;
		while (running)
		{
			vector<string> command = interface<double>::getUserInput();
			running = interface<double>::handleCommand(command, &tree);

		}
	}
	if (consoleInput == command_typeString)
	{
		CTree<string>* tree = NULL;
		while (running)
		{
			vector<string> command = interface<string>::getUserInput();
			running = interface<string>::handleCommand(command, &tree);
		}
	}
}



template <typename T> vector<string> interface<T>::getUserInput() {
	interface<T>::print(notification_awaitingCommand);
	string consoleInput;
	std::getline(cin, consoleInput);
	return splitBySpace(consoleInput);

}

template <typename T> void interface<T>::print(const vector<string>& message) {
	for (int i = 0; i < message.size(); i++)
	{
		interface<int>::printSpace(message[i]);
	}
	interface<int>::print(""); // newline
}

template <typename T> vector<string> interface<T>::splitBySpace(const string& input) {
	vector<string> result;
	string currentWord;
	for (int i = 0; i < input.length(); i++)
	{
		if (input[i] == char_space) // if current char is a space, add currentWord to result and reset currentWord
		{
			if (currentWord != "") { result.push_back(currentWord); } //Skip multiple spaces
			currentWord = "";
		}
		else { currentWord += input[i]; } // add character to current word

	}
	if (currentWord != "") result.push_back(currentWord);
	return result;
}

template <typename T> bool interface<T>::handleCommand(const vector<string>& command, CTree<T>** tree) // TODO: add other types
{
	//call an appropriate method based on the command
	//return false if exiting the program
	//wrong command should print to console and continue
	string commandName = command[0];
	// cannot use switch in a nice way beacause dealing with string type (enum is C++11)

	if (commandName == command_exit || commandName == command_quit)  // end the program
	{
		interface<int>::print(notification_exit);
		if (*tree != NULL) { delete* tree; }
		return false;
	}

	else if (commandName == command_enterTree)
	{
		if (*tree != NULL)
		{
			delete* tree;
		}
		*tree = new CTree<T>(command);
		interface<int>::print((*tree)->clearErrors());
		interface<int>::print(notification_printingExpression);
		print((*tree)->getExpression());
		return true;
	}

	else if (commandName == command_listVars)
	{
		if ((!treeIsInitialized(tree))) { interface<int>::print(notification_noTree); }
		else
		{
			vector<string> vars = (*tree)->getVars();
			interface<int>::print(notification_printingVars);
			for (int i = 0; i < vars.size(); i++)
			{
				interface<int>::printSpace(vars[i]);
			}
			interface<int>::print(""); // newline

		}
		return true;
	}

	else if (commandName == command_printTree)
	{
		if ((!treeIsInitialized(tree))) { interface<int>::print(notification_noTree); }
		else
		{
			interface<int>::print(notification_printingTree);
			print((*tree)->getExpression());
		}
		return true;
	}

	else if (commandName == command_calculate)
	{
		if (!treeIsInitialized(tree)) { interface<int>::print(notification_noTree); return true; }
		interface<int>::print((*tree)->calculate(command));
		interface<int>::print((*tree)->clearErrors());
		return true;
	}

	else if (commandName == command_addTree)
	{
		if (!treeIsInitialized(tree)) { interface<int>::print(notification_noTree); return true; }
		(**tree) = move((**tree) + CTree<T>(command));
		interface<int>::print((*tree)->clearErrors());
		interface<int>::print(notification_printingExpression);
		print((*tree)->getExpression());
		return true;
	}

	else { interface<int>::print(notification_wrongCommand); return true; }

}

template <typename T> bool interface<T>::treeIsInitialized(CTree<T>** tree)
{
	if (*tree == NULL || !(*tree)->isInitialized()) { return false; }
	else { return true; }
}

//Avoid linker errors by using explicit instantations:
template class interface<int>;
template class interface<double>;
template class interface<string>;
