#define _CRT_SECURE_NO_WARNINGS
#include"midterm.h"
#include"HaffmanTree.h"
#include <iostream>
FileCompress::FileCompress() {
    // 初始化 filebyteinfo 陣列，每個元素的 ch 設置為對應的 ASCII 值
    for (int i = 0; i < 256; i++) {
        filebyteinfo[i].ch = i; // 設置 ch 為對應 ASCII 字元
    }
}
bool FileCompress::Compress(const string& filepath) {
    // 1. 獲取文件中每個字節出現的次數
    FILE* fp = fopen(filepath.c_str(), "rb"); // 以二進制讀取模式打開文件
    if (fp == nullptr) { // 如果文件打開失敗
        perror("fopen"); // 輸出錯誤資訊
        return false;
    }

    // 循環讀取文件內容，計算每個字元的頻率
    unsigned char readbuff[1024]; // 緩衝區
    while (1) {
        size_t read_size = fread(readbuff, 1, 1024, fp); // 每次讀取 1024 字節
        if (read_size == 0) { // 如果讀取結束
            break;
        }
        // 更新每個字元的出現次數
        for (size_t i = 0; i < read_size; ++i) {
            filebyteinfo[readbuff[i]].count++;
        }
    }

    // 2. 根據字元頻率構建哈夫曼樹
    HuffmanTree<ByteInfo> ht; // 創建哈夫曼樹
    ByteInfo invalid; // 定義無效節點（用於標識空節點）
    ht.CreateHuffmanTree(filebyteinfo, 256, invalid); // 构建哈夫曼?

    // 3. 生成哈夫曼編碼
    CreateHuffmanCode(ht.GetRoot());

    // 4. 保存壓縮資訊到文件
    string filename_Compress;
    cout << "輸入壓縮後的文件名稱" << endl;
    cin >> filename_Compress; // 從用戶輸入壓縮後的文件名
    FILE* fl = fopen(filename_Compress.c_str(), "wb"); // 以二進制寫入模式打開文件
    CompressInfo(fl, filepath); // 保存壓縮資訊

    // 5. 使用哈夫曼編碼重新寫入壓縮內容
    fseek(fp, 0, SEEK_SET); // 重置文件指針到文件頭部
    unsigned char ch = 0;   // 保存當前字元
    int bitcount = 0;       // 記錄位數
    while (1) {
        size_t read_size = fread(readbuff, 1, 1024, fp);
        if (read_size == 0) {
            break;
        }
        for (size_t i = 0; i < read_size; ++i) {
            string& strcode = filebyteinfo[readbuff[i]].strcode; // 獲取哈夫曼編碼
            for (size_t j = 0; j < strcode.size(); j++) {
                ch <<= 1; // 左移一位
                if (strcode[j] == '1') {
                    ch |= 1; // 將當前位設為 1
                }
                bitcount++;
                if (bitcount == 8) { // 當累積 8 位時，寫入文件
                    fputc(ch, fl);
                    bitcount = 0;
                }
            }
        }
    }

    // 如果還有剩餘未滿 8 位的數據
    if (bitcount > 0 && bitcount < 8) {
        ch <<= (8 - bitcount); // 左移補齊位數
        fputc(ch, fl); // 寫入文件
    }

    // 顯示壓縮結果
    int fpSize = ftell(fl); // 壓縮後文件大小
    int outSize = ftell(fp); // 壓縮前文件大小
    cout << "\n***********************************\n" << endl;
    cout << "壓縮前：" << fpSize * 1024 << "字節" << endl;
    cout << "壓縮後：" << outSize * 1024 << "字節" << endl;
    cout << "***********************************" << endl;

    fclose(fp); 
    fclose(fl); 
    cout << "壓縮成功" << endl;
    return true;
}
bool FileCompress::UnCompress(const string& filepath) {
    // 1. 獲取壓縮文件中的壓縮資訊
    FILE* FIn = fopen(filepath.c_str(), "rb"); // 以二進制讀取模式打開壓縮文件
    if (FIn == nullptr) { // 如果打開失敗
        perror("fopen"); // 打印錯誤資訊
        return false;
    }

    // 讀取後綴、總行數和頻次資訊
    string PostFix;
    GetLine(FIn, PostFix);
    string strContent;
    GetLine(FIn, strContent);
    size_t linecount = atoi(strContent.c_str()); // 總行數
    for (size_t i = 0; i < linecount; ++i) {
        GetLine(FIn, strContent); // 每次讀取一行資訊
        if (strContent == "") { // 如果出現換行
            strContent += "\n";
            GetLine(FIn, strContent);
        }
        filebyteinfo[(unsigned char)strContent[0]].count = atoi(strContent.c_str() + 2); // 獲取頻次資訊
    }

    // 2. 恢復哈夫曼樹
    HuffmanTree<ByteInfo> ht;
    ByteInfo invalid;
    ht.CreateHuffmanTree(filebyteinfo, 256, invalid);

    // 3. 解壓縮文件
    string filename_UnCompress;
    cout << "輸入解壓後的文件名稱" << endl;
    cin >> filename_UnCompress;
    FILE* fout = fopen(filename_UnCompress.c_str(), "wb"); // 打開解壓縮文件
    unsigned char readbuff[1024]; // 緩衝區
    unsigned char bitcount = 0;
    HuffmanTreeNode<ByteInfo>* cur = ht.GetRoot(); // 指向哈夫曼樹根節點
    const int FileSize = cur->weight.count; // 根節點權值表示文件大小
    int CompressSize = 0;

    // 遍歷壓縮文件，逐位解碼
    while (1) {
        size_t read_size = fread(readbuff, 1, 1024, FIn); // 讀取壓縮文件
        if (read_size == 0) {
            break;
        }
        for (size_t i = 0; i < read_size; i++) {
            unsigned char ch = readbuff[i];
            bitcount = 0;
            while (bitcount < 8) {
                if (ch & 0x80) { // 檢查當前位是否為 1
                    cur = cur->right;
                } else {
                    cur = cur->left;
                }
                if (cur->left == nullptr && cur->right == nullptr) { // 如果到達葉子節點
                    fputc(cur->weight.ch, fout); // 輸出對應字元
                    cur = ht.GetRoot(); // 回到根節點
                    CompressSize++;
                    if (CompressSize == FileSize) { // 解壓縮完成
                        break;
                    }
                }
                bitcount++;
                ch <<= 1; // 左移檢查下一位
            }
        }
    }

    // 顯示解壓縮結果
    int fpSize = ftell(fout);
    int outSize = ftell(FIn);
    cout << "\n***********************************\n" << endl;
    cout << "解壓縮前：" << fpSize * 1024 << "字節" << endl;
    cout << "解壓縮後：" << outSize * 1024 << "字節" << endl;
    cout << "***********************************" << endl;

    fclose(FIn); // 關閉壓縮文件
    fclose(fout); // 關閉解壓文件
    cout << "解壓縮成功" << endl;
    return true;
}

 

