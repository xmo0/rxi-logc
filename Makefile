
# obj_name = simple
# obj = src/log.c test/simple.c
bin_dir = bin

all:
	cc src/log.c test/simple.c -o $(bin_dir)/simple -DLOG_USE_COLOR
	cc src/log.c test/pthread.c -o $(bin_dir)/pthread -DLOG_USE_COLOR -lpthread

simple:
	cc src/log.c test/simple.c -o $(bin_dir)/simple -DLOG_USE_COLOR

pthread:
	cc src/log.c test/pthread.c -o $(bin_dir)/pthread -DLOG_USE_COLOR -lpthread
