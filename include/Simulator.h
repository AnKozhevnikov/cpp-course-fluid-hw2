#pragma once

#include <bits/stdc++.h>
using namespace std;

template <typename P, typename V, typename VFLOW, size_t S1, size_t S2>
class Simulator
{
  public:
    static constexpr std::array<pair<int, int>, 4> deltas{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
    static constexpr P inf = P::get_infinity();
    static constexpr P eps = P::get_epsilon();

    template <typename T> struct VectorFieldStatic
    {
        array<T, 4> v[S1][S2];

        VectorFieldStatic() : v()
        {
        }

        T &add(int x, int y, int dx, int dy, T dv)
        {
            return get(x, y, dx, dy) += dv;
        }

        T &get(int x, int y, int dx, int dy)
        {
            size_t i = find(deltas.begin(), deltas.end(), pair(dx, dy)) - deltas.begin();
            assert(i < deltas.size());
            return v[x][y][i];
        }
    };

    int N = S1;
    int M = S2;
    int dirs[S1][S2];
    VectorFieldStatic<V> velocity;
    VectorFieldStatic<VFLOW> velocity_flow;
    P rho[256];
    P p[S1][S2];
    P old_p[S1][S2];
    int last_use[S1][S2];
    int UT;
    char field[S1][S2 + 1];

    Simulator() : velocity(), velocity_flow()
    {
        UT = 0;
    }
    tuple<P, bool, pair<int, int>> propagate_flow(int x, int y, P lim)
    {
        last_use[x][y] = UT - 1;
        P ret = 0;
        for (auto [dx, dy] : deltas)
        {
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] != '#' && last_use[nx][ny] < UT)
            {
                auto cap = velocity.get(x, y, dx, dy);
                auto flow = velocity_flow.get(x, y, dx, dy);
                if (flow == cap)
                {
                    continue;
                }
                auto vp = min(lim, P(cap - flow));
                if (last_use[nx][ny] == UT - 1)
                {
                    velocity_flow.add(x, y, dx, dy, vp);
                    last_use[x][y] = UT;
                    return {P(vp), 1, {nx, ny}};
                }
                auto [t, prop, end] = propagate_flow(nx, ny, vp);
                ret += t;
                if (prop)
                {
                    velocity_flow.add(x, y, dx, dy, t);
                    last_use[x][y] = UT;
                    return {P(t), prop && end != pair(x, y), end};
                }
            }
        }
        last_use[x][y] = UT;
        return {ret, 0, {0, 0}};
    }

    void propagate_stop(int x, int y, bool force = false)
    {
        if (!force)
        {
            bool stop = true;
            for (auto [dx, dy] : deltas)
            {
                int nx = x + dx, ny = y + dy;
                if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 && velocity.get(x, y, dx, dy) > 0)
                {
                    stop = false;
                    break;
                }
            }
            if (!stop)
            {
                return;
            }
        }
        last_use[x][y] = UT;
        for (auto [dx, dy] : deltas)
        {
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] == '#' || last_use[nx][ny] == UT || velocity.get(x, y, dx, dy) > 0)
            {
                continue;
            }
            propagate_stop(nx, ny);
        }
    }

    P move_prob(int x, int y)
    {
        P sum = 0;
        for (size_t i = 0; i < deltas.size(); ++i)
        {
            auto [dx, dy] = deltas[i];
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] == '#' || last_use[nx][ny] == UT)
            {
                continue;
            }
            auto v = velocity.get(x, y, dx, dy);
            if (v < 0)
            {
                continue;
            }
            sum += v;
        }
        return sum;
    }

    struct ParticleParams
    {
        char type;
        P cur_p;
        array<V, deltas.size()> v;

        void swap_with(Simulator &sim, int x, int y)
        {
            swap(sim.field[x][y], type);
            swap(sim.p[x][y], cur_p);
            swap(sim.velocity.v[x][y], v);
        }
    };

    bool propagate_move(int x, int y, bool is_first)
    {
        last_use[x][y] = UT - is_first;
        bool ret = false;
        int nx = -1, ny = -1;
        do
        {
            array<P, deltas.size()> tres;
            P sum = 0;
            for (size_t i = 0; i < deltas.size(); ++i)
            {
                auto [dx, dy] = deltas[i];
                int nx = x + dx, ny = y + dy;
                if (field[nx][ny] == '#' || last_use[nx][ny] == UT)
                {
                    tres[i] = sum;
                    continue;
                }
                auto v = velocity.get(x, y, dx, dy);
                if (v < 0)
                {
                    tres[i] = sum;
                    continue;
                }
                sum += v;
                tres[i] = sum;
            }

            if (sum == 0)
            {
                break;
            }

            P p = P::random01() * sum;
            size_t d = std::ranges::upper_bound(tres, p) - tres.begin();

            auto [dx, dy] = deltas[d];
            nx = x + dx;
            ny = y + dy;
            assert(velocity.get(x, y, dx, dy) > 0 && field[nx][ny] != '#' && last_use[nx][ny] < UT);

            ret = (last_use[nx][ny] == UT - 1 || propagate_move(nx, ny, false));
        } while (!ret);
        last_use[x][y] = UT;
        for (size_t i = 0; i < deltas.size(); ++i)
        {
            auto [dx, dy] = deltas[i];
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 && velocity.get(x, y, dx, dy) < 0)
            {
                propagate_stop(nx, ny);
            }
        }
        if (ret)
        {
            if (!is_first)
            {
                ParticleParams pp{};
                pp.swap_with(*this, x, y);
                pp.swap_with(*this, nx, ny);
                pp.swap_with(*this, x, y);
            }
        }
        return ret;
    }

    void run(std::string source)
    {
        rho[' '] = 0.01;
        rho['.'] = 1000;
        P g = 0.1;

        FILE *f = fopen(source.c_str(), "r");
        size_t n, m;
        fscanf(f, "%zu %zu\n", &n, &m);
        for (size_t x = 0; x < N; ++x)
        {
            for (size_t y = 0; y < M; ++y)
            {
                field[x][y] = fgetc(f);
            }
            field[x][M] = '\0';
            fgetc(f);
        }
        fclose(f);

        for (size_t x = 0; x < N; ++x)
        {
            for (size_t y = 0; y < M; ++y)
            {
                if (field[x][y] == '#')
                    continue;
                for (auto [dx, dy] : deltas)
                {
                    dirs[x][y] += (field[x + dx][y + dy] != '#');
                }
            }
        }

        for (size_t i = 0; i < T; ++i)
        {
            P total_delta_p = 0;
            for (size_t x = 0; x < N; ++x)
            {
                for (size_t y = 0; y < M; ++y)
                {
                    if (field[x][y] == '#')
                        continue;
                    if (field[x + 1][y] != '#')
                        velocity.add(x, y, 1, 0, V(g));
                }
            }

            for (int x = 0; x < N; ++x)
            {
                for (int y = 0; y < M; ++y)
                {
                    old_p[x][y] = p[x][y];
                }
            }
            for (size_t x = 0; x < N; ++x)
            {
                for (size_t y = 0; y < M; ++y)
                {
                    if (field[x][y] == '#')
                        continue;
                    for (auto [dx, dy] : deltas)
                    {
                        int nx = x + dx, ny = y + dy;
                        if (field[nx][ny] != '#' && old_p[nx][ny] < old_p[x][y])
                        {
                            auto delta_p = old_p[x][y] - old_p[nx][ny];
                            auto force = delta_p;
                            auto &contr = velocity.get(nx, ny, -dx, -dy);
                            if (contr * rho[(int)field[nx][ny]] >= force)
                            {
                                contr -= force / rho[(int)field[nx][ny]];
                                continue;
                            }
                            force -= contr * rho[(int)field[nx][ny]];
                            contr = 0;
                            velocity.add(x, y, dx, dy, force / rho[(int)field[x][y]]);
                            p[x][y] -= force / P(dirs[x][y]);
                            total_delta_p -= force / P(dirs[x][y]);
                        }
                    }
                }
            }

            velocity_flow = {};
            bool prop = false;
            do
            {
                UT += 2;
                prop = 0;
                for (size_t x = 0; x < N; ++x)
                {
                    for (size_t y = 0; y < M; ++y)
                    {
                        if (field[x][y] != '#' && last_use[x][y] != UT)
                        {
                            auto [t, local_prop, _] = propagate_flow(x, y, 1);
                            if (t > 0)
                            {
                                prop = 1;
                            }
                        }
                    }
                }
            } while (prop);

            for (size_t x = 0; x < N; ++x)
            {
                for (size_t y = 0; y < M; ++y)
                {
                    if (field[x][y] == '#')
                        continue;
                    for (auto [dx, dy] : deltas)
                    {
                        auto old_v = velocity.get(x, y, dx, dy);
                        auto new_v = velocity_flow.get(x, y, dx, dy);
                        if (old_v > 0)
                        {
                            assert(new_v <= old_v);
                            velocity.get(x, y, dx, dy) = new_v;
                            auto force = (old_v - new_v) * rho[(int)field[x][y]];
                            if (field[x][y] == '.')
                                force *= P(0.8);
                            if (field[x + dx][y + dy] == '#')
                            {
                                p[x][y] += force / P(dirs[x][y]);
                                total_delta_p += force / P(dirs[x][y]);
                            }
                            else
                            {
                                p[x + dx][y + dy] += force / P(dirs[x + dx][y + dy]);
                                total_delta_p += force / P(dirs[x + dx][y + dy]);
                            }
                        }
                    }
                }
            }

            UT += 2;
            prop = false;
            for (size_t x = 0; x < N; ++x)
            {
                for (size_t y = 0; y < M; ++y)
                {
                    if (field[x][y] != '#' && last_use[x][y] != UT)
                    {
                        if (P::random01() < move_prob(x, y))
                        {
                            prop = true;
                            propagate_move(x, y, true);
                        }
                        else
                        {
                            propagate_stop(x, y, true);
                        }
                    }
                }
            }

            if (prop)
            {
                cout << "Tick " << i << ":\n";
                for (size_t x = 0; x < N; ++x)
                {
                    cout << field[x] << "\n";
                }
            }
        }
    }
};