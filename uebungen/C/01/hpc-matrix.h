#ifndef __HPC_MATRIX_H__
#define __HPC_MATRIX_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define HPC_TYPE_MATRIX			(hpc_matrix_get_type ())
#define HPC_MATRIX(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), HPC_TYPE_MATRIX, HpcMatrix))
#define HPC_MATRIX_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), HPC_TYPE_MATRIX, HpcMatrix const))
#define HPC_MATRIX_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), HPC_TYPE_MATRIX, HpcMatrixClass))
#define HPC_IS_MATRIX(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), HPC_TYPE_MATRIX))
#define HPC_IS_MATRIX_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), HPC_TYPE_MATRIX))
#define HPC_MATRIX_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), HPC_TYPE_MATRIX, HpcMatrixClass))

typedef struct _HpcMatrix		HpcMatrix;
typedef struct _HpcMatrixClass		HpcMatrixClass;
typedef struct _HpcMatrixPrivate	HpcMatrixPrivate;

struct _HpcMatrix
{
	GObject parent;

	HpcMatrixPrivate *priv;
};

struct _HpcMatrixClass
{
	GObjectClass parent_class;
};

GType hpc_matrix_get_type (void) G_GNUC_CONST;
HpcMatrix *hpc_matrix_new (int cols, int rows);
void hpc_matrix_random(HpcMatrix *self);
void hpc_matrix_print(HpcMatrix *self);
double hpc_matrix_get(HpcMatrix *self, int x, int y);
void hpc_matrix_set(HpcMatrix *self, int x, int y, double v);
void hpc_matrix_process(HpcMatrix *self, double (*func)(double));
void hpc_matrix_process_parallel(HpcMatrix *self, double (*func)(double v));

G_END_DECLS

#endif /* __HPC_MATRIX_H__ */
