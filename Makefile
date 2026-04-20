CC = gcc
CFLAGS = -Wall -g
TARGETS = Shell redirections

all: $(TARGETS)

Shell: Shell_project.c job_control.c job_control.h
	$(CC) $(CFLAGS) Shell_project.c job_control.c -o Shell

redirections: redirections.c
	$(CC) $(CFLAGS) redirections.c -o redirections

clean:
	rm -f $(TARGETS) *.o
