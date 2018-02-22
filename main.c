#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

/*
 *      STRUCT DECLARATION -------------------------------------------------------
 * */

typedef struct contact_t {
    // Data
    char first_name[20+1];
    char last_name[20+1];
    char company[30+1];
    char email[30+1];
    char tel_number[20+1];

    // Pointers
    struct contact_t* prev;
    struct contact_t* next;
} contact_t;


/*
 *      FUNCTION DECLARATIONS ----------------------------------------------------
 * */

/*
 * PRINT MENU
 * */

// Main Menu
void printMainMenu(contact_t*);
// Add Contact
void printAddContactMenu(contact_t*);
// Show Contacts
void printContacts(contact_t*);
// Edit Contacts
void printEditContactMenu(contact_t*);
void printEditSingleContactMenu(contact_t*, contact_t*);
void printEditPropertyMenu(contact_t*, int, contact_t*);
// Search Contact
void printSearchContactMenu(contact_t*);
// Delete Contact
void printDeleteContactMenu(contact_t*);
// Sort Contacts
void printSortContactsMenu(contact_t*);
// Calculate Memory
void printMemoryMenu(contact_t*);
// Save to File
void printSaveToFileMenu(contact_t*);
// Load from File
void printLoadFromFileMenu(contact_t*);


/*
 * PRINT HELPER
 * */

// prints contact
void printContact(contact_t*);
// prints names of all contacts
void printContactNames(contact_t*);
// prints name of single contact
void printContactName(contact_t*, contact_t*);
// prints '- You didn't select anything -'
void printSelectionError();
// prints 'Please press Enter to continue'
void printContinueMenu();
// prints some secret or handles bad input
void printSecret(int, contact_t*);
// prints all files in './text-files'
void printTextFiles();

/*
 * LIST EDIT
 * */

// creates new contact entry
contact_t* createNewContact(contact_t);
// adds contact to the end of the list
contact_t* addContact(contact_t, contact_t*);
// deletes contact at index
contact_t* deleteContact(int, contact_t*);
// deletes first contact in list
contact_t* deleteFirstContact(contact_t*);
// deletes last contact in list
void deleteLastContact(contact_t*);
// deletes every contact
contact_t* deleteAllContacts(contact_t*);
// sorts by given Criterion (from 1-4)
contact_t* sortByCriterion(int, contact_t*);
// swaps given contact with its following entry
contact_t* swapContacts(contact_t*, contact_t*);
// compares two contacts by given criterion (from 1-4)
short firstFollowsSecond(contact_t*, contact_t*, int);
// saves list to file
void saveToFile(const char*, contact_t*);
// loads list from file
contact_t* loadFromFile(const char*, contact_t*);

/*
 * LIST HELPER
 * */

// returns index of given contact in list
int getIndexOfContact(contact_t*, contact_t*);
// returns contact at given index in list
contact_t* getContactAtIndex(int, contact_t*);
// returns the number of contacts in list
int getLength(contact_t*);
// returns the last contact from the list
contact_t* getLastContact(contact_t*);
// fills the list with sample data
contact_t* fillList(contact_t*);

/*
 *      LIST FUNCTIONS -----------------------------------------------------------
 * */

// Creates a new contact in memory and returns its pointer
contact_t* createNewContact(contact_t person) {
    contact_t* newContact = (contact_t*)malloc(sizeof(contact_t));

    strcpy(newContact->first_name, person.first_name);
    strcpy(newContact->last_name, person.last_name);
    strcpy(newContact->company, person.company);
    strcpy(newContact->email, person.email);
    strcpy(newContact->tel_number, person.tel_number);

    newContact->prev = NULL;
    newContact->next = NULL;

    return newContact;
}
// Adds a contact to the end of the list
contact_t* addContact(contact_t contact, contact_t* head) {
    contact_t* current = head;
    contact_t* newContact = createNewContact(contact);
    if (head == NULL) {
        head = newContact;
        return head;
    }
    while (current->next != NULL) current = current->next;
    current->next = newContact;
    newContact->prev = current;

    return head;
}

