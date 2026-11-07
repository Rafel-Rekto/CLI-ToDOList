#include <stdio.h>
#include <string.h>

#define MAX 100
#define NAMA_FILE "database.txt"

struct Tugas {
    char nama[MAX];
    char deskripsi[MAX];
    char deadline[MAX];
    int prioritas;
    int status;
};

void tambahTugas(struct Tugas daftarTugas[], int *jumlahTugas);
void lihatTugas(struct Tugas daftarTugas[], int jumlahTugas);
void hapusTugas(struct Tugas daftarTugas[], int *jumlahTugas);
void ubahStatusTugas(struct Tugas daftarTugas[], int jumlahTugas);
void tampilkanBerdasarkanPrioritas(struct Tugas daftarTugas[], int jumlahTugas);
void statistikJumlahTugas(struct Tugas daftarTugas[], int jumlahTugas);
void cetakTugas(struct Tugas tugas, int nomor);
void bersihkanBuffer();
void simpanTugas(struct Tugas daftarTugas[], int jumlahTugas);
void muatTugas(struct Tugas daftarTugas[], int *jumlahTugas);
int bacaBaris(FILE *file, char tujuan[]);

int main(void){
    struct Tugas daftarTugas[MAX];
    int jumlahTugas = 0;
    int pilihan;

    muatTugas(daftarTugas, &jumlahTugas);

    do{
        printf("\n========== TO DO LIST ==========\n");
        printf("1. Tambah Tugas\n");
        printf("2. Lihat Tugas\n");
        printf("3. Hapus Tugas\n");
        printf("4. Ubah Status Tugas\n");
        printf("5. Tampilkan berdasarkan prioritas\n");
        printf("6. Statistik Jumlah Tugas\n");
        printf("0. Keluar\n");
        printf("Pilihan: ");
        if(scanf("%d", &pilihan) != 1){
            bersihkanBuffer();
            pilihan = -1;
        }

        switch(pilihan){
            case 1:
                tambahTugas(daftarTugas, &jumlahTugas);
                simpanTugas(daftarTugas, jumlahTugas);
                break;
            case 2:
                lihatTugas(daftarTugas, jumlahTugas);
                break;
            case 3:
                hapusTugas(daftarTugas, &jumlahTugas);
                simpanTugas(daftarTugas, jumlahTugas);
                break;
            case 4:
                ubahStatusTugas(daftarTugas, jumlahTugas);
                simpanTugas(daftarTugas, jumlahTugas);
                break;
            case 5:
                tampilkanBerdasarkanPrioritas(daftarTugas, jumlahTugas);
                break;
            case 6:
                statistikJumlahTugas(daftarTugas, jumlahTugas);
                break;
            case 0:
                printf("Terima kasih telah menggunakan aplikasi TO DO LIST.\n");
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }

    } while(pilihan != 0);

    return 0;
}

//Fungsi

void bersihkanBuffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void cetakTugas(struct Tugas tugas, int nomor) {
    printf("%d. Nama Tugas: %s\n", nomor, tugas.nama);
    printf("   Deskripsi: %s\n", tugas.deskripsi);
    printf("   Deadline: %s\n", tugas.deadline);
    printf("   Prioritas: %d\n", tugas.prioritas);
    printf("   Status: %s\n", tugas.status == 1 ? "Selesai" : "Belum Selesai");
}

void tambahTugas(struct Tugas daftarTugas[], int *jumlahTugas) {
    if(*jumlahTugas >= MAX){
        printf("Daftar tugas penuh!\n");
        return;
    }

    struct Tugas *tugas = &daftarTugas[*jumlahTugas];

    printf("Masukkan nama tugas: ");
    scanf(" %99[^\n]", tugas->nama);
    printf("Masukkan deskripsi tugas: ");
    scanf(" %99[^\n]", tugas->deskripsi);
    printf("Masukkan deadline tugas (format: YYYY-MM-DD): ");
    scanf(" %99[^\n]", tugas->deadline);

    do{
        printf("Masukkan prioritas tugas (1-5): ");
        if(scanf("%d", &tugas->prioritas) != 1){
            bersihkanBuffer();
            tugas->prioritas = 0;
        }
        if(tugas->prioritas < 1 || tugas->prioritas > 5){
            printf("Prioritas harus antara 1 sampai 5.\n");
        }
    } while(tugas->prioritas < 1 || tugas->prioritas > 5);

    tugas->status = 0;
    (*jumlahTugas)++;

    printf("Tugas berhasil ditambahkan!\n");
}

void lihatTugas(struct Tugas daftarTugas[], int jumlahTugas) {
    if(jumlahTugas == 0){
        printf("Belum ada tugas.\n");
        return;
    }

    printf("Daftar Tugas:\n");
    for(int i = 0; i < jumlahTugas; i++){
        cetakTugas(daftarTugas[i], i + 1);
    }
}

