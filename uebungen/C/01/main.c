
#include "hpc-matrix.h"
#include <glib-object.h>
#include <math.h>


double
identity( double v )
{
	int i;
	double elem = v;
	for ( i = 0; i < 1000; ++i)
	{
		elem = exp(log(elem));
	}
//	g_print("%f\n", elem);
	return elem;
}

int
main (int argc, char *argv[])
{
	g_type_init();
	
	GTimer *timer = g_timer_new();
	
	HpcMatrix* matrix = hpc_matrix_new(100,100);
	hpc_matrix_random(matrix);
	g_timer_start(timer);
//	hpc_matrix_process(matrix, identity);
	hpc_matrix_process_parallel(matrix, identity);
	g_timer_stop(timer);
	
	g_print("time: %f\n", g_timer_elapsed(timer, NULL));
	
	return 0;
}
