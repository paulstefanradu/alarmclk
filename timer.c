#include <dbus/dbus-glib.h>
#include <dbus-1.0/dbus/dbus.h>
#include <gio/gio.h>
#include "alarmClockStubs.h"
#include <time.h>
#include "timer.h"


/*private data*/
gint64 deltaTimeS;
guint timeUntilAlarmRingS=0;
TIM_tm timeToRingTheAlarm;


/*end of private data*/
guint getTimeUntilAlarmRingS(){
    return timeUntilAlarmRingS;
}
gboolean TIM_initTimerModule(){
    timeToRingTheAlarm.tm_hour=0;
    timeToRingTheAlarm.tm_min=0;
    timeToRingTheAlarm.tm_sec=0;
    return TRUE;
}
gboolean TIM_updateTimeToRingTheAlarm(TIM_tm newTime){
    timeToRingTheAlarm.tm_hour=newTime.tm_hour;
    timeToRingTheAlarm.tm_min=newTime.tm_min;
    timeToRingTheAlarm.tm_sec=newTime.tm_sec;
    return TRUE;
}
guint TIM_startTimer(GSourceFunc callbackFunction,alarmClockAlarmClock *ifc){
    guint ret=FALSE;
    TIM_tm alarmClockLocalTime;
   // guint timeUntilAlarmRingS=0;
    guint timeUntilAlarmRingMS=0;
    guint hoursUntilAlarmRing=0;
    guint minutesUntilAlarmRing=0;
    guint secondsUntilAlarmRing=0;

    TIM_calculateAlarmClockLocalTime(&alarmClockLocalTime);
    if(timeToRingTheAlarm.tm_hour > alarmClockLocalTime.tm_hour){
        if(timeToRingTheAlarm.tm_min > alarmClockLocalTime.tm_min){
            hoursUntilAlarmRing = (timeToRingTheAlarm.tm_hour - alarmClockLocalTime.tm_hour);
            minutesUntilAlarmRing = (timeToRingTheAlarm.tm_min - alarmClockLocalTime.tm_min);
            secondsUntilAlarmRing = (-alarmClockLocalTime.tm_sec);
            timeUntilAlarmRingS = 3600*hoursUntilAlarmRing + 60*minutesUntilAlarmRing + secondsUntilAlarmRing;
        }
        else if(timeToRingTheAlarm.tm_min < alarmClockLocalTime.tm_min){
            hoursUntilAlarmRing = (timeToRingTheAlarm.tm_hour - alarmClockLocalTime.tm_hour);
            minutesUntilAlarmRing = (timeToRingTheAlarm.tm_min - alarmClockLocalTime.tm_min);
            secondsUntilAlarmRing = -(alarmClockLocalTime.tm_sec);
            timeUntilAlarmRingS = 3600*hoursUntilAlarmRing + 60*minutesUntilAlarmRing + secondsUntilAlarmRing;
        }
        else if(timeToRingTheAlarm.tm_min == alarmClockLocalTime.tm_min){
            hoursUntilAlarmRing = (timeToRingTheAlarm.tm_hour - alarmClockLocalTime.tm_hour);
            minutesUntilAlarmRing = (0);
            secondsUntilAlarmRing = -(alarmClockLocalTime.tm_sec);
            timeUntilAlarmRingS = 3600*hoursUntilAlarmRing + 60*minutesUntilAlarmRing + secondsUntilAlarmRing;
        }
        else ;//do nothing here
    }
    else if(timeToRingTheAlarm.tm_hour < alarmClockLocalTime.tm_hour){
        if(timeToRingTheAlarm.tm_min > alarmClockLocalTime.tm_min){
            hoursUntilAlarmRing = (timeToRingTheAlarm.tm_hour + (24 - alarmClockLocalTime.tm_hour));
            minutesUntilAlarmRing = (timeToRingTheAlarm.tm_min - alarmClockLocalTime.tm_min);
            secondsUntilAlarmRing = -(alarmClockLocalTime.tm_sec);
            timeUntilAlarmRingS = 3600*hoursUntilAlarmRing + 60*minutesUntilAlarmRing + secondsUntilAlarmRing;
        }
        else if(timeToRingTheAlarm.tm_min < alarmClockLocalTime.tm_min){
            hoursUntilAlarmRing = (timeToRingTheAlarm.tm_hour + (24 - alarmClockLocalTime.tm_hour));
            minutesUntilAlarmRing = (timeToRingTheAlarm.tm_min - alarmClockLocalTime.tm_min);
            secondsUntilAlarmRing = -(alarmClockLocalTime.tm_sec);
            timeUntilAlarmRingS = 3600*hoursUntilAlarmRing + 60*minutesUntilAlarmRing + secondsUntilAlarmRing;
        }
        else if(timeToRingTheAlarm.tm_min == alarmClockLocalTime.tm_min){
            hoursUntilAlarmRing = (timeToRingTheAlarm.tm_hour + (24 - alarmClockLocalTime.tm_hour));
            minutesUntilAlarmRing = (0);
            secondsUntilAlarmRing = (-alarmClockLocalTime.tm_sec);
            timeUntilAlarmRingS = 3600*hoursUntilAlarmRing + 60*minutesUntilAlarmRing + secondsUntilAlarmRing;
        }
        else ;//do nothing here
    }
    else if(timeToRingTheAlarm.tm_hour == alarmClockLocalTime.tm_hour){
        if(timeToRingTheAlarm.tm_min > alarmClockLocalTime.tm_min){
            hoursUntilAlarmRing = (0);
            minutesUntilAlarmRing = (timeToRingTheAlarm.tm_min - alarmClockLocalTime.tm_min);
            secondsUntilAlarmRing = (-alarmClockLocalTime.tm_sec);
            timeUntilAlarmRingS = 3600*hoursUntilAlarmRing + 60*minutesUntilAlarmRing + secondsUntilAlarmRing;
        }
        else if(timeToRingTheAlarm.tm_min < alarmClockLocalTime.tm_min){
            hoursUntilAlarmRing = (24);
            minutesUntilAlarmRing = (timeToRingTheAlarm.tm_min - alarmClockLocalTime.tm_min);
            secondsUntilAlarmRing = (-alarmClockLocalTime.tm_sec);
            timeUntilAlarmRingS = 3600*hoursUntilAlarmRing + 60*minutesUntilAlarmRing + secondsUntilAlarmRing;
        }
        else if(timeToRingTheAlarm.tm_min == alarmClockLocalTime.tm_min){
            hoursUntilAlarmRing = (0);
            minutesUntilAlarmRing = (0);
            secondsUntilAlarmRing = (0);
            timeUntilAlarmRingS = 3600*hoursUntilAlarmRing + 60*minutesUntilAlarmRing + secondsUntilAlarmRing;
        }
        else ;//do nothing here
    }
    else; //do nothing here
    timeUntilAlarmRingMS = 1000*timeUntilAlarmRingS;
    ret = g_timeout_add(timeUntilAlarmRingMS,callbackFunction,ifc);
    return ret;

    }

