#include <iostream>
#include <string>
#include <ctime>

using namespace std;

// ============================================================
//   KONSTANTA DAN KODE WARNA ANSI
// ============================================================
const int MAKS_MK = 5;
const int MAKS_MHS = 50;
const int MAKS_DSN = 20;
const int MAKS_ABSEN = 200;

#define NAVY_BLUE   "\033[38;2;12;25;115m"
#define BLUE_LIGHT  "\033[38;2;21;95;160m"
#define CYAN_TEAL   "\033[38;2;31;145;165m"
#define ICE_BLUE    "\033[38;2;140;185;195m"
#define RESET       "\033[0m"

// ============================================================
//   STRUCTS
// ============================================================
struct Mahasiswa {
    string nim;
    string nama_mhs;
    string pass;
    char kelas;
    int mk_diambil[MAKS_MK]; // Ditambahkan agar sesuai dengan fungsi pilihMatkul
    int jml_mk = 0;          // Ditambahkan agar sesuai dengan fungsi pilihMatkul         
    int mk_pilihan = -1; 
    int status_absen = 0;   
};

struct Dosen {
    string nama;
    string usn;
    string pass;
    int mk_diampu[5];
    int jml_diampu = 0;
    int sedang_diampu = -1;   // Ditambahkan agar sinkron dengan menuDosen & bukaAbsensi      
    char kls_diampu; 
    
    
};

struct MataKuliah {
    int id;
    string nama_mk;
};

struct Absensi {
    string nim;
    string nama;
    string matkul;
    char kelas;
    string tgl;
    string jam;
    string ket;
};

// ============================================================
//   DATABASE GLOBAL
// ============================================================
Mahasiswa data_mhs[MAKS_MHS];
int jml_mhs = 0; // Diselaraskan menggunakan satu variabel ini

Dosen data_dsn[MAKS_DSN];
int jml_dsn = 0;

Absensi data_absen[MAKS_ABSEN];
int jml_absen = 0;

// Diselaraskan menggunakan struct MataKuliah agar fungsi tampilMatkul() bekerja
MataKuliah list_mk[MAKS_MK] = {
    {1, "Algoritma dan Pemrograman"},
    {2, "Sistem Digital"},
    {3, "Aljabar Linear"},
    {4, "Komputer dan Masyarakat"},
    {5, "Matematika Diskrit"}
};

bool buka_absen = false;
int dsn_aktif = -1;
int mk_aktif = -1;
char kls_aktif; 

// ============================================================
//   PROTOTIPE FUNGSI
// ============================================================
void cetak_banner();
void reg_mhs();     // Pakai POINTER di dalamnya         
int login_mhs();
void menu_mhs(int idx);        
void mhs_pilih_mk(int idx);
void mhs_absen(int idx);
void histori_mhs(int idx);

void reg_dsn();      // Pakai POINTER di dalamnya            
int login_dsn();
void menu_dsn(int idx);            
void dsn_pilih_mk(int idx);
void dsn_buka_absen(int idx);
void dsn_tutup_absen();
void histori_dsn(int idx);

void tampil_mk();
void menu_edit_mhs();
void lihat_data_mhs();
void hapus_mhs();
void hapus_mk_mhs();
string ambil_tgl();       // Pakai POINTER tm* bawaan ctime           
string ambil_jam();       // Pakai POINTER tm* bawaan ctime          



