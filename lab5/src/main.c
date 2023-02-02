#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define ASCII_SIZE 256
#define MAX_HEIGHT 100
#define MAX_BUFFER_SIZE 1024

FILE* in;
FILE* out;

typedef struct HuffmanTreeNode {
    unsigned char data;
    unsigned frequency;
    struct HuffmanTreeNode* left;
    struct HuffmanTreeNode* right;
} Node;

typedef struct HuffmanCodes {
    int size[256];
    int matrix[256][100];
} Codes;

typedef struct PriorityQueue {
    int size;
    unsigned capacity;
    Node** array;
} Queue;

Node* newNode(unsigned char data, unsigned freq) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->frequency = freq;
    return temp;
}

Queue* createQueue(unsigned capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->size = 0;
    queue->capacity = capacity;
    queue->array = (Node**)malloc(queue->capacity * sizeof(Node*));
    return queue;
}

void SwapNodes(Node** a, Node** b) {
    Node* t = *a;
    *a = *b;
    *b = t;
}

void Heapify(Queue* queue, int index) {
    int root = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if (left < queue->size &&
        queue->array[left]->frequency < queue->array[root]->frequency)
        root = left;
    if (right < queue->size &&
        queue->array[right]->frequency < queue->array[root]->frequency)
        root = right;

    if (root != index) {
        SwapNodes(&queue->array[root], &queue->array[index]);
        Heapify(queue, root);
    }
}

int IsSingleNode(struct PriorityQueue* minHeap) {
    return (minHeap->size == 1);
}

Node* GetMinimumFreq(struct PriorityQueue* minHeap) {
    Node* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    Heapify(minHeap, 0);
    return temp;
}

void InsertNode(Queue* queue, Node* new_node) {
    if (!queue) {
        return;
    }
    ++queue->size;
    int i = queue->size - 1;
    while (i && new_node->frequency < queue->array[(i - 1) / 2]->frequency) {
        queue->array[i] = queue->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    queue->array[i] = new_node;
}

void Heapsort(struct PriorityQueue* minHeap) {
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i > -1; --i) {
        Heapify(minHeap, i);
    }
}

int IsLeaf(Node* root) {
    return (!(root->left) && !(root->right));
}

Queue* InitializeHeap(unsigned char data[], int freq[], int size) {
    Queue* queue = createQueue(size);
    for (int i = 0; i < size; ++i)
        queue->array[i] = newNode(data[i], freq[i]);
    queue->size = size;
    Heapsort(queue);
    return queue;
}

Node* BuildHuffmanTree(Queue* minHeap) {
    if (IsSingleNode(minHeap)) {
        Node* root = newNode(minHeap->array[0]->data, 0);
        return root;
    }
    Node* left;
    Node* right;
    while (!IsSingleNode(minHeap)) {
        left = GetMinimumFreq(minHeap);
        right = GetMinimumFreq(minHeap);
        Node* top;
        long long int left_frequency = 0;
        long long int right_frequency = 0;
        if (left) {
            left_frequency = left->frequency;
        }
        if (right) {
            right_frequency = right->frequency;
        }
        top = newNode('$', left_frequency + right_frequency);
        top->left = left;
        top->right = right;
        InsertNode(minHeap, top);
    }
    Node* root = GetMinimumFreq(minHeap);
    return root;
}

void GetCodes(Node* root,
              int temp_code[],
              int temp_code_size,
              Codes* alphabet) {
    if (root->left) {
        temp_code[temp_code_size] = 0;
        GetCodes(root->left, temp_code, temp_code_size + 1, alphabet);
    }
    if (root->right) {
        temp_code[temp_code_size] = 1;
        GetCodes(root->right, temp_code, temp_code_size + 1, alphabet);
    }
    if (IsLeaf(root)) {
        for (int i = 0; i < temp_code_size; ++i) {
            alphabet->matrix[root->data][i] = temp_code[i];
        }
        alphabet->size[root->data] = temp_code_size;
    }
}

Node* HuffmanCodes(Codes* codes_alphabet, Queue* minHeap) {
    Node* root = BuildHuffmanTree(minHeap);
    int temp_code[MAX_HEIGHT];
    int temp_code_size = 0;
    GetCodes(root, temp_code, temp_code_size, codes_alphabet);
    return root;
}

