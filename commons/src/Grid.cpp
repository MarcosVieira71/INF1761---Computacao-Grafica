#include "Grid.h"

#include <cassert>
#include <iostream>

#define INDEX(i,j,nx) ((unsigned int)((j)*(nx+1)+(i)))

GridPtr Grid::Make (int nx, int ny)
{
  return GridPtr(new Grid(nx,ny));
}

Grid::Grid(int nx, int ny)
  : m_nx(nx), m_ny(ny)
{
  // Alocar e preencher coordenadas
  m_coords.reserve(2 * VertexCount());
  float dx = 1.0f / nx;
  float dy = 1.0f / ny;

  for (int j = 0; j <= m_ny; ++j) {
    for (int i = 0; i <= m_nx; ++i) {
      m_coords.push_back(i * dx);
      m_coords.push_back(j * dy);
    }
  }

  // Alocar e preencher índices
  m_indices.reserve(IndexCount());
  for (int j = 0; j < ny; ++j) {
    for (int i = 0; i < nx; ++i) {
      m_indices.push_back(INDEX(i, j, nx));
      m_indices.push_back(INDEX(i + 1, j, nx));
      m_indices.push_back(INDEX(i + 1, j + 1, nx));

      m_indices.push_back(INDEX(i, j, nx));
      m_indices.push_back(INDEX(i + 1, j + 1, nx));
      m_indices.push_back(INDEX(i, j + 1, nx));
    }
  }
}

Grid::~Grid () = default;

int Grid::GetNx ()
{
  return m_nx;
}

int Grid::GetNy ()
{
  return m_ny;
}

int Grid::VertexCount ()
{
  return (m_nx+1)*(m_ny+1);
}

int Grid::IndexCount ()
{
  return 6*m_nx*m_ny;
}

const std::vector<float>& Grid::GetCoords () const
{
  return m_coords;
}

const std::vector<unsigned int>& Grid::GetIndices() const
{
  return m_indices;
}
