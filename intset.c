#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define initBUFFERSIZE 2

//forward declaration
struct Set;
int duplicates(struct Set *set, int num);
void sortSet(struct Set *set);
void removeItem(struct Set *set, int num);
void createSet(struct Set *set);
void add(struct Set *set, int val);
void printSet(struct Set *set);
void mainloop();

//Create the set struct
struct Set {
  int *arr;
  size_t size, cap;
};

// Lecture code
void add(struct Set *set, int val) {
  //check if duplicate, returns true, don't run function.
  int flag = duplicates(set, val);
  if (!flag) {
    if (set->size == set->cap) {
      int *newArr = malloc(set->cap*sizeof(int)*2);
      for (size_t i = 0; i < set->size; ++i) newArr[i] = set->arr[i];
      free(set->arr);
      set->arr = newArr;
      set->cap = set->cap*2;
    }
    set->arr[(set->size)++] = val;
  }
  sortSet(set);
}

// Sorting function
void bubbleSort(int *arr, int len) {
  for (int i = 0; i < len -1; i++) {
    for (int j = 0; j < len - i - 1; j++) {
      if (arr[j] > arr[j+1]) {
        int temp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = temp;
      }
    }
  }
}

// Uses calls bubble sort, just makes things easier to read ig
void sortSet(struct Set *set) {
  bubbleSort(set->arr, set->size);
}

//Write function to check for duplicate numbers
int duplicates(struct Set *set, int num) {
  for (int i = 0; i < set-> size; i++) {
    if (set->arr[i] == num) {
      return 1;
    }
  }
  return 0;
}

void removeItem(struct Set *set, int num) {
  int inArr = 0;
  int index;
  for (int i = 0; i < set->size; i++) {
    if (set->arr[i] == num) {
      index = i;
      inArr = 1;
    }
  }
  if (inArr) {
    for (int i = index; i < set->size - 1; i++) {
      set->arr[i] = set->arr[i+1];
    }
    --set->size;
  }
}

void findUnion(struct Set *x, struct Set *y) {
  struct Set u;
  createSet(&u);
  for (int i = 0; i < x->size; i++) {
    if (!duplicates(&u, x->arr[i])) {
      add(&u, x->arr[i]);
    }
  }
  for (int j = 0; j < y->size; j++) {
    if (!duplicates(&u, y->arr[j])) {
      add(&u, y->arr[j]);
    }
  }
  printSet(&u);
  free(u.arr);
}

void intersection(struct Set *x, struct Set *y) {
  struct Set i;
  createSet(&i);
  for (int k = 0; k < y->size; k++) {
    if (duplicates(x, y->arr[k])) {
      add(&i, y->arr[k]);
    }
  }
  if (i.size != 0) {
    printSet(&i);
  }
  free(i.arr);
}

// Prints the set to terminal
void printSet(struct Set *set) {
  if (set->size != 0) {
    for (int i = 0; i < set->size; i++) {
      printf("%d", set->arr[i]);
      if (i != set->size-1) {
        printf(" ");
      }
  }
  printf("\n");
  }
}

//Read the input
// Returns the input as a string, you will then have to use strtok to read each line of the input.
char* readInput() {
  char *buffer;
  size_t bufferSize = 12;
  size_t characters;
  buffer = (char *)malloc(bufferSize * sizeof(char));
  characters = getline(&buffer,&bufferSize,stdin);
  if (characters == -1) {
    printf("Getline didn't work");
  }
  return buffer;
}

// poorly worded, but reads the input and determines which operation to use.
// loop this function until the input is q
void mainloop() {
  struct Set x;
  struct Set y;
  createSet(&x);
  createSet(&y);
  int finished = 0;
  int opFlag = 0;
  int setFlag = 0;
  int numFlag = 0;
  int flag = 0;
  while (!finished) {
    char *inputs = readInput();
    const char s[3] = " \t";
    char *tokens = strtok(inputs, s);
    //prints out the inputs, for testing reasons.
    char operation;
    char setToUse;
    int num;

    if (flag) {
      operation = ' ';
      num = 0;
      setToUse = ' ';
      flag = 0; //reset flag
      //reset other flags
      opFlag = 0;
      setFlag = 0;
      numFlag = 0;
    }

    while (tokens != NULL) { //loop through the inputs
      //printf("[%s]", tokens);
      //commands[index++] = strdup(tokens);
      char *ch = tokens;
      char n = *ch;
      //printf("Token: %s\n", tokens);
      if ((n >= '0' && n <= '9') || ( n == '-')) {
        num = atoi(ch);
        numFlag = 1;
        //printf("num: %d\n", num);
      } else {
        switch(*tokens) {
            case 'a':
            case 'r':
            case 'u':
            case 'i':
            case 'p':
            case 'q':
              operation = *tokens;
              opFlag = 1;
              break;
            case 'x':
            case 'y':
              setToUse = *tokens;
              setFlag = 1;
              break;
        }
      }
      tokens = strtok(NULL, s);
    } // End read inputs
    //printf("op: %c  set: %c num: %d\n", operation, setToUse, num);
    //Check the command
    switch (operation) {
      case 'a':
        if (opFlag && setFlag && numFlag) {
          if (setToUse == 'x') {
            add(&x, num);
          } else if (setToUse == 'y') {
            add(&y, num);
          }
          flag = 1;
        }
        //printf("add\n"); 
        break;
      case 'r':
        //printf("remove\n");
        if (opFlag && setFlag && numFlag) {
          if (setToUse == 'x') {
            removeItem(&x, num);
          } else if (setToUse == 'y') {
            removeItem(&y, num);
          }
          flag = 1;
        }
        break;
      case 'u':
        //printf("union\n");
        findUnion(&x, &y);
        flag = 1;
        break;
      case 'i':            
        //printf("intersection\n");
        intersection(&x, &y);
        flag = 1;
        break;
      case 'p':
        //printf("print set\n");
        if (opFlag && setFlag) {
            if (setToUse == 'x') {
            printSet(&x);
          } else if (setToUse == 'y') {
            printSet(&y);
          }
          flag = 1;
        }
        break;
    } // end of inner while loop
    if (operation == 'q') {
      flag = 1;
      break;
    }
    free(inputs);
  } // End Program
  free(x.arr);
  free(y.arr);
}

// Function to automate making sets
void createSet(struct Set *set) {
  set->size = 0;
  set->cap = 4;
  set->arr = (int*)malloc(set->cap * sizeof(int));
}

int main(int argc, const char **argv) {
  mainloop();
  return 0;
}