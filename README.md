# SISOP 2019 : Modul 2 [Soal Shift] Kelompok D8

## #1 grey_.png dan pindah
### Pembuatan
* Inisialisasi variabel:
    ```c
		DIR *d;
		struct dirent *dir;
		char dir_o[100] = "/home/agarpac/Pictures/", 
			dir_n[100] = "/home/agarpac/modul2/gambar/", 
			dir_e1[100], dir_e2[100],
			replacement[10] = "_grey", 
			extension[5] = ".png",
			*attach, *fname;
    ```
    `dir_o` dan `dir_n` adalah default direktori yang akan digunakan.
    untuk `dir_e[1-2]` akan berubah terus (full diretory of filename).
    `replacement` untuk memberikan tambahan `_grey` dan `extension` untuk akhir filename (karena `replacement`).
    
* Buildup the string
    `strcpy` dan `strcat` adalah kunci untuk manipulasi string yang akan disimpan dalam `dir_e`!
    > Untuk membentuk string `sumber` dan `target`, akan menggunakan variabel `dir_o` dan `dir_n` respectively sebagai basis.
    
    Setelah itu, saatnya rename:
    ```c
        rename(dir_e1, dir_e2);
    ```

## #2 Lupakan elen.ku
### Pembuatan
* Pencarian elen.ku (dan kendala)
    Terdapat banyak `pemeriksaan`
    Dibagi menjadi poin penting berikut;
    * mencari nama file `elen.ku`
        ```c
            attach = strstr(dir->d_name, "elen.ku");
			if(attach != NULL)
        ```
    * mencari `user` dari file tersebut (untuk mendapatkan `uid`)
        note : `str` dialokasikan memori supaya tidak `segment error`
        ```c
            strcpy(str, tempat);
			strcat(str, dir->d_name);
			if (stat(str, &buf) == 0)
        ```
        Setelah itu, dapatkan data dari user tersebut (untuk mendapatkan `user name`)
        ```c
        p = getpwuid(buf.st_uid);
        ```
    * apakah `user owner` = `www-data` (pembanding)?
        ```c
            char uname[100] = "";
			stpcpy(uname, p->pw_name);
			if(strcmp(uname, pembanding) == 0)
        ```
    Belum mendapatkan solusi yang lebih pendek.
* chmod-ing
    ```c
        chmod (str, 777)
    ```
    `777` berarti akses untuk seluruh user(?).
### Waktu sebelum lanjut loop?
```c
    sleep(3);
```
Ini akan berjalan dalam `daemon`.

## #3 Extract dan listing file
### Pembuatan
* fork()-ing
    ada 3 variabel yang digunakan untuk `fork()`ing. Tidak ada perbedaan dengan penggunaan 1 variabel selain visual dalam kodingan.
* pipe
    > Mengimplementasikan `pipe` dalam C ribet.

    Karena itu, pencarian referensi dilakukan.
    Sumber : http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html
    > ` 0>||>1`  `2>||>3 `
    
    Untuk 2 pipe
    Ganjil mempresentasikan `Output`
    Genap mempresentasikan `Input`
* output terakhir
    Untuk mengoutput menjadi file, lakukan berikut ini;
    Simpan apa dalam `output pipe` ke dalam variabel
    ```c
        char filetxt[1000];
		read(pipefd[2],filetxt,sizeof(filetxt));
		close(pipefd[2]);
	```
	Menyimpan variabel yang tersimpan kepada `text file`
	```c
		FILE *out = fopen("daftar.txt","w+");
		fputs(filetxt, out);
		fclose(out);
    ```