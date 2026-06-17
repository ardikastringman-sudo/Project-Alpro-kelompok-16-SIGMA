#include <iostream>
#include <string>
#include <ctime>
#include <conio.h>   

using namespace std;

// ============================================================
//   KONSTANTA DAN KODE WARNA ANSI
// ============================================================
const int MAX_MATKUL = 5;
const int MAX_MHS = 50;
const int MAX_DOSEN = 20;
const int MAX_ABSENSI = 200;

#define NAVY_BLUE   "\033[38;2;12;25;115m"
#define BLUE_LIGHT  "\033[38;2;21;95;160m"
#define CYAN_TEAL   "\033[38;2;31;145;165m"
#define ICE_BLUE    "\033[38;2;140;185;195m"
#define RED         "\033[31m" 
#define RESET       "\033[0m"

// ============================================================
//   STRUCTS
// ============================================================
struct Mahasiswa {
    string NIM;
    string nama_lengkap;
    string password;
    char kelas;
    int matkul_diambil[MAX_MATKUL];
    int jumlah_matkul = 0;
    int matkulDipilih = -1; 
    int status_absen = 0;   

    void signupMahasiswa(string inputNama,
                         string inputNIM,
                         string inputPassword,
                         char inputKelas) {
        nama_lengkap = inputNama;
        NIM = inputNIM;
        password = inputPassword;
        kelas = inputKelas;
    }

    bool loginMahasiswa(string inputNIM,
                        string inputPassword) {
        return (NIM == inputNIM &&
                password == inputPassword);
    }
};

struct Dosen {
    string nama;
    string username;
    string password;
    int matkul_diampu[5];
    int jumlah_ampu = 0;
    int matkulDiampu = -1;
    char kelasDiampu; 

    void signup_dosen(string inputNama, string inputPassword, string inputUsername) {
        nama = inputNama;
        password = inputPassword;
        username = inputUsername;
    }
    bool loginDosen(string inputUsername, string inputPassword) {
        if (username == inputUsername && password == inputPassword) {
            return true;
        }
        return false;
    }
};

struct MataKuliah {
    int id;
    string namaMatkul;
};

struct Absensi {
    string nim;
    string nama;
    string matkul;
    char kelas;
    string tanggal;
    string waktu;
    string keterangan;
};

// ============================================================
//   DATABASE GLOBAL
// ============================================================
Mahasiswa database_mhs[MAX_MHS];
int jumlahMahasiswa = 0;

Dosen database_dosen[MAX_DOSEN];
int jumlah_dosen = 0;

Absensi database_absensi[MAX_ABSENSI];
int jumlahAbsensi = 0;

MataKuliah matkul[MAX_MATKUL] = {
    {1, "Algoritma dan Pemrograman"},
    {2, "Sistem Digital"},
    {3, "Aljabar Linear"},
    {4, "Komputer dan Masyarakat"},
    {5, "Matematika Diskrit"}
};

bool absensiDibuka = false;
int dosenAktif = -1;
int matkulAktif = -1;
char kelasAktif; 

// ============================================================
//   PROTOTIPE FUNGSI
// ============================================================
void banner();
void Signup_mahasiswa();
int login_mahasiswa();
void menuMahasiswa(int index);       
void pilihMatkulMahasiswa(int index);
void isiAbsensi(int index);
void riwayatMahasiswa(int index);

void menuDosen(int index);           
void pilihMatkulDosen(int index);
void bukaAbsensi(int index);
void tutupAbsensi();
void riwayatDosen(int index);

void tampilMatkul();
void menuEditMhs();
void tampilMahasiswa();
void hapusMahasiswa();
void hapusMatkulMahasiswa();
string getTanggal();
string getWaktu();
void menuUtama(); 


