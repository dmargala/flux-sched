/*****************************************************************************\
 * Copyright 2022 Lawrence Livermore National Security, LLC
 * (c.f. AUTHORS, NOTICE.LLNS, LICENSE)
 *
 * This file is part of the Flux resource manager framework.
 * For details, see https://github.com/flux-framework.
 *
 * SPDX-License-Identifier: LGPL-3.0
\*****************************************************************************/

#ifndef RESOURCE_READER_RV1EXEC_HPP
#define RESOURCE_READER_RV1EXEC_HPP

extern "C" {
#include <flux/hostlist.h>
}

#include "resource/schema/resource_graph.hpp"
#include "resource/readers/resource_reader_base.hpp"

namespace Flux {
namespace resource_model {


/*! RV1EXEC resource reader class.
 */
class resource_reader_rv1exec_t : public resource_reader_base_t {
public:

    virtual ~resource_reader_rv1exec_t ();

    /*! Unpack str into a resource graph.
     *
     * \param g      resource graph
     * \param m      resource graph meta data
     * \param str    string containing a RV1's .execution key value
     * \param rank   assign rank to all of the newly created resource vertices
     * \return       0 on success; non-zero integer on an error
     *                   EOVERFLOW: hostname suffix too large
     *                   EEXIST: mal-formed ranks
     *                   ENOMEM: out of memory
     *                   EINVAL: invalid input or operation
     */
    virtual int unpack (resource_graph_t &g, resource_graph_metadata_t &m,
                        const std::string &str, int rank = -1);

    /*! Unpack str into a resource graph and graft
     *  the top-level vertices to vtx.
     *
     * \param g      resource graph
     * \param m      resource graph meta data
     * \param vtx    parent vtx at which to graft the deserialized graph
     * \param str    string containing a RV1's .execution key value
     * \param rank   assign this rank to all the newly created resource vertices
     * \return       -1 with ENOTSUP (Not supported yet)
     */
    virtual int unpack_at (resource_graph_t &g, resource_graph_metadata_t &m,
                           vtx_t &vtx, const std::string &str, int rank = -1);

    /*! Update resource graph g with str.
     *
     * \param g      resource graph
     * \param m      resource graph meta data
     * \param str    resource set string
     * \param jobid  jobid of str
     * \param at     start time of this job
     * \param dur    duration of this job
     * \param rsv    true if this update is for a reservation.
     * \param trav_token
     *               token to be used by traverser
     * \return       -1 with ENOTSUP (Not supported yet)
     */
    virtual int update (resource_graph_t &g, resource_graph_metadata_t &m,
                        const std::string &str, int64_t jobid, int64_t at,
                        uint64_t dur, bool rsv, uint64_t trav_token);

    /*! Is the selected reader format support allowlist
     *
     * \return       false
     */
    virtual bool is_allowlist_supported ();

private:
    vtx_t add_vertex (resource_graph_t &g,
                      resource_graph_metadata_t &m,
                      vtx_t parent, int id,
                      const std::string &subsys,
                      const std::string &type,
                      const std::string &basename,
                      const std::string &name,
                      const std::map<std::string,
                                     std::string> &props, int size, int rank);

    int add_metadata (resource_graph_t &g,
                      resource_graph_metadata_t &m,
                      edg_t e, vtx_t src, vtx_t dst);

    int add_edges (resource_graph_t &g,
                   resource_graph_metadata_t &m,
                   vtx_t src, vtx_t dst,
                   const std::string &subsys,
                   const std::string &relation,
                   const std::string &rev_relation);

    int add_cluster_vertex (resource_graph_t &g, resource_graph_metadata_t &m);

    int build_rmap (json_t *rlite, std::map<unsigned, unsigned> &rmap);

    int unpack_child (resource_graph_t &g,
                      resource_graph_metadata_t &m, vtx_t parent,
                      const char *resource_type,
                      const char *resource_ids, unsigned rank);

    int unpack_children (resource_graph_t &g,
                         resource_graph_metadata_t &m,
                         vtx_t parent, json_t *children, unsigned rank);

    int unpack_rank (resource_graph_t &g,
                     resource_graph_metadata_t &m,
                     vtx_t parent, unsigned rank, json_t *children,
                     struct hostlist *hlist, std::map<unsigned,
                                                      unsigned> &rmap);

    int unpack_rlite_entry (resource_graph_t &g,
                            resource_graph_metadata_t &m,
                            vtx_t parent, json_t *entry,
                            struct hostlist *hlist, std::map<unsigned,
                                                             unsigned> &rmap);
    int unpack_rlite (resource_graph_t &g,
                      resource_graph_metadata_t &m, json_t *rlite,
                      struct hostlist *hlist, std::map<unsigned,
                                                       unsigned> &rmap);

    int unpack_internal (resource_graph_t &g,
                         resource_graph_metadata_t &m, json_t *rv1);
};

} // namespace resource_model
} // namespace Flux

#endif // RESOURCE_READER_RV1EXEC_HPP

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
