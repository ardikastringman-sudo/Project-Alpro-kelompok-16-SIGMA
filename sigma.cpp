#include <iostream>
#include <string>
 
using namespace std;
 
// ============================================================
//  KODE WARNA ANSI
// ============================================================
#define NAVY_BLUE   "\033[38;2;12;25;115m"
#define BLUE_LIGHT  "\033[38;2;21;95;160m"
#define CYAN_TEAL   "\033[38;2;31;145;165m"
#define ICE_BLUE    "\033[38;2;140;185;195m"
#define RESET       "\033[0m"
 
// ============================================================
//  STRUCT MAHASISWA
// ============================================================
struct Mahasiswa {
    int status_absen;
    char kelas;
    string NIM;
    string password;
    string nama_lengkap;
};
Mahasiswa database[50];
 
// ============================================================
//  STRUCT DAN DATABASE DOSEN (TAMBAHAN)
// ============================================================
struct Dosen {
    string nama;
    string username;
    string password;
};
Dosen database_dosen[20];
int jumlah_dosen = 0;
 
void banner() {
    cout << "+==============================================================+\n";
    cout << NAVY_BLUE  "|        :####:    ######    :####:   ###  ###    :##:         |\n";
    cout << NAVY_BLUE  "|       :######    ######    ######   ###  ###     ##          |\n";
    cout << NAVY_BLUE  "|       ##:   :#     ##     :##:  .#  ###::###    ####         |\n";
    cout << BLUE_LIGHT "|       ##           ##     ##:       ###  ###    ####         |\n";
    cout << BLUE_LIGHT "|       ###:         ##     ##.       ## ## ##   :#  #:        |\n";
    cout << BLUE_LIGHT "|       :#####:      ##     ##        ##:##:##    #::#         |\n";
    cout << CYAN_TEAL  "|        .#####:     ##     ##  ####  ##.##.##   ##  ##        |\n";
    cout << CYAN_TEAL  "|            :###    ##     ##. ####  ## ## ##   ######        |\n";
    cout << CYAN_TEAL  "|              ##    ##     ##:   ##  ##    ##  .######.       |\n";
    cout << ICE_BLUE   "|       #:.   :##    ##     :##:  ##  ##    ##  :##  ##:       |\n";
    cout << ICE_BLUE   "|       #######:   ######    #######  ##    ##  ###  ###       |\n";
    cout << ICE_BLUE   "|       .#####:    ######     :####.  ##    ##  ##:  :##       |\n";
    cout << RESET;
    cout << "+==============================================================+\n";
    cout << "+==============================================================+\n";
    cout << "|               SISTEM GUNA ABSENSI MAHASISWA                  |\n";
    cout << "+==============================================================+\n";
}
 
// ============================================================
//  SIGN UP MAHASISWA 
// ============================================================
void Signup_mahasiswa() {
    system("cls");
    banner();
    int jumlah_mhs = 0;
    if (jumlah_mhs < 50) {
        cout << "\nREGISTRASI MAHASISWA BARU\n";
        cout << "Masukan nama lengkap : ";
        getline(cin, database[jumlah_mhs].nama_lengkap);
        cout << "Masukan NIM          : ";
        getline(cin, database[jumlah_mhs].NIM);
        cout << "Masukan password     : ";
        getline(cin, database[jumlah_mhs].password);
        cout << "Masukan Kelas (A/B/C): ";
        cin >> database[jumlah_mhs].kelas;
        jumlah_mhs++;
        char daftar_kelas[4] = {'A', 'B', 'C', 'D'};
        database[jumlah_mhs].kelas = daftar_kelas[jumlah_mhs % 4];
        cout << "[Sistem] Kelas Anda otomatis ditetapkan: Kelas " << database[jumlah_mhs].kelas << "\n";
    } else {
        cout << "\n[-] Maaf, database penuh!\n";
    }
}
 
