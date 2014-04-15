In case you want to test it :

our program has several part. 
filecenter ip port creates a global file system.
./testJ 1  filesyetem IP , filesystem port, master IP, master port
creates master on a certain machine.(use the correct IP for master IP, port can be assigned as wish)

./testJ 0  filesyetem IP , filesystem port, workerIP, worker port, master IP, master port

creates a worker on this machine. (use the machine IP as workerIP)

the file cv has a ./main to create random pictures for test. Put the pictures generated in ./image/ . We already have 5000 pictures already.

the file sigle is a program that do all the work locally. It is used to make comparison to our distributede solution.

Thank you!!
