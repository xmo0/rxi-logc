
# obj_name = simple
# obj = src/log.c test/simple.c
bin_dir = build

all:
	cc src/log.c test/simple.c -o $(bin_dir)/simple -DLOG_USE_COLOR
	cc src/log.c test/pthread.c -o $(bin_dir)/pthread -DLOG_USE_COLOR -lpthread

simple:
	cc src/log.c test/simple.c -o $(bin_dir)/simple -DLOG_USE_COLOR

pthread:
	cc src/log.c test/pthread.c -o $(bin_dir)/pthread -DLOG_USE_COLOR -lpthread

filelogs:
	cc src/log.c test/filelogs.c -o $(bin_dir)/filelogs -DLOG_USE_COLOR

