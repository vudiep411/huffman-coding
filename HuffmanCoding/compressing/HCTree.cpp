
#include "HCTree.hpp"
#include <queue>
#include <stack>
using namespace std;

/*
 * Implement the HCTree destructor.
 */
HCTree::~HCTree()
{
    if (root != nullptr)
    {
        //Create empty queue and push root node into queue
        queue<HCNode*> queue;
        queue.push(root);

        //Initialize a front node
        HCNode* front = nullptr;

        //If queue is not empty
        while (!queue.empty())
        {
            front = queue.front();
            queue.pop();

            if (front->c0)
                queue.push(front->c0);

            if (front->c1)
                queue.push(front->c1);

            delete front;
        }

        root = nullptr;
    }
}


/**
 * Use the Huffman algorithm to build a Huffman coding tree.
 * PRECONDITION:  freqs is a vector of ints, such that freqs[i] is the
 *                frequency of occurrence of byte i in the input file.
 * POSTCONDITION: root points to the root of the trie, and leaves[i]
 *                points to the leaf node containing byte i.
 *
 * @param freqs frequency vector
 */
void HCTree::build(const vector<int>& freqs)
{
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> priorityQueue;

    for (unsigned int i = 0; i < freqs.size(); i++)
    {
        if (freqs[i] != 0)
        {
            HCNode* newNode = new HCNode(freqs[i], i);
            leaves[i] = newNode;
            priorityQueue.push(newNode);
        }
    }

    while (priorityQueue.size() > 1)
    {
        HCNode* zeroChild = priorityQueue.top();
        priorityQueue.pop();

        HCNode* oneChild = priorityQueue.top();
        priorityQueue.pop();


        int newNodeCount = zeroChild->count + oneChild->count;
        unsigned char newNodeSymbol = oneChild->symbol;

        HCNode* newNode = new HCNode(newNodeCount, newNodeSymbol);
        newNode->c0 = zeroChild;
        newNode->c1 = oneChild;
        zeroChild->p = newNode;
        oneChild->p = newNode;
        priorityQueue.push(newNode);

    }

    root = priorityQueue.top();
}

/**
 * Write to the given FancyOutputStream the sequence of bits coding the
 * given symbol.
 * PRECONDITION: build() has been called, to create the coding tree,
 *               and initialize root pointer and leaves vector.
 *
 * @param symbol symbol to encode
 * @param out output stream for the encoded bits
 */
void HCTree::encode(unsigned char symbol, FancyOutputStream& out) const
{
    HCNode* currentNode = leaves[symbol];
    stack<unsigned int> bitStack;

    if (currentNode == root)
    {
        out.write_bit(0);
        return;
    }

    while (currentNode != root)
    {
        HCNode* parent = currentNode->p;


        if (currentNode == parent->c0)
            bitStack.push(0);
        
        else if (currentNode == parent->c1)
            bitStack.push(1);
        
        currentNode = parent;
    }

    while (!bitStack.empty())
    {
        out.write_bit(bitStack.top());
        bitStack.pop();
    }
}

void HCTree::serialize(FancyOutputStream& out)
{
    if (root == nullptr)
    {
        out.write<int>(0);
        return;
    }
    out.write<int>(root->count);
    serialize(root, out);
}

void HCTree::serialize(HCNode* curr, FancyOutputStream& out)
{
    if (curr == nullptr)
        return;
    if (curr->c0 == nullptr && curr->c1 == nullptr)
    {
        out.write_bit(1);
        for (int i = 0; i < 8; ++i) {
            if (curr->symbol & (1 << (7 - i))) {
                out.write_bit(1);
            }
            else {
                out.write_bit(0);
            }
        }
    }
    else
    {
        out.write_bit(0);
        serialize(curr->c0, out);
        serialize(curr->c1, out);
    }
}

/**
 * Return symbol coded in the next sequence of bits from the stream.
 * PRECONDITION: build() has been called, to create the coding tree, and
 *               initialize root pointer and leaves vector.
 *
 * @param in input stream to find encoded bits
 * @return a single char decoded from the input stream
 */
unsigned char HCTree::decode(FancyInputStream& in) const
{
    HCNode* currentNode = root;

    while (currentNode->c1 || currentNode->c0)
    {
        auto read_bit = in.read_bit();
        if (read_bit == 0)
            currentNode = currentNode->c0;
        else if (read_bit == 1)
            currentNode = currentNode->c1;
    }

    return currentNode->symbol;
}

void HCTree::deserialize(FancyInputStream& in)
{
    int count = in.read<int>();
    if (count == 0)
    {
        root = nullptr;
        return;
    }

    root = deserializeNode(in);
    root->count = count;
}

HCNode* HCTree::deserializeNode(FancyInputStream& in)
{
    if (in.read_bit() == 0)
    {
        HCNode* curr = new HCNode(0, '0');

        curr->c0 = deserializeNode(in);
        curr->c1 = deserializeNode(in);

        curr->c1->p = curr;
        curr->c0->p = curr;

        return curr;
    }
    else
    {
        unsigned char symbol = 0;
        for (int i = 0; i < 8; i++) {
            symbol = (symbol << 1) | in.read_bit();
        }

        HCNode* curr = new HCNode(0, symbol);

        leaves[curr->symbol] = curr;

        return curr;
    }

}