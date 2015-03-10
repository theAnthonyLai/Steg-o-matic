#include "provided.h"
#include <string>
#include <vector>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
	string bitString = "";
    string encodedString = "";
    for (int i = 0; i < numbers.size(); i++) {
        bitString = convertNumberToBitString(numbers[i]);
        
        //  compute from binary to spaces and tabs
        for (int j = 0; j < bitString.size(); j++) {
            if (bitString[j] == '0')
                encodedString += ' ';
            else
                encodedString += '\t';
        }
                                                     
    }
    return encodedString;
}

bool BinaryConverter::decode(const string& bitString, vector<unsigned short>& numbers)
{
	if (bitString.size() % 16 != 0)
        return false;
    
    //  convert tabs and spaces to 1s and 0s
    //  return false if contain none-space and none-tab char
    string binaryString = "";
    for (int i = 0; i < bitString.size(); i++){
        if (bitString[i] == ' ')
            binaryString += '0';
        else if (bitString[i] == '\t')
            binaryString += '1';
        else
            return false;
    }
    
    //  convert binary to digital
    numbers.clear();
    int nNum = binaryString.size() / 16;
    unsigned short convertedNumber;
    for (int i = 0; i < nNum; i++) {
        if (!convertBitStringToNumber(binaryString.substr((i+i*16),16), convertedNumber))
            return false;
        numbers.push_back(convertedNumber);
    }
    
    return true;
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}
