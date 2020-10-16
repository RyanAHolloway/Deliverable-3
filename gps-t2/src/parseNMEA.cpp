#include "earth.h"
#include "parseNMEA.h"
#include <iostream>
#include <regex>
#include <vector>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <regex>
#include <locale>
#include <string.h>
#include <assert.h>
#include <numeric>

using namespace std;

namespace NMEA
{

//REFACTOR WHOLE THING
bool isWellFormedSentence(string NMEAString)
  {
	  bool isOk;
	  string firstDelim = "$";
	  string lastDelim = "*";
	  int nmeaCodeLength = 5;
	  string letterCheck = "[A-Z]+$";
	  string check$ = NMEAString.substr (0,1); //Get first character


	  // Refactor this part
	  size_t s = NMEAString.find(firstDelim); // Find $ in string

	  //Check if first char is $
	  if(check$ == firstDelim){
	    //Check if following char length is 5 chars
	    string checkAZ = NMEAString.substr (1,5); //Get following chars
	    if(checkAZ.length() == nmeaCodeLength){ //Currently only GPXXX however need to change to first string common
	        //string checkAZ = NMEAString.substr (1,5); //Get following chars
	  		bool onlyAlpha = regex_match(checkAZ, regex(letterCheck)); // Check if checkAZ contains only letters
	  		//Check if those 5 chars are all letters
    	    if(onlyAlpha == true){

                string NMEACheck = "\\$GP([A-Z]{3})(,([A-Z a-z 0-9\\,\\.]*)|)\\*[0-9A-Fa-f]{2}"; // Checks NMEAString contains $ | 5 letters | any fields (if applicable) | * + checksum

                bool hasStar = regex_match(NMEAString, regex(NMEACheck));

    	        if(hasStar == true){

    	            isOk = true;
    	        }
    	        else{
    	            isOk = false;
    	        }
    	    }
    	    else{
    	        isOk = false;
    	    }
	  	}
	  	else{
	  	    isOk = false;
	  	}
	  }
	  else{
		  isOk = false;
	  }

      return isOk;
  }

bool hasValidChecksum(string NMEAString)
	  {
		bool isValid;
	   bool pre = isWellFormedSentence(NMEAString); // Pre-condition: is a well formed NMEA sentence
	   //If isWellFormedSentence does not equal false
	   if(pre != false){

		    // Put given NMEAString into a string stream
			istringstream givenNMEA(NMEAString);
			string NMEASentence;
			getline(givenNMEA, NMEASentence, '*');// get the whole string up to the delimiter: *

			// Save checksum provided in NMEAString to: rCheckSum
			string rCheckSum;
			getline(givenNMEA, rCheckSum);

			// Calculate checksum through XOR
			char checkSum = accumulate(NMEASentence.begin()+1, NMEASentence.end(), 0, [](char sum, char ch) { return sum ^ ch; });

			// Convert variable to string for comparison
			stringstream sCheck;
			sCheck << (int)checkSum;
			string checkValue = sCheck.str();

			// Convert int to string for comparison
			int num = stoi(rCheckSum, 0, 16);
			string numToString = to_string(num);

			//Compare the given checksum with the calculated checksum
			if(numToString == checkValue){
				isValid = true;
			}
			else{
				isValid = false;
			}
	   }
	   //If isWellFormedSentence = false
	   else{
		   isValid = false;
	   }
		return isValid;;
	  }

  SentenceData extractSentenceData(std::string)
  {
      // Stub definition, needs implementing
      return {"",{}};
  }

  GPS::Position positionFromSentenceData(SentenceData)
  {
      // Stub definition, needs implementing
      return GPS::Earth::NorthPole;
  }

  Route routeFromLog(std::istream &)
  {
      // Stub definition, needs implementing
      return {};
  }

}
