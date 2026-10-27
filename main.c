#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int userIDs[10];
char userNames[10][50];
float balances[10][10];

int RegisterNewUser(int *UserCount) {
    int i, n, start;
    char input[100];

    if (*UserCount >= 10) {
        printf("Maximum user limit reached. Cannot register more users.\n");
        return 0;
    }

    printf("Enter the number of users you wish to add (Max: %d): ", 10 - *UserCount);

    if (scanf("%d", &n) != 1){
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n');
        return 0;
    }

    while (getchar() != '\n');

    if (n <= 0 || n > 10 - *UserCount) {
        printf("Invalid number of users requested.\n");
        return 0;
    }

    printf("\n");
    start = *UserCount;

    for (i = start; i < start + n; i++)  {
        printf("Enter the name of user %d: ", i + 1);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading name.\n");
            return 0;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            printf("Name cannot be empty. Please try again.\n");
            i--;
            continue;
        }

        if (strlen(input) >= 50) {
            printf("Name is too long. Maximum length is 49 characters.\n");
            i--;
            continue;
        }

        strcpy(userNames[i], input);
        userIDs[i] = i + 1;

        (*UserCount)++;

        printf("User registered successfully! ID: %d, Name: %s\n\n", userIDs[i], userNames[i]);
    }
    return 1;
}

void ViewUsers(int UserCount) {
    int i;

    if (UserCount == 0) {
        printf("No users registered yet.\n");
        return;
    }

    printf("\n------ Registered Users ------\n");

    for (i = 0; i < UserCount; i++) {
        printf("ID: %d | Name: %s\n", userIDs[i], userNames[i]);
    }
}

void LogNewExpense(int UserCount) {
    int i, j, PayerID, ParticipantCount, participants[10], ParticipantID;
    int duplicate;
    float amount, share;

    if (UserCount == 0) {
        printf("No users registered yet.\n");
        return;
    }

    printf("\n------ Registered Users ------\n");

    for (i = 0; i < UserCount; i++) {
        printf("ID: %d | Name: %s\n", userIDs[i], userNames[i]);
    }

    printf("Enter the ID of the User who paid: ");
    scanf("%d", &PayerID);

    if (PayerID <= 0 || PayerID > 10 || PayerID > UserCount) {
        printf("Invalid payer id, retry again with a valid id.\n");
        return;
    }

    printf("Enter the expense done: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid amount entered, retry again with a valid amount.\n");
        return;
    }

    printf("How many people are sharing this expense (including payer): ");
    scanf("%d", &ParticipantCount);

    if (ParticipantCount <= 0 || ParticipantCount > UserCount) {
        printf("Invalid participant count, retry again with a valid participant count.\n");
        return;
    }

    for (i = 0; i < ParticipantCount; i++) {
        printf("Enter the user id of participant (%d): ", i + 1);
        scanf("%d", &ParticipantID);

        if (ParticipantID <= 0 || ParticipantID > UserCount) {
            printf("Invalid participant id, retry again with a valid participant id.\n");
            return;
        }

        duplicate = 0;

        for (j = 0; j < i; j++) {
            if (participants[j] == ParticipantID) {
                duplicate = 1;
                break;
            }
        }

        if (duplicate == 1) {
            printf("This user has already been selected. Please try the expense again.\n");
            return;
        }
        participants[i] = ParticipantID;
    }

    share = amount / ParticipantCount;
    printf("Each person's share is: %.2f\n", share);

    for (i = 0; i < ParticipantCount; i++) {
        if (PayerID != participants[i]) {
            balances[participants[i] - 1][PayerID - 1] += share;
        }
    }

}


void ViewDebtMatrix(int UserCount) {
    int i, j;

    if (UserCount == 0) {
        printf("No users registered yet.\n");
        return;
    }

    printf("\n---------- SplitWise: Debt Matrix ----------\n\n");
    printf("%-15s", "Owes / To");

    for (i = 0; i < UserCount; i++) {
        printf("%-12s", userNames[i]);
    }

    printf("\n");

    for (i = 0; i < UserCount; i++) {
        printf("%-15s", userNames[i]);
        for (j = 0; j < UserCount; j++) {
            printf("%-12.2f", balances[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void ViewSimplifiedDebts(int UserCount) {
    int i, j;
    float net[10], payment;
    int debtor, creditor;
    int transactions = 0;
    float epsilon = 0.005f;

    if (UserCount == 0) {
        printf("No users registered yet.\n");
        return;
    }

    printf("\n---------- SplitWise: Simplified Debts ----------\n\n");

    for (i = 0; i < UserCount; i++) {
        net[i] = 0.0f;
        for (j = 0; j < UserCount; j++) {
            net[i] += balances[j][i] - balances[i][j];
        }
    }

    while (1)
    {
        debtor = -1;
        creditor = -1;

        for (i = 0; i < UserCount; i++) {
            if (net[i] < -epsilon) {
                debtor = i;
            } else if (net[i] > epsilon) {
                creditor = i;
            }
        }

        if (debtor == -1 || creditor == -1) {
            break;
        }

        if (-net[debtor] < net[creditor]) {
            payment = -net[debtor];
        } else {
            payment = net[creditor];
        }

        printf("%s pays %s: Rs. %.2f\n",
               userNames[debtor],
               userNames[creditor],
               payment);

        net[debtor] += payment;
        net[creditor] -= payment;

        transactions++;
    }

    if (transactions == 0) {
        printf("All expenses are already settled.\n");
    }

    printf("\n");
}

void SaveData() {
    printf("File saving is in progress.\n");
}

void menu() {
    int choice;
    int UserCount = 0;

    do {
        printf("\n");
        printf("----------------- SplitWise -----------------\n");
        printf("1. Register a new user.\n");
        printf("2. View registered users.\n");
        printf("3. Log a new expense.\n");
        printf("4. View the debt matrix.\n");
        printf("5. View simplified debts.\n");
        printf("6. Save data.\n");
        printf("7. Exit.\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        printf("\n");

        switch (choice) {
            case 1:
                RegisterNewUser(&UserCount);
                break;

            case 2:
                ViewUsers(UserCount);
                break;

            case 3:
                LogNewExpense(UserCount);
                break;

            case 4:
                ViewDebtMatrix(UserCount);
                break;

            case 5:
                ViewSimplifiedDebts(UserCount);
                break;

            case 6:
                SaveData();
                break;

            case 7:
                printf("Exiting SplitWise.\n");
                break;

            default:
                printf("Invalid choice selected.\n");
        }

    } while (choice != 7);
}

int main() {
    menu();
    return 0;
}