#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum bool { false, true };




/*
 *      STRUCTS ------------------------------------------
 * */

struct contact_t {
    // Data
    char first_name[20+1];
    char last_name[20+1];
    char company[30+1];
    char email[30+1];
    char tel_number[20+1];

    // Pointers
    struct contact_t* prev;
    struct contact_t* next;
};

// TODO: don't declare as global variable
struct contact_t* head;


/*
 *      FUNCTION DECLARATIONS
 * */

// PRINT MENU
void printMainMenu();
void printContinueMenu();
void printAddContactMenu();
void printEditContactMenu();
void printEditSingleContactMenu(struct contact_t* contact);
void printEditPropertyMenu(struct contact_t* contact, int code);
void printDeleteContactMenu();
void printSortContactsMenu();
void printSecret(int code);

// PRINT HELPER
void printContacts();
void printContact(struct contact_t* contact);
void printContactNames();
void printContactName(struct contact_t* contact);
void printSelectionError();

// LIST EDIT
struct contact_t* createNewContact(struct contact_t person);
void addContact(struct contact_t contact);
void deleteContact(int index);
void deleteFirstContact();
void deleteLastContact();
void sortByCriterion(int criterion);
void swapContacts(struct contact_t* first);
enum bool firstFollowsSecond(struct contact_t* first, struct contact_t* second, int criterion);

// LIST HELPER
int getIndexOfContact(struct contact_t* contact);
struct contact_t* getContactAtIndex(int index);
int getLength();
struct contact_t* getLastContact();
void fillList();


/*
 *      LIST ---------------------------------------------
 * */

// Creates a new contact in memory and returns its pointer
struct contact_t* createNewContact(struct contact_t person) {
    struct contact_t* newContact = (struct contact_t*)malloc(sizeof(struct contact_t));
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
void addContact(struct contact_t contact) {
    struct contact_t* current = head;
    struct contact_t* newContact = createNewContact(contact);
    if (head == NULL) {
        head = newContact;
        return;
    }
    while (current->next != NULL) current = current->next;
    current->next = newContact;
    newContact->prev =current;
}

/*
 *      DELETE
 * */

// deletes a contact at a given index
void deleteContact(int index) {
    if (getLength() <= 1) {
        head = NULL;
    } else {
        if (index < 0 || index >= getLength()) { return; }
        if (index == 0) { deleteFirstContact(); return; }
        if (index == getLength()-1) { deleteLastContact(); return; }

        struct contact_t* current;
        current = getContactAtIndex(index);

        current->prev->next = current->next;
        current->next->prev = current->prev;

        free(current);
    }
}

// deletes first contact in list
void deleteFirstContact() {
    struct contact_t* todelete = head;
    head->next->prev = NULL;
    head = head->next;
    free(todelete);
}

// deletes last contact in list
void deleteLastContact() {
    struct contact_t* last = getLastContact();
    last->prev->next = NULL;
    free(last);
}

/*
 *      SORTING
 * */

// sorts list using 'bubble sort'
void sortByCriterion(int criterion) {

    int i, j;
    struct contact_t* current;

    for (i = getLength(); i > 1; i--) {
        for (j = 0; j < i-1; j++) {
            current = getContactAtIndex(j);
            if (firstFollowsSecond(current, current->next, criterion))
                swapContacts(getContactAtIndex(j));
        }
    }
}

// swaps contact with its next contact
void swapContacts(struct contact_t* first) {

    if (first->next) {
        struct contact_t* before = first->prev;
        struct contact_t* second = first->next;
        struct contact_t* after = second->next;

        second->next = first;
        first->next = after;
        second->prev = before;
        first->prev = second;

        if (before != NULL) before->next = second;
        if (after != NULL) after->prev = first;
        if (first == head) head = second;
    }
}

// compares specific property (criterion) of two contacts
enum bool firstFollowsSecond(struct contact_t* first, struct contact_t* second, int criterion) {
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

