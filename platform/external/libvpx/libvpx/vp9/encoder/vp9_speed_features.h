/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef VP9_ENCODER_VP9_SPEED_FEATURES_H_
#define VP9_ENCODER_VP9_SPEED_FEATURES_H_

#include "vp9/common/vp9_enums.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  DIAMOND = 0,
  NSTEP = 1,
  HEX = 2,
  BIGDIA = 3,
  SQUARE = 4,
  FAST_HEX = 5,
  FAST_DIAMOND = 6
} SEARCH_METHODS;

typedef enum {
  
  DISALLOW_RECODE = 0,
  
  ALLOW_RECODE_KFMAXBW = 1,
  
  ALLOW_RECODE_KFARFGF = 2,
  
  ALLOW_RECODE = 3,
} RECODE_LOOP_TYPE;

typedef enum {
  SUBPEL_TREE = 0,
  
} SUBPEL_SEARCH_METHODS;

typedef enum {
  LAST_FRAME_PARTITION_OFF = 0,
  LAST_FRAME_PARTITION_LOW_MOTION = 1,
  LAST_FRAME_PARTITION_ALL = 2
} LAST_FRAME_PARTITION_METHOD;

typedef enum {
  USE_FULL_RD = 0,
  USE_LARGESTINTRA,
  USE_LARGESTINTRA_MODELINTER,
  USE_LARGESTALL
} TX_SIZE_SEARCH_METHOD;

typedef enum {
  NOT_IN_USE = 0,
  RELAXED_NEIGHBORING_MIN_MAX = 1,
  STRICT_NEIGHBORING_MIN_MAX = 2
} AUTO_MIN_MAX_MODE;

typedef enum {
  
  LPF_PICK_FROM_FULL_IMAGE,
  
  LPF_PICK_FROM_SUBIMAGE,
  
  LPF_PICK_FROM_Q,
} LPF_PICK_METHOD;

typedef enum {
  
  
  FLAG_EARLY_TERMINATE = 1 << 0,

  
  FLAG_SKIP_COMP_BESTINTRA = 1 << 1,

  
  
  
  FLAG_SKIP_COMP_REFMISMATCH = 1 << 2,

  
  FLAG_SKIP_INTRA_BESTINTER = 1 << 3,

  
  
  FLAG_SKIP_INTRA_DIRMISMATCH = 1 << 4,

  
  FLAG_SKIP_INTRA_LOWVAR = 1 << 5,
} MODE_SEARCH_SKIP_LOGIC;

typedef enum {
  
  SEARCH_PARTITION = 0,

  
  FIXED_PARTITION = 1,

  
  
  VAR_BASED_FIXED_PARTITION = 2,

  REFERENCE_PARTITION = 3,

  
  
  VAR_BASED_PARTITION,

  
  SOURCE_VAR_BASED_PARTITION
} PARTITION_SEARCH_TYPE;

typedef enum {
  
  
  TWO_LOOP = 0,

  
  ONE_LOOP = 1,

  
  
  ONE_LOOP_REDUCED = 2
} FAST_COEFF_UPDATE;

typedef struct {
  
  int frame_parameter_update;

  
  SEARCH_METHODS search_method;

  RECODE_LOOP_TYPE recode_loop;

  
  
  
  
  SUBPEL_SEARCH_METHODS subpel_search_method;

  
  int subpel_iters_per_step;

  
  int subpel_force_stop;

  
  
  int max_step_search_steps;

  
  
  int reduce_first_step_size;

  
  
  int auto_mv_step_size;

  
  int optimize_coefficients;

  
  
  
  
  
  int static_segmentation;

  
  
  
  
  int comp_inter_joint_search_thresh;

  
  
  int adaptive_rd_thresh;

  
  
  
  int skip_encode_sb;
  int skip_encode_frame;
  
  
  int allow_skip_recode;

  
  
  
  
  
  
  
  LAST_FRAME_PARTITION_METHOD use_lastframe_partitioning;

  
  
  
  TX_SIZE_SEARCH_METHOD tx_size_search_method;

  
  
  int use_lp32x32fdct;

  

  
  
  
  int mode_skip_start;

  
  int reference_masking;

  PARTITION_SEARCH_TYPE partition_search_type;

  
  BLOCK_SIZE always_this_block_size;

  
  
  int less_rectangular_check;

  
  int use_square_partition_only;

  
  
  AUTO_MIN_MAX_MODE auto_min_max_partition_size;

  
  
  BLOCK_SIZE min_partition_size;
  BLOCK_SIZE max_partition_size;

  
  
  int adjust_partitioning_from_last_frame;

  
  
  int last_partitioning_redo_frequency;

  
  
  
  
  int constrain_copy_partition;

  
  
  
  int disable_split_mask;

  
  
  
  int adaptive_motion_search;

  
  
  
  
  int adaptive_pred_interp_filter;

  
  
  int partition_check;

  
  
  int force_frame_boost;

  
  int max_delta_qindex;

  
  
  
  unsigned int mode_search_skip_flags;

  
  unsigned int disable_split_var_thresh;

  
  
  unsigned int disable_filter_search_var_thresh;

  
  
  int intra_y_mode_mask[TX_SIZES];
  int intra_uv_mode_mask[TX_SIZES];

  
  
  
  int use_rd_breakout;

  
  
  
  int use_uv_intra_rd_estimate;

  
  LPF_PICK_METHOD lpf_pick;

  
  
  FAST_COEFF_UPDATE use_fast_coef_updates;

  
  int use_nonrd_pick_mode;

  
  
  int encode_breakout_thresh;

  
  
  int disable_inter_mode_mask[BLOCK_SIZES];

  
  
  int use_fast_coef_costing;

  
  
  int recode_tolerance;

  
  
  
  BLOCK_SIZE max_intra_bsize;

  
  
  int search_type_check_frequency;

  
  int source_var_thresh;
} SPEED_FEATURES;

struct VP9_COMP;

void vp9_set_speed_features(struct VP9_COMP *cpi);

#ifdef __cplusplus
}  
#endif

#endif  
