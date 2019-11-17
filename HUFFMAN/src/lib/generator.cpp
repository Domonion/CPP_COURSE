//
// Created by kekosik on 13.05.18.
//

#include <algorithm>
#include <iostream>
#include "headers/generator.h"

generator::generator(OccurrencyMap const &a) {
    textSize = 0;
    for (int i = 0; i < 256; i++)
        if (a.get(i) != 0)
            tree.emplace_back(i, a.get(i)), textSize += a.get(i);

    //if file empty
    if (tree.size() == 0) {
        return;
    }

    //if file consist only 1 type of bit
    if (tree.size() == 1) {
        uchar newByte = (tree.back().byte ? 0 : 1);
        tree.emplace_back(newByte, 0);
    }

    //building tree
    std::vector<int> treeBuilder(tree.size());
    for (size_t i = 0; i < tree.size(); i++)
        treeBuilder[i] = i;
    comparator comp(tree);
    while (treeBuilder.size() != 1) {
        sort(treeBuilder.begin(), treeBuilder.end(), comp);
        tree.emplace_back(treeBuilder[treeBuilder.size() - 1], treeBuilder[treeBuilder.size() - 2],
                          tree[treeBuilder[treeBuilder.size() - 1]].weight + tree[treeBuilder[treeBuilder.size() - 2]].weight);
        treeBuilder.pop_back();
        treeBuilder.pop_back();
        treeBuilder.emplace_back(tree.size() - 1);
    }
    defaultNow = tree.size() - 1;
    indNow = defaultNow;
    dfsCodes(defaultNow, std::vector<bool>());
}

uint64_t generator::getTextSize() const {
    return textSize;
}

uint16_t generator::getTreeSize() const {
    return static_cast<uint16_t>(tree.size());
}

std::vector<uchar> generator::encodeTree() const {
    if (textSize == 0) {
        return std::vector<uchar>();
    }
    std::vector<bool> dfsRes = dfsTree(defaultNow);
    std::vector<uchar> res;
    uchar now = 0;
    int j = 0;
    for (int i = 0; i < dfsRes.size(); i++) {
        if (j == 8) {
            res.emplace_back(now);
            now = 0;
            j = 0;
        }
        j++;
        now <<= 1;
        now += dfsRes[i];
    }
    if (now != 0)
        res.emplace_back(now << (8 - j));
    return res;
}

std::vector<bool> generator::dfsTree(int v) const {
    std::vector<bool> res;
    if (!tree[v].isLeave) {
        res.push_back(0);
        std::vector<bool> resl = dfsTree(tree[v].l);
        res.insert(res.end(), resl.begin(), resl.end());
        resl = dfsTree(tree[v].r);
        res.insert(res.end(), resl.begin(), resl.end());
    } else
        res.emplace_back(1);
    return res;
}

std::vector<uchar> generator::encodeTreeLeaves() const {
    if (textSize == 0) {
        return std::vector<uchar>();
    }
    return dfsLeaves(defaultNow);
}

std::vector<uchar> generator::dfsLeaves(int v) const {
    std::vector<uchar> res;
    if (!tree[v].isLeave) {
        std::vector<uchar> resl = dfsLeaves(tree[v].l);
        res.insert(res.end(), resl.begin(), resl.end());
        resl = dfsLeaves(tree[v].r);
        res.insert(res.end(), resl.begin(), resl.end());
    } else
        res.emplace_back(tree[v].byte);
    return res;
}

std::vector<bool> const &generator::getCode(uchar value) const {
    return codes[value];
}

void generator::dfsCodes(int v, std::vector<bool> code) {
    if (!tree[v].isLeave) {
        code.emplace_back(0);
        dfsCodes(tree[v].l, code);
        code.back() = 1;
        dfsCodes(tree[v].r, code);
    } else
        codes[tree[v].byte] = code;
}

void generator::generate(size_t n, std::vector<uchar> codedTree, std::vector<uchar> codedTreeLeaves, uint64_t length) {
    textSize = length;
    tree.emplace_back(0, 0, 0);
    indNow = 0;
    std::vector<bool> decodedTree;
    if (n > codedTree.size() * 8)
        throw std::runtime_error("Incorrect tree size");
    if ((n + 1) / 2 != codedTreeLeaves.size())
        throw std::runtime_error("Incorrect leaves number");
    for (size_t i = 0; i < n; i++) {
        decodedTree.emplace_back((codedTree[i / 8] >> (7 - (i % 8))) % 2);
    }
    int a = 0;
    int b = 0;
    int c = 0;
    try {
        dfsGenerator(a, b, c, decodedTree, codedTreeLeaves);
    }
    catch (...) {
        throw std::runtime_error("Leaves does not match with tree");
    }
    indNow = 0;
    defaultNow = indNow;
}

bool generator::getReverseCode(bool way, char &res) {
    indNow = tree[indNow].r * way + tree[indNow].l * (!way);
    if (tree[indNow].isLeave) {//~~~~
        res = tree[indNow].byte;
        indNow = defaultNow;
        return true;
    }
    return false;
}
//bool generator::getReverseCode(bool way, char &res) {
//    indNow = tree[indNow].r * way + tree[indNow].l * (!way);
//    res = tree[indNow].byte;
//    indNow = defaultNow * tree[indNow].isLeave + indNow * (!tree[indNow].isLeave);
//    return (indNow == defaultNow);
//}

void generator::dfsGenerator(int v, int &codedTreeInd, int &codedTreeLeavesInd, std::vector<bool> const &codedTree, std::vector<uchar> const &codedTreeLeaves) {
    if (textSize == 0) {
        return;
    }
    if(codedTreeInd == codedTree.size() || codedTreeLeavesInd == codedTreeLeaves.size())
        throw std::runtime_error("");
    if (codedTree[codedTreeInd++] == 1) {
        tree[v].isLeave = 1;
        tree[v].byte = codedTreeLeaves[codedTreeLeavesInd++];
        tree[v].l = tree[v].r = -1;
    } else {
        tree[v].l = tree.size();
        tree.emplace_back(0, 0, 0);
        dfsGenerator(tree[v].l, codedTreeInd, codedTreeLeavesInd, codedTree, codedTreeLeaves);
        tree[v].r = tree.size();
        tree.emplace_back(0, 0, 0);
        dfsGenerator(tree[v].r, codedTreeInd, codedTreeLeavesInd, codedTree, codedTreeLeaves);
    }
}

//void generator::dfs(int v) {
//    if(!tree[v].isLeave){
//        std::cout << v << ' ' << tree[v].l << std::endl;
//        std::cout << v << ' ' << tree[v].r << std::endl;
//        dfs(tree[v].l);
//        dfs(tree[v].r);
//    }
//    else
//    {
//        std::cout << tree[v].byte << std::endl;
//    }
//}
//
//void generator::writeGenerator() {
//    dfs(0);
//}

