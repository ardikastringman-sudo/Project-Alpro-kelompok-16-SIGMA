#include <iostream>
#include <string>

using namespace std;
//struct nya mahasiswa, kalo dosen buat baru
struct Mahasiswa {
    int status_absen;
    char kelas;
    string NIM;
    string password;
    string nama_lengkap;
};
Mahasiswa database[50]; // Array untuk menyimpan data mahasiswa, maksimal 50 orang, bisa juga pake const int MAX_MAHASISWA = 50;

void Signup_mahasiswa() {
    system("cls");
    int jumlah_mhs = 0; // Untuk menghitung berapa mahasiswa yang sudah daftar
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
              
        // Setelah selesai input, tambah jumlah mahasiswa yang terdaftar
        jumlah_mhs++;
        //ini buat increment kelasnya otomatis, caranya dia liat keberapa mahasiswanya daftar misal nomor ke 20 kalo habis dibagi 4 dia dapet 0 jadi kelasnya a
        char daftar_kelas[4] = {'A', 'B', 'C', 'D'};
        database[jumlah_mhs].kelas = daftar_kelas[jumlah_mhs % 4];
        cout << "[Sistem] Kelas Anda otomatis ditetapkan: Kelas " << database[jumlah_mhs].kelas << "\n";
    }else {
        cout << "\n[-] Maaf, database penuh!\n";
    }
}

void login_mahasiswa() {
    system("cls");
    string login_nama, login_Nim, login_password;
    cout << "\nLOGIN MAHASISWA\n";
    cout << "Masukan nama lengkap : ";
    getline(cin, login_nama);
    cout << "Masukan NIM          : ";
    getline(cin, login_Nim);
    cout << "Masukan password     : ";
    getline(cin, login_password);
    //ini looping buat nyari data yang login, dia nyocokin nama, nim sama password yang ada di array database makanya login_nama == database 
    for (int i = 0; i < 50; i++) {
        if (login_nama == database[i].nama_lengkap && login_Nim == database[i].NIM && login_password == database[i].password) {
            cout << "\n[+] Login berhasil! Selamat datang, " << database[i].nama_lengkap << "!\n";
            return;
        }
    }
    cout << "\n[-] Login gagal! Data tidak ditemukan.\n";
}

int main() {
    int jumlah_mhs = 0; // buat ngitung berapa mahasiswa yang sudah daftar
    int pilihan, pilihan_awal;
        cout << "masuk sebagai: ";
        cout << "\n1. Mahasiswa";
        cout << "\n2. Dosen";
        cout << "\npilihan: ";
        cin >> pilihan_awal;
        cin.ignore(); // Membersihkan newline setelah input pilihan
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
                    continue; // disini masukin menu absennya hapus continue nya abistu kasi percabangan lagi
                } else if (pilihan == 0) {
                    cout << "\nkeluar dari mahasiswa\n";
                    break;
                }
                else {
                    cout << "\n[ERROR] Pilihan tidak valid!\n";
                    continue;
                }
            }else if (pilihan_awal == 2) {
                //sign up dan login dosen
            }
        }
    return 0;
} 
// catatan: itu kerangka utamanya kalo mo ditanyain info ae di wa
// absennya kalo menurut oe pake nomor ae 1.masuk 2.sakit 3.izin di pilihan absennya, pas coutnya nanti kalo status_absen == 1 dia ditulis hadir