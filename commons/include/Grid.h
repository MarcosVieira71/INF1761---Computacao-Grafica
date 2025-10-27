#pragma once

#include <vector>
#include <memory>

class Grid;
using GridPtr = std::shared_ptr<Grid>; 



class Grid {
  int m_nx, m_ny;
  std::vector<float> m_coords;
  std::vector<unsigned int> m_indices;

protected:
  Grid (int nx, int ny);

public:

  static GridPtr Make (int nx, int ny);
  virtual ~Grid ();
  int GetNx ();
  int GetNy ();
  int VertexCount ();
  int IndexCount ();
  
  const std::vector<float>& GetCoords() const;
  const std::vector<unsigned int>& GetIndices() const;
};
