/**
 * @file main.cpp
 * @author QiuYeDx (qiuyedx@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-21
 * 
 * @copyright Copyright (c) 2023
 * 
 * Q: 在长度为n的正文串中，查找某一字符是否出现，若出现输出1，否则输出0，设计时间复杂度为o(n)的算法，并求解这个问题。
 * A: 可以使用线性扫描算法在O(n)时间内解决该问题。具体步骤如下：
 *    从正文串的开头开始，依次扫描每个字符，直到扫描到正文串的结尾。
 *    在扫描过程中，如果找到了目标字符，则立即输出1并结束扫描。
 *    如果扫描完整个正文串仍未找到目标字符，则输出0。
 */

#include <iostream>
#define LEN 100
using namespace std;

int findChar(char*, char, int);

int main(){
    char str[LEN] = "Hello, world";
    int ans = findChar(str, 'w', LEN);
    cout << ans << endl;
    return 0;
}

int findChar(char* text, char target, int n) {
    for (int i = 0; i < n; i++) {
        if (text[i] == target) {
            return 1;
        }
    }
    return 0;
}
