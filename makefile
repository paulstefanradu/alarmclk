#Compiler choose for the process:
CC=gcc
#Compilation flags:
#pkg-config --libs --cflags dbus-1 gobject-2.0 glib-2.0 gio-2.0 gthread-2.0 gio-unix-2.0
CFLAGS=-c -Wall

INCLUDES=-I/usr/include/glib-2.0 \
-I/usr/include/dbus-1.0 \
-I/usr/include/gio-unix-2.0 \
-I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
-I/usr/lib/x86_64-linux-gnu/dbus-1.0/include \
-I/usr/lib/x86_64-linux-gnu

LIBS=`pkg-config --libs --cflags dbus-1 glib-2.0 gio-2.0 gthread-2.0 gio-unix-2.0 gobject-2.0`

all:$1
build: main.o alarmClockStubs.o timer.o
	$(CC) main.o alarmClockStubs.o timer.o -o startAlarmClock $(LIBS)
main.o: main.c alarmClockStubs.c
	$(CC) $(CFLAGS) main.c -o $@ $(INCLUDES)
alarmClockStubs.o: alarmClockStubs.c alarmClockStubs.h
	$(CC) $(CFLAGS) alarmClockStubs.c -o $@ $(INCLUDES)
timer.o: timer.c timer.h
	$(CC) $(CFLAGS) timer.c -o $@ $(INCLUDES)
alarmClockStubs.c:
	gdbus-codegen --generate-c-code alarmClockStubs --c-namespace alarmClock --interface-prefix com.time.service. --c-generate-object-manager time.service.alarmClock.xml
clean:
	rm *o startAlarmClock alarmClockStubs.c alarmClockStubs.h
