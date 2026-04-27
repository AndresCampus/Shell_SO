CC = gcc
CFLAGS = -Wall -g
TARGETS = a.out Shell redirections example_list

all: $(TARGETS)

a.out: Shell_project.c job_control.c job_control.h
	$(CC) $(CFLAGS) Shell_project.c job_control.c -o a.out

Shell: Shell_project.c job_control.c job_control.h
	$(CC) $(CFLAGS) Shell_project.c job_control.c -o Shell

redirections: redirections.c
	$(CC) $(CFLAGS) redirections.c -o redirections

example_list: example_list.c job_control.c job_control.h
	$(CC) $(CFLAGS) example_list.c job_control.c -o example_list

clean:
	rm -f $(TARGETS) *.o