/*
 *      DELETE
 * */

// deletes a contact at a given index
contact_t* deleteContact(int index, contact_t* head) {
    if (getLength(head) <= 1) {
        head = NULL;
    } else {
        if (index < 0 || getLength(head) <= index) { return head; }
        if (index == 0) { return deleteFirstContact(head); }
        if (index == getLength(head)-1) { deleteLastContact(head); return head; }

        contact_t* current;
        current = getContactAtIndex(index, head);

        current->prev->next = current->next;
        current->next->prev = current->prev;

        free(current);
    }

    return head;
}
// deletes first contact in list
contact_t* deleteFirstContact(contact_t* head) {
    contact_t* contactToDelete = head;
    head->next->prev = NULL;
    head = head->next;
    free(contactToDelete);

    return head;
}
// deletes last contact in list
void deleteLastContact(contact_t* head) {
    contact_t* last = getLastContact(head);
    last->prev->next = NULL;
    free(last);
}
// deletes all contacts
contact_t* deleteAllContacts(contact_t* head) {
    contact_t* current = head;

    if (current != NULL) {
        while (current->next != NULL) {
            free(current->prev);

            current = current->next;
        }
        free(current);
        head = NULL;
    }

    return head;
}

/*
 *      SORTING
 * */

// sorts list using 'bubble sort'
contact_t* sortByCriterion(int criterion, contact_t* head) {
    int i, j;
    contact_t* current;

    for (i = getLength(head); i > 1; i--) {
        for (j = 0; j < i-1; j++) {
            current = getContactAtIndex(j, head);
            if (firstFollowsSecond(current, current->next, criterion))
                head = swapContacts(getContactAtIndex(j, head), head);
        }
    }

    return head;
}
// swaps contact with its next contact
contact_t* swapContacts(contact_t* first, contact_t* head) {

    if (first->next) {
        contact_t* before = first->prev; // can be NULL
        contact_t* second = first->next;
        contact_t* after = second->next; // can be NULL


        second->next = first;
        first->next = after;
        second->prev = before;
        first->prev = second;

        if (before != NULL) before->next = second;
        if (after != NULL) after->prev = first;
        if (first == head) head = second;
    }

    return head;
}
// compares specific property (criterion) of two contacts
short firstFollowsSecond(contact_t* first, contact_t* second, int criterion) {
    int compareResult = 0;

    switch (criterion) {
        case 1: // Last Name
            compareResult = strcmp(first->last_name, second->last_name);
            break;
        case 2: // First Name
            compareResult = strcmp(first->first_name, second->first_name);
            break;
        case 3: // Company
            compareResult = strcmp(first->company, second->company);
            break;
        case 4: // Email
            compareResult = strcmp(first->email, second->email);
            break;
        default:
            break;
    }

    return (short) (compareResult > 0);
}

/*
 *      SAVE/LOAD FILE
 * */

// saves to file with given name
void saveToFile(const char* fileName, contact_t* head) {
    contact_t* current = head;
    char fullFileName[100+1];
    sprintf(fullFileName, "./%s", fileName);
    FILE *f;

    f = fopen(fullFileName, "w");

    if (f == NULL) {
        printf("\nError: Couldn't save file");
        printf("\n");
        return;
    }

    while (current != NULL) {
        fprintf(f, "%s\n", current->first_name);
        fprintf(f, "%s\n", current->last_name);
        fprintf(f, "%s\n", current->company);
        fprintf(f, "%s\n", current->email);
        fprintf(f, "%s\n", current->tel_number);

        current = current->next;
    }

    fclose(f);

    printf("\n -> Saved to file '%s' successfully.", fileName);
    printf("\n");
}
// loads from file with given name
contact_t* loadFromFile(const char* fileName, contact_t* head) {
    char fullFileName[100+1];
    sprintf(fullFileName, "./%s", fileName);

    FILE *f = fopen(fullFileName, "r");
    char* line = NULL;
    size_t len = 0;
    int index = 0;
    contact_t newContact = {0};

    // Check if file could be opened
    if (f == NULL) {
        printf("\nError: Couldn't load file");
        printf("\n");
        return head;
    }

    // delete all entries
    head = deleteAllContacts(head);

    while (getline(&line, &len, f) != -1) {
        line[strlen(line) - 1] = 0;

        switch (index % 5) {
            case 0:
                strcpy(newContact.first_name, line);
                break;
            case 1:
                strcpy(newContact.last_name, line);
                break;
            case 2:
                strcpy(newContact.company, line);
                break;
            case 3:
                strcpy(newContact.email, line);
                break;
            case 4:
                strcpy(newContact.tel_number, line);
                head = addContact(newContact, head);
                break;
            default:
                break;
        }

        index++;
    }

    fclose(f);
    if (line) free(line);

    printf("\n -> Loaded from file '%s' successfully.", fileName);
    printf("\n");

    return head;
}

