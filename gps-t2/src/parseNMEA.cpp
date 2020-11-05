#include "earth.h"
#include "parseNMEA.h"
#include <iostream>
#include <vector> // Add use of vectors
#include <sstream> // Add use of string streams
#include <string>
#include <algorithm>
#include <regex> //Regular expressions
#include <locale>
#include <string.h>
#include <assert.h> // Assertions
#include <numeric> // Accumulate
#include <stdexcept> // Exception throw
#include <utility> // Pair

using namespace std;

/*!
*This application has been created to read and validate NMEA sentences \n
*It does this by extracting the data from the sentence and ensuring the format and checksum are suitable.
*/


namespace NMEA
{
    /*! \fn bool isWellFormedSentence(string NMEAString)
     * \brief This function determines whether the parameter is a well-formed NMEA sentence \n
     *\n
     * A NMEA sentence contains the following contents:\n
     * - The prefix "$GP" \n
     * - Followed by a sequence of three (English) alphabet characters identifying \n
     * - The sentence format \n
     * - Followed by a sequence of comma-separated data fields \n
     * - Followed by a '*' character \
     * - Followed by a two-character hexadecimal checksum \n
     * - The '$' and '*' characters are reserved, and may not appear in the data fields \n
     *\n
     * For ill-formed sentences, this function returns false (it does not throw an exception or terminate the program) \n
     * \param NMEAString
     * \return 'true' if all the criteria is met
     */

bool isWellFormedSentence(string NMEAString)
  {
	  bool isOk;
	  string firstDelim = "$";
	  string lastDelim = "*";
	  int nmeaCodeLength = 5;
	  string letterCheck = "[A-Z]+$";
	  string check$ = NMEAString.substr (0,1); //Get first character

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

/*! \fn bool hasValidChecksum(string NMEAString)
 *\brief This function verifies whether a sentence has a valid checksum.\n
 *To be valid, the checksum value should equal the XOR reduction of the character codes of all characters between the '$' and the '*' (exclusive).\n
 *\par Pre-condition
 *The parameter is a well-formed NMEA sentence.\n
 *\param NMEAString
 *\return 'true' if the checksum calculated matches the checksum provided
 */
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


/*! \fn bool extractSentenceData(string NMEAString)
 *\brief This function extracts the sentence format and the field contents from a NMEA sentence.\n
 *The '$GP' and the checksum are ignored.\n
*\par Pre-condition
 *The parameter is a well-formed NMEA sentence.\n
 *\param NMEAString
 *\return The sentence data is returned
 */
  SentenceData extractSentenceData(string NMEAString)
  {
	  pair <string, vector<string>> sentenceData;

	  bool pre = isWellFormedSentence(NMEAString); // Pre-condition: is a well formed NMEA sentence
	  //If isWellFormedSentence does not equal false
	  if(pre != false)
	  {
		  vector<string> givenNMEASentence; // vector for fields
		  string comma = ",";

		  //Get whole string up to *
		  istringstream givenNMEA(NMEAString);
		  string NMEASentence;
		  getline(givenNMEA, NMEASentence, '*');

		  //Get the format type
		  string nmeaFormat = NMEASentence.substr(3,3); //e.g $GPABC -> ABC

		  //If NMEASentence has a , then procede to add fields to vector
		  if(NMEASentence.find(comma) != std::string::npos){

		  	    string aNMEA = NMEASentence.substr(NMEASentence.find(comma)+1); //NMEASentence - $GPXXX - 1st ,

		  	    stringstream ss(aNMEA);

				  while(ss.good()) {
					  string field;
					  getline(ss, field, ',');
					  givenNMEASentence.push_back(field);
				  }
		  	}

		  //Create sentenceData pair from format and vector
		  sentenceData = make_pair(string(nmeaFormat), givenNMEASentence);
	  }
	  else
	  {
		  throw invalid_argument( "The NMEA sentence was not well formed" );
	  }

      return sentenceData; //{"",{}}
  }


  /*! \fn GPS::Position positionFromSentenceData (SentenceData sentenceData)
   *\brief This function computes a Position from NMEA Sentence Data.\n
   * Currently only supports the GLL, GGA and RMC sentence formats.\n
  *\par Exceptions
  * Throws an invalid argument exception for unsupported sentence formats, or if the necessary data fields are missing or contain invalid data.
  *\param SentenceData sentenceData
   *\return The position data from the NMEA sentence data
   */
  GPS::Position positionFromSentenceData(SentenceData sentenceData)
  {
	  string checkLat = "([0-9\\.]*)";
	  string checkLong = "([0-9\\.]*)";
	  string checkNS = "([NS]+)";
	  string checkEW = "([EW]+)";
	  string format = sentenceData.first;
	  string gll = "GLL";
	  string rmc = "RMC";
	  string gga = "GGA";

	  //If the vector in the pair is empty
	  if(sentenceData.second.empty()){
		  throw invalid_argument("Vector field is empty");
	  }else{
		  if(format == gll){ //If the format type is GLL

			  vector<string> gllFields = sentenceData.second; //vector of fields

			  string latitude = gllFields[0]; // get latitude field

			  string upOrDown = gllFields[1]; // get N or S field
			  char NorS = upOrDown[0];

			  string longitude = gllFields[2]; // get longitude field

			  string leftOrRight = gllFields[3]; // get E or W field
			  char EorW = leftOrRight[0];


			  bool latit = regex_match(latitude, regex(checkLat)); // compare format of field for latitude is valid
			  bool longit = regex_match(longitude, regex(checkLong)); // compare format of field for longitude is valid
			  bool ns = regex_match(upOrDown, regex(checkNS)); // compare format of field for N or S is valid
			  bool ew = regex_match(leftOrRight, regex(checkEW)); // compare format of field for E or W is valid

			  if(latit && longit && ns && ew == true){

				  string elev = "0";
				    // Create GPS::Position object called posit and return the object
					GPS::Position posit(latitude, NorS, longitude, EorW, elev);
					return posit;
			  }
			  else{
				  throw invalid_argument("Sentence field data is invalid!");
			  }

		  }
		  else if(format == rmc){ // If the format type is RMC
			  vector<string> rmcFields = sentenceData.second; //vector of fields

			  string latitude = rmcFields[2]; // get latitude field

			  string upOrDown = rmcFields[3]; // get N or S field
			  char NorS = upOrDown[0];

			  string longitude = rmcFields[4]; // get longitude field

			  string leftOrRight = rmcFields[5]; // get E or W field
			  char EorW = leftOrRight[0];

			  bool latit = regex_match(latitude, regex(checkLat)); // compare format of field for latitude is valid
			  bool longit = regex_match(longitude, regex(checkLong)); // compare format of field for longitude is valid
			  bool ns = regex_match(upOrDown, regex(checkNS)); // compare format of field for N or S is valid
			  bool ew = regex_match(leftOrRight, regex(checkEW)); // compare format of field for E or W is valid

			  if(latit && longit && ns && ew == true){

				  string elev = "0";

				    // Create GPS::Position object called posit and return the object
					GPS::Position posit(latitude, NorS, longitude, EorW, elev);
					return posit;
			  }
			  else{
				  throw invalid_argument("Sentence field data is invalid!");
			  }

		  }
		  else if(format == gga){ //If the format type is gga
			  vector<string> ggaFields = sentenceData.second; //vector of fields

			  string latitude = ggaFields[1]; // get latitude field

			  string upOrDown = ggaFields[2]; // get N or S field
			  char NorS = upOrDown[0];

			  string longitude = ggaFields[3]; // get longitude field

			  string leftOrRight = ggaFields[4]; // get E or W field
			  char EorW = leftOrRight[0];

			  string elevationValue = ggaFields[8]; // get the elevation field

			  bool latit = regex_match(latitude, regex(checkLat)); // compare format of field for latitude is valid
			  bool longit = regex_match(longitude, regex(checkLong)); // compare format of field for longitude is valid
			  bool ns = regex_match(upOrDown, regex(checkNS)); // compare format of field for N or S is valid
			  bool ew = regex_match(leftOrRight, regex(checkEW)); // compare format of field for E or W is valid

			  if(latit && longit && ns && ew == true){

				    // Create GPS::Position object called posit and return the object
					GPS::Position posit(latitude, NorS, longitude, EorW, elevationValue);
					return posit;
			  }
			  else{
				  throw invalid_argument("Sentence field data is invalid!");
			  }

		  }
		  else{
			  throw invalid_argument("Format not supported");
		  }
	  }

  }

  /*! \fn Route routeFromLog (std::istream &ss)
   *\brief This function reads a stream of NMEA sentences (one sentence per line), and constructs a route, ignoring any lines that do not contain valid sentences.\n
  *A sentence is valid if:\n
   *- It is a well-formed NMEA sentence\n
   *- The checksum is valid\n
   *- The sentence format is supported (currently GLL, GGA and RMC)\n
   *- The necessary data fields are present and contain valid data\n
  *\param std::istream &ss
   *\return The valid route positions in a vector
   */
  Route routeFromLog(std::istream &ss){

	  string sentenceFromLog; // Data to hold string stream info
	  vector <GPS::Position> routePositions; // Vector of GPS:Position objects

	  while(getline(ss, sentenceFromLog)){
		  // If the log is empty
		  if(!sentenceFromLog.size() == 0){
			  //If the sentence from the log is well formed
			  if(isWellFormedSentence(sentenceFromLog) != false){
				  //If the sentence has a valid checksum value
				  if(hasValidChecksum(sentenceFromLog) != false){
					  //Create SentenceData object
					  SentenceData theData = extractSentenceData(sentenceFromLog);
						  //Attempt to create position object and push into route vector
						  try{
							  GPS::Position positionObject = positionFromSentenceData(theData);//If invalid format the function execution is dropped through invalid_argument
							  //Push the position object into the route vector
							  routePositions.push_back(positionObject);
						  }
						  catch(...){ //If exception from positionFromSentenceData received
							  //Catch exceptions to allow log to be missed and carry on processing other logs
						  }
				  }

			  }
		  }
	  }

      return routePositions; //return vector
  }

}