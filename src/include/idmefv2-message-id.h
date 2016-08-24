
/*****
*
* Copyright (C) 2004-2016 CS-SI. All Rights Reserved.
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
 * Template file: generate-top-level-class-id.h.mako
 *
 */



#ifndef _LIBIDMEFV2_IDMEFV2_MESSAGE_ID_H
#define _LIBIDMEFV2_IDMEFV2_MESSAGE_ID_H

/*
 * Misc value
 */

#define IDMEFV2_MSG_OWN_FORMAT                                 253
#define IDMEFV2_MSG_END_OF_TAG                                 254

/*
 * Tag value
 */


#define IDMEFV2_MSG_ADDRESS_TAG 0

#define IDMEFV2_MSG_USER_ID_TAG 1

#define IDMEFV2_MSG_NODE_NAME_TAG 2

#define IDMEFV2_MSG_LOCATION_TAG 3

#define IDMEFV2_MSG_INTERFACE_TAG 4

#define IDMEFV2_MSG_FILE_ACCESS_TAG 5

#define IDMEFV2_MSG_INODE_TAG 6

#define IDMEFV2_MSG_CHECKSUM_TAG 7

#define IDMEFV2_MSG_STREAM_TAG 8

#define IDMEFV2_MSG_FILE_TAG 9

#define IDMEFV2_MSG_LINKAGE_TAG 10

#define IDMEFV2_MSG_CONTAINER_TAG 11

#define IDMEFV2_MSG_PROCESS_TAG 12

#define IDMEFV2_MSG_SERVICE_TAG 13

#define IDMEFV2_MSG_NODE_TAG 14

#define IDMEFV2_MSG_USER_TAG 15

#define IDMEFV2_MSG_IMPACT_TYPE_TAG 16

#define IDMEFV2_MSG_SOURCE_TAG 17

#define IDMEFV2_MSG_TARGET_TAG 18

#define IDMEFV2_MSG_ORIGINAL_DATA_TAG 19

#define IDMEFV2_MSG_OBSERVABLE_TAG 20

#define IDMEFV2_MSG_TAKEN_ACTION_TAG 21

#define IDMEFV2_MSG_REFERENCE_TAG 22

#define IDMEFV2_MSG_ANALYZER_TAG 23

#define IDMEFV2_MSG_ADDITIONAL_DATA_TAG 24

#define IDMEFV2_MSG_CORRELATION_ALERT_TAG 25

#define IDMEFV2_MSG_ANALYSIS_DATA_TAG 26

#define IDMEFV2_MSG_CONFIDENCE_TAG 27

#define IDMEFV2_MSG_IMPACT_TAG 28

#define IDMEFV2_MSG_RECOMMANDED_ACTION_TAG 29

#define IDMEFV2_MSG_ASSESSMENT_TAG 30

#define IDMEFV2_MSG_CLASSIFICATION_TAG 31

#define IDMEFV2_MSG_HEARTBEAT_TAG 32

#define IDMEFV2_MSG_ALERT_TAG 33
/*
 * IDMEFV2 Address
 */


#define IDMEFV2_MSG_ADDRESS_IDENT 34

#define IDMEFV2_MSG_ADDRESS_TRANSLATION 35

#define IDMEFV2_MSG_ADDRESS_CATEGORY 36

#define IDMEFV2_MSG_ADDRESS_VLAN_NAME 37

#define IDMEFV2_MSG_ADDRESS_VLAN_NUM 38

#define IDMEFV2_MSG_ADDRESS_ADDRESS 39

#define IDMEFV2_MSG_ADDRESS_NETMASK 40

/*
 * IDMEFV2 User_id
 */


#define IDMEFV2_MSG_USER_ID_IDENT 34

#define IDMEFV2_MSG_USER_ID_CATEGORY 35

#define IDMEFV2_MSG_USER_ID_TTY 36

#define IDMEFV2_MSG_USER_ID_NAME 37

#define IDMEFV2_MSG_USER_ID_NUMBER 38

/*
 * IDMEFV2 Node_name
 */


#define IDMEFV2_MSG_NODE_NAME_CATEGORY 34

#define IDMEFV2_MSG_NODE_NAME_NAME 35

/*
 * IDMEFV2 Location
 */


#define IDMEFV2_MSG_LOCATION_NAME 34

#define IDMEFV2_MSG_LOCATION_LATITUDE 35

#define IDMEFV2_MSG_LOCATION_LONGITUDE 36

#define IDMEFV2_MSG_LOCATION_ALTITUDE 37

#define IDMEFV2_MSG_LOCATION_COUNTRY 38

#define IDMEFV2_MSG_LOCATION_CITY 39

#define IDMEFV2_MSG_LOCATION_POSTCODE 40

/*
 * IDMEFV2 Interface
 */


#define IDMEFV2_MSG_INTERFACE_NAME 34

