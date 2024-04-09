
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CUSTOMERS 50
#define MAX_PRODUCTS 10
#define GST_RATE 0.18
#define DISCOUNT_RATE 0.1 // 10% discount

#define MAX_LOGIN_ATTEMPTS 3
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20

typedef struct {
    char first_name[50];
    char last_name[50];
    int id;
    char phone_number[15];
    float total_amount;
} Customer;

typedef struct {
    char name[50];
    float price;
    int quantity;
} Product;

bool login();
void addCustomer(Customer *customers, int *num_customers);
void modifyCustomer(Customer *customers, int num_customers);
void generateBill(Customer *customers, int num_customers, Product *products, int num_products);
void displayCustomers(Customer *customers, int num_customers);
void loadCustomersFromFile(Customer *customers, int *num_customers);
void saveCustomersToFile(Customer *customers, int num_customers);

int main() {
    if (!login()) {
        printf("Login failed. Exiting...\n");
        return 1;
    }

    Customer customers[MAX_CUSTOMERS];
    Product products[MAX_PRODUCTS] = {
        {"Product1", 10.00, 20},
        {"Product2", 20.00, 15},
        {"Product3", 15.00, 25},
        {"Product4", 25.00, 30},
        {"Product5", 30.00, 18},
        {"Product6", 18.00, 22},
        {"Product7", 22.00, 10},
        {"Product8", 35.00, 12},
        {"Product9", 40.00, 28},
        {"Product10", 50.00, 20}
    };
    int num_customers = 0;
    int choice;

    loadCustomersFromFile(customers, &num_customers);

    do {
        printf("\n===== Customer Billing System =====\n");
        printf("1. Add Customer\n");
        printf("2. Modify Customer Details\n");
        printf("3. Generate Bill\n");
        printf("4. Display Customers\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addCustomer(customers, &num_customers);
                break;
            case 2:
                modifyCustomer(customers, num_customers);
                break;
            case 3:
                generateBill(customers, num_customers, products, MAX_PRODUCTS);
                break;
            case 4:
                displayCustomers(customers, num_customers);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    saveCustomersToFile(customers, num_customers);

    return 0;
}

bool login() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int attempts = 0;

    printf("===== Login =====\n");
    do {
        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%s", password);

        if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
            printf("Login successful!\n");
            return true;
        } else {
            attempts++;
            printf("Login failed. Please try again.\n");
        }
    } while (attempts < MAX_LOGIN_ATTEMPTS);

    printf("Maximum login attempts reached. Exiting...\n");
    return false;
}

void addCustomer(Customer *customers, int *num_customers) {
    Customer new_customer;
    printf("Enter customer first name: ");
    scanf("%s", new_customer.first_name);
    printf("Enter customer last name: ");
    scanf("%s", new_customer.last_name);
    printf("Enter customer ID: ");
    scanf("%d", &new_customer.id);
    printf("Enter customer phone number: ");
    scanf("%s", new_customer.phone_number);
    new_customer.total_amount = 0;
    customers[*num_customers] = new_customer;
    (*num_customers)++;
    printf("Customer added successfully.\n");
}

void modifyCustomer(Customer *customers, int num_customers) {
    int customer_id, choice;
    printf("Enter customer ID to modify details: ");
    scanf("%d", &customer_id);

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == customer_id) {
            printf("Customer found. What do you want to modify?\n");
            printf("1. First Name\n");
            printf("2. Last Name\n");
            printf("3. ID\n");
            printf("4. Phone Number\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    printf("Enter new first name: ");
                    scanf("%s", customers[i].first_name);
                    printf("First name updated successfully.\n");
                    break;
                case 2:
                    printf("Enter new last name: ");
                    scanf("%s", customers[i].last_name);
                    printf("Last name updated successfully.\n");
                    break;
                case 3:
                    printf("Enter new ID: ");
                    scanf("%d", &customers[i].id);
                    printf("ID updated successfully.\n");
                    break;
                case 4:
                    printf("Enter new phone number: ");
                    scanf("%s", customers[i].phone_number);
                    printf("Phone number updated successfully.\n");
                    break;
                default:
                    printf("Invalid choice.\n");
            }
            return;
        }
    }
    printf("Customer not found.\n");
}

void generateBill(Customer *customers, int num_customers, Product *products, int num_products) {
    int customer_id, product_index, quantity;
    float total_bill = 0;
    printf("Enter customer ID: ");
    scanf("%d", &customer_id);

    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == customer_id) {
            printf("Customer: %s %s\n", customers[i].first_name, customers[i].last_name);
            printf("Enter products purchased:\n");

            do {
                printf("Available products:\n");
                for (int j = 0; j < num_products; j++) {
                    printf("%d. %s - ₹%.2f (Quantity: %d)\n", j + 1, products[j].name, products[j].price, products[j].quantity);
                }
                printf("Enter product index (-1 to stop): ");
                scanf("%d", &product_index);

                if (product_index >= 1 && product_index <= num_products) {
                    printf("Enter quantity: ");
                    scanf("%d", &quantity);

                    if (quantity > 0 && quantity <= products[product_index - 1].quantity) {
                        total_bill += products[product_index - 1].price * quantity;
                        products[product_index - 1].quantity -= quantity;
                    } else {
                        printf("Invalid quantity or not enough stock.\n");
                    }
                } else if (product_index != -1) {
                    printf("Invalid product index.\n");
                }
            } while (product_index != -1);

            float discount = total_bill * DISCOUNT_RATE;
            float gst_amount = (total_bill - discount) * GST_RATE;
            total_bill += gst_amount - discount;

            customers[i].total_amount += total_bill;
            printf("Total bill (including GST %.2f%% and 10%% discount): ₹%.2f\n", GST_RATE * 100, total_bill);
            printf("Bill generated successfully.\n");
            return;
        }
    }
    printf("Customer not found.\n");
}

void displayCustomers(Customer *customers, int num_customers) {
    if (num_customers == 0) {
        printf("No customers to display.\n");
        return;
    }
    printf("===== Customers =====\n");
    for (int i = 0; i < num_customers; i++) {
        printf("Name: %s %s, ID: %d, Phone: %s, Total Amount: ₹%.2f\n", customers[i].first_name, customers[i].last_name, customers[i].id, customers[i].phone_number, customers[i].total_amount);
    }
}

void loadCustomersFromFile(Customer *customers, int *num_customers) {
    FILE *file = fopen("customers.txt", "r");
    if (file == NULL) {
        printf("File not found or unable to open. Starting with no customers.\n");
        return;
    }

    while (fread(&customers[*num_customers], sizeof(Customer), 1, file)) {
        (*num_customers)++;
    }

    fclose(file);
}

void saveCustomersToFile(Customer *customers, int num_customers) {
    FILE *file = fopen("customers.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < num_customers; i++) {
        fprintf(file, "%s %s %d %s %.2f\n", customers[i].first_name, customers[i].last_name, customers[i].id, customers[i].phone_number, customers[i].total_amount);
    }

    fclose(file);
}
