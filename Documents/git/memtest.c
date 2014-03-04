#include "ackerman.h"
#include "my_allocator.h"

int main(int argc, char ** argv) 
{

   //input parameters (100, 6000)

   init_allocator(32, 60000000);

   ackerman_main();

   release_allocator();
}
