/** @file binaryInteger.cpp
@brief Declares, defines and implements an Integer class
@author Connor Hennen
@date October 4, 2016

First an Integer class is declared, then it is used in a main function that performs and outputs arithmetic and comparisons, and then the class is defined.
*/


#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iterator>

#ifndef INTEGER_H
#define INTEGER_H

/** @class Integer
@brief Stores an integer value using bits

This class is designed to mimic the int data type but with no inherent limit on the size of the number to be stored. There is a variable for the
binary representation of the number, and functions for performing and outputting the results of addition and multiplication, as well as comparisons.
*/
class Integer {
public:
	// Constructors
	Integer();
	Integer(unsigned int a);

	// Overloaded operators
	Integer& operator+=(const Integer& rhs); //overloaded += operator
	Integer& operator*=(const Integer& rhs); //overloaded *= operator
	bool operator<(const Integer& rhs) const; //overloaded < operator
	bool operator==(const Integer& rhs) const; //overloaded == operator

	// Procedures
	void print_as_int()  const; // prints value in decimal form
	void print_as_bits() const; // prints value in binary form

	// Accessors
	std::vector<bool> get_binary() const; // creates copy of object's bit vector

private:
	//Variable
	std::vector<bool> bit; // stores value in little endian binary format

	//Procedure
	void clean_up(); //removes any leading zero's from bit variable
};

// Non-member functions
Integer operator+(Integer a, const Integer b); // overloaded + operator
Integer operator*(Integer a, const Integer b); // overloaded * operator
bool operator>(const Integer& lhs, const Integer& rhs); // overloaded > operator
bool operator<=(const Integer& lhs, const Integer& rhs); // overloaded <= operator
bool operator>=(const Integer& lhs, const Integer& rhs); // overloaded >= operator
bool operator!=(const Integer& lhs, const Integer& rhs); // overloaded != operator

#endif

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

/** @function main
@brief Performs arithmetic and comparisons on two unsigned int inputs using binary representation
@author Connor Hennen
@date October 4, 2016

Takes in two input int values, stores them as Integers, outputs their binary representations, performs and outputs results of addition, multiplication, and comparisons
*/

int main() {
	unsigned int uint_value;

	std::cout << "Please input an integer a: ";
	std::cin >> uint_value;
	Integer a = uint_value; // Create Integer type with input value
	std::cout << "The base-2 representation of a is: "; a.print_as_bits();
	std::cout << std::endl;
	std::cout << "Please input an integer b: ";
	std::cin >> uint_value;
	Integer b = uint_value; // Create Integer type with input value
	std::cout << "The base-2 representation of b is: "; b.print_as_bits();
	std::cout << std::endl;
	// Let ' s do some basic arithmetic
	Integer c = a + b;
	Integer d = a*b;
	// Print out values
	std::cout << "a + b = "; c.print_as_int(); std::cout << " = ";
	c.print_as_bits(); std::cout << std::endl;
	std::cout << "a * b = "; d.print_as_int(); std::cout << " = "; d.print_as_bits();
	std::cout << std::endl;
	// Test comparison functions
	if (a < b) std::cout << "a < b" << std::endl;
	if (a <= b) std::cout << "a <= b" << std::endl;
	if (a == b) std::cout << "a == b" << std::endl;
	if (a != b) std::cout << "a != b" << std::endl;
	if (a >= b) std::cout << "a >= b" << std::endl;
	if (a > b) std::cout << "a > b" << std::endl;


	return 0;
}

/** Default constructor with no parameters
Sets private variable of Integer class object to a vector with one boolean input, FALSE
*/
Integer::Integer() {
	bit.push_back(0);
}

/** Constructor with one parameter
@param a is the numeric value to be stored

@return Object of Integer class, representing a numeric value in little endian binary format
*/
Integer::Integer(unsigned int a) {
	// input is 0
	if (a == 0) bit.push_back(0);
	// input is at least 1
	while (a > 0) {
		bit.push_back(a % 2);
		a /= 2;
	}
	clean_up();
}

/** Prints number in standard decimal base 10 format
*/
void Integer::print_as_int() const {
	unsigned long long value = 0;

	// Converts binary representation of a number into decimal representation
	for (size_t i = 0, j = bit.size(); i < j; ++i) {
		// Adds 2^i to our decimal value if i == 1
		if (bit[i] == 1) {
			value = value + pow(2, (i));
		}
		else {
			value = value;
		}
	}
	std::cout << value;
}

/** Prints integer in big endian binary format
*/
void Integer::print_as_bits() const {
	std::cout << "(";
	std::reverse_copy(bit.begin(), bit.end(), std::ostream_iterator<int>(std::cout));
	std::cout << ")_2";
}

/** Accesses by value private variables of Integer class
@returns Copy of private bit variable
*/
std::vector<bool> Integer::get_binary() const {
	return bit;
}

