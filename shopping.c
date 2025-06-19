#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h>
#define PAUSE() getch()
#define CLEAR_SCREEN "cls"
#else
#define PAUSE() do { printf("Press Enter to continue..."); getchar(); getchar(); } while(0)
#define CLEAR_SCREEN "clear"
#endif

#define MAX_NAME 50

// Product Node
typedef struct Product {
    int id;
    char name[MAX_NAME];
    int price;
    int qty;
    struct Product *next;
} Product;

// Cart Node
typedef struct CartItem {
    int id;
    int qty;
    struct CartItem *next;
} CartItem;

Product *productList = NULL;
CartItem *cartList = NULL;

Product* findProduct(int id) {
    Product *p = productList;
    while (p) {
        if (p->id == id) return p;
        p = p->next;
    }
    return NULL;
}

CartItem* findCartItem(int id) {
    CartItem *c = cartList;
    while (c) {
        if (c->id == id) return c;
        c = c->next;
    }
    return NULL;
}

void addProduct() {
    Product *newP = (Product *)malloc(sizeof(Product));
    printf("\nEnter Product ID: ");
    scanf("%d", &newP->id);
    if (findProduct(newP->id)) {
        printf("Product ID already exists!\n");
        free(newP);
        PAUSE();
        return;
    }

    printf("Enter Product Name: ");
    scanf("%49s", newP->name);
    printf("Enter Price: ");
    scanf("%d", &newP->price);
    printf("Enter Quantity: ");
    scanf("%d", &newP->qty);

    if (newP->price < 0 || newP->qty < 0) {
        printf("Invalid input! Price and Quantity must be non-negative.\n");
        free(newP);
        PAUSE();
        return;
    }

    newP->next = productList;
    productList = newP;
    printf("Product added successfully!\n");
    PAUSE();
}

void displayProducts() {
    Product *p = productList;
    if (!p) {
        printf("\nNo products available.\n");
        return;
    }
    printf("\n%-10s %-20s %-10s %-10s\n", "ID", "Name", "Qty", "Price");
    while (p) {
        printf("%-10d %-20s %-10d %-10d\n", p->id, p->name, p->qty, p->price);
        p = p->next;
    }
}

void removeProduct() {
    int id;
    printf("Enter Product ID to remove: ");
    scanf("%d", &id);
    Product *p = productList, *prev = NULL;
    while (p && p->id != id) {
        prev = p;
        p = p->next;
    }
    if (!p) {
        printf("Product not found!\n");
        PAUSE();
        return;
    }
    if (!prev) productList = p->next;
    else prev->next = p->next;
    free(p);
    printf("Product removed successfully.\n");
    PAUSE();
}

void addToCart() {
    int id, qty;
    printf("Enter Product ID to add to cart: ");
    scanf("%d", &id);
    Product *p = findProduct(id);
    if (!p) {
        printf("Product not found!\n");
        PAUSE();
        return;
    }
    printf("Enter quantity (1-10): ");
    scanf("%d", &qty);
    if (qty < 1 || qty > 10 || qty > p->qty) {
        printf("Invalid quantity.\n");
        PAUSE();
        return;
    }
    CartItem *c = findCartItem(id);
    if (c) {
        if (c->qty + qty > p->qty) {
            printf("Cannot add more than available stock.\n");
            PAUSE();
            return;
        }
        c->qty += qty;
    } else {
        c = (CartItem *)malloc(sizeof(CartItem));
        c->id = id;
        c->qty = qty;
        c->next = cartList;
        cartList = c;
    }
    printf("Added to cart.\n");
    PAUSE();
}

void removeFromCart() {
    int id;
    printf("Enter Product ID to remove from cart: ");
    scanf("%d", &id);

    CartItem *c = cartList, *prev = NULL;
    while (c && c->id != id) {
        prev = c;
        c = c->next;
    }

    if (!c) {
        printf("Item not found in cart.\n");
    } else {
        if (!prev) cartList = c->next;
        else prev->next = c->next;
        free(c);
        printf("Item removed from cart.\n");
    }
    PAUSE();
}

