#define _CRT_SECURE_NO_WARNINGS
#include"midterm.h"
#include"HaffmanTree.h"
#include <iostream>
FileCompress::FileCompress() {
    // ��l�� filebyteinfo �}�C�A�C�Ӥ����� ch �]�m�������� ASCII ��
    for (int i = 0; i < 256; i++) {
        filebyteinfo[i].ch = i; // �]�m ch ������ ASCII �r��
    }
}
bool FileCompress::Compress(const string& filepath) {
    // 1. �����󤤨C�Ӧr�`�X�{������
    FILE* fp = fopen(filepath.c_str(), "rb"); // �H�G�i��Ū���Ҧ����}���
    if (fp == nullptr) { // �p�G��󥴶}����
        perror("fopen"); // ��X���~��T
        return false;
    }

    // �`��Ū����󤺮e�A�p��C�Ӧr�����W�v
    unsigned char readbuff[1024]; // �w�İ�
    while (1) {
        size_t read_size = fread(readbuff, 1, 1024, fp); // �C��Ū�� 1024 �r�`
        if (read_size == 0) { // �p�GŪ������
            break;
        }
        // ��s�C�Ӧr�����X�{����
        for (size_t i = 0; i < read_size; ++i) {
            filebyteinfo[readbuff[i]].count++;
        }
    }

    // 2. �ھڦr���W�v�c�ث��ҰҾ�
    HuffmanTree<ByteInfo> ht; // �Ыث��ҰҾ�
    ByteInfo invalid; // �w�q�L�ĸ`�I�]�Ω���ѪŸ`�I�^
    ht.CreateHuffmanTree(filebyteinfo, 256, invalid); // �۫ث��Ұ�?

    // 3. �ͦ����Ұҽs�X
    CreateHuffmanCode(ht.GetRoot());

    // 4. �O�s���Y��T����
    string filename_Compress;
    cout << "��J���Y�᪺���W��" << endl;
    cin >> filename_Compress; // �q�Τ��J���Y�᪺���W
    FILE* fl = fopen(filename_Compress.c_str(), "wb"); // �H�G�i��g�J�Ҧ����}���
    CompressInfo(fl, filepath); // �O�s���Y��T

    // 5. �ϥΫ��Ұҽs�X���s�g�J���Y���e
    fseek(fp, 0, SEEK_SET); // ���m�����w�����Y��
    unsigned char ch = 0;   // �O�s��e�r��
    int bitcount = 0;       // �O�����
    while (1) {
        size_t read_size = fread(readbuff, 1, 1024, fp);
        if (read_size == 0) {
            break;
        }
        for (size_t i = 0; i < read_size; ++i) {
            string& strcode = filebyteinfo[readbuff[i]].strcode; // ������Ұҽs�X
            for (size_t j = 0; j < strcode.size(); j++) {
                ch <<= 1; // �����@��
                if (strcode[j] == '1') {
                    ch |= 1; // �N��e��]�� 1
                }
                bitcount++;
                if (bitcount == 8) { // ��ֿn 8 ��ɡA�g�J���
                    fputc(ch, fl);
                    bitcount = 0;
                }
            }
        }
    }

    // �p�G�٦��Ѿl���� 8 �쪺�ƾ�
    if (bitcount > 0 && bitcount < 8) {
        ch <<= (8 - bitcount); // �����ɻ����
        fputc(ch, fl); // �g�J���
    }

    // ������Y���G
    int fpSize = ftell(fl); // ���Y����j�p
    int outSize = ftell(fp); // ���Y�e���j�p
    cout << "\n***********************************\n" << endl;
    cout << "���Y�e�G" << fpSize * 1024 << "�r�`" << endl;
    cout << "���Y��G" << outSize * 1024 << "�r�`" << endl;
    cout << "***********************************" << endl;

    fclose(fp); 
    fclose(fl); 
    cout << "���Y���\" << endl;
    return true;
}
bool FileCompress::UnCompress(const string& filepath) {
    // 1. ������Y��󤤪����Y��T
    FILE* FIn = fopen(filepath.c_str(), "rb"); // �H�G�i��Ū���Ҧ����}���Y���
    if (FIn == nullptr) { // �p�G���}����
        perror("fopen"); // ���L���~��T
        return false;
    }

    // Ū�����B�`��ƩM�W����T
    string PostFix;
    GetLine(FIn, PostFix);
    string strContent;
    GetLine(FIn, strContent);
    size_t linecount = atoi(strContent.c_str()); // �`���
    for (size_t i = 0; i < linecount; ++i) {
        GetLine(FIn, strContent); // �C��Ū���@���T
        if (strContent == "") { // �p�G�X�{����
            strContent += "\n";
            GetLine(FIn, strContent);
        }
        filebyteinfo[(unsigned char)strContent[0]].count = atoi(strContent.c_str() + 2); // ����W����T
    }

    // 2. ��_���ҰҾ�
    HuffmanTree<ByteInfo> ht;
    ByteInfo invalid;
    ht.CreateHuffmanTree(filebyteinfo, 256, invalid);

    // 3. �����Y���
    string filename_UnCompress;
    cout << "��J�����᪺���W��" << endl;
    cin >> filename_UnCompress;
    FILE* fout = fopen(filename_UnCompress.c_str(), "wb"); // ���}�����Y���
    unsigned char readbuff[1024]; // �w�İ�
    unsigned char bitcount = 0;
    HuffmanTreeNode<ByteInfo>* cur = ht.GetRoot(); // ���V���ҰҾ�ڸ`�I
    const int FileSize = cur->weight.count; // �ڸ`�I�v�Ȫ�ܤ��j�p
    int CompressSize = 0;

    // �M�����Y���A�v��ѽX
    while (1) {
        size_t read_size = fread(readbuff, 1, 1024, FIn); // Ū�����Y���
        if (read_size == 0) {
            break;
        }
        for (size_t i = 0; i < read_size; i++) {
            unsigned char ch = readbuff[i];
            bitcount = 0;
            while (bitcount < 8) {
                if (ch & 0x80) { // �ˬd��e��O�_�� 1
                    cur = cur->right;
                } else {
                    cur = cur->left;
                }
                if (cur->left == nullptr && cur->right == nullptr) { // �p�G��F���l�`�I
                    fputc(cur->weight.ch, fout); // ��X�����r��
                    cur = ht.GetRoot(); // �^��ڸ`�I
                    CompressSize++;
                    if (CompressSize == FileSize) { // �����Y����
                        break;
                    }
                }
                bitcount++;
                ch <<= 1; // �����ˬd�U�@��
            }
        }
    }

    // ��ܸ����Y���G
    int fpSize = ftell(fout);
    int outSize = ftell(FIn);
    cout << "\n***********************************\n" << endl;
    cout << "�����Y�e�G" << fpSize * 1024 << "�r�`" << endl;
    cout << "�����Y��G" << outSize * 1024 << "�r�`" << endl;
    cout << "***********************************" << endl;

    fclose(FIn); // �������Y���
    fclose(fout); // �����������
    cout << "�����Y���\" << endl;
    return true;
}

 

