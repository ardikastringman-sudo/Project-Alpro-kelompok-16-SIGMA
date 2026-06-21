#include <iostream>
#include <string>
#include <ctime>
#include <conio.h>

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
#define RED         "\033[31m"
#define RESET       "\033[0m"

// ============================================================
//   STRUCTS
// ============================================================
struct mata_kuliah {
    int id;
    string nama_mk;
};

struct Mahasiswa {
    string nim;
    string nama_mhs;
    string pass;
    char kelas;
    mata_kuliah* mk_diambil[MAKS_MK];   // pointer ke setiap matkul yg diambil
    int jml_mk = 0;
    mata_kuliah* mk_pilihan = nullptr;  // pointer ke matkul yg sedang aktif
    int status_absen = 0;

    void signup_mahasiswa(string input_nama,
                         string input_NIM,
                         string input_password,
                         char input_kelas) {
        nama_mhs = input_nama;
        nim = input_NIM;
        pass = input_password;
        kelas = input_kelas;
    }

    bool login_mahasiswa(const string &nim_login,
                        const string &pass_login) const {
        return (nim == nim_login &&
                pass == pass_login);
    }
};

struct Dosen {
    string nama;
    string usn;
    string pass;
    mata_kuliah* mk_diampu[MAKS_MK];     // pointer ke matkul yg diampu
    int jml_diampu = 0;
    mata_kuliah* sedang_diampu = nullptr; // pointer ke matkul aktif diampu
    char kls_diampu;