    return (enum bool) (compareResult > 0);
}

/* HELPER */

// returns the index of a given contact in the list
int getIndexOfContact(struct contact_t* contact) {
    int index = 0;
    struct contact_t* current = head;
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
struct contact_t* getContactAtIndex(int index) {
    struct contact_t* current = head;
    int length = getLength();
    if (index < 0) index = 0;
    if (index >= length - 1) index = length-1;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}
// returns length of list
int getLength() {
    int length = 0;
    struct contact_t* current = head;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}
// returns pointer to last element in list ('NULL' when list is empty)
struct contact_t* getLastContact() {
    struct contact_t* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}


/*
 *      MENUS -------------------------------------------
 * */


/* MENUS */

void printMainMenu() {
    int a = 0; // User input

    printf("\n--------------------------------");
    printf("\n---------- Main Menu -----------");
    printf("\n--------------------------------");
    printf("\n");
    printf("\n(1) Add Contact");
    printf("\n(2) Show Contacts");
    printf("\n(3) Edit Contact");
    printf("\n(4) Delete Contact");
    printf("\n(5) Sort Contacts");
    printf("\n");
    printf("\n(0) Close Program");
    printf("\n");
    printf("\nPlease enter: ");

    scanf("%d", &a);

    switch (a) {
        case 1:
            printAddContactMenu();
            break;
        case 2:
            printContacts();
            break;
        case 3:
            printEditContactMenu();
            break;
        case 4:
            printDeleteContactMenu();
            break;
        case 5:
            printSortContactsMenu();
            break;
        case 0:
            return;
        default:
            printSecret(a);
            return;
    }

}

void printAddContactMenu() {
    struct contact_t newContact = {0};

    /*char first_name[20+1];
    char last_name[20+1];
    char company[30+1];
    char email[30+1];
    char tel_number[20+1];*/

    printf("\n--------------------------------");
    printf("\n-------- Add Contact -----------");
    printf("\n");
    printf("\nFirst Name: ");
    scanf("%s", newContact.first_name);
    printf("Last Name : ");
    scanf("%s", newContact.last_name);
    printf("Company   : ");
    scanf("%s", newContact.company);
    printf("Email     : ");
    scanf("%s", newContact.email);
    printf("Telephone : ");
    scanf("%s", newContact.tel_number);

    addContact(newContact);

    printf("\n%s %s was added to your Address Book.", newContact.first_name, newContact.last_name);
    printf("\n");

    printContinueMenu();
    printMainMenu();
}

void printDeleteContactMenu() {
    int a = 0; // User Input
    int oldLen = getLength(); // Length of list before deletion

    printf("\n--------------------------------");
    printf("\n-------- Delete Contact --------");
    printf("\n");
    if (getLength() == 0) {
        printf("\nYou have no entries yet.");
        printf("\n");
    } else {
        printf("\n- Select contact to delete -");
        printf("\n");
        printContactNames();
        printf("\n(0) Cancel");
        printf("\n");
        printf("\nPlease enter: ");
        scanf("%d", &a);

        if (a == 0) {
            printMainMenu();
            return;
        } else {
            a--;

            struct contact_t* deleted = getContactAtIndex(a);
            deleteContact(a);

            if (a < 0 || a >= oldLen) {
                printSelectionError();
                printDeleteContactMenu();
                return;
            } else {
                printf("\n%s %s was deleted from your Address Book.", deleted->first_name, deleted->last_name);
                printf("\n");
            }
        }
    }

    printContinueMenu();
    printMainMenu();
}

void printContacts() {
    struct contact_t* current = head;

    printf("\n--------------------------------");
    printf("\n-------- My Contacts -----------");
    printf("\n--------------------------------");
    printf("\n");

    if (current == NULL) {
        printf("\nYou have no entries yet.");
        printf("\n");
    }

    while(current != NULL) {
        printf("\n------- Contact (%d) -----------", getIndexOfContact(current) + 1);
        printContact(current);

        current = current->next;
    }
    printContinueMenu();
    printMainMenu();
}

void printEditContactMenu() {
    int a = 0; // User Input

    printf("\n--------------------------------");
    printf("\n--------- Edit Contact ---------");
    printf("\n");
    printf("\n- Select contact to edit -");
    printf("\n");
    printContactNames();
    printf("\n(0) Cancel");
    printf("\n");
    printf("\nPlease Enter: ");

    scanf("%d", &a);

    if (a > 0 && a <= getLength()) {
        a--;
        printEditSingleContactMenu(getContactAtIndex(a));
    } else if (a == 0) {
        printMainMenu();
    } else {
        printSelectionError();
        printEditContactMenu();
    }

}
void printEditSingleContactMenu(struct contact_t* contact) {
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
        printEditPropertyMenu(contact, a);
    } else if (a == 0) {
        printEditContactMenu();
    } else {
        printSelectionError();
        printEditSingleContactMenu(contact);
    }


    // TODO: Edit Single Contact
}
void printEditPropertyMenu(struct contact_t* contact, int code) {
    printf("\n--------------------------------");
    printf("\n--------- Edit Contact ---------");
    printf("\n");
    printContact(contact);

    switch (code) {
        case 1:
            printf("\nNew First Name: ");
            scanf("%s", contact->first_name);
            break;
        case 2:
            printf("\nNew Last Name: ");
            scanf("%s", contact->last_name);
            break;
        case 3:
            printf("\nNew Company: ");
            scanf("%s", contact->company);
            break;
        case 4:
            printf("\nNew Email: ");
            scanf("%s", contact->email);
            break;
        case 5:
            printf("\nNew Tel. Number: ");
            scanf("%s", contact->tel_number);
            break;
        default:
            break;
    }
    printf("\n");
    printf("Change was successful.");
    printf("\n");

    printContinueMenu();
    printEditSingleContactMenu(contact);
}

void printSortContactsMenu() {
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
        sortByCriterion(a);
        printf("\nSorted list successfully.");
        printf("\n");

        printContinueMenu();
        printMainMenu();
    } else if (a == 0) {
        printMainMenu();
    } else {
        printSelectionError();
        printSortContactsMenu();
    }
}


