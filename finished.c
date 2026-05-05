// Kubus Berputar Menggunakan C
// Based on "Kubus Berputar Menggunakan C" by Kemas Nayar
// Completed implementation

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

// ─────────────────────────────────────────
// Fase 1: Fondasi Matematis
// ─────────────────────────────────────────

#define PI 3.14159265358979323846

double DEG2RAD(double x) {
    return ((x) * PI / 180.0);
}

double A, B, C;          // Sudut rotasi (derajat)
double sinA, sinB, sinC;
double cosA, cosB, cosC;

// Pre-kalkulasi sin/cos untuk efisiensi
void inis() {
    double radA = DEG2RAD(A);
    double radB = DEG2RAD(B);
    double radC = DEG2RAD(C);

    sinA = sin(radA);
    sinB = sin(radB);
    sinC = sin(radC);

    cosA = cos(radA);
    cosB = cos(radB);
    cosC = cos(radC);
}

// Rotasi 3D: hasil koordinat X setelah rotasi Rx(A) * Ry(B) * Rz(C)
double sumbuX(double i, double j, double k) {
    return i * cosB * cosC
         + j * sinA * sinB * cosC - k * cosA * sinB * cosC
         + j * cosA * sinC + k * sinA * sinC;
}

// Koordinat Y setelah rotasi
double sumbuY(double i, double j, double k) {
    return i * cosB * -sinC
         + j * sinA * sinB * -sinC + k * cosA * sinB * sinC
         + j * cosA * cosC + k * sinA * cosC;
}

// Koordinat Z setelah rotasi
double sumbuZ(double i, double j, double k) {
    return i * sinB - j * sinA * cosB + k * cosA * cosB;
}

// ─────────────────────────────────────────
// Fase 2 & 3: Proyeksi + Buffer
// ─────────────────────────────────────────

#define WIDTH  160
#define HEIGHT 44

// K1 = faktor skala, K2 = jarak kamera
const int K1 = 40;
const int K2 = 100;

char  Buffer[WIDTH * HEIGHT];
float zBuffer[WIDTH * HEIGHT];
int   bgASCII = ' ';

void titikPemrosesan(double cubeX, double cubeY, double cubeZ, char ch) {
    // Hitung koordinat 3D setelah rotasi
    double x = sumbuX(cubeX, cubeY, cubeZ);
    double y = sumbuY(cubeX, cubeY, cubeZ);
    double z = sumbuZ(cubeX, cubeY, cubeZ) + K2;

    // 1/Z: semakin besar = semakin dekat ke kamera
    float seperZ = 1.0f / (float)z;

    // Proyeksi perspektif
    // *2 pada X untuk koreksi aspect ratio terminal (karakter lebih tinggi dari lebar)
    int x_projected = (int)(x * seperZ * K1 * 2);
    int y_projected = (int)(y * seperZ * K1);

    // Geser origin ke tengah layar
    int screen_x = x_projected + (WIDTH  / 2);
    int screen_y = y_projected + (HEIGHT / 2);

    // Periksa batas layar
    if (screen_x >= 0 && screen_x < WIDTH &&
        screen_y >= 0 && screen_y < HEIGHT) {

        int idx = screen_y * WIDTH + screen_x;

        // Z-Buffering: hanya gambar jika titik ini lebih dekat
        if (seperZ > zBuffer[idx]) {
            zBuffer[idx] = seperZ;
            Buffer[idx]  = ch;
        }
    }
}

// ─────────────────────────────────────────
// Fase 4: Menggambar Permukaan Kubus
// ─────────────────────────────────────────

// Setiap sisi kubus diwakili karakter ASCII berbeda agar mudah dibedakan
// Kubus dari -S hingga +S pada setiap sumbu
#define S 20          // setengah panjang sisi kubus
#define STEP 0.5f     // kepadatan titik per permukaan

void gambarKubus() {
    double t;  // parameter iterasi permukaan

    for (t = -S; t <= S; t += STEP) {
        double u;
        for (u = -S; u <= S; u += STEP) {

            // Sisi depan  (Z = +S) → karakter '@'
            titikPemrosesan(t, u,  S, '@');

            // Sisi belakang (Z = -S) → karakter '$'
            titikPemrosesan(t, u, -S, '$');

            // Sisi kiri   (X = -S) → karakter '~'
            titikPemrosesan(-S, t, u, '~');

            // Sisi kanan  (X = +S) → karakter '#'
            titikPemrosesan( S, t, u, '#');

            // Sisi atas   (Y = +S) → karakter ';'
            titikPemrosesan(t,  S, u, ';');

            // Sisi bawah  (Y = -S) → karakter '+'
            titikPemrosesan(t, -S, u, '+');
        }
    }
}

// ─────────────────────────────────────────
// Fase 5: Render Loop Utama
// ─────────────────────────────────────────

void bersihkanBuffer() {
    memset(Buffer,  bgASCII, sizeof(Buffer));
    memset(zBuffer, 0,       sizeof(zBuffer));
}

void cetakLayar() {
    // Pindahkan kursor ke pojok kiri atas (ANSI escape)
    printf("\x1b[H");

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putchar(Buffer[y * WIDTH + x]);
        }
        putchar('\n');
    }
}

int main() {
    // Sembunyikan kursor terminal
    printf("\x1b[?25l");
    // Bersihkan layar sekali di awal
    printf("\x1b[2J");

    A = 0; B = 0; C = 0;

    while (1) {
        bersihkanBuffer();

        // Pre-kalkulasi sin/cos untuk frame ini
        inis();

        // Gambar semua permukaan kubus ke buffer
        gambarKubus();

        // Cetak buffer ke terminal
        cetakLayar();

        // Increment sudut rotasi
        A += 0.6;   // rotasi sumbu X
        B += 0.3;   // rotasi sumbu Y
        C += 0.1;   // rotasi sumbu Z

        // ~30 FPS
        usleep(16000);
    }

    // Tampilkan kembali kursor (tidak akan tercapai, tapi good practice)
    printf("\x1b[?25h");
    return 0;
}
