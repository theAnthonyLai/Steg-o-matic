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
    string copiedHostIn = hostIn;
    for (int i = 0; i < hostIn.size(); i++)
        if (hostIn[i] == '\n')
            nLines++;
    if (hostIn[hostIn.size() - 1] != '\n') {
        //  hostIn does not end with new line char
        nLines++;
        copiedHostIn += "\n";
    }
    
    //encodedMsg = "         	     	";
    int nLinesToHaveExtra = encodedMsg.size() % nLines;
    int nCharInExtra = encodedMsg.size() / nLines + 1;
    int cutStartPos = 0;    //  position to start cutting the encodedMsg
    
    hostOut = "";
    string thisLine = "";
    string encodedToAppend;
    string whiteSpace = " \t";
    int lineStart = 0;
    for (int i = 0; i < copiedHostIn.size(); i++) {
        if (copiedHostIn[i] == '\n') {
            //  prepare the substring of original stuff from hostIn
            thisLine = copiedHostIn.substr(lineStart, i - lineStart);
            lineStart = i + 1;
            //  VAGUE what if there are more than one \r?
            if (thisLine[thisLine.size()-1] == '\r')
                //  delete the last character if it is '\r'
                thisLine.pop_back();
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
            hostOut += "\n";
        }
    }
    return true;
}

bool Steg::reveal(const string& host, string& msg)
{
    string copiedHost = host;
    if (host[host.size() - 1] != '\n')
        copiedHost += "\n";
    
    string encodedMsg = "";
    string thisLine = "";
    string whiteSpace = " \t";
    int lineStart = 0;
    for (int i = 0; i < copiedHost.size(); i++) {
        if (copiedHost[i] == '\n') {
            //  get the substring of original encodedMsg from host
            thisLine = copiedHost.substr(lineStart, i - lineStart);
            lineStart = i + 1;
            if (thisLine[thisLine.size() - 1] == '\r')
                thisLine.pop_back();
            int toCut = thisLine.find_last_not_of(whiteSpace);
            //  ADD stuff here to check if valid
            thisLine = thisLine.substr(toCut + 1); //  get everythign from this pos to the end
            
            //  append the substring of original encodedMsg
            encodedMsg += thisLine;
        }
    }
    
    msg = "";
    vector<unsigned short> msgNum;
    if (!BinaryConverter::decode(encodedMsg, msgNum))
        return false;
    if (!Compressor::decompress(msgNum, msg))
        return false;
    
    return true;
}