/** Overloaded += operator
@param rhs An object of Integer class

@return The integer class object that invoked the function, with its value updated
*/
Integer& Integer::operator+=(const Integer& rhs) {
	// Declare copies of the bit variable of the object that is to be operated on, and of the input parameter
	std::vector<bool> rhsCopy = rhs.bit;
	std::vector<bool> copyBit = bit;

	// Make vectors of binary values to be added the same size.
	for (size_t i = 0, n = bit.size(); i<n; ++i)
	{
		rhsCopy.push_back(0);
	}

	for (size_t i = 0, n = rhs.bit.size(); i<n; ++i)
	{
		bit.push_back(0);
	}

	/** Declare vector of ints that will store the sum. Note: because bool cannot take a value of 2, we use a vector of ints to help account for carrying in binary arithmetic */
	std::vector<int> sum(bit.size() + rhsCopy.size());

	// If bit is greater than rhs.bit, we will add rhs.bit to the int equivalent of bit
	if (bit.size() >= rhs.bit.size()) {

		// Converts bools of 0's and 1's in bit to ints of 0's and 1's in sum
		for (size_t i = 0, n = bit.size(); i<n; ++i) {
			if (bit[i] == 1) {
				sum[i] = 1;
			}
			else
			{
				sum[i] = 0;
			}
		}

		// Perform arithmetic with little endian binary vectors
		for (size_t i = 0, n = bit.size(); i<n; ++i) {
			if (sum[i] >= 1 && rhsCopy[i] == 1)
			{
				sum[i] = sum[i] - 1;
				sum[i + 1] = sum[i + 1] + 1;
			}
			else if (sum[i] > 1 && rhsCopy[i] == 0)
			{
				sum[i] = 0;
				sum[i + 1] = sum[i + 1] + 1;
			}
			else if (sum[i] == 1 ^ rhsCopy[i] == 1)
			{
				sum[i] = 1;
			}
			else
			{
				sum[i] = 0;
			}
		}
	}

	// If bit is lesser than rhs.bit, we will add bit to the int equivalent of rhs.bit
	if (rhs.bit.size() > copyBit.size()) {
		//Converts bools of 0's and 1's in rhs.bit to ints of 0's and 1's in sum
		for (size_t i = 0, n = rhs.bit.size(); i<n; ++i) {
			if (rhsCopy[i] == 1) {
				sum[i] = 1;
			}
			else
			{
				sum[i] = 0;
			}
		}

		// Perform arithmetic with little endian binary vectors
		for (size_t i = 0, n = bit.size(); i<n; ++i) {

			if (sum[i] >= 1 && bit[i] == 1)
			{
				sum[i] = sum[i] - 1;
				sum[i + 1] = sum[i + 1] + 1;
			}

			else if (sum[i] == 0 && bit[i] == 1)
			{
				sum[i] = 1;
			}
			else if (sum[i] == 1 && bit[i] == 0)
			{
				sum[i] = 1;
			}
			else
			{
				sum[i] = 0;
			}
		}
	}

	// Declare vector that will store boolean values of sum
	std::vector<bool> bitCopy(sum.size() + 2);

	// Convert ints of 0's and 1's in sum to bools of 0's and 1's in bitCopy
	for (size_t i = 0, n = sum.size(); i < n; ++i) {
		if (sum[i] == 1) {
			bitCopy[i] = true;
		}

		else if (sum[i] == 0)
		{
			bitCopy[i] = false;
		}

	}
	bit.push_back(0);
	bit.resize(bitCopy.size());
	//Set bit equal to bitCopy
	bit = bitCopy;
	clean_up();

	return *this;
}

