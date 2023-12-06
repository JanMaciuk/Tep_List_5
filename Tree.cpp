#include "Tree.h"
#include <algorithm>
#include "interface.h"

//CTree:
template <typename T> CTree<T>::CTree(const vector<string> expression)
{
	int index = 1;
	root = new CNode<T>(expression, NULL, &index);
	// if position is not at the end of vector, print notification and ommit leftovers
	if (index < expression.size())
	{
		root->logErrorSpace(notification_ommitingLeftovers);
		for (int i = index; i < expression.size(); i++)
		{
			root->logErrorSpace(expression[i]);
		}
		root->logError(emptyString); // newline
	}
}

template <typename T> CTree<T>::CTree(const CTree& otherInstance) // copy constructor
{
	vector<string> expression = otherInstance.getExpression();
	expression.insert(expression.begin(), command_enterTree);
	int index = 1;
	root = new CNode<T>(expression, NULL, &index);
	// No need to check for leftovers, original tree must be valid
}

template <typename T> CTree<T>::CTree(CTree&& otherInstance) noexcept // move constructor
{
	root = otherInstance.root;
	otherInstance.root = NULL;
	cout << "Tree moved by constructor" << endl; //TODO: remove
}

template <typename T> CTree<T>::CTree() // default constructor
{
	root = NULL;
}



template <typename T> vector<string> CTree<T>::getVars() const
{
	vector<string> accumulator;
	vector<string> vars = root->getVars(&accumulator);
	return vars;

}

template <typename T> T CTree<T>::calculate(vector<string> command) const
{
	vector<string> accumulator;
	vector<string> vars = root->getVars(&accumulator);
	vector<T> values;
	for (int i = 1; i < command.size(); i++)
	{
		double value = atof(command[i].c_str()); // convert string to double using atof (c++98 standard)
		if (value > 0.0) { values.push_back(value); } // only add valid positive doubles
		else { root->logError(command[i] + notification_invalidVariableValue); return 0; }
	}
	if (values.size() != getVars().size()) { root->logError(notification_invalidVariablesNumber); return 0; }
	return CNode<T>::calculate(root, vars, values);
}

template <> string CTree<string>::calculate(vector<string> command) const {
	vector<string> accumulator;
	vector<string> vars = root->getVars(&accumulator);
	vector<string> values;
	for (int i = 1; i < command.size(); i++)
	{
		values.push_back(command[i]);
	}
	if (values.size() != getVars().size()) { root->logError(notification_invalidVariablesNumber); return emptyString; }
	return CNode<string>::calculate(root, vars, values);

}

template <typename T> vector<string> CTree<T>::getExpression() const
{
	vector<string> accumulator;
	vector<string> expression = root->inOrderWalk(&accumulator);
	return expression;
}

template <typename T> string CTree<T>::getErrors() const {
	return root->getErrors(false);
}
template <typename T> string CTree<T>::clearErrors() {
	return root->getErrors(true);
}


template <typename T> void CTree<T>::operator=(const CTree& otherInstance)
{
	// set current tree to a copy of another tree
	// if current tree is not empty, delete it
	if (this->root != NULL) { this->root->deleteTree(); }
	// create a new tree with the same expression as the other tree
	vector<string> expression = otherInstance.getExpression();
	expression.insert(expression.begin(), command_enterTree);
	int index = 1;
	this->root = new CNode<T>(expression, NULL, &index);
}

template <typename T> void CTree<T>::operator=(CTree&& otherInstance) noexcept // move operator - move another tree to the current tree
{
	// if current tree is not empty, delete it
	if (this->root != NULL) { this->root->deleteTree(); }
	// set root pointer to point to the other tree's root
	this->root = otherInstance.root;
	// set the other tree to an empty tree by dereferencing its root pointer
	otherInstance.root = NULL;

	cout << "Tree moved" << endl; //TODO: remove
}

template <typename T> CTree<T> CTree<T>::operator+(const CTree& otherInstance) const
{
	// join two trees using operator+
	// if both trees are empty, return empty tree
	if (this->root == NULL && otherInstance.root == NULL) { return CTree(); }
	// if one of the trees is empty, return the other tree
	else if (this->root == NULL) { return otherInstance; }
	else if (otherInstance.root == NULL) { return *this; }
	// if both trees are not empty, create a new tree which is a copy of the first with second one added to it.
	else
	{
		// get expressions of both trees
		// remove last element from the first expression (it is a leaf)
		// add the second expression to the first one
		// create a new tree from the new expression
		vector<string> thisExpression = this->getExpression();
		vector<string> otherExpression = otherInstance.getExpression();
		thisExpression.pop_back();
		thisExpression.insert(thisExpression.begin(), command_enterTree); // add first placeholder to expression
		for (int i = 0; i < otherExpression.size(); i++)
		{
			thisExpression.push_back(otherExpression[i]);
		}
		CTree resultTree = CTree(thisExpression);
		return move(resultTree);

	}
}

