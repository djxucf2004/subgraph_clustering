#include "subgraph_clustering.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace subgraph {

SubgraphClustering::SubgraphClustering(const std::string& filename) {
  adjacent_matrix_.Reset();
  ReadMatrix(filename, &adjacent_matrix_);
}

SubgraphClustering::~SubgraphClustering() {}

void SubgraphClustering::Transform(const std::string& filename,
                                   Matrix* adjacent_matrix_B,
                                   std::vector<int>* weights_C) {
  // Read the transform
  Matrix transform;
  if (!ReadMatrix(filename, &transform)) {
    std::cout << "Unable to read the transform matrix." << std::endl;
    return;
  }

  // Step 1 - Process the transform matrix
  const int kCols = transform.cols;
  const int kRows = transform.rows;
  std::vector<std::vector<int> > h(kCols);
  for (int j = 0; j < kCols; ++j) {
    h[j].clear();
    for (int i = 0; i < kRows; ++i) {
      int pos = i*kCols + j;
      if (transform.data[pos] == 1) {
        h[j].push_back(i);
      }
    }
  }

  // Step 2 - calculate the output adjacency matrix B
  const int kSizeB = kCols*kCols;
  // Initialize the matrix
  adjacent_matrix_B->data.resize(kSizeB, 0);
  adjacent_matrix_B->rows = kCols;
  adjacent_matrix_B->cols = kCols;
  for (int i = 0; i < kCols; ++i) {
    for (int j = 0; j < kCols; ++j) {
      int pos = i*kCols + j;
      if (j > i) {
        // Calculate the weights between the nodes h[i] and h[j]
        adjacent_matrix_B->data[pos] = GetAjacencyWeights(h[i], h[j]);
      } else {  // j <= i
        int sym_pos = j*kCols + i;
        // The matrix is symmetric
        adjacent_matrix_B->data[pos] = adjacent_matrix_B->data[sym_pos];
      }
    }
  }

  // Step 3 - calculate the output weights C matrix
  weights_C->resize(kCols, 0);
  for (int i = 0; i < kCols; ++i) {
    weights_C->at(i) = GetCWeights(h[i]);
  }
}

void SubgraphClustering::Display() {
  for (int i = 0; i < adjacent_matrix_.rows; ++i) {
    for (int j = 0; j < adjacent_matrix_.cols; ++j) {
      int pos = adjacent_matrix_.cols * i + j;
      printf("%d, ", adjacent_matrix_.data[pos]);
    }
    printf("\n");
  }
}

bool SubgraphClustering::ReadMatrix(const std::string& filename,
                                    Matrix* matrix) {
  std::vector<int>& data = matrix->data;
  int rows = 0;
  std::vector<int> vcols;
  std::ifstream infile(filename.c_str(), std::ifstream::in);
  while (infile) {
    std::string s;
    if (!std::getline(infile, s)) {
      break;
    }
    std::istringstream ss(s);
    std::vector<int> record;
    int count = 0;
    while(ss) {
      if (!std::getline(ss, s, ',')) {
        break;
      }
      data.push_back(std::stoi(s));
      count++;
    }
    vcols.push_back(count);
    if (vcols.size() > 1) {
      if (vcols.front() != vcols.back()) {
        std::cout << "Matrix data are not correct" << std::endl;
        return false;
      }
    }
    // Update the rows
    rows++;
  }
  matrix->rows = rows;
  matrix->cols = vcols.front();
  return true;
}

int SubgraphClustering::GetAjacencyWeights(const std::vector<int>& hr,
                                           const std::vector<int>& hc) {
  int weights = 0;
  const int kRows = static_cast<int>(hr.size());
  const int kCols = static_cast<int>(hc.size());
  for (int i = 0; i < kRows; ++i) {
    for (int j = 0; j < kCols; ++j) {
      int pos = hr[i]*adjacent_matrix_.cols + hc[j];
      weights += adjacent_matrix_.data[pos];
    }
  }
  return weights;
}

int SubgraphClustering::GetCWeights(const std::vector<int>& h) {
  int weights = 0;
  int kSize = static_cast<int>(h.size());
  for (int i = 0; i < kSize-1; ++i) {
    for (int j = 0; j < kSize; ++j) {
      int pos = h[i]*adjacent_matrix_.cols + h[j];
      weights += adjacent_matrix_.data[pos];
    }
  }
  return weights;
}

}  // namespace subgraph