// ============================================================
//  LOGIN MAHASISWA 
// ============================================================
void login_mahasiswa() {
    system("cls");
    banner();
    string login_nama, login_Nim, login_password;
    cout << "\nLOGIN MAHASISWA\n";
    cout << "Masukan nama lengkap : ";
    getline(cin, login_nama);
    cout << "Masukan NIM          : ";
    getline(cin, login_Nim);
    cout << "Masukan password     : ";
    getline(cin, login_password);
    for (int i = 0; i < 50; i++) {
        if (login_nama == database[i].nama_lengkap && login_Nim == database[i].NIM && login_password == database[i].password) {
            cout << "\n[+] Login berhasil! Selamat datang, " << database[i].nama_lengkap << "!\n";
            return;
        }
    }
    cout << "\n[-] Login gagal! Data tidak ditemukan.\n";
}
 
// ============================================================
//  SIGN UP DOSEN 
// ============================================================
void Signup_dosen() {
    system("cls");
    banner();
    if (jumlah_dosen >= 20) {
        cout << "\n[-] Maaf, database dosen penuh!\n";
        return;
    }
    cout << "\nREGISTRASI DOSEN BARU\n";
    cout << "Masukan nama     : ";
    getline(cin, database_dosen[jumlah_dosen].nama);
    string input_username;
    cout << "Masukan username : ";
    getline(cin, input_username);
    for (int i = 0; i < jumlah_dosen; i++) {
        if (database_dosen[i].username == input_username) {
            cout << "\n[-] Username sudah digunakan, coba username lain.\n";
            return;
        }
    }
    database_dosen[jumlah_dosen].username = input_username;
    cout << "Masukan password : ";
    getline(cin, database_dosen[jumlah_dosen].password);
    jumlah_dosen++;
    cout << "\n[+] Registrasi dosen berhasil!\n";
    cout << "+==============================================================+\n";
    cout << "  Nama     : " << database_dosen[jumlah_dosen-1].nama     << "\n";
    cout << "  Username : " << database_dosen[jumlah_dosen-1].username << "\n";
    cout << "  Password : " << database_dosen[jumlah_dosen-1].password << "\n";
    cout << "+==============================================================+\n";
}
 
// ============================================================
//  MAIN 
// ============================================================
int main() {
    int jumlah_mhs = 0;
    int pilihan, pilihan_awal;
    banner();
        cout << "Masuk sebagai: ";
        cout << "\n1. Mahasiswa";
        cout << "\n2. Dosen";
        cout << "\npilihan: ";
        cin >> pilihan_awal;
        cin.ignore();
        while (true) {
            if (pilihan_awal == 1) {
                cout << "\n[Menu Mahasiswa]\n";
                cout << "\n1. Sign Up";
                cout << "\n2. Sign In";
                cout << "\npilihan: ";
                cin >> pilihan;
                cin.ignore();
                if (pilihan == 1) {
                    Signup_mahasiswa();
                    continue;
                }
                else if (pilihan == 2) {
                    login_mahasiswa();
                    continue;
                } else if (pilihan == 0) {
                    cout << "\nkeluar dari mahasiswa\n";
                    break;
                }
                else {
                    cout << "\n[ERROR] Pilihan tidak valid!\n";
                    continue;
                }
            } else if (pilihan_awal == 2) {
                cout << "\n[Menu Dosen]\n";
                cout << "\n1. Sign Up";
                cout << "\n2. Sign In";
                cout << "\npilihan: ";
                cin >> pilihan;
                cin.ignore();
                if (pilihan == 1) {
                    Signup_dosen();
                    continue;
                } else if (pilihan == 2) {
                    continue;
                } else if (pilihan == 0) {
                    cout << "\nkeluar dari dosen\n";
                    break;
                } else {
                    cout << "\n[ERROR] Pilihan tidak valid!\n";
                    continue;
                }
            }
        }
    return 0;
}
// catatan: itu kerangka utamanya kalo mo ditanyain info ae di wa
// absennya kalo menurut oe pake nomor ae 1.masuk 2.sakit 3.izin di pilihan absennya, pas coutnya nanti kalo status_absen == 1 dia ditulis hadir