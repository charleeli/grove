#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <stdlib.h>

//配置输出日志的目录：
#define LOGDIR "log"
#define MKDIR "mkdir -p "LOGDIR

//将信息输出到单独的文件和 LOG(ERROR)
void SignalHandle(const char* data, int size)
{
    std::string str = std::string(data,size);
    LOG(ERROR)<<str;
}

class GLogHelper
{
public:
    //GLOG配置：
    GLogHelper(char* program);
    //GLOG内存清理：
    ~GLogHelper();
};

//GLOG配置：
GLogHelper::GLogHelper(char* program)
{
    system(MKDIR);
    google::InitGoogleLogging(program);
   
    //设置级别高于 google::FATAL 的日志同时输出到屏幕
    google::SetStderrLogging(google::FATAL); 
    //设置输出到屏幕的日志显示相应颜色
    FLAGS_colorlogtostderr=true;
    //设置 google::INFO 级别的日志存储路径和文件名前缀  
    google::SetLogDestination(google::INFO,LOGDIR"/INFO_"); 
    //设置 google::WARNING 级别的日志存储路径和文件名前缀
    google::SetLogDestination(google::WARNING,LOGDIR"/WARN_"); 
    //设置 google::ERROR 级别的日志存储路径和文件名前缀  
    google::SetLogDestination(google::ERROR,LOGDIR"/ERROR_"); 
    //设置 google::FATAL 级别的日志存储路径和文件名前缀 
    google::SetLogDestination(google::FATAL, LOGDIR"/FATAL_"); 
    //缓冲日志输出，默认为30秒，此处改为立即输出 
    FLAGS_logbufsecs =0; 
    //最大日志大小为 100MB       
    FLAGS_max_log_size =100;
    //当磁盘被写满时，停止日志输出
    FLAGS_stop_logging_if_full_disk = true; 
    //捕捉 core dumped    
    google::InstallFailureSignalHandler();
    //默认捕捉 SIGSEGV 信号信息输出会输出到 stderr      
    google::InstallFailureWriter(&SignalHandle);    
}

//GLOG内存清理：
GLogHelper::~GLogHelper()
{
    google::ShutdownGoogleLogging();
}

/*
int main(int argc,char* argv[])
{
    //要使用 GLOG ，只需要在 main 函数开始处添加这句即可
    //g++ GLogHelper.cpp -lglog
    GLogHelper gh(argv[0]);

    LOG(INFO)<<"[INFO ]";
    LOG(ERROR)<<"[ERROR]";
    return 0;
}
*/