// ============================================================
//   FUNGSI UTILITAS / HELPERS
// ============================================================
string ambil_tgl() {
    time_t now = time(0);
    tm *ltm = localtime(&now); 
    char buffer_tgl[20];
    sprintf(buffer_tgl, "%02d-%02d-%04d", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
    return string(buffer_tgl);
}

string ambil_jam() {
    time_t now = time(0);
    tm *ltm = localtime(&now); 
    char buffer_jam[20];
    sprintf(buffer_jam, "%02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return string(buffer_jam);
}

void tampil_mk() {
    cout << "\n===== DAFTAR MATA KULIAH =====\n";
    for(int i = 0; i < MAKS_MK; i++) {
        cout << list_mk[i].id << ". " << list_mk[i].nama_mk << endl;
    }
}

void cetak_banner() {
    cout << "+==============================================================+\n";
    cout << NAVY_BLUE  "|        :####:    ######    :####:   ###  ###    :##:         |\n";
    cout << NAVY_BLUE  "|       :######    ######    ######   ###  ###     ##          |\n";
    cout << NAVY_BLUE  "|       ##:    :#     ##     :##:  .#  ###::###    ####          |\n";
    cout << BLUE_LIGHT "|       ##           ##     ##:       ###  ###    ####          |\n";
    cout << BLUE_LIGHT "|       ###:         ##     ##.       ## ## ##    :#  #:        |\n";
    cout << BLUE_LIGHT "|       :#####:      ##     ##        ##:##:##    #::#          |\n";
    cout << CYAN_TEAL  "|        .#####:     ##     ##  ####  ##.##.##   ##  ##         |\n";
    cout << CYAN_TEAL  "|            :###    ##     ##. ####  ## ## ##   ######         |\n";
    cout << CYAN_TEAL  "|              ##    ##     ##:   ##  ##    ##  .######.        |\n";
    cout << ICE_BLUE   "|       #:.    :##    ##     :##:  ##  ##    ##  :##  ##:        |\n";
    cout << ICE_BLUE   "|       #######:    ######     #######  ##    ##  ###  ###        |\n";
    cout << ICE_BLUE   "|       .#####:     ######     :####.   ##    ##  ##:  :##        |\n";
    cout << RESET;
    cout << "+==============================================================+\n";
    cout << "|                SISTEM GUNA ABSENSI MAHASISWA                 |\n";
    cout << "+==============================================================+\n";
}

// ============================================================
//   LOGIKA MAHASISWA
// ============================================================
void reg_mhs() {
    system("cls");
    cetak_banner();
    if (jml_mhs < MAKS_MHS) {
    	// PENGGUNAAN POINTER SANGAT DISARANKAN
        Mahasiswa* mhs_baru = &data_mhs[jml_mhs];

        cout << "\nREGISTRASI MAHASISWA BARU\n";
        cout << "Masukan nama lengkap : ";
        getline(cin, mhs_baru->nama_mhs);
            
        cout << "Masukan NIM          : ";
        getline(cin, mhs_baru->nim);
                
        cout << "Masukan password     : ";
        getline(cin, mhs_baru->pass);
               
        cout << "Masukan Kelas (A/B/C/D): ";
        cin >> mhs_baru->kelas;
        cin.ignore();
              
        char daftar_kelas[4] = {'A', 'B', 'C', 'D'};
        mhs_baru->kelas = daftar_kelas[jml_mhs % 4];
        cout << "[Sistem] Kelas Anda otomatis ditetapkan: Kelas " << mhs_baru->kelas << "\n";
        
        mhs_baru->jml_mk = 0;
        jml_mhs++;
        cout << "\n[+] Registrasi mahasiswa berhasil!\n";
    } else {
        cout << "\n[-] Maaf, database penuh!\n";
    }
    system("pause");
}
 
int login_mhs() {
    system("cls");
    cetak_banner();
    string nim_login, pass_login;
    cout << "\nLOGIN MAHASISWA\n";
    cout << "Masukan NIM      : ";
    getline(cin, nim_login);
    cout << "Masukan password : ";
    getline(cin, pass_login);

    for (int i = 0; i < jml_mhs; i++) {
        if (nim_login == data_mhs[i].nim && pass_login == data_mhs[i].pass) {
            cout << "\n[+] Login berhasil! Selamat datang, " << data_mhs[i].nama_mhs << "!\n";
            system("pause");
            return i;
        }
    }
    cout << "\n[-] Login gagal! NIM atau Password salah.\n";
    system("pause");
    return -1;
}

void menu_mhs(int idx) {
    int pil;
    while(true) {
        system("cls");
        cout << "\n=== DASHBOARD MAHASISWA (" << data_mhs[idx].nama_mhs << " - Kelas " << data_mhs[idx].kelas << ") ===\n";
        cout << "1. Pilih Mata Kuliah\n";
        cout << "2. Isi Absensi Sesi Aktif\n";
        cout << "3. Lihat Riwayat Absensi Saya\n";
        cout << "0. Log Out\n";
        cout << "Pilihan: ";
        cin >> pil;
        cin.ignore();
        
        switch (pil){
            case 1:
                mhs_pilih_mk(idx);
                break;
            case 2:
                mhs_absen(idx);
                break;
            case 3:
                histori_mhs(idx);
                break;
            case 0:
                return;
            default:
                cout << "Pilihan tidak valid!\n";
                system("pause");
        
		}
   
   
    }
}

void mhs_pilih_mk(int idx) {
    system("cls");
    tampil_mk();
    cout << "\nPilih nomor mata kuliah yang ingin diambil: ";
    int pil; cin >> pil; cin.ignore();
    
    
	
	if (pil >= 1 && pil <= MAKS_MK) {
        int idx_mk = pil - 1;
        
        bool cek = false;
        for (int i = 0; i < data_mhs[idx].jml_mk; i++) {
            if (data_mhs[idx].mk_diambil[i] == idx_mk) {
                cek = true;
                break;
            }
        }
        
        if (!cek) {
            int pos = data_mhs[idx].jml_mk;
            data_mhs[idx].mk_diambil[pos] = idx_mk;
            data_mhs[idx].mk_pilihan = idx_mk; 
            data_mhs[idx].jml_mk++;
            cout << "[+] Berhasil mengambil mata kuliah: " << list_mk[idx_mk].nama_mk << "\n";
        } else {
            cout << "[-] Mata kuliah ini sudah Anda ambil sebelumnya.\n";
        }
    } else {
        cout << "[-] Pilihan tidak valid!\n";
    }
    system("pause");
}

void mhs_absen(int idx) {
    system("cls");
    if(!buka_absen) {
        cout << "\n[Sistem] Absensi belum dibuka oleh Dosen!\n";
        system("pause");
        return;
    }
    if(data_mhs[idx].mk_pilihan == -1) {
        cout << "\n[Sistem] Anda belum memilih mata kuliah di dashboard Anda!\n";
        system("pause");
        return;
    }
    if(data_mhs[idx].mk_pilihan != mk_aktif || data_mhs[idx].kelas != kls_aktif) {
        cout << "\n[Sistem] Tidak ada sesi absensi aktif untuk Mata Kuliah / Kelas Anda saat ini.\n";
        system("pause");
        return;
    }

    for(int i = 0; i < jml_absen; i++) {
        if(data_absen[i].nim == data_mhs[idx].nim &&
           data_absen[i].tgl == ambil_tgl() &&
           data_absen[i].matkul == list_mk[mk_aktif].nama_mk) {
             cout << "\n[Sistem] Anda SUDAH melakukan absensi pada hari ini!\n";
             system("pause");
             return;
        }
    }

    int pil_ket;
    cout << "\n===== PENGISIAN ABSENSI =====\n";
    cout << "Nama   : " << data_mhs[idx].nama_mhs << endl;
    cout << "Matkul : " << list_mk[mk_aktif].nama_mk << " (Kelas " << kls_aktif << ")\n";
    cout << "\nKeterangan Kehadiran:\n";
    cout << "1. Hadir\n";
    cout << "2. Izin\n";
    cout << "3. Sakit\n";
    cout << "Pilih status (1-3): ";
    cin >> pil_ket;
    cin.ignore();

    string simpan_ket;
    data_mhs[idx].status_absen = pil_ket; 
    
    switch(pil_ket) {
        case 1: simpan_ket = "Hadir"; break;
        case 2: simpan_ket = "Izin"; break;
        case 3: simpan_ket = "Sakit"; break;
        default:
            cout << "\nPilihan tidak valid! Absen dibatalkan.\n";
            system("pause");
            return;
    }

    data_absen[jml_absen].nim = data_mhs[idx].nim;
    data_absen[jml_absen].nama = data_mhs[idx].nama_mhs;
    data_absen[jml_absen].matkul = list_mk[mk_aktif].nama_mk;
    data_absen[jml_absen].kelas = data_mhs[idx].kelas;
    data_absen[jml_absen].tgl = ambil_tgl();
    data_absen[jml_absen].jam = ambil_jam();
    data_absen[jml_absen].ket = simpan_ket;
    jml_absen++;

    cout << "\n[Sistem] Absensi Anda (" << simpan_ket << ") berhasil disimpan!\n";
    system("pause");
}

void histori_mhs(int idx) {
    system("cls");
    bool ketemu = false;
    cout << "\n===== RIWAYAT ABSENSI ANDA =====\n";

    for(int i = 0; i < jml_absen; i++) {
        if(data_absen[i].nim == data_mhs[idx].nim) {
            ketemu = true;
            cout << "Tanggal  : " << data_absen[i].tgl << " (" << data_absen[i].jam << ")\n";
            cout << "Matkul   : " << data_absen[i].matkul << " (Kelas " << data_absen[i].kelas << ")\n";
            cout << "Status   : " << data_absen[i].ket << "\n";
            cout << "-------------------------------------------\n";
        }
    }
    if(!ketemu) {
        cout << "\nBelum ada riwayat absensi tercatat.\n";
    }
    system("pause");
}

// ============================================================
//   LOGIKA DOSEN
// ============================================================
void reg_dsn() {
    system("cls");
    cetak_banner();
    if (jml_dsn >= MAKS_DSN) {
        cout << "\n[-] Maaf, database dosen penuh!\n";
        system("pause");
        return;
    }
    
    Dosen* dsn_baru = &data_dsn[jml_dsn];
    
    cout << "\nREGISTRASI DOSEN BARU\n";
    cout << "Masukan nama     : ";
    getline(cin, dsn_baru->nama);
    string input_usn;
    cout << "Masukan username : ";
    getline(cin, input_usn);
    
    for (int i = 0; i < jml_dsn; i++) {
        if (data_dsn[i].usn == input_usn) {
            cout << "\n[-] Username sudah digunakan, coba username lain.\n";
            system("pause");
            return;
        }
    }
    dsn_baru->usn = input_usn;
    cout << "Masukan password : ";
    getline(cin, dsn_baru->pass);
    
    cout << "\n[+] Registrasi dosen berhasil!\n";
    jml_dsn++;
    
    cout << "+==============================================================+\n";
    cout << "  Nama     : " << dsn_baru->nama     << "\n";
    cout << "  Username : " << dsn_baru->usn << "\n";
    cout << "  Password : " << dsn_baru->pass << "\n";
    cout << "+==============================================================+\n";
    system("pause");
}

int login_dsn() {
    system("cls");
    cetak_banner();
    string usn, pass;
    cout << "\n===== LOGIN DOSEN =====\n";
    cout << "Username : ";
    getline(cin, usn);
    cout << "Password : ";
    getline(cin, pass);

    for(int i = 0; i < jml_dsn; i++) {
        if(usn == data_dsn[i].usn && pass == data_dsn[i].pass) {
            cout << "\n[+] Login Berhasil! Selamat datang, " << data_dsn[i].nama << "\n";
            system("pause");
            return i;
        }
    }
    cout << "\n[-] Username atau Password salah!\n";
    system("pause");
    return -1;
}

void menu_dsn(int idx) {
    int pil;
    while(true) {
        system("cls");
        cout << "\n=== DASHBOARD DOSEN (" << data_dsn[idx].nama << ") ===\n";
        if(data_dsn[idx].sedang_diampu != -1) {
            cout << "Matkul diampu : " << list_mk[data_dsn[idx].sedang_diampu].nama_mk 
                 << " (Kelas " << data_dsn[idx].kls_diampu << ")\n";
        } else {
            cout << "Matkul diampu : Belum memilih\n";
        }
        cout << "Status Absensi Berjalan: " << (buka_absen ? "TERBUKA" : "TERTUTUP") << "\n";
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

        if(pil == 1) dsn_pilih_mk(idx);
        else if(pil == 2) dsn_buka_absen(idx);
        else if(pil == 3) dsn_tutup_absen();
        else if(pil == 4) histori_dsn(idx);
        else if(pil == 5) menu_edit_mhs();
        else if(pil == 0) break;
        else {
            cout << "Pilihan tidak valid!\n";
            system("pause");
        }
    }
}

void dsn_pilih_mk(int idx) {
    system("cls");
    if (buka_absen && dsn_aktif == idx) {
        cout << "\n[Sistem] Tidak bisa mengganti mata kuliah saat sesi absensi masih aktif.\n";
        system("pause");
        return;
    }
    tampil_mk();
    cout << "\nPilih nomor mata kuliah yang ingin diampu: ";
    int pil; cin >> pil; cin.ignore();
    
    if (pil >= 1 && pil <= MAKS_MK) {
        int idx_mk = pil - 1;
        
        bool cek = false;
        for (int i = 0; i < data_dsn[idx].jml_diampu; i++) {
            if (data_dsn[idx].mk_diampu[i] == idx_mk) {
                cek = true;
                break;
            }
        }
        
        if (!cek) {
            int pos = data_dsn[idx].jml_diampu;
            data_dsn[idx].mk_diampu[pos] = idx_mk;
            data_dsn[idx].sedang_diampu = idx_mk; 
            data_dsn[idx].jml_diampu++;
            
            cout << "Masukkan kelas yang diampu (A/B/C/D): ";
            cin >> data_dsn[idx].kls_diampu;
            cin.ignore();
            
            cout << "[+] Berhasil mengampu mata kuliah: " << list_mk[idx_mk].nama_mk << "\n";
        } else {
            cout << "[-] Anda sudah mengampu mata kuliah ini.\n";
        }
    } else {
        cout << "[-] Pilihan tidak valid!\n";
    }
    system("pause");
}
void dsn_buka_absen(int idx) {
    system("cls");
    if (buka_absen) {
        cout << "\n[Sistem] Sudah ada sesi aktif. Tutup dulu sebelum membuka baru.\n";
        system("pause");
        return;
    }
    if (buka_absen) {
        cout << "\n[Sistem] Sudah ada sesi berjalan. Tutup dulu sebelum membuka lagi.\n";
        system("pause");
        return;
    }
    
    if (data_dsn[idx].sedang_diampu == -1) {
        cout << "\n[Sistem] Anda belum memilih mata kuliah!\n";
        system("pause");
        return;
    }

    buka_absen = true;
    dsn_aktif = idx;
    mk_aktif = data_dsn[idx].sedang_diampu;
    kls_aktif = data_dsn[idx].kls_diampu;

    cout << "\n===== ABSENSI DIBUKA =====\n";
    cout << "Mata Kuliah : " << list_mk[mk_aktif].nama_mk << "\n";
    cout << "Kelas       : " << kls_aktif << "\n";
    cout << "Tanggal     : " << ambil_tgl() << "\n";
    cout << "Waktu       : " << ambil_jam() << "\n";
    cout << "Status      : TERBUKA\n";

    system("pause");
}
void dsn_tutup_absen() {
    system("cls");

    if (!buka_absen) {
        cout << "\n[Sistem] Absensi sudah tertutup.\n";
        system("pause");
        return;
    }

    cout << "\n===== ABSENSI DITUTUP =====\n";
    cout << "Mata Kuliah : " << list_mk[mk_aktif].nama_mk << "\n";
    cout << "Kelas       : " << kls_aktif << "\n";
    cout << "Tanggal     : " << ambil_tgl() << "\n";
    cout << "Waktu       : " << ambil_jam() << "\n";
    cout << "Status      : TERTUTUP\n";

    buka_absen = false;
    dsn_aktif = -1;
    mk_aktif = -1;
    kls_aktif = '\0';

    system("pause");
}
void histori_dsn(int idx) {
    system("cls");
    
    if (data_dsn[idx].sedang_diampu == -1) {
        cout << "\n[Sistem] Anda belum memilih mata kuliah dan kelas yang diampu!\n";
        system("pause");
        return;
    }

    string mk_dsn = list_mk[data_dsn[idx].sedang_diampu].nama_mk;
    char kls_dsn = data_dsn[idx].kls_diampu;
    bool ketemu = false;

    cout << "\n===== RIWAYAT ABSENSI MAHASISWA =====\n";
    cout << "Mata Kuliah : " << mk_dsn << "\n";
    cout << "Kelas       : " << kls_dsn << "\n";
    cout << "-------------------------------------------\n";

    for(int i = 0; i < jml_absen; i++) {
        if(data_absen[i].matkul == mk_dsn && data_absen[i].kelas == kls_dsn) {
            ketemu = true;
            cout << "Tanggal  : " << data_absen[i].tgl << " (" << data_absen[i].jam << ")\n";
            cout << "NIM      : " << data_absen[i].nim << "\n";
            cout << "Nama     : " << data_absen[i].nama << "\n";
            cout << "Status   : " << data_absen[i].ket << "\n";
            cout << "-------------------------------------------\n";
        }
    }    
    if(!ketemu) {
        cout << "Belum ada data absensi mahasiswa untuk kelas ini.\n";
    }
    system("pause");
}

// =============================
// EDIT DAN HAPUS
// =============================

void menu_edit_mhs(){
    system ("cls");
    cout << "\n===== EDIT DATA MAHASISWA =====\n" << endl;
    cout << endl;
    cout << "1. Lihat Data Mahasiswa\n";
    cout << "2. Hapus Data Mahasiswa\n";
    cout << "3. Hapus Mata Kuliah Mahasiswa\n";
    cout << "0. Kembali\n";
    int pil;
    cout << "Pilihan: ";
    cin >> pil;
    
    switch (pil){
        case 1:
            lihat_data_mhs();
            system ("pause");
            break;
        case 2:
            hapus_mhs();
            break;
        case 3:
            hapus_mk_mhs();
            break;
        case 0:
            return;
        default:
            cout << "Input tidak valid!";
    }
}

void lihat_data_mhs(){
    cout << "\n===== DATA MAHASISWA =====\n";

    if (jml_mhs == 0) {
        cout << "Belum ada data mahasiswa.\n";
        return;
    }
    for (int i = 0; i < jml_mhs; i++) {
        cout << i + 1 << ". "
             << data_mhs[i].nama_mhs
             << " | NIM : " << data_mhs[i].nim
             << " | Kelas : " << data_mhs[i].kelas
             << endl;
    }
    
}
void hapus_mhs() {
    system("cls");
    
	cout << "\n===== DATA MAHASISWA =====\n";

    if (jml_mhs == 0) {
        cout << "Belum ada data mahasiswa.\n";
        system("pause");
        return;
    }
    for (int i = 0; i < jml_mhs; i++) {
        cout << i + 1 << ". "
             << data_mhs[i].nama_mhs
             << " | NIM : " << data_mhs[i].nim
             << " | Kelas : " << data_mhs[i].kelas
             << endl;
    }
    int pil;
    cout << "\nPilih nomor mahasiswa yang akan dihapus : ";
    cin >> pil;
    cin.ignore();

    if (pil < 1 || pil > jml_mhs) {
        cout << "\nPilihan tidak valid!\n";
        system("pause");
        return;
    }
    pil--; 

    for (int i = pil; i < jml_mhs - 1; i++) {
        data_mhs[i] = data_mhs[i + 1];
    }
    jml_mhs--;

    cout << "\n[+] Data mahasiswa berhasil dihapus.\n";
    system("pause");
}
void hapus_mk_mhs() {
    system("cls");
   
    lihat_data_mhs();

    if (jml_mhs == 0) {
        system("pause");
        return;
    }
    int pil_mhs;
    cout << "\nPilih nomor mahasiswa : ";
    cin >> pil_mhs;
    cin.ignore();

    if (pil_mhs < 1 || pil_mhs > jml_mhs) {
        cout << "\nPilihan tidak valid!\n";
        system("pause");
        return;
    }

    pil_mhs--;

    if (data_mhs[pil_mhs].jml_mk == 0) {
        cout << "\nMahasiswa belum mengambil mata kuliah.\n";
        system("pause");
        return;
    }

    cout << "\n===== DAFTAR MATA KULIAH =====\n";

    for (int i = 0; i < data_mhs[pil_mhs].jml_mk; i++) {
        int idx = data_mhs[pil_mhs].mk_diambil[i];
       
	    cout << i + 1 << ". " 
			 << list_mk[idx].nama_mk 
			 << endl;
    }

    int pil_mk;
    
    cout << "\nPilih mata kuliah yang akan dihapus : ";
    cin >> pil_mk;
    cin.ignore();

    if (pil_mk < 1 || 
		pil_mk > data_mhs[pil_mhs].jml_mk) {
        
		cout << "\nPilihan tidak valid!\n";
        system("pause");
        return;
    }

    pil_mk--;

    for (int i = pil_mk; 
		i < data_mhs[pil_mhs].jml_mk - 1;
		i++) {
        
		data_mhs[pil_mhs].mk_diambil[i] = 
		data_mhs[pil_mhs].mk_diambil[i + 1];
    }

    data_mhs[pil_mhs].jml_mk--;

    if (data_mhs[pil_mhs].jml_mk == 0) {
        data_mhs[pil_mhs].mk_pilihan = -1;
    }

    cout << "\n[+] Mata kuliah berhasil dihapus.\n";
    system("pause");
}
// ============================================================
//   MAIN PROGRAM
// ============================================================
int main() {
    int menu_awal;
    while (true) {
        system("cls");
        cetak_banner();
        cout << "Masuk sebagai:\n";
        cout << "1. Mahasiswa\n";
        cout << "2. Dosen\n";
        cout << "0. Keluar Aplikasi\n";
        cout << "Pilihan: ";
        cin >> menu_awal;
        cin.ignore();

        if (menu_awal == 0) {
            cout << "\nTerima kasih telah menggunakan sistem ini!\n";
            break;
        }

        if (menu_awal == 1) {
            int pil;
            while (true) {
                system("cls");
                cetak_banner();
                cout << "\n[MENU UTAMA MAHASISWA]\n";
                cout << "1. Sign Up (Registrasi)\n";
                cout << "2. Sign In (Login)\n";
                cout << "0. Kembali ke Menu Utama\n";
                cout << "Pilihan: ";
                cin >> pil;
                cin.ignore();

                if (pil == 1) {
                    reg_mhs();
                } else if (pil == 2) {
                    int id = login_mhs();
                    if (id != -1) {
                        menu_mhs(id);
                    }
                } else if (pil == 0) {
                    break;
                } else {
                    cout << "\n[ERROR] Pilihan tidak valid!\n";
                    system("pause");
                }
            }
        } else if (menu_awal == 2) {
            int pil;
            while (true) {
                system("cls");
                cetak_banner();
                cout << "\n[MENU UTAMA DOSEN]\n";
                cout << "1. Sign Up (Registrasi)\n";
                cout << "2. Sign In (Login)\n";
                cout << "0. Kembali ke Menu Utama\n";
                cout << "Pilihan: ";
                cin >> pil;
                cin.ignore();

                if (pil == 1) {
                    reg_dsn();
                } else if (pil == 2) {
                    int id = login_dsn();
                    if (id != -1) {
                        menu_dsn(id);
                    }
                } else if (pil == 0) {
                    break;
                } else {
                    cout << "\n[ERROR] Pilihan tidak valid!\n";
                    system("pause");
                }
            }
        } else {
            cout << "\n[ERROR] Pilihan tidak valid!\n";
            system("pause");
        }
    }
    return 0;
}s