#ifndef _MY_DIJKSTRA_H_
#define _MY_DIJKSTRA_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
#include <stack>
#include <utility>
typedef unsigned int NodeType;
typedef unsigned int WeightType;
typedef std::vector<std::vector<std::pair<NodeType, WeightType>>> GraphType; 
//graph as adjacent list
/*------------------------------------------------------------------------------
ShortestPath_Dijkstra
Find and print the shortest path from source to end using the Dijkstra's
algorithm
Vairables:
- graph: the input graph
- source: the source node
- end: the target node
- path_len: the the summation of weights of all edges in the shortest path
- path: the shortest path represented as a list of nodes
------------------------------------------------------------------------------*/
void ShortestPath_Dijkstra(
const GraphType& graph,
const NodeType & source,
const NodeType & end,
WeightType & path_len,
std::vector<NodeType> & path)
{
/*------ CODE BEGINS ------*/
    
    const size_t node_number = graph.size();
    std::vector<WeightType> distance(node_number,
    std::numeric_limits<WeightType>::max());

    distance[source] = 0;
    std::vector<NodeType> predecessor(node_number, 0);
    std::vector<bool> processed(node_number, false);
    std::priority_queue<std::pair<WeightType, NodeType>,
    std::vector<std::pair<WeightType, NodeType>>,
    std::greater<std::pair<WeightType, NodeType>>>
    pqueue;
    pqueue.push(std::make_pair(0, source));
    for (;!pqueue.empty();)
    {
        NodeType u = pqueue.top().second;
        pqueue.pop();
        if (processed[u])
        {
            continue;
        }
        processed[u] = true;
        for (const auto &neighbor : graph[u])
        {
            NodeType v = neighbor.first;
            WeightType w = neighbor.second;
            if (!processed[v] && distance[u] + w < distance[v])
            {
                distance[v] = distance[u] + w;
                predecessor[v] = u;
                pqueue.push(std::make_pair(distance[v], v));
            }
        }
        if (u == end)
        {
            break;
        }
    }
    path.clear();
    NodeType now = end;
    for(;now != source;)
    {
    path.push_back(now);
    now = predecessor[now];
    }
    path.push_back(source);
    std::reverse(path.begin(), path.end());
/*------ CODE ENDS ------*/
}
#endif
