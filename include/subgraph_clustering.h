#ifndef SUBGRAPH_CLUSTERING_H_
#define SUBGRAPH_CLUSTERING_H_

#include <string>
#include <vector>
#include <memory>

namespace subgraph {

struct Matrix {
  Matrix() {
   Reset();
  }

  void Reset() {
    rows = 0;
    cols = 0;
    data.clear();
  }

  std::vector<int> data;
  int rows;
  int cols;
};

class SubgraphClustering {
public:
  SubgraphClustering(const std::string& adj_matrix_fname);
  ~SubgraphClustering();

  void Transform(const std::string& trans_fname, Matrix* adjacent_matrix_B,
                 std::vector<int>* weights_C);

  void Display();

  inline int GetRows() {
    return adjacent_matrix_.rows;
  }

  inline int GetCols() {
    return adjacent_matrix_.cols;
  }

private:
  bool ReadMatrix(const std::string& matrix_filename, Matrix* matrix);
  int GetAjacencyWeights(const std::vector<int>& h1, const std::vector<int>& h2);
  int GetCWeights(const std::vector<int>& h);

  Matrix adjacent_matrix_;
};

}  // namespace subgraph

#endif  // SUBGRAPH_CLUSTERING_H_