/* HELPER */

// returns the index of a given contact in the list
int getIndexOfContact(contact_t* contact, contact_t* head) {
    int index = 0;
    contact_t* current = head;
    while (current != NULL) {
        if (current == contact) {
            return index;
        }
        index++;
        current = current->next;
    }
    return -1; // Couldn't find element
}
// returns the pointer to the contact at index
contact_t* getContactAtIndex(int index, contact_t* head) {
    contact_t* current = head;
    int length = getLength(head);
    if (index < 0) index = 0;
    if (index >= length - 1) index = length-1;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}
// returns length of list
int getLength(contact_t* head) {
    int length = 0;
    contact_t* current = head;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}
// returns pointer to last element in list ('NULL' when list is empty)
contact_t* getLastContact(contact_t* head) {
    contact_t* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}





/*
 *      MENUS --------------------------------------------------------------------
 * */


/* MENUS */

void printMainMenu(contact_t* head) {
    int a = 0; // User input

    printf("\n--------------------------------");
    printf("\n---------- Main Menu -----------");
    printf("\n--------------------------------");
    printf("\n");
    printf("\n(1) Add Contact");
    printf("\n(2) Show Contacts");
    printf("\n(3) Edit Contact");
    printf("\n(4) Search for Contact");
    printf("\n(5) Delete Contact");
    printf("\n(6) Sort Contacts");
    printf("\n(7) Calculate Memory");
    printf("\n");
    printf("\n(8) Load from file");
    printf("\n(9) Save to file");
    printf("\n");
    printf("\n(0) Close Program");
    printf("\n");
    printf("\nPlease enter: ");

    scanf("%d", &a);

    switch (a) {
        case 1:
            printAddContactMenu(head);
            break;
        case 2:
            printContacts(head);
            break;
        case 3:
            printEditContactMenu(head);
            break;
        case 4:
            printSearchContactMenu(head);
            break;
        case 5:
            printDeleteContactMenu(head);
            break;
        case 6:
            printSortContactsMenu(head);
            break;
        case 7:
            printMemoryMenu(head);
            break;
        case 8:
            printLoadFromFileMenu(head);
            break;
        case 9:
            printSaveToFileMenu(head);
            break;
        case 0:
            return;
        default:
            printSecret(a, head);
            return;
    }

}
void printAddContactMenu(contact_t* head) {
    contact_t newContact = {0};

    printf("\n--------------------------------");
    printf("\n-------- Add Contact -----------");
    printf("\n");
    printf("\nFirst Name: ");
    fgets(newContact.first_name, 100, stdin);
    scanf("%[^\n]s", newContact.first_name);
    printf("Last Name : ");
    fgets(newContact.last_name, 100, stdin);
    scanf("%[^\n]s", newContact.last_name);
    printf("Company   : ");
    fgets(newContact.company, 100, stdin);
    scanf("%[^\n]s", newContact.company);
    printf("Email     : ");
    fgets(newContact.email, 100, stdin);
    scanf("%[^\n]s", newContact.email);
    printf("Telephone : ");
    fgets(newContact.tel_number, 100, stdin);
    scanf("%[^\n]s", newContact.tel_number);

    head = addContact(newContact, head);

    printf("\n -> %s %s was added to your Address Book.", newContact.first_name, newContact.last_name);
    printf("\n");

    printContinueMenu();
    printMainMenu(head);
}
void printDeleteContactMenu(contact_t* head) {
    int a = 0; // User Input
    int oldLen = getLength(head); // Length of list before deletion

    printf("\n--------------------------------");
    printf("\n-------- Delete Contact --------");
    printf("\n");
    if (getLength(head) == 0) {
        printf("\nYou have no entries yet.");
        printf("\n");
    } else {
        printf("\n- Select contact to delete -");
        printf("\n");
        printContactNames(head);
        printf("\n(0) Cancel");
        printf("\n");
        printf("\nPlease enter: ");
        scanf("%d", &a);

        if (a == 0) {
            printMainMenu(head);
            return;
        } else {
            a--;

            contact_t* deleted = getContactAtIndex(a, head);
            head = deleteContact(a, head);

            if (a < 0 || a >= oldLen) {
                printSelectionError();
                printDeleteContactMenu(head);
                return;
            } else {
                printf("\n -> %s %s was deleted from your address book.", deleted->first_name, deleted->last_name);
                printf("\n");
            }
        }
    }

    printContinueMenu();
    printMainMenu(head);
}
void printContacts(contact_t* head) {
    contact_t* current = head;

    printf("\n--------------------------------");
    printf("\n-------- My Contacts -----------");
    printf("\n--------------------------------");
    printf("\n");

    if (current == NULL) {
        printf("\nYou have no entries yet.");
        printf("\n");
    }

    while(current != NULL) {
        printf("\n------- Contact (%d) -----------", getIndexOfContact(current, head) + 1);
        printContact(current);

        current = current->next;
    }
    printContinueMenu();
    printMainMenu(head);
}
void printEditContactMenu(contact_t* head) {
    int a = 0; // User Input

    printf("\n--------------------------------");
    printf("\n--------- Edit Contact ---------");
    printf("\n");
    printf("\n- Select contact to edit -");
    printf("\n");
    printContactNames(head);
    printf("\n(0) Cancel");
    printf("\n");
    printf("\nPlease Enter: ");

    scanf("%d", &a);

    if (a > 0 && a <= getLength(head)) {
        a--;
        printEditSingleContactMenu(getContactAtIndex(a, head), head);
    } else if (a == 0) {
        printMainMenu(head);
    } else {
        printSelectionError();
        printEditContactMenu(head);
    }

}
void printEditSingleContactMenu(contact_t* contact, contact_t* head) {
    int a;

    printf("\n--------------------------------");
    printf("\n--------- Edit Contact ---------");
    printf("\n");
    printf("\n- Select field to edit -");
    printf("\n");
    printf("\n(1) First Name: %s", contact->first_name);
    printf("\n(2) Last Name : %s", contact->last_name);
    printf("\n(3) Company   : %s", contact->company);
    printf("\n(4) Email     : %s", contact->email);
    printf("\n(5) Telephone : %s", contact->tel_number);
    printf("\n");
    printf("\n(0) Cancel");
    printf("\n");
    printf("\nPlease enter: ");

    scanf("%d", &a);

    if (a > 0 && a < 6) {
        printEditPropertyMenu(contact, a, head);
    } else if (a == 0) {
        printEditContactMenu(head);
    } else {
        printSelectionError();
        printEditSingleContactMenu(contact, head);
    }

}
void printEditPropertyMenu(contact_t* contact, int code, contact_t* head) {
    printf("\n--------------------------------");
    printf("\n--------- Edit Contact ---------");
    printf("\n");
    printContact(contact);

    switch (code) {
        case 1:
            printf("\nNew First Name: ");
            fgets(contact->first_name, 100, stdin);
            scanf("%[^\n]s", contact->first_name);
            break;
        case 2:
            printf("\nNew Last Name: ");
            fgets(contact->last_name, 100, stdin);
            scanf("%[^\n]s", contact->last_name);
            break;
        case 3:
            printf("\nNew Company: ");
            fgets(contact->company, 100, stdin);
            scanf("%[^\n]s", contact->company);
            break;
        case 4:
            printf("\nNew Email: ");
            fgets(contact->email, 100, stdin);
            scanf("%[^\n]s", contact->email);
            break;
        case 5:
            printf("\nNew Tel. Number: ");
            fgets(contact->tel_number, 100, stdin);
            scanf("%[^\n]s", contact->tel_number);
            break;
        default:
            break;
    }
    printf("\n");
    printf(" -> Change was successful.");
    printf("\n");

    printContinueMenu();
    printEditSingleContactMenu(contact, head);
}
void printSearchContactMenu(contact_t* head) {
    char searchString[40+1]; // User Input
    contact_t* current = head;
    int entriesFound = 0;

    printf("\n--------------------------------");
    printf("\n------ Search for Contact ------");
    printf("\n");
    printf("\n- Search by search string -");
    printf("\n");
    printf("\nWill print all contacts containing\nyour search string in any of their information.");
    printf("\n");
    printf("\nPlease enter: ");
    fgets(searchString, 100, stdin);
    scanf("%[^\n]s", searchString);

    while (current != NULL) {

        if (strstr(current->first_name, searchString) ||
            strstr(current->last_name, searchString) ||
            strstr(current->email, searchString) ||
            strstr(current->company, searchString) ||
            strstr(current->tel_number, searchString)) {
            printf("\n------- Contact (%d) -----------", getIndexOfContact(current, head) + 1);
            printContact(current);

            entriesFound++;
        }

        current = current->next;
    }

    if (entriesFound == 0) {
        printf("\n -> Couldn't find any entries for this query.");
        printf("\n");
    } else if (entriesFound == 1) {
        printf("\n -> Found 1 entry.");
        printf("\n");
    } else {
        printf("\n -> Found %d entries.", entriesFound);
        printf("\n");
    }

    printContinueMenu();
    printMainMenu(head);
}
void printSortContactsMenu(contact_t* head) {
    int a = 0; // User Input

    printf("\n--------------------------------");
    printf("\n-------- Sort Contacts ---------");
    printf("\n");
    printf("\nSort by:");
    printf("\n");
    printf("\n(1) Last Name");
    printf("\n(2) First Name");
    printf("\n(3) Company");
    printf("\n(4) Email");
    printf("\n");
    printf("\n(0) Cancel");
    printf("\n");
    printf("\nPlease enter: ");
    scanf("%d", &a);

    if (a > 0 && a < 5) {
        head = sortByCriterion(a, head);
        printf("\n -> Sorted list successfully.");
        printf("\n");

        printContinueMenu();
        printMainMenu(head);
    } else if (a == 0) {
        printMainMenu(head);
    } else {
        printSelectionError();
        printSortContactsMenu(head);
    }
}
void printMemoryMenu(contact_t* head) {
    printf("\n -> Your address book has a size of %lu bytes.", getLength(head) * sizeof(contact_t));
    printf("\n");

    printContinueMenu();
    printMainMenu(head);
}
void printSaveToFileMenu(contact_t* head) {
    char fileName[30+1];

    printf("\n--------------------------------");
    printf("\n------ Save Address Book -------");
    printf("\n");
    printf("\n- Enter name of file to save to (either existing or new one) -");
    printf("\n");
    printTextFiles();
    printf("\nPlease enter: ");

    scanf("%s", fileName);

    saveToFile(fileName, head);

    printContinueMenu();
    printMainMenu(head);
}
void printLoadFromFileMenu(contact_t* head) {
    char fileName[30+1];

    printf("\n--------------------------------");
    printf("\n------ Load Address Book -------");
    printf("\n");
    printf("\n- Enter name of file to load -");
    printf("\n");
    printTextFiles();
    printf("\nPlease enter: ");

    scanf("%s", fileName);

    head = loadFromFile(fileName, head);

    printContinueMenu();
    printMainMenu(head);
}



