#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Ʈ���� ��� ����ü ����
// �� ���� �ִ� 26���� �ڽ� ��带 ���� �� ������, �̴� ���� ���ĺ� �ҹ���(a~z)�� �ش�
typedef struct TrieNode {
    struct TrieNode* children[26]; // ���ĺ� ����ŭ �ڽ� ��� �迭
    int is_end_of_word; // �ܾ��� �� ���θ� ��Ÿ���� �÷���(1�̸� �ܾ��� ��, 0�̸� ��� ����)
} TrieNode;

// ���ο� ��� ���� �Լ�
// Ʈ������ �� ��带 �ʱ�ȭ�ϴ� �Լ�
TrieNode* createNode(void) {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode)); // ���ο� ��带 ���� �Ҵ�
    node->is_end_of_word = 0; // ��尡 �ܾ��� ���� �ƴ��� �ʱ�ȭ
    for (int i = 0; i < 26; i++) {
        node->children[i] = NULL; // ��� �ڽ� ��带 NULL�� �ʱ�ȭ(���� �ڽ� ��尡 ����)
    }
    return node; // ������ ��� ��ȯ
}

// Ʈ���̿� �ܾ �����ϴ� �Լ�
// �ܾ��� �� ���ڸ� ���󰡸� Ʈ���̿� ��带 �߰�
void insert(TrieNode* root, const char* word) {
    TrieNode* current = root; // ���� ��带 ��Ʈ�� ����
    while (*word) { // �ܾ��� �� ���ڸ� ���������� ó��
        int index = *word - 'a'; // ���� ������ �ε����� ���('a'�� 0, 'b'�� 1, ..., 'z'�� 25)
        if (!current->children[index]) { // �ش� ���ڸ� ���� �ڽ� ��尡 ���ٸ�
            current->children[index] = createNode(); // ���ο� ��带 ����
        }
        current = current->children[index]; // ���� ��带 �ڽ� ���� �̵�
        word++; // ���� ���ڷ� �̵�
    }
    current->is_end_of_word = 1; // ������ ���ڿ� �����ϸ� �ܾ��� ������ ǥ��
}

// Ʈ���̿��� �ܾ �˻��ϴ� �Լ�
// �־��� �ܾ Ʈ���̿� �����ϴ��� Ȯ��
int search(TrieNode* root, const char* word) {
    TrieNode* current = root; // ���� ��带 ��Ʈ�� ����
    while (*word) { // �ܾ��� �� ���ڸ� ���������� ó��
        int index = *word - 'a'; // ���� ������ �ε����� ���
        if (!current->children[index]) { // �ش� ���ڸ� ���� �ڽ� ��尡 ������
            return 0; // �ܾ �������� ����
        }
        current = current->children[index]; // ���� ��带 �ڽ� ���� �̵�
        word++; // ���� ���ڷ� �̵�
    }
    return current->is_end_of_word; // �ܾ��� ���� �����ߴ��� ���θ� ��ȯ(1�̸� ����, 0�̸� ������)
}

// Ʈ���̿��� �ܾ �����ϴ� ��� �Լ�
// �ܾ ������ ��, ���ʿ��� ��嵵 �Բ� ����
int deleteWord(TrieNode* root, const char* word, int depth) {
    // ���� ���: Ʈ���̰� ��� �ְų� �߸��� ȣ���� ��
    if (!root) {
        return 0; // ������ ���� �����Ƿ� ����
    }

    // �ܾ��� ������ ���ڿ� �������� ��
    if (depth == strlen(word)) {
        if (root->is_end_of_word) { // �ش� ��尡 �ܾ��� ���̸�
            root->is_end_of_word = 0; // �ܾ��� �� ǥ�ø� ����
        }

        // �ش� ��尡 �ڽ��� ������(�� �̻� ���ڰ� ������) ��带 ����
        for (int i = 0; i < 26; i++) {
            if (root->children[i]) { // �ڽ� ��尡 �ϳ��� ������
                return 0; // �������� �ʰ� ����
            }
        }
        free(root); // ��带 ����
        return 1; // ��尡 �����Ǿ����� ��ȯ
    }

    // ��������� �ڽ� ��带 Ž���ϸ� ����
    int index = word[depth] - 'a'; // ���� ������ �ε����� ���
    if (deleteWord(root->children[index], word, depth + 1)) { // �ڽ� ���� ��� ȣ��
        root->children[index] = NULL; // �ش� �ڽ� ��带 ����

        // ���� ��尡 �ܾ��� ���� �ƴϰ�, �ڽ� ��尡 ������ ���� ��嵵 ����
        if (!root->is_end_of_word) {
            for (int i = 0; i < 26; i++) {
                if (root->children[i]) { // �ڽ� ��尡 �ϳ��� ������
                    return 0; // �������� �ʰ� ����
                }
            }
            free(root); // ��带 ����
            return 1; // ��尡 �����Ǿ����� ��ȯ
        }
    }
    return 0; // �������� �ʾ����� ��ȯ
}

// ���� �Լ�
// Ʈ���� �ڷᱸ���� �׽�Ʈ�ϴ� �ڵ�
int main() {
    TrieNode* root = createNode(); // ��Ʈ ��带 ����

    insert(root, "hello"); // "hello" �ܾ Ʈ���̿� ����
    insert(root, "world"); // "world" �ܾ Ʈ���̿� ����

    printf("Search 'hello': %d\n", search(root, "hello")); // "hello" �˻� ��� ���(�����ϸ� 1)
    printf("Search 'world': %d\n", search(root, "world")); // "world" �˻� ��� ���(�����ϸ� 1)
    printf("Search 'hi': %d\n", search(root, "hi")); // "hi" �˻� ��� ���(�������� ������ 0)

    deleteWord(root, "hello", 0); // "hello" �ܾ Ʈ���̿��� ����
    printf("Search 'hello' after deletion: %d\n", search(root, "hello")); // ���� �� "hello" �˻�(�������� ������ 0)

    return 0; // ���α׷� ����
}
