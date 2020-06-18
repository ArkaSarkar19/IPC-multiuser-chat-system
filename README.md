# IPC-multiuser-chat-system
* Multi-user Chat System. The code is in C. 

### IMPLEMENTATION 
* The system consists of **server.c** and **client.c**. IPC sockets were used. </br>
* Server treats every client differently creates a **thread** for each client so one or more clients can request the server at the same time. </br>
* Server is constantly listening for new connections via the **accept()** system call. If a client requests to send a message the server uses the **recv()** system call to get the message and then uses **send()** system call to send it to the required client. </br>
* Server keeps tracks of all the threads of the clients and once the server is terminated all the threads are wiped out one by one. </br>
 * Client has two different threads one for receiving and one for sending. The receiving thread is running an infinite while loop which uses **recv()** system call connected to the **server socket**. The other thread takes input from the client to send it to other clients via the server. </br>
 
### COMPILATION 
* To compile it type the **make** command on the terminal. The executables server and client are created and have to be run individually. </br>
* Server has to be executed before the clients.

        Commands :
        $ make
        $ ./server
        $./client

### USER INPUTS
* If a User wants to send a message, has to first type the client socket to send to and then the message in the following line.
* Example :

          Available clients : |4 |5
          Enter the client to send: 5 
          Enter the message: hi
          
          Available clients : |4 |5
          Enter the client to send: EVERYONE
          Enter the message: hi
          
          ‘exit' command exits the client terminal
          
          (EVERYONE command sends message to all users connected to that server)
          
### EXPECTED OUTPUT
* If a clients sends you a message it flashes on the terminal as :

        Connected to Server
        NEW MESSAGE : 5 says hi
        
### ERROR HANDLING
* If a user enters a invalid socket number or a string its throws an error.
* Ctrl C doesn’t terminate instantly but clears all the pending threads.

### CITATIONS
* ![http://beej.us/guide/bgipc/pdf/bgipc_USLetter.pdf](http://beej.us/guide/bgipc/pdf/bgipc_USLetter.pdf)
