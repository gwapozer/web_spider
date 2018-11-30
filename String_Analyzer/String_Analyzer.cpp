#include <string>
#include "String_Analyzer.h"
using namespace std;

String_Analyzer::String_Analyzer() {

}

String_Analyzer::~String_Analyzer() {

}

void String_Analyzer::Trim(string& str) {
    string::size_type pos = str.find_last_not_of(' ');
    if (pos != string::npos) {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');
        if (pos != string::npos) str.erase(0, pos);
    } else str.erase(str.begin(), str.end());
}

void String_Analyzer::Tolower(string &str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = tolower((unsigned char) str[i]);
        }
    }
}

void String_Analyzer::RemoveSpecialChar(string &str) {
    string tmp_buffer = "";

    for (int i = 0; str[i] != '\0'; i++) {
        if (!(str[i] >= 0x00 && str[i] <= 0x40) && !(str[i] >= 0x5b
                && str[i] <= 0x60) && !(str[i] >= 0x7b && str[i] <= 0x7f)) {
            tmp_buffer = tmp_buffer + *(&str[i]);
        }
    }
    str = tmp_buffer;
}

void String_Analyzer::RemoveNonBlankSpecialChar(string &str) {
    string tmp_buffer = "";

    for (int i = 0; str[i] != '\0'; i++) {
        if (!(str[i] >= 0x00 && str[i] <= 0x1F) && !(str[i] >= 0x21 && str[i] <= 0x2F)
                && !(str[i] >= 0x3A && str[i] <= 0x40) && !(str[i] >= 0x5b
                && str[i] <= 0x60) && !(str[i] >= 0x7b && str[i] <= 0x7f)) {
            tmp_buffer = tmp_buffer + *(&str[i]);
        }
    }
    str = tmp_buffer;
}

void String_Analyzer::TokenStr(string &str, int HexAscii) {
    string buffer = "";
    string tmp_buffer = "";

    int pos = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != HexAscii) {
            tmp_buffer = tmp_buffer + *(&str[i]);
        } else {
            if (tmp_buffer != "") {
                if (pos == 0) {
                    buffer = tmp_buffer;
                    pos++;
                } else
                    buffer = buffer + " " + tmp_buffer;

                //printf("Token:%s\n", tmp_buffer.c_str());
            }
            tmp_buffer = "";
        }
    }

    if (tmp_buffer != "") {
        if (pos == 0) {
            buffer = tmp_buffer;
        } else
            buffer = buffer + " " + tmp_buffer;

        //printf("Token:%s\n", tmp_buffer.c_str());
    }

    str = buffer;
}

void String_Analyzer::RemoveStr(string &Data, const string &find) {
    string::size_type pos = 0;

    while ((pos = Data.find(find, pos)) != string::npos) {
        Data.erase(pos, find.length());
        pos += find.length();
    }
}

void String_Analyzer::ReplaceStr(string &Data, const string &find, string &replace_with) {
    string::size_type pos = 0;

    while ((pos = Data.find(find, pos)) != string::npos) {
        Data.erase(pos, find.length());
        Data.insert(pos, replace_with);
        pos += replace_with.length();
    }
}

string String_Analyzer::ParseTokenData(std::string StrData, char* Delimiter) {

    std::string StrBuffer;
    std::string TmpBuffer;

    char *Data;
    char *TokenPtr;

    int datasize = StrData.length() + 1;

    Data = new char[datasize];
    memset(Data, 0x0, datasize);
    memcpy(Data, StrData.c_str(), datasize);

    StrBuffer = "";

    TokenPtr = strtok(Data, Delimiter);

    while (TokenPtr != NULL) {
        TmpBuffer = TokenPtr;
        StrBuffer = StrBuffer + " " + TmpBuffer;
        TokenPtr = strtok(NULL, Delimiter);
    }

    delete[] Data;

    return StrBuffer;
}

