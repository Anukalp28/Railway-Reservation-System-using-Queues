#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SEATS 5
#define MAX_WAIT 5

// Structure to store passenger information
typedef struct {
    int id;
    char name[30];
    int age;
} Passenger;

// Confirmed list 
Passenger confirmed[MAX_SEATS];
int confirmed_count = 0;

// Waiting List Queue (FIFO) 
Passenger wait_queue[MAX_WAIT];
int front = -1, rear = -1;

int next_id = 101;

// Check if waiting queue is full 
int isQueueFull() {
    return (rear == MAX_WAIT - 1);
}

// Check if waiting queue is empty 
int isQueueEmpty() {
    return (front == -1 || front > rear);
}

// Enqueue a passenger into waiting list
void enqueue(Passenger p) {
    if (isQueueFull()) {
        printf("\nSorry, Waiting List is also FULL. Cannot book ticket.\n");
        return;
    }
    if (front == -1) front = 0;
    rear++;
    wait_queue[rear] = p;
    printf("\nConfirmed seats full! Passenger %s added to WAITING LIST (Position: %d, ID: %d).\n", 
           p.name, (rear - front + 1), p.id);
}

// Dequeue the first passenger from waiting list
int dequeue(Passenger *p) {
    if (isQueueEmpty()) return 0;
    *p = wait_queue[front++];
    if (front > rear) front = rear = -1; // reset queue
    return 1;
}

// Book a ticket
void bookTicket() {
    Passenger p;
    p.id = next_id++;

    printf("\nEnter Passenger Name: ");
    scanf(" %29[^\n]", p.name);
    printf("Enter Age: ");
    if (scanf("%d", &p.age) != 1) {
        printf("Invalid age entered.\n");
        while (getchar() != '\n'); // clear buffer
        return;
    }

    if (confirmed_count < MAX_SEATS) {
        confirmed[confirmed_count++] = p;
        printf("\nSuccess: Ticket CONFIRMED! (ID: %d, Name: %s, Seat No: %d)\n", 
               p.id, p.name, confirmed_count);
    } else {
        enqueue(p);
    }
}

//  Cancel a ticket
void cancelTicket() {
    if (confirmed_count == 0) {
        printf("\nNo confirmed tickets to cancel.\n");
        return;
    }

    int id, found = -1;
    printf("\nEnter Ticket ID to cancel: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }

    for (int i = 0; i < confirmed_count; i++) {
        if (confirmed[i].id == id) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("\nTicket ID %d not found in confirmed list.\n", id);
        return;
    }

    printf("\nTicket ID %d for %s CANCELLED successfully.\n", confirmed[found].id, confirmed[found].name);

    Passenger promoted;
    if (dequeue(&promoted)) {
        confirmed[found] = promoted;
        printf(">> Waiting list passenger %s (ID: %d) PROMOTED to Confirmed Seat %d!\n", 
               promoted.name, promoted.id, found + 1);
    } else {
        for (int i = found; i < confirmed_count - 1; i++) {
            confirmed[i] = confirmed[i + 1];
        }
        confirmed_count--;
    }
}

// Display reservation and queue status
void displayStatus() {
    printf("\n================ RESERVATION STATUS ================\n");
    printf("-- Confirmed Bookings (%d/%d) --\n", confirmed_count, MAX_SEATS);
    if (confirmed_count == 0) {
        printf("  No confirmed reservations.\n");
    } else {
        for (int i = 0; i < confirmed_count; i++) {
            printf("  Seat %d | ID: %d | Name: %-15s | Age: %d\n", 
                   i + 1, confirmed[i].id, confirmed[i].name, confirmed[i].age);
        }
    }

    printf("\n-- Waiting List Queue (Capacity: %d, FIFO) --\n", MAX_WAIT);
    if (isQueueEmpty()) {
        printf("  Waiting list is empty.\n");
    } else {
        int pos = 1;
        for (int i = front; i <= rear; i++) {
            printf("  WL #%d | ID: %d | Name: %-15s | Age: %d\n", 
                   pos++, wait_queue[i].id, wait_queue[i].name, wait_queue[i].age);
        }
    }
    printf("====================================================\n");
}

int main() {
    int choice;
    while (1) {
        printf("\n===== RAILWAY RESERVATION SYSTEM =====\n");
        printf("1. Book Ticket\n");
        printf("2. Cancel Ticket\n");
        printf("3. Display Status\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid choice. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1: bookTicket(); break;
            case 2: cancelTicket(); break;
            case 3: displayStatus(); break;
            case 4: 
                printf("\nExiting system. Thank you!\n");
                return 0;
            default:
                printf("\nInvalid option! Please enter 1 to 4.\n");
        }
    }
    return 0;
}

