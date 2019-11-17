// Created by kekosik on 13.05.18.
//

#ifndef HUFFMAN_GENERATOR_H
#define HUFFMAN_GENERATOR_H

#include <constants.h>
#include <vector>
#include <OccurrencyMap.h>

class generator {
public:

    generator(){};

    generator(OccurrencyMap const &a);

    uint16_t getTreeSize() const ;

    uint64_t getTextSize() const ;

    std::vector<uchar> encodeTree() const;

    std::vector<uchar> encodeTreeLeaves() const;

    std::vector<bool> const& getCode(uchar value) const;

    bool getReverseCode(bool way, char & res);

    void generate(size_t n, std::vector<uchar> codedTree, std::vector<uchar> codedTreeLeaves,uint64_t length);
//
//    void writeGenerator();

private:

    size_t indNow;
    size_t defaultNow;

    struct node{
        int l, r;
        uint64_t weight;
        bool isLeave;
        uchar byte;
        node(uchar _byte, uint64_t _weight) : l(-1), r(-1), isLeave(true), weight(_weight), byte(_byte){}
        node(int _l, int _r, uint64_t _weight) :l(_l), r(_r),isLeave(false), weight(_weight), byte(-1){}
    };

    std::vector<node> tree;
    std::vector<bool> codes[256];

    uint64_t textSize;

    struct comparator{
        std::vector<node> const& comparatorTree;
        comparator(const std::vector<node> & tree) : comparatorTree(tree){}
        bool operator()(int i, int j) const{
            return comparatorTree[i].weight > comparatorTree[j].weight;
        }
    };

    std::vector<bool> dfsTree(int v) const;

    std::vector<uchar> dfsLeaves(int v) const;

    void dfsCodes(int v,std::vector<bool> code);

    void dfsGenerator(int v, int & codedTreeInd, int & codedTreeLeavesInd, std::vector<bool> const & codedTree, std::vector<uchar> const & codedTreeLeaves);
//
//    void dfs(int v);
};

#endif //HUFFMAN_GENERATOR_H
