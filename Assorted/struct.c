#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* name;
    int id;
    float salary;
} Person;

void printPersonCopy(Person p) {
    printf("Name: %s\nID: %d\nSalary: $%0.2f\n\n", p.name, p.id, p.salary);
}

void printPersonPtr(Person* p) {
    printf("Name: %s\nID: %d\nSalary: $%0.2f\n\n", p->name, p->id, p->salary);
}

Person* createPerson(const char name[], int id, float salary) {
    Person* pPtr = malloc(sizeof(Person));
    pPtr->name = malloc(strlen(name));

    strcpy(pPtr->name, name);
    pPtr->id = id;
    pPtr->salary = salary;

    return pPtr;
}

void destroyPerson(Person* p) {
    free(p->name);
    free(p);
}

int main() {
    // Put struct before declaration
    Person guyOne = { "Timmy", 0, 123.456 };
    printPersonCopy(guyOne);

    Person employees[] = {
        { "Bob",     82,  38000 },
        { "Jackson", 73, 121000 },
        { "Sally",   28, 112000 }
    };

    for (size_t i = 0; i < 3; i++)
        printPersonCopy(employees[i]);

    Person* fella = createPerson("Sarah", 101, 87525.28);
    printPersonPtr(fella);

    destroyPerson(fella);
}