/** Overloaded *= operator
@param rhs An object of Integer class

@return The integer class object that invoked the function, with its value updated
*/
Integer& Integer::operator*=(const Integer& rhs) {
	//Create copies of parameter's bit variables
	std::vector<bool> rhsCopy = rhs.bit;

	// Next nine lines make the two vectors the same length
	for (size_t i = 0, n = bit.size(); i<n; ++i)
	{
		rhsCopy.push_back(0);
	}

	for (size_t i = 0, n = rhs.bit.size(); i<n; ++i)
	{
		bit.push_back(0);
	}


	// Declare vector of ints
	std::vector<int> sum(bit.size() + rhsCopy.size());

	//Make sum the int equivalent of the value of object to be operated on
	for (size_t i = 0, n = bit.size(); i<n; ++i) {
		if (bit[i] == 1) {
			sum[i] = 1;
		}
		else
		{
			sum[i] = 0;
		}
	}

	// Declare vector of ints that will store the sum as we multiply through the bottom numbers inputs
	std::vector<int> rollingSum(bit.size() + rhsCopy.size());

	/** Perform vector multiplication. Goes through each input of the bottom number and if it equals 1, then the top number is added a copy of itself with placeholding zero's, in the same way that binary multiplication works */
	for (size_t i = 0, n = bit.size(); i < n; ++i) {
		if (rhsCopy[i] == 1)
		{
			//Next four lines will add placeholder 0's, as is done in standard multiplication arithmetic
			std::vector<int> intermediateSum = sum;
			std::vector<int>::iterator it;
			it = intermediateSum.begin();
			intermediateSum.insert(it, i, 0);

			//Adds a copy of the top number with placeholding 0's accounted for to another copy of the top number
			for (size_t q = 0, m = bit.size(); q < m; ++q)
			{
				if (rollingSum[q] >= 1 && intermediateSum[q] == 1)
				{
					rollingSum[q] = rollingSum[q] - 1;
					rollingSum[q + 1] = rollingSum[q + 1] + 1;
				}
				else if (rollingSum[q] > 1 && intermediateSum[q] == 0) {
					rollingSum[q] = 0;
					rollingSum[q + 1] = rollingSum[q + 1] + 1;
				}
				else if (rollingSum[q] == 1 ^ intermediateSum[q] == 1)
				{
					rollingSum[q] = 1;
				}
				else {
					rollingSum[q] = 0;
				}
			}
		}
		else
		{
			rollingSum = rollingSum;
		}
	}

	// Declare vector that will store boolean values of sum
	std::vector<bool> bitCopy(bit.size() + 2);

	// Convert ints of 0's and 1's in rollingSum to bools of 0's and 1's in bitCopy
	for (size_t i = 0, n = bit.size(); i <= n; ++i) {
		if (rollingSum[i] == 1) {
			bitCopy[i] = true;
		}

		else if (rollingSum[i] == 0)
		{
			bitCopy[i] = false;
		}

	}
	bit.push_back(0);
	bit.resize(bitCopy.size());
	//Set bit equal to bitCopy
	bit = bitCopy;
	clean_up();
	return *this;
}

/** Overloaded < operator
@param rhs An object of Integer class

@return A bool that is true if the implicit parameter is smaller than the explicit parameter, and false otherwise
*/
bool Integer::operator<(const Integer& rhs) const
{
	// Assume bool is false by default
	bool a = false;

	// Check vector size, if the explicit parameter rhs Integer bit size is larger then it must be greater
	if (bit.size() < rhs.bit.size()) {
		a = true;
	}

	// If the bit sizes are equal, check each element one by one
	if (bit.size() == rhs.bit.size()) {
		for (size_t p = bit.size(), q = rhs.bit.size(), i = 1; p >= i && (bit[p - i] <= rhs.bit[q - i]); ++i) {
			if (bit[p - i] < rhs.bit[q - i]) {
				a = true;
				break;
			}
		}
	}
	return a;
}

/** Overloaded == operator
@param rhs An object of Integer class

@return A bool that is true if the implicit parameter is equal to the explicit parameter, and false otherwise
*/
bool Integer::operator==(const Integer& rhs) const
{
	// Assume bool is true by default
	bool a = true;

	// If the two vectors being compared aren't the same size, they cannot be equal
	if (bit.size() != rhs.bit.size()) {
		a = false;
	}

	// If the two vectors are the same size, check each element for equality
	else {
		for (size_t p = bit.size(), q = rhs.bit.size(), i = 0; p > i && q > i; ++i) {
			if (bit[i] != rhs.bit[i]) {
				a = false;
				break;
			}
		}
	}

	return a;
}

// Removes any potential 0's that come after the last 1, which in little endian format aren't significant
void Integer::clean_up() {

	size_t p = bit.size(); // store the size of current unmodified vector

	// Removes all zero's starting at the end of the function, until it reaches the first 1 value
	for (size_t i = 1; bit[p - i] == 0; ++i) {
		bit.pop_back();
	}
}

/** Overloaded + operator
@param a An object of Integer class
@param a An object of Integer class

@return A copy of the result of value a iterated by value b
*/
Integer operator+(Integer a, const Integer b) {
	return a += b;
}

/** Overloaded * operator
@param a An object of Integer class
@param a An object of Integer class

@return A copy of the result of value a iterated by value a b times
*/
Integer operator*(Integer a, const Integer b) {
	return a *= b;
}

/** Overloaded > operator
@param lhs An object of Integer class
@param rhs An object of Integer class

@return Bool true if lhs is greater than rhs and false otherwise
*/
bool operator>(const Integer& lhs, const Integer& rhs) {
	return rhs < lhs;
}

/** Overloaded > operator
@param lhs An object of Integer class
@param rhs An object of Integer class

@return Bool true if lhs is less than or equal to rhs and false otherwise
*/
bool operator<=(const Integer& lhs, const Integer& rhs) {
	return !(lhs > rhs);
}

/** Overloaded > operator
@param lhs An object of Integer class
@param rhs An object of Integer class

@return Bool true if lhs is greater than or equal to rhs and false otherwise
*/
bool operator>=(const Integer& lhs, const Integer& rhs) {
	return !(lhs < rhs);
}

/** Overloaded != operator
@param lhs An object of Integer class
@param rhs An object of Integer class

@return Bool true if lhs is not equal to rhs and false if lhs is equal to rhs
*/
bool operator!=(const Integer& lhs, const Integer& rhs) {
	return !(lhs == rhs);
}
