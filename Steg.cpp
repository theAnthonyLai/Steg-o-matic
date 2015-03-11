#include "provided.h"
#include <string>
#include <vector>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
	if (hostIn.size() == 0)
        return false;
    
    vector<unsigned short> msgNum;
    Compressor::compress(msg, msgNum);
    string encodedMsg = BinaryConverter::encode(msgNum);
    
    //  compute the number of lines in hostIn
    int nLines = 0;
    for (int i = 0; i < hostIn.size(); i++)
        if (hostIn[i] == '\n')
            nLines++;
    if (hostIn[hostIn.size() - 1] != '\n')
        //  hostIn does not end with new line char
        nLines++;
    
    int nLinesToHaveExtra = encodedMsg.size() % nLines;
    int nCharInExtra = encodedMsg.size() / nLines + 1;
    int cutStartPos = 0;    //  position to start cutting the encodedMsg
    
    hostOut = "";
    string thisLine = "";
    string encodedToAppend;
    string whiteSpace = " \t\r";
    int lineStart = 0;
    for (int i = 0; i < hostIn.size(); i++) {
        if (hostIn[i] == '\n') {
            //  prepare the substring of original stuff from hostIn
            thisLine = hostIn.substr(lineStart, i - lineStart);
            int toCut = thisLine.find_last_not_of(whiteSpace);
            thisLine.erase(toCut + 1);
            
            //  prepare the substring of encodedMsg to append
            if (nLinesToHaveExtra > 0) {
                encodedToAppend = encodedMsg.substr(cutStartPos, nCharInExtra);
                cutStartPos += nCharInExtra;    //  update the start of the cut position
                nLinesToHaveExtra--;    //  one less number of lines to have extra
            } else {
                encodedToAppend = encodedMsg.substr(cutStartPos, nCharInExtra - 1);
                cutStartPos += (nCharInExtra - 1);    //  update the start of the cut position
            }
            
            //  append the original stuff and encoded stuff to result
            hostOut += thisLine;
            hostOut += encodedToAppend;
        }
    }
    return true;
}

bool Steg::reveal(const string& hostIn, string& msg) 
{
	return false;  // This compiles, but may not be correct
}
