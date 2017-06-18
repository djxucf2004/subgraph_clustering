#include "subgraph_clustering.h"

#include <string>
#include <iostream>
#include <fstream>
#include <memory>

using subgraph::SubgraphClustering;
using subgraph::Matrix;

std::string A_matrix_filename;
std::string T_matrix_filename;
std::string B_matrix_filename;
std::string c_vector_filename;
std::shared_ptr<SubgraphClustering> subgraph_clustering;

int main(int argc, char** argv) {
  // reset
  A_matrix_filename.clear();
  T_matrix_filename.clear();
  // output by default
  B_matrix_filename = "B.csv";
  c_vector_filename = "C.csv";

  // parse arguments
  {
    if (argc < 3) {
      std::cout << "Both adjacency matrix A and transform matrix T are required"
                << std::endl;
      return -1;
    }

    A_matrix_filename = std::string(argv[1]);
    T_matrix_filename = std::string(argv[2]);

    if (argc >= 4) {
      B_matrix_filename = std::string(argv[3]);
    }

    if (argc >= 5) {
      c_vector_filename = std::string(argv[4]);
    }
  }

  subgraph_clustering.reset(new SubgraphClustering(A_matrix_filename));

  // Display row number
  std::cout << "Adjacency matrix rows= " << subgraph_clustering->GetRows()
            << std::endl;
  // Display column number
  std::cout << "Adjacency matrix cols= " << subgraph_clustering->GetCols()
            << std::endl;
  // Display the data
  subgraph_clustering->Display();

  // Apply the transfom, calculate the adjacency matrix and the weights vector
  Matrix B_matrix;
  std::vector<int> C_vector;
  subgraph_clustering->Transform(T_matrix_filename, &B_matrix, &C_vector);

  // Display B_matrix and save to file
  FILE *fp = fopen(B_matrix_filename.c_str(), "w");
  printf("\nB_matrix\n");
  for (int i = 0; i < B_matrix.rows; ++i) {
    for (int j = 0; j < B_matrix.cols; ++j) {
      int pos = i*B_matrix.cols + j;
      printf("%d, ", B_matrix.data[pos]);
      if (j == B_matrix.cols-1) {
        fprintf(fp, "%d\n", B_matrix.data[pos]);
      } else {
        fprintf(fp, "%d,", B_matrix.data[pos]);
      }
    }
    printf("\n");
  }
  fclose(fp);

  // Display C_vector and save to file
  fp = fopen(c_vector_filename.c_str(), "w");
  printf("\nC_vector\n");
  for (size_t i = 0; i < C_vector.size(); ++i) {
    if (i == C_vector.size()-1) {
      fprintf(fp, "%d", C_vector[i]);
    } else {
      fprintf(fp, "%d,", C_vector[i]);
    }
    printf("%d, ", C_vector[i]);
  }
  printf("\n");
  fclose(fp);

  return 0;
}
