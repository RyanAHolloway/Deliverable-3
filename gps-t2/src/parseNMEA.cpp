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

                string NMEACheck = "\\$([A-Z]{5})\\*[0-9A-Za-z]{2}"; // Checks NMEAString contains $ | 5 letters | any fields (if applicable) | * + checksum

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






  bool hasValidChecksum(std::string)
  {
      // Stub definition, needs implementing
      return false;
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