void modifyCartQuantity() {
    int id, qty;
    printf("Enter Product ID to modify in cart: ");
    scanf("%d", &id);
    CartItem *c = findCartItem(id);
    Product *p = findProduct(id);
    if (!c || !p) {
        printf("Item not found in cart or product list.\n");
        PAUSE();
        return;
    }

    printf("Enter new quantity (1-10): ");
    scanf("%d", &qty);
    if (qty < 1 || qty > 10 || qty > p->qty) {
        printf("Invalid quantity.\n");
    } else {
        c->qty = qty;
        printf("Quantity updated.\n");
    }
    PAUSE();
}

void viewCart() {
    CartItem *c = cartList;
    if (!c) {
        printf("Cart is empty.\n");
        PAUSE();
        return;
    }

    printf("\n%-10s %-20s %-10s %-10s\n", "ID", "Name", "Qty", "Price");
    int total = 0;
    while (c) {
        Product *p = findProduct(c->id);
        if (p) {
            printf("%-10d %-20s %-10d %-10d\n", p->id, p->name, c->qty, p->price);
            total += c->qty * p->price;
        }
        c = c->next;
    }
    printf("\nTotal Amount: Rs %d\n", total);

    int ch;
    printf("\n1. Modify Cart Quantity\n2. Remove Item\n0. Back\nEnter choice: ");
    scanf("%d", &ch);
    switch (ch) {
        case 1: modifyCartQuantity(); break;
        case 2: removeFromCart(); break;
        default: break;
    }
}

void checkout() {
    CartItem *c = cartList;
    while (c) {
        Product *p = findProduct(c->id);
        if (p && p->qty >= c->qty) {
            p->qty -= c->qty;
        }
        c = c->next;
    }
    while (cartList) {
        CartItem *tmp = cartList;
        cartList = cartList->next;
        free(tmp);
    }
    printf("\nCheckout complete. Thank you for shopping!\n");
    PAUSE();
}

void manageProductsMenu() {
    int choice;
    while (1) {
        system(CLEAR_SCREEN);
        printf("\n--- Manage Products ---\n");
        printf("1. Add Product\n2. Display Products\n3. Remove Product\n0. Back\nEnter choice: ");
        scanf("%d", &choice);

        system(CLEAR_SCREEN);
        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                displayProducts();
                PAUSE();
                break;
            case 3:
                removeProduct();
                break;
            case 0:
                return;
            default:
                printf("Invalid choice!\n");
                PAUSE();
        }
    }
}

void purchaseMenu() {
    int choice;
    while (1) {
        system(CLEAR_SCREEN);
        printf("\n--- Purchase Products ---\n");
        displayProducts();
        printf("\n1. Add to Cart\n2. View Cart\n3. Checkout\n0. Back\nEnter choice: ");
        scanf("%d", &choice);

        system(CLEAR_SCREEN);
        switch (choice) {
            case 1:
                addToCart();
                break;
            case 2:
                viewCart();
                break;
            case 3:
                checkout();
                break;
            case 0:
                return;
            default:
                printf("Invalid choice!\n");
                PAUSE();
        }
    }
}

void cleanup() {
    Product *p;
    while (productList) {
        p = productList;
        productList = productList->next;
        free(p);
    }
    CartItem *c;
    while (cartList) {
        c = cartList;
        cartList = cartList->next;
        free(c);
    }
}

int main() {
    int choice;
    do {
        system(CLEAR_SCREEN);
        printf("\n===== Shopping Cart System =====\n");
        printf("1. Manage Products\n2. Purchase Products\n0. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: manageProductsMenu(); break;
            case 2: purchaseMenu(); break;
            case 0: break;
            default:
                printf("Invalid choice!\n");
                PAUSE();
        }
    } while (choice != 0);

    cleanup();
    printf("Goodbye!\n");
    return 0;
}

