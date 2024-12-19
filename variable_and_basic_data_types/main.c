#include <stdio.h>

// Define an enumeration for device states
enum DeviceState { OFF, ON, SUSPENDED };

int main() {
  // Primitive data types with type modifiers
  signed char sc = -50;
  unsigned char uc = 200;
  short int si = -30000;
  unsigned short int usi = 60000;
  int i = -2000000000;
  unsigned int ui = 4000000000U;
  long int li = -9000000000L;
  unsigned long int uli = 9000000000UL;
  long long int lli = -9000000000000000000LL;
  unsigned long long int ulli = 18000000000000000000ULL;
  float f = 3.14f;
  double d = 2.718281828459;
  long double ld = 1.618033988749895L;

  // Enumeration
  enum DeviceState state = SUSPENDED;

  // Printing sizes
  printf("Sizes of Various Types:\n");
  printf("signed char: %zu bytes\n", sizeof(sc));
  printf("unsigned char: %zu bytes\n", sizeof(uc));
  printf("short int: %zu bytes\n", sizeof(si));
  printf("unsigned short int: %zu bytes\n", sizeof(usi));
  printf("int: %zu bytes\n", sizeof(i));
  printf("unsigned int: %zu bytes\n", sizeof(ui));
  printf("long int: %zu bytes\n", sizeof(li));
  printf("unsigned long int: %zu bytes\n", sizeof(uli));
  printf("long long int: %zu bytes\n", sizeof(lli));
  printf("unsigned long long int: %zu bytes\n", sizeof(ulli));
  printf("float: %zu bytes\n", sizeof(f));
  printf("double: %zu bytes\n", sizeof(d));
  printf("long double: %zu bytes\n\n", sizeof(ld));

  // Printing values with format specifiers
  printf("Values of Various Types:\n");
  printf("signed char sc = %d\n", sc);
  printf("unsigned char uc = %u\n", uc);
  printf("short int si = %d\n", si);
  printf("unsigned short int usi = %u\n", usi);
  printf("int i = %d\n", i);
  printf("unsigned int ui = %u\n", ui);
  printf("long int li = %ld\n", li);
  printf("unsigned long int uli = %lu\n", uli);
  printf("long long int lli = %lld\n", lli);
  printf("unsigned long long int ulli = %llu\n", ulli);
  printf("float f = %.2f\n", f);
  printf("double d = %.12lf\n", d);
  printf("long double ld = %.15Lf\n\n", ld);

  // Printing enumeration
  printf("Enumeration Data Type:\n");
  printf("Device State (numeric): %d\n", state);

  // Making enum readable
  printf("Device State (readable): ");
  switch (state) {
  case OFF:
    printf("OFF\n");
    break;
  case ON:
    printf("ON\n");
    break;
  case SUSPENDED:
    printf("SUSPENDED\n");
    break;
  default:
    printf("UNKNOWN\n");
  }

  return 0;
}
