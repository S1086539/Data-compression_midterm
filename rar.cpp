#include <cstdio>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;

// 定義結構：節點元素和哈夫曼樹節點
struct ele {
    int nd;         // 節點編號
    long long v;    // 節點頻率值
};
struct NODE {
    int l, r;           // 左右子節點
    long long v;        // 頻率值
    unsigned char c;    // 對應的字符
};

// 比較器，用於優先佇列
struct cmp {
    bool operator() (ele a, ele b) {
        return a.v > b.v; // 頻率小的優先級更高
    }
};

// 全域變數
NODE ND[1024];                    // 哈夫曼樹節點陣列
unsigned char code[1000];         // 暫存編碼
unsigned char mapcode[512][515];  // 字符對應的哈夫曼編碼
FILE *fin, *fout;                 // 輸入、輸出檔案指標

// 將單個位元壓入緩衝區並寫入檔案
int push_buf(int bit) {
    static int idx = 7, now = 0; // 位元緩衝區和索引
    static unsigned char bufcode = 0;
    bufcode |= bit << idx;       // 將位元寫入緩衝
    idx--;
    if (idx < 0) {               // 緩衝滿時寫入檔案
        fwrite(&bufcode, 1, 1, fout);
        idx = 7;
        bufcode = 0;
        now++;
    }
}

// 深度優先搜尋 (DFS) 建立哈夫曼編碼
void dfs(int nd, int dep) {
    if (ND[nd].l == 0 && ND[nd].r == 0) { // 遇到葉子節點
        code[dep] = '\0';                 // 結束符號
        for (int i = 0; i <= dep; i++)    // 儲存編碼
            mapcode[ND[nd].c][i] = code[i];
        push_buf(0);                      // 標記葉子
        for (int i = 7; i >= 0; i--)      // 寫入字符的二進位表示
            push_buf((ND[nd].c >> i) & 1);
        return;
    }
    push_buf(1);                          // 非葉子標記
    code[dep] = '0';
    dfs(ND[nd].l, dep + 1);               // 左子樹
    code[dep] = '1';
    dfs(ND[nd].r, dep + 1);               // 右子樹
}

// 主函式：壓縮檔案
int main(int argc, char* argv[]) {
    if (argc != 2) { // 檢查參數
        puts("Usage: ./compress <input file>");
        return 0;
    }

    // 開啟檔案
    fin = fopen(argv[1], "rb");
    string filename(argv[1]);
    filename += ".morris"; // 壓縮檔案名稱
    fout = fopen(filename.c_str(), "wb");

    if (fin == NULL || fout == NULL) { // 檔案開啟失敗
        puts("fopen r/w fail");
        return 0;
    }

    // 讀取檔案
    fseek(fin, 0, SEEK_END);
    int file_length = ftell(fin);
    rewind(fin);
    printf("file size %d\n", file_length);

    unsigned char *buf = new unsigned char[file_length];
    fread(buf, 1, file_length, fin);

    // 計算 ASCII 字符頻率
    int i, j, ASCI[256] = {}, n;
    for (i = 0; i < file_length; i++)
        ASCI[buf[i]]++;

    // 建立優先佇列
    ele l, r, e;
    priority_queue<ele, vector<ele>, cmp> pQ;
    for (i = 0, n = 0; i < 256; i++) {
        if (ASCI[i]) { // 有效字符
            e.nd = ++n, e.v = ASCI[i];
            ND[n].l = 0, ND[n].r = 0;
            ND[n].v = e.v;
            ND[n].c = i;
            pQ.push(e);
        }
    }

    // 建立哈夫曼樹
    int size = n;
    while (pQ.size() > 1) {
        l = pQ.top(), pQ.pop();
        r = pQ.top(), pQ.pop();
        size++;
        e.nd = size, e.v = l.v + r.v;
        ND[size].l = l.nd;
        ND[size].r = r.nd;
        ND[size].v = l.v + r.v;
        pQ.push(e);
    }

    // 進行 DFS 編碼
    dfs(size, 0);

    // 寫入檔案長度
    for (i = 31; i >= 0; i--)
        push_buf((file_length >> i) & 1);

    // 壓縮數據
    int percent = 0;
    for (i = 0; i < file_length; i++) {
        for (j = 0; mapcode[buf[i]][j]; j++)
            push_buf(mapcode[buf[i]][j] - '0');
        if (i * 100 / file_length > percent) {
            percent += 10;
            printf("%d%% ...\n", percent);
        }
    }

    // 填充位元
    for (i = 0; i < 8; i++)
        push_buf(1);
    // 顯示壓縮率
    float compression_ratio = (float)compressed_size / file_length * 100;
    printf("Original file size: %d bytes\n", file_length);
    printf("Compressed file size: %d bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", compression_ratio);
    // 關閉檔案並釋放記憶體
    fclose(fin);
    fclose(fout);
    delete[] buf;
    return 0;
}

