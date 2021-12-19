//
// Created by alex on 21.11.2021.
//

#ifndef PARSER_EXECUTABLE_H
#define PARSER_EXECUTABLE_H


#include "File.h"
#include <list>
#include <utility>
#include <vector>
#include <memory>
#include <LIEF/LIEF.hpp>

struct Node;
struct DependencyGraph;

class Executable : public File {
    std::unique_ptr<LIEF::MachO::FatBinary> macho;
public:
    Executable() = default;
    explicit Executable(std::string);
    int read();
    ~Executable() override = default;
};

//struct Node {
//    Executable vertex;
//    unsigned int idx;
//    Node(Executable e, unsigned int i) {
//        this->vertex = std::move(e);
//        this->idx = i;
//    }
//};
//
//class DependencyGraph {
//    unsigned int numVertex;
//    std::vector<std::list<Node>> adjList;
//public:
//    DependencyGraph() { this->numVertex = 0; }
//    void addEdge(Node & src, Node & dest) {
//        adjList[src.idx].push_front(dest);
//    }
//    void addVertex(const Executable & e) {
//        Node n = Node(e, numVertex);
//        numVertex++;
//        adjList.emplace_back(std::list<Node>(1, n));
//    }
//};


#endif //PARSER_EXECUTABLE_H
