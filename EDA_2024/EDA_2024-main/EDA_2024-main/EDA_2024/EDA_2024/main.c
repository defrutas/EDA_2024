#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for a node in the linked list
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Function to create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a new node at the end of the list
void insertNode(Node** head, int data) {
    Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
    }
    else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Function to print the matrix
void printMatrix(Node** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        Node* current = matrix[i];
        while (current != NULL) {
            printf("%d ", current->data);
            current = current->next;
        }
        printf("\n");
    }
}

// Function to free memory allocated for the matrix
void freeMatrix(Node** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        Node* current = matrix[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(matrix);
}

// Function to update a value in the matrix
void updateMatrix(Node** matrix, int row, int col, int newValue) {
    Node* current = matrix[row - 1]; // Adjusting for 1-based indexing
    for (int i = 1; i < col; i++) {
        if (current == NULL) {
            printf("Invalid position\n");
            return;
        }
        current = current->next;
    }
    if (current == NULL) {
        printf("Invalid position\n");
        return;
    }
    current->data = newValue;
}

// Function to add a new row to the matrix
void addRow(Node*** matrix, int* rows, int* cols) {
    // Ensure matrix is not NULL
    if (*matrix == NULL) {
        fprintf(stderr, "Matrix is NULL\n");
        return;
    }

    // Allocate memory for the new row
    *matrix = (Node**)realloc(*matrix, (*rows + 1) * sizeof(Node*));
    if (*matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Initialize the new row to NULL
    (*matrix)[*rows] = NULL;

    // Determine the number of columns
    int newCols = 0;
    if (*rows > 0) {
        Node* current = (*matrix)[*rows - 1]; // Get the previous row
        while (current != NULL) {
            newCols++;
            current = current->next;
        }
    }

    // Update the number of columns if necessary
    if (newCols > *cols) {
        *cols = newCols;
    }

    // Loop through columns and insert nodes
    for (int i = 0; i < *cols; i++) {
        int data;
        printf("Enter data for row %d, column %d: ", *rows + 1, i + 1);
        scanf("%d", &data);

        // Insert node into the row
        insertNode(&((*matrix)[*rows]), data);
    }

    // Increment the number of rows
    (*rows)++;
}


// Function to add a new column to the matrix
void addColumn(Node** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        Node* current = matrix[i];

        int columnCount = 0; // Initialize column count

        // Traverse the linked list to count the number of columns
        while (current->next != NULL) {
            current = current->next;
            columnCount++;
        }

        // Print the appropriate prompt
        printf("Enter data for row %d, column %d: ", i + 1, columnCount + 2);

        int data;
        scanf("%d", &data);

        // Insert the new node containing the entered data
        insertNode(&(matrix[i]), data);
    }
}


// Function to remove a row from the matrix
void removeRow(Node*** matrix, int* rows, int rowToRemove) {
    if (rowToRemove < 1 || rowToRemove > *rows) {
        printf("Invalid row number\n");
        return;
    }
    free((*matrix)[rowToRemove - 1]);
    for (int i = rowToRemove - 1; i < *rows - 1; i++) {
        (*matrix)[i] = (*matrix)[i + 1];
    }
    *matrix = (Node**)realloc(*matrix, (*rows - 1) * sizeof(Node*));
    (*rows)--;
}

// Function to remove a column from the matrix
void removeColumn(Node** matrix, int rows, int colToRemove) {
    for (int i = 0; i < rows; i++) {
        Node* current = matrix[i];
        if (colToRemove == 1) {
            matrix[i] = current->next;
            free(current);
        }
        else {
            for (int j = 1; j < colToRemove - 1; j++) {
                if (current == NULL || current->next == NULL) {
                    printf("Invalid column number\n");
                    return;
                }
                current = current->next;
            }
            Node* temp = current->next;
            current->next = temp->next;
            free(temp);
        }
    }
}

// Function to find the maximum sum
int maxUniqueColumnSum(int** matrix, int rows, int cols) {
    // Create a DP table to store maximum sums
    int** dp = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        dp[i] = (int*)malloc(cols * sizeof(int));
    }

    // Initialize DP table
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            dp[i][j] = 0;

    // Fill the DP table
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // For the first row, maximum sum is the value itself
            if (i == 0)
                dp[i][j] = matrix[i][j];
            else {
                // For subsequent rows, find the maximum sum
                // considering values from different columns
                int maxSum = 0;
                for (int k = 0; k < cols; k++) {
                    if (j != k) {
                        if (dp[i - 1][k] + matrix[i][j] > maxSum)
                            maxSum = dp[i - 1][k] + matrix[i][j];
                    }
                }
                dp[i][j] = maxSum;
            }
        }
    }

    // Find the maximum sum from the last row of the DP table
    int maxSum = 0;
    for (int j = 0; j < cols; j++) {
        if (dp[rows - 1][j] > maxSum)
            maxSum = dp[rows - 1][j];
    }

    // Free dynamically allocated memory
    for (int i = 0; i < rows; i++) {
        free(dp[i]);
    }
    free(dp);

    return maxSum;
}

int main() {
    FILE* file = fopen("matrix.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Read the matrix from file
    char line[100];
    int rows = 0;
    int cols = 0;
    Node** matrix = NULL;

    while (fgets(line, sizeof(line), file)) {
        // Tokenize the line based on the delimiter ";"
        char* token = strtok(line, ";");
        int cols = 0;

        // Allocate memory for the matrix row
        matrix = (Node**)realloc(matrix, (rows + 1) * sizeof(Node*));
        if (matrix == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        }
        matrix[rows] = NULL;

        while (token != NULL) {
            int num = atoi(token);
            insertNode(&matrix[rows], num);
            token = strtok(NULL, ";");
            cols++;
        }

        rows++;
    }

    fclose(file);

    system("cls");

    int choice;
    do {
        printf("\nMatrix:\n");
        printMatrix(matrix, rows, cols);
        printf("\n\n");

        printf("\nMenu:\n");
        printf("1. Update Matrix\n");
        printf("2. Add Row\n");
        printf("3. Add Column\n");
        printf("4. Remove Row\n");
        printf("5. Remove Column\n");
        printf("6. Highest Sum\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
        {
            int row, col, newValue;
            printf("Enter the position (row column) and the new value: ");
            scanf("%d %d %d", &row, &col, &newValue);
            updateMatrix(matrix, row, col, newValue);
            printf("Value updated successfully.\n"); 
        }
        break;
        case 2:
            addRow(&matrix, &rows, &cols);
            printf("Row added successfully.\n");
            break;
        case 3:
            addColumn(matrix, rows, &cols); // Assuming the matrix has 5 columns
            printf("Column added successfully.\n");
            break;
        case 4:
        {
            int rowToRemove;
            printf("Enter the row number to remove: ");
            scanf("%d", &rowToRemove);
            removeRow(&matrix, &rows, rowToRemove);
            printf("Row removed successfully.\n");
        }
        break;
        case 5:
        {
            int colToRemove;
            printf("Enter the column number to remove: ");
            scanf("%d", &colToRemove);
            removeColumn(matrix, rows, colToRemove);
            printf("Column removed successfully.\n");
        }
        break;
        case 6:
            printf("The highest sum is: %d\n", maxUniqueColumnSum(matrix, rows, cols));
			break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    // Free memory allocated for matrix
    freeMatrix(matrix, rows);

    return 0;
}
