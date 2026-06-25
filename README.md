# Kelompok 16
### 📝 SIGMA: Sistem Guna Manajemen Absensi 📝

-------------

## Deskripsi

**SIGMA** merupakan aplikasi berbasis Command Line Interface (CLI) yang dikembangkan menggunakan bahasa pemrograman C++ untuk membantu pengelolaan absensi mahasiswa secara digital. Program ini menyediakan fitur autentikasi berupa Sign Up dan Sign In untuk mahasiswa dan dosen, pemilihan mata kuliah, pembukaan dan penutupan sesi absensi oleh dosen, pengisian absensi dengan status hadir, izin, atau sakit oleh mahasiswa, serta riwayat kehadiran yang dapat dipantau oleh kedua pengguna. Sistem juga dilengkapi berbagai validasi untuk menjaga keakuratan data, seperti pembatasan absensi hanya saat sesi dibuka dan pencegahan absensi ganda. Dalam implementasinya, program memanfaatkan struktur data struct, array, dan pointer untuk mengelola data mahasiswa, dosen, mata kuliah, dan absensi secara efisien, serta didukung tampilan CLI berwarna dan navigasi menu menggunakan tombol panah agar lebih interaktif dan mudah digunakan.

-----------------------------------------------

## Anggota Kelompok

|             Nama               |     NIM    |
|--------------------------------|------------|
| **My Ardika Wijaya Prasetyo**  | F1D02410147|
| **Aura Permata Grasya**        | F1D02510004|
| **Muzakir Mataho**             | F1D02510018|
| **Adelya Ega Syafa**           | F1D02510033|
| **Atika Rahmadani**            | F1D02510039|
| **Evalinda Gracia Faozi**      | F1D02510048|
| **I Made Duta Atmajaya**       | F1D02510056|

-----------------------------------------------


## Struktur Proyek

```
SIGMA (Sistem Guna Manajemen Absensi)
├── Sign In
│   AS Mahasiswa
│   ├── Pilih Mata Kuliah
│   │   ├── Algoritma Pemrograman
│   │   ├── Sistem Digital
│   │   ├── Aljabar Linear
│   │   ├──Komputer dan Masyarakat
│   │   └──Matematika Diskrit
│   ├── Isi Absensi
│   │   ├── Hadir
│   │   ├── Izin
│   │   └── Sakit
│   └── Lihat Riwayat Absensi
│
│   Dosen
│   ├── Pilih Mata Kuliah dan Kelas
│   │   ├── Algoritma Pemrograman
│   │   │   ├── Kelas A
│   │   │   ├── Kelas B
│   │   │   ├── Kelas C
│   │   │   └── Kelas D
│   │   ├── Sistem Digital
│   │   │   ├── Kelas A
│   │   │   ├── Kelas B
│   │   │   ├── Kelas C
│   │   │   └── Kelas D
│   │   ├── Aljabar Linear
│   │   │   ├── Kelas A
│   │   │   ├── Kelas B
│   │   │   ├── Kelas C
│   │   │   └── Kelas D
│   │   ├── Komputer dan Masyarakat
│   │   │   ├── Kelas A
│   │   │   ├── Kelas B
│   │   │   ├── Kelas C
│   │   │   └── Kelas D
│   │   └── Matematika Diskrit
│   │       ├── Kelas A
│   │       ├── Kelas B
│   │       ├── Kelas C
│   │       └── Kelas D
│   ├── Buka Absensi
│   │   
│   ├── Tutup Absensi
│   │  
│   ├── Riwayat Absensi
│   │
│   └── Edit Mahasiswa
│       ├── Lihat Data Mahasiswa
│       ├── Hapus Mahasiswa
│       └── Hapus Mata Kuliah Mahasiswa
│
├── Sign Up
│   AS Mahasiswa
│   ├── Nama Lengkap
│   ├── NIM
│   └── Password
│  
│   AS Dosen
│   ├── Nama
│   ├── USN
│   └── Password
│   
│
└── Keluar Aplikasi
```
---
