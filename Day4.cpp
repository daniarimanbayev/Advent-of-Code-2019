/******************************************************************************

Advent of Code Day 4, Daniar Imanbayev

Objective: Count how many numbers (passwords) that are in between two six-digit numbers, 246540 and 787419, that follow the below constraints:
  - is a six-digit number.
  - at least two adjacent digits are the same (like 22 in 122345).
  - at least two adjacent digits are NOT part of a larger group of matching digits (123444 no longer meets the criteria)
  - From left to right, the digits never decrease (only ever increase or stay the same like 111123 or 135679)



I designed this code to run for two numbers of ANY (matching) length, and with any input integer of said length (it felt wrong to hardcode it to just 6 digits)
  - To input your own values for the input minimum and maximum, see first two lines of main. 
  - To change your input value digit length, see #define directives

*******************************************************************************/
#include <stdio.h>
#include<iostream> 
using namespace std;

#define PSWDLEN 6



void int_to_intarr(int input, int* arr)
{
	int num = input;

	for (int i = PSWDLEN - 1; i >= 0; i--)
	{
		arr[i] = num % 10;
		num /= 10;
	}
}

int intarr_to_int(int* input)
{
	int num = 0;

	for (int i = 0; i < PSWDLEN; i++)
	{
		num *= 10;
		num += input[i];
	}
	return num;
}

int validMin(int mininput)
{
	static int* minimumpswd = new int[PSWDLEN];

	int_to_intarr(mininput, minimumpswd);


	int highest = 0;


	for (int i = 0; i < PSWDLEN; i++)
	{
		if (minimumpswd[i] < highest)
		{
			minimumpswd[i] = highest;   // replace lower digit with highest 
		}
		else if (minimumpswd[i] > highest)
		{
			if (i == PSWDLEN - 1)           // if last digit is greater than highest
			{
				minimumpswd[PSWDLEN - 2]++;       // increment digit before (add 10)
				minimumpswd[PSWDLEN - 1] = minimumpswd[PSWDLEN - 2];    // and make last digit = to highest (making a double adjacent)
			}
			else
			{
				highest = minimumpswd[i];   // digit is new highest digit
			}
		}
		else
		{
			//same digit
		}
	}

	int validmin = intarr_to_int(minimumpswd);
	delete[] minimumpswd;

	return validmin;
}

int validMax(int maxinput)
{
	static int* maximumpswd = new int[PSWDLEN];

	int_to_intarr(maxinput, maximumpswd);

	int highest = 0;
	int highest_index = 0;


	for (int i = 0; i < PSWDLEN; i++)
	{
		// next digit is smaller or is last digit (and greater), go back to when highest was found and repeat 9 to end
		if (maximumpswd[i] < highest)
		{
			maximumpswd[highest_index] = highest - 1;
			for (int j = highest_index+1; j < PSWDLEN; j++)
			{
				maximumpswd[j] = 9;
			}
		}
		// next digit is last digit and greater than highest (prev), so make last digit equal to highest 
		else if ((maximumpswd[i] > highest) && (i == PSWDLEN - 1)) 
		{
			maximumpswd[highest_index] = highest;
		}
		// next digit is greater so update highest and highest_index
		else if (maximumpswd[i] > highest)
		{
			highest = maximumpswd[i];
			highest_index = i;
		}
		// next digit is same, do nothing
		else
		{
		}
	}

	int validmax = intarr_to_int(maximumpswd);
	delete[] maximumpswd;

	return validmax;
}

int countAdjacents(int higher, int lower)
{
	int count = 0;

	for (int i = lower; i <= higher; i++)
	{
		int has_adjacent_flag = 0;

		int* num = new int[PSWDLEN];
		int_to_intarr(i, num);

		for (int j = 0; j < PSWDLEN - 1; j++)
		{
			if (num[j] == num[j + 1])
			{
				if ( (j == 0) && (num[j + 1] != num[j + 2]))	// 
				{
					has_adjacent_flag = 1;
				}
				if ( (j == PSWDLEN-1) && (num[j - 1] != num[j]))
				{
					has_adjacent_flag = 1;
				}
				if ((num[j - 1] != num[j]) && (num[j + 1] != num[j + 2])) 
				{
					has_adjacent_flag = 1;
				}
			}
		}

		int always_increasing = 1;

		for (int j = PSWDLEN-1; j >= 1; j--)
		{
			if (num[j] < num[j - 1])
			{
				always_increasing = 0;
			}
		}

		delete[] num;

		if (has_adjacent_flag && always_increasing)
		{
			count++;
		}
	}

	return count;
}



int main()
{
	int inputmin = 246540;
	int inputmax = 787419;

	int validmin = validMin(inputmin);
	int validmax = validMax(inputmax);

	cout << "validmin: " << validmin << "   validmax: " << validmax;

	int validPasswords = countAdjacents(validmax, validmin);

	cout << "     Valid passwords: " << validPasswords;

}




// below are some extra cases i wrote for myself when working on figuring out the maximum/minimum values

// with the start passwords, make a function to find their true min and true max
// essentially, find the smallest valid password above true min (should most likely be )
// cases: validmin(123454) = 123455 
// cases: validmin(900000) = 999999
// cases: validmin(123456) = 123466  because smallest allowed double adjacent would be adding 10
// cases: validmin(123459) = 123466  increase tens place and force a double adjacent
// cases: validmin(123789) = 123799  
// cases: validmin(123788) = 123788  
// cases: validmin(123778) = 123788
// cases: validmin(123987) = 123999  basically, if next char < highestchar, repeat highestcar  
// cases: validmin(212345) = 222345  basically, if next char < highestchar, repeat highestcar  
// cases: validmin(123322) = 123333 

// for validmax, store highestchar, store highestcharlocation. all start at zero.
// on first iteration, highestchar = a, highestcharlocation = 0.
// on 2nd iteration, highestchar = b, highestcharlocation = 1, and so on until the end.
// cases: validmax(123456) = 123455  if next char > highestchar AND this is last character, go to when highestchar was found, highestchar - 1, and repeat to end
// cases: validmin(654321) = 123999    
