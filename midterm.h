#pragma once  // ����ƥ]�t�����Y�ɮ�
#include <iostream>
#include <string>
#include "HaffmanTree.h" 
using namespace std;

// �w�q�@�ӵ��c�A�Ω�s�x�r���������H��
struct ByteInfo {
    unsigned char ch;  // �r��
    int count;         // �r���X�{���W�v
    string strcode;    // �r�����������Ұҽs�X

    ByteInfo(int BIcount = 0)
        : count(BIcount)
    {}

    //�p���� ByteInfo ���`�W�v
    ByteInfo operator+(const ByteInfo& b) const {
        ByteInfo tmp;
        tmp.count = count + b.count;  // �X���W�v
        return tmp;
    }

    //����W�v�j�p
    bool operator>(const ByteInfo& b) const {
        return count > b.count;  // �W�v�j���u��
    }

    //�ˬd�W�v�O�_���P
    bool operator!=(const ByteInfo& b) const {
        return count != b.count;  // �W�v���P�h��^ true
    }

    //�ˬd�W�v�O�_�ۦP
    bool operator==(const ByteInfo& b) const {
        return count == b.count;  // �W�v�ۦP�h��^ true
    }
};

// ���Y�M�����Y������O
class FileCompress {
public:
    // �غc���
    FileCompress();

    // ���Y���w���|�����
    bool Compress(const string& filepath);

    // �����Y���w���|�����
    bool UnCompress(const string& filepath);


private:
    // �q���Ū���@��A�æs�x�� strContent ��
    void GetLine(FILE* FIn, string& strContent);

    // �M�����ҰҾ�H�ͦ��C�Ӧr�������Ұҽs�X
    void CreateHuffmanCode(HuffmanTreeNode<ByteInfo>* root);

    // ���Y��󪺨����{�A�N���Y�H���s�x����
    void CompressInfo(FILE* fl, const string& filepath);

    // �Ω�s�x�Ҧ��i��r���� ByteInfo �H���]ASCII �d�� 0-255�^
    ByteInfo filebyteinfo[256];
};

