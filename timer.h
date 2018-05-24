#ifndef __TIMER_H__
#define __TIMER_H__

typedef struct tm TIM_tm;
guint TIM_startTimer(GSourceFunc callbackFunction,alarmClockAlarmClock *ifc);
gboolean TIM_calculateAlarmClockLocalTime(TIM_tm *ret_data);
guint getTimeUntilAlarmRingS();
gboolean TIM_updateTimeToRingTheAlarm(TIM_tm newTime);
gboolean TIM_initTimerModule();








#endif /* __TIMER_H__ */