gboolean TIM_syncronizeTime(TIM_tm newTime){
    time_t rawtime;
    time(&rawtime); //get the system raw time in seconds
    TIM_tm *actualTime;
    actualTime=localtime(&rawtime); //convert rawtime to date/hour/minute/second format
    if(((newTime.tm_hour >= 0) && (newTime.tm_hour <=23)) &&
       ((newTime.tm_min >= 0) && (newTime.tm_min <=59))   &&
       ((newTime.tm_sec >= 0) && (newTime.tm_sec <=59)))
    {
        deltaTimeS = (newTime.tm_hour - actualTime->tm_hour)*3600 +
                     (newTime.tm_min - actualTime->tm_min)*60 +
                     (newTime.tm_sec - actualTime->tm_sec);
    }
    else{
        return FALSE;
    }
    return TRUE;
}


gboolean TIM_calculateAlarmClockLocalTime(TIM_tm *ret_data){
    time_t alarmtime;
    time_t rawtime;
    time(&rawtime); //get the system raw time in seconds
    alarmtime = rawtime + deltaTimeS;
    TIM_tm *actualTime;
    actualTime=localtime(&alarmtime); //convert rawtime to date/hour/minute/second format
    ret_data->tm_hour = actualTime->tm_hour;
    ret_data->tm_min = actualTime->tm_min;
    ret_data->tm_sec = actualTime->tm_sec;

    return TRUE;


}

