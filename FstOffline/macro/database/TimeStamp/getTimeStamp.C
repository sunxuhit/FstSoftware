#include <iostream>
#include <ctime>

// const std::string unixtime_to_utc_date( long ut, const std::string& format = "%Y%m%dT%H%M%S" ) {
//   struct tm res;
//   gmtime_r(&ut, &res);
//   char date[20];
//   strftime(date, sizeof(date), format.c_str(), &res);
//   return std::string( date );
// }

void getTimeStamp() 
{
  char paraDir[256];
  sprintf(paraDir, "./runTime_run22.txt");
  cout << paraDir << endl;

  long runNumber, runTime;
  FILE *file_timeInfo;
  file_timeInfo = fopen(paraDir, "r");
  if (file_timeInfo==0) {
    cout << "Cannot find file " << paraDir << endl;
    exit(0);
  }
  else {
    while(!feof(file_timeInfo)) {
      char buff[256];

      if(fgets(buff,sizeof(buff),file_timeInfo) == 0) continue ;
      int ret = sscanf(buff,"%d %d",&runNumber,&runTime);
      if(ret!=2) continue;
      cout << "runNumber = " << runNumber << ", runTime = " << runTime << endl;

      // get UTC/GMT time
      long unixtime = runTime;
      TTimeStamp ts( unixtime, true );
      std::cout << "date: " << ts.AsString("s") << "\n";
    }
    fclose(file_timeInfo);
  }

}