/* HELPER */

// prints single contact
void printContact(contact_t* contact) {
    //printf("\n--------------------------------");
    printf("\nName : %s %s (%s)", contact->first_name, contact->last_name, contact->company);
    printf("\nEmail: %s", contact->email);
    printf("\nTel. : %s", contact->tel_number);
    printf("\n");
}
// prints only the names of all contacts
void printContactNames(contact_t* head) {
    contact_t* current = head;
    while (current != NULL) {
        printContactName(current, head);
        current = current->next;
    }
    printf("\n");
}
// prints only the name of one contact
void printContactName(contact_t* contact, contact_t* head) {
    printf("\n(%d) %s %s (%s)", getIndexOfContact(contact, head) + 1, contact->first_name, contact->last_name, contact->company);
}
// prints '- Press Enter to continue -' and waits for 'enter' key
void printContinueMenu() {
    printf("\n- Press Enter to continue -");
    getchar();
    getchar();
    printf("\n");
}
// prints 'You didn't select anything.' and waits for 'enter key'
void printSelectionError() {
    printf("\n -> You didn't select anything.");
    printf("\n");
    printContinueMenu();
}
// prints all files in ./text-files
void printTextFiles() {
    DIR *d;
    struct dirent *dir;
    d = opendir("./files");
    if (d) {
        printf("\nFiles in 'files/': \n");
        while ((dir = readdir(d)) != NULL) {
            printf("\n%s", dir->d_name);
        }
        closedir(d);
        printf("\n");
    }
}


