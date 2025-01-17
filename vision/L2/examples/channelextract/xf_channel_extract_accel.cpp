/*
 * Copyright 2022 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "xf_channel_extract_accel_config.h"
extern "C" {
void channel_extract_accel(
    ap_uint<INPUT_PTR_WIDTH>* img_rgba, ap_uint<OUTPUT_PTR_WIDTH>* img_gray, uint16_t channel, int rows, int cols) {
// clang-format off
    #pragma HLS INTERFACE m_axi     port=img_rgba  	offset=slave bundle=gmem1
    #pragma HLS INTERFACE m_axi     port=img_gray  	offset=slave bundle=gmem2
    #pragma HLS INTERFACE s_axilite port=rows              	 
    #pragma HLS INTERFACE s_axilite port=cols              	 
    #pragma HLS INTERFACE s_axilite port=channel              	 
    #pragma HLS INTERFACE s_axilite port=return
    // clang-format on

    xf::cv::Mat<IN_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_IN_0> imgInput0;
    imgInput0.rows = rows;
    imgInput0.cols = cols;
    xf::cv::Mat<OUT_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_OUT_0> imgOutput0;
    imgOutput0.rows = rows;
    imgOutput0.cols = cols;

// clang-format off
    #pragma HLS DATAFLOW
    // clang-format on
    xf::cv::Array2xfMat<INPUT_PTR_WIDTH, IN_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_IN_0>(img_rgba, imgInput0);
    xf::cv::extractChannel<IN_TYPE, OUT_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_IN_0, XF_CV_DEPTH_OUT_0>(
        imgInput0, imgOutput0, channel);
    xf::cv::xfMat2Array<OUTPUT_PTR_WIDTH, OUT_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_OUT_0>(imgOutput0, img_gray);
}
}
