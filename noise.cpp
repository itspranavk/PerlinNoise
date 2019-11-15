#include <stdio.h>
#include <math.h>
#include <stdlib.h>
using namespace std;

#define MAX_SEEDS 10

int num_x = 512, num_y = 512, num_octaves = 7, seed_index = 0;;
double persistence = 0.5;

int seeds[MAX_SEEDS][3] = {
  { 995615039, 600173719, 701464987 },
  { 831731269, 162318869, 136250887 },
  { 174329291, 946737083, 245679977 },
  { 362489573, 795918041, 350777237 },
  { 457025711, 880830799, 909678923 },
  { 787070341, 177340217, 593320781 },
  { 405493717, 291031019, 391950901 },
  { 458904767, 676625681, 424452397 },
  { 531736441, 939683957, 810651871 },
  { 997169939, 842027887, 423882827 }
};

double noise(int i, int x, int y) {
    int n = ((x + y * 57) << 13) ^ (x + y * 57);
    int t = (n * (n * n * seeds[i][0] + seeds[i][1]) + seeds[i][2]) &
        0x7fffffff;
    return 1.0 - (double)(t)/1073741824.0;
}

double smooth_noise(int i, int x, int y) {
    double vertex = (noise(i, x - 1, y - 1) + noise(i, x + 1, y - 1) + 
        noise(i, x - 1, y + 1) + noise(i, x + 1, y + 1)) / 16;
    double edge = (noise(i, x - 1, y) + noise(i, x + 1, y) + 
        noise(i, x, y - 1) + noise(i, x, y + 1)) / 8;
    double center = noise(i, x, y) / 4;
    return vertex + edge + center;
}

double interpolate(double a, double b, double x) {  // cosine interpolation
    double f = (1 - cos(x * 3.1415927)) * 0.5;
    return  a*(1 - f) + b*f;
}

double interpolated_noise(int i, double x, double y) {
    int int_x = x;
    double delta_x = x - int_x;
    int int_y = y;
    double delta_y = y - int_y;

    double v1 = smooth_noise(i, int_x, int_y),
        v2 = smooth_noise(i, int_x + 1, int_y),
        v3 = smooth_noise(i, int_x, int_y + 1),
        v4 = smooth_noise(i, int_x + 1, int_y + 1),
        i1 = interpolate(v1, v2, delta_x),
        i2 = interpolate(v3, v4, delta_x);
    return interpolate(i1, i2, delta_y);
}

double generate_noise(double x, double y) {
    double total = 0, f = pow(2, num_octaves), a = 1;
    for (int i = 0; i < num_octaves; ++i) {
        f /= 2;
        a *= persistence;
        total += interpolated_noise((seed_index + i) % MAX_SEEDS,
            x / f, y / f) * a;
    }
    return total / f;
}

int main(int argc, char** args) {
    FILE *f = fopen("raw.dat", "w");;

    num_x = atoi(args[1]);
    num_y = atoi(args[2]);
    num_octaves = atoi(args[3]);
    persistence = atof(args[4]);
    seed_index = atoi(args[5]) % MAX_SEEDS;

    printf("X: %d, Y: %d, Octaves: %d, Persistence: %.2f\n",
        num_x, num_y, num_octaves, persistence);
    
    if (seed_index) {
        printf("Seed Index: %d\n", seed_index);
    }
    else {
        printf("Default seed used.\n");
    }

    printf("[%d, %d, %d]\n", seeds[seed_index][0], seeds[seed_index][1],
        seeds[seed_index][2]);

    for (int i = 0; i < num_x; i++) {
        for (int j = 0; j < num_y; j++) {
            double noise = generate_noise(i,j);
            if (j != num_y - 1) {
                fprintf(f, "%.5f ", noise);
            }
            else {
                fprintf(f, "%.5f\n", noise);
            }
        }
    }

    fclose(f);

    return 0;
}