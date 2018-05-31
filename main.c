
#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <dbus-1.0/dbus/dbus.h>
#include <gio/gio.h>
#include "alarmClockStubs.h"
#include <time.h>
#include "timer.h"

/*end of includes*/

/*private data*/
static alarmClockAlarmClock *alclkAlarm; // new instance
char* bname="com.time.service.alarmClock";
typedef struct _ACLK_time_dataType{
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
}ACLK_time_dataType;
typedef struct _ACLK_RuntimeData_dataType{
    ACLK_time_dataType ACLK_time;
    unsigned char alarmStatus;

}ACLK_RuntimeData_dataType;

ACLK_RuntimeData_dataType ACLK_RuntimeData;

guint timerID=0;
GAsyncQueue *ACLK_AsyncQueue;
GThread *ACLK_backgroundWorker;
gboolean ACLK_alarmIsRinging=FALSE;

/*end of private data*/


/*functions definitions*/
gboolean
ACLK_doBackgroundWork(alarmClockAlarmClock *ifc){
    gboolean timeToRingTheAlarm=NULL;
    while(TRUE){
        timeToRingTheAlarm = g_async_queue_try_pop(ACLK_AsyncQueue);
        if(timeToRingTheAlarm == NULL){
            ;
        }
        else{
            printf("\ncallbackcalled\n");
            ACLK_alarmIsRinging = FALSE;
            timerID=0;
            ACLK_RuntimeData.alarmStatus = FALSE;
            alarm_clock_alarm_clock_emit_ring_alarm
                    (ifc,
                     TRUE);
        }
    }
    return FALSE;
}

gboolean
ACLK_RingAlarm(alarmClockAlarmClock *ifc){

    ACLK_alarmIsRinging = TRUE;
    g_async_queue_push(ACLK_AsyncQueue,&ACLK_alarmIsRinging);


    return FALSE;
}
gboolean
ACLK_setTime(alarmClockAlarmClock *ifc,GDBusMethodInvocation *inv,guchar hour,guchar minute, guchar second){
    TIM_tm newTime;
    newTime.tm_hour = hour;
    newTime.tm_min = minute;
    newTime.tm_sec = second;
    TIM_syncronizeTime(newTime);
    alarm_clock_alarm_clock_complete_set_time
            (ifc,
             inv);

    return TRUE;

}
gboolean
ACLK_setAlarmTime(alarmClockAlarmClock *ifc,GDBusMethodInvocation *inv,guchar hour,guchar minute){
    TIM_tm newAlarmTime;
    newAlarmTime.tm_hour = hour;
    newAlarmTime.tm_min = minute;
    newAlarmTime.tm_sec = 0;
    TIM_updateTimeToRingTheAlarm(newAlarmTime);
    if(ACLK_RuntimeData.alarmStatus)
    {
        if(timerID){
            g_source_remove(timerID); //cancel the running timer
            timerID=0;
        }else ; //do nothing
        timerID = TIM_startTimer(ACLK_RingAlarm,ifc);

    }else ; // do nothing
    alarm_clock_alarm_clock_complete_set_alarm_time
            (ifc,
             inv);
    return TRUE;
}
gboolean
ACLK_getAlarmStatus(alarmClockAlarmClock *ifc,GDBusMethodInvocation *inv){
    alarm_clock_alarm_clock_complete_get_alarm_status
            (ifc,
             inv,
             ACLK_RuntimeData.alarmStatus);
    return TRUE;
}
gboolean
ACLK_setAlarmStatus(alarmClockAlarmClock *ifc,GDBusMethodInvocation *inv,guchar status){
    ACLK_RuntimeData.alarmStatus = status;
    if(status == 0){
        if(timerID != 0){GAsyncQueue *ACLK_AsyncQueue;
            g_source_remove(timerID); //cancel the running timer
            timerID=0;
        }else; //do nothing
    }else{
        if(timerID){
            g_source_remove(timerID); //cancel the running timer
            timerID=0;
        }else ; //do nothing
        timerID = TIM_startTimer(ACLK_RingAlarm,ifc);
    }
    alarm_clock_alarm_clock_complete_set_alarm_status
            (ifc,
             inv);
    return TRUE;
}
gboolean
ACLK_checkDebugVariables(alarmClockAlarmClock *ifc,GDBusMethodInvocation *inv){
    TIM_tm localTime;
    TIM_calculateAlarmClockLocalTime(&localTime);
    alarm_clock_alarm_clock_complete_check_debug_variables
            (ifc,
             inv,
             timerID,
             getTimeUntilAlarmRingS(),
             getTimeUntilAlarmRingS());
   /* alarm_clock_alarm_clock_complete_check_debug_variables
            (ifc,
             inv,
             TIM_getDelayHour(),
             TIM_getDelayMin(),
             TIM_getDelaySec());*/
    return TRUE;
}
void
ACLK_linkMethodCallbacks(alarmClockAlarmClock *ifc){
 g_signal_connect(ifc,   "handle-set-time",      G_CALLBACK(ACLK_setTime),  NULL);
 g_signal_connect(ifc,   "handle-set-alarm-time",      G_CALLBACK(ACLK_setAlarmTime),  NULL);
 g_signal_connect(ifc,   "handle-get-alarm-status",      G_CALLBACK(ACLK_getAlarmStatus),  NULL);
 g_signal_connect(ifc,   "handle-set-alarm-status",      G_CALLBACK(ACLK_setAlarmStatus),  NULL);
 g_signal_connect(ifc,   "handle-check-debug-variables",      G_CALLBACK(ACLK_checkDebugVariables),  NULL);

}

static void
on_bus_acquired (GDBusConnection *connection,
                 const gchar     *name,
                 gpointer         user_data)
{
  /* This is where we'd export some objects on the bus */




    alarmClockObjectSkeleton *object; // from stub
    GDBusObjectManagerServer *manager = NULL;


    /* create a new server */
    manager = g_dbus_object_manager_server_new("/com/time/alarmClock/managerDir");
    object = alarm_clock_object_skeleton_new("/com/time/alarmClock/managerDir/Object");

    alclkAlarm = alarm_clock_alarm_clock_skeleton_new();

    alarm_clock_object_skeleton_set_alarm_clock(object, alclkAlarm);

    g_object_unref(alclkAlarm);

    ACLK_linkMethodCallbacks(alclkAlarm);


    g_dbus_object_manager_server_export(manager, G_DBUS_OBJECT_SKELETON(object));
    g_object_unref(object);


    g_dbus_object_manager_server_set_connection(manager, connection);
    TIM_initTimerModule();
    ACLK_AsyncQueue = g_async_queue_new();
    ACLK_backgroundWorker = g_thread_new("backgroundworker",ACLK_doBackgroundWork,alclkAlarm);




    g_print("Bus successfully acquired: %s\n", bname);
}

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
  g_print ("Acquired the name %s on the system bus\n", name);
}

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{

  g_print ("Lost the name %s on the system bus\n", name);
}




/*end of function definitions*/






int main(int argc, char *argv[]){


      guint owner_id;
      GMainLoop *loop;

      owner_id = g_bus_own_name (G_BUS_TYPE_SYSTEM,
                                 bname,
                                 G_BUS_NAME_OWNER_FLAGS_REPLACE,
                                 on_bus_acquired,
                                 on_name_acquired,
                                 on_name_lost,
                                 NULL,
                                 NULL);

      loop = g_main_loop_new (NULL, FALSE);
      g_main_loop_run (loop);

      g_bus_unown_name (owner_id);


     return 0;
}



