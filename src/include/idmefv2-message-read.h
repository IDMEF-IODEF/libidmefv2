
/*****
*
* Copyright (C) 2001-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@prelude-ids.com>
* Author: Nicolas Delon <nicolas.delon@prelude-ids.com>
*
* This file is part of the Libidmefv2 library.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*****/

/*
 * This file was automatically generated by our generator, version 1
 *
 * Do not make changes to this file unless you know what you are doing.
 * modify the template interface file instead.
 * IDMEFV2 version : Not released yet
 * Template file: generate-top-level-class-read.h.mako
 *
 */

#ifndef _LIBIDMEFV2_IDMEFV2_MESSAGE_READ_H
#define _LIBIDMEFV2_IDMEFV2_MESSAGE_READ_H

#include "libidmefv2-inttypes.h"
#include "libidmefv2-msgbuf.h"

#ifdef __cplusplus
 extern "C" {
#endif


int idmefv2_address_read(idmefv2_address_t *address, libidmefv2_msg_t *msg);
int idmefv2_user_id_read(idmefv2_user_id_t *user_id, libidmefv2_msg_t *msg);
int idmefv2_node_name_read(idmefv2_node_name_t *node_name, libidmefv2_msg_t *msg);
int idmefv2_location_read(idmefv2_location_t *location, libidmefv2_msg_t *msg);
int idmefv2_interface_read(idmefv2_interface_t *interface, libidmefv2_msg_t *msg);
int idmefv2_file_access_read(idmefv2_file_access_t *file_access, libidmefv2_msg_t *msg);
int idmefv2_inode_read(idmefv2_inode_t *inode, libidmefv2_msg_t *msg);
int idmefv2_checksum_read(idmefv2_checksum_t *checksum, libidmefv2_msg_t *msg);
int idmefv2_stream_read(idmefv2_stream_t *stream, libidmefv2_msg_t *msg);
int idmefv2_file_read(idmefv2_file_t *file, libidmefv2_msg_t *msg);
int idmefv2_linkage_read(idmefv2_linkage_t *linkage, libidmefv2_msg_t *msg);
int idmefv2_container_read(idmefv2_container_t *container, libidmefv2_msg_t *msg);
int idmefv2_process_read(idmefv2_process_t *process, libidmefv2_msg_t *msg);
int idmefv2_service_read(idmefv2_service_t *service, libidmefv2_msg_t *msg);
int idmefv2_node_read(idmefv2_node_t *node, libidmefv2_msg_t *msg);
int idmefv2_user_read(idmefv2_user_t *user, libidmefv2_msg_t *msg);
int idmefv2_impact_type_read(idmefv2_impact_type_t *impact_type, libidmefv2_msg_t *msg);
int idmefv2_source_read(idmefv2_source_t *source, libidmefv2_msg_t *msg);
int idmefv2_target_read(idmefv2_target_t *target, libidmefv2_msg_t *msg);
int idmefv2_original_data_read(idmefv2_original_data_t *original_data, libidmefv2_msg_t *msg);
int idmefv2_observable_read(idmefv2_observable_t *observable, libidmefv2_msg_t *msg);
int idmefv2_taken_action_read(idmefv2_taken_action_t *taken_action, libidmefv2_msg_t *msg);
int idmefv2_reference_read(idmefv2_reference_t *reference, libidmefv2_msg_t *msg);
int idmefv2_analyzer_read(idmefv2_analyzer_t *analyzer, libidmefv2_msg_t *msg);
int idmefv2_additional_data_read(idmefv2_additional_data_t *additional_data, libidmefv2_msg_t *msg);
int idmefv2_correlation_alert_read(idmefv2_correlation_alert_t *correlation_alert, libidmefv2_msg_t *msg);
int idmefv2_analysis_data_read(idmefv2_analysis_data_t *analysis_data, libidmefv2_msg_t *msg);
int idmefv2_confidence_read(idmefv2_confidence_t *confidence, libidmefv2_msg_t *msg);
int idmefv2_impact_read(idmefv2_impact_t *impact, libidmefv2_msg_t *msg);
int idmefv2_recommanded_action_read(idmefv2_recommanded_action_t *recommanded_action, libidmefv2_msg_t *msg);
int idmefv2_assessment_read(idmefv2_assessment_t *assessment, libidmefv2_msg_t *msg);
int idmefv2_classification_read(idmefv2_classification_t *classification, libidmefv2_msg_t *msg);
int idmefv2_heartbeat_read(idmefv2_heartbeat_t *heartbeat, libidmefv2_msg_t *msg);
int idmefv2_alert_read(idmefv2_alert_t *alert, libidmefv2_msg_t *msg);
int idmefv2_message_read(idmefv2_message_t *message, libidmefv2_msg_t *msg);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIDMEFV2_IDMEFV2_MESSAGE_READ_H */
