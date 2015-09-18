#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

using namespace rapidjson;
using namespace std;

int main() {
    const char* filename_ = "./test.json";
    char *json_;
    size_t length_;
    Document d;
    
    FILE* fp = NULL;
    fp = fopen(filename_, "rb");
    if (fp) {
        filename_ = filename_;
    }
     
    fseek(fp, 0, SEEK_END);
    length_ = (size_t)ftell(fp);
    fseek(fp, 0, SEEK_SET);
    json_ = (char*)malloc(length_ + 1);
    size_t readLength = fread(json_, 1, length_, fp);
    json_[readLength] = '\0';
    fclose(fp);

    d.Parse(json_);
    
    printf("****%d****\n",d["RedisAdmin"]["port"].GetInt());
    
    free(json_);
    json_ = NULL;

    return 0;
}

