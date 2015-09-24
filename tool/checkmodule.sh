#!/bin/sh
export LD_LIBRARY_PATH="/usr/local/lib64":$LD_LIBRARY_PATH
export LD_LIBRARY_PATH="/usr/local/lib":$LD_LIBRARY_PATH
logPath=/usr/local/grove/tool
listModule="EchoModule:19001:2 AdminModule:19002:2 "
listPort="6379 6380 6381"
check_cgi()
{
	if test $( pgrep -f $1 | wc -l ) -lt $3 
	then
		echo `date "+%G-%m-%d %H:%M:%S"` "restart $1...."  >> $logPath/check.log
		killall $1
		spawn-fcgi -a 127.0.0.1 -p $2 -C 25 -f /usr/local/grove/fast-cgi/$1 -F $3 >> $logPath/check.log
	fi 
    return
}

check_redis()
{
	if test $( ps aux | grep redis-server | grep $1 | wc -l ) -eq 0 
	then 
		echo `date "+%G-%m-%d %H:%M:%S"` "restart redis($1)...."  >> $logPath/check.log
		redis-server /usr/local/grove/conf/redis_$1.conf >> $logPath/check.log
	fi 
    return
}

check()
{
	if test $( pgrep -f $1 | wc -l ) -eq 0 
	then 
		echo `date "+%G-%m-%d %H:%M:%S"`  "restart $1...."   >> $logPath/check.log
		service $1 restart >> $logPath/check.log
	fi 
    return
}
####-------------------program start here------------------------####
check "mysqld"
check "nginx"

for i in $listPort; do
    check_redis $i
done

for j in $listModule; do
	num=$(echo $j | awk -F":" '{print NF}')
    modName=$(echo $j | awk -F":" '{print $1}')
	modPort=$(echo $j | awk -F":" '{print $2}')
	modNum=$(echo $j | awk -F":" '{print $3}')
    if [ $num -eq 3 ]
    then
        check_cgi $modName $modPort $modNum
    fi
done
exit 0

