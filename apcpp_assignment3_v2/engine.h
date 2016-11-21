#pragma once

#include <array>
#include <list>


struct Particle;

const auto g_cellColumns = 16u;
const auto g_cellRows    =  9u;

void initialize();
void simulate();

using cells_t = std::array<float, g_cellRows * g_cellColumns>;
const cells_t & cells();

using particles_t = std::list<Particle *>;
const particles_t & particles();
