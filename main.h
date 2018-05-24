#ifndef __MAIN_H__
#define __MAIN_H__

/*functions prototypes*/

gboolean ACLK_setTime(alarmClockAlarmClock *ifc,GDBusMethodInvocation *inv,guchar hour,guchar minute, guchar second);
gboolean ACLK_setAlarmtime(alarmClockAlarmClock *ifc,GDBusMethodInvocation *inv,guchar hour,guchar minute, guchar second);
gboolean ACLK_getAlarmStatus(void);
gboolean ACLK_setAlarmStatus(alarmClockAlarmClock *ifc,GDBusMethodInvocation *inv,guchar status);
gboolean ACLK_RingAlarm(alarmClockAlarmClock *ifc);

/*end of function prototypes*/

#endif /* __MAIN_H__ */
