/**
 * @file main.cpp
 * @author Shinichi Inoue (inoue.shinichi.1800@gmail.com)
 * @brief std::bindとlambda式の比較
 * @version 0.1
 * @date 2021-03-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */
// 参考 https://qiita.com/grainrigi/items/1aeeaf19d75d9827d037
#include <iostream>
#include <functional>
#include <chrono>

/*バインド対象のクラス*/
class TargetHoge
{
    int value_;
public:
    TargetHoge(int value) : value_(value) {}
    int get_value() const { return value_; }
    int add(int x, int y) { return x + y; }
};


int main(int, char**) 
{
    std::cout << "Hello, std::bind vs lambda!\n";

    // std::bindの使い方 -> メンバ関数をグローバル関数のように使用できる.
    {
        TargetHoge hoge(1);
        std::function<int()> get_value_of_hoge = std::bind(&TargetHoge::get_value, &hoge);

        std::cout << get_value_of_hoge() << std::endl;
    }
    
    // 仮引数つきのメンバ関数をバインドする by std::bind
    {
        TargetHoge hoge(1);
        std::function<int(int,int)> add_of_hoge = std::bind(&TargetHoge::add, 
                                                            &hoge,
                                                            std::placeholders::_1,
                                                            std::placeholders::_2);
        
        std::cout << add_of_hoge(1, 2) << std::endl;
    }

    // ラムダ式によるバインド
    {
        TargetHoge hoge(1);
        std::function<int()> get_value_of_hoge = [&]() -> int {
            return hoge.get_value();
        };

        std::cout << get_value_of_hoge() << std::endl;

        
        std::function<int(int,int)> add_of_hoge = [&](int x, int y) -> int {
            return hoge.add(x, y);
        };

        std::cout << add_of_hoge(5, 5) << std::endl;
    }

    // 速度比較
    {
        constexpr int LOOP_COUNT = 1000000;

        using std::chrono::nanoseconds;
        using std::chrono::duration_cast;
        using std::chrono::high_resolution_clock;

        TargetHoge hoge(1);
        std::function<int()> get_value_of_bind = std::bind(&TargetHoge::get_value, &hoge);
        std::function<int()> get_value_of_hoge = [&]() -> int {
            return hoge.get_value();
        };

        // std::bindの計測
        auto start = high_resolution_clock::now();
        for (size_t i = 0; i < LOOP_COUNT; ++i) {
            get_value_of_bind();
        }
        auto end = high_resolution_clock::now();

        size_t count = duration_cast<nanoseconds>(end - start).count();
        std::cout << "std::bind" << std::endl;
        std::cout << "total:" << count << "ns" << std::endl;
        std::cout << "mean:" << (double)count / LOOP_COUNT << "ns" << std::endl;

        // lambda式の計測
        start = high_resolution_clock::now();
        for (size_t i = 0; i < LOOP_COUNT; ++i) {
            get_value_of_hoge();
        }
        end = high_resolution_clock::now();

        count = duration_cast<nanoseconds>(end - start).count();
        std::cout << "labmda formula" << std::endl;
        std::cout << "total:" << count << "ns" << std::endl;
        std::cout << "mean:" << (double)count / LOOP_COUNT << "ns" << std::endl;
    }
}
