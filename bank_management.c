#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Account {
    int accountNumber;
    char name[50];
    float balance;
    char type[20];
};

void createAccount();
void displayAccount();
void deposit();
void withdraw();
void transfer();
void deleteAccount();

int main() {
    int choice;
    
    do {
        printf("\nBank Management System\n");
        printf("1. Create Account\n");
        printf("2. Display Account\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. Transfer\n");
        printf("6. Delete Account\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                createAccount();
                break;
            case 2:
                displayAccount();
                break;
            case 3:
                deposit();
                break;
            case 4:
                withdraw();
                break;
            case 5:
                transfer();
                break;
            case 6:
                deleteAccount();
                break;
            case 7:
                printf("\nThank you for using Bank Management System!\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while(choice != 7);

    return 0;
}

void createAccount() {
    struct Account acc;
    FILE *fp;
    
    printf("\nEnter Account Details:\n");
    printf("Account Number: ");
    scanf("%d", &acc.accountNumber);
    printf("Name: ");
    scanf("%s", acc.name);
    printf("Initial Balance: ");
    scanf("%f", &acc.balance);
    printf("Account Type (Savings/Current): ");
    scanf("%s", acc.type);

    fp = fopen("accounts.dat", "ab");
    if(fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fwrite(&acc, sizeof(struct Account), 1, fp);
    fclose(fp);
    printf("\nAccount created successfully!\n");
}

void displayAccount() {
    struct Account acc;
    int accNo;
    FILE *fp;
    
    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    fp = fopen("accounts.dat", "rb");
    if(fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int found = 0;
    while(fread(&acc, sizeof(struct Account), 1, fp)) {
        if(acc.accountNumber == accNo) {
            printf("\nAccount Details:\n");
            printf("Account Number: %d\n", acc.accountNumber);
            printf("Name: %s\n", acc.name);
            printf("Balance: %.2f\n", acc.balance);
            printf("Type: %s\n", acc.type);
            found = 1;
            break;
        }
    }

    if(!found) {
        printf("\nAccount not found!\n");
    }

    fclose(fp);
}

void deposit() {
    struct Account acc;
    int accNo;
    float amount;
    FILE *fp, *temp;
    
    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);
    printf("Enter Amount to Deposit: ");
    scanf("%f", &amount);

    fp = fopen("accounts.dat", "rb");
    temp = fopen("temp.dat", "wb");
    if(fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int found = 0;
    while(fread(&acc, sizeof(struct Account), 1, fp)) {
        if(acc.accountNumber == accNo) {
            acc.balance += amount;
            found = 1;
        }
        fwrite(&acc, sizeof(struct Account), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if(found) {
        remove("accounts.dat");
        rename("temp.dat", "accounts.dat");
        printf("\nAmount deposited successfully!\n");
    } else {
        remove("temp.dat");
        printf("\nAccount not found!\n");
    }
}

void withdraw() {
    struct Account acc;
    int accNo;
    float amount;
    FILE *fp, *temp;
    
    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);
    printf("Enter Amount to Withdraw: ");
    scanf("%f", &amount);

    fp = fopen("accounts.dat", "rb");
    temp = fopen("temp.dat", "wb");
    if(fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int found = 0;
    while(fread(&acc, sizeof(struct Account), 1, fp)) {
        if(acc.accountNumber == accNo) {
            if(acc.balance >= amount) {
                acc.balance -= amount;
                found = 1;
            } else {
                printf("\nInsufficient balance!\n");
            }
        }
        fwrite(&acc, sizeof(struct Account), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if(found) {
        remove("accounts.dat");
        rename("temp.dat", "accounts.dat");
        printf("\nAmount withdrawn successfully!\n");
    } else if(!found) {
        remove("temp.dat");
        printf("\nAccount not found!\n");
    }
}

void transfer() {
    struct Account acc;
    int fromAcc, toAcc;
    float amount;
    FILE *fp, *temp;
    
    printf("\nEnter Source Account Number: ");
    scanf("%d", &fromAcc);
    printf("Enter Destination Account Number: ");
    scanf("%d", &toAcc);
    printf("Enter Amount to Transfer: ");
    scanf("%f", &amount);

    fp = fopen("accounts.dat", "rb");
    temp = fopen("temp.dat", "wb");
    if(fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int fromFound = 0, toFound = 0;
    while(fread(&acc, sizeof(struct Account), 1, fp)) {
        if(acc.accountNumber == fromAcc) {
            if(acc.balance >= amount) {
                acc.balance -= amount;
                fromFound = 1;
            } else {
                printf("\nInsufficient balance in source account!\n");
            }
        }
        fwrite(&acc, sizeof(struct Account), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if(fromFound) {
        fp = fopen("accounts.dat", "rb");
        temp = fopen("temp.dat", "wb");
        if(fp == NULL || temp == NULL) {
            printf("Error opening file!\n");
            return;
        }

        while(fread(&acc, sizeof(struct Account), 1, fp)) {
            if(acc.accountNumber == toAcc) {
                acc.balance += amount;
                toFound = 1;
            }
            fwrite(&acc, sizeof(struct Account), 1, temp);
        }

        fclose(fp);
        fclose(temp);

        if(toFound) {
            remove("accounts.dat");
            rename("temp.dat", "accounts.dat");
            printf("\nAmount transferred successfully!\n");
        } else {
            remove("temp.dat");
            printf("\nDestination account not found!\n");
        }
    } else {
        remove("temp.dat");
        printf("\nSource account not found!\n");
    }
}

void deleteAccount() {
    struct Account acc;
    int accNo;
    FILE *fp, *temp;
    
    printf("\nEnter Account Number to Delete: ");
    scanf("%d", &accNo);

    fp = fopen("accounts.dat", "rb");
    temp = fopen("temp.dat", "wb");
    if(fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int found = 0;
    while(fread(&acc, sizeof(struct Account), 1, fp)) {
        if(acc.accountNumber != accNo) {
            fwrite(&acc, sizeof(struct Account), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    if(found) {
        remove("accounts.dat");
        rename("temp.dat", "accounts.dat");
        printf("\nAccount deleted successfully!\n");
    } else {
        remove("temp.dat");
        printf("\nAccount not found!\n");
    }
} 