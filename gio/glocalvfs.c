#include <config.h>
#include "glocalvfs.h"
#include "glocalfile.h"

static void g_local_vfs_class_init     (GLocalVfsClass *class);
static void g_local_vfs_vfs_iface_init (GVfsIface       *iface);
static void g_local_vfs_finalize       (GObject         *object);

struct _GLocalVfs
{
  GObject parent;
};

G_DEFINE_TYPE_WITH_CODE (GLocalVfs, g_local_vfs, G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (G_TYPE_VFS,
						g_local_vfs_vfs_iface_init))
 
static void
g_local_vfs_class_init (GLocalVfsClass *class)
{
  GObjectClass *object_class;
  
  object_class = (GObjectClass *) class;

  object_class->finalize = g_local_vfs_finalize;
}

static void
g_local_vfs_finalize (GObject *object)
{
  /* must chain up */
  G_OBJECT_CLASS (g_local_vfs_parent_class)->finalize (object);
}

static void
g_local_vfs_init (GLocalVfs *vfs)
{
}

GVfs *
g_local_vfs_new (void)
{
  return g_object_new (G_TYPE_LOCAL_VFS, NULL);
}

static GFile *
g_local_vfs_get_file_for_path  (GVfs       *vfs,
				const char *path)
{
  return g_local_file_new (path);
}

static GFile *
g_local_vfs_get_file_for_uri   (GVfs       *vfs,
				const char *uri)
{
  char *path;
  GFile *file;

  path = g_filename_from_uri (uri, NULL, NULL);

  if (path != NULL)
    file = g_local_file_new (path);
  else
    file = NULL;

  g_free (path);

  return file;
}

static GFile *
g_local_vfs_parse_name (GVfs       *vfs,
			const char *parse_name)
{
  GFile *file;
  char *filename;
  
  g_return_val_if_fail (G_IS_VFS (vfs), NULL);
  g_return_val_if_fail (parse_name != NULL, NULL);

  if (g_ascii_strncasecmp ("file:", parse_name, 5))
    filename = g_filename_from_uri (parse_name, NULL, NULL);
  else
    filename = g_filename_from_utf8 (parse_name, -1, NULL, NULL, NULL);
    
  file = g_local_file_new (filename);
  g_free (filename);

  return file;
}

static void
g_local_vfs_vfs_iface_init (GVfsIface *iface)
{
  iface->get_file_for_path = g_local_vfs_get_file_for_path;
  iface->get_file_for_uri = g_local_vfs_get_file_for_uri;
  iface->parse_name = g_local_vfs_parse_name;
}