/*
 *      MAIN ---------------------------------------------------------------------
 * */

int main(int argc, const char* argv[], const char * env[]) {
    contact_t* head = NULL; // first element of list

    if (argc == 2) {
        head = loadFromFile(argv[1], head);
        printContinueMenu();
    } else {
        head = fillList(head);  // fill list with sample data
    }

    printMainMenu(head);    // prints main menu

    return 0;
}


// fills list with sample data
contact_t* fillList(contact_t* head) {

    contact_t person = {0};

    strcpy(person.first_name, "Bill");
    strcpy(person.last_name, "Gates");
    strcpy(person.company, "Microsoft");
    strcpy(person.email, "bill@microsoft.com");
    strcpy(person.tel_number, "84720583");

    head = addContact(person, head);

    strcpy(person.first_name, "Steve");
    strcpy(person.last_name, "Jobs");
    strcpy(person.company, "Apple");
    strcpy(person.email, "steve@apple.com");
    strcpy(person.tel_number, "37462940");

    head = addContact(person, head);

    strcpy(person.first_name, "Elon");
    strcpy(person.last_name, "Musk");
    strcpy(person.company, "SpaceX");
    strcpy(person.email, "elon@spacex.com");
    strcpy(person.tel_number, "20572948");

    head = addContact(person, head);

    strcpy(person.first_name, "Mark");
    strcpy(person.last_name, "Zuckerberg");
    strcpy(person.company, "Facebook");
    strcpy(person.email, "zuck@facebook.com");
    strcpy(person.tel_number, "52837783");

    head = addContact(person, head);

    return head;
}
// secret
void printSecret(int code, contact_t* head) {
    char msg[50+1];

    switch (code) {
        case 42:
            strcpy(msg, "That's the answer to everything.");
            break;
        default:
            printSelectionError();
            printMainMenu(head);
            return;
    }

    printf("\n%s", msg);
    printf("\n");
    printContinueMenu();
}



/*
 *
 * REQUIREMENTS:
 *  1. add element
 *  2. show list
 *  3. delete element
 *  4. sort list
 *  5. save in file (File System)
 *  6. load from file (File System)
 *  7. search for ...
 *  8. calculate memory (Extras)
 *  9. edit element
 *  0. exit/end
 *
 * */