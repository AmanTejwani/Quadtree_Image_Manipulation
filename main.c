#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "qt.h"

int main(int argc , char *argv[]){

    int Height , Width , max_no_of_colour , i ,j ,threshold ;
    
    //   for compression  use  ---- ./quadtree -c threshold < file_name .ppm format > < file output .out format >
    
    if(strcmp(argv[1],"-c")==0){
        
        colour_pixel **mat;

        FILE *fp=fopen(argv[3],"rb");

        char mag_id[3];             //stores the magic PPM identifier 
        
        threshold=atoi(argv[2]);    //   atoi converts string to  int
        
        fscanf(fp,"%2s",mag_id);
        mag_id[2]='\0';

        fscanf(fp,"%d",&Height);
        fscanf(fp,"%d",&Width);
        fscanf(fp,"%d",&max_no_of_colour);
        
        char c;
        fscanf(fp,"%c",&c);
		
        //   creating matrix of size  H x W of type colour_pixel

        mat=malloc(Height*sizeof(colour_pixel *));

        for(i=0;i<Height;i++){
            mat[i]=malloc(Width*sizeof(colour_pixel));
        }
        
        //  blank matrix is created.......   Now we need to fill the matrix from ppm file.

        for(i=0;i<Height;i++){
            for(j=0;j<Width;j++){
                fread(&mat[i][j].R , sizeof(unsigned char),1,fp);
                fread(&mat[i][j].G , sizeof(unsigned char),1,fp);
                fread(&mat[i][j].B , sizeof(unsigned char),1,fp);
            }
        }
        fclose(fp);


        // int count=0;
        // for (int i = 0; i < Height; i++)
        // {
        //     for (int j = 0; j < Width; j++)
        //         count++;          
        // }
        // printf("Height = %d  Width = %d   , Height X Width  %d \n", Height, Width , Height*Width);
        // printf("%d ",count);



        //   converting the matrix data into  quadtree and compressing it 
        
        Quad_Node *root=NULL;
        int no_of_colour=0, no_of_nodes=0 , depth=0;

        Compression(&root,0,0,Width,Width,mat,threshold,&no_of_colour,&no_of_nodes);
        
        printf("no_of_colour -- %d  ----- no_of_nodes -- %d ", no_of_colour,no_of_nodes);
        // no_of_colour -- 12632904  ----- no_of_nodes -- 101672

        depth=Depth(root);
        // printf(" \n \n Depth  %d ",depth);




        int x,y,l;
 		x = y =0;
		l = Width;

        //  converting our quadtree to our matirx

        create_matrix(root,mat,x,y,l,&no_of_colour ,no_of_nodes);
        create_image(mat,Width,Width,max_no_of_colour,argv[4]);
        del_Matrix(mat,Height);
        del_tree(root);
    }
    
    if(strcmp(argv[1],"-m") == 0){
        colour_pixel **mat;
        int mirror;
        if(strcmp(argv[2],"h")==0)
            mirror=1;
        else if(strcmp(argv[2],"v")==0)
            mirror=2;
        else
            mirror=0;
        
        threshold=atoi(argv[3]);

        FILE *fp=fopen(argv[4],"rb"); 

        char mag_id[3];
		fscanf(fp,"%2s",mag_id);
		mag_id[2]='\0';
		fscanf(fp, "%d", &Height);
    	fscanf(fp, "%d", &Width);
    	fscanf(fp, "%d", &max_no_of_colour);
    	char c;
    	fscanf(fp,"%c",&c);  

    	mat = malloc(Height * sizeof(colour_pixel*));
		for(i = 0; i < Height; i++)
			mat[i] = malloc(Width*sizeof(colour_pixel));

		for(i = 0; i < Height; i++){
			for(j = 0; j < Width; j++)
				fread(&mat[i][j], sizeof(colour_pixel), 1, fp);
		}
		fclose(fp); 

        Quad_Node *root=NULL;
        int no_of_colour=0, no_of_nodes=0 , depth=0;
		int x,y,l;
        Compression(&root,0,0,Width,Width,mat,threshold,&no_of_colour,&no_of_nodes);
        // printf("no_of_colour -- %d  ----- no_of_nodes -- %d ", no_of_colour,no_of_nodes);
        depth=Depth(root);
        if (mirror==1)
        {
            Horizontal_flip(&root);
        }
        else if (mirror==2)
        {
            Vertical_flip(&root);
        }

		x = y =0;
		l = Width; 

        create_matrix(root,mat,x,y,l,&no_of_colour ,no_of_nodes);
        create_image(mat,Width,Width,max_no_of_colour,argv[5]);
        del_Matrix(mat,Height);
        del_tree(root);
        
    }

    if(strcmp(argv[1],"-o")==0){
        
        colour_pixel **mat1 , **mat2;
        int Width1 , Height1 ;
        int Width2 , Height2 ;

        threshold =atoi(argv[2]);

        //   reading file1
        FILE *f1=fopen(argv[3],"rb");
        char mag_id1[3];
		fscanf(f1,"%2s",mag_id1);
		mag_id1[2]='\0';
		fscanf(f1, "%d", &Height1);
    	fscanf(f1, "%d", &Width1);
    	fscanf(f1, "%d", &max_no_of_colour);
    	char c;
    	fscanf(f1,"%c",&c);  

    	mat1 = malloc(Height1 * sizeof(colour_pixel*));
		for(i = 0; i < Height1; i++)
			mat1[i] = malloc(Width1*sizeof(colour_pixel));

		for(i = 0; i < Height1; i++){
			for(j = 0; j < Width1; j++)
				fread(&mat1[i][j], sizeof(colour_pixel), 1, f1);
		}
		fclose(f1); 

        //   reading file2
        FILE *f2=fopen(argv[4],"rb");      
        char mag_id2[3];
		fscanf(f2,"%2s",mag_id2);
		mag_id2[2]='\0';
		fscanf(f2, "%d", &Height2);
    	fscanf(f2, "%d", &Width2);
    	fscanf(f2, "%d", &max_no_of_colour);
    	fscanf(f1,"%c",&c);  

    	mat2 = malloc(Height2 * sizeof(colour_pixel*));
		for(i = 0; i < Height2; i++)
			mat2[i] = malloc(Width2*sizeof(colour_pixel));

		for(i = 0; i < Height2; i++){
			for(j = 0; j < Width2; j++)
				fread(&mat2[i][j], sizeof(colour_pixel), 1, f2);
		}
		fclose(f2); 
        // printf("H1 %d  W1 %d H2 %d W2 %d",Height1,Width1,Height2,Width2);

        Quad_Node *root1=NULL;
        Quad_Node *root2=NULL;

        int no_of_colour1=0, no_of_nodes1=0 , depth1=0;
        int no_of_colour2=0, no_of_nodes2=0 , depth2=0;
		int x,y,l;
        x=0 , y=0 , l=Width1;
        Compression(&root1 ,0 ,0 , Width1 , Width1 , mat1 , threshold ,&no_of_colour1 , &no_of_nodes1 );
        Compression(&root2 ,0 ,0 , Width2 , Width2 , mat2 , threshold ,&no_of_colour2 , &no_of_nodes2 );
    
        depth1=Depth(root1);
        depth2=Depth(root2);
        printf("no_of_colour1 %d  no_of_nodes1 %d depth1 %d ",no_of_colour1,no_of_nodes1,depth1);
        printf("no_of_colour2 %d  no_of_nodes2 %d depth2 %d ",no_of_colour2,no_of_nodes2,depth2);
        int max_depth;
        
        if(depth1 > depth2)
            max_depth=depth1;
        else
            max_depth=depth2;
        printf("%d ",max_depth);
        int k = 1;

        complete_tree(&root1,Width1,max_depth,1);
        complete_tree(&root2,Width2,max_depth,1);

        depth1 = Depth(root1);
		depth2 = Depth(root2);

        Overlap_imgs(root1, root2, mat1 , x, y, l);
        create_image(mat1,Width1,Width1,max_no_of_colour,argv[5]);

        del_Matrix(mat1, Width1);
        del_tree(root1);
        del_Matrix(mat2, Width2);
        del_tree(root2);
    }

    if(strcmp(argv[1],"-a") == 0){
		
		int colour;

		if(strcmp(argv[2],"red") == 0)
			colour = 1; 

		else if(strcmp(argv[2],"green") == 0)
			colour = 2; 

		else if(strcmp(argv[2],"blue")==0)
			colour = 3;  

		else if(strcmp(argv[2],"grey")==0)
			colour = 4;  

		else if(strcmp(argv[2],"black_white")==0)
			colour = 5;  

		else
			colour = 0;	

        Quad_Node *root1=NULL;
        colour_pixel **mat1;
        int Height1 , Width1;
        threshold=atoi(argv[3]);

        FILE *fp=fopen(argv[4],"rb");
        char mag_id[3];
        fscanf(fp, "%2s", mag_id);
		mag_id[2]= '\0';
		fscanf(fp, "%d", &Height1);
		fscanf(fp, "%d", &Width1);
		fscanf(fp, "%d", &max_no_of_colour);
		char c;
		fscanf(fp,"%c",&c);

		mat1 = malloc(Height1 * sizeof(colour_pixel*));
		for(i = 0; i < Height1; i++)
			mat1[i] = malloc(Width1 * sizeof(colour_pixel));

		for(i = 0; i < Height1; i++){
			for(j = 0; j < Width1; j++)
				fread(&mat1[i][j], sizeof(colour_pixel), 1, fp);
		}
        fclose(fp);

        int no_of_colour=0, no_of_nodes=0 , depth=0;
        int x=0 , y=0 , L;
        L=Width1;
        Compression(&root1,0,0,Width1,Width1,mat1,threshold,&no_of_colour,&no_of_nodes);
        depth=Depth(root1);
        
        if(colour==1){
            red(root1,mat1,x,y,L);
        }

        else if(colour==2){
            green(root1,mat1,x,y,L);
        }

        else if(colour==3){
            blue(root1,mat1,x,y,L);
        }

        else if(colour==4){
            grey(root1,mat1,x,y,L);
        }

        else if(colour==5){
            black_white(root1,mat1,x,y,L);
        }
        else{
            printf("Your selected chice is wrong");
            return 0;
        }

		create_image(mat1, Width1, Width1, max_no_of_colour, argv[5]);
		del_Matrix(mat1, Width1);
		del_tree(root1);
    }

    return 0;    
}