void WriteBit(unsigned char* bit_array, unsigned int index) {
    unsigned int byte = (index) / 8;
    char bit_position = (char)((index) % 8);
    bit_array[byte] |= (1 << (7 - bit_position));
}

void WriteZero(unsigned char* bit_array, unsigned int index) {
    unsigned int byte = (index) / 8;
    char bit_position = (char)((index) % 8);
    bit_array[byte] &= ~(1 << (7 - bit_position));
}

bool ReadBit(const unsigned char* bit_array, unsigned int index) {
    unsigned int byte = (index) / 8;
    unsigned char bit_position = (unsigned)((index) % 8);
    return bit_array[byte] & (1 << (7 - bit_position));
}

void CalcCompressedTreeSize(Node* root, int* tree_size) {
    if (!IsLeaf(root)) {
        *tree_size = *tree_size + 1;
        if (root->left) {
            CalcCompressedTreeSize(root->left, tree_size);
        }
        if (root->right) {
            CalcCompressedTreeSize(root->right, tree_size);
        }
    } else {
        *tree_size = *tree_size + 1;
        *tree_size += 8;
    }
}

void WriteCompressedTree(Node* root, unsigned char* bit_array, int* index) {
    if (!IsLeaf(root)) {
        WriteZero(bit_array, *index);
        *index = (*index + 1);
        if (root->left) {
            WriteCompressedTree(root->left, bit_array, index);
        }
        if (root->right) {
            WriteCompressedTree(root->right, bit_array, index);
        }
    } else {
        WriteBit(bit_array, *index);
        *index = (*index + 1);
        for (int i = 0; i < 8; ++i) {
            if (ReadBit(&root->data, i)) {
                WriteBit(bit_array, *index);
            } else {
                WriteZero(bit_array, *index);
            }
            *index = (*index + 1);
        }
    }
}

// Clear "non-array" tree
void FreeTree(Node* root) {
    if (!root)
        return;
    FreeTree(root->left);
    FreeTree(root->right);
    free(root);
}

void EncodeText(int unique_symbols, Codes codes_alphabet, const unsigned char* data) {
    unsigned char read_txt_byte = 0;
    int current_txt_byte = 0;
    unsigned char encoded_byte = 0;
    if (unique_symbols == 1) {
        codes_alphabet.size[data[0]] = 1;
        codes_alphabet.matrix[data[0]][0] = 0;
    }
    while (fread(&read_txt_byte, 1, 1, in)) {
        for (int i = 0; i < codes_alphabet.size[read_txt_byte]; i++) {
            int new_bit = codes_alphabet.matrix[read_txt_byte][i];
            if (new_bit) {
                WriteBit(&encoded_byte, current_txt_byte);
            } else {
                WriteZero(&encoded_byte, current_txt_byte);
            }
            current_txt_byte++;
            if (current_txt_byte == 8) {
                fprintf(out, "%c", encoded_byte);
                current_txt_byte = 0;
                encoded_byte = 0;
            }
        }
    }
    if (current_txt_byte) {
        fprintf(out, "%c", encoded_byte);
    }
    fseek(out, 0, SEEK_SET);
    fprintf(out, "%c", current_txt_byte + 1);
}

void FreeData(Node* root,
              Queue* minHeap,
              int* freq,
              unsigned char* data,
              unsigned char* compressed_tree) {
    FreeTree(root);
    for (int i = 0; i < minHeap->size; ++i) {
        free(minHeap->array[i]);
    }
    free(minHeap->array);
    free(minHeap);
    free(freq);
    free(data);
    free(compressed_tree);
}

void CountFreq(int temp_table[ASCII_SIZE], int* unique_symbols) {
    unsigned char symbol = 0;
    int read_symbol = (int)fread(&symbol, 1, 1, in);
    if (!read_symbol) {
        exit(0);
    }
    while (read_symbol) {
        if (!temp_table[(int)symbol])
            (*unique_symbols)++;
        temp_table[(int)symbol]++;
        read_symbol = (int)fread(&symbol, 1, 1, in);
    }
}

void TransferFreqNData(const int temp_table[], unsigned char* data, int* freq) {
    int current_unique_symbol = 0;
    for (int i = 0; i < ASCII_SIZE; ++i) {
        if (temp_table[i]) {
            data[current_unique_symbol] = (unsigned char)i;
            freq[current_unique_symbol] = temp_table[i];
            current_unique_symbol++;
        }
    }
}