string String_Analyzer::GetStringForWebListDescription(std::string&Data, const std::string&find, int MaxStr) {

    string tmp_buffer = "";
    int StrLen = 0;
    std::string::size_type pos = 0;
    int counter = 0;

    int CurrPos = 0;
    int SubStrDatasize = 0;

    string Tmp = "";

    StrLen = Data.length();
    SubStrDatasize = 50;

    if (SubStrDatasize > StrLen) {
        SubStrDatasize = StrLen;
    }

    while ((pos = Data.find(find, pos)) != std::string::npos) {

        CurrPos = pos - 20;
        if (CurrPos < 0) {
            CurrPos = pos;
        }
        //Ensure we do not append blank space in head of string.
        while (CurrPos <= pos) {
            Tmp = Data.substr(CurrPos, 1);
            if (Tmp == " ")
                CurrPos++;
            else
                break;
        }

        tmp_buffer = tmp_buffer + Data.substr(CurrPos, SubStrDatasize);
        counter++;
        pos += find.length();
    }

    if (MaxStr > tmp_buffer.length())
        MaxStr = tmp_buffer.length();
    tmp_buffer = tmp_buffer.substr(0, MaxStr);

    GetStringFixDescription(tmp_buffer);

    return tmp_buffer;
}

string String_Analyzer::GetStringForWebListDescriptionForAverage(std::string&StrData, const std::string&find) {
   
    string tmp_buffer = "";
    char* Delimiter = " ";
    char *Data;
    char *TokenPtr, *TKSaved;

    int datasize = StrData.length() + 1;

    Data = new char[datasize];

    memset(Data, 0x0, datasize);
    memcpy(Data, find.c_str(), datasize);

    TokenPtr = strtok_r(Data, Delimiter, &TKSaved);

    for (int i = 0; TokenPtr != NULL; i++) {
        string Word1 = "";
        Word1 = TokenPtr;
        Trim(Word1);

        if (IsKeyWordRelevant(Word1) == 1)
            tmp_buffer += GetStringForWebListDescription(StrData, TokenPtr, 55) + ". ";
        
        TokenPtr = strtok_r(NULL, Delimiter, &TKSaved);
    }

    int MaxStr = 255;
    if (MaxStr > tmp_buffer.length())
        MaxStr = tmp_buffer.length();
    tmp_buffer = tmp_buffer.substr(0, MaxStr);

    GetStringFixDescription(tmp_buffer);

    return tmp_buffer;
}

void String_Analyzer::GetStringFixDescription(std::string&StrData) {
    
    string tmp_buffer = "";
    char* Delimiter = " ";
    char *Data;
    char *TokenPtr, *TKSaved;

    int datasize = StrData.length() + 1;

    Data = new char[datasize];

    memset(Data, 0x0, datasize);
    memcpy(Data, StrData.c_str(), datasize);

    TokenPtr = strtok_r(Data, Delimiter, &TKSaved);

    for (int i = 0; TokenPtr != NULL; i++) {
        string BuffStr = "";

        BuffStr = TokenPtr;
        Trim(BuffStr);

        if (BuffStr.length() < 26)
            tmp_buffer += BuffStr + " ";

        TokenPtr = strtok_r(NULL, Delimiter, &TKSaved);
    }

    StrData = tmp_buffer;
}

int String_Analyzer::CountStringOccurrence(std::string&Data, const std::string&find) {
    std::string::size_type pos = 0;
    int counter = 0;

    while ((pos = Data.find(find, pos)) != std::string::npos) {
        counter++;
        pos += find.length();
    }

    return counter;
}

int String_Analyzer::CountStringOccurrenceByAverage(std::string&StrData, const std::string&find, double Average) {
    std::string::size_type pos = 0;
    int counter = 0;

    char* Delimiter = " ";

    char *Data;
    char *TokenPtr;

    int datasize = find.length() + 1;

    Data = new char[datasize];

    memset(Data, 0x0, datasize);
    memcpy(Data, find.c_str(), datasize);

    TokenPtr = strtok(Data, Delimiter);

    int TotalWord = 0;
    int WordCnt = 0;

    for (int i = 0; TokenPtr != NULL; i++) {
        TotalWord++;

        if (CountStringOccurrence(StrData, TokenPtr) > 0)
            WordCnt++;

        TokenPtr = strtok(NULL, Delimiter);
    }

    double CalcAvg = 0;

    CalcAvg = ((double) WordCnt / (double) TotalWord) * 100;

    //cout << "TotalWord: "  << TotalWord << endl << endl;
    //cout << "WORDCNT: "  << WordCnt << endl << endl;
    //cout << "AVG: "  << CalcAvg << endl << endl;

    if (CalcAvg > Average) {
        counter = 1;
    }

    return counter;
}