template <typename T> CTree<T>::~CTree() { root->deleteTree(); }



//CNode::
template <typename T> CNode<T>::CNode(const vector<string> expression, CNode<T>* parentNode, int* currentIndex)
{
	parent = parentNode;
	string val = defaultNodeValue;
	if (*currentIndex < expression.size()) { val = expression[*currentIndex]; } // if there are values left in the vector, get the next one
	else
	{ // if there are no values left, notify user and use default value
		if (parentNode == NULL) { logError(notification_missingValue + defaultNodeValue); }
		else { logError(parentNode->value + notification_missingValue + defaultNodeValue); }
	}
	(*currentIndex)++;
	type = getType(&val); // get type of the value, if its a variable, turn it into a valid variable name
	value = val;
	//fill vector with NULL children to avoid accessing unalocated memory by [] operator
	for (int i = 0; i < maxChildrenCount; i++)
	{
		children.push_back(NULL);
	}
	if (type == 1) // if operation with 1 child, create left child only
	{
		children[0] = new CNode<T>(expression, this, currentIndex);
		children[1] = NULL;
	}
	else if (type == 2) // if operation with 2 children, create left and right children 
	{
		children[0] = new CNode<T>(expression, this, currentIndex);
		children[1] = new CNode<T>(expression, this, currentIndex);
	}


}


template <typename T> vector<string> CNode<T>::inOrderWalk(vector<string>* accumulator) const
{
	if (this == NULL) { return *accumulator; }
	accumulator->push_back(value);
	children[0]->inOrderWalk(accumulator);
	children[1]->inOrderWalk(accumulator);
	return *accumulator;
}

template <typename T> void CNode<T>::logError(const string message)
{
	errMsg += message + endLine;
}
template <typename T> void CNode<T>::logErrorSpace(const string message)
{
	errMsg += message + space;
}
template <typename T> string CNode<T>::getErrors(bool clear)
{
	string result = errMsg;
	if (clear) errMsg = emptyString;
	return result;
}

template <typename T> int CNode<T>::getType(string* value)
{
	// 1 - operation with 1 child, 2 - operation with 2 children, 3 - constant, 4 - variable (name is made valid)
	if (find(operations1child.begin(), operations1child.end(), *value) != operations1child.end()) { return 1; } // if value is in the list of operations with 1 child
	else if (find(operations2children.begin(), operations2children.end(), *value) != operations2children.end()) { return 2; } // if value is in the list of operators with 2 children
	else if (isNumber(*value)) // if value is a number/constant return 3
	{
		//ensure the number is not zero
		bool isZero = true;
		for (int i = 0; i < value->length(); i++)
		{
			if ((*value)[i] != minDigit && (*value)[i] != decimalSeparator) { isZero = false; }
		}
		if (isZero)
		{
			logError(notification_zeroNotAllowed + defaultNodeValue);
			*value = defaultNodeValue;
		}
		return IDconstant;
	}
	else // if value is a variable validate it
	{
		*value = validateVariableName(*value);
		return IDvariable;

	}

}


template <typename T> bool CNode<T>::isNumber(const string value)	// version for integers
{
	for (int i = 0; i < value.length(); i++)
	{ // if any character is not withing ascii range of digits, return false
		if (value[i] < minDigit || value[i] > maxDigit) { return false; }
	}
	return true;
}
template <> bool CNode<double>::isNumber(const string value)		// version for doubles
{
	bool separatorFound = false;
	for (int i = 0; i < value.length(); i++)
	{	// if any character is not withing ascii range of digits,
		if ((value[i] < minDigit || value[i] > maxDigit))
		{
			// if found one dot, ignore it (still a number)
			if (value[i] == decimalSeparator && !separatorFound && (i < value.length() - 1)) { separatorFound = true; }
			else return false;
		}
	}
	return true;
}
template <> bool CNode<string>::isNumber(const string value)	// version for strings
{
	// check if first and last char are "
	return (value[0] == quotationMark && value[value.length() - 1] == quotationMark);
}