void hapusTugas(struct Tugas daftarTugas[], int *jumlahTugas) {
    int nomor;

    if(*jumlahTugas == 0){
        printf("Belum ada tugas yang bisa dihapus.\n");
        return;
    }

    lihatTugas(daftarTugas, *jumlahTugas);
    printf("Masukkan nomor tugas yang akan dihapus (0 untuk batal): ");
    if(scanf("%d", &nomor) != 1){
        bersihkanBuffer();
        printf("Input tidak valid.\n");
        return;
    }

    if(nomor == 0){
        printf("Penghapusan dibatalkan.\n");
        return;
    }
    if(nomor < 1 || nomor > *jumlahTugas){
        printf("Nomor tugas tidak ditemukan.\n");
        return;
    }

    for(int i = nomor - 1; i < *jumlahTugas - 1; i++){
        daftarTugas[i] = daftarTugas[i + 1];
    }
    (*jumlahTugas)--;

    printf("Tugas berhasil dihapus!\n");
}

void ubahStatusTugas(struct Tugas daftarTugas[], int jumlahTugas) {
    int nomor;

    if(jumlahTugas == 0){
        printf("Belum ada tugas yang bisa diubah.\n");
        return;
    }

    lihatTugas(daftarTugas, jumlahTugas);
    printf("Masukkan nomor tugas yang akan diubah statusnya (0 untuk batal): ");
    if(scanf("%d", &nomor) != 1){
        bersihkanBuffer();
        printf("Input tidak valid.\n");
        return;
    }

    if(nomor == 0){
        printf("Perubahan status dibatalkan.\n");
        return;
    }
    if(nomor < 1 || nomor > jumlahTugas){
        printf("Nomor tugas tidak ditemukan.\n");
        return;
    }

    daftarTugas[nomor - 1].status = !daftarTugas[nomor - 1].status;

    printf("Status tugas berhasil diubah menjadi %s!\n",
           daftarTugas[nomor - 1].status == 1 ? "Selesai" : "Belum Selesai");
}

void tampilkanBerdasarkanPrioritas(struct Tugas daftarTugas[], int jumlahTugas) {
    if(jumlahTugas == 0){
        printf("Belum ada tugas.\n");
        return;
    }

    printf("Daftar Tugas Berdasarkan Prioritas (5 = paling penting):\n");
    for(int p = 5; p >= 1; p--){
        for(int i = 0; i < jumlahTugas; i++){
            if(daftarTugas[i].prioritas == p){
                cetakTugas(daftarTugas[i], i + 1);
            }
        }
    }
}

void statistikJumlahTugas(struct Tugas daftarTugas[], int jumlahTugas) {
    int selesai = 0;

    for(int i = 0; i < jumlahTugas; i++){
        if(daftarTugas[i].status == 1){
            selesai++;
        }
    }

    printf("Statistik Tugas:\n");
    printf("Total tugas       : %d\n", jumlahTugas);
    printf("Sudah selesai     : %d\n", selesai);
    printf("Belum selesai     : %d\n", jumlahTugas - selesai);
    if(jumlahTugas > 0){
        printf("Persentase selesai: %.1f%%\n", (float)selesai / jumlahTugas * 100);
    }
}

void simpanTugas(struct Tugas daftarTugas[], int jumlahTugas) {
    FILE *file = fopen(NAMA_FILE, "w");

    if(file == NULL){
        printf("Gagal menyimpan data ke file.\n");
        return;
    }

    for(int i = 0; i < jumlahTugas; i++){
        fprintf(file, "%s\n", daftarTugas[i].nama);
        fprintf(file, "%s\n", daftarTugas[i].deskripsi);
        fprintf(file, "%s\n", daftarTugas[i].deadline);
        fprintf(file, "%d %d\n", daftarTugas[i].prioritas, daftarTugas[i].status);
    }

    fclose(file);
}

int bacaBaris(FILE *file, char tujuan[]) {
    char baris[MAX + 2];

    if(fgets(baris, sizeof(baris), file) == NULL){
        return 0;
    }

    baris[strcspn(baris, "\r\n")] = '\0';
    strcpy(tujuan, baris);
    return 1;
}

void muatTugas(struct Tugas daftarTugas[], int *jumlahTugas) {
    FILE *file = fopen(NAMA_FILE, "r");
    char baris[MAX + 2];

    if(file == NULL){
        return; // file belum ada, berarti belum pernah menyimpan tugas
    }

    while(*jumlahTugas < MAX){
        struct Tugas *tugas = &daftarTugas[*jumlahTugas];

        if(!bacaBaris(file, tugas->nama)) break;
        if(!bacaBaris(file, tugas->deskripsi)) break;
        if(!bacaBaris(file, tugas->deadline)) break;
        if(fgets(baris, sizeof(baris), file) == NULL) break;
        if(sscanf(baris, "%d %d", &tugas->prioritas, &tugas->status) != 2) break;

        (*jumlahTugas)++;
    }

    fclose(file);
}