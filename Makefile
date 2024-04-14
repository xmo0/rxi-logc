
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

add_filp:
	cc src/log.c test/add_filp.c -o $(bin_dir)/add_filp -DLOG_USE_COLOR

add_file:
	cc src/log.c test/add_file.c -o $(bin_dir)/add_file -DLOG_USE_COLOR

rolling-file-logs:
	cc src/log.c test/rolling-file-logs.c -o $(bin_dir)/rolling-file-logs -DLOG_USE_COLOR