template <typename T> string CNode<T>::errMsg = emptyString;
template <typename T> string CNode<T>::validateVariableName(const string value)
{
	// turn string into a valid variable name, requirements to be valid:
	// cannot be empty
	// cannot contain only numbers
	// can only contain letters and numbers

	string result = emptyString;
	for (int i = 0; i < value.length(); i++)
	{
		if ((value[i] >= minDigit && value[i] <= maxDigit) || (value[i] >= minSmalLetter && value[i] <= maxSmallLetter) || (value[i] >= minCapitalLetter && value[i] <= maxCapitalLetter))
		{
			result += value[i];
		}
		else // if character is not a letter or a number, print notification and ommit it
		{
			logError(value[i] + notification_ommitingInvalidChar);
		}
	}
	if (result == emptyString) // if variable name is empty, add default name to make it valid
	{
		logError(notification_emptyVariableName + defaultVarName);
		result = defaultVarName;
	}
	if (isNumber(result)) // if variable name is only numbers, add default name to the beginning to make it valid
	{
		logError(notification_varNameOnlyNumbers);
		result = defaultVarName + result;
	}
	return result;
}

template <typename T> vector<string> CNode<T>::getVars(vector<string>* accumulator) const
{
	if (this == NULL) { return *accumulator; }
	if ((this->type == IDvariable) && (find((*accumulator).begin(), (*accumulator).end(), (this->value)) == (*accumulator).end()))
	{ // if node is a variable and is not in the accumulator, add it
		accumulator->push_back(this->value);
	} // then walk throught the rest of the tree
	children[0]->getVars(accumulator);
	children[1]->getVars(accumulator);
	return *accumulator;
}


template <> string CNode<string>::calculate(CNode<string>* node, const vector<string> vars, const vector<string> values) {
	if (node == NULL) { return emptyString; }

	else if (node->type == IDconstant) { return node->value; }

	else if (node->type == IDvariable) // if its a variable, find its value in values vector
	{
		int index = find(vars.begin(), vars.end(), node->value) - vars.begin(); // variable must be in the vars vector, so index will be valid (length is checked before)
		return values[index];
	}

	else if (node->type == 2) // if its a normal operator, calculate the values
	{
		string leftResult = calculate(node->children[0], vars, values);
		string rightResult = calculate(node->children[1], vars, values);

		if (node->value == operations2children[0]) // addition
		{
			return leftResult + rightResult;
		}

		else if (node->value == operations2children[1]) // subtraction
		{
			if (rightResult[0] == quotationMark) { rightResult = rightResult.substr(1, rightResult.length() - 2); } // remove quotation marks from searched string
			if (rightResult == emptyString) { return leftResult; }
			int position = leftResult.rfind(rightResult);
			if (position != string::npos) { return leftResult.erase(position, rightResult.length()); }
			else return leftResult;
		}

		else if (node->value == operations2children[multiplicationIndex]) // multiplication
		{
			if (rightResult[0] == quotationMark) { rightResult = rightResult.substr(1, rightResult.length() - 2); } // remove quotation marks from searched string
			if (rightResult == emptyString) { return leftResult; }
			// find all occurences of first character of rightResult in leftResult:
			vector<int> positions;
			for (int i = 0; i < leftResult.length(); i++)
			{
				if (leftResult[i] == rightResult[0]) { positions.push_back(i); }
			}
			if (positions.size() == 0) { return leftResult; }
			//append right string to each position:
			string result = leftResult.substr(0, positions[0]); // first part of the string before first position to add
			for (int i = 0; i < positions.size(); i++)
			{
				result += rightResult; // add right string
				if (i < positions.size() - 1)
					result += leftResult.substr(positions[i] + 1, positions[i + 1] - (positions[i] + 1)); // add part of the string between current and next position
				else
					result += leftResult.substr(positions[i] + 1, leftResult.length() - 1);
			}
			return result;
		}
		else if (node->value == operations2children[divisionIndex]) //division
		{
			if (rightResult[0] == quotationMark) { rightResult = rightResult.substr(1, rightResult.length() - 2); } // remove quotation marks from searched string
			if (rightResult == emptyString) { return leftResult; }
			//find all occurences of rightResult in leftResult:
			vector<int> positions;
			for (int i = 0; i < leftResult.length(); i++)
			{
				if (leftResult[i] == rightResult[0])
				{
					bool areIdentical = true;
					for (int j = 0; j < rightResult.length(); j++)
					{
						if (leftResult[i + j] != rightResult[j]) { areIdentical = false; }
						if (j == rightResult.length() - 1 && areIdentical) { positions.push_back(i); }
					}
				}
			}
			if (positions.size() == 0) { return leftResult; }
			//remove all occurences of rightResult from leftResult:
			string result = leftResult.substr(0, positions[0] + 1); // first part of the string before first position to substract
			for (int i = 0; i < positions.size(); i++)
			{
				if (i < positions.size() - 1)
					result += leftResult.substr(positions[i] + rightResult.length(), positions[i + 1] + 1 - (positions[i] + rightResult.length())); // add part of the string between current and next position

				else
					result += leftResult.substr(positions[i] + rightResult.length(), leftResult.length() - 1);
			}
			return result;

		}
	}

	else if (node->type == 1)
	{
		string childResult = calculate(node->children[0], vars, values);
		if (node->value == operations1child[0] || node->value == operations1child[1]) { return node->value + openingBracket + childResult + closingBracket; }
	}
	return emptyString;
}

