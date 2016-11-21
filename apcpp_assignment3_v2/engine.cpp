#include "engine.h"


#include <algorithm>
#include <random>
#include <chrono>

#include <QtGui/QVector2D>

#include "particle.h"


namespace
{
    const auto g_energy = 1.f;
    auto g_time = time_point{};

    const auto g_numCellsF = std::array<float, 2>{
        static_cast<float>(g_cellColumns), static_cast<float>(g_cellRows) };

    auto g_cells = cells_t{};
    auto g_particles = particles_t{};

    std::random_device seed;
    auto engine = std::mt19937_64{ seed() };
    auto real_dist = std::uniform_real_distribution<float>{ 0.f, 1.f };
    auto cell_dist = std::uniform_int_distribution<>{ 0, g_cellColumns * g_cellRows - 1 };
    auto time_dist = std::uniform_int_distribution<>{ 512, 4096 };
    auto part_dist = std::uniform_real_distribution<float>{ 0.0000002f, 0.000008f };
}

const cells_t & cells()
{
    return g_cells;
}

const particles_t & particles()
{
    return g_particles;
}

void initialize()
{
    // setup random cell energy by assigning energy quota until energy is depleted
    std::fill(g_cells.begin(), g_cells.end(), 0.f);

    auto energy = g_energy;
    while(energy > 0.f)
    {
        const auto e = real_dist(engine) * 0.001f;

        energy -= e;
        g_cells[cell_dist(engine)] += e;
    }

    g_time = std::chrono::high_resolution_clock::now();
}

void spawn()
{
    for(auto y = 0u; y < g_cellRows; ++y)
        for(auto x = 0u; x < g_cellColumns; ++x)
        {
            const auto i = y * g_cellColumns + x;
            auto & cell = g_cells[i];

            if(real_dist(engine) > cell)
                continue;

            while(cell > part_dist.max())
            {
                auto particle = new Particle();
                if(!particle)
                    break;

                particle->position.setX(x + real_dist(engine));
                particle->position.setY(y + real_dist(engine));
                particle->velocity.setX(real_dist(engine) * 2.0f - 1.0f);
                particle->velocity.setY(real_dist(engine) * 2.0f - 1.0f);
                particle->velocity.normalize();
                particle->velocity *= 1.f;
                particle->tod = g_time + std::chrono::milliseconds(time_dist(engine));
                particle->energy = part_dist(engine);

                cell -= particle->energy;
                g_particles.push_back(particle);
            }
        }
}

void simulate()
{
    spawn();

    const auto t0 = g_time;
    g_time = std::chrono::high_resolution_clock::now();

    const auto elapsed = secs(g_time - t0).count();
    const auto elapsed2 = elapsed * elapsed;

    for (auto it = g_particles.begin(); it != g_particles.end(); ++it)
    {
        auto particle = *it;

        auto & p = particle->position;
        auto & v = particle->velocity;

        //const auto f = gravity - v * friction;
        const auto f = v;

        p += (v * elapsed) + (0.5f * f * elapsed2);
        //v += (f * elapsed);

        if (p.y() < 0.f && v.y() < 0.f)
        {
            p.setY(p.y() + g_numCellsF[1]);
        }
        else if(p.y() > g_numCellsF[1] && v.y() > 0.f)
        {
            p.setY(p.y() - g_numCellsF[1]);
        }

        if (p.x() < 0.f && v.x() < 0.f)
        {
            p.setX(p.x() + g_numCellsF[0]);
        }
        else if(p.x() > g_numCellsF[0] && v.x() > 0.f)
        {
            p.setX(p.x() - g_numCellsF[0]);
        }
    }

    for (auto it = g_particles.begin(); it != g_particles.end(); )
    {
        auto particle = *it;
        if(g_time < particle->tod)
        {
            ++it;
            continue;
        }

        // identify underlying cell
        const auto i = qFloor(particle->position.y()) * g_cellColumns
            + qFloor(particle->position.x());

        // add energy underlying cell
        g_cells[i] += particle->energy;

        // remove particle
        delete particle;
        it = g_particles.erase(it);
    }
}
