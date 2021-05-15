#include "timeUtil.h"

using namespace std;



void time_parser()
{
  char time_to_be_parsed[24];

  for (uint8_t l = 0; l < 25; l++)
  {
    time_to_be_parsed[l] = WiFi.my_tok[l];
  }
  char *my_month;
  int my_day;
  int my_hour;
  int my_min;
  int my_sec;
  int my_year;
  char *clocky;
  int _month;
  char *token = strtok(time_to_be_parsed, " ");
  int i = 0;

  while (token != NULL)
  {
    switch (i)
    {
    case 0:

      break;
    case 1:
      my_month = token;
      break;
    case 2:
      my_day = stoi(token);
      break;
    case 3:
      clocky = token;

      break;
    case 4:
      my_year = stoi(token);
      break;
    }

    token = strtok(NULL, " ");
    i++;
  }

  my_hour = stoi(strtok(clocky, ":"));
  my_min = stoi(strtok(NULL, ":"));
  my_sec = stoi(strtok(NULL, ":"));

  switch (hash_string(my_month))
  {
  case HASH_S16("Jan"):
    _month = 1;
    break;
  case HASH_S16("Feb"):
    _month = 2;
    break;
  case HASH_S16("Mar"):
    _month = 3;
    break;
  case HASH_S16("Apr"):
    _month = 4;
    break;
  case HASH_S16("May"):
    _month = 5;
    break;
  case HASH_S16("Jun"):
    _month = 6;
    break;
  case HASH_S16("Jul"):
    _month = 7;
    break;
  case HASH_S16("Aug"):
    _month = 8;
    break;
  case HASH_S16("Sep"):
    _month = 9;
    break;
  case HASH_S16("Oct"):
    _month = 10;
    break;

  case HASH_S16("Nov"):
    _month = 11;
    break;
  case HASH_S16("Dec"):
    _month = 12;
    break;
  default:
    break;
  }

  Serial.println(my_month);
  Serial.println(my_day);
  Serial.println(my_hour);
  Serial.println(my_sec);
  Serial.println(my_min);
  Serial.println(my_year);
  Serial.println(_month);

  struct tm setTime;

  setTime.tm_mday = my_day;           // day of month (0 - 31)
  setTime.tm_mon = _month - 1;         // month are from (0 - 11)
  setTime.tm_year = my_year - 1900;  // years since 1900
  setTime.tm_hour = my_hour;            // hour (0 - 23)
  setTime.tm_min = my_min;            // minutes (0 - 59)
  setTime.tm_sec = my_sec;             // seconds (0 -   59)


  set_time( mktime( &setTime ) );

  /*setTime   ->  Initiaties real time clock with the fetched time data via internet, and keeps its internal time without any more need for syncronisation*/
  //set_time(my_hour, my_min, my_sec, my_day, _month, my_year);
}

unsigned int hash_string(char *String)
{
  unsigned int hash = 0, i;
  assert_static(sizeof(unsigned int) == 4);
  assert_static(sizeof(unsigned char) == 1);

  for (i = 0; String[i]; ++i)
    hash = 65599 * hash + String[i];
  return hash ^ (hash >> 16);
}
void get_my_time() {
  
  time_t seconds = time(NULL);
      
 
        char buffer[32];
        strftime(buffer, 32, "%Y-%M-%eT%H:%M.%S.000Z\n", localtime(&seconds));
        Serial.println( buffer);
        //2009-02-22T23:33:02.971Z
        
      
  }
