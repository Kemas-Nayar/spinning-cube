#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define PI 3.14159265358979323846

double DEG2RAD(double x) {
  return ((x) * PI / 180.0);
}
double A, B, C;
double sinA, sinB, sinC;
double cosA, cosB, cosC;

void inis(){
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

const int K1 = 100;
const int K2 = 250;


#define WIDTH 160
#define HEIGHT 40

float zBuffer[WIDTH * HEIGHT];
char Buffer[WIDTH * HEIGHT];
int bgASCII = ' ';


double sumbuX(int i, int j, int k){
  return i * cosB * cosC + j * sinA * sinB * cosC + k * cosA * -sinB * cosC 
         + j * cosA * sinC + k * sinA * sinC;
}

double sumbuY(int i, int j, int k){
  return i * cosB * -sinC + j * sinA * sinB * -sinC + k * cosA * sinB * sinC 
         + j * cosA * cosC + k * sinA * cosC;
}

double sumbuZ(int i, int j, int k){
  return i * sinB + j * -sinA * cosB + k * cosA * cosB;
}


void titikPemrosesan(float cubeX, float cubeY, float cubeZ, float nx, float ny, float nz ){
  // Menghitung X, Y, dan Z yang dirotasikan
  float x = sumbuX(cubeX, cubeY, cubeZ);
  float y = sumbuY(cubeX, cubeY, cubeZ);
  float z = sumbuZ(cubeX, cubeY, cubeZ) + K2;

  // Rotasi Vektor Normal
  float r_nx = sumbuX(nx, ny, nz);
  float r_ny = sumbuY(nx, ny, nz);
  float r_nz = sumbuZ(nx, ny, nz);

  // Luminasi (Dot Product)
  float L = (r_nx * 0) + (r_ny * 1) + (r_nz * -1);

  // Semakin besar nilai 1/Z semakin dekat titiknya terhadap kamera.
  float seperZ = 1.0 / (float)z;

  // Rumus Proyeksi
  int x_projected = (int)(x * seperZ * K1 * 2);
  int y_projected = (int)(y * seperZ * K1);

  // Geser koordinat (0,0) ke tengah WIDTH dan HEIGHT
  int screen_x = x_projected + (WIDTH / 2);
  int screen_y = y_projected + (HEIGHT / 2);

  //Periksa batar dan Z-Buffering
  if (screen_x >= 0 && screen_x < WIDTH && screen_y >= 0 && screen_y < HEIGHT) {
    int idx = screen_y * WIDTH + screen_x;
    if (seperZ > zBuffer[idx]) {
        zBuffer[idx] = seperZ;

      // Jika L > 0, permukaan menghadap cahaya
      if (L > 0) {
        // Mapping nilai L ke urutan karakter ASCII (paling gelap ke paling terang)
        const char* brightness = ".,-~:;=!*#$@";
        int b_idx = L * 8; // Menyesuaikan dengan panjang string
        Buffer[idx] = brightness[b_idx];
      } else {
        // Sisi yang membelakangi cahaya tetap diberikan karakter redup
        Buffer[idx] = '.';
      }
    }
  }
}

int main(){
  // Bersihkan layar terminal
  printf("\x1b[2J");

  while(1){
    // Persiapkan fram
    // Reset screen buffer menjadi background ASCII (' ')
    memset(Buffer, bgASCII, WIDTH * HEIGHT);
    // Reset Z-Buffer 
    memset(zBuffer, 0, WIDTH * HEIGHT * sizeof(float));

    // Panggil inis()
    inis();

    const int S = 30; // setengah panjang sisi kubus
    const float STEP = 0.5;  // kepadatan titik per permukaan

    // Menghasilkan semua sisi kubus
    for(float kubuX = -S; kubuX<S; kubuX += STEP){
      for(float kubuY = -S; kubuY<S; kubuY += STEP){

        // Sisi depan diisi dengan @
        titikPemrosesan(kubuX, kubuY, S, 0, 0, 1);


        // Sisi belakang diisi dengan $
        titikPemrosesan(kubuX, kubuY, -S, 0, 0, -1);


        // Sisi kiri diisi dengan ~
        titikPemrosesan(-S, kubuY, kubuX, -1, 0, 0);


        // Sisi kanan diisi dengan #
        titikPemrosesan(S, kubuY, kubuX, 1, 0, 0);


        // Sisi atas diisi dengan ;
        titikPemrosesan(kubuX, -S, kubuY, 0, -1, 0);


        // Sisi bawah diisi dengan +
        titikPemrosesan(kubuX, S, kubuY, 0, 1, 0);
      }
    }


    // Kembalikan cursor terminal ke pojok kiri atas.
    printf("\x1b[H");

    // Loop Screen Buffer 1D dan print 
    for(int k=0; k < WIDTH * HEIGHT; k++){
      // Terminal tidak tahu kapan baris array selesai
      // jika index sudah melebih ukuran terminal, print '\n'
      // Selainnya, print karakter yang kita simpan di buffer 
      putchar(k % WIDTH ? Buffer[k] : '\n');
    }

    // Increment sudutnya. 
    A += 0.6;   // rotasi sumbu X
    B += 0.3;   // rotasi sumbu Y
    C += 0.1;   // rotasi sumbu Z

    usleep(16000); // Kecepatan sekitar 60 fps
  }
  return 0;
}
