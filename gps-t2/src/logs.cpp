#include <string>

#include "logs.h"

namespace GPS
{
  namespace LogFiles
  {
<<<<<<< HEAD
      const std::string logsDir = "logs/";
=======
      const std::string logsDir = "logs/"; //SHOULD NOT HARDCODE PATHS PEDRO!!!!
>>>>>>> 8b97e7a8d921f25605e33b73c1be4aaa9f2b9a25
      const std::string NMEALogsDir  = logsDir + "NMEA/";
      const std::string GPXRoutesDir = logsDir + "GPX/routes/";
      const std::string GPXTracksDir = logsDir + "GPX/tracks/";
  }
}
