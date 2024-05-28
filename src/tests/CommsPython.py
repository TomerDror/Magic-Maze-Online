from multiprocessing import shared_memory

# Create a shared memory block
shm = shared_memory.SharedMemory(create=True, size=1024, name='my_shared_memory')

# Access the shared memory as a byte array
buffer = shm.buf
buffer[:4] = bytearray([1, 2, 3, 4])  # Example data

input("Press Enter to continue and release shared memory...")

# Clean up
shm.close()
shm.unlink()