#define IDMEFV2_MSG_INTERFACE_CATEGORY 35

#define IDMEFV2_MSG_INTERFACE_MEANING 36

/*
 * IDMEFV2 File_access
 */


#define IDMEFV2_MSG_FILE_ACCESS_PERMISSION 34

/*
 * IDMEFV2 Inode
 */


#define IDMEFV2_MSG_INODE_NUMBER 34

#define IDMEFV2_MSG_INODE_MAJOR_DEVICE 35

#define IDMEFV2_MSG_INODE_MINOR_DEVICE 36

#define IDMEFV2_MSG_INODE_C_MAJOR_DEVICE 37

#define IDMEFV2_MSG_INODE_C_MINOR_DEVICE 38

#define IDMEFV2_MSG_INODE_CHANGE_TIME 39

/*
 * IDMEFV2 Checksum
 */


#define IDMEFV2_MSG_CHECKSUM_VALUE 34

#define IDMEFV2_MSG_CHECKSUM_KEY 35

#define IDMEFV2_MSG_CHECKSUM_ALGORITHM 36

/*
 * IDMEFV2 Stream
 */


#define IDMEFV2_MSG_STREAM_OFFSETUNIT 34

#define IDMEFV2_MSG_STREAM_OFFSET 35

/*
 * IDMEFV2 File
 */


#define IDMEFV2_MSG_FILE_IDENT 34

#define IDMEFV2_MSG_FILE_NAME 35

#define IDMEFV2_MSG_FILE_PATH 36

#define IDMEFV2_MSG_FILE_CREATE_TIME 37

#define IDMEFV2_MSG_FILE_MODIFY_TIME 38

#define IDMEFV2_MSG_FILE_ACCESS_TIME 39

#define IDMEFV2_MSG_FILE_DATA_SIZE 40

#define IDMEFV2_MSG_FILE_DISK_SIZE 41

#define IDMEFV2_MSG_FILE_CATEGORY 42

#define IDMEFV2_MSG_FILE_FILE_TYPE 43

/*
 * IDMEFV2 Linkage
 */


#define IDMEFV2_MSG_LINKAGE_CATEGORY 34

#define IDMEFV2_MSG_LINKAGE_NAME 35

#define IDMEFV2_MSG_LINKAGE_PATH 36

/*
 * IDMEFV2 Container
 */


#define IDMEFV2_MSG_CONTAINER_NAME 34

#define IDMEFV2_MSG_CONTAINER_PATH 35

/*
 * IDMEFV2 Process
 */


#define IDMEFV2_MSG_PROCESS_IDENT 34

#define IDMEFV2_MSG_PROCESS_NAME 35

#define IDMEFV2_MSG_PROCESS_PID 36

#define IDMEFV2_MSG_PROCESS_PATH 37

#define IDMEFV2_MSG_PROCESS_ARG 38

#define IDMEFV2_MSG_PROCESS_ENV 39

#define IDMEFV2_MSG_PROCESS_TID 40

/*
 * IDMEFV2 Service
 */


#define IDMEFV2_MSG_SERVICE_IDENT 34

#define IDMEFV2_MSG_SERVICE_IANA_PROTOCOL_NUMBER 35

#define IDMEFV2_MSG_SERVICE_IANA_PROTOCOL_NAME 36

#define IDMEFV2_MSG_SERVICE_NAME 37

#define IDMEFV2_MSG_SERVICE_PORT 38

#define IDMEFV2_MSG_SERVICE_PORTLIST 39

#define IDMEFV2_MSG_SERVICE_PROTOCOL 40

/*
 * IDMEFV2 Node
 */


#define IDMEFV2_MSG_NODE_IDENT 34

/*
 * IDMEFV2 User
 */


#define IDMEFV2_MSG_USER_IDENT 34

#define IDMEFV2_MSG_USER_CATEGORY 35

/*
 * IDMEFV2 Impact_type
 */


#define IDMEFV2_MSG_IMPACT_TYPE_ORIGIN 34

#define IDMEFV2_MSG_IMPACT_TYPE_MEANING 35

#define IDMEFV2_MSG_IMPACT_TYPE_NAME 36

#define IDMEFV2_MSG_IMPACT_TYPE_URL 37

/*
 * IDMEFV2 Source
 */


#define IDMEFV2_MSG_SOURCE_IDENT 34

#define IDMEFV2_MSG_SOURCE_SPOOFED 35

/*
 * IDMEFV2 Target
 */


#define IDMEFV2_MSG_TARGET_IDENT 34

#define IDMEFV2_MSG_TARGET_DECOY 35

/*
 * IDMEFV2 Original_data
 */


#define IDMEFV2_MSG_ORIGINAL_DATA_TIMESTAMP 34

#define IDMEFV2_MSG_ORIGINAL_DATA_NAME 35

#define IDMEFV2_MSG_ORIGINAL_DATA_DATA 36

