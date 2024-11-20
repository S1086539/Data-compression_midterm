#pragma once  // 防止重複包含此標頭檔案
#include <iostream>
#include <string>
#include "HaffmanTree.h" 
using namespace std;

// 定義一個結構，用於存儲字元的相關信息
struct ByteInfo {
    unsigned char ch;  // 字元
    int count;         // 字元出現的頻率
    string strcode;    // 字元對應的哈夫曼編碼

    ByteInfo(int BIcount = 0)
        : count(BIcount)
    {}

    //計算兩個 ByteInfo 的總頻率
    ByteInfo operator+(const ByteInfo& b) const {
        ByteInfo tmp;
        tmp.count = count + b.count;  // 合併頻率
        return tmp;
    }

    //比較頻率大小
    bool operator>(const ByteInfo& b) const {
        return count > b.count;  // 頻率大的優先
    }

    //檢查頻率是否不同
    bool operator!=(const ByteInfo& b) const {
        return count != b.count;  // 頻率不同則返回 true
    }

    //檢查頻率是否相同
    bool operator==(const ByteInfo& b) const {
        return count == b.count;  // 頻率相同則返回 true
    }
};

// 壓縮和解壓縮文件的類別
class FileCompress {
public:
    // 建構函數
    FileCompress();

    // 壓縮指定路徑的文件
    bool Compress(const string& filepath);

    // 解壓縮指定路徑的文件
    bool UnCompress(const string& filepath);


private:
    // 從文件中讀取一行，並存儲到 strContent 中
    void GetLine(FILE* FIn, string& strContent);

    // 遍歷哈夫曼樹以生成每個字元的哈夫曼編碼
    void CreateHuffmanCode(HuffmanTreeNode<ByteInfo>* root);

    // 壓縮文件的具體實現，將壓縮信息存儲到文件中
    void CompressInfo(FILE* fl, const string& filepath);

    // 用於存儲所有可能字元的 ByteInfo 信息（ASCII 範圍 0-255）
    ByteInfo filebyteinfo[256];
};