template <typename T> T CNode<T>::calculate(CNode<T>* node, const vector<string> vars, const vector<T> values)
{
	if (node == NULL) { return 0; }

	else if (node->type == IDconstant)  // if its a constant, simply return its value
	{
		bool overflow;
		double value = strToNumber(node->value, &overflow);
		if (overflow)
		{
			value = strToNumber(defaultNodeValue, &overflow);
			logError(notification_overflow + defaultNodeValue);
		}
		return value;
	}

	else if (node->type == IDvariable) // if its a variable, find its value in values vector
	{
		int index = find(vars.begin(), vars.end(), node->value) - vars.begin(); // variable must be in the vars vector, so index will be valid (length is checked before)
		return values[index];
	}

	else if (node->type == 2) // if its a normal operator, calculate the values
	{
		double leftResult = calculate(node->children[0], vars, values);
		double rightResult = calculate(node->children[1], vars, values);

		if (node->value == operations2children[0])
		{
			if ((leftResult + rightResult) < 0) // Checking for overflow
			{
				logError(notification_overflow + defaultNodeValue);
				bool overflow;
				return strToNumber(defaultNodeValue, &overflow);
			}
			return leftResult + rightResult;
		}
		else if (node->value == operations2children[1]) { return leftResult - rightResult; } // never overflows: both positive valid ints
		else if (node->value == operations2children[multiplicationIndex])
		{
			if ((leftResult * rightResult) < 0) // Checking for overflow
			{
				logError(notification_overflow + defaultNodeValue);
				bool overflow;
				return strToNumber(defaultNodeValue, &overflow);
			}
			return leftResult * rightResult;
		}
		else if (node->value == operations2children[divisionIndex])
		{
			if (rightResult == 0) { logError(notification_divisionByZero); return rightResult; }
			else return double(leftResult) / double(rightResult);
		}
	}

	else if (node->type == 1)
	{
		double childResult = calculate(node->children[0], vars, values);
		if (node->value == operations1child[0]) { return sin(childResult); }
		if (node->value == operations1child[1]) { return cos(childResult); }
	}

	return 0;
}






template <typename T> double CNode<T>::strToNumber(const string value, bool* overflow)
{
	// convert string to int
	// of overflow, set overflow to true
	// value must be a number (checked before call, function is private)
	*overflow = false;
	double result = 0;
	bool isInt = true;
	int reduceValueAfterSeparator = 1;
	for (int i = 0; i < value.length(); i++) {
		if (value[i] == decimalSeparator)
		{
			isInt = false;
			i++;
		}
		if (isInt)
		{
			result *= baseNumber;
			result += (value[i] - minDigit);
		}
		else
		{
			result += double(value[i] - minDigit) / (baseNumber * reduceValueAfterSeparator);
			reduceValueAfterSeparator *= baseNumber;
		}

		if (result < 0) {
			*overflow = true;
		}
	}

	return result;
}



template <typename T> void CNode<T>::deleteTree() noexcept // delete the called node and all its descendants
{
	if (this == NULL) { return; }
	for (int i = 0; i < maxChildrenCount; i++)
	{
		children[i]->deleteTree();
	}
	delete this;
}


//Avoid linker errors by using explicit instantations:
template class CTree<int>;
template class CNode<int>;
template class CTree<double>;
template class CNode<double>;
template class CTree<string>;
template class CNode<string>;