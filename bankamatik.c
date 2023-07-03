#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Kullanıcı bilgilerini temsil eden yapı
struct User {
    char name[50];
    int accountNumber;
    float balance;
};

// Dosyaya kullanıcıları kaydetme işlemi
void saveUsers(struct User users[], int count) {
    FILE *file = fopen("users.txt", "w");
    if (file == NULL) {
        printf("Dosya oluşturulamadı.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d %.2f\n", users[i].name, users[i].accountNumber, users[i].balance);
    }

    fclose(file);
}

// Dosyadan kullanıcıları yükleme işlemi
int loadUsers(struct User users[]) {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Dosya bulunamadı.\n");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%s %d %f", users[count].name, &users[count].accountNumber, &users[count].balance) == 3) {
        count++;
    }

    fclose(file);
    return count;
}

// Kullanıcı kayıt işlemi
void createUser(struct User users[], int *count) {
    printf("İsim: ");
    scanf("%s", users[*count].name);

    printf("Hesap numarası: ");
    scanf("%d", &users[*count].accountNumber);

    printf("Başlangıç bakiyesi: ");
    scanf("%f", &users[*count].balance);

    (*count)++;
    saveUsers(users, *count);
    printf("Kullanıcı oluşturuldu.\n");
}

// Kullanıcı güncelleme işlemi
void updateUser(struct User users[], int count) {
    int accountNumber;
    printf("Güncellenecek hesap numarası: ");
    scanf("%d", &accountNumber);

    for (int i = 0; i < count; i++) {
        if (users[i].accountNumber == accountNumber) {
            printf("Yeni isim: ");
            scanf("%s", users[i].name);

            printf("Yeni hesap numarası: ");
            scanf("%d", &users[i].accountNumber);

            printf("Yeni bakiye: ");
            scanf("%f", &users[i].balance);

            saveUsers(users, count);
            printf("Kullanıcı güncellendi.\n");
            return;
        }
    }

    printf("Hesap numarası bulunamadı.\n");
}

// Kullanıcı silme işlemi
void deleteUser(struct User users[], int *count) {
    int accountNumber;
    printf("Silinecek hesap numarası: ");
    scanf("%d", &accountNumber);

    for (int i = 0; i < *count; i++) {
        if (users[i].accountNumber == accountNumber) {
            for (int j = i; j < *count - 1; j++) {
                users[j] = users[j + 1];
            }

            (*count)--;
            saveUsers(users, *count);
            printf("Kullanıcı silindi.\n");
            return;
        }
    }

    printf("Hesap numarası bulunamadı.\n");
}

// Hesap işlemleri menüsü
void accountMenu(struct User users[], int count, int index) {
    int choice;
    float amount;

    while (1) {
        printf("\nHesap Menüsü:\n");
        printf("1. Bakiye Görüntüle\n");
        printf("2. Bakiye Güncelle\n");
        printf("3. Para Yatır\n");
        printf("4. Para Çek\n");
        printf("5. Çıkış\n");
        printf("Seçiminizi yapın: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Bakiye: %.2f\n", users[index].balance);
                break;
            case 2:
                printf("Yeni bakiye: ");
                scanf("%f", &users[index].balance);
                saveUsers(users, count);
                printf("Bakiye güncellendi.\n");
                break;
            case 3:
                printf("Yatırılacak miktar: ");
                scanf("%f", &amount);
                users[index].balance += amount;
                saveUsers(users, count);
                printf("%.2f TL hesaba yatırıldı.\n", amount);
                break;
            case 4:
                printf("Çekilecek miktar: ");
                scanf("%f", &amount);

                if (amount <= users[index].balance) {
                    users[index].balance -= amount;
                    saveUsers(users, count);
                    printf("%.2f TL hesaptan çekildi.\n", amount);
                } else {
                    printf("Yetersiz bakiye.\n");
                }
                break;
            case 5:
                return;
            default:
                printf("Geçersiz seçim.\n");
        }
    }
}

// Kullanıcı girişi
void login(struct User users[], int count) {
    int accountNumber;
    char password[50];

    printf("Hesap numarası: ");
    scanf("%d", &accountNumber);

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (users[i].accountNumber == accountNumber) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Hesap bulunamadı.\n");
        return;
    }

    printf("Şifre: ");
    scanf("%s", password);

    // Şifre kontrolü için uygun bir mekanizma ekleyebilirsiniz

    printf("Hoş geldiniz, %s!\n", users[index].name);
    accountMenu(users, count, index);
}

// Kullanıcı arayüzü
void userInterface(struct User users[], int count) {
    int choice;

    while (1) {
        printf("\nAna Menü:\n");
        printf("1. Yeni Hesap Oluştur\n");
        printf("2. Hesap Güncelle\n");
        printf("3. Hesap Sil\n");
        printf("4. Giriş Yap\n");
        printf("5. Çıkış\n");
        printf("Seçiminizi yapın: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createUser(users, &count);
                break;
            case 2:
                updateUser(users, count);
                break;
            case 3:
                deleteUser(users, &count);
                break;
            case 4:
                login(users, count);
                break;
            case 5:
                return;
            default:
                printf("Geçersiz seçim.\n");
        }
    }
}

int main() {
    struct User users[100];
    int count = loadUsers(users);

    userInterface(users, count);

    return 0;
}
