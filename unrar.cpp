#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

// 定義節點結構，用於哈夫曼樹的每個節點
struct NODE {
    int l, r;             // 左子節點和右子節點的索引
    unsigned char c;      // 節點對應的字符 (葉子節點使用)
};

// 全域變數，用於存放哈夫曼樹節點和其他輔助資料
NODE ND[1024];                 // 儲存哈夫曼樹的節點 (最多1024個)
unsigned char mapcode[515][515]; // 暫時未使用，原本可能用於字符編碼
FILE *fin, *fout;              // 文件指標，用於讀取和寫入檔案
unsigned char *buf;            // 用於讀取壓縮檔案的緩衝區

// 從緩衝區提取一個位元
int pop_buf() {
    static int idx = 7, now = 0, ans; // idx 指向當前字節的第幾位，now 指向當前字節位置
    ans = (buf[now] >> idx) & 1;      // 提取當前字節的第 idx 位
    idx--;                            // 移動到下一位
    if (idx < 0) {                    // 當 idx 小於 0 時，移動到下一個字節
        idx = 7;
        now++;
    }
    return ans;                       // 返回當前位的值 (0 或 1)
}

// 使用遞迴構建哈夫曼樹
void build(int nd, int* size) {
    int l, r;
    l = r = pop_buf();
    if (l == 0 && r == 0) {
        ND[nd].l = ND[nd].r = 0;
        ND[nd].c = 0;
        for (int i = 7; i >= 0; i--)
            ND[nd].c |= pop_buf() << i;
        return;
    }
    ND[nd].l = ++(*size);
    build(*size, size);
    ND[nd].r = ++(*size);
    build(*size, size);
}

int main(int argc, char* argv[]) {
    // 檢查參數數量是否正確
    if (argc != 3) {
        puts("Usage: ./decompress <input file> <output file>");          
        return 0;                    
    }

    int file_length;                 // 壓縮檔案的長度

    // 開啟輸入檔案 (壓縮檔案) 和輸出檔案
    fin = fopen(argv[1], "rb");
    fout = fopen(argv[2], "wb");

    // 檢查檔案是否成功開啟
    if (fin == NULL || fout == NULL) {
        puts("fopen r/w fail");      // 輸出錯誤訊息
        return 0;                    // 結束程式
    }

    // 獲取壓縮檔案的大小
    fseek(fin, 0, SEEK_END);
    file_length = ftell(fin);        // 獲取檔案指標位置 (檔案大小)
    rewind(fin);                     // 重置檔案指標到檔案開頭

    buf = new unsigned char[file_length]; // 分配緩衝區以存放壓縮檔案內容

    fread(buf, 1, file_length, fin); // 將壓縮檔案內容讀入緩衝區

    int size = 1;           // 主程式定義並初始化 size
    build(size, &size);     // 傳遞 size 的位址                    

    int i, j, n, idx;
    // 讀取解壓後檔案的大小 (32 位)
    for (i = 31, n = 0; i >= 0; i--)
        n |= (1 & pop_buf()) << i;

    while (n--) {                    // 解壓每個字符
        idx = 1;                     // 從哈夫曼樹根節點開始
        while (1) {
            if (!ND[idx].l && !ND[idx].r) { // 如果到達葉子節點
                fwrite(&ND[idx].c, 1, 1, fout); // 將該字符寫入解壓檔案
                break;
            }
            i = pop_buf();           // 獲取下一個位元，決定移動到左或右子節點
            if (i) idx = ND[idx].r;  // 移動到右子節點
            else idx = ND[idx].l;    // 移動到左子節點
        }
    }

    // 關閉檔案並釋放資源
    fclose(fin);
    fclose(fout);
    delete[] buf;

    return 0;
}

 

