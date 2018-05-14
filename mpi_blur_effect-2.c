
#include <mpi.h>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
//compila con "make"
//corre con "mpirun -np 4 mpi_blur_effect 720p.jpg 15"
using namespace cv;

int main(int argc, char** argv) {
 
  MPI_Init(NULL, NULL);

 
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

 
  printf("Hello world from processor %s, rank %d out of %d processors\n",
         processor_name, world_rank, world_size);
	 IplImage* img = 0; 
  int height,width,step,channels;
  uchar *data;
double  delta;
  int x,xx,y,yy,i,pR,pG,pB,R,G,B,kernel;

  if(argc<3){
    printf("Faltan argumentos: imagen.jgp kernel\n\7");
    exit(0);
  }
  // cargar la imagen
  img=cvLoadImage(argv[1]);
  kernel = strtol(argv[2], NULL, 10);
  if(!img){
    printf("No se pudo cargar el archivo de imagen: %s\n",argv[1]);
    exit(0);
  }

  // obtener los datos de la imagen
  height    = img->height;
  width     = img->width;
  step      = img->widthStep;
  channels  = img->nChannels;
  data      = (uchar *)img->imageData;
int NUM_THREADS = world_size;
int tid = world_rank;
	int lim1= tid*width/NUM_THREADS;
	int lim2= (tid+1)*width/NUM_THREADS;

	for(xx = 0 + lim1; xx < lim2; xx++)
	{
    		for(yy = 0; yy < height; yy++)
		{
        		pB = pG = pR = 0;
        		i = 0;
        		for(x = xx; x < width && x < xx + kernel; x++)
			{
          			 for(y = yy; y < height && y < yy + kernel; y++)
				{
               			 pR += data[x*3 + y*width*3 + 2];
               			 pG += data[x*3 + y*width*3 + 1];
               			 pB += data[x*3 + y*width*3 + 0];
                		i++;
            			}
      		        }
       		        pR = pR/i;
        		pG = pG/i;
       		        pB = pB/i;

        		data[xx*3 + yy*width*3 + 2] = pR;
        		data[xx*3 + yy*width*3 + 1] = pG;
       		        data[xx*3 + yy*width*3 + 0] = pB;
    		}
	}
	

// crear ventana 	
  //cvNamedWindow("Imagen editada", CV_WINDOW_NORMAL); 
  //cvMoveWindow("Imagen editada", 100, 100);
  // mostrar imagen
  //cvShowImage("Imagen editada", img );
  // esperar por llave
  //cvWaitKey(0);
  // release de imagen
  char str[16]="blur-sec-";
  char *str1=argv[1];
  Mat m = cvarrToMat(img);
  imwrite(strcat(str,str1), m);
  cvReleaseImage(&img);
  cvReleaseImage(&img );
  
  MPI_Finalize();
}
