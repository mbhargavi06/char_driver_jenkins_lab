/* #include <stdio.h>

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/virtual_keyboard"
#define TEST_DATA "keyboard test from user app"

int main() {
    char read_buf[256];
    int fd;

    // Open the device file
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return 1;
    }

    printf("Writing: %s\n", TEST_DATA);
    if (write(fd, TEST_DATA, strlen(TEST_DATA)) < 0) {
        perror("Write failed");
        close(fd);
        return 1;
    }

    // Reset the read buffer
    memset(read_buf, 0, sizeof(read_buf));

    // Read back the data
    if (read(fd, read_buf, sizeof(read_buf)) < 0) {
        perror("Read failed");
        close(fd);
        return 1;
    }

    printf("Read back: %s\n", read_buf);

    // Compare
    if (strcmp(TEST_DATA, read_buf) == 0) {
        printf("✅ Test PASSED!\n");
    } else {
        printf("❌ Test FAILED: Mismatch in data\n");
    }

    close(fd);
    return 0;
}
*/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/virtual_keyboard"
#define MAX_BUF 256

typedef struct {
    const char *test_name;
    const char *input;
    const char *expected;
} TestCase;

TestCase test_cases[] = {
    {"Basic Write/Read", "keyboard test", "keyboard test"},
    {"Empty String", "", ""},
    {"Max Length", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", NULL}, // expected will be input
    {"Special Characters", "!@#$%^&*()_+-=[]{}|;':,./<>?", "!@#$%^&*()_+-=[]{}|;':,./<>?"},
    {"Newlines and Tabs", "Line1\nLine2\tTabbed", "Line1\nLine2\tTabbed"},
};

void run_test(TestCase test) {
    char read_buf[MAX_BUF];
    int fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Error opening device");
        return;
    }

    write(fd, test.input, strlen(test.input));

    memset(read_buf, 0, sizeof(read_buf));
    read(fd, read_buf, sizeof(read_buf));

    const char *expected = test.expected ? test.expected : test.input;

    if (strcmp(read_buf, expected) == 0) {
        printf("✅ %s: PASSED\n", test.test_name);
    } else {
        printf("❌ %s: FAILED\n", test.test_name);
        printf("   Expected: %s\n", expected);
        printf("   Got     : %s\n", read_buf);
    }

    close(fd);
}

int main() {
    int num_tests = sizeof(test_cases) / sizeof(TestCase);
    printf("Running %d tests...\n\n", num_tests);

    for (int i = 0; i < num_tests; i++) {
        run_test(test_cases[i]);
    }

    return 0;
}
