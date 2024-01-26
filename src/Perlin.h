#ifndef PERLIN_H
#define PERLIN_H

#include "RtWeekend.h"

class Perlin {
  public:
    Perlin() {
        randomDouble = new double[pointCount];
        for (int i = 0; i < pointCount; ++i) {
            randomDouble[i] = RT_WEEKEND_H::randomDouble();
        }

        permX = perlinGeneratePerm();
        permY = perlinGeneratePerm();
        permZ = perlinGeneratePerm();
    }

    ~Perlin() {
        delete[] randomDouble;
        delete[] permX;
        delete[] permY;
        delete[] permZ;
    }

    double getNoise(const Vec3& p) const {
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());

        // Hermite cubic
        u = u*u*(3-2*u);
        v = v*v*(3-2*v);
        w = w*w*(3-2*w);

        auto i = static_cast<int>(floor(p.x()));
        auto j = static_cast<int>(floor(p.y()));
        auto k = static_cast<int>(floor(p.z()));
        double c[2][2][2];

        for (int di=0; di < 2; di++)
            for (int dj=0; dj < 2; dj++)
                for (int dk=0; dk < 2; dk++)
                    c[di][dj][dk] = randomDouble[
                        permX[(i+di) & 255] ^
                        permY[(j+dj) & 255] ^
                        permZ[(k+dk) & 255]
                    ];

        return trilinear_interpolation(c, u, v, w);
    }

  private:
    static const int pointCount = 256;
    double* randomDouble;
    int* permX;
    int* permY;
    int* permZ;

    static int* perlinGeneratePerm() {
        auto p = new int[pointCount];

        for (int i = 0; i < Perlin::pointCount; i++)
            p[i] = i;

        permute(p, pointCount);

        return p;
    }

    static void permute(int* p, int n) {
        for (int i = n-1; i > 0; i--) {
            int target = randomInt(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static double trilinear_interpolation(double c[2][2][2], double u, double v, double w) {
        auto accum = 0.0;
        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++)
                    accum += (i*u + (1-i)*(1-u))*
                            (j*v + (1-j)*(1-v))*
                            (k*w + (1-k)*(1-w))*c[i][j][k];

        return accum;
    }
};

#endif