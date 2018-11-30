#ifndef _STRING_ANALYZER_H
#define	_STRING_ANALYZER_H

#include <string>
#include <iostream>
#include <stdio.h>
        
using namespace std;

class String_Analyzer
{
public:    
    int counter;
    
    String_Analyzer();
    ~String_Analyzer();
    
    void Tolower(string&);
    
    void RemoveSpecialChar(string&);
    void RemoveNonBlankSpecialChar(string&);

    void TokenStr(string&, int);
    
    void RemoveStr(string&, const string&);
    void ReplaceStr(string&, const string&, string&);
  
    string ParseTokenData(std::string, char*);    //Rid of empty spaces inside a string

    string GetStringForWebListDescription(std::string&, const std::string&, int MaxStr);
    string GetStringForWebListDescriptionForAverage(std::string&, const std::string&);
    void GetStringFixDescription(std::string&StrData);
    
    //Count of occurence of string
    int CountStringOccurrence(std::string&, const std::string&);
    //Count of occurence of string by average found
    int CountStringOccurrenceByAverage(std::string&, const std::string&, double Average);
    int CountStringOccurrenceMatchingByAverage(std::string&StrData, const std::string&find, double Average);

    int IsKeyWordRelevant(string Keyword);
    string GetFirstToken(std::string&StrData);
    string GetNotFirstToken(std::string&StrData);
    void Trim(string& str);

private:
    
};
#endif	/* _STRING_ANALYSIS_H */

