#pragma once
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

using namespace std;

void writeResult(const string& filename, const vector<int>& component);

vector<int> bfsComponent(const vector<vector<int>>& graph, int start, int n);

vector<vector<int>> readGraph(const string& filename, int& n, int& m, int& start);