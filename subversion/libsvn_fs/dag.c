/* dag.c : DAG-like interface filesystem, private to libsvn_fs
 *
 * ====================================================================
 * Copyright (c) 2000 CollabNet.  All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at http://subversion.tigris.org/license-1.html.
 * If newer versions of this license are posted there, you may use a
 * newer version instead, at your option.
 * ====================================================================
 */

#include "svn_path.h"
#include "svn_fs.h"
#include "dag.h"
#include "err.h"
#include "fs.h"
#include "nodes-table.h"
#include "rev-table.h"
#include "skel.h"
#include "trail.h"



/* Initializing a filesystem.  */

struct dag_node_t
{
  /* The filesystem this dag node came from. */
  svn_fs_t *fs;

  /* The identifier for this dag node in FS (i.e., the DB key, parsed
     into an svn_fs_id_t). */
  svn_fs_id_t *id;

  /* The contents of the node (i.e., the DB value, parsed into a
     skel). */
  skel_t *contents;

  /* Not yet sure exactly what gets allocated in this pool.  If I
     don't get sure soon, it'll go away.  Probably this node and
     subnodes. */
  apr_pool_t *pool;
};


/* Trail body for svn_fs__dag_init_fs. */
static svn_error_t *
dag_init_fs (void *fs_baton, trail_t *trail)
{
  svn_fs_t *fs = fs_baton;

  /* Create empty root directory with node revision 0.0:
     "nodes" : "0.0" -> "(fulltext [(dir ()) ()])" */
  {
    static char rep_skel[] = "(fulltext ((dir ()) ()))";
    SVN_ERR (svn_fs__put_rep (fs,
                              svn_fs_parse_id ("0.0", 3, trail->pool),
                              svn_fs__parse_skel (rep_skel,
                                                  sizeof (rep_skel) - 1,
                                                  trail->pool),
                              trail->db_txn,
                              trail->pool));
  } 

  /* Link it into filesystem revision 0:
     "revisions" : 0 -> "(revision  3 0.0  ())" */
  {
    static char rev_skel[] = "(revision  3 0.0  ())";
    svn_revnum_t rev = 0;
    SVN_ERR (svn_fs__put_rev (&rev, fs,
                              svn_fs__parse_skel (rev_skel,
                                                  sizeof (rev_skel) - 1,
                                                  trail->pool),
                              trail->db_txn,
                              trail->pool));

    if (rev != 0)
      return svn_error_createf (SVN_ERR_FS_CORRUPT, 0, 0, fs->pool,
                                "initial revision number is not `0'"
                                " in filesystem `%s'",
                                fs->env_path);
  }

  return SVN_NO_ERROR;
}


svn_error_t *
svn_fs__dag_init_fs (svn_fs_t *fs)
{
  return svn_fs__retry_txn (fs, dag_init_fs, fs, fs->pool);
}



/* ### these functions are defined so that we can load the library.
   ### without them, we get undefined references from tree.c
   ### obviously, they don't work and will need to be filled in...
*/
svn_error_t *svn_fs__dag_txn_node (dag_node_t **node_p,
                                   svn_fs_t *fs,
                                   const char *txn,
                                   const svn_fs_id_t *id,
                                   trail_t *trail)
{
  abort();
  /* NOTREACHED */
  return NULL;
}
void svn_fs__dag_close (dag_node_t *node)
{
  abort();
}
const svn_fs_id_t *svn_fs__dag_get_id (dag_node_t *node)
{
  abort();
  /* NOTREACHED */
  return NULL;
}
svn_error_t *svn_fs__dag_get_proplist (skel_t **proplist_p,
                                       dag_node_t *node,
                                       trail_t *trail)
{
  abort();
  /* NOTREACHED */
  return NULL;
}
svn_error_t *svn_fs__dag_set_proplist (dag_node_t *node,
                                       skel_t *proplist,
                                       trail_t *trail)
{
  abort();
  /* NOTREACHED */
  return NULL;
}
svn_error_t *svn_fs__dag_clone_child (dag_node_t **child_p,
                                      dag_node_t *parent,
                                      const char *name,
                                      trail_t *trail)
{
  abort();
  /* NOTREACHED */
  return NULL;
}


svn_error_t *
svn_fs__dag_revision_root (dag_node_t **node_p,
                           svn_fs_t *fs,
                           svn_revnum_t rev,
                           trail_t *trail)
{
#if 0
  kff todo: coding here;
  svn_error_t *err;
  err = svn_fs__get_rep (skel_t **skel_p,
                         svn_fs_t *fs,
                         const svn_fs_id_t *id,
                         DB_TXN *db_txn,
                         apr_pool_t *pool);
#endif

  return SVN_NO_ERROR;
}


svn_error_t *
svn_fs__dag_clone_root (dag_node_t **root_p,
                        svn_fs_t *fs,
                        const char *svn_txn,
                        trail_t *trail)
{
  svn_error_t *err;
  dag_node_t *root_node;
  svn_revnum_t revision;  /* Hmm, how to get this? */

  /* Step 1. Find the immutable root node for this transaction.
   * Step 2. Make a new node with the same contents, except mutable.
   * Step 3. Store the new node under a successor key (see
   *         svn_fs__new_successor_id).
   */

  /* Step 1. */
  err = svn_fs__dag_revision_root (&root_node,
                                   fs,
                                   revision,
                                   trail);
  if (err)
    return err;

  /* Step 2. */
  abort();  /* heh */

  /* Step 3. */
  /* What, you're still here? */

  return SVN_NO_ERROR;
}


svn_error_t *svn_fs__dag_open (dag_node_t **child_p,
                               dag_node_t *parent,
                               const char *name,
                               trail_t *trail)
{
  abort();
  /* NOTREACHED */
  return NULL;
}

svn_error_t *
svn_fs__dag_open_path (dag_node_t **child_p,
                       dag_node_t **parent_p,
                       dag_node_t *root,
                       const char *path,
                       trail_t *trail)
{
#if 0
  dag_node_t *child = root;
  dag_node_t *parent = NULL;
  char *mutable_path = apr_pstrdup (trail->pool, path);
  const char *name;

  while (svn_path_first_component(&name, &mutable_path,
                                  svn_path_repos_style))
    {
      parent = child;
      SVN_ERR (svn_fs__dag_open (&child, parent, name, trail));
    }

  *child_p = child;
  *parent_p = parent;
#else
  abort ();
#endif
  return SVN_NO_ERROR;
}


svn_error_t *svn_fs__dag_delete (dag_node_t *parent,
                                 const char *name,
                                 trail_t *trail)
{
  abort();
  /* NOTREACHED */
  return NULL;
}



/* 
 * local variables:
 * eval: (load-file "../svn-dev.el")
 * end:
 */
