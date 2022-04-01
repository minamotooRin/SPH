#include <chrono>
 
class Timer {
public:
 
    Timer():_name("Time elapsed:") {
        restart();
    }
 
    explicit Timer(const std::string &name):_name(name + ":") {
        restart();
    }
 
    /**
    * 启动计时
    */
    inline void restart() {
        _start_time = std::chrono::steady_clock::now();
    }
 
    /**
    * 结束计时
    * @return 返回ms数
    */
    inline double elapsed(bool restart = false) {
        _end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = _end_time-_start_time;
        if(restart)
            this->restart();
        return diff.count()*1000;
    }
 
private:
    std::chrono::steady_clock::time_point _start_time;
    std::chrono::steady_clock::time_point _end_time;
    std::string _name;
}; 