// ============================================================
//   FUNGSI UTILITAS / HELPERS
// ============================================================
string getTanggal() {
    time_t now = time(0);
    tm *ltm = localtime(&now); 
    char tanggal[20];
    sprintf(tanggal, "%02d-%02d-%04d", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
    return string(tanggal);
}

string getWaktu() {
    time_t now = time(0);
    tm *ltm = localtime(&now); 
    char waktu[20];
    sprintf(waktu, "%02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return string(waktu);
}

void tampilMatkul() {
    cout << "\n===== DAFTAR MATA KULIAH =====\n";
    for(int i = 0; i < MAX_MATKUL; i++) {
        cout << matkul[i].id << ". " << matkul[i].namaMatkul << endl;
    }
}

void banner() {
    cout << "+==============================================================+\n";
    cout << NAVY_BLUE  "|         :####:   ######    :####:   ###  ###    :##:         |\n";
    cout << NAVY_BLUE  "|        :######   ######    ######   ###  ###     ##          |\n";
    cout << NAVY_BLUE  "|       ##:    :#    ##     :##:  .#  ###::###    ####         |\n";
    cout << BLUE_LIGHT "|       ##           ##     ##:       ###  ###    ####         |\n";
    cout << BLUE_LIGHT "|       ###:         ##     ##.       ## ## ##   :#  #:        |\n";
    cout << BLUE_LIGHT "|       :#####:      ##     ##        ##:##:##    #::#         |\n";
    cout << CYAN_TEAL  "|        .#####:     ##     ##  ####  ##.##.##   ##  ##        |\n";
    cout << CYAN_TEAL  "|            :###    ##     ##. ####  ## ## ##   ######        |\n";
    cout << CYAN_TEAL  "|              ##    ##     ##:   ##  ##    ##  .######.       |\n";
    cout << ICE_BLUE   "|       #:.    :##   ##     :##:  ##  ##    ##  :##  ##:       |\n";
    cout << ICE_BLUE   "|        #######:  ######    #######  ##    ##  ###  ###       |\n";
    cout << ICE_BLUE   "|        .#####:   ######     :####.  ##    ##  ##:  :##       |\n";
    cout << RESET;
    cout << "+==============================================================+\n";
    cout << NAVY_BLUE "|                SISTEM GUNA ABSENSI MAHASISWA                 |\n";
    cout << RESET;
    cout << "+==============================================================+\n";
}

// ============================================================
//   MAHASISWA
// ============================================================

void menuMahasiswa(int index) {
    int pil;
    while(true) {
        system("cls");
        cout << "\n=== DASHBOARD MAHASISWA (" << database_mhs[index].nama_lengkap << " - Kelas " << database_mhs[index].kelas << ") ===\n";
        cout << "1. Pilih Mata Kuliah\n";
        cout << "2. Isi Absensi Sesi Aktif\n";
        cout << "3. Lihat Riwayat Absensi Saya\n";
        cout << "0. Log Out\n";
        cout << "Pilihan: ";
        cin >> pil;
        cin.ignore();
        
        switch (pil){
            case 1:
                pilihMatkulMahasiswa(index);
                break;
            case 2:
                isiAbsensi(index);
                break;
            case 3:
                riwayatMahasiswa(index);
                break;
            case 0:
                return;
            default:
                cout << "Pilihan tidak valid!\n";
                system("pause");
        }
    }
}

void pilihMatkulMahasiswa(int idx_mhs) {
    system("cls");
    tampilMatkul();
    cout << "\nPilih nomor mata kuliah yang ingin diambil: ";
    int pil; cin >> pil; cin.ignore();
    
    if (pil >= 1 && pil <= MAX_MATKUL) {
        int idx_matkul = pil - 1;
        
        bool sudah_ada = false;
        for (int i = 0; i < database_mhs[idx_mhs].jumlah_matkul; i++) {
            if (database_mhs[idx_mhs].matkul_diambil[i] == idx_matkul) {
                sudah_ada = true;
                break;
            }
        }
        
        if (!sudah_ada) {
            int pos = database_mhs[idx_mhs].jumlah_matkul;
            database_mhs[idx_mhs].matkul_diambil[pos] = idx_matkul;
            database_mhs[idx_mhs].matkulDipilih = idx_matkul; 
            database_mhs[idx_mhs].jumlah_matkul++;
            cout << "[+] Berhasil mengambil mata kuliah: " << matkul[idx_matkul].namaMatkul << "\n";
        } else {
            cout << "[-] Mata kuliah ini sudah Anda ambil sebelumnya.\n";
        }
    } else {
        cout << "[-] Pilihan tidak valid!\n";
    }
    system("pause");
}

void isiAbsensi(int index) {
    system("cls");
    if(!absensiDibuka) {
        cout << "\n[Sistem] Absensi belum dibuka oleh Dosen!\n";
        system("pause");
        return;
    }
    if(database_mhs[index].matkulDipilih == -1) {
        cout << "\n[Sistem] Anda belum memilih mata kuliah di dashboard Anda!\n";
        system("pause");
        return;
    }
    if(database_mhs[index].matkulDipilih != matkulAktif || database_mhs[index].kelas != kelasAktif) {
        cout << "\n[Sistem] Tidak ada sesi absensi aktif untuk Mata Kuliah / Kelas Anda saat ini.\n";
        system("pause");
        return;
    }

    for(int i = 0; i < jumlahAbsensi; i++) {
        if(database_absensi[i].nim == database_mhs[index].NIM &&
           database_absensi[i].tanggal == getTanggal() &&
           database_absensi[i].matkul == matkul[matkulAktif].namaMatkul) {
            cout << "\n[Sistem] Anda SUDAH melakukan absensi pada hari ini!\n";
            system("pause");
            return;
        }
    }

    int pilihKet;
    cout << "\n===== PENGISIAN ABSENSI =====\n";
    cout << "Nama   : " << database_mhs[index].nama_lengkap << endl;
    cout << "Matkul : " << matkul[matkulAktif].namaMatkul << " (Kelas " << kelasAktif << ")\n";
    cout << "\nKeterangan Kehadiran:\n";
    cout << "1. Hadir\n";
    cout << "2. Izin\n";
    cout << "3. Sakit\n";
    cout << "Pilih status (1-3): ";
    cin >> pilihKet;
    cin.ignore();

    string ket;
    database_mhs[index].status_absen = pilihKet; 
    
    switch(pilihKet) {
        case 1: ket = "Hadir"; break;
        case 2: ket = "Izin"; break;
        case 3: ket = "Sakit"; break;
        default:
            cout << "\nPilihan tidak valid! Absen dibatalkan.\n";
            system("pause");
            return;
    }

    database_absensi[jumlahAbsensi].nim = database_mhs[index].NIM;
    database_absensi[jumlahAbsensi].nama = database_mhs[index].nama_lengkap;
    database_absensi[jumlahAbsensi].matkul = matkul[matkulAktif].namaMatkul;
    database_absensi[jumlahAbsensi].kelas = database_mhs[index].kelas;
    database_absensi[jumlahAbsensi].tanggal = getTanggal();
    database_absensi[jumlahAbsensi].waktu = getWaktu();
    database_absensi[jumlahAbsensi].keterangan = ket;
    jumlahAbsensi++;

    cout << "\n[Sistem] Absensi Anda (" << ket << ") berhasil disimpan!\n";
    system("pause");
}

void riwayatMahasiswa(int index) {
    system("cls");
    bool ditemukan = false;
    cout << "\n===== RIWAYAT ABSENSI ANDA =====\n";

    for(int i = 0; i < jumlahAbsensi; i++) {
        if(database_absensi[i].nim == database_mhs[index].NIM) {
            ditemukan = true;
            cout << "Tanggal  : " << database_absensi[i].tanggal << " (" << database_absensi[i].waktu << ")\n";
            cout << "Matkul   : " << database_absensi[i].matkul << " (Kelas " << database_absensi[i].kelas << ")\n";
            cout << "Status   : " << database_absensi[i].keterangan << "\n";
            cout << "-------------------------------------------\n";
        }
    }
    if(!ditemukan) {
        cout << "\nBelum ada riwayat absensi tercatat.\n";
    }
    system("pause");
}

// ============================================================
//   LOGIKA DOSEN
// ============================================================

void menuDosen(int index) {
    int pil;
    while(true) {
        system("cls");
        cout << "\n=== DASHBOARD DOSEN (" << database_dosen[index].nama << ") ===\n";
        if(database_dosen[index].matkulDiampu != -1) {
            cout << "Matkul diampu : " << matkul[database_dosen[index].matkulDiampu].namaMatkul 
                 << " (Kelas " << database_dosen[index].kelasDiampu << ")\n";
        } else {
            cout << "Matkul diampu : Belum memilih\n";
        }
        cout << "Status Absensi Berjalan: " << (absensiDibuka ? "TERBUKA" : "TERTUTUP") << "\n";
        cout << "-------------------------------------------\n";
        cout << "1. Pilih Mata Kuliah & Kelas Ampuan\n";
        cout << "2. Buka Sesi Absensi\n";
        cout << "3. Tutup Sesi Absensi\n";
        cout << "4. Riwayat Absensi\n";
        cout << "5. Edit Mahasiswa\n";
        cout << "0. Log Out\n";
        cout << "Pilihan: ";
        cin >> pil;
        cin.ignore();

        if(pil == 1) pilihMatkulDosen(index);
        else if(pil == 2) bukaAbsensi(index);
        else if(pil == 3) tutupAbsensi();
        else if(pil == 4) riwayatDosen(index);
        else if(pil == 5) menuEditMhs();
        else if(pil == 0) break;
        else {
            cout << "Pilihan tidak valid!\n";
            system("pause");
        }
    }
}

void pilihMatkulDosen(int idx_dosen) {
    system("cls");
    if (absensiDibuka && dosenAktif == idx_dosen) {
        cout << "\n[Sistem] Tidak bisa mengganti mata kuliah saat sesi absensi masih aktif.\n";
        system("pause");
        return;
    }
    tampilMatkul();
    cout << "\nPilih nomor mata kuliah yang ingin diampu: ";
    int pil; cin >> pil; cin.ignore();
    
    if (pil >= 1 && pil <= MAX_MATKUL) {
        int idx_matkul = pil - 1;
        
        bool sudah_ada = false;
        for (int i = 0; i < database_dosen[idx_dosen].jumlah_ampu; i++) {
            if (database_dosen[idx_dosen].matkul_diampu[i] == idx_matkul) {
                sudah_ada = true;
                break;
            }
        }
        
        if (!sudah_ada) {
            int pos = database_dosen[idx_dosen].jumlah_ampu;
            database_dosen[idx_dosen].matkul_diampu[pos] = idx_matkul;
            database_dosen[idx_dosen].matkulDiampu = idx_matkul;
            database_dosen[idx_dosen].jumlah_ampu++;
            
            cout << "Masukkan kelas yang diampu (A/B/C/D): ";
            cin >> database_dosen[idx_dosen].kelasDiampu;
            cin.ignore();
            
            cout << "[+] Berhasil mengampu mata kuliah: " << matkul[idx_matkul].namaMatkul << "\n";
        } else {
            cout << "[-] Anda sudah mengampu mata kuliah ini.\n";
        }
    } else {
        cout << "[-] Pilihan tidak valid!\n";
    }
    system("pause");
}

void bukaAbsensi(int index) {
    system("cls");
    if (absensiDibuka) {
        cout << "\n[Sistem] Sudah ada sesi aktif. Tutup dulu sebelum membuka baru.\n";
        system("pause");
        return;
    }

    if (database_dosen[index].matkulDiampu == -1) {
        cout << "\n[Sistem] Anda belum memilih mata kuliah!\n";
        system("pause");
        return;
    }

    absensiDibuka = true;
    dosenAktif = index;
    matkulAktif = database_dosen[index].matkulDiampu;
    kelasAktif = database_dosen[index].kelasDiampu;

    cout << "\n===== ABSENSI DIBUKA =====\n";
    cout << "Mata Kuliah : " << matkul[matkulAktif].namaMatkul << "\n";
    cout << "Kelas       : " << kelasAktif << "\n";
    cout << "Tanggal     : " << getTanggal() << "\n";
    cout << "Waktu       : " << getWaktu() << "\n";
    cout << "Status      : TERBUKA\n";

    system("pause");
}

void tutupAbsensi() {
    system("cls");

    if (!absensiDibuka) {
        cout << "\n[Sistem] Absensi sudah tertutup.\n";
        system("pause");
        return;
    }

    cout << "\n===== ABSENSI DITUTUP =====\n";
    cout << "Mata Kuliah : " << matkul[matkulAktif].namaMatkul << "\n";
    cout << "Kelas       : " << kelasAktif << "\n";
    cout << "Tanggal     : " << getTanggal() << "\n";
    cout << "Waktu       : " << getWaktu() << "\n";
    cout << "Status      : TERTUTUP\n";

    absensiDibuka = false;
    dosenAktif = -1;
    matkulAktif = -1;
    kelasAktif = '\0';

    system("pause");
}

void riwayatDosen(int index) {
    system("cls");
    
    if (database_dosen[index].matkulDiampu == -1) {
        cout << "\n[Sistem] Anda belum memilih mata kuliah dan kelas yang diampu!\n";
        system("pause");
        return;
    }

    string matkulDosen = matkul[database_dosen[index].matkulDiampu].namaMatkul;
    char kelasDosen = database_dosen[index].kelasDiampu;
    bool ditemukan = false;

    cout << "\n===== RIWAYAT ABSENSI MAHASISWA =====\n";
    cout << "Mata Kuliah : " << matkulDosen << "\n";
    cout << "Kelas       : " << kelasDosen << "\n";
    cout << "-------------------------------------------\n";

    for(int i = 0; i < jumlahAbsensi; i++) {
        if(database_absensi[i].matkul == matkulDosen && database_absensi[i].kelas == kelasDosen) {
            ditemukan = true;
            cout << "Tanggal  : " << database_absensi[i].tanggal << " (" << database_absensi[i].waktu << ")\n";
            cout << "NIM      : " << database_absensi[i].nim << "\n";
            cout << "Nama     : " << database_absensi[i].nama << "\n";
            cout << "Status   : " << database_absensi[i].keterangan << "\n";
            cout << "-------------------------------------------\n";
        }
    }    
    if(!ditemukan) {
        cout << "Belum ada data absensi mahasiswa untuk kelas ini.\n";
    }
    system("pause");
}

// =============================
// EDIT DAN HAPUS
// =============================

void menuEditMhs(){
    system("cls");
    cout << "\n===== EDIT DATA MAHASISWA =====\n" << endl;
    cout << endl;
    cout << "1. Lihat Data Mahasiswa\n";
    cout << "2. Hapus Data Mahasiswa\n";
    cout << "3. Hapus Mata Kuliah Mahasiswa\n";
    cout << "0. Kembali\n";
    int pilih;
    cout << "Pilihan: ";
    cin >> pilih;
    
    switch (pilih){
        case 1:
            tampilMahasiswa();
            system("pause");
            break;
        case 2:
            hapusMahasiswa();
            break;
        case 3:
            hapusMatkulMahasiswa();
            break;
        case 0:
            return;
        default:
            cout << "Input tidak valid!";
    }
}

void tampilMahasiswa(){
    cout << "\n===== DATA MAHASISWA =====\n";

    if (jumlahMahasiswa == 0) {
        cout << "Belum ada data mahasiswa.\n";
        return;
    }
    for (int i = 0; i < jumlahMahasiswa; i++) {
        cout << i + 1 << ". "
             << database_mhs[i].nama_lengkap
             << " | NIM : " << database_mhs[i].NIM
             << " | Kelas : " << database_mhs[i].kelas
             << endl;
    }
}

void hapusMahasiswa() {
    system("cls");

    cout << "\n===== DATA MAHASISWA =====\n";

    if (jumlahMahasiswa == 0) {
        cout << "Belum ada data mahasiswa.\n";
        system("pause");
        return;
    }
    for (int i = 0; i < jumlahMahasiswa; i++) {
        cout << i + 1 << ". "
             << database_mhs[i].nama_lengkap
             << " | NIM : " << database_mhs[i].NIM
             << " | Kelas : " << database_mhs[i].kelas
             << endl;
    }
    int pilih;
    cout << "\nPilihan nomor mahasiswa yang akan dihapus : ";
    cin >> pilih;
    cin.ignore();

    if (pilih < 1 || pilih > jumlahMahasiswa) {
        cout << "\nPilihan tidak valid!\n";
        system("pause");
        return;
    }
    pilih--;

    for (int i = pilih; i < jumlahMahasiswa - 1; i++) {
        database_mhs[i] = database_mhs[i + 1];
    }
    jumlahMahasiswa--;

    cout << "\n[+] Data mahasiswa berhasil dihapus.\n";
    system("pause");
}

void hapusMatkulMahasiswa() {
    system("cls");

    tampilMahasiswa();

    if (jumlahMahasiswa == 0) {
        system("pause");
        return;
    }
    int pilihMhs;
    cout << "\nPilih nomor mahasiswa : ";
    cin >> pilihMhs;
    cin.ignore();

    if (pilihMhs < 1 || pilihMhs > jumlahMahasiswa) {
        cout << "\nPilihan tidak valid!\n";
        system("pause");
        return;
    }

    pilihMhs--;

    if (database_mhs[pilihMhs].jumlah_matkul == 0) {
        cout << "\nMahasiswa belum mengambil mata kuliah.\n";
        system("pause");
        return;
    }

    cout << "\n===== DAFTAR MATA KULIAH =====\n";

    for (int i = 0; i < database_mhs[pilihMhs].jumlah_matkul; i++) {
        int idx = database_mhs[pilihMhs].matkul_diambil[i];
        cout << i + 1 << ". "
             << matkul[idx].namaMatkul
             << endl;
    }

    int pilihMatkul;
    cout << "\nPilih mata kuliah yang akan dihapus : ";
    cin >> pilihMatkul;
    cin.ignore();

    if (pilihMatkul < 1 ||
        pilihMatkul > database_mhs[pilihMhs].jumlah_matkul) {
        cout << "\nPilihan tidak valid!\n";
        system("pause");
        return;
    }

    pilihMatkul--;

    for (int i = pilihMatkul;
         i < database_mhs[pilihMhs].jumlah_matkul - 1;
         i++) {
        database_mhs[pilihMhs].matkul_diambil[i] =
        database_mhs[pilihMhs].matkul_diambil[i + 1];
    }

    database_mhs[pilihMhs].jumlah_matkul--;

    if (database_mhs[pilihMhs].jumlah_matkul == 0) {
        database_mhs[pilihMhs].matkulDipilih = -1;
    }

    cout << "\n[+] Mata kuliah berhasil dihapus.\n";
    system("pause");
}

// ============================================================
//   FUNGSI MENU UTAMA 
// ============================================================
void menuUtama() {
    string opsi[] = {
        "MASUK SEBAGAI MAHASISWA",
        "  MASUK SEBAGAI DOSEN  ",
        "    KELUAR APLIKASI    "
    };

    int sel = 0;
    char key;
    const int TOTAL_MENU = 3;

    while (true) {
        system("cls");
        banner();
        cout << "  Data Mhs: " << jumlahMahasiswa << "/" << MAX_MHS << "  |  Panah: navigasi  |  Enter: pilih" << endl;
        cout << "---------------------------------------------------------------" << endl;

        for (int i = 0; i < TOTAL_MENU; i++) {
            cout << "              ==================================" << endl;
            if (i == sel) {
                if (i == 2) { 
                    cout << "              |" << RED << " >> " << opsi[i] << " << " << RESET << "|" << endl;
                } else { 
                    cout << "              |" << BLUE_LIGHT << " >> " << opsi[i] << " << " << RESET << "|" << endl;
                }
            } else {
                cout << "              |    " << opsi[i] << "    |" << endl;
            }
        }
        cout << "              ==================================" << endl;
        cout << "---------------------------------------------------------------" << endl;

        key = _getch();
        if (key == 0 || key == (char)224) {
            key = _getch();
            if (key == 72) { sel--; if (sel < 0) sel = TOTAL_MENU - 1; } // Panah ATAS
            if (key == 80) { sel++; if (sel >= TOTAL_MENU) sel = 0; }    // Panah BAWAH
        } 
        else if (key == 13 || key == 32) { // Tombol Enter atau Spasi
            if (sel == 2) {
                // Keluar Aplikasi
                system("cls");
                cout << "\nTerima kasih telah menggunakan sistem ini!\n";
                break;
            }

            // ---- MENU MAHASISWA ----
            if (sel == 0) {
                int pilihan;
                while (true) {
                    system("cls");
                    banner();
                    cout << "\n[MENU UTAMA MAHASISWA]\n";
                    cout << "1. Sign Up (Registrasi)\n";
                    cout << "2. Sign In (Login)\n";
                    cout << "0. Kembali ke Menu Utama\n";
                    cout << "Pilihan: ";
                    cin >> pilihan;
                    cin.ignore();

                    if (pilihan == 1) {
                        system("cls");
                        banner();
                        if (jumlahMahasiswa < MAX_MHS) {
                            string nama, nim, password;
                            cout << "Masukan nama lengkap : ";
                            getline(cin, nama);
                            cout << "Masukan NIM : ";
                            getline(cin, nim);
                            cout << "Masukan password : ";
                            getline(cin, password);

                            char daftar_kelas[4] = {'A','B','C','D'};
                            char kelas = daftar_kelas[jumlahMahasiswa % 4];

                            database_mhs[jumlahMahasiswa].signupMahasiswa(nama, nim, password, kelas);
                            cout << "[Sistem] Kelas Anda otomatis ditetapkan: " << kelas << endl;
                            jumlahMahasiswa++;
                            cout << "\n[+] Registrasi mahasiswa berhasil!\n";
                        } else {
                            cout << "\n[-] Database penuh!\n";
                        }
                        system("pause");
                    } else if (pilihan == 2) {
                        system("cls");
                        banner();
                        string nim, pass;
                        cout << "NIM : ";
                        getline(cin, nim);
                        cout << "Password : ";
                        getline(cin, pass);

                        bool status_login = false;
                        for(int i = 0; i < jumlahMahasiswa; i++) {
                            if(database_mhs[i].loginMahasiswa(nim, pass)) {
                                cout << "\nLogin berhasil! Selamat datang, "
                                     << database_mhs[i].nama_lengkap << endl;
                                system("pause");
                                menuMahasiswa(i);
                                status_login = true;
                                break;
                            }
                        }
                        if(!status_login) {
                            cout << "\n[ERROR] NIM atau Password salah!\n";
                            system("pause");
                        }
                    } else if (pilihan == 0) {
                        break;
                    } else {
                        cout << "\n[ERROR] Pilihan tidak valid!\n";
                        system("pause");
                    }
                }
            }

            // ---- MENU DOSEN ----
            else if (sel == 1) {
                int pilihan;
                while (true) {
                    system("cls");
                    banner();
                    cout << "\n[MENU UTAMA DOSEN]\n";
                    cout << "1. Sign Up (Registrasi)\n";
                    cout << "2. Sign In (Login)\n";
                    cout << "0. Kembali ke Menu Utama\n";
                    cout << "Pilihan: ";
                    cin >> pilihan;
                    cin.ignore();

                    if (pilihan == 1) {
                        if (jumlah_dosen < MAX_DOSEN) {
                            string namasementara, usernamesementara, passwordsementara;
                            cout << "\nREGISTRASI DOSEN BARU\n";
                            cout << "Masukan nama     : "; getline(cin, namasementara);
                            cout << "Masukan username : "; getline(cin, usernamesementara);
                            bool username_terpakai = false;
                            for (int i = 0; i < jumlah_dosen; i++) {
                                if (database_dosen[i].username == usernamesementara) {
                                    username_terpakai = true;
                                    break;
                                }
                            }
                            if (username_terpakai) {
                                cout << "\n[ERROR] Username sudah digunakan, coba username lain.\n";
                                system("pause");
                            } else {
                                cout << "Masukan password : "; getline(cin, passwordsementara);
                                database_dosen[jumlah_dosen].signup_dosen(namasementara, passwordsementara, usernamesementara);
                                cout << "\n[+] Registrasi dosen berhasil!\n";
                                cout << "+==============================================================+\n";
                                cout << "  Nama     : " << database_dosen[jumlah_dosen].nama     << "\n";
                                cout << "  Username : " << database_dosen[jumlah_dosen].username << "\n";
                                cout << "  Password : " << database_dosen[jumlah_dosen].password << "\n";
                                cout << "+==============================================================+\n";
                                jumlah_dosen++;
                            }
                        } else {
                            cout << "\n[ERROR] Maaf, database dosen penuh!\n";
                        }
                        system("pause");
                    } else if (pilihan == 2) {
                        system("cls");
                        banner();
                        string inputUsername, inputPassword;
                        cout << "\n===== LOGIN DOSEN =====\n";
                        cout << "Username : "; getline(cin, inputUsername);
                        cout << "Password : "; getline(cin, inputPassword);
                        bool status_login = false;
                        for (int i = 0; i < jumlah_dosen; i++){
                            if (database_dosen[i].loginDosen(inputUsername, inputPassword)) {
                                cout << "\nLogin Berhasil! Selamat datang, " << database_dosen[i].nama << "\n";
                                system("pause");
                                menuDosen(i);
                                status_login = true;
                                break;
                            }
                        }
                        if (!status_login) {
                            cout << "\n[ERROR] Username atau Password salah!\n";
                            system("pause");
                        }
                    } else if (pilihan == 0) {
                        break;
                    } else {
                        cout << "\n[ERROR] Pilihan tidak valid!\n";
                        system("pause");
                    }
                }
            }
        }
    }
}

// ============================================================
//   MAIN PROGRAM
// ============================================================
int main() {
    menuUtama();
    return 0;
}