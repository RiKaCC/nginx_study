
default:	build

clean:
	rm -rf Makefile objs

build:
	$(MAKE) -f objs/Makefile

install:
	$(MAKE) -f objs/Makefile install

modules:
	$(MAKE) -f objs/Makefile modules

upgrade:
	/home/chenruijia/work/src/study/ruijia/nginx_study/process/sbin/nginx -t

	kill -USR2 `cat /home/chenruijia/work/src/study/ruijia/nginx_study/process/logs/nginx.pid`
	sleep 1
	test -f /home/chenruijia/work/src/study/ruijia/nginx_study/process/logs/nginx.pid.oldbin

	kill -QUIT `cat /home/chenruijia/work/src/study/ruijia/nginx_study/process/logs/nginx.pid.oldbin`