void Encode() {
    int unique_symbols = 0;
    int temp_table[ASCII_SIZE] = {0};
    CountFreq(temp_table, &unique_symbols);

    unsigned char* data =
            (unsigned char*)malloc(sizeof(unsigned char) * unique_symbols);
    int* freq = (int*)malloc((sizeof(int) * unique_symbols));
    TransferFreqNData(temp_table, data, freq);

    Codes codes_alphabet;
    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 100; ++j) {
            codes_alphabet.matrix[i][j] = -1;
        }
    }

    Queue* minHeap = InitializeHeap(data, freq, unique_symbols);
    Node* root = HuffmanCodes(&codes_alphabet, minHeap);

    int tree_size = 0;
    CalcCompressedTreeSize(root, &tree_size);
    tree_size = (tree_size + (8 - tree_size % 8) % 8) / 8;
    unsigned char* compressed_tree = calloc(tree_size + 1, 1);
    int index = 0;

    fseek(out, 1, SEEK_SET);
    WriteCompressedTree(root, compressed_tree, &index);
    for (int i = 0; i < tree_size; ++i) {
        fprintf(out, "%c", compressed_tree[i]);
    }

    fseek(in, 1, SEEK_SET);
    EncodeText(unique_symbols, codes_alphabet, data);

    FreeData(root, minHeap, freq, data, compressed_tree);
}

int ReadBuffer(unsigned char buffer[MAX_BUFFER_SIZE]) {
    return (int)fread(buffer, 1, MAX_BUFFER_SIZE, in);
}

Node* RecoverTree(unsigned char bitArray[MAX_BUFFER_SIZE], int* index) {
    if (ReadBit(bitArray, *index)) {
        (*index)++;
        unsigned char character = 0;
        character |= (char)(bitArray[(int)*index / 8] << (*index % 8));
        (*index) += 8;
        character |= (char)(bitArray[(int)*index / 8] >> (8 - (*index % 8)));
        Node* node = malloc(sizeof(Node));
        *node = (Node){character, character, NULL, NULL};
        return node;
    } else {
        (*index)++;
        Node* left = RecoverTree(bitArray, index);
        Node* right = RecoverTree(bitArray, index);

        Node* node = malloc(sizeof(Node));
        *node = (Node){'$', 0, left, right};

        return node;
    }
}

void DecodeText(Node* root, Node* currentNode, int extra_bits, int* index) {
    unsigned char read_byte, next_byte;
    if (!fscanf(in, "%c", &read_byte))
        return;
    if (!extra_bits) {
        extra_bits = 8;
    }
    bool read = true;
    while (read) {
        int bits_to_write;
        if (fscanf(in, "%c", &next_byte) != EOF) {
            bits_to_write = 8;
        } else {
            bits_to_write = extra_bits;
            read = false;
        }
        for (int i = 0; i < bits_to_write; (*index)++, i++) {
            if (IsLeaf(root)) {
                fprintf(out, "%c", currentNode->data);
            } else {
                if ((unsigned char)(read_byte << (*index % 8)) &
                    (unsigned char)(1 << 7)) {
                    currentNode = currentNode->right;
                } else {
                    currentNode = currentNode->left;
                }
                if (IsLeaf(currentNode)) {
                    fprintf(out, "%c", currentNode->data);
                    currentNode = root;
                }
            }
        }
        read_byte = next_byte;
    }
}

void Decode() {
    unsigned char buffer[MAX_BUFFER_SIZE];
    unsigned char extra_bits;
    if (!fread(&extra_bits, 1, 1, in)) {
        return;
    }

    extra_bits -= 1;

    int read_buffer = ReadBuffer(buffer);
    if (!read_buffer)
        return;
    int index = 0;
    Node* root = RecoverTree(buffer, &index);

    fseek(in, (long long int)(2 + ceil((double)index / 8)), SEEK_SET);

    index = 0;
    DecodeText(root, root, extra_bits, &index);
    FreeTree(root);
}

int main() {
    in = fopen("in.txt", "r");
    out = fopen("out.txt", "w");
    char mode;
    if (!fscanf(in, "%c", &mode)) {
        return 0;
    }
    if (mode == 'c') {
        Encode();
    }
    if (mode == 'd') {
        Decode();
    }
}
