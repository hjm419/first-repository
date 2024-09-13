#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 트라이 노드 구조체 정의
// 각 노드는 최대 26개의 자식 노드를 가질 수 있으며
typedef struct TrieNode {
    struct TrieNode* children[26]; // 알파벳 수만큼 자식 노드 배열
    int is_end_of_word; // 단어의 끝 여부를 나타내는 플래그(1이면 단어의 끝, 0이면 계속 진행)
} TrieNode;

// 새로운 노드 생성 함수
// 트라이의 각 노드를 초기화하는 함수
TrieNode* createNode(void) {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode)); // 새로운 노드를 동적 할당
    node->is_end_of_word = 0; // 노드가 단어의 끝이 아님을 초기화
    for (int i = 0; i < 26; i++) {
        node->children[i] = NULL; // 모든 자식 노드를 NULL로 초기화(아직 자식 노드가 없음)
    }
    return node; // 생성된 노드 반환
}

// 트라이에 단어를 삽입하는 함수
// 단어의 각 문자를 따라가며 트라이에 노드를 추가
void insert(TrieNode* root, const char* word) {
    TrieNode* current = root; // 현재 노드를 루트로 설정
    while (*word) { // 단어의 각 문자를 순차적으로 처리
        int index = *word - 'a'; // 현재 문자의 인덱스를 계산('a'는 0, 'b'는 1, ..., 'z'는 25)
        if (!current->children[index]) { // 해당 문자를 위한 자식 노드가 없다면
            current->children[index] = createNode(); // 새로운 노드를 생성
        }
        current = current->children[index]; // 현재 노드를 자식 노드로 이동
        word++; // 다음 문자로 이동
    }
    current->is_end_of_word = 1; // 마지막 문자에 도착하면 단어의 끝임을 표시
}

// 트라이에서 단어를 검색하는 함수
// 주어진 단어가 트라이에 존재하는지 확인
int search(TrieNode* root, const char* word) {
    TrieNode* current = root; // 현재 노드를 루트로 설정
    while (*word) { // 단어의 각 문자를 순차적으로 처리
        int index = *word - 'a'; // 현재 문자의 인덱스를 계산
        if (!current->children[index]) { // 해당 문자를 위한 자식 노드가 없으면
            return 0; // 단어가 존재하지 않음
        }
        current = current->children[index]; // 현재 노드를 자식 노드로 이동
        word++; // 다음 문자로 이동
    }
    return current->is_end_of_word; // 단어의 끝에 도달했는지 여부를 반환(1이면 존재, 0이면 미존재)
}

// 트라이에서 단어를 삭제하는 재귀 함수
// 단어를 삭제할 때, 불필요한 노드도 함께 삭제
int deleteWord(TrieNode* root, const char* word, int depth) {
    // 기저 사례: 트라이가 비어 있거나 잘못된 호출일 때
    if (!root) {
        return 0; // 삭제할 것이 없으므로 종료
    }

    // 단어의 마지막 문자에 도달했을 때
    if (depth == strlen(word)) {
        if (root->is_end_of_word) { // 해당 노드가 단어의 끝이면
            root->is_end_of_word = 0; // 단어의 끝 표시를 해제
        }

        // 해당 노드가 자식이 없으면(더 이상 문자가 없으면) 노드를 삭제
        for (int i = 0; i < 26; i++) {
            if (root->children[i]) { // 자식 노드가 하나라도 있으면
                return 0; // 삭제하지 않고 종료
            }
        }
        free(root); // 노드를 삭제
        return 1; // 노드가 삭제되었음을 반환
    }

    // 재귀적으로 자식 노드를 탐색하며 삭제
    int index = word[depth] - 'a'; // 현재 문자의 인덱스를 계산
    if (deleteWord(root->children[index], word, depth + 1)) { // 자식 노드로 재귀 호출
        root->children[index] = NULL; // 해당 자식 노드를 삭제

        // 현재 노드가 단어의 끝이 아니고, 자식 노드가 없으면 현재 노드도 삭제
        if (!root->is_end_of_word) {
            for (int i = 0; i < 26; i++) {
                if (root->children[i]) { // 자식 노드가 하나라도 있으면
                    return 0; // 삭제하지 않고 종료
                }
            }
            free(root); // 노드를 삭제
            return 1; // 노드가 삭제되었음을 반환
        }
    }
    return 0; // 삭제되지 않았음을 반환
}

// 메인 함수
// 트라이 자료구조를 테스트하는 코드
int main() {
    TrieNode* root = createNode(); // 루트 노드를 생성

    insert(root, "hello"); // "hello" 단어를 트라이에 삽입
    insert(root, "world"); // "world" 단어를 트라이에 삽입

    printf("Search 'hello': %d\n", search(root, "hello")); // "hello" 검색 결과 출력(존재하면 1)
    printf("Search 'world': %d\n", search(root, "world")); // "world" 검색 결과 출력(존재하면 1)
    printf("Search 'hi': %d\n", search(root, "hi")); // "hi" 검색 결과 출력(존재하지 않으면 0)

    deleteWord(root, "hello", 0); // "hello" 단어를 트라이에서 삭제
    printf("Search 'hello' after deletion: %d\n", search(root, "hello")); // 삭제 후 "hello" 검색(존재하지 않으면 0)

    return 0; // 프로그램 종료
}
