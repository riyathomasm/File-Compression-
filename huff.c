#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100

// Huffman tree node
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// MinHeap structure
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

// Create a new Huffman tree node
struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// Swap function for heap
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// MinHeapify function
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx, left = 2 * idx + 1, right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;
    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Extract the minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);
    return temp;
}

// Insert a new node into the min heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    int i = minHeap->size++;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// Build Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = size;
    minHeap->capacity = size;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    while (minHeap->size != 1) {
        struct MinHeapNode* left = extractMin(minHeap);
        struct MinHeapNode* right = extractMin(minHeap);
        struct MinHeapNode* top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

// Print Huffman codes
void printCodes(struct MinHeapNode* root, int arr[], int top, FILE *treeFile) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1, treeFile);
    }
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1, treeFile);
    }
    if (!(root->left) && !(root->right)) {
        fprintf(treeFile, "%c %d\n", root->data, root->freq);
        printf("%c: ", root->data);
        for (int i = 0; i < top; i++) printf("%d", arr[i]);
        printf("\n");
    }
}

// Read text from file and count character frequencies
void readFileAndBuildHuffman(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    int freq[256] = {0};
    char ch;
    while ((ch = fgetc(fp)) != EOF)
        freq[(unsigned char)ch]++;

    fclose(fp);

    int size = 0;
    char data[256];
    int newFreq[256];

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            data[size] = (char)i;
            newFreq[size] = freq[i];
            size++;
        }
    }

    struct MinHeapNode* root = buildHuffmanTree(data, newFreq, size);

    FILE *treeFile = fopen("tree_output.txt", "w");
    int arr[MAX_TREE_HT];
    printCodes(root, arr, 0, treeFile);
    fclose(treeFile);

    printf("Huffman tree saved to tree_output.txt\n");
}

int main() {
    char filename[100];
    printf("Enter filename containing text: ");
    scanf("%s", filename);
    readFileAndBuildHuffman(filename);
    return 0;
}
