#pragma once
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

void writeResult(const string& filename, const vector<int>& component);

vector<int> bfsComponent(const vector<vector<int>>& graph, int start, int n);

vector<vector<int>> readGraph(const string& filename, int& n, int& m, int& start);

vector<vector<int>> findGraphComponents(const vector<vector<int>>& graph, int n);

void writeComponents(const string& filename, const vector<vector<int>>& components);

void writeAdjacencyList(const string& filename, const vector<vector<int>>& graph);

void mergeGraphs(const string& filename);
