#pragma once 
#include <queue>
#include <vector>
using namespace std;

// 定義哈夫曼樹的節點結構
template <class T>
struct HuffmanTreeNode {
    HuffmanTreeNode<T>* left;   // 左子節點指標
    HuffmanTreeNode<T>* right;  // 右子節點指標
    HuffmanTreeNode<T>* parent; // 父節點指標
    T weight;                   // 節點的權重

    // 構造函數，用於初始化節點
    HuffmanTreeNode(const T& w)
        : left(NULL)        // 左子節點初始化為空
        , right(NULL)       // 右子節點初始化為空
        , parent(NULL)      // 父節點初始化為空
        , weight(w)            // 設置權重
    {}
};

template <class T>
struct Compare {
    typedef HuffmanTreeNode<T> Node; // 定義別名，便於使用
    bool operator()(const Node* left, const Node* right) {
        // 權重較小的節點優先
        return left->weight > right->weight;
    }
};

// 定義哈夫曼樹類別
template <class T>
class HuffmanTree {
    typedef HuffmanTreeNode<T> Node; // 定義別名，節點類型

public:
    // 默認構造函數，初始化根節點為空
    HuffmanTree()
        : root(NULL)
    {}

    // 析構函數，銷毀哈夫曼樹
    ~HuffmanTree() {
        DestroyHuffmanTree(root);
    }

    // 創建哈夫曼樹
    void CreateHuffmanTree(const T array[], size_t size, const T& invalid) {
        // 使用優先級佇列保存哈夫曼樹節點
        std::priority_queue<Node*, vector<Node*>, Compare<T>> q;

        // 1. 根據權值數組創建節點並插入佇列
        for (size_t i = 0; i < size; ++i) {
            // 跳過無效節點（權值為 `invalid`）
            if (array[i] != invalid)
                q.push(new Node(array[i]));
        }

        // 2. 反覆合併最小權值的兩顆子樹，直到只剩下一顆樹
        while (q.size() > 1) {
            // 取出權值最小的兩顆子樹
            Node* left = q.top();
            q.pop();
            Node* right = q.top();
            q.pop();

            // 創建新節點，權值為兩子樹權值之和
            Node* parent = new Node(left->weight + right->weight);
            parent->left = left;   // 設置左子節點
            parent->right = right; // 設置右子節點
            left->parent = parent; // 更新左子節點的父節點指標
            right->parent = parent;// 更新右子節點的父節點指標

            // 將新節點加入到優先級佇列中
            q.push(parent);
        }

        // 優先級佇列中剩下的最後一顆樹即為哈夫曼樹
        root = q.top();
    }

    // 獲取哈夫曼樹的根節點
    Node* GetRoot() {
        return root;
    }

    // 銷毀哈夫曼樹（遞迴方式）
    void DestroyHuffmanTree(Node*& proot) {
        if (proot != nullptr) {
            // 銷毀左子樹
            DestroyHuffmanTree(proot->left);
            // 銷毀右子樹
            DestroyHuffmanTree(proot->right);
            // 設置當前節點為空
            proot = nullptr;
        }
    }

private:
    Node* root; // 哈夫曼樹的根節點
};