/*
 * IDMEFV2 Observable
 */


#define IDMEFV2_MSG_OBSERVABLE_ORIGIN 34

#define IDMEFV2_MSG_OBSERVABLE_MEANING 35

#define IDMEFV2_MSG_OBSERVABLE_URL 36

#define IDMEFV2_MSG_OBSERVABLE_NAME 37

/*
 * IDMEFV2 Taken_action
 */


#define IDMEFV2_MSG_TAKEN_ACTION_CATEGORY 34

/*
 * IDMEFV2 Reference
 */


#define IDMEFV2_MSG_REFERENCE_ORIGIN 34

#define IDMEFV2_MSG_REFERENCE_MEANING 35

#define IDMEFV2_MSG_REFERENCE_AUTHOR 36

#define IDMEFV2_MSG_REFERENCE_PUBLICATION_TIME 37

#define IDMEFV2_MSG_REFERENCE_VERSION 38

#define IDMEFV2_MSG_REFERENCE_NAME 39

#define IDMEFV2_MSG_REFERENCE_URL 40

/*
 * IDMEFV2 Analyzer
 */


#define IDMEFV2_MSG_ANALYZER_ANALYZERID 34

#define IDMEFV2_MSG_ANALYZER_NAME 35

#define IDMEFV2_MSG_ANALYZER_MANUFACTURER 36

#define IDMEFV2_MSG_ANALYZER_MODEL 37

#define IDMEFV2_MSG_ANALYZER_VERSION 38

#define IDMEFV2_MSG_ANALYZER_CLASS 39

#define IDMEFV2_MSG_ANALYZER_OSTYPE 40

#define IDMEFV2_MSG_ANALYZER_OSVERSION 41

#define IDMEFV2_MSG_ANALYZER_EXT_CLASS 42

#define IDMEFV2_MSG_ANALYZER_ANALYZERACTION 43

#define IDMEFV2_MSG_ANALYZER_ACTIONHISTORY 44

#define IDMEFV2_MSG_ANALYZER_ANALYZER_TIME 45

/*
 * IDMEFV2 Additional_data
 */


#define IDMEFV2_MSG_ADDITIONAL_DATA_MEANING 40

#define IDMEFV2_MSG_ADDITIONAL_DATA_TYPE 34

#define IDMEFV2_MSG_ADDITIONAL_DATA_DATA 35

/*
 * IDMEFV2 Correlation_alert
 */


#define IDMEFV2_MSG_CORRELATION_ALERT_NAME 34

#define IDMEFV2_MSG_CORRELATION_ALERT_ALERTIDENT 35

#define IDMEFV2_MSG_CORRELATION_ALERT_DURATION 36

#define IDMEFV2_MSG_CORRELATION_ALERT_COUNTER 37

/*
 * IDMEFV2 Analysis_data
 */


/*
 * IDMEFV2 Confidence
 */


#define IDMEFV2_MSG_CONFIDENCE_RATING 34

/*
 * IDMEFV2 Impact
 */


#define IDMEFV2_MSG_IMPACT_SEVERITY 34

#define IDMEFV2_MSG_IMPACT_COMPLETION 35

#define IDMEFV2_MSG_IMPACT_AVAILABILITY 36

#define IDMEFV2_MSG_IMPACT_INTEGRITY 37

#define IDMEFV2_MSG_IMPACT_CONFIDENTIALITY 38

#define IDMEFV2_MSG_IMPACT_DESCRIPTION 39

/*
 * IDMEFV2 Recommanded_action
 */


#define IDMEFV2_MSG_RECOMMANDED_ACTION_ACTION 34

#define IDMEFV2_MSG_RECOMMANDED_ACTION_STEP 35

/*
 * IDMEFV2 Assessment
 */


/*
 * IDMEFV2 Classification
 */


#define IDMEFV2_MSG_CLASSIFICATION_IDENT 34

#define IDMEFV2_MSG_CLASSIFICATION_TEXT 35

/*
 * IDMEFV2 Heartbeat
 */


#define IDMEFV2_MSG_HEARTBEAT_MESSAGEID 34

#define IDMEFV2_MSG_HEARTBEAT_CREATE_TIME 35

#define IDMEFV2_MSG_HEARTBEAT_HEARTBEAT_INTERVAL 36

/*
 * IDMEFV2 Alert
 */


#define IDMEFV2_MSG_ALERT_MESSAGEID 34

#define IDMEFV2_MSG_ALERT_ANALYZERHASH 35

#define IDMEFV2_MSG_ALERT_CREATE_TIME 36

#define IDMEFV2_MSG_ALERT_DETECT_TIME 37

/*
 * IDMEFV2 Message
 */


#define IDMEFV2_MSG_MESSAGE_VERSION 34

#endif /* _LIBIDMEFV2_IDMEFV2_MESSAGE_ID_H */