int String_Analyzer::CountStringOccurrenceMatchingByAverage(std::string&StrData, const std::string&find, double Average) {
    std::string::size_type pos = 0;
    int counter = 0;

    char* Delimiter = " ";

    //Find Token
    char *Data;
    char *TokenPtr, *TKSaved;

    int datasize = find.length() + 1;

    Data = new char[datasize];

    memset(Data, 0x0, datasize);
    memcpy(Data, find.c_str(), datasize);

    TokenPtr = strtok_r(Data, Delimiter, &TKSaved);
    //End

    int TotalWord = 0;
    int WordCnt = 0;

    for (int i = 0; TokenPtr != NULL; i++) {
        string Word1 = "";
        Word1 = TokenPtr;
        Trim(Word1);

        if (IsKeyWordRelevant(Word1) == 1)
            TotalWord++;

        char *Data2;
        char *TokenPtr2, *TKSaved2;

        int datasize2 = StrData.length() + 1;

        Data2 = new char[datasize2];

        memset(Data2, 0x0, datasize2);
        memcpy(Data2, StrData.c_str(), datasize2);

        TokenPtr2 = strtok_r(Data2, Delimiter, &TKSaved2);

        for (int j = 0; TokenPtr2 != NULL; j++) {
            string Word2 = "";
            Word2 = TokenPtr2;
            Trim(Word2);

            if (IsKeyWordRelevant(Word1) == 1) {
                if (Word1 == Word2) {
                    WordCnt++;
                    break;
                }
            }
            TokenPtr2 = strtok_r(NULL, Delimiter, &TKSaved2);
        }

        delete Data2;

        TokenPtr = strtok_r(NULL, Delimiter, &TKSaved);
    }

    double CalcAvg = 0;

    CalcAvg = ((double) WordCnt / (double) TotalWord) * 100;

    //cout << "TotalWord: "  << TotalWord << endl << endl;
    //cout << "WORDCNT: "  << WordCnt << endl << endl;
    //cout << "AVG: "  << CalcAvg << endl << endl;

    if (CalcAvg > Average) {
        counter = 1;
    }

    return counter;
}

const char* IrrelevantKeyWord[8] = {

    "a", "the", "to", "of", "an", "that", "i", "is"
};

int String_Analyzer::IsKeyWordRelevant(string Keyword) {
    int IsRelevant = 1;

    for (int i = 0; i < 8; i++) {
        if (Keyword == IrrelevantKeyWord[i]) {
            IsRelevant = 0;
        }
    }

    return IsRelevant;
}

string String_Analyzer::GetFirstToken(std::string&StrData) {

    string tmp_buffer = "";
    char* Delimiter = " ";
    char *Data;
    char *TokenPtr, *TKSaved;

    int datasize = StrData.length() + 1;

    Data = new char[datasize];

    memset(Data, 0x0, datasize);
    memcpy(Data, StrData.c_str(), datasize);

    TokenPtr = strtok_r(Data, Delimiter, &TKSaved);

    for (int i = 0; TokenPtr != NULL; i++) {
        string BuffStr = "";

        BuffStr = TokenPtr;
        Trim(BuffStr);

        if(i == 0)
            tmp_buffer = BuffStr;

        TokenPtr = strtok_r(NULL, Delimiter, &TKSaved);
    }

    return tmp_buffer;
}

string String_Analyzer::GetNotFirstToken(std::string&StrData) {

    string first_tmp_buffer = "";
    string tmp_buffer = "";
    char* Delimiter = " ";
    char *Data;
    char *TokenPtr, *TKSaved;

    int datasize = StrData.length() + 1;

    Data = new char[datasize];

    memset(Data, 0x0, datasize);
    memcpy(Data, StrData.c_str(), datasize);

    TokenPtr = strtok_r(Data, Delimiter, &TKSaved);

    for (int i = 0; TokenPtr != NULL; i++) {
        string BuffStr = "";

        BuffStr = TokenPtr;
        Trim(BuffStr);

        if(i == 0)
            first_tmp_buffer = BuffStr;
        if(i > 0)
            tmp_buffer += BuffStr + " ";

        TokenPtr = strtok_r(NULL, Delimiter, &TKSaved);
    }

    if(tmp_buffer == "")
        tmp_buffer = first_tmp_buffer;

    Trim(tmp_buffer);

    return tmp_buffer;
}