    void signup_dosen(string input_nama, string input_password, string input_username) {
        nama = input_nama;
        pass = input_password;
        usn = input_username;
    }
    bool login_dosen(const string &usn_login, const string &pass_login) const {
        return (usn == usn_login && pass == pass_login);
    }
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
//   DATABASE GLOBAL (TERMASUK DATA DUMMY BERDASARKAN MATKUL)
// ============================================================
mata_kuliah list_mk[MAKS_MK] = {
    {1, "Algoritma dan Pemrograman"},
    {2, "Sistem Digital"},
    {3, "Aljabar Linear"},
    {4, "Komputer dan Masyarakat"},
    {5, "Matematika Diskrit"}
};

// Data Dummy Mahasiswa yang dikelompokkan mengambil kelas 'A' Matkul 'Algoritma dan Pemrograman'
Mahasiswa data_mhs[MAKS_MHS] = {
    {"F1D02510001", "I Made Duta Atmajaya", "mhs123", 'A', {&list_mk[0], &list_mk[1]}, 2, &list_mk[0], 1},
    {"F1D02510002", "Atika Rahmadani", "mhs123", 'A', {&list_mk[0], &list_mk[2]}, 2, &list_mk[0], 1},
    {"F1D02510003", "Muzakir Mataho", "mhs123", 'A', {&list_mk[0]}, 1, &list_mk[0], 0}, // Mataho belum melakukan pengisian absen hari ini
    {"F1D02510004", "Aura Permata Grasya", "mhs123", 'A', {&list_mk[0], &list_mk[1]}, 2, &list_mk[0], 1},
    {"F1D02510005", "Adelya Ega Syafa", "mhs123", 'A', {&list_mk[0], &list_mk[3]}, 2, &list_mk[0], 1},
    {"F1D02510006", "Evalinda Gracia Faozi", "mhs123", 'A', {&list_mk[0], &list_mk[1]}, 2, &list_mk[0], 1}
};
int jml_mhs = 6;

// Data Dummy Dosen default mengampu Algoritma dan Pemrograman (Kelas A)
// Login: Username = dsn1 , Password = dsn123
Dosen data_dsn[MAKS_DSN] = {
    {"Dr. Ardika Prasetyo, S.T., M.T.", "dsn1", "dsn123", {&list_mk[0]}, 1, &list_mk[0], 'A'}
};
int jml_dsn = 1;

// Data Dummy Log Absensi Mahasiswa di sistem Dosen
Absensi data_absen[MAKS_ABSEN] = {
    {"F1D02510001", "I Made Duta Atmajaya", "Algoritma dan Pemrograman", 'A', "19-06-2026", "08:15:22", "Hadir"},
    {"F1D02510002", "Atika Rahmadani", "Algoritma dan Pemrograman", 'A', "19-06-2026", "08:18:45", "Hadir"},
    {"F1D02510004", "Aura Permata Grasya", "Algoritma dan Pemrograman", 'A', "19-06-2026", "08:19:30", "Hadir"},
    {"F1D02510005", "Adelya Ega Syafa", "Algoritma dan Pemrograman", 'A', "19-06-2026", "08:20:10", "Izin"},
    {"F1D02510006", "Evalinda Gracia Faozi", "Algoritma dan Pemrograman", 'A', "19-06-2026", "08:22:30", "Sakit"}
};
int jml_absen = 5;

bool buka_absen = false;
Dosen* dsn_aktif = nullptr;        // pointer ke dosen pemilik sesi aktif
mata_kuliah* mk_aktif = nullptr;    // pointer ke matkul yang sesi-nya aktif
char kls_aktif;

// ============================================================
//   PROTOTIPE FUNGSI
// ============================================================
void cetak_banner();

void menu_mahasiswa(Mahasiswa* mhs);
void mhs_pilih_mk(Mahasiswa* mhs);
void mhs_absen(Mahasiswa* mhs);
void histori_mhs(Mahasiswa* mhs);

void menu_dosen(Dosen* dsn);
void pilih_matkul_dosen(Dosen* dsn);
void dsn_buka_absen(Dosen* dsn);
void tutup_absensi();
void riwayat_dosen(Dosen* dsn);

void tampil_matkul();
void menu_edit_mhs();
void lihat_data_mhs();
void hapus_mahasiswa();
void hapus_matkul_mahasiswa();
string ambil_tgl();
string ambil_jam();
void menu_utama();

Mahasiswa* cari_login_mahasiswa(const string &nim, const string &pass);
Dosen* cari_login_dosen(const string &usn, const string &pass);

// Fungsi Perapi Tampilan Tengah
string format_teks_tengah(string teks, int lebar_kotak);
void cetak_baris_tengah(string baris_teks, int lebar_terminal);

// ============================================================
//   FUNGSI UTILITAS / HELPERS TAMPILAN TENGAH
// ============================================================
string format_teks_tengah(string teks, int lebar_kotak) {
    int panjang_murni = 0;
    for (size_t i = 0; i < teks.length(); i++) {
        if (teks[i] == '\033') {
            while (i < teks.length() && teks[i] != 'm') {
                i++;
            }
        } else {
            panjang_murni++;
        }
    }

    if (panjang_murni >= lebar_kotak) return teks;

    int sisa_spasi = lebar_kotak - panjang_murni;
    int spasi_kiri = sisa_spasi / 2;
    int spasi_kanan = sisa_spasi - spasi_kiri;

    string hasil = "";
    for (int i = 0; i < spasi_kiri; i++) {
        hasil += " ";
    }
    hasil += teks;
    for (int i = 0; i < spasi_kanan; i++) {
        hasil += " ";
    }
    return hasil;
}

void cetak_baris_tengah(string baris_teks, int lebar_terminal) {
    int panjang_murni = 0;
    for (size_t i = 0; i < baris_teks.length(); i++) {
        if (baris_teks[i] == '\033') {
            while (i < baris_teks.length() && baris_teks[i] != 'm') {
                i++;
            }
        } else {
            panjang_murni++;
        }
    }

    int spasi_pendorong = (lebar_terminal - panjang_murni) / 2;
    if (spasi_pendorong < 0) spasi_pendorong = 0;

    for (int i = 0; i < spasi_pendorong; i++) {
        cout << " ";
    }
    cout << baris_teks << endl;
}

string ambil_tgl() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char tanggal[20];
    sprintf(tanggal, "%02d-%02d-%04d", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
    return string(tanggal);
}

string ambil_jam() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char waktu[20];
    sprintf(waktu, "%02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return string(waktu);
}

void tampil_matkul() {
    cout << "\n===== DAFTAR MATA KULIAH =====\n";
    mata_kuliah* p = list_mk;
    for (int i = 0; i < MAKS_MK; i++, p++) {
        cout << p->id << ". " << p->nama_mk << endl;
    }
}

Mahasiswa* cari_login_mahasiswa(const string &nim, const string &pass) {
    Mahasiswa* p = data_mhs;
    for (int i = 0; i < jml_mhs; i++, p++) {
        if (p->login_mahasiswa(nim, pass)) {
            return p;
        }
    }
    return nullptr;
}

Dosen* cari_login_dosen(const string &usn, const string &pass) {
    Dosen* p = data_dsn;
    for (int i = 0; i < jml_dsn; i++, p++) {
        if (p->login_dosen(usn, pass)) {
            return p;
        }
    }
    return nullptr;
}

void cetak_banner() {
    cout << "+==============================================================+\n";
    cout << NAVY_BLUE  "|          :####:   ######   :####:   ###  ###     :##:        |\n";
    cout << NAVY_BLUE  "|         :######   ######   ######   ###  ###      ##         |\n";
    cout << NAVY_BLUE  "|        ##:   :#     ##    :##: .#   ###::###     ####        |\n";
    cout << BLUE_LIGHT "|        ##           ##    ##:       ###  ###     ####        |\n";
    cout << BLUE_LIGHT "|        ###:         ##    ##.       ## ## ##    :#  #:       |\n";
    cout << BLUE_LIGHT "|        :#####:      ##    ##        ##:##:##     #::#        |\n";
    cout << CYAN_TEAL  "|         .#####:     ##    ##  ####  ##.##.##    ##  ##       |\n";
    cout << CYAN_TEAL  "|             :###    ##    ##. ####  ## ## ##    ######       |\n";
    cout << CYAN_TEAL  "|               ##    ##    ##:   ##  ##    ##   .######.      |\n";
    cout << ICE_BLUE   "|        #:.    :##   ##   :##:   ##  ##    ##   :##  ##:      |\n";
    cout << ICE_BLUE   "|         #######:  ######  #######   ##    ##   ###  ###      |\n";
    cout << ICE_BLUE   "|         .#####:   ######   :####.   ##    ##   ##:  :##      |\n";
    cout << RESET;
    cout << "+==============================================================+\n";
    cout << NAVY_BLUE "|                SISTEM GUNA ABSENSI MAHASISWA                 |\n";
    cout << RESET;
    cout << "+==============================================================+\n";
}

// ============================================================
//   MAHASISWA
// ============================================================
void menu_mahasiswa(Mahasiswa* mhs) {
    int pil;
    while (true) {
        system("cls");
        cetak_banner();
        cout << "\n=== DASHBOARD MAHASISWA (" << mhs->nama_mhs << " - Kelas " << mhs->kelas << ") ===\n";
        cout << "1. Pilih Mata Kuliah\n";
        cout << "2. Isi Absensi Sesi Aktif\n";
        cout << "3. Lihat Riwayat Absensi Saya\n";
        cout << "0. Log Out\n";
        cout << "Pilihan: ";
        cin >> pil;
        cin.ignore();

        switch (pil) {
            case 1:
                mhs_pilih_mk(mhs);
                break;
            case 2:
                mhs_absen(mhs);
                break;
            case 3:
                histori_mhs(mhs);
                break;
            case 0:
                return;
            default:
                cout << "Pilihan tidak valid!\n";
                system("pause");
        }
    }
}

void mhs_pilih_mk(Mahasiswa* mhs) {
    system("cls");
    cetak_banner();
    tampil_matkul();
    cout << "\nPilih nomor mata kuliah yang ingin diambil: ";
    int pil; cin >> pil; cin.ignore();

    if (pil >= 1 && pil <= MAKS_MK) {
        mata_kuliah* mk = list_mk + (pil - 1);

        bool cek = false;
        for (int i = 0; i < mhs->jml_mk; i++) {
            if (mhs->mk_diambil[i] == mk) {
                cek = true;
                break;
            }
        }

        if (!cek) {
            int pos = mhs->jml_mk;
            mhs->mk_diambil[pos] = mk;
            mhs->mk_pilihan = mk;
            mhs->jml_mk++;
            cout << "[+] Berhasil mengambil mata kuliah: " << mk->nama_mk << "\n";
        } else {
            cout << "[-] Mata kuliah ini sudah Anda ambil sebelumnya.\n";
        }
    } else {
        cout << "[-] Pilihan tidak valid!\n";
    }
    system("pause");
}

void mhs_absen(Mahasiswa* mhs) {
    system("cls");
    cetak_banner();
    if (!buka_absen) {
        cout << "\n[Sistem] Absensi belum dibuka oleh Dosen!\n";
        system("pause");
        return;
    }
    if (mhs->mk_pilihan == nullptr) {
        cout << "\n[Sistem] Anda belum memilih mata kuliah di dashboard Anda!\n";
        system("pause");
        return;
    }
    if (mhs->mk_pilihan != mk_aktif || mhs->kelas != kls_aktif) {
        cout << "\n[Sistem] Tidak ada sesi absensi aktif untuk Mata Kuliah / Kelas Anda saat ini.\n";
        system("pause");
        return;
    }

    string tgl_skrg = ambil_tgl();
    Absensi* p = data_absen;
    for (int i = 0; i < jml_absen; i++, p++) {
        if (p->nim == mhs->nim &&
            p->tgl == tgl_skrg &&
            p->matkul == mk_aktif->nama_mk) {
            cout << "\n[Sistem] Anda SUDAH melakukan absensi pada hari ini!\n";
            system("pause");
            return;
        }
    }

    int pil_ket;
    cout << "\n===== PENGISIAN ABSENSI =====\n";
    cout << "Nama   : " << mhs->nama_mhs << endl;
    cout << "Matkul : " << mk_aktif->nama_mk << " (Kelas " << kls_aktif << ")\n";
    cout << "\nKeterangan Kehadiran:\n";
    cout << "1. Hadir\n";
    cout << "2. Izin\n";
    cout << "3. Sakit\n";
    cout << "Pilih status (1-3): ";
    cin >> pil_ket;
    cin.ignore();

    string ket_status;
    mhs->status_absen = pil_ket;

    switch (pil_ket) {
        case 1: ket_status = "Hadir"; break;
        case 2: ket_status = "Izin"; break;
        case 3: ket_status = "Sakit"; break;
        default:
            cout << "\nPilihan tidak valid! Absen dibatalkan.\n";
            system("pause");
            return;
    }

    Absensi* baru = &data_absen[jml_absen];
    baru->nim = mhs->nim;
    baru->nama = mhs->nama_mhs;
    baru->matkul = mk_aktif->nama_mk;
    baru->kelas = mhs->kelas;
    baru->tgl = ambil_tgl();
    baru->jam = ambil_jam();
    baru->ket = ket_status;
    jml_absen++;

    cout << "\n[Sistem] Absensi Anda (" << ket_status << ") berhasil disimpan!\n";
    system("pause");
}

void histori_mhs(Mahasiswa* mhs) {
    system("cls");
    cetak_banner();
    bool ditemukan = false;
    cout << "\n===== RIWAYAT ABSENSI ANDA =====\n";

    Absensi* p = data_absen;
    for (int i = 0; i < jml_absen; i++, p++) {
        if (p->nim == mhs->nim) {
            ditemukan = true;
            cout << "Tanggal  : " << p->tgl << " (" << p->jam << ")\n";
            cout << "Matkul   : " << p->matkul << " (Kelas " << p->kelas << ")\n";
            cout << "Status   : " << p->ket << "\n";
            cout << "-------------------------------------------\n";
        }
    }
    if (!ditemukan) {
        cout << "\nBelum ada riwayat absensi tercatat.\n";
    }
    system("pause");
}

// ============================================================
//   LOGIKA DOSEN
// ============================================================
void menu_dosen(Dosen* dsn) {
    int pil;
    while (true) {
        system("cls");
        cetak_banner();
        cout << "\n=== DASHBOARD DOSEN (" << dsn->nama << ") ===\n";
        if (dsn->sedang_diampu != nullptr) {
            cout << "Matkul diampu : " << dsn->sedang_diampu->nama_mk
                 << " (Kelas " << dsn->kls_diampu << ")\n";
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

        if (pil == 1) pilih_matkul_dosen(dsn);
        else if (pil == 2) dsn_buka_absen(dsn);
        else if (pil == 3) tutup_absensi();
        else if (pil == 4) riwayat_dosen(dsn);
        else if (pil == 5) menu_edit_mhs();
        else if (pil == 0) break;
        else {
            cout << "Pilihan tidak valid!\n";
            system("pause");
        }
    }
}

void pilih_matkul_dosen(Dosen* dsn) {
    system("cls");
    cetak_banner();
    if (buka_absen && dsn_aktif == dsn) {
        cout << "\n[Sistem] Tidak bisa mengganti mata kuliah saat sesi absensi masih aktif.\n";
        system("pause");
        return;
    }
    tampil_matkul();
    cout << "\nPilih nomor mata kuliah yang ingin diampu: ";
    int pil; cin >> pil; cin.ignore();

    if (pil >= 1 && pil <= MAKS_MK) {
        mata_kuliah* mk = list_mk + (pil - 1);

        // Cek apakah sudah ada di daftar ampunan dosen
        bool cek = false;
        for (int i = 0; i < dsn->jml_diampu; i++) {
            if (dsn->mk_diampu[i] == mk) {
                cek = true;
                break;
            }
        }

        // Jika BELUM pernah diambil, masukkan ke dalam daftar riwayat ampunan dosen
        if (!cek) {
            int pos = dsn->jml_diampu;
            dsn->mk_diampu[pos] = mk;
            dsn->jml_diampu++;
        }

        // perbarui matkul yang SEDANG AKTIF diampu saat ini
        dsn->sedang_diampu = mk;

        cout << "Masukkan kelas yang diampu (A/B/C/D): ";
        cin >> dsn->kls_diampu;
        cin.ignore();

        cout << "[+] Berhasil mengaktifkan mata kuliah: " << mk->nama_mk << " (Kelas " << dsn->kls_diampu << ")\n";
    } else {
        cout << "[-] Pilihan tidak valid!\n";
    }
    system("pause");
}

void dsn_buka_absen(Dosen* dsn) {
    system("cls");
    cetak_banner();
    if (buka_absen) {
        cout << "\n[Sistem] Sudah ada sesi aktif. Tutup dulu sebelum membuka baru.\n";
        system("pause");
        return;
    }

    if (dsn->sedang_diampu == nullptr) {
        cout << "\n[Sistem] Anda belum memilih mata kuliah!\n";
        system("pause");
        return;
    }

    buka_absen = true;
    dsn_aktif = dsn;
    mk_aktif = dsn->sedang_diampu;
    kls_aktif = dsn->kls_diampu;

    cout << "\n===== ABSENSI DIBUKA =====\n";
    cout << "Mata Kuliah : " << mk_aktif->nama_mk << "\n";
    cout << "Kelas       : " << kls_aktif << "\n";
    cout << "Tanggal     : " << ambil_tgl() << "\n";
    cout << "Waktu       : " << ambil_jam() << "\n";
    cout << "Status      : TERBUKA\n";

    system("pause");
}

void tutup_absensi() {
    system("cls");
    cetak_banner();

    if (!buka_absen) {
        cout << "\n[Sistem] Absensi sudah tertutup.\n";
        system("pause");
        return;
    }

    cout << "\n===== ABSENSI DITUTUP =====\n";
    cout << "Mata Kuliah : " << mk_aktif->nama_mk << "\n";
    cout << "Kelas       : " << kls_aktif << "\n";
    cout << "Tanggal     : " << ambil_tgl() << "\n";
    cout << "Waktu       : " << ambil_jam() << "\n";
    cout << "Status      : TERTUTUP\n";

    buka_absen = false;
    dsn_aktif = nullptr;
    mk_aktif = nullptr;
    kls_aktif = '\0';

    system("pause");
}

void riwayat_dosen(Dosen* dsn) {
    system("cls");
    cetak_banner();

    if (dsn->sedang_diampu == nullptr) {
        cout << "\n[Sistem] Anda belum memilih mata kuliah dan kelas yang diampu!\n";
        system("pause");
        return;
    }

    mata_kuliah* matkul_dosen = dsn->sedang_diampu;
    char kelas_dosen = dsn->kls_diampu;

    cout << "\n===== STATUS PRESENSI KELAS =====\n";
    cout << "Mata Kuliah : " << matkul_dosen->nama_mk << "\n";
    cout << "Kelas       : " << kelas_dosen << "\n";
    cout << "-------------------------------------------\n";

    bool ada_mahasiswa = false;

    // Loop data_mhs untuk melacak siapa saja yang mengambil matkul dosen bersangkutan
    for (int i = 0; i < jml_mhs; i++) {
        Mahasiswa* m = &data_mhs[i];
        
        if (m->kelas == kelas_dosen) {
            bool mengambil_matkul = false;
            for (int j = 0; j < m->jml_mk; j++) {
                if (m->mk_diambil[j] == matkul_dosen) {
                    mengambil_matkul = true;
                    break;
                }
            }

            if (mengambil_matkul) {
                ada_mahasiswa = true;
                cout << "NIM  : " << m->nim << "\n";
                cout << "Nama : " << m->nama_mhs << "\n";

                // Cek apakah mahasiswa ini datanya ada di log array data_absen
                bool sudah_absen = false;
                string status_kehadiran = "Belum Mengisi Absen";
                string waktu_absen = "-";

                for (int k = 0; k < jml_absen; k++) {
                    if (data_absen[k].nim == m->nim && data_absen[k].matkul == matkul_dosen->nama_mk) {
                        sudah_absen = true;
                        status_kehadiran = data_absen[k].ket;
                        waktu_absen = data_absen[k].tgl + " " + data_absen[k].jam;
                        break;
                    }
                }

                cout << "Log  : " << waktu_absen << "\n";
                if (sudah_absen) {
                    cout << "Status: " << string(CYAN_TEAL) << status_kehadiran << string(RESET) << "\n";
                } else {
                    cout << "Status: " << string(RED) << status_kehadiran << string(RESET) << "\n";
                }
                cout << "-------------------------------------------\n";
            }
        }
    }

    if (!ada_mahasiswa) {
        cout << "Belum ada data mahasiswa terdaftar yang mengambil kelas ini.\n";
    }
    system("pause");
}

// ============================================================
//   EDIT DAN HAPUS
// ============================================================
void menu_edit_mhs() {
    system("cls");
    cetak_banner();
    cout << "\n===== EDIT DATA MAHASISWA =====\n" << endl;
    cout << "1. Lihat Data Mahasiswa\n";
    cout << "2. Hapus Data Mahasiswa\n";
    cout << "3. Hapus Mata Kuliah Mahasiswa\n";
    cout << "0. Kembali\n";
    int pil;
    cout << "Pilihan: ";
    cin >> pil;

    switch (pil) {
        case 1:
            system ("cls");
            lihat_data_mhs();
            system("pause");
            break;
        case 2:
            hapus_mahasiswa();
            break;
        case 3:
            system("cls");
            hapus_matkul_mahasiswa();
            break;
        case 0:
            return;
        default:
            cout << "Input tidak valid!";
    }
}

void lihat_data_mhs() {
    cetak_banner();
    cout << "\n===== DATA MAHASISWA =====\n";

    if (jml_mhs == 0) {
        cout << "Belum ada data mahasiswa.\n";
        return;
    }

    Mahasiswa* p = data_mhs;
    for (int i = 0; i < jml_mhs; i++, p++) {
        cout << i + 1 << ". "
             << p->nama_mhs
             << " | NIM : " << p->nim
             << " | Kelas : " << p->kelas
             << endl;
    }
}

void hapus_mahasiswa() {
    system("cls");
    cetak_banner();
    cout << "\n===== DATA MAHASISWA =====\n";

    if (jml_mhs == 0) {
        cout << "Belum ada data mahasiswa.\n";
        system("pause");
        return;
    }

    Mahasiswa* p = data_mhs;
    for (int i = 0; i < jml_mhs; i++, p++) {
        cout << i + 1 << ". "
             << p->nama_mhs
             << " | NIM : " << p->nim
             << " | Kelas : " << p->kelas
             << endl;
    }

    int pil;
    cout << "\nPilihan nomor mahasiswa yang akan dihapus : ";
    cin >> pil;
    cin.ignore();

    if (pil < 1 || pil > jml_mhs) {
        cout << "\nPilihan tidak valid!\n";
        system("pause");
        return;
    }

    Mahasiswa* target = data_mhs + (pil - 1);
    Mahasiswa* akhir  = data_mhs + (jml_mhs - 1);

    for (Mahasiswa* q = target; q < akhir; q++) {
        *q = *(q + 1);
    }
    jml_mhs--;

    cout << "\n[+] Data mahasiswa berhasil dihapus.\n";
    system("pause");
}

void hapus_matkul_mahasiswa() {
    system("cls");
    lihat_data_mhs();

    if (jml_mhs == 0) {
        system("pause");
        return;
    }
    int pil;
    cout << "\nPilih nomor mahasiswa : ";
    cin >> pil;
    cin.ignore();

    if (pil < 1 || pil > jml_mhs) {
        cout << "\nPilihan tidak valid!\n";
        system("pause");
        return;
    }

    Mahasiswa* mhs = data_mhs + (pil - 1);

    if (mhs->jml_mk == 0) {
        cout << "\nMahasiswa belum mengambil mata kuliah.\n";
        system("pause");
        return;
    }

    cout << "\n===== DAFTAR MATA KULIAH =====\n";
    for (int i = 0; i < mhs->jml_mk; i++) {
        mata_kuliah* mk = mhs->mk_diambil[i];
        cout << i + 1 << ". " << mk->nama_mk << endl;
    }

    int pil_mk;
    cout << "\nPilih mata kuliah yang akan dihapus : ";
    cin >> pil_mk;
    cin.ignore();

    if (pil_mk < 1 || pil_mk > mhs->jml_mk) {
        cout << "\nPilihan tidak valid!\n";
        system("pause");
        return;
    }

    mata_kuliah** target = mhs->mk_diambil + (pil_mk - 1);
    mata_kuliah** akhir  = mhs->mk_diambil + (mhs->jml_mk - 1);

    for (mata_kuliah** q = target; q < akhir; q++) {
        *q = *(q + 1);
    }

    mhs->jml_mk--;

    if (mhs->jml_mk == 0) {
        mhs->mk_pilihan = nullptr;
    }

    cout << "\n[+] Mata kuliah berhasil dihapus.\n";
    system("pause");
}

// ============================================================
//   FUNGSI MENU UTAMA & SUBMENU
// ============================================================
void menu_utama() {
    string opsi[] = {
        "MASUK SEBAGAI MAHASISWA",
        "  MASUK SEBAGAI DOSEN  ",
        "    KELUAR APLIKASI    "
    };

    string opsi_sub_menu[] = {
        "SIGN UP (REGISTRASI)",
        "  SIGN IN (LOGIN)   ",
        "      KEMBALI       "
    };

    int sel = 0;
    char key;
    const int total_menu = 3;
    
    int lebar_kotak = 46;       
    int lebar_terminal = 66;   

    while (true) {
        system("cls");
        cetak_banner();
        
        string info_str = "Data Mhs: " + to_string(jml_mhs) + "/" + to_string(MAKS_MHS) + "  |  Panah: navigasi  |  Enter: pilih";
        cetak_baris_tengah(info_str, lebar_terminal);
        cetak_baris_tengah("---------------------------------------------------------------", lebar_terminal);
        cout << endl;

        string pembatas = "+";
        for(int i = 0; i < lebar_kotak; i++) pembatas += "=";
        pembatas += "+";

        for (int i = 0; i < total_menu; i++) {
            cetak_baris_tengah(pembatas, lebar_terminal);
            if (i == sel) {
                string format_opsi = " >> " + opsi[i] + " << ";
                if (i == 2) {
                    cetak_baris_tengah("|" + format_teks_tengah(string(RED) + format_opsi + RESET, lebar_kotak) + "|", lebar_terminal);
                } else {
                    cetak_baris_tengah("|" + format_teks_tengah(string(BLUE_LIGHT) + format_opsi + RESET, lebar_kotak) + "|", lebar_terminal);
                }
            } else {
                cetak_baris_tengah("|" + format_teks_tengah(opsi[i], lebar_kotak) + "|", lebar_terminal);
            }
        }
        cetak_baris_tengah(pembatas, lebar_terminal);
        cout << endl;
        cetak_baris_tengah("---------------------------------------------------------------", lebar_terminal);

        key = _getch();
        if (key == 0 || key == (char)224) {
            key = _getch();
            if (key == 72) { sel--; if (sel < 0) sel = total_menu - 1; } 
            if (key == 80) { sel++; if (sel >= total_menu) sel = 0; }    
        }
        else if (key == 13 || key == 32) { 
            if (sel == 2) {
                system("cls");
                cout << "Terima kasih telah menggunakan SIGMA!\n";
                system("pause");
                break;
            }

            // ---- SUBMENU MAHASISWA ----
            if (sel == 0) {
                int sel_sub = 0;
                while (true) {
                    system("cls");
                    cetak_banner();
                    cout << endl;
                    cetak_baris_tengah("[MENU UTAMA MAHASISWA]", lebar_terminal);
                    cout << endl;
                    
                    for (int i = 0; i < total_menu; i++) {
                        cetak_baris_tengah(pembatas, lebar_terminal);
                        if (i == sel_sub) {
                            string format_sub = " >> " + opsi_sub_menu[i] + " << ";
                            if (i == 2) {
                                cetak_baris_tengah("|" + format_teks_tengah(string(RED) + format_sub + RESET, lebar_kotak) + "|", lebar_terminal);
                            } else {
                                cetak_baris_tengah("|" + format_teks_tengah(string(BLUE_LIGHT) + format_sub + RESET, lebar_kotak) + "|", lebar_terminal);
                            }
                        } else {
                            cetak_baris_tengah("|" + format_teks_tengah(opsi_sub_menu[i], lebar_kotak) + "|", lebar_terminal);
                        }
                    }
                    cetak_baris_tengah(pembatas, lebar_terminal);

                    char sub_key = _getch();
                    if (sub_key == 0 || sub_key == (char)224) {
                        sub_key = _getch();
                        if (sub_key == 72) { sel_sub--; if (sel_sub < 0) sel_sub = total_menu - 1; } 
                        if (sub_key == 80) { sel_sub++; if (sel_sub >= total_menu) sel_sub = 0; }   
                    }
                    else if (sub_key == 13 || sub_key == 32) {
                        if (sel_sub == 0) { // Sign Up Mahasiswa
                            system("cls");
                            cetak_banner();
                            if (jml_mhs < MAKS_MHS) {
                                string nama, nim, password;
                                bool valid = true;

                                cout << "\nMasukan nama lengkap : ";
                                getline(cin, nama);
                                
                                // Validasi Nama Mahasiswa (Hanya huruf dan spasi)
                                if (nama.length() == 0) valid = false;
                                for (size_t idx = 0; idx < nama.length(); idx++) {
                                    char c = nama[idx];
                                    if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')) {
                                        valid = false;
                                        break;
                                    }
                                }
                                if (!valid) {
                                    cout << "\n[ERROR] Nama lengkap harus diisi dan hanya boleh berisi huruf!\n";
                                    system("pause");
                                    continue;
                                }

                                cout << "Masukan NIM          : ";
                                getline(cin, nim);
                                
                                // Validasi NIM Mahasiswa (Harus tepat 11 karakter)
                                if (nim.length() != 11) {
                                    cout << "\n[ERROR] NIM harus berisi tepat 11 karakter!\n";
                                    system("pause");
                                    continue;
                                }

                                cout << "Masukan password     : ";
                                getline(cin, password);
                                
                                // Validasi Password kosong
                                if (password.length() == 0) {
                                    cout << "\n[ERROR] Password tidak boleh kosong!\n";
                                    system("pause");
                                    continue;
                                }

                                char daftar_kelas[4] = {'A', 'B', 'C', 'D'};
                                char kelas = daftar_kelas[jml_mhs % 4];

                                Mahasiswa* baru = data_mhs + jml_mhs;
                                baru->signup_mahasiswa(nama, nim, password, kelas);
                                cout << "[Sistem] Kelas Anda otomatis ditetapkan: " << kelas << endl;
                                jml_mhs++;
                                cout << "\n[+] Registrasi mahasiswa berhasil!\n";
                            } else {
                                cout << "\n[-] Database penuh!\n";
                            }
                            system("pause");
                        } 
                        else if (sel_sub == 1) { // Sign In Mahasiswa
                            system("cls");
                            cetak_banner();
                            string nim_login, pass_login;
                            cout << "\nNIM      : ";
                            getline(cin, nim_login);
                            cout << "Password : ";
                            getline(cin, pass_login);

                            // Validasi Input Kosong pas Sign In
                            if (nim_login.length() == 0 || pass_login.length() == 0) {
                                cout << "\n[ERROR] NIM dan Password tidak boleh kosong!\n";
                                system("pause");
                                continue;
                            }

                            Mahasiswa* mhs_login = cari_login_mahasiswa(nim_login, pass_login);
                            if (mhs_login != nullptr) {
                                cout << "\nLogin berhasil! Selamat datang, " << mhs_login->nama_mhs << endl;
                                system("pause");
                                menu_mahasiswa(mhs_login);
                            } else {
                                cout << "\n[ERROR] NIM atau Password salah!\n";
                                system("pause");
                            }
                        } 
                        else if (sel_sub == 2) { 
                            break;
                        }
                    }
                }
            }

            // ---- SUBMENU DOSEN ----
            else if (sel == 1) {
                int sel_sub = 0;
                while (true) {
                    system("cls");
                    cetak_banner();
                    cout << endl;
                    cetak_baris_tengah("[MENU UTAMA DOSEN]", lebar_terminal);
                    cout << endl;

                    for (int i = 0; i < total_menu; i++) {
                        cetak_baris_tengah(pembatas, lebar_terminal);
                        if (i == sel_sub) {
                            string format_sub = " >> " + opsi_sub_menu[i] + " << ";
                            if (i == 2) {
                                cetak_baris_tengah("|" + format_teks_tengah(string(RED) + format_sub + RESET, lebar_kotak) + "|", lebar_terminal);
                            } else {
                                cetak_baris_tengah("|" + format_teks_tengah(string(BLUE_LIGHT) + format_sub + RESET, lebar_kotak) + "|", lebar_terminal);
                            }
                        } else {
                            cetak_baris_tengah("|" + format_teks_tengah(opsi_sub_menu[i], lebar_kotak) + "|", lebar_terminal);
                        }
                    }
                    cetak_baris_tengah(pembatas, lebar_terminal);

                    char sub_key = _getch();
                    if (sub_key == 0 || sub_key == (char)224) {
                        sub_key = _getch();
                        if (sub_key == 72) { sel_sub--; if (sel_sub < 0) sel_sub = total_menu - 1; } 
                        if (sub_key == 80) { sel_sub++; if (sel_sub >= total_menu) sel_sub = 0; }   
                    }
                    else if (sub_key == 13 || sub_key == 32) {
                        if (sel_sub == 0) { // Sign Up Dosen
                            system("cls");
                            cetak_banner();
                            if (jml_dsn < MAKS_DSN) {
                                string namas, usernames, passwords;
                                bool valid_dsn = true;

                                cout << "\nREGISTRASI DOSEN BARU\n";
                                cout << "Masukan nama     : "; getline(cin, namas);
                                
                                // Validasi Nama Dosen (Hanya huruf dan spasi)
                                if (namas.length() == 0) valid_dsn = false;
                                for (size_t idx = 0; idx < namas.length(); idx++) {
                                    char c = namas[idx];
                                    if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')) {
                                        valid_dsn = false;
                                        break;
                                    }
                                }
                                if (!valid_dsn) {
                                    cout << "\n[ERROR] Nama dosen harus diisi dan hanya boleh berisi huruf!\n";
                                    system("pause");
                                    continue;
                                }

                                cout << "Masukan username : "; getline(cin, usernames);
                                if (usernames.length() == 0) {
                                    cout << "\n[ERROR] Username tidak boleh kosong!\n";
                                    system("pause");
                                    continue;
                                }

                                bool cek = false;
                                Dosen* p = data_dsn;
                                for (int i = 0; i < jml_dsn; i++, p++) {
                                    if (p->usn == usernames) {
                                        cek = true;
                                        break;
                                    }
                                }
                                if (cek) {
                                    cout << "\n[ERROR] Username sudah digunakan, coba username lain.\n";
                                } else {
                                    cout << "Masukan password : "; getline(cin, passwords);
                                    
                                    // Validasi password kosong
                                    if (passwords.length() == 0) {
                                        cout << "\n[ERROR] Password tidak boleh kosong!\n";
                                        system("pause");
                                        continue;
                                    }

                                    Dosen* baru = data_dsn + jml_dsn;
                                    baru->signup_dosen(namas, passwords, usernames);
                                    cout << "\n[+] Registrasi dosen berhasil!\n";
                                    cout << "+==============================================================+\n";
                                    cout << "  Nama     : " << baru->nama << "\n";
                                    cout << "  Username : " << baru->usn << "\n";
                                    cout << "  Password : " << baru->pass << "\n";
                                    cout << "+==============================================================+\n";
                                    jml_dsn++;
                                }
                            } else {
                                cout << "\n[ERROR] Maaf, database dosen penuh!\n";
                            }
                            system("pause");
                        } 
                        else if (sel_sub == 1) { // Sign In Dosen
                            system("cls");
                            cetak_banner();
                            string usn_login, pass_login;
                            cout << "\n===== LOGIN DOSEN =====\n";
                            cout << "Username : "; getline(cin, usn_login);
                            cout << "Password : "; getline(cin, pass_login);

                            // Validasi Input Kosong pas Sign In Dosen
                            if (usn_login.length() == 0 || pass_login.length() == 0) {
                                cout << "\n[ERROR] Username dan Password tidak boleh kosong!\n";
                                system("pause");
                                continue;
                            }

                            Dosen* dsn_login = cari_login_dosen(usn_login, pass_login);
                            if (dsn_login != nullptr) {
                                cout << "\nLogin Berhasil! Selamat datang, " << dsn_login->nama << "\n";
                                system("pause");
                                menu_dosen(dsn_login);
                            } else {
                                cout << "\n[ERROR] Username atau Password salah!\n";
                                system("pause");
                            }
                        } 
                        else if (sel_sub == 2) { 
                            break;
                        }
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
    menu_utama();
    return 0;
}