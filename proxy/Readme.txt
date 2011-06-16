设备端代理使用说明

补全代码：
    由于运行系统的配置和功能的多样性，当需要在特定系统下
	使用本代理时，需要补充system.c中的函数的功能。
	如果要保存日志，修改下面这行的定义，将printf修为自己的函数名
	#define savelog printf

编译：
	linux下直接使用make，

	交叉编译需要自行修改Makefile中的编译器路径


使用：
   pproxy -d -g level -h ip

   -d        以后台进程运行
   -g level  指定调试级别
   -h ip     指定代理的目标设备，默认为localhost

