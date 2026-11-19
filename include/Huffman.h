
#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <string>
using namespace std;

// Represents one node in the Huffman Tree
struct HuffmanNode {
    char character;
    int frequency;

    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char ch, int freq) {
        character = ch;
        frequency = freq;

        left = nullptr;
        right = nullptr;
    }
};


// Comparator for the Min Priority Queue
struct CompareFrequency {

    bool operator()(HuffmanNode* a, HuffmanNode* b) {

        return a->frequency > b->frequency;
    }
};




HuffmanNode* buildHuffmanTree(string text) {

    // Step 1: Count frequency of every character
    int frequency[256] = {0};

    for (int i = 0; i < text.length(); i++) {
        unsigned char ch = text[i];
        frequency[ch]++;
    }

    // Step 2: Create a Min Priority Queue
    priority_queue<
        HuffmanNode*,
        vector<HuffmanNode*>,
        CompareFrequency
    > minHeap;

    // Step 3: Create a node for every character
    for (int i = 0; i < 256; i++) {

        if (frequency[i] > 0) {

            HuffmanNode* node =
                new HuffmanNode((char)i, frequency[i]);

            minHeap.push(node);
        }
    }

    // Step 4: Combine the two smallest frequency nodes
    while (minHeap.size() > 1) {

        HuffmanNode* left = minHeap.top();
        minHeap.pop();

        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        // Create a new parent node
        HuffmanNode* parent =
            new HuffmanNode('\0',
                            left->frequency + right->frequency);

        parent->left = left;
        parent->right = right;

        // Insert the parent back into the heap
        minHeap.push(parent);
    }

    // Step 5: The remaining node is the root
    if (minHeap.empty()) {
        return nullptr;
    }

    return minHeap.top();
}




void generateHuffmanCodes(
    HuffmanNode* root,
    string code,
    string codes[]
) {

    // Base case: If tree is empty
    if (root == nullptr) {
        return;
    }

    // If this is a leaf node, store its code
    if (root->left == nullptr && root->right == nullptr) {

        codes[(unsigned char)root->character] = code;
        return;
    }

    // Move left and add 0
    generateHuffmanCodes(root->left, code + "0", codes);

    // Move right and add 1
    generateHuffmanCodes(root->right, code + "1", codes);
}



string encodeText(string text, string codes[]) {

    string encodedText = "";

    // Visit every character in the original text
    for (int i = 0; i < text.length(); i++) {

        unsigned char ch = text[i];

        // Add its Huffman code
        encodedText += codes[ch];
    }

    return encodedText;
}


string decodeText(string encodedText, HuffmanNode* root) {

    string decodedText = "";

    // If the tree is empty, return empty text
    if (root == nullptr) {
        return decodedText;
    }

    // Start from the root
    HuffmanNode* current = root;

    // Read every bit
    for (int i = 0; i < encodedText.length(); i++) {

        // If bit is 0, move left
        if (encodedText[i] == '0') {
            current = current->left;
        }

        // If bit is 1, move right
        else {
            current = current->right;
        }

        // If we reach a leaf node
        if (current->left == nullptr &&
            current->right == nullptr) {

            decodedText += current->character;

            // Start again from the root
            current = root;
        }
    }

    return decodedText;
}
