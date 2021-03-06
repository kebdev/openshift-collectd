/* GIO - GLib Input, Output and Streaming Library
 * 
 * Copyright (C) 2006-2007 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Alexander Larsson <alexl@redhat.com>
 *         David Zeuthen <davidz@redhat.com>
 */

#if !defined (__GIO_GIO_H_INSIDE__) && !defined (GIO_COMPILATION)
#error "Only <gio/gio.h> can be included directly."
#endif

#ifndef __G_VOLUME_H__
#define __G_VOLUME_H__

#include <glib-object.h>
#include <gio/gfile.h>
#include <gio/gdrive.h>

G_BEGIN_DECLS

#define G_VOLUME_IDENTIFIER_KIND_HAL_UDI "hal-udi"
#define G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE "unix-device"
#define G_VOLUME_IDENTIFIER_KIND_LABEL "label"
#define G_VOLUME_IDENTIFIER_KIND_UUID "uuid"
#define G_VOLUME_IDENTIFIER_KIND_NFS_MOUNT "nfs-mount"


#define G_TYPE_VOLUME            (g_volume_get_type ())
#define G_VOLUME(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), G_TYPE_VOLUME, GVolume))
#define G_IS_VOLUME(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_TYPE_VOLUME))
#define G_VOLUME_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), G_TYPE_VOLUME, GVolumeIface))

/**
 * GVolumeIface:
 * @g_iface: The parent interface.
 * @changed: Changed signal that is emitted when the volume's state has changed.
 * @removed: The removed signal that is emitted when the #GVolume have been removed. If the recipient is holding references to the object they should release them so the object can be finalized.
 * @get_name: Gets a string containing the name of the #GVolume.
 * @get_icon: Gets a #GIcon for the #GVolume.
 * @get_uuid: Gets the UUID for the #GVolume. The reference is typically based on the file system UUID for the mount in question and should be considered an opaque string. Returns %NULL if there is no UUID available.
 * @get_drive: Gets a #GDrive the volume is located on. Returns %NULL if the #GVolume is not associated with a #GDrive.
 * @get_mount: Gets a #GMount representing the mounted volume. Returns %NULL if the #GVolume is not mounted.
 * @can_mount: Returns %TRUE if the #GVolume can be mounted.
 * @can_eject: Checks if a #GVolume can be ejected.
 * @mount_fn: Mounts a given #GVolume.
 * @mount_finish: Finishes a mount operation.
 * @eject: Ejects a given #GVolume.
 * @eject_finish: Finishes an eject operation.
 * @get_identifier: Returns the identifier of the given kind, or %NULL if 
 *    the #GVolume doesn't have one.
 * @enumerate_identifiers: Returns an array strings listing the kinds
 *    of identifiers which the #GVolume has.
 * 
 * Interface for implementing operations for mountable volumes.
 **/
typedef struct _GVolumeIface    GVolumeIface;

struct _GVolumeIface
{
  GTypeInterface g_iface;

  /* signals */

  void (*changed) (GVolume *volume);
  void (*removed) (GVolume *volume);
  
  /* Virtual Table */

  char *    (*get_name)       (GVolume             *volume);
  GIcon *   (*get_icon)       (GVolume             *volume);
  char *    (*get_uuid)       (GVolume             *volume);
  GDrive *  (*get_drive)      (GVolume             *volume);
  GMount *  (*get_mount)      (GVolume             *volume);
  gboolean  (*can_mount)      (GVolume             *volume);
  gboolean  (*can_eject)      (GVolume             *volume);
  void      (*mount_fn)       (GVolume             *volume,
                               GMountOperation     *mount_operation,
                               GCancellable        *cancellable,
                               GAsyncReadyCallback  callback,
                               gpointer             user_data);
  gboolean  (*mount_finish)   (GVolume             *volume,
                               GAsyncResult        *result,
                               GError             **error);
  void      (*eject)          (GVolume             *volume,
			       GMountUnmountFlags   flags,
                               GCancellable        *cancellable,
                               GAsyncReadyCallback  callback,
                               gpointer             user_data);
  gboolean  (*eject_finish)   (GVolume             *volume,
                               GAsyncResult        *result,
                               GError             **error);
  
  char *   (*get_identifier)           (GVolume             *volume,
					const char          *kind);
  char **  (*enumerate_identifiers)    (GVolume             *volume);
};

GType     g_volume_get_type       (void) G_GNUC_CONST;

char *   g_volume_get_name              (GVolume              *volume);
GIcon *  g_volume_get_icon              (GVolume              *volume);
char *   g_volume_get_uuid              (GVolume              *volume);
GDrive * g_volume_get_drive             (GVolume              *volume);
GMount * g_volume_get_mount             (GVolume              *volume);
gboolean g_volume_can_mount             (GVolume              *volume);
gboolean g_volume_can_eject             (GVolume              *volume);
void     g_volume_mount                 (GVolume              *volume,
					 GMountOperation      *mount_operation,
					 GCancellable         *cancellable,
					 GAsyncReadyCallback   callback,
					 gpointer              user_data);
gboolean g_volume_mount_finish          (GVolume              *volume,
					 GAsyncResult         *result,
					 GError              **error);
void     g_volume_eject                 (GVolume              *volume,
					 GMountUnmountFlags    flags,
					 GCancellable         *cancellable,
					 GAsyncReadyCallback   callback,
					 gpointer              user_data);
gboolean g_volume_eject_finish          (GVolume              *volume,
					 GAsyncResult         *result,
					 GError              **error);
char *   g_volume_get_identifier        (GVolume              *volume,
					 const char           *kind);
char **  g_volume_enumerate_identifiers (GVolume              *volume);


G_END_DECLS

#endif /* __G_VOLUME_H__ */
