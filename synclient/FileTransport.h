struct datapack
{
  int size;
  char data[BUFFSIZE];
  datapack()
  {
    memset(data,'\0',BUFFSIZE);
  }
};

int UploadFile(const char * filepath,int sockConn)
{
  printf("tranporting file%s\n",filepath);

  int filefd=open(filepath,O_RDONLY);
  if(filefd<0)
  {
    printf("file open error!\n");
    return -1;
  }
  char alibaba[JSONSIZE]={'\0'};
  while(1)
  {
    struct datapack *Buf=new datapack();
    int len = read(filefd,Buf->data,BUFFSIZE);
    Buf->size=len;
    send(sockConn,Buf,JSONSIZE,0);
    recv(sockConn,alibaba,JSONSIZE,0);
    if(len < 1)
    {
      break;
    }
    delete Buf;
  }
  return 0;
}
int DownloadFile(const char *filepath,int clientfd)
{
 
  printf("downloading file %s......\n",filepath);

  

  char *recvBuf[JSONSIZE]={'\0'};
  int filefd;
  long long totallength = 0;
  filefd = open(filepath,O_RDWR|O_CREAT,0777);
  while(1)
  {
    recv(clientfd,recvBuf,JSONSIZE,0);
    struct datapack *Buf=(struct datapack *)recvBuf;

    int len=Buf->size;
    send(clientfd,"acknowledge",JSONSIZE,0);
    if(len==0)
    {
      break;
    }
    write(filefd ,Buf->data ,len);
    totallength+=len;
  }
  int num=lseek(filefd,0,SEEK_END);
 ftruncate(filefd,totallength);
}