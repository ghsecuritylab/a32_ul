
/*
 * Copyright (c) 2007-2013 Nicira, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _UAPI__LINUX_OPENVSWITCH_H
#define _UAPI__LINUX_OPENVSWITCH_H 1

#include <linux/types.h>
#include <linux/if_ether.h>


struct ovs_header {
	int dp_ifindex;
};


#define OVS_DATAPATH_FAMILY  "ovs_datapath"
#define OVS_DATAPATH_MCGROUP "ovs_datapath"

#define OVS_DATAPATH_VERSION 2

#define OVS_DP_VER_FEATURES 2

enum ovs_datapath_cmd {
	OVS_DP_CMD_UNSPEC,
	OVS_DP_CMD_NEW,
	OVS_DP_CMD_DEL,
	OVS_DP_CMD_GET,
	OVS_DP_CMD_SET
};

enum ovs_datapath_attr {
	OVS_DP_ATTR_UNSPEC,
	OVS_DP_ATTR_NAME,		
	OVS_DP_ATTR_UPCALL_PID,		
	OVS_DP_ATTR_STATS,		
	OVS_DP_ATTR_MEGAFLOW_STATS,	
	OVS_DP_ATTR_USER_FEATURES,	
	__OVS_DP_ATTR_MAX
};

#define OVS_DP_ATTR_MAX (__OVS_DP_ATTR_MAX - 1)

struct ovs_dp_stats {
	__u64 n_hit;             
	__u64 n_missed;          
	__u64 n_lost;            
	__u64 n_flows;           
};

struct ovs_dp_megaflow_stats {
	__u64 n_mask_hit;	 
	__u32 n_masks;		 
	__u32 pad0;		 
	__u64 pad1;		 
	__u64 pad2;		 
};

struct ovs_vport_stats {
	__u64   rx_packets;		
	__u64   tx_packets;		
	__u64   rx_bytes;		
	__u64   tx_bytes;		
	__u64   rx_errors;		
	__u64   tx_errors;		
	__u64   rx_dropped;		
	__u64   tx_dropped;		
};

#define OVS_DP_F_UNALIGNED	(1 << 0)

#define OVSP_LOCAL      ((__u32)0)


#define OVS_PACKET_FAMILY "ovs_packet"
#define OVS_PACKET_VERSION 0x1

enum ovs_packet_cmd {
	OVS_PACKET_CMD_UNSPEC,

	
	OVS_PACKET_CMD_MISS,    
	OVS_PACKET_CMD_ACTION,  

	
	OVS_PACKET_CMD_EXECUTE  
};

enum ovs_packet_attr {
	OVS_PACKET_ATTR_UNSPEC,
	OVS_PACKET_ATTR_PACKET,      
	OVS_PACKET_ATTR_KEY,         
	OVS_PACKET_ATTR_ACTIONS,     
	OVS_PACKET_ATTR_USERDATA,    
	__OVS_PACKET_ATTR_MAX
};

#define OVS_PACKET_ATTR_MAX (__OVS_PACKET_ATTR_MAX - 1)


#define OVS_VPORT_FAMILY  "ovs_vport"
#define OVS_VPORT_MCGROUP "ovs_vport"
#define OVS_VPORT_VERSION 0x1

enum ovs_vport_cmd {
	OVS_VPORT_CMD_UNSPEC,
	OVS_VPORT_CMD_NEW,
	OVS_VPORT_CMD_DEL,
	OVS_VPORT_CMD_GET,
	OVS_VPORT_CMD_SET
};

enum ovs_vport_type {
	OVS_VPORT_TYPE_UNSPEC,
	OVS_VPORT_TYPE_NETDEV,   
	OVS_VPORT_TYPE_INTERNAL, 
	OVS_VPORT_TYPE_GRE,      
	OVS_VPORT_TYPE_VXLAN,	 
	__OVS_VPORT_TYPE_MAX
};

#define OVS_VPORT_TYPE_MAX (__OVS_VPORT_TYPE_MAX - 1)

enum ovs_vport_attr {
	OVS_VPORT_ATTR_UNSPEC,
	OVS_VPORT_ATTR_PORT_NO,	
	OVS_VPORT_ATTR_TYPE,	
	OVS_VPORT_ATTR_NAME,	
	OVS_VPORT_ATTR_OPTIONS, 
	OVS_VPORT_ATTR_UPCALL_PID, 
	OVS_VPORT_ATTR_STATS,	
	__OVS_VPORT_ATTR_MAX
};

#define OVS_VPORT_ATTR_MAX (__OVS_VPORT_ATTR_MAX - 1)

enum {
	OVS_TUNNEL_ATTR_UNSPEC,
	OVS_TUNNEL_ATTR_DST_PORT, 
	__OVS_TUNNEL_ATTR_MAX
};

#define OVS_TUNNEL_ATTR_MAX (__OVS_TUNNEL_ATTR_MAX - 1)


#define OVS_FLOW_FAMILY  "ovs_flow"
#define OVS_FLOW_MCGROUP "ovs_flow"
#define OVS_FLOW_VERSION 0x1

enum ovs_flow_cmd {
	OVS_FLOW_CMD_UNSPEC,
	OVS_FLOW_CMD_NEW,
	OVS_FLOW_CMD_DEL,
	OVS_FLOW_CMD_GET,
	OVS_FLOW_CMD_SET
};

struct ovs_flow_stats {
	__u64 n_packets;         
	__u64 n_bytes;           
};

enum ovs_key_attr {
	OVS_KEY_ATTR_UNSPEC,
	OVS_KEY_ATTR_ENCAP,	
	OVS_KEY_ATTR_PRIORITY,  
	OVS_KEY_ATTR_IN_PORT,   
	OVS_KEY_ATTR_ETHERNET,  
	OVS_KEY_ATTR_VLAN,	
	OVS_KEY_ATTR_ETHERTYPE,	
	OVS_KEY_ATTR_IPV4,      
	OVS_KEY_ATTR_IPV6,      
	OVS_KEY_ATTR_TCP,       
	OVS_KEY_ATTR_UDP,       
	OVS_KEY_ATTR_ICMP,      
	OVS_KEY_ATTR_ICMPV6,    
	OVS_KEY_ATTR_ARP,       
	OVS_KEY_ATTR_ND,        
	OVS_KEY_ATTR_SKB_MARK,  
	OVS_KEY_ATTR_TUNNEL,    
	OVS_KEY_ATTR_SCTP,      
	OVS_KEY_ATTR_TCP_FLAGS,	

#ifdef __KERNEL__
	OVS_KEY_ATTR_IPV4_TUNNEL,  
#endif
	__OVS_KEY_ATTR_MAX
};

#define OVS_KEY_ATTR_MAX (__OVS_KEY_ATTR_MAX - 1)

enum ovs_tunnel_key_attr {
	OVS_TUNNEL_KEY_ATTR_ID,                 
	OVS_TUNNEL_KEY_ATTR_IPV4_SRC,           
	OVS_TUNNEL_KEY_ATTR_IPV4_DST,           
	OVS_TUNNEL_KEY_ATTR_TOS,                
	OVS_TUNNEL_KEY_ATTR_TTL,                
	OVS_TUNNEL_KEY_ATTR_DONT_FRAGMENT,      
	OVS_TUNNEL_KEY_ATTR_CSUM,               
	__OVS_TUNNEL_KEY_ATTR_MAX
};

#define OVS_TUNNEL_KEY_ATTR_MAX (__OVS_TUNNEL_KEY_ATTR_MAX - 1)

enum ovs_frag_type {
	OVS_FRAG_TYPE_NONE,
	OVS_FRAG_TYPE_FIRST,
	OVS_FRAG_TYPE_LATER,
	__OVS_FRAG_TYPE_MAX
};

#define OVS_FRAG_TYPE_MAX (__OVS_FRAG_TYPE_MAX - 1)

struct ovs_key_ethernet {
	__u8	 eth_src[ETH_ALEN];
	__u8	 eth_dst[ETH_ALEN];
};

struct ovs_key_ipv4 {
	__be32 ipv4_src;
	__be32 ipv4_dst;
	__u8   ipv4_proto;
	__u8   ipv4_tos;
	__u8   ipv4_ttl;
	__u8   ipv4_frag;	
};

struct ovs_key_ipv6 {
	__be32 ipv6_src[4];
	__be32 ipv6_dst[4];
	__be32 ipv6_label;	
	__u8   ipv6_proto;
	__u8   ipv6_tclass;
	__u8   ipv6_hlimit;
	__u8   ipv6_frag;	
};

struct ovs_key_tcp {
	__be16 tcp_src;
	__be16 tcp_dst;
};

struct ovs_key_udp {
	__be16 udp_src;
	__be16 udp_dst;
};

struct ovs_key_sctp {
	__be16 sctp_src;
	__be16 sctp_dst;
};

struct ovs_key_icmp {
	__u8 icmp_type;
	__u8 icmp_code;
};

struct ovs_key_icmpv6 {
	__u8 icmpv6_type;
	__u8 icmpv6_code;
};

struct ovs_key_arp {
	__be32 arp_sip;
	__be32 arp_tip;
	__be16 arp_op;
	__u8   arp_sha[ETH_ALEN];
	__u8   arp_tha[ETH_ALEN];
};

struct ovs_key_nd {
	__u32 nd_target[4];
	__u8  nd_sll[ETH_ALEN];
	__u8  nd_tll[ETH_ALEN];
};

enum ovs_flow_attr {
	OVS_FLOW_ATTR_UNSPEC,
	OVS_FLOW_ATTR_KEY,       
	OVS_FLOW_ATTR_ACTIONS,   
	OVS_FLOW_ATTR_STATS,     
	OVS_FLOW_ATTR_TCP_FLAGS, 
	OVS_FLOW_ATTR_USED,      
	OVS_FLOW_ATTR_CLEAR,     
	OVS_FLOW_ATTR_MASK,      
	__OVS_FLOW_ATTR_MAX
};

#define OVS_FLOW_ATTR_MAX (__OVS_FLOW_ATTR_MAX - 1)

enum ovs_sample_attr {
	OVS_SAMPLE_ATTR_UNSPEC,
	OVS_SAMPLE_ATTR_PROBABILITY, 
	OVS_SAMPLE_ATTR_ACTIONS,     
	__OVS_SAMPLE_ATTR_MAX,
};

#define OVS_SAMPLE_ATTR_MAX (__OVS_SAMPLE_ATTR_MAX - 1)

enum ovs_userspace_attr {
	OVS_USERSPACE_ATTR_UNSPEC,
	OVS_USERSPACE_ATTR_PID,	      
	OVS_USERSPACE_ATTR_USERDATA,  
	__OVS_USERSPACE_ATTR_MAX
};

#define OVS_USERSPACE_ATTR_MAX (__OVS_USERSPACE_ATTR_MAX - 1)

struct ovs_action_push_vlan {
	__be16 vlan_tpid;	
	__be16 vlan_tci;	
};


enum ovs_action_attr {
	OVS_ACTION_ATTR_UNSPEC,
	OVS_ACTION_ATTR_OUTPUT,	      
	OVS_ACTION_ATTR_USERSPACE,    
	OVS_ACTION_ATTR_SET,          
	OVS_ACTION_ATTR_PUSH_VLAN,    
	OVS_ACTION_ATTR_POP_VLAN,     
	OVS_ACTION_ATTR_SAMPLE,       
	__OVS_ACTION_ATTR_MAX
};

#define OVS_ACTION_ATTR_MAX (__OVS_ACTION_ATTR_MAX - 1)

#endif 
