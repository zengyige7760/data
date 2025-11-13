#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <regex>
#include<windows.h>
using namespace std;

enum class Mode { ADB, ZJ };

// 辅助函数：确保数字转换为两位字符串
string toTwoDigits(int num) {
    if (num < 10) {
        return "0" + to_string(num);
    }
    return to_string(num);
}

// 实现v1算法
string v1(const string& code, Mode mode) {
    try {
        if (mode == Mode::ADB) {
            if (code.length() != 10) return "";
            
            int i1 = stoi(code.substr(0, 2));
            int i2 = stoi(code.substr(2, 2));
            int i3 = stoi(code.substr(4, 2));
            int i4 = stoi(code.substr(6, 2));
            int i5 = stoi(code.substr(8, 2));

            int i6 = i5 ^ (i3 + i4);
            int i7 = i4 ^ i6;
            int i8 = i3 ^ i6;
            int i9 = i1 ^ i6;
            int i10 = i2 ^ i6;

            string i;
            i += toTwoDigits(i9);
            i += toTwoDigits(i10);
            i += toTwoDigits(i8);
            i += toTwoDigits(i7);
            i += toTwoDigits(i6);

            int i1_2 = stoi(i.substr(0, 2));
            int i2_2 = stoi(i.substr(2, 2));
            int i3_2 = stoi(i.substr(4, 2));
            int i4_2 = stoi(i.substr(6, 2));
            int i5_2 = stoi(i.substr(8, 2));

            int i6_2 = i4_2 ^ i3_2;
            int i7_2 = i5_2 ^ i3_2;
            int i8_2 = i3_2 ^ (i6_2 + i7_2);
            int i9_2 = i1_2 ^ i7_2;
            int i10_2 = i2_2 ^ i7_2;

            string i2_result;
            i2_result += toTwoDigits(i9_2);
            i2_result += toTwoDigits(i10_2);
            i2_result += toTwoDigits(i6_2);
            i2_result += toTwoDigits(i7_2);
            i2_result += toTwoDigits(i8_2);

            return i2_result;
        } else if (mode == Mode::ZJ) {
            if (code.length() != 6) return "";
            
            int i1 = stoi(code.substr(0, 2));
            int i2 = stoi(code.substr(2, 2));
            int i3 = stoi(code.substr(4, 2));

            int i5 = i3 ^ (i1 + i2);
            int i6 = i1 ^ i5;
            int i4 = i2 ^ i5;

            string i;
            i += toTwoDigits(i6);
            i += toTwoDigits(i4);
            i += toTwoDigits(i5);

            int i1_2 = stoi(i.substr(0, 2));
            int i2_2 = stoi(i.substr(2, 2));
            int i3_2 = stoi(i.substr(4, 2));

            int i5_2 = i2_2 ^ i1_2;
            int i6_2 = i3_2 ^ i1_2;
            int i4_2 = i1_2 ^ (i5_2 + i6_2);

            string i2_result;
            i2_result += toTwoDigits(i5_2);
            i2_result += toTwoDigits(i6_2);
            i2_result += toTwoDigits(i4_2);

            return i2_result;
        }
    } catch (...) {
        return "";
    }
    return "";
}

// 实现v2算法
string v2(const string& code, Mode mode) {
    try {
        int num = (mode == Mode::ADB) ? 2 : 1;
        
        // 检查是否为纯数字
        regex digits_regex("^\\d+$");
        if (!regex_match(code, digits_regex)) {
            return "";
        }

        if (code.length() != 8) return "";
        
        int key = (code[7] - '0') ^ num;
        int v7 = ( (code[key] - '0') - key + 10 ) % 10;
        
        string result1;
        for (int i = 0; i < 7; i++) {
            int curKey = v7;
            if (i == key) {
                result1 += to_string(v7);
            } else {
                int curInt = code[i] - '0';
                result1 += to_string( (curInt + 10 - curKey) % 10 );
            }
        }

        // 初始化随机数生成器
        srand(time(nullptr));
        int keyId = rand() % 7;
        int keyValue = result1[keyId] - '0';
        
        string result;
        for (int i = 0; i < 7; i++) {
            int curKey = (i == keyId) ? keyId : keyValue;
            int curInt = result1[i] - '0';
            result += to_string( (curInt + curKey) % 10 );
        }
        
        result += to_string( num ^ keyId );
        return result;
    } catch (...) {
        return "";
    }
}

// 主函数：根据输入长度选择合适的算法
string getCode(const string& code, Mode mode) {
    if (code.length() == 8) {
        string result;
        do {
            result = v2(code, mode);
        } while (result.empty() || result == code);
        return result;
    } else {
        return v1(code, mode);
    }
}

// 程序入口
int main() 
{
    SetConsoleOutputCP(65001);
    string code;
    string modeStr;
    Mode mode;
    
    cout << "XTC校验码计算器" << endl;
    cout << "请输入校验码: ";
    cin >> code;
    
    cout << "请选择模式 (adb/zj): ";
    cin >> modeStr;
    
    if (modeStr == "adb") {
        mode = Mode::ADB;
    } else if (modeStr == "zj") {
        mode = Mode::ZJ;
    } else {
        cout << "无效的模式" << endl;
        return 1;
    }
    
    // 验证输入是否为纯数字
    regex digits_regex("^\\d+$");
    if (!regex_match(code, digits_regex)) {
        cout << "错误: 校验码必须为数字" << endl;
        return 1;
    }
    
    string result = getCode(code, mode);
    
    if (result.empty()) {
        cout << "计算失败，请检查输入格式" << endl;
    } else {
        cout << "计算结果: " << result << endl;
    }
    
    return 0;
}