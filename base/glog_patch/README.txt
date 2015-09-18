修改glog-0.3.3源码，按小时滚动
    
    1.utilities.h中添加定义
        bool DayHasChanged()；
    
    2.utilities.cc中添加定义
        static int32 g_main_day = 0;
        bool DayHasChanged()
        {
            time_t raw_time;
            struct tm* tm_info;

            time(&raw_time);
            tm_info = localtime(&raw_time);

            if (tm_info->tm_mday != g_main_day)
            {
                g_main_day = tm_info->tm_mday;
                return true;
            }

            return false;
        }

    3.修改logging.cc中的使用
        
    a.  函数LogFileObject::Write中
        if (static_cast<int>(file_length_ >> 20) >= MaxLogSize() ||
            PidHasChanged() ) {
            
        修改为
        if (static_cast<int>(file_length_ >> 20) >= MaxLogSize() ||
            DayHasChanged() ) {
        
    b.  << '-'
        << setw(2) << tm_time.tm_hour
        << setw(2) << tm_time.tm_min
        << setw(2) << tm_time.tm_sec
        << '.'
        << GetMainThreadPid();
        << ".log";
        
        修改为
        //<< '-'
        << setw(2) << tm_time.tm_hour
        //<< setw(2) << tm_time.tm_min
        //<< setw(2) << tm_time.tm_sec
        //<< '.'
        //<< GetMainThreadPid();
        << ".log";
        
        
    c.  函数LogFileObject::CreateLogfile中
        int fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0664);
        修改为
        int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0664);
        
        
