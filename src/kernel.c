void infloop();

void kernel_entry() {
  *((short int*) 0xB8000) = 0;
  infloop();
}