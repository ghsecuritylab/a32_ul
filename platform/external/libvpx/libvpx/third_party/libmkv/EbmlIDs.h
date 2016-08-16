/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#ifndef MKV_DEFS_HPP
#define MKV_DEFS_HPP 1


enum mkv {
  EBML = 0x1A45DFA3,
  EBMLVersion = 0x4286,
  EBMLReadVersion = 0x42F7,
  EBMLMaxIDLength = 0x42F2,
  EBMLMaxSizeLength = 0x42F3,
  DocType = 0x4282,
  DocTypeVersion = 0x4287,
  DocTypeReadVersion = 0x4285,
  Void = 0xEC,
  SignatureSlot = 0x1B538667,
  SignatureAlgo = 0x7E8A,
  SignatureHash = 0x7E9A,
  SignaturePublicKey = 0x7EA5,
  Signature = 0x7EB5,
  SignatureElements = 0x7E5B,
  SignatureElementList = 0x7E7B,
  SignedElement = 0x6532,
  
  Segment = 0x18538067,
  
  SeekHead = 0x114D9B74,
  Seek = 0x4DBB,
  SeekID = 0x53AB,
  SeekPosition = 0x53AC,
  
  Info = 0x1549A966,
  TimecodeScale = 0x2AD7B1,
  Segment_Duration = 0x4489,
  DateUTC = 0x4461,
  MuxingApp = 0x4D80,
  WritingApp = 0x5741,
  
  Cluster = 0x1F43B675,
  Timecode = 0xE7,
  PrevSize = 0xAB,
  BlockGroup = 0xA0,
  Block = 0xA1,
  BlockAdditions = 0x75A1,
  BlockMore = 0xA6,
  BlockAddID = 0xEE,
  BlockAdditional = 0xA5,
  BlockDuration = 0x9B,
  ReferenceBlock = 0xFB,
  LaceNumber = 0xCC,
  SimpleBlock = 0xA3,
  
  Tracks = 0x1654AE6B,
  TrackEntry = 0xAE,
  TrackNumber = 0xD7,
  TrackUID = 0x73C5,
  TrackType = 0x83,
  FlagEnabled = 0xB9,
  FlagDefault = 0x88,
  FlagForced = 0x55AA,
  FlagLacing = 0x9C,
  DefaultDuration = 0x23E383,
  MaxBlockAdditionID = 0x55EE,
  Name = 0x536E,
  Language = 0x22B59C,
  CodecID = 0x86,
  CodecPrivate = 0x63A2,
  CodecName = 0x258688,
  
  Video = 0xE0,
  FlagInterlaced = 0x9A,
  StereoMode = 0x53B8,
  AlphaMode = 0x53C0,
  PixelWidth = 0xB0,
  PixelHeight = 0xBA,
  PixelCropBottom = 0x54AA,
  PixelCropTop = 0x54BB,
  PixelCropLeft = 0x54CC,
  PixelCropRight = 0x54DD,
  DisplayWidth = 0x54B0,
  DisplayHeight = 0x54BA,
  DisplayUnit = 0x54B2,
  AspectRatioType = 0x54B3,
  FrameRate = 0x2383E3,
  
  
  Audio = 0xE1,
  SamplingFrequency = 0xB5,
  OutputSamplingFrequency = 0x78B5,
  Channels = 0x9F,
  BitDepth = 0x6264,
  
  
  
  
  Cues = 0x1C53BB6B,
  CuePoint = 0xBB,
  CueTime = 0xB3,
  CueTrackPositions = 0xB7,
  CueTrack = 0xF7,
  CueClusterPosition = 0xF1,
  CueBlockNumber = 0x5378
  
  
  
};
#endif