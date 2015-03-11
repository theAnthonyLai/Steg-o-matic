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
            //  VAGUE what if there are more than one \r?
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
            hostOut += "\n";
            hostOut += encodedToAppend;
        }
    }
    return true;
}

bool Steg::reveal(const string& host, string& msg)
{
    bool isHostValid = true;
    string encodedMsg = "";
    string thisLine = "";
    string whiteSpace = " \t\r";
    int lineStart = 0;
    for (int i = 0; i < host.size(); i++) {
        if (host[i] == '\n') {
            //  get the substring of original encodedMsg from host
            thisLine = host.substr(lineStart, i - lineStart);
            int toCut = thisLine.find_last_of(whiteSpace);
            if (toCut < 0) {
                //  VAGUE should break immeadiately?
                //  b/c if using hide, every line after this should not end with any space or tab
                isHostValid = false;
                continue;
            } else if (!isHostValid)
                //  there is line before this that does not end with spaces or tabs
                //  but this line end with spaces and/or tabs
                return false;
            thisLine.substr(toCut); //  get everythign from this pos to the end
            
            //  append the substring of original encodedMsg
            encodedMsg += thisLine;
        }
    }
    
    vector<unsigned short> msgNum;
    if (!BinaryConverter::decode(encodedMsg, msgNum))
        return false;
    if (!Compressor::decompress(msgNum, msg))
        return false;
    
    return true;
}