/* HELPER */

// prints single contact
void printContact(struct contact_t* contact) {
    //printf("\n--------------------------------");
    printf("\nName : %s %s (%s)", contact->first_name, contact->last_name, contact->company);
    printf("\nEmail: %s", contact->email);
    printf("\nTel. : %s", contact->tel_number);
    printf("\n");
}
// prints only the names of all contacts
void printContactNames() {
    struct contact_t* current = head;
    while (current != NULL) {
        printContactName(current);
        current = current->next;
    }
    printf("\n");
}
// prints only the name of one contact
void printContactName(struct contact_t* contact) {
    printf("\n(%d) %s %s (%s)", getIndexOfContact(contact) + 1, contact->first_name, contact->last_name, contact->company);
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
    printf("\nYou didn't select anything.");
    printf("\n");
    printContinueMenu();
}


/*
 *      MAIN --------------------------------------------
 * */

int main() {

    fillList();
    printMainMenu();


    return 0;
}



void fillList() {

    struct contact_t person = {0};

    strcpy(person.first_name, "Bill");
    strcpy(person.last_name, "Gates");
    strcpy(person.company, "Microsoft");
    strcpy(person.email, "bill@microsoft.com");
    strcpy(person.tel_number, "84720583");

    addContact(person);

    strcpy(person.first_name, "Steve");
    strcpy(person.last_name, "Jobs");
    strcpy(person.company, "Apple");
    strcpy(person.email, "steve@apple.com");
    strcpy(person.tel_number, "37462940");

    addContact(person);

    strcpy(person.first_name, "Elon");
    strcpy(person.last_name, "Musk");
    strcpy(person.company, "SpaceX");
    strcpy(person.email, "elon@spacex.com");
    strcpy(person.tel_number, "20572948");

    addContact(person);

    strcpy(person.first_name, "Mark");
    strcpy(person.last_name, "Zuckerberg");
    strcpy(person.company, "Facebook");
    strcpy(person.email, "zuck@facebook.com");
    strcpy(person.tel_number, "52837783");

    addContact(person);
}

void printSecret(int code) {
    char msg[50+1];

    switch (code) {
        case 42:
            strcpy(msg, "That's the answer to everything.");
            break;
        default:
            printSelectionError();
            printMainMenu();
            return;
    }

    printf("\n%s", msg);
    printf("\n");
    printContinueMenu();
}



/*
 * ANFORDERUNGEN:
 *  1. add element  YES
 *  2. show list    YES
 *  3. delete element   YES
 *  4. sort list    YES
 *  5. save in file (File System) NO
 *  6. load from file (File System) NO
 *  7. search for ...   NO
 *  8. calculate memory (Extras)    NO
 *  9. edit element YES
 *  0. exit/end     YES
 *
 *
 *
 * */