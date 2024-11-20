#pragma once 
#include <queue>
#include <vector>
using namespace std;

// �w�q���ҰҾ𪺸`�I���c
template <class T>
struct HuffmanTreeNode {
    HuffmanTreeNode<T>* left;   // ���l�`�I����
    HuffmanTreeNode<T>* right;  // �k�l�`�I����
    HuffmanTreeNode<T>* parent; // ���`�I����
    T weight;                   // �`�I���v��

    // �c�y��ơA�Ω��l�Ƹ`�I
    HuffmanTreeNode(const T& w)
        : left(NULL)        // ���l�`�I��l�Ƭ���
        , right(NULL)       // �k�l�`�I��l�Ƭ���
        , parent(NULL)      // ���`�I��l�Ƭ���
        , weight(w)            // �]�m�v��
    {}
};

template <class T>
struct Compare {
    typedef HuffmanTreeNode<T> Node; // �w�q�O�W�A�K��ϥ�
    bool operator()(const Node* left, const Node* right) {
        // �v�����p���`�I�u��
        return left->weight > right->weight;
    }
};

// �w�q���ҰҾ����O
template <class T>
class HuffmanTree {
    typedef HuffmanTreeNode<T> Node; // �w�q�O�W�A�`�I����

public:
    // �q�{�c�y��ơA��l�Ʈڸ`�I����
    HuffmanTree()
        : root(NULL)
    {}

    // �R�c��ơA�P�����ҰҾ�
    ~HuffmanTree() {
        DestroyHuffmanTree(root);
    }

    // �Ыث��ҰҾ�
    void CreateHuffmanTree(const T array[], size_t size, const T& invalid) {
        // �ϥ��u���Ŧ�C�O�s���ҰҾ�`�I
        std::priority_queue<Node*, vector<Node*>, Compare<T>> q;

        // 1. �ھ��v�ȼƲճЫظ`�I�ô��J��C
        for (size_t i = 0; i < size; ++i) {
            // ���L�L�ĸ`�I�]�v�Ȭ� `invalid`�^
            if (array[i] != invalid)
                q.push(new Node(array[i]));
        }

        // 2. ���ЦX�ֳ̤p�v�Ȫ������l��A����u�ѤU�@����
        while (q.size() > 1) {
            // ���X�v�ȳ̤p�������l��
            Node* left = q.top();
            q.pop();
            Node* right = q.top();
            q.pop();

            // �Ыطs�`�I�A�v�Ȭ���l���v�Ȥ��M
            Node* parent = new Node(left->weight + right->weight);
            parent->left = left;   // �]�m���l�`�I
            parent->right = right; // �]�m�k�l�`�I
            left->parent = parent; // ��s���l�`�I�����`�I����
            right->parent = parent;// ��s�k�l�`�I�����`�I����

            // �N�s�`�I�[�J���u���Ŧ�C��
            q.push(parent);
        }

        // �u���Ŧ�C���ѤU���̫�@����Y�����ҰҾ�
        root = q.top();
    }

    // ������ҰҾ𪺮ڸ`�I
    Node* GetRoot() {
        return root;
    }

    // �P�����ҰҾ�]���j�覡�^
    void DestroyHuffmanTree(Node*& proot) {
        if (proot != nullptr) {
            // �P�����l��
            DestroyHuffmanTree(proot->left);
            // �P���k�l��
            DestroyHuffmanTree(proot->right);
            // �]�m��e�`�I����
            proot = nullptr;
        }
    }

private:
    Node* root; // ���ҰҾ𪺮ڸ`�I
};

