


reduce(){};
void getInfo(int fd, rio_t client, int id){
	char buf1[MAXLINE];
	int jobID=id;
	int numBytes;
  	char * saveptr=NULL;
	Rio_readinitb( &client,fd);
	while(true){
  		numBytes = Rio_readlineb(&client, buf1, MAXLINE);
		id1 = strtok_r(buf1, " \r\n",&saveptr);
		int fileStartID=atoi(id1);
		id2 = strtok_r(buf1, " \r\n",&saveptr);
		int fileEndID=atoi(id2);

	numBytes = Rio_readlineb(c, buf, MAXLINE);
  	if(numBytes<0){
    		Close(fd);
    		return ans;
  	}
  	buf[numBytes]=0;
  	seperate(buf, &ans.pre);


	fd=Open_clientfd(ipAddress, atoi(port));

}
