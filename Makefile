
obj_name = simple
obj = src/log.c test/simple.c
bin_dir = bin


all:
	cc $(obj) -o $(obj_name) -DLOG_USE_COLOR

simple:
	cc src/log.c test/simple.c -o $(bin_dir)/simple -DLOG_USE_COLOR