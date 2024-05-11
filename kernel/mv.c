#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fileMove( char *srcPath, char *dstPath )
{
    int     retVal;
    int     len = 0;
    char    szTemp[512];
    char    buff[4096];
    FILE    *in_fp, *out_fp;
    size_t  n_size;     


    in_fp  = fopen( srcPath, "r" );
    if ( in_fp == NULL )
    {
        printf( "fopen()...failed, srcPath=(%s) not found\n", srcPath );
        return -1;
    }

   

    //타겟파일 쓰기모드로 오픈

    out_fp = fopen( dstPath, "w" );
    if ( out_fp == NULL )
    {
        printf( "fopen()...failed, dstPath=(%s) not found\n", dstPath );

        fclose(in_fp);  //이미 오픈된 srcPath에 대한 fp close 
        return -1;
    }

      

    //원본파일을 타켓파일로 복사

    while( 0 < (n_size = fread( buff, 1, sizeof(buff), in_fp)) )
    {
        fwrite( buff, 1, n_size, out_fp );
    }

 

    //파일복사 완료후 fp close

    fclose( in_fp );
    fclose( out_fp );

 

    len += sprintf( szTemp, "File Move Success.\n" );
    len += sprintf( &szTemp[len], " - srcPath    = (%s)\n", srcPath );
    len += sprintf( &szTemp[len], " - dstPath    = (%s)\n", dstPath );
   printf( "%s", szTemp );

 

    //복사한 원본파일 제거

    retVal = unlink( srcPath );
    if ( retVal == -1 )
    {
        printf( "unlink()...failed, retVal=%d\n", retVal );
        return -1;
    }

 

    return 0;

} 


