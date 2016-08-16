
#include <stdio.h>


#include "jpeglib.h"


#include <setjmp.h>







extern JSAMPLE * image_buffer;	
extern int image_height;	
extern int image_width;		



GLOBAL(void)
write_JPEG_file (char * filename, int quality)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  
  FILE * outfile;		
  JSAMPROW row_pointer[1];	
  int row_stride;		

  

  cinfo.err = jpeg_std_error(&jerr);
  
  jpeg_create_compress(&cinfo);

  
  

  if ((outfile = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  

  cinfo.image_width = image_width; 	
  cinfo.image_height = image_height;
  cinfo.input_components = 3;		
  cinfo.in_color_space = JCS_RGB; 	
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE );

  

  jpeg_start_compress(&cinfo, TRUE);

  /* Step 5: while (scan lines remain to be written) */
  

  row_stride = image_width * 3;	

  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  

  jpeg_finish_compress(&cinfo);
  
  fclose(outfile);

  

  
  jpeg_destroy_compress(&cinfo);

  
}


/*
 * SOME FINE POINTS:
 *
 * In the above loop, we ignored the return value of jpeg_write_scanlines,
 * which is the number of scanlines actually written.  We could get away
 * with this because we were only relying on the value of cinfo.next_scanline,
 * which will be incremented correctly.  If you maintain additional loop
 * variables then you should be careful to increment them properly.
 * Actually, for output to a stdio stream you needn't worry, because
 * then jpeg_write_scanlines will write all the lines passed (or else exit
 * with a fatal error).  Partial writes can only occur if you use a data
 * destination module that can demand suspension of the compressor.
 * (If you don't know what that's for, you don't need it.)
 *
 * If the compressor requires full-image buffers (for entropy-coding
 * optimization or a multi-scan JPEG file), it will create temporary
 * files for anything that doesn't fit within the maximum-memory setting.
 * (Note that temp files are NOT needed if you use the default parameters.)
 * On some systems you may need to set up a signal handler to ensure that
 * temporary files are deleted if the program is interrupted.  See libjpeg.doc.
 *
 * Scanlines MUST be supplied in top-to-bottom order if you want your JPEG
 * files to be compatible with everyone else's.  If you cannot readily read
 * your data in that order, you'll need an intermediate array to hold the
 * image.  See rdtarga.c or rdbmp.c for examples of handling bottom-to-top
 * source data using the JPEG code's internal virtual-array mechanisms.
 */







struct my_error_mgr {
  struct jpeg_error_mgr pub;	

  jmp_buf setjmp_buffer;	
};

typedef struct my_error_mgr * my_error_ptr;


METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  
  
  (*cinfo->err->output_message) (cinfo);

  
  longjmp(myerr->setjmp_buffer, 1);
}




GLOBAL(int)
read_JPEG_file (char * filename)
{
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  
  FILE * infile;		
  JSAMPARRAY buffer;		
  int row_stride;		


  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return 0;
  }

  

  
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 0;
  }
  
  jpeg_create_decompress(&cinfo);

  

  jpeg_stdio_src(&cinfo, infile);

  

  (void) jpeg_read_header(&cinfo, TRUE);

  


  

  (void) jpeg_start_decompress(&cinfo);

 
  
  row_stride = cinfo.output_width * cinfo.output_components;
  
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  
  

  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    
    put_scanline_someplace(buffer[0], row_stride);
  }

  

  (void) jpeg_finish_decompress(&cinfo);

  

  
  jpeg_destroy_decompress(&cinfo);

  fclose(infile);


  
  return 1;
}

