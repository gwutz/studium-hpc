#include "hpc-matrix.h"


#define HPC_MATRIX_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), HPC_TYPE_MATRIX, HpcMatrixPrivate))

struct _HpcMatrixPrivate
{
	int cols;
	int rows;
	double **matrix;
};

G_DEFINE_TYPE (HpcMatrix, hpc_matrix, G_TYPE_OBJECT)

static void
hpc_matrix_finalize (GObject *object)
{
	G_OBJECT_CLASS (hpc_matrix_parent_class)->finalize (object);
}

static void
hpc_matrix_class_init (HpcMatrixClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = hpc_matrix_finalize;

	g_type_class_add_private (object_class, sizeof (HpcMatrixPrivate));
}
static void
_hpc_matrix_empty_array(HpcMatrix* self)
{
	HpcMatrixPrivate* priv = self->priv;
	int i, j;
	
	for( i = 0; i < priv->cols; ++i)
	{
		for( j = 0; j < priv->rows; ++j)
		{
			priv->matrix[i][j] = 0;
		}
	}
}

static void
_hpc_matrix_init_array(HpcMatrix* self)
{
	int i;
	HpcMatrixPrivate* priv = self->priv;
	
	priv->matrix = g_malloc( sizeof(double*) * priv->rows );
	for (i = 0; i < priv->rows; ++i) {
		priv->matrix[i] = g_malloc(sizeof(double) * priv->cols );
	}
	
	_hpc_matrix_empty_array(self);
}

static void
hpc_matrix_init (HpcMatrix *self)
{
	self->priv = HPC_MATRIX_GET_PRIVATE (self);
}

HpcMatrix *
hpc_matrix_new (int cols, int rows)
{
	HpcMatrix* obj = g_object_new (HPC_TYPE_MATRIX, NULL);
	obj->priv->cols = cols;
	obj->priv->rows = rows;
	_hpc_matrix_init_array(obj);
	return obj;
}

void
hpc_matrix_random(HpcMatrix *self)
{
	int i, j;
	HpcMatrixPrivate *priv = self->priv;
	GRand *rand = g_rand_new_with_seed(1);
	
	for (i = 0; i < priv->cols; ++i) {
		for (j = 0; j < priv->rows; ++j){
			priv->matrix[i][j] = g_rand_double(rand);
		}
	}
}

void
hpc_matrix_print(HpcMatrix *self)
{
	int i, j;
	HpcMatrixPrivate *priv = self->priv;
	
	for ( i=0; i < priv->cols; ++i) {
		for ( j=0; j < priv->rows; ++j) {
			g_print("%.3f ", priv->matrix[i][j]);
		}
		g_print("\n");
	}
}

double
hpc_matrix_get(HpcMatrix *self, int x, int y)
{
	g_return_if_fail( x < self->priv->cols );
	g_return_if_fail( y < self->priv->rows );
	
	HpcMatrixPrivate *priv = self->priv;
	
	return priv->matrix[x][y];
}

void
hpc_matrix_set(HpcMatrix *self, int x, int y, double v)
{
	g_return_if_fail( x < self->priv->cols );
	g_return_if_fail( y < self->priv->rows );
	
	HpcMatrixPrivate *priv = self->priv;
	
	priv->matrix[x][y] = v;
}

void
hpc_matrix_process(HpcMatrix *self, double (*func)(double v))
{
	g_return_if_fail( func != NULL );
	
	int i, j;
	HpcMatrixPrivate *priv = self->priv;
	
	for (i = 0; i < priv->cols; ++i)
	{
		for (j = 0; j < priv->rows; ++j)
		{
			priv->matrix[i][j] = func(priv->matrix[i][j]);
		}
	}
}

struct ThreadData {
	HpcMatrix *self;
	double (*func) (double);
};

void
thread_func(gpointer data)
{
	struct ThreadData *str_data = (struct ThreadData*)data;
	HpcMatrixPrivate *priv = str_data->self->priv;
	
	int collim = priv->cols/2;
	int i, j;
	
	for ( i = 0; i < collim; ++i ) 
	{
		for ( j = 0; j < priv->rows; ++j)
		{
			priv->matrix[i][j] = str_data->func(priv->matrix[i][j]);
		}
	}
}

void
hpc_matrix_process_parallel(HpcMatrix *self, double (*func)(double v))
{
	g_return_if_fail( func != NULL );
	
	int collim = self->priv->cols / 2;
	
	// Thread
	struct ThreadData data;
	data.self = self;
	data.func = func;
	
	GThread *t = g_thread_new("worker", thread_func, &data);
	
	int i, j;
	HpcMatrixPrivate *priv = self->priv;
	
	for (i = collim; i < priv->cols; ++i)
	{
		for (j = 0; j < priv->rows; ++j)
		{
			priv->matrix[i][j] = func(priv->matrix[i][j]);
		}
	}
	
	g_thread_join(t);
		
}
