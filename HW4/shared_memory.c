
/Referenced by Sandeep Raj*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdbool.h>

//Structure of the data which is communicated between the parent and the child using pipes.
typedef struct datastructure
{
  bool led;
  char message[16];
} datastruct;

int main(void)
{
  int shm;
  sem_t *sem;
  pid_t cpid;
  void *map_addr;
  datastruct data;

  printf("Forking a child.\n");

  bzero(&data, sizeof(datastruct));
  shm_unlink("ipc_shmem");
  sem_unlink("ipc_shsem");



  switch (cpid = fork())
  {
    case -1: /* fork() failed */
      perror("fork");
      break;

    case 0: /* Child of successful fork() comes here */
      printf("Spawned child.\n");
      sem = sem_open("ipc_shsem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 0);

      shm = shm_open("ipc_shmem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);


      if(shm == -1)
        perror("client side creation of shared memory descriptor");

      if(ftruncate(shm, sizeof(datastruct)) == -1)
        perror("fruncate");

      map_addr = mmap(NULL, sizeof(datastruct), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
      if (map_addr == MAP_FAILED)
        perror("mmap");

      printf(" Created shared descriptor, set its size to %lu bytes and virtual memory mapped it to the parent process.\n", sizeof(datastruct));

      sem_wait(sem);
      memcpy((void *) &data, map_addr, sizeof(datastruct));           /* Copy shared memory to data*/

      printf(" Received message: \"%s\". Received LED State: %s.\n", data.message, data.led ? "true" : "false");

      strcat(data.message, " World");
      data.led = !data.led;
      printf(" Sending modified message: \"%s\". Modified Received LED State: %s.\n", data.message, data.led ? "true" : "false");

      memcpy(map_addr, (void *) &data, sizeof(datastruct));           /* Copy data to shared memory */
      sem_post(sem);

      close(shm);
      sem_close(sem);
      sem_unlink("ipc_shsem");
      printf(" Communication successful. Unlinked the ipc_shsem.\n");

      break;

    default: /* Parent comes here after successful fork() */
      sem = sem_open("ipc_shsem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 0);


      shm = shm_open("ipc_shmem", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);


      if(shm == -1)
        perror("parent side creation of shared memory descriptor");

      if(ftruncate(shm, sizeof(datastruct)) == -1)
        perror("fruncate");

      map_addr = mmap(NULL, sizeof(datastruct), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
      if (map_addr == MAP_FAILED)
        perror("mmap");

      printf(" Created shared descriptor, set its size to %lu bytes and virtual memory mapped it to the parent process.\n", sizeof(datastruct));

      strcpy(data.message, "Hello");
      data.led = false;


      printf(" Updating message in the shared memory: \"%s\". LED State: %s.\n", data.message, data.led ? "true" : "false");

      memcpy(map_addr, (void *) &data, sizeof(datastruct));           /* Copy data to shared memory */
      sem_post(sem);
      usleep(500);

      bzero(&data, sizeof(datastruct));

      sem_wait(sem);
      memcpy((void *) &data, map_addr, sizeof(datastruct));           /* Copy shared memory to data*/

      printf(" Received message: \"%s\". Received LED State: %s.\n", data.message, data.led ? "true" : "false");

      close(shm);
      sem_close(sem);
      sem_unlink("ipc_shsem");
      shm_unlink("ipc_shmem");
      printf(" Communication successful. Closed and unlinked shared memory and ipc_shsems.\n");

      break;
  }
 exit (0);
}
